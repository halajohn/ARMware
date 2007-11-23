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

// Branch instructions includes:
// B, BL, BLX, BX
//

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // signed_immed_24 field
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    Imm_Helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const /* branch */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    Imm_Helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      return static_cast<int32_t>(branch->mInst.mRawData << 8) >> 6;
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline uint32_t
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  Imm() const
  {
    return Imm_Helper(this);
  }
  
#if ENABLE_DYNAMIC_TRANSLATOR || ENABLE_THREADED_CODE
  /////////////////////////////////////////////////////////////////////////////
  // dest imm - counted by instruction count
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline uint32_t
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  dest_imm() const
  {
    // :NOTE: Wei 2004-Aug-31:
    //
    // Because 'Imm()' is counted by 'byte offset',
    // thus, I have to devide it by 4 to count it by 'instruction count'.
    //
    // And, because Imm() returns a value of type 'uint32_t',
    // thus, if I want to get a correct instruction count, I have to type casting to 'int32_t'
    // to let 'sign-extension' taking effect.
    return static_cast<int32_t>(Imm()) >> 2;
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm number
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline CoreRegNum
    RmNum_Helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const /* branch */)
    {
      assert(!"Should not reach here.");
      return static_cast<CoreRegNum>(0);
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline CoreRegNum
    RmNum_Helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      return static_cast<CoreRegNum>(branch->mInst.mRegBranch.mRm);
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline CoreRegNum
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  RmNum() const
  {
    return RmNum_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    Rm_Helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const /* branch */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    // Used in BX
    template<ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    Rm_Helper(InstBranch<RegBranch, NO_LINK, exchange_to_thumb, T_usage> const * const branch)
    {
      // :ARM-DDI-0100E: p.A4-19: Wei 2004-Feb-12:
      //
      // Register 15 can be specified for <Rm>, but doing so is discouraged.
      //
      // In a BX R15 instruction, R15 is read as normal for ARM code, that is,
      // it is the address of the BX instruction itself plus 8. The result is
      // to branch to the second following word, executing in ARM state. This
      // is precisely the same effect that would have been obtained if a B instruction
      // with an offset field of 0 had been executed, or an ADD PC,PC,#0 or
      // MOV PC,PC instruction. In new code, use these instructions in preference
      // to the more complex BX PC instruction.
      return ((CR_PC == branch->RmNum())
              ? gp_core->ReadPC()
              : gp_core->CurReg(branch->RmNum()));
    }
    
    // Used in BLX(2)
    template<ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    Rm_Helper(InstBranch<RegBranch, LINK, exchange_to_thumb, T_usage> const * const branch)
    {
      return gp_core->CurReg(branch->RmNum());
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline uint32_t
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  Rm() const
  {
    return Rm_Helper(this);
  }
  
#if CHECK_UNPREDICTABLE
  /////////////////////////////////////////////////////////////////////////////
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    CheckRmRequirement_Helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const /* branch */)
    { }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    CheckRmRequirement_Helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      // Used in BX & BLX(2)
      
      // :NOTE: Wei 2004-Aug-31:
      //
      // If this branch instruction is of register-addressing mode, then this instruction must be
      // BX or BLX_2.
      // Whatever it is, the 'exchange_to_thumb' template variable has to be 'EXCHANGE'.
      assert(EXCHANGE == exchange_to_thumb);
      
      // :ARM-DDI-0100E: p.A4-19: Wei 2004-Feb-12:
      //
      // If Rm[1:0] == 0b10, the result is UNPREDICTABLE,
      // as branches to non word-aligned addresses are impossible in ARM state.
      assert((branch->Rm() & 0x3) != 0x2);
      
      if (LINK == is_link)
      {
        // BLX_2
        
        // :ARM-DDI-0100E: p.A4-18: Wei 2004-Aug-31:
        //
        // If R15 is specified for <Rm>, the results are UNPREDICTABLE.
        assert(branch->RmNum() != CR_PC);
      }
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline void
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  CheckRmRequirement() const
  {
    CheckRmRequirement_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  namespace
  {
    // Used in BLX(1) & B & BL
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    CheckInstBits_Helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const /* branch */)
    {
    }
    
    // Used in BLX(2) & BX
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    CheckInstBits_Helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      assert(0xFFF00 == (branch->mInst.mRawData & 0xFFF00));
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline void
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  CheckInstBits() const
  {
    CheckInstBits_Helper(this);
  }
  
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // Preform link
  
  namespace
  {
    template<typename mode, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    PreformLink_Helper(InstBranch<mode, NO_LINK, exchange_to_thumb, T_usage> const * const /* branch */)
    {
    }
    
    template<typename mode, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    PreformLink_Helper(InstBranch<mode, LINK, exchange_to_thumb, T_usage> const * const /* branch */)
    {
      // :ARM-DDI-0100E: p.A4-10: Wei 2004-Feb-12:
      //
      // LR = address of the instruction after the branch instruction
      gp_core->rCurReg(CR_LR) = gp_core->UsrReg(CR_PC) + 4;
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline void
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  PreformLink() const
  {
    PreformLink_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Preform exchange
  
  namespace
  {
    template<typename mode, IsLink is_link, UsageEnum T_usage>
    inline void
    PreformExchange_Helper(InstBranch<mode, is_link, NO_EXCHANGE, T_usage> const * const /* branch */)
    {
    }
    
    // Used in BX & BLX(2)
    template<IsLink is_link, UsageEnum T_usage>
    inline void
    PreformExchange_Helper(InstBranch<RegBranch, is_link, EXCHANGE, T_usage> const * const branch)
    {
      if (branch->Rm() & 0x1)
      {
        // :NOTE: Wei 2004-May-24:
        //
        // For now, ARMware doesn't support Thumb instruction set.
        assert(!"Should not reach here.");
        
        gp_core->set_CPSR_T_bit();
      }
      else
      {
        gp_core->clear_CPSR_T_bit();
      }
    }
    
    // Used in BLX(1)
    template<IsLink is_link, UsageEnum T_usage>
    inline void
    PreformExchange_Helper(InstBranch<ImmBranch, is_link, EXCHANGE, T_usage> const * const /* branch */)
    {
      // :NOTE: Wei 2004-May-24:
      //
      // For now, ARMware doesn't support Thumb instruction set.
      assert(!"Should not reach here.");
      
      gp_core->set_CPSR_T_bit();
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline void
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  PreformExchange() const
  {
    PreformExchange_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // NewPCAddon
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    NewPCAddon_Helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const /* branch */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    // Used in B, BL
    template<IsLink is_link, UsageEnum T_usage>
    inline uint32_t
    NewPCAddon_Helper(InstBranch<ImmBranch, is_link, NO_EXCHANGE, T_usage> const * const /* branch */)
    {
      return 0;
    }
    
    // Used in BLX(1)
    template<IsLink is_link, UsageEnum T_usage>
    inline uint32_t
    NewPCAddon_Helper(InstBranch<ImmBranch, is_link, EXCHANGE, T_usage> const * const branch)
    {
      // :ARM-DDI-0100E: p.A4-16: Wei 2004-May-19:
      //
      // depends on H bit.
      if (branch->mInst.mRawData & (1 << 24))
      {
        return (1 << 1);
      }
      else
      {
        return 0;
      }
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline uint32_t
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  NewPCAddon() const
  {
    return NewPCAddon_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // NewPC
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    NewPC_Helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      return branch->Rm() & 0xFFFFFFFE;
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    NewPC_Helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      return gp_core->ReadPC() + branch->Imm() + branch->NewPCAddon();
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline uint32_t
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  NewPC() const
  {
#if CHECK_UNPREDICTABLE
    CheckRmRequirement();
#endif
    
    return NewPC_Helper(this);
  }
  
#if ENABLE_DYNAMIC_TRANSLATOR || ENABLE_THREADED_CODE
  /////////////////////////////////////////////////////////////////////////////
  // dest offset - counted by instruction count
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    dest_offset_helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const /* branch */)
    {
      // :NOTE: Wei 2004-Aug-31:
      //
      // I don't want to get the offset when the addressing mode of the branch instruction is
      // register-addressing mode.
      assert(!"Should not reach here.");
      
      return 0;
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline uint32_t
    dest_offset_helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      // :NOTE: Wei 2004-Aug-31:
      //
      // Because dest_offset() is counted by 'instruction count',
      // thus, the value returned by NewPCAddon() has to be devided by 4 to convert to the instruction count.
      //
      // And the values returned by 'NewPCAddon()' are 0 or 2 only.
      // The return value '2' is using to call THUMB codes.
      // However, I didn't consider how to implement facilities to accomplish the THUMB instruction set,
      // thus, 'NewPCAddon()' has to return 0.
      assert(0 == branch->NewPCAddon());
      
      // :NOTE: Wei 2004-Aug-31:
      //
      // the value '2' at the beginning is derived from 'ReadPC()' will return the current instruction pointer
      // plus 8, and I have to devide 8 to get the value '2' to convert to the instruction count.
      return 2 + branch->dest_imm() /* + ( branch->NewPCAddon() >> 2) */;
      //     ^
    }
  }
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline uint32_t
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  dest_offset() const
  {
    return dest_offset_helper(this);
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
  inline void
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  Operation() const
  {
#if CHECK_UNPREDICTABLE
    CheckInstBits();
#endif
    
#if ENABLE_INST_RECALL
    gp_core->CurrentInstRecallEntry().mInstNameTag = INST_BRANCH;
#endif
    
    PreformLink();
    PreformExchange();
    
    // :ARM-DDI-0100E: p.A2-7: Wei 2003-Dec-03:
    //
    // When an instruction reads R15 without breaking any of the restrictions
    // on its use, the value read is the address of the instruction plus 8 bytes.
    //
    // :ARM-DDI-0100E: p.A4-10: Wei 2004-Feb-12:
    //
    // Adding this to the contents of the PC, which contains the address
    // of the branch instruction plus 8.
    //
    // :NOTE: Wei 2004-Jan-08:
    //
    // Because I have already handled the problem of the positive or negative value
    // of immediate field in the BranchImm(), thus I just need to perform the add
    // operation here (i.e. +).
    
#if ENABLE_INST_RECALL
    gp_core->CurrentInstRecallEntry().mRd = NewPC();
#endif
    
    gp_core->ModifyPC(NewPC());
    
#if ENABLE_THREADED_CODE
    if (USAGE_NORMAL == T_usage)
    {
      // Non-sequential next PC
      gp_core->finalize_chunk();
    }
#endif
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Process
  
  namespace
  {
    template<typename mode, IsLink is_link, ExchangeToThumb exchange_to_thumb>
    inline bool
    check_pc_range_helper(InstBranch<mode, is_link, exchange_to_thumb, USAGE_NORMAL> const * const /* branch */)
    {
      assert(!"Should not reach here.");
      return true;
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb>
    inline bool
    check_pc_range_helper(InstBranch<RegBranch, is_link, exchange_to_thumb, USAGE_THREADED_CODE> const * const /* branch */)
    {
      return false;
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb>
    inline bool
    check_pc_range_helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, USAGE_THREADED_CODE> const * const branch)
    {
      int32_t inst_idx = gp_core->curr_chunk()->inst_idx() + branch->dest_offset();
      
      if ((inst_idx >= 0) && (inst_idx < static_cast<int32_t>(gp_core->curr_chunk()->length())))
      {
        gp_core->set_next_tc_inst_idx(inst_idx);
        
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  
  template<typename mode,
           IsLink is_link,
           ExchangeToThumb exchange_to_thumb,
           UsageEnum T_usage>
  inline ExecResultEnum
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  Process(Inst const inst)
  {
    mInst = inst;
    
    Operation();
    
    switch (T_usage)
    {
    case USAGE_NORMAL:
      return ER_NORMAL;
      
    case USAGE_THREADED_CODE:
      if (true == check_pc_range_helper(this))
      {
        return gp_core->template check_periphery<false, false>();
      }
      else
      {
        // :NOTE: Wei 2004-Aug-09:
        //
        // In 'RegBranch' mode, ARMware will always go back to the interpreter.
        //
        // And 'branch' instruction doesn't have the ability to generate an exception.
        return ER_NORMAL_MODIFY_PC;
      }
      
    default:
      assert(!"Should not reach here.");
      return ER_NORMAL;
    }
  }
}
