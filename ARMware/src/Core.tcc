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

#include "Log.hpp"

namespace ARMware
{
#if ENABLE_INST_RECALL
  namespace
  {
    // :NOTE: Wei 2004-Feb-13:
    //
    // set physical address
    template<AccessTypeEnum accessTypeEnum>
    inline void
    RecordPaddr(Core const * const core, uint32_t const address)
    {
      core->CurrentInstRecallEntry().mPaddr[core->CurrentInstRecallEntry().mCurrentRegNum] = address;
    }
    
    template<>
    inline void
    RecordPaddr<INST>(Core const * const core, uint32_t const address)
    {
      core->CurrentInstRecallEntry().mIpPaddr = address;
    }
  }
#endif // #if ENABLE_INST_RECALL
  
#if DUMP_CPU_INFO
  template<bool T_dump_USR>
  void
  Core::dump_cpu_info() const
  {
    if (true == m_exec_in_chunk)
    {
      if (false == T_dump_USR)
      {
        g_log_file << "--> CUR: "
                   << std::hex;
        
        for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
        {
          g_log_file << CoreRegName(i)
                     << ":0x"
                     << m_curr_reg_bank[i]
                     << " ";
        }
      }
      else
      {
        g_log_file << std::endl
                   << "--> USR: "
                   << std::hex;
        
        for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
        {
          g_log_file << CoreRegName(i)
                     << ":0x"
                     << m_USR_reg_bank[i]
                     << " ";
        }
      }
      
      g_log_file << std::endl
                 << "--> CPSR:0x"
                 << m_CPSR
                 << ", CUR_SPSR:0x"
                 << m_curr_SPSR
                 << ", FIQ:0x"
                 << m_FIQ_SPSR
                 << ", SVC:0x"
                 << m_SVC_SPSR
                 << ", ABT:0x"
                 << m_ABT_SPSR
                 << ", IRQ:0x"
                 << m_IRQ_SPSR
                 << ", UND:0x"
                 << m_UND_SPSR
                 << std::endl
                 << "--> dev timer = "
                 << std::dec
                 << m_device_timer_increment
                 << std::endl;
    }
  }
#endif
  
  template<bool T_is_in_idle_mode, bool T_update_core>
  inline ExecResultEnum
  Core::check_periphery()
  {
    if (true == T_is_in_idle_mode)
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // ARMware is in idle mode.
      //
      // I found that distinguish the usage of the OS timer in the idle mode and the normal mode
      // will result in a faster cursor movement (and faster a lot !).
      //
      // Thus I think the cursor movement is depent on the OS timer interrupt.
      mpOsTimer->run();
      
      mp_serial_1->run();
      mp_serial_3->run();
    }
    else
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Normal OS timer.
#if ENABLE_THREADED_CODE
      m_3_6864_mhz_clocks += m_device_timer_increment;
#else
      ++m_3_6864_mhz_clocks;
#endif
      
      if (_3_6864_MHZ_FREQ
#if ENABLE_THREADED_CODE
          <=
#else
          ==
#endif
          m_3_6864_mhz_clocks)
      {
        mpOsTimer->run();
        
        m_3_6864_mhz_clocks = 0;
      }
      
      // :NOTE: Wei 2004-Sep-30:
      //
      // UART
#if ENABLE_THREADED_CODE
      m_UART_clocks += m_device_timer_increment;
#else
      ++m_UART_clocks;
#endif
      
