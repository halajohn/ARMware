// ARMware - an ARM emulator
// Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

// The ARM core

#include <iostream>
#include <cassert>

#include "Core.hpp"
#include "ARMware.hpp"
#include "SysCoProc.hpp"
#include "RomFile.hpp"
#include "Exception.hpp"
#include "MemoryConf.hpp"
#include "PowerManager.hpp"
#include "ResetController.hpp"
#include "Mmu.hpp"
#include "Serial_1.hpp"
#include "Serial_2.hpp"
#include "Serial_3.hpp"
#include "Egpio.hpp"
#include "LCDController.hpp"
#include "OsTimer.hpp"
#include "platform_dep.hpp"

#if ENABLE_THREADED_CODE
#include "ChunkChain.hpp"
#endif

#include "Database/MachineImpl.hpp"

#include "Gtk/MachineScreen.hpp"
#include "Gtk/SoftKeyboard.hpp"
#include "Utils/Number.hpp"

#include "AddressSpace.tcc"
#include "Core.tcc"
#include "Mmu.tcc"
#include "Decode.tcc"

namespace ARMware
{
  // Global object pointer
  
  Core * gp_core;
  
  ////////////////////////////////// Public ///////////////////////////////////
  //============================== Life cycle =================================
  
  Core::Core(MachineImpl const &machineImpl,
             MachineScreen * const machine_screen,
             SoftKeyboard * const soft_keyboard,
             SerialConsole &serial_console)
    : mpSysCoProc(new SysCoProc(this)),
      mpEgpio(new Egpio(this)),
      mp_memory(new Memory(32 * 1024 * 1024, machine_screen)),
      
      mp_GPIO_control_block(new GPIOControlBlock()),
      mpOsTimer(new OsTimer()),
      mpRTC(new RTC()),
      mp_LCD_controller(new LCDController(this, mp_memory.get(), machine_screen)),
      
      mp_PPC(new PPC()),
      
      mp_serial_1(new Serial_1(this, mp_PPC.get(), machine_screen)),
      mp_serial_2(new Serial_2()),
      mp_serial_3(new Serial_3(serial_console)),
      
      mpIntController(new IntController(this,
                                        mp_GPIO_control_block.get(),
                                        mpOsTimer.get(),
                                        mpRTC.get(),
                                        mp_LCD_controller.get(),
                                        mp_serial_1.get(),
                                        mp_serial_2.get(),
                                        mp_serial_3.get())),
      
      mp_memory_conf(new MemoryConf()),
      mpPowerManager(new PowerManager()),
      mpResetController(new ResetController()),
      mpRomFile(new RomFile(this, mpEgpio.get(), machineImpl.get_rom_file())),
      mpAddressSpace(new AddressSpace(this, mpOsTimer.get())),
      mpMmu(new Mmu(this, mpSysCoProc.get(), mpAddressSpace.get())),
      m_idle_mode(false),
#if USE_SHIFTER_CARRY
      mShifterCarry(0),
#endif
#if ENABLE_THREADED_CODE
      m_chunk_start(false),
      m_chunk_length(0),
      m_chunk_disappear(false),
      m_device_timer_increment(1),
      mp_curr_chunk(0),
#if CHECK_CHUNK_CODE
      mp_delay_clean_chunk(0),
#endif
      mp_chunk_chain(new ChunkChain(this)),
#endif
      m_UART_clocks(0),
      m_3_6864_mhz_clocks(0),
      m_get_event_clocks(0),
      mp_machine_screen(machine_screen)
#if ENABLE_INST_RECALL
    , mInstRecallEntryIndex(0)
#endif
#if ENABLE_DYNAMIC_TRANSLATOR
    , m_all_native_code_buffer_size(0),
      m_all_compiled_arm_code_size(0),
      m_average_native_code_buffer_size(0)
#endif
#if PRINT_CHUNK_CODE
    , m_exec_in_chunk(false)
#endif
  {
    assert(GENERATE_THREADED_CODE_THRESHOLD > Chunk::INITIAL_HIT_COUNT);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    assert(GENERATE_DT_BUFFER_THRESHOLD > Chunk::INITIAL_HIT_COUNT);    
    assert(GENERATE_DT_BUFFER_THRESHOLD > GENERATE_THREADED_CODE_THRESHOLD);
#endif
    
#if ENABLE_INST_RECALL
    assert(true == power_of_two(INST_RECALL_BUFFER_SIZE));
#endif
    
#if ENABLE_THREADED_CODE
    mp_memory->register_chunk_chain(mp_chunk_chain.get());
    
#if PRINT_CHUNKS_LENGTH
    for (uint32_t i = 0; i < CL_TOTAL; ++i)
    {
      m_chunks_length[i] = 0;
    }
#endif
    
#endif // ENABLE_THREADED_CODE
    
    mp_machine_screen->register_memory(mp_memory->get_memory_buffer());
    mp_machine_screen->register_LCD_controller(mp_LCD_controller.get());
    
    soft_keyboard->register_GPIO_control_block(mp_GPIO_control_block.get());
    
    mpAddressSpace->RegisterIntController(mpIntController.get());
    mpAddressSpace->RegisterGPIOControlBlock(mp_GPIO_control_block.get());
    
    mpAddressSpace->RegisterSerial_1(mp_serial_1.get());
    mpAddressSpace->RegisterSerial_2(mp_serial_2.get());
    mpAddressSpace->RegisterSerial_3(mp_serial_3.get());
    
    mpAddressSpace->RegisterRTC(mpRTC.get());
    mpAddressSpace->RegisterPPC(mp_PPC.get());
    mpAddressSpace->RegisterEgpio(mpEgpio.get());
    mpAddressSpace->RegisterMemory(mp_memory.get());
    mpAddressSpace->RegisterMemoryConf(mp_memory_conf.get());
    mpAddressSpace->RegisterPowerManager(mpPowerManager.get());
    mpAddressSpace->RegisterResetController(mpResetController.get());
    mpAddressSpace->RegisterRomFile(mpRomFile.get());
    mpAddressSpace->RegisterLCDController(mp_LCD_controller.get());
    
    // :NOTE: Wei 2005-Mar-11:
    //
    // Because reset() will set the machine mode to SVC mode,
    // thus if I want change_reg_bank() to work as I expected,
    // I have to set 'm_curr_mode' to modes other than MODE_SVC,
    // and I choose MODE_USR here.
    m_curr_mode = MODE_USR;
    
#if CHECK_CHUNK_CODE
    // :NOTE: Wei 2005-Apr-08:
    //
    // Although the ARM-DDI-0100E doesn't specify the contents of other bits for the CPSR,
    // however, due to the simplicity of comparsion between the result of threaded codes
    // and that of dynamic compiled codes, I initialize m_CPSR to 0 when I turn on the CHECK_CHUNK_CODE.
    m_CPSR = 0;
    
    m_FIQ_SPSR = 0;
    m_SVC_SPSR = 0;
    m_ABT_SPSR = 0;
    m_IRQ_SPSR = 0;
    m_UND_SPSR = 0;
    
    for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
    {
      m_curr_reg_bank[i] = 0;
      
      m_USR_reg_bank[i] = 0;
      m_FIQ_reg_bank[i] = 0;
      m_IRQ_reg_bank[i] = 0;
      m_SVC_reg_bank[i] = 0;
      m_ABT_reg_bank[i] = 0;
      m_UND_reg_bank[i] = 0;
    }
#endif
    
    reset();
  }
  
  // :NOTE: Wei 2004-May-27:
  //
  // This definition makes Core::~Core() doesn't inline,
  // and this is what I want.
  //
  // Because if this destructor is inline,
  // then StartMachine.cpp has to include Egpio.hpp, IntController.hpp, ...
  //
  // This is because Core contains server std::~auto_ptr() objects,
  // and the destructor of std::auto_ptr is inline, and will call the underlying object's
  // desctructor.
  //
  // Thus if Core::~Core() is line, then I have to include Egpio.hpp, ...
  // in StartMachine.cpp to make that compilation unit knows the existences of Egpio::~Egpio(),
  // ...
  //
  // This is not what I want.
  Core::~Core()
  {
  }
  
