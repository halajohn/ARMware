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
  // Rn number
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    RnNum_Helper(InstLSH<ImmLSH,
                 addressing_mode,
                 direction,
                 is_signed,
                 data_width_enum,
                 is_load,
                 T_usage> const * const lsh)
    {
      return static_cast<CoreRegNum>(lsh->mInst.mImmLSH.mRn);
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    RnNum_Helper(InstLSH<RegLSH,
                 addressing_mode,
                 direction,
                 is_signed,
                 data_width_enum,
                 is_load,
                 T_usage> const * const lsh)
    {
      return static_cast<CoreRegNum>(lsh->mInst.mRegLSH.mRn);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreRegNum
  InstLSH<mode,
          addressing_mode,
          direction,
          is_signed,
          data_width_enum,
          is_load,
          T_usage>::
  RnNum() const
  {
    return RnNum_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm number
  //
  // ImmLSH doesn't have Rm field
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    RmNum_Helper(InstLSH<ImmLSH,
                 addressing_mode,
                 direction,
                 is_signed,
                 data_width_enum,
                 is_load,
                 T_usage> const * const /* lsh */)
    {
      assert(!"Should not reach here.");
      return static_cast<CoreRegNum>(0);
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    RmNum_Helper(InstLSH<RegLSH,
                 addressing_mode,
                 direction,
                 is_signed,
                 data_width_enum,
                 is_load,
                 T_usage> const * const lsh)
    {
      return static_cast<CoreRegNum>(lsh->mInst.mRegLSH.mRm);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreRegNum
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  RmNum() const
  {
    return RmNum_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rd
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    RdNum_Helper(InstLSH<ImmLSH,
                 addressing_mode,
                 direction,
                 is_signed,
                 data_width_enum,
                 is_load,
                 T_usage> const * const lsh)
    {
      return static_cast<CoreRegNum>(lsh->mInst.mImmLSH.mRd);
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    RdNum_Helper(InstLSH<RegLSH,
                 addressing_mode,
                 direction,
                 is_signed,
                 data_width_enum,
                 is_load,
                 T_usage> const * const lsh)
    {
      return static_cast<CoreRegNum>(lsh->mInst.mRegLSH.mRd);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreRegNum
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  RdNum() const
  {
    return RdNum_Helper(this);
  }
  
#if CHECK_UNPREDICTABLE
  /////////////////////////////////////////////////////////////////////////////
  // Rn requirement
  
  namespace
  {
    // Used for pre-index & post-index addressing mode
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckRnRequirement_Helper(InstLSH<mode,
                              addressing_mode,
                              direction,
                              is_signed,
                              data_width_enum,
                              is_load,
                              T_usage> const * const lsh)
    {
      // :ARM-DDI-0100E: p.A2-17: Wei 2004-Feb-09:
      //
      // All instructions that allow this to be specified have UNPREDICTABLE results
      // if base register writeback is specified and the base register is the PC,
      // so only general-purpose registers other than the PC can legitimately be
      // modified in this way.
      //
      // :ARM-DDI-0100E: p.A5-41: Wei 2004-Feb-09:
      //
      // Specifying R15 as register Rn has UNPREDICTABLE results.
      assert(lsh->RnNum() != CR_PC);
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckRnRequirement_Helper(InstLSH<mode,
                              OFFSET,
                              direction,
                              is_signed,
                              data_width_enum,
                              is_load,
                              T_usage> const * const /* lsh */)
    { }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  CheckRnRequirement() const
  {
    return CheckRnRequirement_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm Requirement
  //
  // ImmLSH doesn't have Rm field
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckRmRequirement_Helper(InstLSH<ImmLSH,
                              addressing_mode,
                              direction,
                              is_signed,
                              data_width_enum,
                              is_load,
                              T_usage> const * const /* lsh */)
    {
      assert(!"Should not reach here.");
    }
    
    // Used for pre-index & post-index addressing mode
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckRmRequirement_Helper(InstLSH<RegLSH,
                              addressing_mode,
                              direction,
                              is_signed,
                              data_width_enum,
                              is_load,
                              T_usage> const * const lsh)
    {
      // :ARM-DDI-0100E: p.A5-43: Wei 2004-Feb-09:
      //
      // Specifying R15 as register Rm or Rn has UNPREDICTABLE results.
      // If the same register is specified for Rn and Rm, the result is UNPREDICTABLE.
      assert(lsh->RmNum() != CR_PC);
      assert(lsh->RmNum() != lsh->RnNum());
    }
    
    // Special case for offset addressing mode
    template<Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckRmRequirement_Helper(InstLSH<RegLSH,
                              OFFSET,
                              direction,
                              is_signed,
                              data_width_enum,
                              is_load,
                              T_usage> const * const lsh)
    {
      // :ARM-DDI-0100E: Wei 2004-Feb-09: Wei 2004-Feb-09:
      //
      // Specifying R15 as register Rm has UNPREDICTABLE results.
      assert(lsh->RmNum() != CR_PC);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  CheckRmRequirement() const
  {
    CheckRmRequirement_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Check Rd requirement
  //
  // In all load store halfword and load signed byte instructions,
  // Rd can't be PC
  
  namespace
  {
    // Used for pre-index & post-index addressing mode
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckRdRequirement_Helper(InstLSH<mode,
                              addressing_mode,
                              direction,
                              is_signed,
                              data_width_enum,
                              is_load,
                              T_usage> const * const lsh)
    {
      // :ARM-DDI-0100E: p.A4-44: LDRH: Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-46: LDRSB: Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-48: LDRSH: Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-94: STRH: Wei 2004-Feb-09:
      //
      // If R15 is specified for <Rd>, the result is UNPREDICTABLE.
      assert(lsh->RdNum() != CR_PC);
      
      // :ARM-DDI-0100E: p.A4-45: LDRH: Wei 2003-Nov-6:
      // :ARM-DDI-0100E: p.A4-47: LDRSB: Wei 2003-Nov-6:
      // :ARM-DDI-0100E: p.A4-49: LDRSH: Wei 2003-Nov-6:
      // :ARM-DDI-0100E: p.A4-95: STRH: Wei 2003-Nov-6:
      //
      // If <addressing_mode> specifies base register writeback,
      // and the same register is specified for <Rd> and <Rn>,
      // the results are UNPREDICTABLE.
      assert(lsh->RdNum() != lsh->RnNum());
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckRdRequirement_Helper(InstLSH<mode,
                              OFFSET,
                              direction,
                              is_signed,
                              data_width_enum,
                              is_load,
                              T_usage> const * const lsh)
    {
      // :ARM-DDI-0100E: p.A4-44: LDRH: Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-46: LDRSB: Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-48: LDRSH: Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-94: STRH: Wei 2004-Feb-09:
      //
      // If R15 is specified for <Rd>, the result is UNPREDICTABLE.
      assert(lsh->RdNum() != CR_PC);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  CheckRdRequirement() const
  {
    CheckRdRequirement_Helper(this);
  }
  
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // Rn
  
  namespace
  {
    // Used for pre-index & post-index addressing mode
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    Rn_Helper(InstLSH<mode,
              addressing_mode,
              direction,
              is_signed,
              data_width_enum,
              is_load,
              T_usage> const * const lsh)
    {
#if CHECK_UNPREDICTABLE
      lsh->CheckRnRequirement();
#endif
      
      // Rn number shouldn't be CR_PC in these 2 modes.
      return (gp_core->CurReg(lsh->RnNum()));
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    Rn_Helper(InstLSH<mode,
              OFFSET,
              direction,
              is_signed,
              data_width_enum,
              is_load,
              T_usage> const * const lsh)
    {
#if CHECK_UNPREDICTABLE
      lsh->CheckRnRequirement();
#endif
      
      // :ARM-DDI-0100E: p.A5-36: Wei 2003-Nov-6:
      //
      // In offset addressing, if R15 is specified as register Rn,
      // the value used is the address of the instruction plus 8.
      return ((CR_PC == lsh->RnNum())
              ? (gp_core->ReadPC())
              : (gp_core->CurReg(lsh->RnNum())));
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  Rn() const
  {
    return Rn_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Offset
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    Offset_Helper(InstLSH<ImmLSH,
                  addressing_mode,
                  direction,
                  is_signed,
                  data_width_enum,
                  is_load,
                  T_usage> const * const lsh)
    {
      return ((lsh->mInst.mImmLSH.mImmL) | ((lsh->mInst.mImmLSH.mImmH) << 4));
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    Offset_Helper(InstLSH<RegLSH,
                  addressing_mode,
                  direction,
                  is_signed,
                  data_width_enum,
                  is_load,
                  T_usage> const * const lsh)
    {
#if CHECK_UNPREDICTABLE
      lsh->CheckRmRequirement();
#endif
      
      // :NOTE: Wei 2003-Dec-03:
      //
      // Because all of the RegLSH instrunctions claim that Rm != CR_PC,
      // thus we can avoid the special +8 statement for PC here.
      return gp_core->CurReg(lsh->RmNum());
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  Offset() const
  {
    return Offset_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // address
  
  namespace
  {
    template<typename mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    Address_Helper(InstLSH<mode,
                   OFFSET,
                   direction,
                   is_signed,
                   data_width_enum,
                   is_load,
                   T_usage> const * const lsh)
    {
      uint32_t const address = Operation<direction>(lsh->Rn(), lsh->Offset());
      
      return address;
    }
    
    template<typename mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    Address_Helper(InstLSH<mode,
                   PRE_IDX,
                   direction,
                   is_signed,
                   data_width_enum,
                   is_load,
                   T_usage> const * const lsh)
    {
      uint32_t const address = Operation<direction>(lsh->Rn(), lsh->Offset());
      
      // Pre-indexing means write-back
      gp_core->rCurReg(lsh->RnNum()) = address;
      
      return address;
    }
    
    template<typename mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    Address_Helper(InstLSH<mode,
                   POST_IDX,
                   direction,
                   is_signed,
                   data_width_enum,
                   is_load,
                   T_usage> const * const lsh)
    {
      uint32_t const address = lsh->Rn();
      
      // Post-indexing means write-back
      gp_core->rCurReg(lsh->RnNum()) = Operation<direction>(lsh->Rn(), lsh->Offset());
      
      return address;
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  Address() const
  {
    return Address_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation internal
  
  namespace
  {
    // LDRH or LDRSH or LDRSB
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             UsageEnum T_usage>
    inline void
    OperationInternal_Helper(InstLSH<mode,
                             addressing_mode,
                             direction,
                             is_signed,
                             data_width_enum,
                             LOAD,
                             T_usage> const * const lsh,
                             bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_LDRH;
#endif
      
#if CHECK_UNPREDICTABLE
      lsh->CheckRdRequirement();
#endif
      
      // :NOTE: Wei 2004-Feb-15:
      //
      // Because Address() have a side-effect: the base register may be updated,
      // thus I shouldn't call Address() more than once in OperationInternal().
      uint32_t const address = lsh->Address();
      
      // :NOTE: Wei 2004-May-13:
      //
      // Because address will not be modified in CheckAlign<HALF_WORD> or CheckAlign<BYTE>
      // thus I just ignore the return value (the address used by memory accessing).
      // (i.e. return value == the 'address' argument)
      
      /* uint32_t const real_address = */(void)(gp_core->template CheckAlign<data_width_enum>(address, exception_occur));
      
      if (true == exception_occur)
      {
        return;
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mVaddr[lsh->RdNum()] = address;
      gp_core->CurrentInstRecallEntry().mCurrentRegNum = lsh->RdNum();
#endif
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if get_data() throws an exception,
      // 'value' will not be modified.
      //
      // However, because of 'value' is a local variable,
      // thus I choose overwrite its value whether an exception occurs or not.
      typename DataWidthTraits<data_width_enum>::UnsignedType const value =
        gp_core->template get_data<data_width_enum, DATA>(address, gp_core->CurrentMode(), exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
      if (SIGNED == is_signed)
      {
        gp_core->rCurReg(lsh->RdNum()) =
          static_cast<uint32_t>(static_cast<int32_t>(static_cast<typename DataWidthTraits<data_width_enum>::SignedType>(value)));
      }
      else
      {
        gp_core->rCurReg(lsh->RdNum()) = static_cast<uint32_t>(value);
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(lsh->RdNum());
#endif
    }
    
    // STRH
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             UsageEnum T_usage>
    inline void
    OperationInternal_Helper(InstLSH<mode,
                             addressing_mode,
                             direction,
                             UNSIGNED,
                             HALF_WORD,
                             STORE,
                             T_usage> const * const lsh,
                             bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_STRH;
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(lsh->RdNum());
#endif
      
#if CHECK_UNPREDICTABLE
      lsh->CheckRdRequirement();
#endif
      
      // :NOTE: Wei 2003-Dec-03:
      //
      // Because all kinds of LSH instructions claim Rd != PC,
      // thus we can avoid the special +8 statement here.
      
      DataWidthTraits<HALF_WORD>::UnsignedType const store_value =
        static_cast<DataWidthTraits<HALF_WORD>::UnsignedType>(
          gp_core->CurReg(lsh->RdNum()) & 0xFFFF
                                                              );
      
      // :NOTE: Wei 2004-Feb-15:
      //
      // Because Address() have a side-effect: the base register may be updated,
      // thus I shouldn't call Address() more than once in OperationInternal().
      uint32_t const address = lsh->Address();
      
      // :NOTE: Wei 2004-May-13:
      //
      // Because address will not be modified in CheckAlign<HALF_WORD> or CheckAlign<BYTE>
      // thus I just ignore the return value (the address used by memory accessing).
      // (i.e. return value == the 'address' argument)
      
      /* uint32_t const real_address = */(void)(gp_core->template CheckAlign<HALF_WORD>(address, exception_occur));
      
      if (true == exception_occur)
      {
        return;
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mVaddr[lsh->RdNum()] = address;
      gp_core->CurrentInstRecallEntry().mCurrentRegNum = lsh->RdNum();
#endif
      
      gp_core->template put_data<HALF_WORD>(address, store_value,
                                                gp_core->CurrentMode(),
                                                exception_occur);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  OperationInternal(bool &exception_occur) const
  {
    OperationInternal_Helper(this, exception_occur);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation internal by addressing mode
  
  namespace
  {
    // Used for pre-index & post-index addressing mode
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    OperationInternalByAddressingMode_Helper(InstLSH<mode,
                                             addressing_mode,
                                             direction,
                                             is_signed,
                                             data_width_enum,
                                             is_load,
                                             T_usage> const * const lsh,
                                             bool &exception_occur)
    {
      uint32_t const orig_rn_value = lsh->Rn();
      
      lsh->OperationInternal(exception_occur);
      
      if (true == exception_occur)
      {
        gp_core->rCurReg(lsh->RnNum()) = orig_rn_value;
      }
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    OperationInternalByAddressingMode_Helper(InstLSH<mode,
                                             OFFSET,
                                             direction,
                                             is_signed,
                                             data_width_enum,
                                             is_load,
                                             T_usage> const * const lsh,
                                             bool &exception_occur)
    {
      lsh->OperationInternal(exception_occur);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  OperationInternalByAddressingMode(bool &exception_occur) const
  {
    OperationInternalByAddressingMode_Helper(this, exception_occur);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  Operation(bool &exception_occur) const
  {
#if ENABLE_INST_RECALL
    gp_core->CurrentInstRecallEntry().mRdNum = RdNum();
#endif
    
    OperationInternalByAddressingMode(exception_occur);
    
    if (true == exception_occur)
    {
      return;
    }
    
    // :NOTE: Wei 2003-Dec-03:
    //
    // Because CR_PC can not be the destination register,
    // we can increment PC right here.
    gp_core->IncrePC();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Process
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline ExecResultEnum
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  Process(Inst const inst)
  {
    mInst = inst;
    bool exception_occur = false;
    
#if CHECK_CHUNK_CODE
    assert(inst.mRawData == gp_core->get_curr_inst());
#endif
    
#if DUMP_CPU_INFO_MORE_DETAIL
    if (STORE == is_load)
    {
      if (true == gp_core->is_exec_in_chunk())
      {
        g_log_file << "--> <LSH> "
                   << std::endl;
      }
      
      gp_core->dump_cpu_info<false>();
    }
#endif
    
    Operation(exception_occur);
    
    if (true == exception_occur)
    {
      gp_core->set_data_abort();
      
      return ER_EXCEPTION;
    }
    else
    {
      if (USAGE_THREADED_CODE == T_usage)
      {
        if (STORE == is_load)
        {
          if (0 == gp_core->curr_chunk())
          {
            return ER_CHUNK_DISAPPEAR;
          }
        }
        else
        {
          assert(gp_core->curr_chunk() != 0);
        }
      }
      
      // :NOTE: Wei 2004-Sep-20:
      //
      // Because...
      // (1) If specify 'base-register-writeback', then the base register
      //     should not be CR_PC.
      // (2) Rd register should not be CR_PC in all LSH instructions.
      //
      // thus, even in USAGE_THREADED_CODE mode,
      // this function will return 'ER_NORMAL' here.
      return ER_NORMAL;
    }
  }
}
