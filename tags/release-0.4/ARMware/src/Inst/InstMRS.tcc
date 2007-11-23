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

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // Rd
  
  template<UseWhichStatus use_which_status>
  inline CoreRegNum
  InstMRS<use_which_status>::
  rd_num() const
  {
    return static_cast<CoreRegNum>(m_inst.mMRS.mRd);
  }
  
#if CHECK_UNPREDICTABLE
  /////////////////////////////////////////////////////////////////////////////
  // Rd requirement
  
  template<UseWhichStatus use_which_status>
  inline void
  InstMRS<use_which_status>::
  check_rd_require() const
  {
    // :ARM-DDI-0100E: p.A4-60: Wei 2004-Feb-11:
    //
    // If R15 is specified for <Rd>, the result is UNPREDICTABLE.
    assert(rd_num() != CR_PC);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // some form of MRS have not to be in some mode.
  
  template<>
  inline void
  InstMRS<USE_CPSR>::
  check_proper_core_mode() const
  { }
  
  template<>
  inline void
  InstMRS<USE_CURRENT_SPSR>::
  check_proper_core_mode() const
  {
    // :ARM-DDI-0100E: p.A4-61: Wei 2004-Feb-11:
    //
    // Accessing the SPSR when in User mode is UNPREDICTABLE.
    // Accessing the SPSR when in System mode is UNPREDICTABLE.
    assert(gp_core->CurrentMode() != MODE_USR);
    assert(gp_core->CurrentMode() != MODE_SYS);
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // use which status register
  
  template<>
  inline uint32_t
  InstMRS<USE_CPSR>::
  desired_status_value() const
  {
    return gp_core->Cpsr();
  }
  
  template<>
  inline uint32_t
  InstMRS<USE_CURRENT_SPSR>::
  desired_status_value() const
  {
    return gp_core->CurSpsr();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<UseWhichStatus use_which_status>
  inline void
  InstMRS<use_which_status>::
  operation()
  {
    assert(0xF0000 == (m_inst.mRawData & 0xF0000));
    assert(0 == (m_inst.mRawData & 0xFFF));
    
#if ENABLE_INST_RECALL
    gp_core->CurrentInstRecallEntry().mInstNameTag = INST_MRS;
#endif
    
#if CHECK_UNPREDICTABLE
    check_rd_require();
    check_proper_core_mode();
#endif
    
    gp_core->rCurReg(rd_num()) = desired_status_value();
    
    gp_core->IncrePC();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<UseWhichStatus use_which_status>
  inline ExecResultEnum
  InstMRS<use_which_status>::
  Process(Inst const inst)
  {
    m_inst = inst;
    
    operation();
    
    // :NOTE: Wei 2004-Aug-09:
    //
    // Doesn't have ability to generate an exception.
    //
    // All Rd register in MRS instructions shouldn't be CR_PC.
    return ER_NORMAL;
  }  
}