  //================================ Operation ================================
  
#if ENABLE_INST_RECALL
  void
  Core::DumpInstRecallBuffer() const
  {
    // :NOTE: Wei 2004-Jun-30:
    //
    // The following statement is optimized from:
    //
    // ((mInstRecallEntryIndex + 1) % INST_RECALL_BUFFER_SIZE)
    //
    // Thus INST_RECALL_BUFFER_SIZE have to be power of 2.
    int const start_index = ((mInstRecallEntryIndex + 1) & (INST_RECALL_BUFFER_SIZE - 1));
    
    if (0 == start_index)
    {
      for (uint32_t i = 0; i < INST_RECALL_BUFFER_SIZE; ++i)
      {
        DumpInstRecallEntry(i);
      }
    }
    else
    {
      for (uint32_t i = start_index; i < INST_RECALL_BUFFER_SIZE; ++i)
      {
        DumpInstRecallEntry(i);
      }
      
      for (int i = 0; i < start_index; ++i)
      {
        DumpInstRecallEntry(i);
      }
    }
  }
#endif
    
  void
  Core::run()
  {
    while (1)
    {
      // :SA-1110 Developer's Manual: p.45: Wei 2004-Jan-09:
      //
      // The SA-1110 implements the ARM V4 architecture as defined in the ARM Architecture Reference.
      //
      // :NOTE: Wei 2004-Jun-09:
      //
      // That is to say, SA-1110 doesn't support Thumb instruction set.
      assert(0 == (m_CPSR & TBIT_MASK));
      
#if CHECK_PC_MODIFY
      m_pc_incremented = false;
      m_orig_pc = m_curr_reg_bank[CR_PC];
#endif
      
#if ENABLE_THREADED_CODE
      // :NOTE: Wei 2005-May-06:
      //
      // If I enable threaded code or hotspot compiler,
      // then 'm_device_timer_increment' may not be still 1.
      // However, in normal interpreter situation,
      // (Ex: the first execution of every chunks)
      // this variable should be 1 to increment timer
      // just 1 after every instruction.
      m_device_timer_increment = 1;
#endif
      
      if (false == m_idle_mode)
      {
#if CHECK_EXCEPTION_RAISE
        m_exception_type = EXCEPTION_NONE;
#endif
        
        ExecResultEnum const result = exec();
        
        switch (result)
        {
        case ER_EXCEPTION:
          switch (m_exception_type)
          {
          case EXCEPTION_UND:
            raise_UND_exception();
            break;
            
          case EXCEPTION_SWI:
            raise_SWI_exception();
            break;
            
          case EXCEPTION_PREFETCH:
            raise_prefetch_abort();
            break;
            
          case EXCEPTION_DATA:
            raise_data_abort();
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          break;
          
        case ER_NORMAL:
#if CHECK_EXCEPTION_RAISE
          assert(EXCEPTION_NONE == m_exception_type);
#endif
          break;
          
#if ENABLE_THREADED_CODE
        case ER_IO_IRQ_OCCUR:
          raise_IRQ_exception();
          continue;
          
        case ER_IO_FIQ_OCCUR:
          raise_FIQ_exception();          
          continue;
          
        case ER_CHUNK_DISAPPEAR:
        case ER_NORMAL_CONT_EXEC_IN_CHUNK:
        case ER_NORMAL_MODIFY_PC:
#endif
        default:
          assert(!"Should not reach here.");
          break;
        }
        
#if CHECK_PC_MODIFY
        if (true == m_pc_incremented)
        {
          assert(m_curr_reg_bank[CR_PC] == (m_orig_pc + 4));
        }
#endif
        
        (void)check_periphery<false, true>();
      }
      else
      {
        // :NOTE: Wei 2004-Sep-30:
        //
        // idle mode
        (void)check_periphery<true, true>();
      }
    }
  }
  
  void
  Core::reset()
  {
    // Enter Supervisor mode
    m_CPSR &= ~MODE_MASK;
    m_CPSR |= MODE_SVC;
    
    change_reg_bank(MODE_SVC);
    
    // Execute in ARM state
    m_CPSR &= ~TBIT_MASK;
    
    // Disable fast interrupts
    m_CPSR |= FBIT_MASK;
    
    // Disable normal interrupts
    m_CPSR |= IBIT_MASK;
    
    mpSysCoProc->reset();
    
#if CONFIG_CPU_SA1110
    // :SA-1110 Developer's Manual: Wei 2004-Jan-08:
    //
    // The vector adjust cannot modify the reset vector.
    m_curr_reg_bank[CR_PC] = 0x00000000;
#else
    if (true == mpSysCoProc->use_high_vector())
    {
      m_curr_reg_bank[CR_PC] = 0xFFFF0000;
    }
    else
    {
      m_curr_reg_bank[CR_PC] = 0x00000000;
    }
#endif
    
    mp_GPIO_control_block->reset();
    mpRTC->reset();
    mpIntController->reset();
    mp_memory_conf->reset();
    mpPowerManager->reset();
    mpResetController->reset();
    mp_PPC->reset();
    mp_LCD_controller->reset();
    mpRomFile->reset();
    mpEgpio->reset();
    mpOsTimer->reset();
    
    mp_serial_1->reset();
    mp_serial_2->reset();
    mp_serial_3->reset();
    
    m_RS232_is_enabled = false;
  }
  
  void
  Core::raise_UND_exception()
  {
    // :ARM-DDI-0100E: p.A2-15: Wei 2004-Apr-26:
    //
    // R14_und = address of next instruction after the undefined instruction
    //
    // :NOTE: Wei 2005-Mar-11:
    //
    // When ARMware is in the UND mode,
    // then the real R14_und register will be m_curr_reg_bank[CR_14],
    // rather than m_UND_reg_bank[CR_14].
    if (MODE_UND == m_curr_mode)
    {
      m_curr_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_curr_SPSR = m_CPSR;
    }
    else
    {
      m_UND_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_UND_SPSR = m_CPSR;
    }
    
    m_CPSR &= ~MODE_MASK;
    m_CPSR |= MODE_UND;
    
    change_reg_bank(MODE_UND);
    
    // Execute in ARM state
    m_CPSR &= ~TBIT_MASK;
    
    // Disable normal interrupts
    m_CPSR |= IBIT_MASK;
    
    if (true == mpSysCoProc->use_high_vector())
    {
      m_curr_reg_bank[CR_PC] = 0xFFFF0004;
    }
    else
    {
      m_curr_reg_bank[CR_PC] = 0x00000004;
    }
  }
  
  void
  Core::raise_SWI_exception()
  {
    // :ARM-DDI-0100E: p.A2-16: Wei 2004-Apr-26:
    //
    // R14_svc = address of next instruction after the SWI instruction
    if (MODE_SVC == m_curr_mode)
    {
      m_curr_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_curr_SPSR = m_CPSR;
    }
    else
    {
      m_SVC_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_SVC_SPSR = m_CPSR;
    }
    
    m_CPSR &= ~MODE_MASK;
    m_CPSR |= MODE_SVC;
    
    change_reg_bank(MODE_SVC);
    
    // Execute in ARM state
    m_CPSR &= ~TBIT_MASK;
    
    // Disable normal interrupts
    m_CPSR |= IBIT_MASK;
    
    if (true == mpSysCoProc->use_high_vector())
    {
      m_curr_reg_bank[CR_PC] = 0xFFFF0008;
    }
    else
    {
      m_curr_reg_bank[CR_PC] = 0x00000008;
    }
  }
  
  void
  Core::raise_IRQ_exception()
  {
    // :ARM-DDI-0100E: p.A2-19: Wei 2004-Apr-26:
    //
    // R14_irq = address of next instruction to be executed + 4
    if (MODE_IRQ == m_curr_mode)
    {
      m_curr_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_curr_SPSR = m_CPSR;
    }
    else
    {
      m_IRQ_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_IRQ_SPSR = m_CPSR;
    }
    
    m_CPSR &= ~MODE_MASK;
    m_CPSR |= MODE_IRQ;
    
    change_reg_bank(MODE_IRQ);
    
    // Execute in ARM state
    m_CPSR &= ~TBIT_MASK;
    
    // Disable normal interrupts
    m_CPSR |= IBIT_MASK;
    
    if (true == mpSysCoProc->use_high_vector())
    {
      m_curr_reg_bank[CR_PC] = 0xFFFF0018;
    }
    else
    {
      m_curr_reg_bank[CR_PC] = 0x00000018;
    }
  }
  
  void
  Core::raise_FIQ_exception()
  {
    // :ARM-DDI-0100E: p.A2-20: Wei 2004-Apr-26:
    //
    // R14_fiq = address of next instruction to be executed + 4
    //
    // :NOTE: Wei 2004-Apr-26:
    //
    // Because I check the occurence of the IRQ and FIQ after the incrementing
    // of the PC in each instruction implemantation.
    // (i.e. core->IncrePC() in every instruction implementation)
    // Thus if I enter these 2 functions (raise_IRQ_exception() or raise_FIQ_exception()),
    // then m_curr_reg_bank[CR_PC] will contain the address of next instruction to be
    // executed.
    if (MODE_FIQ == m_curr_mode)
    {
      m_curr_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_curr_SPSR = m_CPSR;
    }
    else
    {
      m_FIQ_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_FIQ_SPSR = m_CPSR;
    }
    
    m_CPSR &= ~MODE_MASK;
    m_CPSR |= MODE_FIQ;
    
    change_reg_bank(MODE_FIQ);
    
    // Execute in ARM state
    m_CPSR &= ~TBIT_MASK;
    
    // Disable fast interrupts
    m_CPSR |= FBIT_MASK;
    
    // Disable normal interrupts
    m_CPSR |= IBIT_MASK;
    
    if (true == mpSysCoProc->use_high_vector())
    {
      m_curr_reg_bank[CR_PC] = 0xFFFF001C;
    }
    else
    {
      m_curr_reg_bank[CR_PC] = 0x0000001C;
    }
  }
  
  void
  Core::raise_prefetch_abort()
  {
    // :ARM-DDI-0100E: p.A2-16: Wei 2004-Apr-26:
    //
    // R14_abt = address of the aborted instruction + 4
    if (MODE_ABT == m_curr_mode)
    {
      m_curr_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_curr_SPSR = m_CPSR;
    }
    else
    {
      m_ABT_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 4;
      m_ABT_SPSR = m_CPSR;
    }
    
    m_CPSR &= ~MODE_MASK;
    m_CPSR |= MODE_ABT;
    
    change_reg_bank(MODE_ABT);
    
    // Execute in ARM state
    m_CPSR &= ~TBIT_MASK;
    
    // Disable normal interrupts
    m_CPSR |= IBIT_MASK;
    
    if (true == mpSysCoProc->use_high_vector())
    {
      m_curr_reg_bank[CR_PC] = 0xFFFF000C;
    }
    else
    {
      m_curr_reg_bank[CR_PC] = 0x0000000C;
    }
  }
  
  void
  Core::raise_data_abort()
  {
    // :ARM-DDI-0100E: p.A2-17: Wei 2004-Apr-26:
    //
    // R14_abt = address of the aborted instruction + 8
    if (MODE_ABT == m_curr_mode)
    {
      m_curr_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 8;
      m_curr_SPSR = m_CPSR;
    }
    else
    {
      m_ABT_reg_bank[CR_14] = m_curr_reg_bank[CR_PC] + 8;
      m_ABT_SPSR = m_CPSR;
    }
    
    m_CPSR &= ~MODE_MASK;
    m_CPSR |= MODE_ABT;
    
    change_reg_bank(MODE_ABT);
    
    // Execute in ARM state
    m_CPSR &= ~TBIT_MASK;
    
    // Disable normal interrupts
    m_CPSR |= IBIT_MASK;
    
    if (true == mpSysCoProc->use_high_vector())
    {
      m_curr_reg_bank[CR_PC] = 0xFFFF0010;
    }
    else
    {
      m_curr_reg_bank[CR_PC] = 0x00000010;
    }
  }
    
  inline PassCondResultEnum
  Core::pass_cond(Inst const inst)
  {
    switch (inst.mRawData & 0xF0000000)
    {
    case EQ: // Equal
      return pass_cond_eq(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case NE: // Not equal
      return pass_cond_ne(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case CS: // Carry set/unsigned higher or same
      return pass_cond_cs(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case CC: // Carry clear/unsigned lower
      return pass_cond_cc(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case MI: // Minus/Negative
      return pass_cond_mi(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case PL: // Plus/Positive
      return pass_cond_pl(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case VS: // Overflow
      return pass_cond_vs(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case VC: // No overflow
      return pass_cond_vc(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case HI: // unsigned higher
      return pass_cond_hi(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case LS: // unsigned lower or same
      return pass_cond_ls(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case GE: // Signed greater than or equal
      return pass_cond_ge(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case LT: // Signed less than
      return pass_cond_lt(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case GT: // Signed greater than
      return pass_cond_gt(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case LE: // Signed less than or equal
      return pass_cond_le(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case AL: // Always
      return pass_cond_al(
#if CONFIG_ARCH_V5
        inst
#endif 
        );
      
    case NV:
      return pass_cond_nv(
#if CONFIG_ARCH_V5
        inst
#endif
        );
      
    default:
      assert(!"Should not reach here.");
      return PASS_FALSE;
    }
  }
  
  ////////////////////////////////// Private //////////////////////////////////
  //================================ Operation ================================
  
#if ENABLE_INST_RECALL
  void
  Core::DumpInstRecallEntry(uint32_t const index) const
  {
    InstRecallEntry const &entry = mInstRecallBuffer[index];
    
    g_log_file << std::hex
               << entry.mIpVaddr << " "
               << entry.mIpPaddr << ": "
               << entry.mInst << ": ";
    
    if (false == entry.mExec)
    {
      g_log_file << "no_exec";
    }
    else
    {
      g_log_file << InstName(entry.mInstNameTag);
      
      switch (entry.mInstNameTag)
      {
      case INST_AND:
      case INST_EOR:
      case INST_SUB:
      case INST_RSB:
      case INST_ADD:
      case INST_ADC:
      case INST_SBC:
      case INST_RSC:
      case INST_TST:
      case INST_TEQ:
      case INST_CMP:
      case INST_CMN:
      case INST_ORR:
      case INST_MOV:
      case INST_BIC:
      case INST_MVN:
        // DPI
        {
          g_log_file << std::hex
                     << ((true == entry.mUpdateStatusRegister) ? "S, " : ", ")
                     << InstType(entry.mInstTypeTag) << ", "
                     << "Rd: " << CoreRegName(entry.mRdNum) << " " << entry.mRd << ", "
                     << "Rn: " << CoreRegName(entry.mRnNum) << " " << entry.mRn << ", ";
          
          switch (entry.mInstTypeTag)
          {
          case IMM:
            break;
            
          case REG_IMM_SFT:
            g_log_file << std::hex << "Rm: " << CoreRegName(entry.mRmNum) << " " << entry.mRm << ", ";
            break;
            
          case REG_REG_SFT:
            g_log_file << std::hex
                      << "Rm: " << CoreRegName(entry.mRmNum) << " " << entry.mRm << ", "
                      << "Rs: " << CoreRegName(entry.mRsNum) << " " << entry.mRs << ", ";
            break;
          }
          
          g_log_file << std::hex << "sft_operand = " << entry.mSftOperand << " ";
        }
        break;
        
      case INST_SMULL:
      case INST_SMLAL:
      case INST_UMULL:
      case INST_UMLAL:
      case INST_MUL:
      case INST_MLA:
        // Multi
        g_log_file << ((true == entry.mUpdateStatusRegister) ? "S, " : ", ");
        break;
        
      case INST_BRANCH:
        g_log_file << ", "
                  << std::hex << "Rd = " << entry.mRd << " ";
        break;
        
      case INST_LDR:
      case INST_LDRB:
      case INST_LDRH:
      case INST_STR:
      case INST_STRB:
      case INST_STRH:
        g_log_file << ", "
                  << std::hex
                  << "Rd: " << CoreRegName(entry.mRdNum) << " " << entry.mRd << ", "
                  << "Vaddr = " << entry.mVaddr[entry.mRdNum] << ", "
                  << "Paddr = " << entry.mPaddr[entry.mRdNum] << ", ";
        break;
        
      case INST_LDM_1:
      case INST_LDM_2:
      case INST_LDM_3:
      case INST_STM_1:
      case INST_STM_2:
        for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
        {
          if (true == entry.mEnable[i])
          {
            g_log_file << std::endl
                      << CoreRegName(i) << " "
                      << std::hex
                      << "Vaddr = " << entry.mVaddr[i] << " "
                      << "Paddr = " << entry.mPaddr[i] << " "
                      << "Value = " << entry.mValue[i] << " ";
          }
        }
        break;
        
      default:
        break;
      }
    }
    
    g_log_file << std::endl;
  }
#endif
  
#if ENABLE_THREADED_CODE
  inline void
  Core::gen_threaded_code()
  {
#if ASSUME_NOT_USE_TINY_PAGE
    assert(mp_curr_chunk->length() < SIZE_4K);
#else
    assert(mp_curr_chunk->length() < SIZE_1K);
#endif
    
    uint32_t chunk_length = mp_curr_chunk->length();
    uint32_t paddr = mp_curr_chunk->start_paddr();
    
    mp_curr_chunk->alloc_tc_buffer();
    
    do
    {
      Inst inst(mp_memory->get_data<DataWidthTraits<WORD>::UnsignedType>(paddr));
      
      (void)decode<USAGE_THREADED_CODE>(inst);
      
      paddr += 4;
    }
    while (--chunk_length > 0);
    
    // :NOTE: Wei 2004-Aug-11:
    //
    // Because of simplifying the detection of self-modified codes,
    // I only generate threaded codes for the codes reside in the emulated dynamic memory.
    // 
    // That is to say, if the codes reside in other places, like emulated ROM,
    // I just left them untranslated, and executes them originally no matter how many times
    // they executed.
    //
    // (In H3600, the emulated dynamic memory resides in 0xC0000000 ~ 0xC1FFFFFF.
    //  However, the 'paddr' field of basic block is the address in the dynamic memory,
    //  thus, its range of 'paddr' of basic block is 0x0 ~ 0x1FFFFFF.
    assert(0 == (mp_curr_chunk->start_paddr() & 0xFE000000));
    
    // :NOTE: Wei 2004-Aug-11:
    //
    // Make the physical memory page non-writable.
    //
    // According to many documents I found using Google,
    // they state that if I want mprotect() to execute successfully,
    // the memory address has to be page-aligned, and the memory size has to be page-aligned, too.
#if CHECK_MPROTECT
    if (
#endif
      memory_protect(page_aligned_addr(mp_memory->get_memory_native_addr(mp_curr_chunk->start_paddr())),
                     page_aligned_size(mp_curr_chunk->length()))
#if CHECK_MPROTECT
      != 0)
    {
      perror("Fail to mprotect()");
      assert(!"Should not reach here.");
    }
#else
    ;
#endif
    
    mp_curr_chunk->set_status(Chunk::ST_THREADED_CODE);
  }
  
  inline ExecResultEnum
  Core::exec_threaded_code()
  {
    // :NOTE: Wei 2004-Oct-01:
    //
    // Because I want to implement when ARMware runs into a branch,
    // in case the branch target is still in this chunk, then ARMware
    // should NOT go back to the interpreter.
    //
    // Hence, I have to ask the 'mp_curr_chunk->m_arm_idx' field is synchronized
    // with the current execuating instruction.
    
    ARMInstInfo *arm_inst_info = mp_curr_chunk->first_arm_inst_info();
    
    mp_curr_chunk->reset_arm_idx();
    
#if CHECK_THREADED_CODE_PC
    uint32_t prev_pc = m_curr_reg_bank[CR_PC];
    
    bool prepare_cross_boundry = false;
#endif
    
    m_device_timer_increment = 0;
    
    // :NOTE: Wei 2004-Aug-02:
    //
    // execute threaded code
    do
    {
#if CHECK_THREADED_CODE_PC
      // :NOTE: Wei 2004-Aug-18:
      //
      // In the end of this 'do loop', if ARMware decide to run the loop again,
      // then 'prev_pc + 4' have to be equal to the 'm_curr_reg_bank[CR_PC].
      // Thus I will test the equivalent of 'm_curr_reg_bank[CR_PC]' and the updatted
      // 'prev_pc' to ensure that every instruction ARMware executes in the threaded
      // code buffer is successive.
      assert(m_curr_reg_bank[CR_PC] == prev_pc);
      
      bool exception_occur = false;
      
      // :NOTE: Wei 2004-Aug-18:
      //
      // I only cache the instructions located in DRAM.
      uint32_t const paddr = virtual_to_physical<INST, READ>(m_curr_reg_bank[CR_PC], m_curr_mode, exception_occur);
      
      assert(false == exception_occur);
      
      assert(paddr >= MachineType::DRAM_BASE);
      assert(paddr <= (MachineType::DRAM_BASE + MachineType::DRAM_SIZE));
      
      // :NOTE: Wei 2004-Aug-18:
      //
      // If 'prepare_cross_boundry == true', then I should not execute any more instructions.
      assert(false == prepare_cross_boundry);
      
#if ASSUME_NOT_USE_TINY_PAGE
      if ((SIZE_4K - 4) == (paddr & (SIZE_4K - 1)))
#else
      if ((SIZE_1K - 4) == (paddr & (SIZE_1K - 1)))
#endif
      {
        prepare_cross_boundry = true;
        
        // :NOTE: Wei 2004-Aug-18:
        //
        // If this instruction is located just before the 1-kb page boundry,
        // then this instruction should be the last one of a basic block.
        assert(mp_curr_chunk->inst_idx() == mp_curr_chunk->length() - 1);
        break;
      }
      
      // :NOTE: Wei 2004-Aug-18:
      //
      // Check to see the instruction located in DRAM now is identical to the instruction
      // cached in the threaded code buffer.
      assert((arm_inst_info->inst()).mRawData ==
             mp_memory->get_data<DataWidthTraits<WORD>::UnsignedType>(paddr - MachineType::DRAM_BASE));
#endif
      
      switch (pass_cond(arm_inst_info->inst()))
      {
      case PASS_TRUE:
        {
          ++m_device_timer_increment;
          
          // Execute the instruction.
          ExecResultEnum const result = (arm_inst_info->func_TC())(arm_inst_info->inst());
          
          switch (result)
          {
          case ER_NORMAL:
            break;
            
          case ER_NORMAL_CONT_EXEC_IN_CHUNK:
            arm_inst_info = mp_curr_chunk->curr_arm_inst_info();
            
#if CHECK_THREADED_CODE_PC
            prev_pc = m_curr_reg_bank[CR_PC];
#endif
            
            // :NOTE: Wei 2004-Oct-01:
            //
            // Because Core::check_periphery() will add 'm_device_timer_increment' to some
            // field. Hence, if 'result == ER_NORMAL_CONT_EXEC_IN_CHUNK', that is to say
            // ARMware has executed Core::check_periphery() in a branch instruction
            // just a moment ago. Thus, I have to reset 'm_device_timer_increment' to 0
            // to reflect this fact.
            m_device_timer_increment = 0;
            continue;
            
          case ER_IO_IRQ_OCCUR:
          case ER_IO_FIQ_OCCUR:
            reset_chunk_after_tc_or_dt();
            return result;
            
          case ER_NORMAL_MODIFY_PC:
            goto done;
            
          case ER_EXCEPTION:
#if CHECK_THREADED_CODE_PC
            // :NOTE: Wei 2004-Aug-16:
            //
            // This is because an exception has occured,
            // thus 'm_curr_reg_bank[CR_PC]' and 'prev_pc' should be the same.
            
            assert(m_curr_reg_bank[CR_PC] == prev_pc);
#endif
            
            // :NOTE: Wei 2004-Aug-02:
            //
            // Finalize current basic block, in order to trigger the next basic block searching.
            reset_chunk_after_tc_or_dt();
            
#if CHECK_PC_MODIFY
            // :NOTE: Wei 2004-Aug-16:
            //
            // When I return from a threaded code buffer, it must be the following conditions:
            //  1) finish this threaded code buffer.
            //  2) an exception occurs.
            //
            // No matter what condition is,
            // 'm_curr_reg_bank[CR_PC]' shall not be the next instruction of 'm_orig_pc',
            // thus I should set 'm_pc_incremented' to false.
            m_pc_incremented = false;
#endif
            return ER_EXCEPTION;
            
          case ER_CHUNK_DISAPPEAR:
#if CHECK_THREADED_CODE_PC
            // :NOTE: Wei 2004-Aug-16:
            //
            // Because this instruction is finished, thus 'm_curr_reg_bank[CR_PC]' should be equal
            // to 'prev_pc + 4'.
            
            assert(m_curr_reg_bank[CR_PC] == (prev_pc + 4));
#endif
            
            assert(mp_delay_clean_chunk != 0);
            
            // :NOTE: Wei 2005-May-17:
            //
            // Here, I can safely clean up the previous chunk.
            delete mp_delay_clean_chunk;
            
#if CHECK_CHUNK_CODE
            mp_delay_clean_chunk = 0;
#endif
            
            reset_chunk_after_tc_or_dt();
            
#if CHECK_PC_MODIFY
            m_pc_incremented = false;
#endif
            return ER_NORMAL;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
        break;
        
      case PASS_FALSE:
        IncrePC();
        break;
        
#if CONFIG_ARCH_V5
      case PASS_EXCEPTION:
        m_exception_type = EXCEPTION_UND;
        
#if CHECK_THREADED_CODE_PC
        // :NOTE: Wei 2004-Aug-16:
        //
        // This is because an exception has occured,
        // thus 'm_curr_reg_bank[CR_PC]' and 'prev_pc' should be the same.
        
        assert(m_curr_reg_bank[CR_PC] == prev_pc);
#endif
        
        // :NOTE: Wei 2004-Aug-02:
        //
        // Finalize current basic block, in order to trigger the next basic block searching.
        reset_chunk_after_tc_or_dt();
        
#if CHECK_PC_MODIFY
        m_pc_incremented = false;
#endif
        
        // :NOTE: Wei 2004-Aug-11:
        //
        // @false: means an exception has occur.
        return ER_EXCEPTION;
#endif
      }
      
#if CHECK_THREADED_CODE_PC
      // :NOTE: Wei 2004-Aug-17:
      //
      // I will close a basic block while ARMware executes an instruction modifying PC.
      //
      // However, some of the instructions included in a basic block may have possibilities
      // to modify PC, because that instruction didn't pass the condition test in its previous
      // run.
      //
      // However, that instruction didn't pass the condition test in its previous run doesn't
      // mean it will not pass the condition test this time.
      //
      // Thus, if it pass the condition test this time, and does modify PC,
      // then I should finish this basic block and return to the interpreter.
      assert(m_curr_reg_bank[CR_PC] == (prev_pc + 4));
      
      prev_pc = m_curr_reg_bank[CR_PC];
#endif
      
      // :NOTE: Wei 2004-Oct-01:
      //
      // Because I want to implement when ARMware runs into a branch,
      // in case the branch target is still in this chunk, then ARMware
      // should NOT go back to the interpreter.
      //
      // Hence, I have to ask the 'mp_curr_chunk->m_arm_idx' field is synchronized
      // with the current execuating instruction.
      
      ++arm_inst_info;
      
      mp_curr_chunk->increment_arm_idx();
    }
    while (mp_curr_chunk->inst_idx() != mp_curr_chunk->length());
    
  done:
    
    // :NOTE: Wei 2004-Aug-16:
    //
    // Finalize current basic block, in order to trigger the next basic block searching.
    reset_chunk_after_tc_or_dt();
    
    // :NOTE: Wei 2004-Aug-16:
    //
    // If we execute here, this means an basic block has
    // been fully executed, and every basic block is end by
    // jumpping to a non-successive PC value. thus
    // 'm_curr_reg_bank[CR_PC]' and 'prev_pc' should not
    // have the following relationship like above.
    //
    // assert(m_curr_reg_bank[CR_PC] == (prev_pc + 4));
    
#if CHECK_PC_MODIFY
    m_pc_incremented = false;
#endif
    
    return ER_NORMAL;
  }
  
#if PRINT_CHUNKS_LENGTH
  void
  Core::int_handler(int /* signum */)
  {
    g_log_file << "  0-  9: " << m_chunks_length[CL_0] << std::endl;
    g_log_file << " 10- 19: " << m_chunks_length[CL_10] << std::endl;
    g_log_file << " 20- 29: " << m_chunks_length[CL_20] << std::endl;
    g_log_file << " 30- 39: " << m_chunks_length[CL_30] << std::endl;
    g_log_file << " 40- 49: " << m_chunks_length[CL_40] << std::endl;
    g_log_file << " 50- 59: " << m_chunks_length[CL_50] << std::endl;
    g_log_file << " 60- 69: " << m_chunks_length[CL_60] << std::endl;
    g_log_file << " 70- 79: " << m_chunks_length[CL_70] << std::endl;
    g_log_file << " 80- 89: " << m_chunks_length[CL_80] << std::endl;
    g_log_file << " 90- 99: " << m_chunks_length[CL_90] << std::endl;
    g_log_file << "100-109: " << m_chunks_length[CL_100] << std::endl;
    g_log_file << "110-119: " << m_chunks_length[CL_110] << std::endl;
    g_log_file << "120-129: " << m_chunks_length[CL_120] << std::endl;
    g_log_file << "130-139: " << m_chunks_length[CL_130] << std::endl;
    g_log_file << "140-149: " << m_chunks_length[CL_140] << std::endl;
    g_log_file << "150-159: " << m_chunks_length[CL_150] << std::endl;
    g_log_file << "160-169: " << m_chunks_length[CL_160] << std::endl;
    g_log_file << "170-179: " << m_chunks_length[CL_170] << std::endl;
    g_log_file << "180-189: " << m_chunks_length[CL_180] << std::endl;
    g_log_file << "190-199: " << m_chunks_length[CL_190] << std::endl;
    g_log_file << ">200:    " << m_chunks_length[CL_OTHER] << std::endl;
    
    exit(-1);
  }
#endif
  
#endif // #if ENABLE_THREADED_CODE
  
#if CHECK_CHUNK_CODE
  uint32_t
  Core::get_curr_inst() const
  {
    bool exception_occur = false;
    uint32_t addr = m_curr_reg_bank[CR_PC];
    
    uint32_t const paddr = virtual_to_physical<INST, READ>(addr,
                                                           m_curr_mode,
                                                           exception_occur);
    
    assert(false == exception_occur);
    
    uint32_t const value = mpAddressSpace->get_data<uint32_t>(paddr, exception_occur);
    
    assert(false == exception_occur);
    
    return value;
  }
#endif
  
#if ENABLE_DYNAMIC_TRANSLATOR
  inline ExecResultEnum
  Core::exec_dt_buffer()
  {
#if PRINT_CHUNK_CODE
    g_log_file << "--> 0x"
               << std::hex
               << mp_curr_chunk->start_paddr()
               << std::endl;
#endif
    
#if 0
    
#if PRINT_CHUNK_CODE
    m_exec_in_chunk = true;
#endif
    
    ExecResultEnum const result = exec_threaded_code();
    
    // :NOTE: Wei 2005-Mar-11:
    //
    // When ARMware retores the CPSR value from one SPSR,
    // it must be the DPI instructions with PC as its destination register
    // or the MSR instruction.
    // And the native code the dynamic compiler generated will return to the
    // interpreter immediately when it sees a DPI instruction with PC as its
    // destination register, or preform a function call to accomplish the MSR
    // instruction. In the first situation, a DPI instruction with PC as its
    // destination register, all operations in the native codes are preform
    // on the previous mode, thus I can change the ARM core register bank
    // after the control has returned to the interpreter.
    //
    // In the other situation, a MSR instruction, all operations before the MSR
    // instruction are preformed on the previous mode, and the LIR CALL instrucion
    // will store all global variables into the proper places. When the control
    // returns to the native code, the core register bank has already changed to
    // the new mode register bank, and the native codes will reload the value from
    // the new register when it needs that value, thus the native code will use
    // the new and correct value.
    //
    // The following 'change_reg_bank()' function call is for the above situation 1.
    change_reg_bank(static_cast<CoreMode>(m_CPSR & MODE_MASK));
    
#if DUMP_CPU_INFO
    dump_cpu_info<false>();
    
    m_exec_in_chunk = false;
#endif
    
    return result;
#else
    ExecResultEnum result;    
    
    m_device_timer_increment = 0;
    
    void const * const addr = mp_curr_chunk->native_code_begin();
    
#if PRINT_CHUNK_CODE
    m_exec_in_chunk = true;
#endif
    
    // :NOTE: Wei 2005-Mar-28:
    //
    // The reason why I add a * in the CALL instruction is because:
    //
    // AT&T absolute (as opposed to PC relative) jump/call operands are prefixed by `*';
    //
    // I have to include EAX, ECX, EDX into the clobber list,
    // because these 3 registers are caller saved.
    // ESI, EDI, EBX are callee saved.
    asm volatile ("call *%1\n\t"
                  "movl %%eax, %0\n\t"
                  : "=g" (result)
                  : "r" (addr)
                  : "%eax", "%ecx", "%edx", "cc", "memory");
    
    change_reg_bank(static_cast<CoreMode>(m_CPSR & MODE_MASK));
    
#if DUMP_CPU_INFO
    dump_cpu_info<false>();
    
    m_exec_in_chunk = false;
#endif
    
    reset_chunk_after_tc_or_dt();
    
#if CHECK_PC_MODIFY
    // :NOTE: Wei 2005-Mar-31:
    //
    // I don't implement the PC checking facility for the dt code.
    m_pc_incremented = false;
#endif
    
    switch (result)
    {
    case ER_EXCEPTION:
      // :NOTE: Wei 2004-Nov-4:
      //
      // The only exception a dynamic translated codes can throw is
      // a data abort.
      assert((EXCEPTION_DATA == m_exception_type) ||
             (EXCEPTION_UND == m_exception_type) ||
             (EXCEPTION_SWI == m_exception_type));
      
    case ER_IO_IRQ_OCCUR:
    case ER_IO_FIQ_OCCUR:
      return result;
      
    case ER_CHUNK_DISAPPEAR:
      assert(mp_delay_clean_chunk != 0);
      
      // :NOTE: Wei 2005-May-17:
      //
      // Here, I can safely clean up the previous chunk.
      delete mp_delay_clean_chunk;
      
#if CHECK_CHUNK_CODE
      mp_delay_clean_chunk = 0;
#endif
      
    case ER_NORMAL:
      // :NOTE: Wei 2004-Oct-04:
      //
      // If this chunk is ended by the normal situation -
      // execuating all instructions in this chunk.
      //
      // Then this chunk will return 'ER_NORMAL'.
    case ER_NORMAL_MODIFY_PC:
      return ER_NORMAL;
      
    case ER_NORMAL_CONT_EXEC_IN_CHUNK:
    default:
      assert(!"Should not reach here.");
      return ER_NORMAL;
    }
#endif
  }
  
  void *
  Core::get_global_var_mem_addr(Variable::GlobalVarEnum const kind)
  {
    switch (kind)
    {
    case Variable::CR_0:
    case Variable::CR_1:
    case Variable::CR_2:
    case Variable::CR_3:
    case Variable::CR_4:
    case Variable::CR_5:
    case Variable::CR_6:
    case Variable::CR_7:
    case Variable::CR_8:
    case Variable::CR_9:
    case Variable::CR_10:
    case Variable::CR_11:
    case Variable::CR_12:
    case Variable::CR_13:
    case Variable::CR_14:
    case Variable::CR_15:
      return &m_curr_reg_bank[static_cast<CoreRegNum>(kind)];
      
    case Variable::CPSR:
      return &m_CPSR;
      
    case Variable::SPSR:
      return &m_curr_SPSR;
      
    case Variable::EXCEPTION_TYPE:
      return &m_exception_type;
      
    case Variable::DEVICE_TIMER_INCREMENT:
      return &m_device_timer_increment;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
#endif
  
  // :NOTE: Wei 2004-Jul-27:
  //
  // @false: an exception has occur.
  ExecResultEnum
  Core::exec()
  {
    bool exception_occur = false;
    
#if ENABLE_INST_RECALL
    NewInstRecallEntry().mIpVaddr = m_curr_reg_bank[CR_PC];
#endif
    
    // :ARM-DDI-0100E: p.A4-11: Wei 2004-May-06:
    //
    // ... because all ARM instructions are word-aligned.
    assert(0 == (m_curr_reg_bank[CR_PC] & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK));
    // :ARM-DDI-0100E: p.B3-21: Wei 2004-May-14:
    //
    // Alignment faults are not generated on any instruction fetch.
    
    // :NOTE: Wei 2004-Jul-30:
    //
    // In the original throw exception version, if get_data() throws an exception,
    // 'inst' will not be modified.
    //
    // However, because of 'inst' is a local variable,
    // thus I choose overwrite its value whether an exception occurs or not.
    Inst inst(get_data<WORD, INST>(m_curr_reg_bank[CR_PC], m_curr_mode, exception_occur));
    
    if (true == exception_occur)
    {
#if ENABLE_THREADED_CODE || ENABLE_DYNAMIC_TRANSLATOR
      // :NOTE: Wei 2004-Aug-14:
      //
      // An exception has occur, that means we have to close our current basic block (if we have
      // one), and we need to reset 'mp_curr_chunk' to trigger the beginning of the next basic block.
      finalize_chunk();
#endif
      m_exception_type = EXCEPTION_PREFETCH;
      
      return ER_EXCEPTION;
    }
    
#if ENABLE_THREADED_CODE || ENABLE_DYNAMIC_TRANSLATOR
    if (true == m_chunk_start)
    {
      // :NOTE: Wei 2004-Aug-14:
      //
      // Here, we are passing the upper getting instruction operation, and no exceptions occur,
      // and if 'm_chunk_start == true', this means that we are starting a basic block,
      // thus we will try to find a threaded code buffer corresponding to the current
      // basic block.
      
      assert(mp_curr_chunk != 0);
      
      switch (mp_curr_chunk->status())
      {
      case Chunk::ST_NONE:
        // :NOTE: Wei 2004-Aug-11:
        //
        // We don't have a threaded code buffer yet,
        // see whether we need to generate it right now or not.
        if (mp_curr_chunk->hit_count() >= GENERATE_THREADED_CODE_THRESHOLD)
        {
          // :NOTE: Wei 2004-Aug-05:
          //
          // Generate threaded code buffer
          gen_threaded_code();
          
          mp_curr_chunk->increase_hit_count();
          
          // :NOTE: Wei 2004-Aug-11:
          //
          // Executes the generating threaded code buffer, and return to run() when we finish
          // executing this basic block.
          return exec_threaded_code();
        }
        break;
        
      case Chunk::ST_THREADED_CODE:
#if ENABLE_DYNAMIC_TRANSLATOR
        if (mp_curr_chunk->hit_count() >= GENERATE_DT_BUFFER_THRESHOLD)
        {
          mp_curr_chunk->gen_dt_code();
          
          return exec_dt_buffer();
        }
        else
        {
#endif
          assert(mp_curr_chunk->tc_buffer() != 0);
          
          mp_curr_chunk->increase_hit_count();
          
          // :NOTE: Wei 2004-Aug-14:
          //
          // We find a threaded code buffer, execute it.
          //
          // And because I enforce every basic block doesn't cross the possible smallest 1-kb page
          // boundary, and we have already passed the upper getting instruction operation,
          // thus I can sure that we can execute this threaded code buffer of the current basic
          // block without fearing the prefetch abort.
          return exec_threaded_code();
#if ENABLE_DYNAMIC_TRANSLATOR
        }
#endif
        break;
        
#if ENABLE_DYNAMIC_TRANSLATOR
      case Chunk::ST_DYNAMIC:
        return exec_dt_buffer();
#endif
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      // :NOTE: Wei 2004-Aug-02:
      //
      // If we execute here, that means we are starting a basic block, but this basic block
      // doesn't have a threaded code buffer yet.
      // Thus, we increase the hit count of this basic block.
      //
      // And mark 'm_chunk_start == false' to indicate that the next instruction is not the
      // beginning of the current basic block.
      mp_curr_chunk->increase_hit_count();
      m_chunk_start = false;
    }
#endif
    
#if ENABLE_INST_RECALL
    CurrentInstRecallEntry().mInst = inst.mRawData;
#endif
    
    switch (pass_cond(inst))
    {
    case PASS_TRUE:
#if ENABLE_INST_RECALL
      CurrentInstRecallEntry().mExec = true;
#endif
      break;
      
    case PASS_FALSE:
#if ENABLE_INST_RECALL
      CurrentInstRecallEntry().mExec = false;
#endif
      
      IncrePC();
      return ER_NORMAL;
      
#if CONFIG_ARCH_V5
    case PASS_EXCEPTION:
      // :NOTE: Wei 2004-Jul-27:
      //
      // an exception has occurs.
      return ER_EXCEPTION;
#endif
    }
    
#if ENABLE_THREADED_CODE
    ExecResultEnum const result = decode<USAGE_NORMAL>(inst);
#else
    ExecResultEnum const result = decode(inst);
#endif
    
#if ENABLE_THREADED_CODE
    switch (result)
    {
    case ER_EXCEPTION:
      finalize_chunk();
      break;
      
    case ER_NORMAL:
      break;
      
    case ER_CHUNK_DISAPPEAR:
    case ER_NORMAL_MODIFY_PC:
    default:
      assert(!"Should not reach here.");
      break;
    }
#endif
    
    return result;
    
    // :SA-1110 Developer's Manual: p.124: Wei 2004-May-14:
    //
    // Reads from reserved address locations (as specified in the memory map) will
    // result in a data abort exception.
    //
    // :NOTE: Wei 2004-Jul-30:
    //
    // In the original exception throwing version, an AccessReservedArea exception will
    // be caught here, and then convert to a 'data abort' exception.
    //
    // However, when every instructions see an exception, then they will use
    // Core::set_data_abort() to trigger a data abort in the CPU core.
    //
    // Thus I don't need to do some conversion here.
  }
  
  void
  Core::change_reg_bank(CoreMode new_mode)
  {
    if (m_curr_mode == new_mode)
    {
      return;
    }
    
#if TRACE_CORE_MODE
    g_log_file << "Core Mode change: " << m_curr_mode << " -> " << new_mode << std::endl;
#endif
    
    // :NOTE: Wei 2004-Jul-8:
    //
    // MODE_USR == 0b10000
    // MODE_SYS == 0b11111
    //
    // And there are no other 2 modes will result 0xF when they XOR together.
    // Thus I use the following codes to replace the original.
    //
    // new:
    //
    // if (0xF == (m_curr_mode ^ new_mode))
    //
    // original:
    //
    // if (((MODE_USR == m_curr_mode) && (MODE_SYS == new_mode)) ||
    //     ((MODE_SYS == m_curr_mode) && (MODE_USR == new_mode)))
    
    if (0xF == (m_curr_mode ^ new_mode))
    {
      assert(((MODE_USR == m_curr_mode) && (MODE_SYS == new_mode)) ||
             ((MODE_SYS == m_curr_mode) && (MODE_USR == new_mode)));
      
      m_curr_mode = new_mode;
      return;
    }
    
    if (MODE_FIQ == new_mode)
    {
      m_USR_reg_bank[CR_8 ] = m_curr_reg_bank[CR_8];
      m_USR_reg_bank[CR_9 ] = m_curr_reg_bank[CR_9];
      m_USR_reg_bank[CR_10] = m_curr_reg_bank[CR_10];
      m_USR_reg_bank[CR_11] = m_curr_reg_bank[CR_11];
      m_USR_reg_bank[CR_12] = m_curr_reg_bank[CR_12];
    }
    
    switch (m_curr_mode)
    {
    case MODE_USR:
    case MODE_SYS:
      m_USR_reg_bank[CR_SP] = m_curr_reg_bank[CR_SP];
      m_USR_reg_bank[CR_LR] = m_curr_reg_bank[CR_LR];
      break;
      
    case MODE_IRQ:
      m_IRQ_reg_bank[CR_SP] = m_curr_reg_bank[CR_SP];
      m_IRQ_reg_bank[CR_LR] = m_curr_reg_bank[CR_LR];
      
      m_IRQ_SPSR = m_curr_SPSR;
      break;
      
    case MODE_SVC:
      m_SVC_reg_bank[CR_SP] = m_curr_reg_bank[CR_SP];
      m_SVC_reg_bank[CR_LR] = m_curr_reg_bank[CR_LR];
      
      m_SVC_SPSR = m_curr_SPSR;
      break;
      
    case MODE_ABT:
      m_ABT_reg_bank[CR_SP] = m_curr_reg_bank[CR_SP];
      m_ABT_reg_bank[CR_LR] = m_curr_reg_bank[CR_LR];
      
      m_ABT_SPSR = m_curr_SPSR;
      break;
      
    case MODE_UND:
      m_UND_reg_bank[CR_SP] = m_curr_reg_bank[CR_SP];
      m_UND_reg_bank[CR_LR] = m_curr_reg_bank[CR_LR];
      
      m_UND_SPSR = m_curr_SPSR;
      break;
      
    case MODE_FIQ:
      m_FIQ_reg_bank[CR_8 ] = m_curr_reg_bank[CR_8];
      m_FIQ_reg_bank[CR_9 ] = m_curr_reg_bank[CR_9];
      m_FIQ_reg_bank[CR_10] = m_curr_reg_bank[CR_10];
      m_FIQ_reg_bank[CR_11] = m_curr_reg_bank[CR_11];
      m_FIQ_reg_bank[CR_12] = m_curr_reg_bank[CR_12];
      
      m_FIQ_reg_bank[CR_SP] = m_curr_reg_bank[CR_SP];
      m_FIQ_reg_bank[CR_LR] = m_curr_reg_bank[CR_LR];
      
      m_FIQ_SPSR = m_curr_SPSR;
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    if (MODE_FIQ == m_curr_mode)
    {
      m_curr_reg_bank[CR_8 ] = m_USR_reg_bank[CR_8];
      m_curr_reg_bank[CR_9 ] = m_USR_reg_bank[CR_9];
      m_curr_reg_bank[CR_10] = m_USR_reg_bank[CR_10];
      m_curr_reg_bank[CR_11] = m_USR_reg_bank[CR_11];
      m_curr_reg_bank[CR_12] = m_USR_reg_bank[CR_12];
    }
    
    switch (new_mode)
    {
    case MODE_USR:
    case MODE_SYS:
      m_curr_reg_bank[CR_SP] = m_USR_reg_bank[CR_SP];
      m_curr_reg_bank[CR_LR] = m_USR_reg_bank[CR_LR];
      break;
      
    case MODE_IRQ:
      m_curr_reg_bank[CR_SP] = m_IRQ_reg_bank[CR_SP];
      m_curr_reg_bank[CR_LR] = m_IRQ_reg_bank[CR_LR];
      
      m_curr_SPSR = m_IRQ_SPSR;
      break;
      
    case MODE_SVC:
      m_curr_reg_bank[CR_SP] = m_SVC_reg_bank[CR_SP];
      m_curr_reg_bank[CR_LR] = m_SVC_reg_bank[CR_LR];
      
      m_curr_SPSR = m_SVC_SPSR;
      break;
      
    case MODE_ABT:
      m_curr_reg_bank[CR_SP] = m_ABT_reg_bank[CR_SP];
      m_curr_reg_bank[CR_LR] = m_ABT_reg_bank[CR_LR];
      
      m_curr_SPSR = m_ABT_SPSR;
      break;
      
    case MODE_UND:
      m_curr_reg_bank[CR_SP] = m_UND_reg_bank[CR_SP];
      m_curr_reg_bank[CR_LR] = m_UND_reg_bank[CR_LR];
      
      m_curr_SPSR = m_UND_SPSR;
      break;
      
    case MODE_FIQ:
      m_curr_reg_bank[CR_8]  = m_FIQ_reg_bank[CR_8];
      m_curr_reg_bank[CR_9]  = m_FIQ_reg_bank[CR_9];
      m_curr_reg_bank[CR_10] = m_FIQ_reg_bank[CR_10];
      m_curr_reg_bank[CR_11] = m_FIQ_reg_bank[CR_11];
      m_curr_reg_bank[CR_12] = m_FIQ_reg_bank[CR_12];
      
      m_curr_reg_bank[CR_SP] = m_FIQ_reg_bank[CR_SP];
      m_curr_reg_bank[CR_LR] = m_FIQ_reg_bank[CR_LR];
      
      m_curr_SPSR = m_FIQ_SPSR;
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    m_curr_mode = new_mode;
  }
  
  void
  Core::restore_cpsr()
  {
    assert((MODE_SVC == m_curr_mode) ||
           (MODE_ABT == m_curr_mode) ||
           (MODE_UND == m_curr_mode) ||
           (MODE_IRQ == m_curr_mode) ||
           (MODE_FIQ == m_curr_mode));
    
    m_CPSR = m_curr_SPSR;
    
    change_reg_bank(static_cast<CoreMode>(m_CPSR & MODE_MASK));
  }
  
  template<>
  uint32_t
  Core::CheckAlign<WORD>(uint32_t const address, bool &exception_occur) const
  {
    if ((address & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK) != 0)
    {
      if (true == mpSysCoProc->enable_align_check())
      {
#if TRACE_MMU
        g_log_file << "MMU: Alignment Fault<"
                  << DataWidthTraits<WORD>::DESC_STRING
                  << ">: vaddr = "
                  << std::hex
                  << address
                  << std::endl;
#endif
        
        // :ARM-DDI-0100E: p.A4-31 for LDM_1: Wei 2004-May-13:
        // :ARM-DDI-0100E: p.A4-33 for LDM_2: Wei 2004-May-13:
        // :ARM-DDI-0100E: p.A4-35 for LDM_3: Wei 2004-May-13:
        //
        // :ARM-DDI-0100E: p.A4-85 for STM_1: Wei 2004-May-13:
        // :ARM-DDI-0100E: p.A4-87 for STM_2: Wei 2004-May-13:
        //
        // :ARM-DDI-0100E: p.A4-39 for LDR: Wei 2004-May-12:
        // :ARM-DDI-0100E: p.A4-50 for LDRT: Wei 2004-May-12:
        //
        // :ARM-DDI-0100E: p.A4-89 for STR: Wei 2004-May-12:
        // :ARM-DDI-0100E: p.A4-97 for STRT: Wei 2004-May-12:
        //
        // :ARM-DDI-0100E: p.A4-103 for SWP: Wei 2004-May-13:
        //
        // If an implementation includes a System Control coprocessor,
        // and alignment checking is enabled, an address with bits[1:0] != 0b00 causes
        // an alignment exception.
        
        update_FSR_FAR(FS_ALIGNMENT, 0, address);
        
        exception_occur = true;
        return 0;
      }
      else
      {
        // :ARM-DDI-0100E: p.A4-31 for LDM_1: Wei 2004-May-06:
        // :ARM-DDI-0100E: p.A4-33 for LDM_2: Wei 2004-May-06:
        // :ARM-DDI-0100E: p.A4-35 for LDM_3: Wei 2004-May-06:
        //
        // Load Multiple instructions ignore the least significant two bits of address
        // (the words are not rotated as for Load Word).
        //
        // :ARM-DDI-0100E: p.A4-85 for STM_1: Wei 2004-May-06:
        // :ARM-DDI-0100E: p.A4-87 for STM_2: Wei 2004-May-06:
        //
        // STM instructions ignore the least significant two bits of address.
        //
        // :ARM-DDI-0100E: p.A2-17 for STR: Wei 2004-May-06:
        //
        // STR instructions ignore the least significant two bits of address.
        // So if these bits are not 0b00, the effects of STR are not precisely opposite to those of LDR.
        //
        // :ARM-DDI-0100E: p.A4-103 for SWP: Wei 2004-May-13:
        
        // :NOTE: Wei 2004-May-13:
        //
        // Although ARM-DDI-0100E doesn't specified this behavior (ignore the least significant 2 bits)
        // for LDR, LDRT, STRT instructions. However, I assume STRT follows this rule, too.
        //
        // And the linux kernel also assume these 3 instructions follow this rule.
        // And one of a message from google also mention that these 3 instructions follow this rule.
        
        // :NOTE: Wei 2004-May-06:
        //
        // Quote a message from Internet:
        //
        // The StrongARM doesn't enforce any given pointer alignment.  However,
        // the behavior on non-word aligned addresses may surprise you.
        //
        // LDR *rotates* the non-aligned address in order to put the address you
        // requested in the LSB.  However, the high byte(s) will be the bytes
        // *before* the address requested.  For example, if we store the string
        // "Hello" starting at (word-aligned) address 0, like so:
        //   0  1  2  3  4
        //   H  e  l  l  o
        // The a load from address 1 will return 'ellH', *not* 'ello' as you might
        // expect.  [The official spec words it this way: "An address offset from a
        // word boundary will cause the data to be rotated into the register so that
        // the addressed byte occupies bits 0 to 7.]
        //
        // STR *ignores* the least-significant two bits of its address.  Storing
        // 'Jell' into address 1, 2, or 3 will produce 'Jello', just like a store
        // into address 0 would.
        //
        // Byte loads and stores (ldrb, strb) do The Right Thing, as you'd expect
        // them to.
        //
        // So -- be careful when reading/writing word-length data that your addresses
        // are properly 4-byte aligned!
        
#if TRACE_UNALIGN_ACCESS
        g_log_file << "Unalign access<"
                  << DataWidthTraits<WORD>::DESC_STRING
                  << ">: vaddr = "
                  << std::hex
                  << address
                  << std::endl;
#endif
        
        return address & ~DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK;
      }
    }
    
    return address;
  }
  
  template<>
  uint32_t
  Core::CheckAlign<HALF_WORD>(uint32_t const address, bool &exception_occur) const
  {
    if ((address & DataWidthTraits<HALF_WORD>::AlignNeeds::IGNORE_BITMASK) != 0)
    {
      if (true == mpSysCoProc->enable_align_check())
      {
#if TRACE_MMU
        g_log_file << "MMU: Alignment Fault<"
                  << DataWidthTraits<HALF_WORD>::DESC_STRING
                  << ">: vaddr = "
                  << std::hex
                  << address
                  << std::endl;
#endif
        
        // :ARM-DDI-0100E: p.A4-45 for LDRH: Wei 2004-May-06:
        // :ARM-DDI-0100E: p.A4-95 for STRH: Wei 2004-May-06:
        //
        // If an implementation includes a System Control coprocessor,
        // and alignment checking is enabled, an address with bit[0] != 0 causes an alignment exception.
        
        update_FSR_FAR(FS_ALIGNMENT, 0, address);
        
        exception_occur = true;
        return 0;
      }
      else
      {
        // :ARM-DDI-0100E: p.A4-45 for LDRH: Wei 2004-May-06:
        // :ARM-DDI-0100E: p.A4-95 for STRH: Wei 2004-May-06:
        //
        // If the load (store) address is not halfword-aligned,
        // the loaded (stored) value is UNPREDICTABLE.
        
#if TRACE_UNALIGN_ACCESS
        g_log_file << "Unalign access<"
                  << DataWidthTraits<WORD>::DESC_STRING
                  << ">: vaddr = "
                  << std::hex
                  << address
                  << std::endl;
#endif
        
        // :NOTE: Wei 2004-May-28:
        //
        // It seems that the UNPREDICTABLE condition in StrongArm is
        // just to ignore bit[0] when the data accesses are not half-word aligned.
        //
        // Also see ARM-DDI-0100E, p.A2-26, Section 'Unaligned data accesses'
        return address & ~DataWidthTraits<HALF_WORD>::AlignNeeds::IGNORE_BITMASK;
      }
    }
    
    return address;
  }
  
  template<>
  uint32_t
  Core::CheckAlign<BYTE>(uint32_t const address, bool & /* exception_occur */) const
  {
    return address;
  }
} // namespace ARMware
