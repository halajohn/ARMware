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

// The ARM system co-processor
//

#include <gtkmm/main.h>

#include <cassert>

#include "Core.hpp"
#include "SysCoProc.hpp"
#include "Mmu.hpp"

namespace ARMware
{
#if CHECK_UNPREDICTABLE
  ////////////////////////////////// Private //////////////////////////////////
  //================================ Operation ================================
  
  template<>
  inline void
  SysCoProc::check_access_right<LOAD>(Inst const inst) const
  {
    // :ARM-DDI-0100E: p.B3-25: Wei 2004-Feb-12:
    //
    // Register 8 is a write-only register.
    // Attempting to read CP15 register 8 with an MRC instruction is UNPREDICTABLE.
    //
    // :ARM-DDI-0100E: p.B5-15: Wei 2004-Feb-12:
    //
    // Register 7 is a write-only register.
    //
    // :SA-1110 Developer's Manual: p.63: Wei 2004-Feb-12:
    //
    // Register 15 is a write-only register.
    assert(CRTCRnNum(inst) != 8);
    assert(CRTCRnNum(inst) != 7);
    assert(CRTCRnNum(inst) != 15);
  }
  
  template<>
  inline void
  SysCoProc::check_access_right<STORE>(Inst const inst) const
  {
    // :SA-1110 Developer's Manual: Wei 2004-Jan-17:
    //
    // Register 0 is a read-only register.
    //
    // :ARM-DDI-0100E: Wei 2003-Nov-2:
    //
    // Writing to CP15 register 0 is UNPREDICTABLE.
    assert(CRTCRnNum(inst) != 0);
    
    // :SA-1110 Developer's Manual: p.59: Wei 2004-Jan-17:
    //
    // Accessing register 4 may yield unpredictable results.
    assert(CRTCRnNum(inst) != 4);
    
    // :SA-1110 Developer's Manual: p.61: Wei 2004-Jan-17:
    //
    // Accessing register 10 - 12 may yield unpredictable results.
    assert(CRTCRnNum(inst) != 10);
    assert(CRTCRnNum(inst) != 11);
    assert(CRTCRnNum(inst) != 12);
  }
#endif
  
  ////////////////////////////////// Public ///////////////////////////////////
  //============================== Life cycle =================================
  
