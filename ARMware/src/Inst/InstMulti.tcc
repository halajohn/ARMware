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
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline CoreRegNum
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  RmNum() const
  {
    return static_cast<CoreRegNum>(mInst.mMulti.mRm);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline CoreRegNum
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  RsNum() const
  {
    return static_cast<CoreRegNum>(mInst.mMulti.mRs);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline CoreRegNum
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  RdNum() const
  {
    return static_cast<CoreRegNum>(mInst.mMulti.mRd);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline CoreRegNum
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  RnNum() const
  {
    return static_cast<CoreRegNum>(mInst.mMulti.mRn);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline CoreRegNum
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  RdHiNum() const
  {
    return static_cast<CoreRegNum>(mInst.mMulti.mRd);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline CoreRegNum
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  RdLoNum() const
  {
    return static_cast<CoreRegNum>(mInst.mMulti.mRn);
  }
  
#if CHECK_UNPREDICTABLE
  /////////////////////////////////////////////////////////////////////////////
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline void
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  CheckRequirement() const
  {
    // the same part of MUL, MLA, Long multi
    assert(RdNum() != CR_PC);
    assert(RmNum() != CR_PC);
    assert(RsNum() != CR_PC);
    assert(RmNum() != RdNum());
    
    if (NORMAL == is_long)
    {
      if (ACCUMULATE == is_accumulate)
      {
        // MLA
        assert(RnNum() != CR_PC);
      }
    }
    else
    {
      // Long multi
      assert(RnNum() != CR_PC);
      assert(RnNum() != RmNum());
      assert(RdNum() != RnNum());
    }
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  
  namespace
  {
    template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate>
    inline void
    UpdateStatusRegister_Helper(InstMulti<is_long, is_signed, is_accumulate, NO_UPDATE_STATUS> const * const /* multi */)
    { }
    
    template<IsSigned is_signed, IsAccumulate is_accumulate>
    inline void
    UpdateStatusRegister_Helper(InstMulti<NORMAL, is_signed, is_accumulate, UPDATE_STATUS> const * const multi)
    {
      // V & C bit unaffected
      gp_core->clear_CPSR_NZ_bit();
      
      // Z bit
      if (0 == multi->mLoResult)
      {
        gp_core->set_CPSR_Z_bit();
      }
      else
      {
        // N bit
        if (IsNegative(multi->mLoResult))
        {
          gp_core->set_CPSR_N_bit();
        }
      }
      
      // :ARM-DDI-0100E: p.A4-55 for MLAS: Wei 2004-Feb-02:
      // :ARM-DDI-0100E: p.A4-67 for MULS: Wei 2004-Feb-02:
      //
      // These instructions is defined to leave the C flag unchanged
      // in ARM architecture version 5 and above. In earlier versions of the architecture,
      // the value of the C flag was UNPREDICTABLE after these instructions.
    }
    
    template<IsSigned is_signed, IsAccumulate is_accumulate>
    inline void
    UpdateStatusRegister_Helper(InstMulti<LONG, is_signed, is_accumulate, UPDATE_STATUS> const * const multi)
    {
      // V & C bit unaffected
      gp_core->clear_CPSR_NZ_bit();
      
      // Z bit
      if (0 == multi->mResult)
      {
        gp_core->set_CPSR_Z_bit();
      }
      else
      {
        // N bit
        if (IsNegative(multi->mHiResult))
        {
          gp_core->set_CPSR_N_bit();
        }
      }
      
      // :ARM-DDI-0100E: p.A4-81  for SMULLS: Wei 2003-Nov-6:
      // :ARM-DDI-0100E: p.A4-79  for SMLALS: Wei 2003-Nov-6:
      // :ARM-DDI-0100E: p.A4-110 for UMLALS: Wei 2003-Nov-6:
      // :ARM-DDI-0100E: p.A4-112 for UMULLS: Wei 2003-Nov-6:
      //
      // These instructions is defined to leave the C and V flags
      // unchanged in ARM architecture version 5 and above. In earlier versions of the
      // architecture, the values of the C and V flags were UNPREDICTABLE after these instructions.
    }
  }
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline void
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  UpdateStatusRegister() const
  {
    UpdateStatusRegister_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline void
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  Finalize() const
  {
    if (NORMAL == is_long)
    {
      gp_core->rCurReg(RdNum()) = mLoResult;
    }
    else
    {
      gp_core->rCurReg(RdHiNum()) = mHiResult;
      gp_core->rCurReg(RdLoNum()) = mLoResult;
    }
    
    UpdateStatusRegister();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  namespace
  {
    // MUL, MULS, MLA, MLAS
    template<IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
    inline void
    OperationInternal_Helper(InstMulti<NORMAL, is_signed, is_accumulate, is_update_status> * const multi)
    {
      if (NO_ACCUMULATE == is_accumulate)
      {
        assert(0 == (multi->mInst.mRawData & 0xF000));
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_MUL;
#endif
      
#if CHECK_UNPREDICTABLE
      multi->CheckRequirement();
#endif
      
      multi->mLoResult = (gp_core->CurReg(multi->RmNum()) * gp_core->CurReg(multi->RsNum()));
      
      if (ACCUMULATE == is_accumulate)
      {
        multi->mLoResult += gp_core->CurReg(multi->RnNum());
      }
      
      multi->Finalize();
    }
    
    // SMULL, SMULLS, SMLAL, SMLALS
    template<IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
    inline void
    OperationInternal_Helper(InstMulti<LONG, SIGNED, is_accumulate, is_update_status> * const multi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_SMLAL;
#endif
      
#if CHECK_UNPREDICTABLE
      multi->CheckRequirement();
#endif
      
      multi->mResult = (static_cast<int64_t>(static_cast<int32_t>(gp_core->CurReg(multi->RmNum()))) *
                        static_cast<int64_t>(static_cast<int32_t>(gp_core->CurReg(multi->RsNum()))));
      
      if (ACCUMULATE == is_accumulate)
      {
        multi->mResult = (static_cast<int64_t>(multi->mResult) +
                          static_cast<int64_t>(static_cast<uint64_t>(gp_core->CurReg(multi->RdLoNum())) +
                                               (static_cast<uint64_t>(gp_core->CurReg(multi->RdHiNum())) << 32)));
      }
      
      multi->mHiResult = static_cast<uint32_t>(multi->mResult >> 32);
      multi->mLoResult = static_cast<uint32_t>(multi->mResult & 0xFFFFFFFF);
      
      multi->Finalize();
    }
    
    // UMULL, UMULLS, UMLAL, UMLALS
    template<IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
    inline void
    OperationInternal_Helper(InstMulti<LONG, UNSIGNED, is_accumulate, is_update_status> * const multi)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_UMLAL;
#endif
      
#if CHECK_UNPREDICTABLE
      multi->CheckRequirement();
#endif
      
      multi->mResult = (static_cast<uint64_t>(gp_core->CurReg(multi->RmNum())) *
                        static_cast<uint64_t>(gp_core->CurReg(multi->RsNum())));
      
      if (ACCUMULATE == is_accumulate)
      {
        multi->mResult += (static_cast<uint64_t>(gp_core->CurReg(multi->RdLoNum())) +
                           (static_cast<uint64_t>(gp_core->CurReg(multi->RdHiNum())) << 32));
      }
      
      multi->mHiResult = static_cast<uint32_t>(multi->mResult >> 32);
      multi->mLoResult = static_cast<uint32_t>(multi->mResult & 0xFFFFFFFF);
      
      multi->Finalize();
    }
  }
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline void
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  OperationInternal()
  {
    OperationInternal_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline void
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  Operation()
  {
#if ENABLE_INST_RECALL
    gp_core->template RecordUpdateStatus<is_update_status>();
#endif
    
    OperationInternal();
    
    // :NOTE: Wei 2003-Dec-03:
    //
    // Because CR_PC can not be the destination register,
    // we can increment PC right here.
    gp_core->IncrePC();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline ExecResultEnum
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  Process(Inst const inst)
  {
    mInst = inst;
    
    Operation();
    
    // :NOTE: Wei 2004-Aug-09:
    //
    // Doesn't have ability to generate an exception.
    //
    // All Rd register in Multi instructions should not be CR_PC.
    return ER_NORMAL;
  }
}
