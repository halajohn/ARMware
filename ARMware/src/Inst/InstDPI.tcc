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
  // the immediate field of 32-bit immediate form
  //
  // RegImmSftDPI & RegRegSftDPI don't have immediate field
  
  namespace
  {
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    Imm_Helper(InstDPI<opcode,
               mode,
               is_update_status,
               T_usage,
               update_status_category_id,
               operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    Imm_Helper(InstDPI<opcode,
               ImmDPI,
               is_update_status,
               T_usage,
               update_status_category_id,
               operand_number_category_id> const * const dpi)
    {
      return dpi->m_inst.mImmDPI.mImm;
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline uint32_t
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  Imm() const
  {
    return Imm_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // the rotate immediate field of 32-bit immediate form
  //
  // RegImmSftDPI & RegRegSftDPI don't have rotate immediate field
  
  namespace
  {
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
    inline uint32_t
    RotImm_Helper(InstDPI<opcode,
                  mode,
                  is_update_status,
                  T_usage,
                  update_status_category_id,
                  operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    RotImm_Helper(InstDPI<opcode,
                  ImmDPI,
                  is_update_status,
                  T_usage,
                  update_status_category_id,
                  operand_number_category_id> const * const dpi)
    {
      return dpi->m_inst.mImmDPI.mRotImm;
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline uint32_t
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  RotImm() const
  {
    return RotImm_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Shift Immediate of immediate shift form
  //
  // ImmDPI & RegRegSftDPI don't have shift immediate field.
  
  namespace
  {
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    SftImm_Helper(InstDPI<opcode,
                  mode,
                  is_update_status,
                  T_usage,
                  update_status_category_id,
                  operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    SftImm_Helper(InstDPI<opcode,
                  RegImmSftDPI,
                  is_update_status,
                  T_usage,
                  update_status_category_id,
                  operand_number_category_id> const * const dpi)
    {
      return dpi->m_inst.mRegImmSftDPI.mSftImm;
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline uint32_t
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  SftImm() const
  {
    return SftImm_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rd
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RdNum_Helper(InstDPI<opcode,
                 ImmDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mImmDPI.mRd);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RdNum_Helper(InstDPI<opcode,
                 RegImmSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mRegImmSftDPI.mRd);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RdNum_Helper(InstDPI<opcode,
                 RegRegSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      
      return static_cast<CoreRegNum>(dpi->m_inst.mRegRegSftDPI.mRd);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline CoreRegNum
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  RdNum() const
  {
    return RdNum_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rn
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RnNum_Helper(InstDPI<opcode,
                 ImmDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mImmDPI.mRn);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RnNum_Helper(InstDPI<opcode,
                 RegImmSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mRegImmSftDPI.mRn);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RnNum_Helper(InstDPI<opcode,
                 RegRegSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mRegRegSftDPI.mRn);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline CoreRegNum
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  RnNum() const
  {
    return RnNum_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm
  //
  // ImmDPI doesn't have Rm field.
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RmNum_Helper(InstDPI<opcode,
                 ImmDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return static_cast<CoreRegNum>(0);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RmNum_Helper(InstDPI<opcode,
                 RegImmSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mRegImmSftDPI.mRm);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RmNum_Helper(InstDPI<opcode,
                 RegRegSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mRegRegSftDPI.mRm);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline CoreRegNum
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  RmNum() const
  {
    return RmNum_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rs
  //
  // ImmDPI & RegImmSftDPI don't have Rs field.
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RsNum_Helper(InstDPI<opcode,
                 ImmDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return static_cast<CoreRegNum>(0);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RsNum_Helper(InstDPI<opcode,
                 RegImmSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return static_cast<CoreRegNum>(0);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline CoreRegNum
    RsNum_Helper(InstDPI<opcode,
                 RegRegSftDPI,
                 is_update_status,
                 T_usage,
                 update_status_category_id,
                 operand_number_category_id> const * const dpi)
    {
      return static_cast<CoreRegNum>(dpi->m_inst.mRegRegSftDPI.mRs);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline CoreRegNum
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  RsNum() const
  {
    return RsNum_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Shift type
  //
  // ImmDPI doesn't have shift type field.
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline SftType
    SftType_Helper(InstDPI<opcode,
                   ImmDPI,
                   is_update_status,
                   T_usage,
                   update_status_category_id,
                   operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return static_cast<SftType>(0);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline SftType
    SftType_Helper(InstDPI<opcode,
                   RegImmSftDPI,
                   is_update_status,
                   T_usage,
                   update_status_category_id,
                   operand_number_category_id> const * const dpi)
    {
      return static_cast<SftType>(dpi->m_inst.mRegImmSftDPI.mSftType);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline SftType
    SftType_Helper(InstDPI<opcode,
                   RegRegSftDPI,
                   is_update_status,
                   T_usage,
                   update_status_category_id,
                   operand_number_category_id> const * const dpi)
    {
      return static_cast<SftType>(dpi->m_inst.mRegRegSftDPI.mSftType);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline SftType
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  WhichSftType() const
  {
    return SftType_Helper(this);
  }
  
#if CHECK_UNPREDICTABLE
  /////////////////////////////////////////////////////////////////////////////
  // Rn requirement
  
  namespace
  {
    // Used for ImmDPI & RegImmSftDPI
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRnRequirement_Helper(InstDPI<opcode,
                              mode,
                              is_update_status,
                              T_usage,
                              update_status_category_id,
                              operand_number_category_id> const * const /* dpi */)
    { }
    
    // Special case for RegRegSftDPI
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRnRequirement_Helper(InstDPI<opcode,
                              RegRegSftDPI,
                              is_update_status,
                              T_usage,
                              update_status_category_id,
                              operand_number_category_id> const * const dpi)
    {
      // :ARM-DDI-0100E: p.A5-10: Wei 2003-Dec-03:
      //
      // Specifying R_PC as Rd, Rm, Rn, Rs has UNPREDICTABLE results.
      assert(dpi->RnNum() != CR_PC);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  CheckRnRequirement() const
  {
    CheckRnRequirement_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm requirement
  
  namespace
  {
    // Used for ImmDPI & RegImmSftDPI
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRmRequirement_Helper(InstDPI<opcode,
                              mode,
                              is_update_status,
                              T_usage,
                              update_status_category_id,
                              operand_number_category_id> const * const /* dpi */)
    { }
    
    // Special case for RegRegSftDPI
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRmRequirement_Helper(InstDPI<opcode,
                              RegRegSftDPI,
                              is_update_status,
                              T_usage,
                              update_status_category_id,
                              operand_number_category_id> const * const dpi)
    {
      // :ARM-DDI-0100E: p.A5-10: Wei 2003-Dec-03:
      //
      // Specifying CR_PC as Rd, Rm, Rn, Rs has UNPREDICTABLE results.
      assert(dpi->RmNum() != CR_PC);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  CheckRmRequirement() const
  {
    CheckRmRequirement_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rd requirement
  
  namespace
  {
    // Used for ImmDPI & RegImmSftDPI
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRdRequirementByMode_Helper(InstDPI<opcode,
                                    mode,
                                    is_update_status,
                                    T_usage,
                                    update_status_category_id,
                                    operand_number_category_id> const * const /* dpi */)
    { }
    
    // Special case for RegRegSftDPI
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRdRequirementByMode_Helper(InstDPI<opcode,
                                    RegRegSftDPI,
                                    is_update_status,
                                    T_usage,
                                    update_status_category_id,
                                    operand_number_category_id> const * const dpi)
    {
      // :ARM-DDI-0100E: p.A5-10: Wei 2003-Dec-03:
      //
      // Specifying CR_PC as Rd, Rm, Rn, Rs has UNPREDICTABLE results.
      assert(dpi->RdNum() != CR_PC);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  CheckRdRequirementByMode() const
  {
    CheckRdRequirementByMode_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  namespace
  {
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id>
    inline void
    CheckRdRequirementByOpcode_Helper(InstDPI<opcode,
                                      mode,
                                      is_update_status,
                                      T_usage,
                                      update_status_category_id,
                                      OpcodeNeedThreeOperand::OperandNumberCategoryId> const * const /* dpi */)
    { }
    
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id>
    inline void
    CheckRdRequirementByOpcode_Helper(InstDPI<opcode,
                                      mode,
                                      is_update_status,
                                      T_usage,
                                      update_status_category_id,
                                      OpcodeNoNeedRd::OperandNumberCategoryId> const * const dpi)
    {
      assert(CR_0 == dpi->RdNum());
    }
    
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id>
    inline void
    CheckRdRequirementByOpcode_Helper(InstDPI<opcode,
                                      mode,
                                      is_update_status,
                                      T_usage,
                                      update_status_category_id,
                                      OpcodeNoNeedRn::OperandNumberCategoryId> const * const dpi)
    {
      assert(CR_0 == dpi->RnNum());
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  CheckRdRequirementByOpcode() const
  {
    CheckRdRequirementByOpcode_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rs requirement
  
  namespace
  {
    // Used for ImmDPI & RegImmSftDPI
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRsRequirement_Helper(InstDPI<opcode,
                              mode,
                              is_update_status,
                              T_usage,
                              update_status_category_id,
                              operand_number_category_id> const * const /* dpi */)
    { }
    
    // Special case for RegRegSftDPI
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    CheckRsRequirement_Helper(InstDPI<opcode,
                              RegRegSftDPI,
                              is_update_status,
                              T_usage,
                              update_status_category_id,
                              operand_number_category_id> const * const dpi)
    {
      // :ARM-DDI-0100E: p.A5-10: Wei 2003-Dec-03:
      //
      // Specifying CR_PC as Rd, Rm, Rn, Rs has UNPREDICTABLE results.
      assert(dpi->RsNum() != CR_PC);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  CheckRsRequirement() const
  {
    CheckRsRequirement_Helper(this);
  }
  
#endif // CHECK_UNPREDICTABLE
  
  /////////////////////////////////////////////////////////////////////////////
  // The value of Rn
  
  namespace
  {
    // Used for ImmDPI & RegImmSftDPI
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    Rn_Helper(InstDPI<opcode,
              mode,
              is_update_status,
              T_usage,
              update_status_category_id,
              operand_number_category_id> const * const dpi)
    {
#if CHECK_UNPREDICTABLE
      dpi->CheckRnRequirement();
#endif
      
      // :NOTE: Wei 2004-Feb-07:
      //
      // I can't find any statement describing the Rn requirement in ARMARM about
      // ImmDPI, thus I assume ImmDPI has the default behavior:
      // when read from PC, they will read PC + 8.
      
      // :ARM-DDI-0100E: p.A5-9: Wei 2003-Dec-03:
      //
      // If CR_PC is specified as Rn, the value used is the address of the current
      // instruction plus 8.
      return ((CR_PC == dpi->RnNum())
              ? (gp_core->ReadPC())
              : (gp_core->CurReg(dpi->RnNum())));
    }
    
    // Special case for RegRegSftDPI
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    Rn_Helper(InstDPI<opcode,
              RegRegSftDPI,
              is_update_status,
              T_usage,
              update_status_category_id,
              operand_number_category_id> const * const dpi)
    {
#if CHECK_UNPREDICTABLE
      dpi->CheckRnRequirement();
#endif
      
      // :NOTE: Wei 2004-Feb-05:
      //
      // Because Rn shouldn't be CR_PC in this mode,
      // thus we don't need to consider the special case of plus 8.
      return gp_core->CurReg(dpi->RnNum());
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline uint32_t
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  Rn() const
  {
    return Rn_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // The value of Rm
  //
  // ImmDPI doesn't have Rm
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    Rm_Helper(InstDPI<opcode,
              ImmDPI,
              is_update_status,
              T_usage,
              update_status_category_id,
              operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    Rm_Helper(InstDPI<opcode,
              RegImmSftDPI,
              is_update_status,
              T_usage,
              update_status_category_id,
              operand_number_category_id> const * const dpi)
    {
#if CHECK_UNPREDICTABLE
      dpi->CheckRmRequirement();
#endif
      
      // :ARM-DDI-0100E: p.A5-9: Wei 2004-Feb-03:
      //
      // If CR_PC is specified as Rm, the value used is the address of the current
      // instruction plus 8.
      
      return ((CR_PC == dpi->RmNum())
              ? (gp_core->ReadPC())
              : (gp_core->CurReg(dpi->RmNum())));
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    Rm_Helper(InstDPI<opcode,
              RegRegSftDPI,
              is_update_status,
              T_usage,
              update_status_category_id,
              operand_number_category_id> const * const dpi)
    {
#if CHECK_UNPREDICTABLE
      dpi->CheckRmRequirement();
#endif
      
      // :NOTE: Wei 2004-Feb-05:
      //
      // Because Rm shouldn't be CR_PC in this mode,
      // thus we don't need to consider the special case of plus 8.
      
      return gp_core->CurReg(dpi->RmNum());
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline uint32_t
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  Rm() const
  {
    return Rm_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // shift operand
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    SftOperand_Helper(InstDPI<opcode,
                      ImmDPI,
                      is_update_status,
                      T_usage,
                      update_status_category_id,
                      operand_number_category_id> const * const dpi)
    {
      // :NOTE: Wei 2004-Apr-16:
      //
      // The value range of the last argument (shift amount) is: 0 ~ 30
      return gp_core->template PreformImmDPISftOper
        <opcode::OPCODE, is_update_status>(dpi->Imm(),
                                           dpi->RotImm() << 1);
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    SftOperand_Helper(InstDPI<opcode,
                      RegImmSftDPI,
                      is_update_status,
                      T_usage,
                      update_status_category_id,
                      operand_number_category_id> const * const dpi)
    {
      return gp_core->template PreformRegImmDPISftOper
        <opcode::OPCODE, is_update_status>(dpi->Rm(),
                                           dpi->WhichSftType(),
                                           dpi->SftImm());
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline uint32_t
    SftOperand_Helper(InstDPI<opcode,
                      RegRegSftDPI,
                      is_update_status,
                      T_usage,
                      update_status_category_id,
                      operand_number_category_id> const * const dpi)
    {
#if CHECK_UNPREDICTABLE
      dpi->CheckRsRequirement();
#endif
      
      // :ARM-DDI-0100E: p.A5-10: Wei 2003-Nov-1:
      //
      // ... shift by the value in the least significant byte of register Rs.
      //
      // :NOTE: Wei 2004-Apr-16:
      //
      // Thus the value range of the last argument (shift amount) is: 0 ~ 255
      return gp_core->template PreformRegRegDPISftOper
        <opcode::OPCODE, is_update_status>
        (dpi->Rm(),
         dpi->WhichSftType(),
         static_cast<uint8_t>(gp_core->CurReg(dpi->RsNum())));
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline uint32_t
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  SftOperand() const
  {
    return SftOperand_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // update_CPSR_flags
  
  namespace
  {
    /////////////////////////////////////////////////////////////////////////////
    // AND
    
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_AND(InstDPI<opcode,
                          mode,
                          is_update_status,
                          T_usage,
                          update_status_category_id,
                          operand_number_category_id> const * const dpi)
    {
      // V bit unaffected
      
      // :NOTE: Wei 2004-Oct-28:
      //
      // Because shifter carry bit is used to reserve the value of the
      // C bit flag, those I can take an optimiziation of modifying the
      // C bit flag directly in the shift operaion stage.
      //
      // Those if I take this optimization, then I should not clear the
      // C bit flag here.
#if USE_SHIFTER_CARRY
      gp_core->clear_CPSR_NZC_bit();
#else
      gp_core->clear_CPSR_NZ_bit();
#endif
      
#if ENABLE_COND_CODES_ASM
      switch (dpi->m_eflags & (IA32_EFLAGS_ZERO | IA32_EFLAGS_SIGN))
      {
      case 0: break;
        
      case IA32_EFLAGS_ZERO : gp_core->set_CPSR_Z_bit(); break;
      case IA32_EFLAGS_SIGN : gp_core->set_CPSR_N_bit(); break;
        
        // :NOTE: Wei 2004-Jul-5:
        //
        // Zero & Negative flags should never be appear together.
      case (IA32_EFLAGS_ZERO  | IA32_EFLAGS_SIGN): assert(!"Should not reach here."); break;
        
      default: assert(!"Should not reach here."); break;
      }
#else
      // Z bit
      if (0 == dpi->result)
      {
        gp_core->set_CPSR_Z_bit();
      }
      else
      {
        // N bit
        if (true == IsNegative(dpi->result))
        {
          gp_core->set_CPSR_N_bit();
        }
      }
#endif
      
#if USE_SHIFTER_CARRY
      assert((1 == gp_core->ShifterCarry()) || (0 == gp_core->ShifterCarry()));
      
      // C bit
      if (1 == gp_core->ShifterCarry())
      {
        gp_core->set_CPSR_C_bit();
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // SUB
    
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_SUB(InstDPI<opcode,
                          mode,
                          is_update_status,
                          T_usage,
                          update_status_category_id,
                          operand_number_category_id> const * const dpi)
    {
      gp_core->clear_CPSR_NZCV_bit();
      
#if ENABLE_COND_CODES_ASM
      // :NOTE: Wei 2004-Jul-06:
      //
      // Becuase the rule of the subtraction kind of ARM instructions updating its carry flag is 'NOT BorrowFrom'.
      // However, the rule of subtraction kind of IA32 instructions updating its carry flag is 'BorrowFrom'
      //
      // Thus I set ARM's carry flag when IA32's carry == 0, and vice versa.
      switch (dpi->m_eflags & (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER))
      {
      case 0: gp_core->set_CPSR_C_bit(); break;
        
      case IA32_EFLAGS_CARRY: break;
      case IA32_EFLAGS_ZERO : gp_core->set_CPSR_ZC_bit(); break;
      case IA32_EFLAGS_SIGN : gp_core->set_CPSR_NC_bit(); break;
      case IA32_EFLAGS_OVER : gp_core->set_CPSR_CV_bit(); break;
        
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO): gp_core->set_CPSR_Z_bit(); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_SIGN): gp_core->set_CPSR_N_bit(); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_OVER): gp_core->set_CPSR_V_bit(); break;
      case (IA32_EFLAGS_ZERO  | IA32_EFLAGS_SIGN): assert(!"Should not reach here."); break;
      case (IA32_EFLAGS_ZERO  | IA32_EFLAGS_OVER): gp_core->set_CPSR_ZCV_bit(); break;
      case (IA32_EFLAGS_SIGN  | IA32_EFLAGS_OVER): gp_core->set_CPSR_NCV_bit(); break;
        
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_SIGN): assert(!"Should not reach here."); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_OVER): gp_core->set_CPSR_ZV_bit(); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER): gp_core->set_CPSR_NV_bit(); break;
      case (IA32_EFLAGS_ZERO  | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER): assert(!"Should not reach here."); break;
        
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER): assert(!"Should not reach here."); break;
        
      default: assert(!"Should not reach here."); break;
      }
#else
      // Z bit
      if (0 == dpi->result)
      {
        gp_core->set_CPSR_Z_bit();
      }
      else
      {
        // N bit
        if (true == IsNegative(dpi->result))
        {
          gp_core->set_CPSR_N_bit();
        }
      }
      
      // C bit
      
      if((IsNegative(dpi->op1) && IsPositive(dpi->op2)) ||
         (IsNegative(dpi->op1) && IsPositive(dpi->result)) ||
         (IsPositive(dpi->op2) && IsPositive(dpi->result)))
      {
        gp_core->set_CPSR_C_bit();
      }
      
      // V bit
      if ((IsNegative(dpi->op1) && IsPositive(dpi->op2) && IsPositive(dpi->result)) ||
          (IsPositive(dpi->op1) && IsNegative(dpi->op2) && IsNegative(dpi->result)))
      {
        gp_core->set_CPSR_V_bit();
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // ADD
    
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_ADD(InstDPI<opcode,
                          mode,
                          is_update_status,
                          T_usage,
                          update_status_category_id,
                          operand_number_category_id> const * const dpi)
    {
      gp_core->clear_CPSR_NZCV_bit();
      
#if ENABLE_COND_CODES_ASM
      switch (dpi->m_eflags & (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER))
      {
      case 0: break;
        
      case IA32_EFLAGS_CARRY: gp_core->set_CPSR_C_bit(); break;
      case IA32_EFLAGS_ZERO : gp_core->set_CPSR_Z_bit(); break;
      case IA32_EFLAGS_SIGN : gp_core->set_CPSR_N_bit(); break;
      case IA32_EFLAGS_OVER : gp_core->set_CPSR_V_bit(); break;
        
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO): gp_core->set_CPSR_ZC_bit(); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_SIGN): gp_core->set_CPSR_NC_bit(); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_OVER): gp_core->set_CPSR_CV_bit(); break;
      case (IA32_EFLAGS_ZERO  | IA32_EFLAGS_SIGN): assert(!"Should not reach here."); break;
      case (IA32_EFLAGS_ZERO  | IA32_EFLAGS_OVER): gp_core->set_CPSR_ZV_bit(); break;
      case (IA32_EFLAGS_SIGN  | IA32_EFLAGS_OVER): gp_core->set_CPSR_NV_bit(); break;
        
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_SIGN): assert(!"Should not reach here."); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_OVER): gp_core->set_CPSR_ZCV_bit(); break;
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER): gp_core->set_CPSR_NCV_bit(); break;
      case (IA32_EFLAGS_ZERO  | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER): assert(!"Should not reach here."); break;
        
      case (IA32_EFLAGS_CARRY | IA32_EFLAGS_ZERO | IA32_EFLAGS_SIGN | IA32_EFLAGS_OVER): assert(!"Should not reach here."); break;
        
      default: assert(!"Should not reach here."); break;
      }
#else
      // Z bit
      if (0 == dpi->result)
      {
        gp_core->set_CPSR_Z_bit();
      }
      else 
      {
        // N bit
        if (true == IsNegative(dpi->result))
        {
          gp_core->set_CPSR_N_bit();
        }
      }
      
      if ((dpi->op1 | dpi->op2) >> 30)
      {
        // C bit
        if ((IsNegative(dpi->op1) && IsNegative(dpi->op2)) ||
            (IsNegative(dpi->op1) && IsPositive(dpi->result)) ||
            (IsNegative(dpi->op2) && IsPositive(dpi->result)))
        {
          gp_core->set_CPSR_C_bit();
        }
        
        // V bit
        if ((IsNegative(dpi->op1) && IsNegative(dpi->op2) && IsPositive(dpi->result)) ||
            (IsPositive(dpi->op1) && IsPositive(dpi->op2) && IsNegative(dpi->result)))
        {
          gp_core->set_CPSR_V_bit();
        }
      }
#endif
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<AND,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<EOR,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<TST,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<TEQ,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<ORR,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<MOV,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<BIC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<MVN,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_AND(dpi);
    }
    
    ////
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<SUB,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_SUB(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<RSB,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_SUB(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<SBC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_SUB(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<RSC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_SUB(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<CMP,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_SUB(dpi);
    }
    
    ////
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<ADD,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_ADD(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<ADC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_ADD(dpi);
    }
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    update_CPSR_flags_Helper(InstDPI<CMN,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> const * const dpi)
    {
      update_CPSR_flags_ADD(dpi);
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  update_CPSR_flags() const
  {
    update_CPSR_flags_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Finalize
  
  namespace
  {
    // Opcode May Not Update Status
    
    template<typename opcode,
             typename mode,
             UsageEnum T_usage,
             typename operand_number_category_id>
    inline void
    Finalize_Helper(InstDPI<opcode,
                    mode,
                    NO_UPDATE_STATUS,
                    T_usage,
                    OpcodeMayNotUpdateStatus::UpdateStatusCategoryId,
                    operand_number_category_id> const * const dpi)
    {
      if (CR_PC != dpi->RdNum())
      {
        gp_core->IncrePC();
      }
#if ENABLE_THREADED_CODE
      else
      {
        if (USAGE_NORMAL == T_usage)
        {
          // Non-sequential next PC
          gp_core->finalize_chunk();
        }
      }
#endif
    }
    
    template<typename opcode,
             typename mode,
             UsageEnum T_usage,
             typename operand_number_category_id>
    inline void
    Finalize_Helper(InstDPI<opcode,
                    mode,
                    UPDATE_STATUS,
                    T_usage,
                    OpcodeMayNotUpdateStatus::UpdateStatusCategoryId,
                    operand_number_category_id> const * const dpi)
    {
      if (CR_PC == dpi->RdNum())
      {
        gp_core->restore_cpsr();
        
#if ENABLE_THREADED_CODE
        if (USAGE_NORMAL == T_usage)
        {
          // Non-sequential next PC
          gp_core->finalize_chunk();
        }
#endif
      }
      else
      {
        dpi->update_CPSR_flags();
        
        gp_core->IncrePC();
      }
    }
    
    // Opcode Has To Update Status
    
    template<typename opcode,
             typename mode,
             UsageEnum T_usage,
             typename operand_number_category_id>
    inline void
    Finalize_Helper(InstDPI<opcode,
                    mode,
                    NO_UPDATE_STATUS,
                    T_usage,
                    OpcodeHasToUpdateStatus::UpdateStatusCategoryId,
                    operand_number_category_id> const * const /* dpi */)
    {
      assert(!"Should not reach here.");
    }
    
    template<typename opcode,
             typename mode,
             UsageEnum T_usage,
             typename operand_number_category_id>
    inline void
    Finalize_Helper(InstDPI<opcode,
                    mode,
                    UPDATE_STATUS,
                    T_usage,
                    OpcodeHasToUpdateStatus::UpdateStatusCategoryId,
                    operand_number_category_id> const * const dpi)
    {
      dpi->update_CPSR_flags();
      
      gp_core->IncrePC();
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  Finalize() const
  {
    Finalize_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation Internal
  
  namespace
  {
    /////////////////////////////////////////////////////////////////////////////
    // AND or ANDS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<AND,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_AND;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
#if CHECK_ASM_CORRECT
        uint32_t const al_op1 = dpi->Rn();
        uint32_t const al_op2 = dpi->SftOperand();
#endif
        
        asm volatile ("andl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->SftOperand()), "1" (dpi->Rn())
                      : "cc");
        
#if CHECK_ASM_CORRECT
        assert(gp_core->rCurReg(dpi->RdNum()) == (al_op1 & al_op2));
#endif
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() & dpi->SftOperand();
      }
#else
      dpi->result = dpi->Rn() & dpi->SftOperand();
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // EOR or EORS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<EOR,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_EOR;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
#if CHECK_ASM_CORRECT
        uint32_t const al_op1 = dpi->Rn();
        uint32_t const al_op2 = dpi->SftOperand();
#endif
        
        asm volatile ("xorl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->SftOperand()), "1" (dpi->Rn())
                      : "cc");
        
#if CHECK_ASM_CORRECT
        assert(gp_core->rCurReg(dpi->RdNum()) == (al_op1 ^ al_op2));
#endif
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() ^ dpi->SftOperand();
      }
#else
      dpi->result = dpi->Rn() ^ dpi->SftOperand();
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // SUB or SUBS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<SUB,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_SUB;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->Rn());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t al_result;
#endif
      
      // :NOTE: Wei 2003-Nov-5:
      //
      // The values stored in registers are _positive_ or _2's-complement negative_.
      // This includes DPIRn<Mode>(core, inst) & DPISftOperand<Mode>(core, inst) in the following.
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
        asm volatile ("subl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->SftOperand()), "1" (dpi->Rn())
                      : "cc");
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() - dpi->SftOperand();
      }
#else
      dpi->op1 = dpi->Rn();
      dpi->op2 = dpi->SftOperand();
      dpi->result = dpi->op1 - dpi->op2;
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      al_result = al_op1 - al_op2;
      
      assert(gp_core->rCurReg(dpi->RdNum()) == static_cast<uint32_t>(al_result));
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      if (UPDATE_STATUS == is_update_status)
      {
        uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
        al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
        
        switch (al_carry)
        {
        case 0:  assert(true == gp_core->IsCpsrCbitSet()); break;
        case 1:  assert(false == gp_core->IsCpsrCbitSet()); break;
        default: assert(!"Should not reach here."); break;
        }
        
        if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
        {
          assert(true == gp_core->IsCpsrVbitSet());
        }
        else
        {
          assert(false == gp_core->IsCpsrVbitSet());
        }
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // RSB or RSBS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<RSB,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_RSB;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->Rn());
      uint64_t al_result;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
        asm volatile ("subl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->Rn()), "1" (dpi->SftOperand())
                      : "cc");
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->SftOperand() - dpi->Rn();
      }
#else
      dpi->op1 = dpi->SftOperand();
      dpi->op2 = dpi->Rn();
      dpi->result = dpi->op1 - dpi->op2;
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      al_result = al_op1 - al_op2;
      
      assert(gp_core->rCurReg(dpi->RdNum()) == static_cast<uint32_t>(al_result));
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      if (UPDATE_STATUS == is_update_status)
      {
        uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
        al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
        
        switch (al_carry)
        {
        case 0:  assert(true == gp_core->IsCpsrCbitSet()); break;
        case 1:  assert(false == gp_core->IsCpsrCbitSet()); break;
        default: assert(!"Should not reach here."); break;
        }
        
        if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
        {
          assert(true == gp_core->IsCpsrVbitSet());
        }
        else
        {
          assert(false == gp_core->IsCpsrVbitSet());
        }
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // ADD or ADDS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<ADD,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_ADD;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->Rn());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t al_result;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
        asm volatile ("addl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->Rn()), "1" (dpi->SftOperand())
                      : "cc");
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() + dpi->SftOperand();
      }
#else
      dpi->op1 = dpi->Rn();
      dpi->op2 = dpi->SftOperand();
      dpi->result = dpi->op1 + dpi->op2;
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      al_result = al_op1 + al_op2;
      
      assert(gp_core->rCurReg(dpi->RdNum()) == static_cast<uint32_t>(al_result));
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      if (UPDATE_STATUS == is_update_status)
      {
        uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
        al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
        
        switch (al_carry)
        {
        case 0:  assert(false == gp_core->IsCpsrCbitSet()); break;
        case 1:  assert(true == gp_core->IsCpsrCbitSet()); break;
        default: assert(!"Should not reach here."); break;
        }
        
        if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
        {
          assert(true == gp_core->IsCpsrVbitSet());
        }
        else
        {
          assert(false == gp_core->IsCpsrVbitSet());
        }
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // ADC or ADCS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<ADC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_ADC;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->Rn());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t al_result;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
        if (true == gp_core->IsCpsrCbitSet())
        {
          asm volatile ("stc\n\t"
                        "adcl %2, %3\n\t"
                        "pushfl\n\t"
                        "popl %0"
                        : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                        : "r" (dpi->Rn()), "1" (dpi->SftOperand())
                        : "cc");
        }
        else
        {
          asm volatile ("addl %2, %3\n\t"
                        "pushfl\n\t"
                        "popl %0"
                        : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                        : "r" (dpi->Rn()), "1" (dpi->SftOperand())
                        : "cc");
        }
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() + dpi->SftOperand() + (gp_core->IsCpsrCbitSet() ? 1 : 0);
      }
#else
      dpi->op1 = dpi->Rn();
      dpi->op2 = dpi->SftOperand();
      dpi->result = dpi->op1 + dpi->op2 + ((gp_core->IsCpsrCbitSet()) ? 1 : 0);
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_addon = (gp_core->IsCpsrCbitSet() ? 1 : 0);
      
      al_result = al_op1 + al_op2 + al_addon;
      
      assert(gp_core->rCurReg(dpi->RdNum()) == static_cast<uint32_t>(al_result));
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      if (UPDATE_STATUS == is_update_status)
      {
        uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
        al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
        
        switch (al_carry)
        {
        case 0:  assert(false == gp_core->IsCpsrCbitSet()); break;
        case 1:  assert(true == gp_core->IsCpsrCbitSet()); break;
        default: assert(!"Should not reach here."); break;
        }
        
        if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
        {
          assert(true == gp_core->IsCpsrVbitSet());
        }
        else
        {
          assert(false == gp_core->IsCpsrVbitSet());
        }
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // SBC or SBCS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<SBC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_SBC;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->Rn());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t al_result;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
        if (true == gp_core->IsCpsrCbitSet())
        {
          asm volatile ("subl %2, %3\n\t"
                        "pushfl\n\t"
                        "popl %0"
                        : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                        : "r" (dpi->SftOperand()), "1" (dpi->Rn())
                        : "cc");
        }
        else
        {
          asm volatile ("stc\n\t"
                        "sbbl %2, %3\n\t"
                        "pushfl\n\t"
                        "popl %0"
                        : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                        : "r" (dpi->SftOperand()), "1" (dpi->Rn())
                        : "cc");
        }
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() - dpi->SftOperand() - ((gp_core->IsCpsrCbitSet()) ? 0 : 1);
      }
#else
      dpi->op1 = dpi->Rn();
      dpi->op2 = dpi->SftOperand();
      dpi->result = dpi->op1 - dpi->op2 - ((gp_core->IsCpsrCbitSet()) ? 0 : 1);
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_suboff = ((gp_core->IsCpsrCbitSet()) ? 0 : 1);
      al_result = al_op1 - al_op2 - al_suboff;
      
      assert(gp_core->rCurReg(dpi->RdNum()) == static_cast<uint32_t>(al_result));
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      if (UPDATE_STATUS == is_update_status)
      {
        uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
        al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
        
        switch (al_carry)
        {
        case 0:  assert(true == gp_core->IsCpsrCbitSet()); break;
        case 1:  assert(false == gp_core->IsCpsrCbitSet()); break;
        default: assert(!"Should not reach here."); break;
        }
        
        if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
        {
          assert(true == gp_core->IsCpsrVbitSet());
        }
        else
        {
          assert(false == gp_core->IsCpsrVbitSet());
        }
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // RSC or RSCS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<RSC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_RSC;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->Rn());
      uint64_t al_result;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
        if (true == gp_core->IsCpsrCbitSet())
        {
          asm volatile ("subl %2, %3\n\t"
                        "pushfl\n\t"
                        "popl %0"
                        : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                        : "r" (dpi->Rn()), "1" (dpi->SftOperand())
                        : "cc");
        }
        else
        {
          asm volatile ("stc\n\t"
                        "sbbl %2, %3\n\t"
                        "pushfl\n\t"
                        "popl %0"
                        : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                        : "r" (dpi->Rn()), "1" (dpi->SftOperand())
                        : "cc");
        }
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->SftOperand() - dpi->Rn() - ((gp_core->IsCpsrCbitSet()) ? 0 : 1);
      }
#else
      dpi->op1 = dpi->SftOperand();
      dpi->op2 = dpi->Rn();
      dpi->result = dpi->op1 - dpi->op2 - ((gp_core->IsCpsrCbitSet()) ? 0 : 1);
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_suboff = ((gp_core->IsCpsrCbitSet()) ? 0 : 1);
      al_result = al_op1 - al_op2 - al_suboff;
      
      assert(gp_core->rCurReg(dpi->RdNum()) == static_cast<uint32_t>(al_result));
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      if (UPDATE_STATUS == is_update_status)
      {
        uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
        al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
        
        switch (al_carry)
        {
        case 0:  assert(true == gp_core->IsCpsrCbitSet()); break;
        case 1:  assert(false == gp_core->IsCpsrCbitSet()); break;
        default: assert(!"Should not reach here."); break;
        }
        
        if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
        {
          assert(true == gp_core->IsCpsrVbitSet());
        }
        else
        {
          assert(false == gp_core->IsCpsrVbitSet());
        }
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // TST
    
    template<typename mode,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<TST,
                             mode,
                             UPDATE_STATUS,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
      assert(0 == (dpi->m_inst.mRawData & 0xF000));
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_TST;
#endif
      
#if ENABLE_COND_CODES_ASM
      asm volatile ("testl %1, %2\n\t"
                    "pushfl\n\t"
                    "popl %0"
                    : "=&r" (dpi->m_eflags)
                    : "r" (dpi->SftOperand()), "r" (dpi->Rn())
                    : "cc");
#else
      dpi->result = dpi->Rn() & dpi->SftOperand();
#endif
      
      dpi->Finalize();
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // TEQ
    
    template<typename mode,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<TEQ,
                             mode,
                             UPDATE_STATUS,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
      assert(0 == (dpi->m_inst.mRawData & 0xF000));
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_TEQ;
#endif
      
#if ENABLE_COND_CODES_ASM
      asm volatile ("xorl %1, %2\n\t"
                    "pushfl\n\t"
                    "popl %0"
                    : "=&r" (dpi->m_eflags)
                    : "r" (dpi->SftOperand()), "r" (dpi->Rn())
                    : "cc");
#else
      dpi->result = dpi->Rn() ^ dpi->SftOperand();
#endif
      
      dpi->Finalize();
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // CMP
    
    template<typename mode,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<CMP,
                             mode,
                             UPDATE_STATUS,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_CMP;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->Rn());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t al_result;
#endif
      
#if ENABLE_COND_CODES_ASM
      asm volatile ("cmpl %1, %2\n\t"
                    "pushfl\n\t"
                    "popl %0"
                    : "=&r" (dpi->m_eflags)
                    : "r" (dpi->SftOperand()), "r" (dpi->Rn())
                    : "cc");
#else
      dpi->op1 = dpi->Rn();
      dpi->op2 = dpi->SftOperand();
      dpi->result = dpi->op1 - dpi->op2;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      al_result = al_op1 - al_op2;
      
#if !ENABLE_COND_CODES_ASM
      assert(dpi->result == static_cast<uint32_t>(al_result));
#endif
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
      al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
      
      switch (al_carry)
      {
      case 0:  assert(true == gp_core->IsCpsrCbitSet()); break;
      case 1:  assert(false == gp_core->IsCpsrCbitSet()); break;
      default: assert(!"Should not reach here."); break;
      }
      
      if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
      {
        assert(true == gp_core->IsCpsrVbitSet());
      }
      else
      {
        assert(false == gp_core->IsCpsrVbitSet());
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // CMN
    
    template<typename mode,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<CMN,
                             mode,
                             UPDATE_STATUS,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_CMN;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint64_t const al_op1 = static_cast<int32_t>(dpi->Rn());
      uint64_t const al_op2 = static_cast<int32_t>(dpi->SftOperand());
      uint64_t al_result;
#endif
      
#if ENABLE_COND_CODES_ASM
      asm volatile ("addl %1, %2\n\t"
                    "pushfl\n\t"
                    "popl %0"
                    : "=&r" (dpi->m_eflags)
                    : "r" (dpi->SftOperand()), "r" (dpi->Rn())
                    : "cc");
#else
      dpi->op1 = dpi->Rn();
      dpi->op2 = dpi->SftOperand();
      dpi->result = dpi->op1 + dpi->op2;
#endif
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      al_result = al_op1 + al_op2;
      
#if !ENABLE_COND_CODES_ASM
      assert(dpi->result == static_cast<uint32_t>(al_result));
#endif
#endif
      
      dpi->Finalize();
      
#if COMPARE_TO_ALTERNATIVE_DPI | CHECK_ASM_CORRECT
      uint32_t al_carry = (static_cast<uint32_t>(al_op1) >> 31) ^ (static_cast<uint32_t>(al_op2) >> 31);
      al_carry ^= (static_cast<uint32_t>(al_result >> 32) & 0x1);
      
      switch (al_carry)
      {
      case 0:  assert(false == gp_core->IsCpsrCbitSet()); break;
      case 1:  assert(true == gp_core->IsCpsrCbitSet()); break;
      default: assert(!"Should not reach here."); break;
      }
      
      if (((al_result >> 31) & 0x1) != ((al_result >> 32) & 0x1))
      {
        assert(true == gp_core->IsCpsrVbitSet());
      }
      else
      {
        assert(false == gp_core->IsCpsrVbitSet());
      }
#endif
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // ORR or ORRS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<ORR,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_ORR;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
#if CHECK_ASM_CORRECT
        uint32_t const al_op1 = dpi->Rn();
        uint32_t const al_op2 = dpi->SftOperand();
#endif
        
        asm volatile ("orl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->SftOperand()), "1" (dpi->Rn())
                      : "cc");
        
#if CHECK_ASM_CORRECT
        assert(gp_core->rCurReg(dpi->RdNum()) == (al_op1 | al_op2));
#endif
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() | dpi->SftOperand();
      }
#else
      dpi->result = dpi->Rn() | dpi->SftOperand();
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // MOV or MOVS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<MOV,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
      assert(0 == (dpi->m_inst.mRawData & 0xF0000));
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_MOV;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
#if CHECK_ASM_CORRECT
        uint32_t const al_op1 = dpi->SftOperand();
#endif
        
        // :NOTE: Wei 2004-Jul-06:
        //
        // The MOV instruction of IA32 doesn't change its EFLAGS,
        // however, the MOV instruction of ARM does change its status flags.
        //
        // Thus I have to use 'addl xx, 0' of IA32 to emulate ARM's MOV instruction.
        asm volatile ("addl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->SftOperand()), "1" (0)
                      : "cc");
        
#if CHECK_ASM_CORRECT
        assert(gp_core->rCurReg(dpi->RdNum()) == al_op1);
#endif
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->SftOperand();
      }
#else
      dpi->result = dpi->SftOperand();
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // BIC or BICS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<BIC,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_BIC;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
#if CHECK_ASM_CORRECT
        uint32_t const al_op1 = dpi->Rn();
        uint32_t const al_op2 = dpi->SftOperand();
#endif
        
        asm volatile ("notl %2\n\t"
                      "andl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->SftOperand()), "1" (dpi->Rn())
                      : "cc");
        
#if CHECK_ASM_CORRECT
        assert(gp_core->rCurReg(dpi->RdNum()) == (al_op1 & (~al_op2)));
#endif
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = dpi->Rn() & (~dpi->SftOperand());
      }
#else
      dpi->result = dpi->Rn() & (~dpi->SftOperand());
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
    }
    
    /////////////////////////////////////////////////////////////////////////////
    // MVN or MVNS
    
    template<typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInternal_Helper(InstDPI<MVN,
                             mode,
                             is_update_status,
                             T_usage,
                             update_status_category_id,
                             operand_number_category_id> * const dpi)
    {
      assert(0 == (dpi->m_inst.mRawData & 0xF0000));
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_MVN;
#endif
      
#if ENABLE_COND_CODES_ASM
      if (UPDATE_STATUS == is_update_status)
      {
#if CHECK_ASM_CORRECT
        uint32_t const al_op1 = dpi->SftOperand();
#endif
        
        // :NOTE: Wei 2004-Jul-06:
        //
        // The MOV instruction of IA32 doesn't change its EFLAGS,
        // however, the MOV instruction of ARM does change its status flags.
        //
        // Thus I have to use 'addl xx, 0' of IA32 to emulate ARM's MOV instruction.
        asm volatile ("notl %2\n\t"
                      "addl %2, %3\n\t"
                      "pushfl\n\t"
                      "popl %0"
                      : "=&r" (dpi->m_eflags), "=r" (gp_core->rCurReg(dpi->RdNum()))
                      : "r" (dpi->SftOperand()), "1" (0)
                      : "cc");
        
#if CHECK_ASM_CORRECT
        assert(gp_core->rCurReg(dpi->RdNum()) == (~al_op1));
#endif
      }
      else
      {
        gp_core->rCurReg(dpi->RdNum()) = ~dpi->SftOperand();
      }
#else
      dpi->result = ~dpi->SftOperand();
      
      gp_core->rCurReg(dpi->RdNum()) = dpi->result;
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(dpi->RdNum());
#endif
      
      dpi->Finalize();
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  OperationInternal()
  {
    OperationInternal_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation with inst recall
  
  namespace
  {
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInstRecall_Helper(InstDPI<opcode,
                               ImmDPI,
                               is_update_status,
                               T_usage,
                               update_status_category_id,
                               operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstTypeTag = IMM;
      
      gp_core->CurrentInstRecallEntry().mRdNum = dpi->RdNum();
      
      gp_core->CurrentInstRecallEntry().mRnNum = dpi->RnNum();
      gp_core->CurrentInstRecallEntry().mRn = dpi->Rn();
      
      gp_core->CurrentInstRecallEntry().mSftOperand = dpi->SftOperand();
#endif
      
      dpi->OperationInternal();
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInstRecall_Helper(InstDPI<opcode,
                               RegImmSftDPI,
                               is_update_status,
                               T_usage,
                               update_status_category_id,
                               operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstTypeTag = REG_IMM_SFT;
      
      gp_core->CurrentInstRecallEntry().mRdNum = dpi->RdNum();
      
      gp_core->CurrentInstRecallEntry().mRnNum = dpi->RnNum();
      gp_core->CurrentInstRecallEntry().mRn = dpi->Rn();
      
      gp_core->CurrentInstRecallEntry().mRmNum = dpi->RmNum();
      gp_core->CurrentInstRecallEntry().mRm = dpi->Rm();
      
      gp_core->CurrentInstRecallEntry().mSftOperand = dpi->SftOperand();
#endif
      
      dpi->OperationInternal();
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    OperationInstRecall_Helper(InstDPI<opcode,
                               RegRegSftDPI,
                               is_update_status,
                               T_usage,
                               update_status_category_id,
                               operand_number_category_id> * const dpi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstTypeTag = REG_REG_SFT;
      
      gp_core->CurrentInstRecallEntry().mRdNum = dpi->RdNum();
      
      gp_core->CurrentInstRecallEntry().mRnNum = dpi->RnNum();
      gp_core->CurrentInstRecallEntry().mRn = dpi->Rn();
      
      gp_core->CurrentInstRecallEntry().mRmNum = dpi->RmNum();
      gp_core->CurrentInstRecallEntry().mRm = dpi->Rm();
      
      gp_core->CurrentInstRecallEntry().mRsNum = dpi->RsNum();
      gp_core->CurrentInstRecallEntry().mRs = gp_core->CurReg(dpi->RsNum());
      
      gp_core->CurrentInstRecallEntry().mSftOperand = dpi->SftOperand();
#endif
      
      dpi->OperationInternal();
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  OperationInstRecall()
  {
    OperationInstRecall_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  Operation()
  {
#if CHECK_UNPREDICTABLE
    CheckRdRequirementByMode();
    CheckRdRequirementByOpcode();
#endif
    
#if ENABLE_INST_RECALL
    gp_core->template RecordUpdateStatus<is_update_status>();
#endif
    
    OperationInstRecall();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // check modify PC
  
  namespace
  {
    template<typename opcode,
             typename mode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline ExecResultEnum
    check_modify_pc_helper(InstDPI<opcode,
                           mode,
                           is_update_status,
                           T_usage,
                           update_status_category_id,
                           operand_number_category_id> const * const dpi)
    {
      // :NOTE: Wei 2004-Sep-20:
      //
      // Only 'RegRegSftDPI' variant in DPI instructions should NOT specify
      // CR_PC as the Rd register.
      
      if (CR_PC == static_cast<CoreRegNum>(dpi->m_inst.mImmDPI.mRd))
      {
        return ER_NORMAL_MODIFY_PC;
      }
      else
      {
        return ER_NORMAL;
      }
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline ExecResultEnum
    check_modify_pc_helper(InstDPI<opcode,
                           RegRegSftDPI,
                           is_update_status,
                           T_usage,
                           update_status_category_id,
                           operand_number_category_id> const * const /* dpi */)
    {
      return ER_NORMAL;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Process
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline ExecResultEnum
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  Process(Inst const inst)
  {
    m_inst = inst;
    
    Operation();
    
    switch (T_usage)
    {
    case USAGE_NORMAL:
      return ER_NORMAL;
      
    case USAGE_THREADED_CODE:
      // :NOTE: Wei 2004-Aug-09:
      //
      // Doesn't have ability to generate an exception.
      return check_modify_pc_helper(this);
      
    default:
      assert(!"Should not reach here.");
      return ER_NORMAL;
    }
  }
}