  SysCoProc::SysCoProc(Core * const core)
    : mp_core(core)
  {
    init_register();
    
    // :SA-1110 Developer's Manual: Wei 2004-Jan-17:
    //
    // Main ID register (0) is a read-only register,
    // and the following value means:
    //
    // 8 : B2 stepping revision of SA-1110
    // 0xB11 : SA1110
    // 0x1 : ARM Architecture version 4
    // 0x69 : Intel
    m_reg[0] = 8 | (0xB11 << 4) | (0x1 << 16) | (0x69 << 24);
    
    // :SA-1110 Developer's Manual: p.57: Wei 2004-Jan-18:
    //
    // All writable bits in this register are forced low by reset.
    m_reg[1] &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) |
                    (1 << 7) | (1 << 8) | (1 << 9) |
                    (1 << 12) | (1 << 13));
    
    // :SA-1110 Developer's Manual: p.57: Wei 2004-May-17:
    //
    // The following 3 bits should always be 1.
    m_reg[1] |= ((1 << 4) | (1 << 5) | (1 << 6));
    
    // :SA-1110 Developer's Manual: p.61: Wei 2004-Jun-09:
    //
    // PID register is zero at reset.
    m_reg[CPR_13] = 0;
  }
  
  //================================ Operation ================================
  
  // MRC
  template<>
  void
  SysCoProc::exec_CRT<LOAD>(Inst const inst)
  {
#if ENABLE_INST_RECALL
    mp_core->CurrentInstRecallEntry().mInstNameTag = INST_MRC;
#endif
    
#if CHECK_UNPREDICTABLE
    check_opcode(inst);
    check_rd_require(inst);
    check_access_right<LOAD>(inst);
#endif
    
#if CHECK_UNPREDICTABLE
    switch (CRTCRnNum(inst))
    {
    case CPR_5:
    case CPR_6:
      // :ARM-DDI-0100E: p.B3-24 for FSR (CPR_5): Wei 2004-May-14:
      // :ARM-DDI-0100E: p.B3-24 for FAR (CPR_6): Wei 2004-May-14:
      //
      // The CRm and opcode2 fields are ignored when reading CP15 register 5 and should be zero.
      assert(0 == CRTCRmNum(inst));
      assert(0 == CRTOpcode2(inst));
      
    default:
      break;
    }
#endif
    
#if TRACE_SYS_CO
    g_log_file << "SYSCo: load " << std::hex << m_reg[CRTCRnNum(inst)] << " from reg " << CRTCRnNum(inst) << std::endl;
#endif
    
    uint32_t const data = m_reg[CRTCRnNum(inst)];
    
    if (CR_PC == CRTRdNum(inst))
    {
      (data & (1 << 31))
        ? mp_core->set_CPSR_N_bit()
        : mp_core->clear_CPSR_N_bit();
      
      (data & (1 << 30))
        ? mp_core->set_CPSR_Z_bit()
        : mp_core->clear_CPSR_Z_bit();
      
      (data & (1 << 29))
        ? mp_core->set_CPSR_C_bit()
        : mp_core->clear_CPSR_C_bit();
      
      (data & (1 << 28))
        ? mp_core->set_CPSR_V_bit()
        : mp_core->clear_CPSR_V_bit();
    }
    else
    {
      mp_core->rCurReg(CRTRdNum(inst)) = data;
    }
  }
  
  // MCR
  template<>
  void
  SysCoProc::exec_CRT<STORE>(Inst const inst)
  {
#if ENABLE_INST_RECALL
    mp_core->CurrentInstRecallEntry().mInstNameTag = INST_MCR;
#endif
    
#if CHECK_UNPREDICTABLE
    check_opcode(inst);
    check_rd_require(inst);
    check_access_right<STORE>(inst);
#endif
    
#if TRACE_SYS_CO
    g_log_file << "SYSCo: store " << std::hex << mp_core->CurReg(CRTRdNum(inst)) << " to reg " << CRTCRnNum(inst) << std::endl;
#endif
    
    switch (CRTCRnNum(inst))
    {
    case CPR_0:
      // :NOTE: Wei 2004-Jan-17:
      //
      // Register 0 - ID
      //
      // Already cause error in check_access_right<STORE>();
      break;
      
    case CPR_1:
      // :SA-1110 Developer's Manual: Wei 2004-Jan-17:
      //
      // Register 1 - Control
      // All unused bits are undefined on Read. Writes ignored.
      //
      // However, due to the performance, I don't implememt this facility,
      // write directly.
#if TRACE_MMU
      if (0x1 == ((m_reg[CPR_1] ^ mp_core->CurReg(CRTRdNum(inst))) & 0x1))
      {
        if (0x1 == (mp_core->CurReg(CRTRdNum(inst)) & 0x1))
        {
          g_log_file << "SYSCo: Enable MMU." << std::endl;
        }
        else
        {
          g_log_file << "SYSCo: Disable MMU." << std::endl;
        }
      }
      
      if (0x2 == ((m_reg[CPR_1] ^ mp_core->CurReg(CRTRdNum(inst))) & 0x2))
      {
        if (0x2 == (mp_core->CurReg(CRTRdNum(inst)) & 0x2))
        {
          g_log_file << "SYSCo: Enable Alignment check." << std::endl;
        }
        else
        {
          g_log_file << "SYSCo: Disable Alignment check." << std::endl;
        }
      }
#endif
      
      m_reg[CPR_1] = mp_core->CurReg(CRTRdNum(inst));
      break;
      
    case CPR_2:
      // Register 2 - Translation Table Base
      
      // :SA-1110 Developer's Manual: Wei 2004-Jan-17:
      //
      // Bits [13:0] are undefined on read, ignored on write.
      m_reg[CPR_2] = (mp_core->CurReg(CRTRdNum(inst))) & 0xFFFFC000;
      break;
      
    case CPR_3:
      // Register 3 - Domain Access Control
      m_reg[CPR_3] = mp_core->CurReg(CRTRdNum(inst));
      break;
      
    case CPR_4:
      // :SA-1110 Developer's Manual: Wei 2004-Jan-17:
      //
      // Already cause error in check_access_right<STORE>();
      break;
      
    case CPR_5:
      // Fault status register
    case CPR_6:
      // Fault address register
#if CHECK_UNPREDICTABLE
      // :ARM-DDI-0100E: p.B3-24 for FSR (CPR_5): Wei 2004-May-14:
      // :ARM-DDI-0100E: p.B3-25 for FAR (CPR_6): Wei 2004-May-14:
      //
      // The CRm and opcode2 fields are ignored when writing CP15 register 5 and should be zero.
      assert(0 == CRTCRmNum(inst));
      assert(0 == CRTOpcode2(inst));
#endif
      m_reg[CRTCRnNum(inst)] = mp_core->CurReg(CRTRdNum(inst));
      break;
      
    case CPR_7:
      // :NOTE: Wei 2004-Jan-18:
      //
      // Register 7 - Cache Control Operations
      //
      // However, I don't implement the cache facility now.
      m_reg[CPR_7] = mp_core->CurReg(CRTRdNum(inst));
      break;
      
    case CPR_8:
      // :SA-1110 Developer's Manual: p.60: Wei 2004-Jan-17:
      //
      // Register 8 - TLB Operations.
      // Register 8 is a write-only register.
      switch (CRTOpcode2(inst))
      {
      case 0:
        switch (CRTCRmNum(inst))
        {
        case CPR_5:
          // :SA-1110 Developer's Manual: p.60: Wei 2004-Jan-17:
          //
          // Flush instruction TLB
          mp_core->flush_inst_tlb();
          break;
          
        case CPR_6:
          // :SA-1110 Developer's Manual: p.60: Wei 2004-Jan-17:
          //
          // Flush data TLB
          mp_core->flush_data_tlb();
          break;
          
        case CPR_7:
          // :SA-1110 Developer's Manual: p.60: Wei 2004-Jan-17:
          //
          // Flush instruction & data TLB
          mp_core->flush_inst_tlb();
          mp_core->flush_data_tlb();
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
        
      case 1:
        switch (CRTCRmNum(inst))
        {
        case CPR_6:
          // :SA-1110 Developer's Manual: p.60: Wei 2004-Jan-17:
          //
          // Flush data TLB single entry
          mp_core->flush_data_tlb_single_entry(m_reg[CPR_6]);
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case CPR_9:
      // :SA-1110 Developer's Manual: Wei 2004-Jan-18:
      //
      // Register 9 - Read-Buffer Operations
      //
      // However, I don't implement the read buffer facility now.
      m_reg[CPR_9] = mp_core->CurReg(CRTRdNum(inst));
      break;
      
    case CPR_10:
    case CPR_11:
    case CPR_12:
      // :SA-1110 Developer's Manual: p.61: Wei 2004-Jan-17:
      //
      // 10~12: reserved registers
      //
      // Already cause error in check_access_right<STORE>();
      break;
      
    case CPR_13:
      if (true == is_access_pid(inst))
      {
        // :SA-1110 Developer's Manual: p.61: Wei 2004-Apr-15:
        //
        // Reserved bits must be written as zero.
        m_reg[CPR_13] = (mp_core->CurReg(CRTRdNum(inst)) & (0x7E << 25));
      }
      break;
      
    case CPR_14:
      // :TODO: Wei 2004-Feb-12:
      //
      // Write breakpoint
      assert(!"Should not reach here.");
      break;
      
    case CPR_15:
      // :TODO: Wei 2004-Feb-12:
      //
      // Test, clock and idle
      switch (CRTOpcode2(inst))
      {
      case 1:
        switch (CRTCRmNum(inst))
        {
        case CPR_1:
          // :SA-1110 Developer's Manual: p.64: Wei 2004-Apr-15:
          //
          // Enable odd-word loading of the linear feedback shift register (LFSR)
          DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
          break;
          
        case CPR_2:
          // Enable even-word loading of LFSR
          DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
          break;
          
        case CPR_4:
          // Clear LFSR
          DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
          break;
          
        case CPR_8:
          // Move LFSR to R14.abort
          DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
          break;
          
        default:
          DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
          break;
        }
        break;
        
      case 2:
        switch (CRTCRmNum(inst))
        {
        case CPR_1:
          // Enable clock switching
          //
          // :NOTE: Wei 2004-Apr-15:
          //
          // I think this is usful for hardware,
          // and I will ignore this in ARMware software emulator.
          break;
          
        case CPR_2:
          // Disable clock switching
          //
          // :NOTE: Wei 2004-Apr-15:
          //
          // I think this is usful for hardware,
          // and I will ignore this in ARMware software emulator.
          break;
          
        case CPR_4:
          // RESERVED
          DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
          break;
          
        case CPR_8:
          // Wait for interrupt
#if TRACE_IDLE_MODE
          fprintf(stderr, "%s\n", "SYSCO: Enter idle mode.");
#endif
          
          mp_core->enter_idle_mode();
          break;
          
        default:
          DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
          break;
        }
        break;
        
      default:
        DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
        break;
      }
      break;
      
    default:
      DiagnosticErrorWithInst(inst, __FILE__, __LINE__);
      break;
    }
  }
}
