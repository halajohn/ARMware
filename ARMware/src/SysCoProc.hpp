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

#ifndef SysCoProc_hpp
#define SysCoProc_hpp

// System include
//

#include <cassert>

// Project include
//

#include "ARMware.hpp"

#include "Inst/CRT.hpp"
#include "Inst/Inst.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Core;
  
  class CoProcRegTransInst;
  class CoProcDataOperInst;
  class LoadStoreCoProcInst;
  
  typedef class SysCoProc SysCoProc;
  class SysCoProc
  {
  private:
    
    // Attribute
    
    Core * const mp_core;
    
    // Register
    
    uint32_t m_reg[16];
    
    // Operation
    
    void
    init_register()
    {
      for (uint32_t i = 0; i < 16; ++i)
      {
        m_reg[i] = 0;
      }
    }
    
#if CHECK_UNPREDICTABLE
    template<IsLoad isLoad>
    void check_access_right(Inst const inst) const;
    
    inline void
    check_opcode(Inst const inst) const
    {
      // :ARM-DDI-0100E: p.B2-3: Wei 2003-Nov-7:
      //
      // Bits[23:21]
      //    These bits of the instruction, which are the <opcode1> field in
      //    generic MRC and MCR instructions, are always 0b000 in valid CP15
      //    instructions. If they are not 0b000, the instruction is UNPREDICTABLE.
      assert(0 == inst.mCRT.mOpcode1);
    }
    
    inline void
    check_rd_require(Inst const inst) const
    {
      // :ARM-DDI-0100E: p.B2-3: Wei 2003-Nov-7:
      //
      // Rd must not be R15, even though MCR instructions normally allow it to be R15.
      // If R15 is specified for <Rd> in a CP15 MRC or MCR instruction, the instruction
      // is UNPREDICTABLE.
      assert(CRTRdNum(inst) != CR_PC);
    }
#endif
    
    // :SA-1110 Developer's Manual: p.61: Wei 2004-Apr-15:
    //
    // There are special Opcode2 and CRm field encodings used to access the process ID
    // register.
    inline bool
    is_access_pid(Inst const inst) const
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Original:
      //
      // return ((CPR_0 == CRTCRmNum(inst)) && (0 == CRTOpcode2(inst))) ? true : false;
      
      if (0 == (inst.mRawData & 0xEF))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    
  public:
    
    // Life cycle
    
    SysCoProc(Core * const core);
    
    // Operation
    
    inline void
    reset()
    {
      disable_MMU();
      
      // :SA-1110 Developer's Manual: Wei 2004-Jan-08:
      //
      // bit 13 of register 1 is cleared at reset.
      m_reg[CPR_1] &= ~(1 << 13);
      
      // :SA-1110 Developer's Manual: p.61: Wei 2004-Apr-15:
      //
      // Process ID register (Register 13) is zero at reset.
      m_reg[CPR_13] = 0;
    }
    
    template<IsLoad isLoad>
    void exec_CRT(Inst const inst);
    
    inline void
    disable_MMU()
    {
      m_reg[CPR_1] &= ~1;
    }
    
    inline bool
    MMU_is_enabled() const
    {
      return ((m_reg[CPR_1] & 1) ? true : false);
    }
    
    inline bool
    use_high_vector() const
    {
      return ((m_reg[CPR_1] & (1 << 13)) ? true : false);
    }
    
    // Inquiry method
    
    inline uint32_t
    get_reg_value(CoProcRegNum const index) const
    {
      return m_reg[static_cast<uint32_t>(index)];
    }
    
    // Control Register
    
    inline uint32_t
    get_ctrl_reg() const
    {
      return m_reg[CPR_1];
    }
    
    // Translation Table Base
    
    inline uint32_t
    get_TTB_reg() const
    {
      return m_reg[CPR_2];
    }
    
    // Domain Access Control
    
    inline uint32_t
    get_DAC_reg() const
    {
      return m_reg[CPR_3];
    }
    
    // Test Alignment Enable bit
    
    inline bool
    enable_align_check() const
    {
      return (m_reg[CPR_1] & 0x2) ? true : false;
    }
    
    // Fault Status & Fault address
    
    inline void
    update_FSR_FAR(FaultStatus const status, uint32_t const domain_num, uint32_t const vaddr)
    {
      m_reg[CPR_5] = status | (domain_num << 4);
      m_reg[CPR_6] = vaddr;
    }
  };
}

#endif