      if (UART_FREQ
#if ENABLE_THREADED_CODE
          <=
#else
          ==
#endif
          m_UART_clocks)
      {
        mp_serial_1->run();
        
        if (true == m_RS232_is_enabled)
        {
          mp_serial_3->run();
        }
        
        m_UART_clocks = 0;
      }
    }
    
    mp_GPIO_control_block->run();
    mpRTC->run();
    
    // :NOTE: Wei 2004-Apr-25:
    //
    // I think the Interrupt Controller should be the last.
    mpIntController->run();
    
    ++m_get_event_clocks;
    if (GET_EVENT_FREQ == m_get_event_clocks)
    {
      mp_machine_screen->get_and_store_next_armware_event();
      
      m_get_event_clocks = 0;
    }
    
    // :ARM DDI 0100E: p.A2-10: Wei 2004-Apr-25:
    //
    // I and F are the interrupt disable bits:
    // I bit Disables IRQ interrupts when it is set.
    // F bit Disables FIQ interrupts when it is set.
    // 
    // :ARM DDI 0100E: p.A2-19: Wei 2004-Apr-25:
    //
    // If the I bit is clear, ARM checks for an IRQ at instruction boundaries.
    // If the F bit is clear, ARM checks for an FIQ at instruction boundaries.
    switch (m_CPSR & (IBIT_MASK | FBIT_MASK))
    {
    case (IBIT_MASK | FBIT_MASK):
      // FIQ disable.
      // IRQ disable.
      return ER_NORMAL_CONT_EXEC_IN_CHUNK;
      
    case FBIT_MASK:
      // FIQ disable.
      if (true == mpIntController->have_pending_irq())
      {
        if (true == T_update_core)
        {
          raise_IRQ_exception();
        }
        
        return ER_IO_IRQ_OCCUR;
      }
      else
      {
        return ER_NORMAL_CONT_EXEC_IN_CHUNK;
      }
      
    case IBIT_MASK:
      // IRQ disable.
      if (true == mpIntController->have_pending_fiq())
      {
        if (true == T_update_core)
        {
          raise_FIQ_exception();
        }
        
        return ER_IO_FIQ_OCCUR;
      }
      else
      {
        return ER_NORMAL_CONT_EXEC_IN_CHUNK;
      }
      
    case 0:
      if (true == mpIntController->have_pending_fiq())
      {
        if (true == T_update_core)
        {
          raise_FIQ_exception();
        }
        
        return ER_IO_FIQ_OCCUR;
      }
      // :NOTE: Wei 2004-Jun-09:
      //
      // Becuase FIQ mode will disable IRQ,
      // thus I have to use 'else if' rather than 'if' in here.
      else if (true == mpIntController->have_pending_irq())
      {
        if (true == T_update_core)
        {
          raise_IRQ_exception();
        }
        
        return ER_IO_IRQ_OCCUR;
      }
      else
      {
        return ER_NORMAL_CONT_EXEC_IN_CHUNK;
      }
      
    default:
      assert(!"Should not reach here.");
      return ER_NORMAL_CONT_EXEC_IN_CHUNK;
    }
  }
  
  template<AccessTypeEnum accessTypeEnum, RWType rw_type>
  inline uint32_t
  Core::virtual_to_physical(uint32_t &vaddr, CoreMode const currentMode, bool &exception_occur) const
  {
    if (true == mpSysCoProc->MMU_is_enabled())
    {
      ModifyFCSEVaddr(vaddr);
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if translate_access() throws an exception,
      // paddr will not be modified.
      //
      // However, because of paddr is a local variable,
      // thus I choose overwrite its value whether an exception occurs or not.
      uint32_t const paddr =
        mpMmu->template translate_access<accessTypeEnum, rw_type>(vaddr, currentMode, exception_occur);
      
      if (true == exception_occur)
      {
        return 0;
      }
      else
      {
        return paddr;
      }
    }
    else
    {
      return vaddr;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Get data
  
  // :ARM-DDI-0100E: p.B3-25: Wei 2004-May-14:
  //
  // If the Fast Context Switch Extension (FCSE) described in Chapter B6 is being used, then:
  //  - When the FAR is updated for a memory fault, the modified virtual address is written to the FAR.
  //  - When the FAR is read by an MRC instruction or written by an MCR instruction,
  //    its value is being treated as data and so no address modification is performed by the FCSE.
  //
  // :NOTE: Wei 2004-May-14:
  //
  // And the following get_data() & put_data() function is following this rule.
  // The argument 'vaddr' may be affected by the FCSE,
  // and the argument 'value' won't be affected by the FCSE.
  
  template<DataWidthEnum dataWidthEnum, AccessTypeEnum accessTypeEnum>
  inline typename DataWidthTraits<dataWidthEnum>::UnsignedType
  Core::get_data(uint32_t vaddr,
                 CoreMode const currentMode,
                 bool &exception_occur)
#if !ENABLE_THREADED_CODE
    const
#endif
  {
#if ENABLE_THREADED_CODE
    if (INST == accessTypeEnum)
    {
      // :NOTE: Wei 2004-Aug-13:
      //
      // This increment statement has to be placed here, in other words,
      // it has to be placed before any statements which can generate exceptions.
      //
      // The reason is...
      //
      // When Core::get_data() tries to get an instruction,
      // maybe one of the following statements will generate an exception.
      //
      // However, although one of them indeed generate an exception,
      // that instruction should still be included in the current basic block,
      // and that instruction should be the last one of that basic block.
      //
      // Thus I increment m_chunk_length here to accomplish this facility.
      ++m_chunk_length;
    }
#endif
    
    uint32_t const paddr = virtual_to_physical<accessTypeEnum, READ>(vaddr, currentMode, exception_occur);
    
    if (true == exception_occur)
    {
      return 0;
    }
    
#if ENABLE_INST_RECALL
    RecordPaddr<accessTypeEnum>(this, paddr);
#endif
    
    typename DataWidthTraits<dataWidthEnum>::UnsignedType const value =
      mpAddressSpace->get_data<typename DataWidthTraits<dataWidthEnum>::UnsignedType>(paddr, exception_occur);
    
    // :NOTE: Wei 2004-Jul-30:
    //
    // AddressSpace::get_data() might throw an exception of accessing reserved memory area.
    if (true == exception_occur)
    {
      return 0;
    }
    
#if ENABLE_THREADED_CODE
    if (INST == accessTypeEnum)
    {
      // :NOTE: Wei 2004-Aug-02:
      //
      // Because of simplifying the detection of self-modified codes,
      // I only generate threaded codes for the codes reside in the emulated dynamic memory.
      // 
      // That is to say, if the codes reside in other places, like emulated ROM,
      // I just left them untranslated, and executes them originally no matter how many times
      // they executed.
      //
      // (In H3600, the emulated dynamic memory resides in 0xC0000000 ~ 0xC1FFFFFF)
      if (0xC0000000 == (paddr & 0xCE000000))
      {
        if (0 == mp_curr_chunk)
        {
          // :NOTE: Wei 2004-Aug-14:
          //
          // There is no current basic block for now,
          // thus we should find a new one or an old one.
          
          // :NOTE: Wei 2004-Aug-16:
          //
          // reset the basic block length to 1.
          m_chunk_length = 1;
          
          m_chunk_start = true;
          
          mp_curr_chunk = mp_chunk_chain->find_chunk(paddr - MachineType::DRAM_BASE);
        }
        else
        {
          // :NOTE: Wei 2004-Aug-13:
          //
          // We have a basic block now.
          
          assert(m_chunk_length != 0);
        }
        
        // :NOTE: Wei 2004-Aug-02:
        //
        // The minimal page size of ARM is 1 Kb,
        // thus I enforce every basic block totally resides in one 1 kb page
        // to ensure that when each basic block is executed,
        // I won't get a prefetch abort exception in the middle of it.
#if ASSUME_NOT_USE_TINY_PAGE
        if ((SIZE_4K - 4) == (paddr & (SIZE_4K - 1)))
#else
        if ((SIZE_1K - 4) == (paddr & (SIZE_1K - 1)))
#endif
        {
          finalize_chunk_no_check();
        }
      }
      else
      {
        // :NOTE: Wei 2004-Aug-18:
        //
        // We have a basic block, however, we try to execute an instruction not located in RAM.
        // Thus I should finish the current basic block.
        if (mp_curr_chunk != 0)
        {
          finalize_chunk_no_check(--m_chunk_length);
        }
      }
    }
#endif
    
    return value;
  }
    
  /////////////////////////////////////////////////////////////////////////////
  // Put data
  
  template<DataWidthEnum dataWidthEnum>
  inline void
  Core::put_data(uint32_t vaddr,
                 typename DataWidthTraits<dataWidthEnum>::UnsignedType const value,
                 CoreMode const currentMode,
                 bool &exception_occur)
  {
    uint32_t const paddr = virtual_to_physical<DATA, WRITE>(vaddr, currentMode, exception_occur);
    
    if (true == exception_occur)
    {
      return;
    }
    
#if ENABLE_INST_RECALL
    RecordPaddr<DATA>(this, paddr);
#endif
    
    mpAddressSpace->put_data<typename DataWidthTraits<dataWidthEnum>::UnsignedType>(paddr, value);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // perform shift operation
  
  template<DPIOpcodeEnum T_opcode, IsUpdateStatus is_update_status>
  inline uint32_t
  Core::PreformImmDPISftOper(uint32_t const value, uint32_t const amount)
  {
    assert(amount <= 30);
    
    uint32_t const result = RotateRight(value, amount);
    
    // :NOTE: Wei 2004-Oct-28:
    //
    // The shifter carry is used to reserve the C bit flag in the shift
    // operation in the DPI instructions.
    //
    // However, the following DPI instructions will modify C bit flag
    // after the main operation:
    //
    // SUB, RSB, SBC, RSC, CMP, ADD, ADC, CMN
    //
    // That is to say, in these instructions, we don't need to 
    // calculate the value of the shifter carry.
    // 
    // On the other hand, in the other DPI instructions:
    //
    // AND, EOR, ORR, BIC, MOV, MVN, TST, TEQ
    //
    // we will need to calculate the value of the shifter carry,
    // and set the C bit flag after these instructions if they want
    // (i.e. S bit is set).
    switch (T_opcode)
    {
    case DPI_OP_AND:
    case DPI_OP_EOR:
    case DPI_OP_ORR:
    case DPI_OP_BIC:
    case DPI_OP_MOV:
    case DPI_OP_MVN:
    case DPI_OP_TST:
    case DPI_OP_TEQ:
      // :NOTE: Wei 2004-May-23:
      //
      // If we don't want to update the status register,
      // then we don't need to calculate the mShifterCarry.
      if (UPDATE_STATUS == is_update_status)
      {
        if (0 == amount)
        {
#if USE_SHIFTER_CARRY
          mShifterCarry = ((m_CPSR & CBIT_MASK) >> CBIT_POS);
#endif
        }
        else
        {
          // :NOTE: Wei 2004-Oct-28:
          //
          // Because shifter carry bit is used to reserve the value of the
          // C bit flag, those I can take an optimiziation of modifying the
          // C bit flag directly.
#if USE_SHIFTER_CARRY
          mShifterCarry = Bit31<MOVE_TO_RIGHT>(result);
#else
          Bit31<NO_MOVE_TO_RIGHT>(result) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
        }
      }
      break;
      
    case DPI_OP_SUB:
    case DPI_OP_RSB:
    case DPI_OP_SBC:
    case DPI_OP_RSC:
    case DPI_OP_CMP:
    case DPI_OP_ADD:
    case DPI_OP_ADC:
    case DPI_OP_CMN:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    return result;
  }
  
  template<DPIOpcodeEnum T_opcode, IsUpdateStatus is_update_status>
  inline uint32_t
  Core::PreformRegImmDPISftOper(uint32_t const value, SftType const type, uint32_t const amount)
  {
    assert(amount <= 31);
    
    uint32_t result;
    
    switch (type)
    {
    case LSL:
      result = value << amount;
      
      switch (T_opcode)
      {
      case DPI_OP_AND:
      case DPI_OP_EOR:
      case DPI_OP_ORR:
      case DPI_OP_BIC:
      case DPI_OP_MOV:
      case DPI_OP_MVN:
      case DPI_OP_TST:
      case DPI_OP_TEQ:
        if (UPDATE_STATUS == is_update_status)
        {
          if (0 == amount)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = ((m_CPSR & CBIT_MASK) >> CBIT_POS);
#endif
          }
          else
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, 32 - amount);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, 32 - amount) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
        }
        break;
        
      case DPI_OP_SUB:
      case DPI_OP_RSB:
      case DPI_OP_SBC:
      case DPI_OP_RSC:
      case DPI_OP_CMP:
      case DPI_OP_ADD:
      case DPI_OP_ADC:
      case DPI_OP_CMN:
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case LSR:
      if (0 == amount)
      {
        result = 0;
        
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bit31<MOVE_TO_RIGHT>(value);
#else
            Bit31<NO_MOVE_TO_RIGHT>(value) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      else
      {
        result = value >> amount;
        
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, amount - 1);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, amount - 1) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      break;
      
    case ASR:
      if (0 == amount)
      {
        if (IsNegative(value))
        {
          switch (T_opcode)
          {
          case DPI_OP_AND:
          case DPI_OP_EOR:
          case DPI_OP_ORR:
          case DPI_OP_BIC:
          case DPI_OP_MOV:
          case DPI_OP_MVN:
          case DPI_OP_TST:
          case DPI_OP_TEQ:
            if (UPDATE_STATUS == is_update_status)
            {
#if USE_SHIFTER_CARRY
              mShifterCarry = 1;
#else
              m_CPSR |= CBIT_MASK;
#endif
            }
            break;
            
          case DPI_OP_SUB:
          case DPI_OP_RSB:
          case DPI_OP_SBC:
          case DPI_OP_RSC:
          case DPI_OP_CMP:
          case DPI_OP_ADD:
          case DPI_OP_ADC:
          case DPI_OP_CMN:
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          result = 0xFFFFFFFF;
        }
        else
        {
          // Positive
          switch (T_opcode)
          {
          case DPI_OP_AND:
          case DPI_OP_EOR:
          case DPI_OP_ORR:
          case DPI_OP_BIC:
          case DPI_OP_MOV:
          case DPI_OP_MVN:
          case DPI_OP_TST:
          case DPI_OP_TEQ:
            if (UPDATE_STATUS == is_update_status)
            {
#if USE_SHIFTER_CARRY
              mShifterCarry = 0;
#else
              m_CPSR &= ~CBIT_MASK;
#endif
            }
            break;
            
          case DPI_OP_SUB:
          case DPI_OP_RSB:
          case DPI_OP_SBC:
          case DPI_OP_RSC:
          case DPI_OP_CMP:
          case DPI_OP_ADD:
          case DPI_OP_ADC:
          case DPI_OP_CMN:
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          result = 0;
        }
      }
      else
      {
        result = static_cast<int32_t>(value) >> amount;
        
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, amount - 1);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, amount - 1) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      break;
      
    case ROR:
      if (0 == amount)
      {
        // RRX
        result = value >> 1;
        
        if (true == IsCpsrCbitSet())
        {
          result |= 0x80000000;
        }
        
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bit0<MOVE_TO_RIGHT>(value);
#else
            Bit0<NO_MOVE_TO_RIGHT>(value) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      else
      {
        result = RotateRight(value, amount);
        
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, amount - 1);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, amount - 1) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      break;
      
    case RRX:
    default:
      assert(!"Should not reach here.");
      result = 0;
      break;
    }
    
    return result;
  }
  
  template<DPIOpcodeEnum T_opcode, IsUpdateStatus is_update_status>
  inline uint32_t
  Core::PreformRegRegDPISftOper(uint32_t const value, SftType const type, uint32_t const amount)
  {
    // :NOTE: Wei 2004-May-23:
    //
    // According to ARM-DDI-0100E, p.A5-3.
    // The max value of the shift amount will be the value of Rs,
    // but ARM core just take the least significant byte of Rs as the shift amount.
    //
    // Thus the max value of shift amount will be 255.
    assert(amount <= 255);
    
    uint32_t result;
    
    switch (type)
    {
    case LSL:
      // Logical Shift Left - 0b00
      switch (amount)
      {
      case 0:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = ((m_CPSR & CBIT_MASK) >> CBIT_POS);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = value;
        break;
        
      case 32:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bit0<MOVE_TO_RIGHT>(value);
#else
            Bit0<NO_MOVE_TO_RIGHT>(value) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = 0;
        break;
        
      case  1:
      case  2:
      case  3:
      case  4:
      case  5:
      case  6:
      case  7:
      case  8:
      case  9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
      case 23:
      case 24:
      case 25:
      case 26:
      case 27:
      case 28:
      case 29:
      case 30:
      case 31:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, 32 - amount);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, 32 - amount) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = (value << amount);
        break;
        
      default:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = 0;
#else
            m_CPSR &= ~CBIT_MASK;
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = 0;
        break;
      }
      break;
      
    case LSR:
      // Logical Shift Right - 0b01
      switch (amount)
      {
      case 0:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = ((m_CPSR & CBIT_MASK) >> CBIT_POS);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = value;
        break;
        
      case 32:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bit31<MOVE_TO_RIGHT>(value);
#else
            Bit31<NO_MOVE_TO_RIGHT>(value) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = 0;
        break;
        
      case  1:
      case  2:
      case  3:
      case  4:
      case  5:
      case  6:
      case  7:
      case  8:
      case  9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
      case 23:
      case 24:
      case 25:
      case 26:
      case 27:
      case 28:
      case 29:
      case 30:
      case 31:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, amount - 1);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, amount - 1) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
          
        result = (value >> amount);
        break;
        
      default:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = 0;
#else
            m_CPSR &= ~CBIT_MASK;
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
          
        result = 0;
        break;
      }
      break;
      
    case ASR:
      // Arthimetical Shift Right - 0b10
      switch (amount)
      {
      case 0:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = ((m_CPSR & CBIT_MASK) >> CBIT_POS);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = value;
        break;
        
      case  1:
      case  2:
      case  3:
      case  4:
      case  5:
      case  6:
      case  7:
      case  8:
      case  9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
      case 23:
      case 24:
      case 25:
      case 26:
      case 27:
      case 28:
      case 29:
      case 30:
      case 31:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, amount - 1);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, amount - 1) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = (static_cast<int32_t>(value) >> amount);
        break;
        
      default:
        if (IsNegative(value))
        {
          switch (T_opcode)
          {
          case DPI_OP_AND:
          case DPI_OP_EOR:
          case DPI_OP_ORR:
          case DPI_OP_BIC:
          case DPI_OP_MOV:
          case DPI_OP_MVN:
          case DPI_OP_TST:
          case DPI_OP_TEQ:
            if (UPDATE_STATUS == is_update_status)
            {
#if USE_SHIFTER_CARRY
              mShifterCarry = 1;
#else
              m_CPSR |= CBIT_MASK;
#endif
            }
            break;
            
          case DPI_OP_SUB:
          case DPI_OP_RSB:
          case DPI_OP_SBC:
          case DPI_OP_RSC:
          case DPI_OP_CMP:
          case DPI_OP_ADD:
          case DPI_OP_ADC:
          case DPI_OP_CMN:
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          result = 0xFFFFFFFF;
        }
        else
        {
          // Positive
          switch (T_opcode)
          {
          case DPI_OP_AND:
          case DPI_OP_EOR:
          case DPI_OP_ORR:
          case DPI_OP_BIC:
          case DPI_OP_MOV:
          case DPI_OP_MVN:
          case DPI_OP_TST:
          case DPI_OP_TEQ:
            if (UPDATE_STATUS == is_update_status)
            {
#if USE_SHIFTER_CARRY
              mShifterCarry = 0;
#else
              m_CPSR &= ~CBIT_MASK;
#endif
            }
            break;
            
          case DPI_OP_SUB:
          case DPI_OP_RSB:
          case DPI_OP_SBC:
          case DPI_OP_RSC:
          case DPI_OP_CMP:
          case DPI_OP_ADD:
          case DPI_OP_ADC:
          case DPI_OP_CMN:
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          result = 0;
        }
        break;
      }
      break;
      
    case ROR:
      // Rotate Right - 0b11
      switch (amount)
      {
      case 0:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = ((m_CPSR & CBIT_MASK) >> CBIT_POS);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = value;
        break;
        
      case 32:
      case 64:
      case 96:
      case 128:
      case 160:
      case 192:
      case 224:
        // :NOTE: Wei 2004-Feb-05:
        //
        // The multiple of 32
        //
        // Because the value range of the last argument (shift amount) is:
        // 0~255 (We can check this fact in ARMware's source code),
        // Thus I just check this 7 values which is the multiple of 32.
        
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bit31<MOVE_TO_RIGHT>(value);
#else
            Bit31<NO_MOVE_TO_RIGHT>(value) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = value;
        break;
        
      default:
        switch (T_opcode)
        {
        case DPI_OP_AND:
        case DPI_OP_EOR:
        case DPI_OP_ORR:
        case DPI_OP_BIC:
        case DPI_OP_MOV:
        case DPI_OP_MVN:
        case DPI_OP_TST:
        case DPI_OP_TEQ:
          if (UPDATE_STATUS == is_update_status)
          {
#if USE_SHIFTER_CARRY
            mShifterCarry = Bitn<MOVE_TO_RIGHT>(value, (amount & 0x1F) - 1);
#else
            Bitn<NO_MOVE_TO_RIGHT>(value, (amount & 0x1F) - 1) ? (m_CPSR |= CBIT_MASK) : (m_CPSR &= ~CBIT_MASK);
#endif
          }
          break;
          
        case DPI_OP_SUB:
        case DPI_OP_RSB:
        case DPI_OP_SBC:
        case DPI_OP_RSC:
        case DPI_OP_CMP:
        case DPI_OP_ADD:
        case DPI_OP_ADC:
        case DPI_OP_CMN:
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        result = RotateRight(value, amount & 0x1F);
        break;
      }
      break;
      
    case RRX:
    default:
      // Rotate Right with Extend - 0b11 with (immediate == 0)
      // If executes here, then means an error.
      assert(!"Should not reach here.");
      result = 0;
      break;
    }
    
    return result;
  }
  
  // CRT
  
  template<IsLoad isLoad>
  inline ExecResultEnum
  Core::exec_CRT(Inst const inst)
  {
    switch (CRTCpNum(inst))
    {
    case CP_0:
    case CP_1:
    case CP_2:
    case CP_3:
    case CP_4:
    case CP_5:
    case CP_6:
    case CP_7:
    case CP_8:
    case CP_9:
    case CP_10:
    case CP_11:
    case CP_12:
    case CP_13:
    case CP_14:
      // :ARM-DDI-0100E: p.A4-58: Wei 2004-May-12:
      //
      // If no coprocessors indicate that they can execute the instruction,
      // an Undefined Instruction exception is generated.
      return exec_UND(inst);
      
    case CP_15:
      mpSysCoProc->exec_CRT<isLoad>(inst);
      
      IncrePC();
      return ER_NORMAL;
      
    default:
      assert(!"Should not reach here.");
      return ER_NORMAL;
    }
  }
}
