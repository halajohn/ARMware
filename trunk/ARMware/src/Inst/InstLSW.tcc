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
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    rn_num_helper(InstLSW<ImmLSW,
                 addressing_mode,
                 direction,
                 data_width_enum,
                 desired_mode,
                 is_load,
                 T_usage> const * const lsw)
    {
      return static_cast<CoreRegNum>(lsw->m_inst.mImmLSW.mRn);
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    rn_num_helper(InstLSW<RegLSW,
                 addressing_mode,
                 direction,
                 data_width_enum,
                 desired_mode,
                 is_load,
                 T_usage> const * const lsw)
    {
      return static_cast<CoreRegNum>(lsw->m_inst.mRegLSW.mRn);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreRegNum
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  rn_num() const
  {
    return rn_num_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rd number
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    rd_num_helper(InstLSW<ImmLSW,
                 addressing_mode,
                 direction,
                 data_width_enum,
                 desired_mode,
                 is_load,
                 T_usage> const * const lsw)
    {
      return static_cast<CoreRegNum>(lsw->m_inst.mImmLSW.mRd);
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    rd_num_helper(InstLSW<RegLSW,
                 addressing_mode,
                 direction,
                 data_width_enum,
                 desired_mode,
                 is_load,
                 T_usage> const * const lsw)
    {
      return static_cast<CoreRegNum>(lsw->m_inst.mRegLSW.mRd);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreRegNum
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  rd_num() const
  {
    return rd_num_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // shift type
  //
  // ImmLSW doesn't have shift type filed.
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline SftType
    which_sft_type_helper(InstLSW<ImmLSW,
                        addressing_mode,
                        direction,
                        data_width_enum,
                        desired_mode,
                        is_load,
                        T_usage> const * const /* lsw */)
    {
      assert(!"Should not reach here.");
      return static_cast<SftType>(0);
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline SftType
    which_sft_type_helper(InstLSW<RegLSW,
                        addressing_mode,
                        direction,
                        data_width_enum,
                        desired_mode,
                        is_load,
                        T_usage> const * const lsw)
    {
      return static_cast<SftType>(lsw->m_inst.mRegLSW.mSftType);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline SftType
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  which_sft_type() const
  {
    return which_sft_type_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // shift immediate
  //
  // ImmLSW doesn't have shift_imm field.
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    sft_imm_helper(InstLSW<ImmLSW,
                  addressing_mode,
                  direction,
                  data_width_enum,
                  desired_mode,
                  is_load,
                  T_usage> const * const /* lsw */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    sft_imm_helper(InstLSW<RegLSW,
                  addressing_mode,
                  direction,
                  data_width_enum,
                  desired_mode,
                  is_load,
                  T_usage> const * const lsw)
    {
      return lsw->m_inst.mRegLSW.mSftImm;
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  sft_imm() const
  {
    return sft_imm_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm
  //
  // ImmLSW doesn't have Rm.
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    rm_num_helper(InstLSW<ImmLSW,
                 addressing_mode,
                 direction,
                 data_width_enum,
                 desired_mode,
                 is_load,
                 T_usage> const * const /* lsw */)
    {
      assert(!"Should not reach here.");
      return static_cast<CoreRegNum>(0);
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreRegNum
    rm_num_helper(InstLSW<RegLSW,
                 addressing_mode,
                 direction,
                 data_width_enum,
                 desired_mode,
                 is_load,
                 T_usage> const * const lsw)
    {
      return static_cast<CoreRegNum>(lsw->m_inst.mRegLSW.mRm);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreRegNum
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  rm_num() const
  {
    return rm_num_helper(this);
  }
  
#if CHECK_UNPREDICTABLE
  /////////////////////////////////////////////////////////////////////////////
  // Rn Requirement
  
  namespace
  {
    // Used for pre-index & post-index addressing mode
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    check_rn_requirement_helper(InstLSW<mode,
                                addressing_mode,
                                direction,
                                data_width_enum,
                                desired_mode,
                                is_load,
                                T_usage> const * const lsw)
    {
      // :ARM-DDI-0100E: p.A2-17: Wei 2004-Feb-09:
      //
      // All instructions that allow this to be specified have UNPREDICTABLE results
      // if base register writeback is specified and the base register is the PC,
      // so only general-purpose registers other than the PC can legitimately be
      // modified in this way.
      //
      // :ARM-DDI-0100E: p.A5-24: Wei 2004-Feb-05:
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-05:
      //
      // Specifying R15 as register Rn has UNPREDICTABLE results.
      assert(lsw->rn_num() != CR_PC);
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    check_rn_requirement_helper(InstLSW<mode,
                                OFFSET,
                                direction,
                                data_width_enum,
                                desired_mode,
                                is_load,
                                T_usage> const * const /* lsw */)
    {
      // :NOTE: Wei 2004-Feb-06:
      //
      // No need to check.
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  check_rn_requirement() const
  {
    check_rn_requirement_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm requirement
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    check_rm_requirement_helper(InstLSW<ImmLSW,
                              addressing_mode,
                              direction,
                              data_width_enum,
                              desired_mode,
                              is_load,
                              T_usage> const * const /* lsw */)
    {
      // No Rm field in this mode.
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    check_rm_requirement_helper(InstLSW<RegLSW,
                                addressing_mode,
                                direction,
                                data_width_enum,
                                desired_mode,
                                is_load,
                                T_usage> const * const lsw)
    {
      assert(lsw->rm_num() != CR_PC);
      
      // :ARM-DDI-0100E: p.A5-25: Wei 2004-Feb-05:
      // :ARM-DDI-0100E: p.A5-31: Wei 2004-Feb-05:
      //
      // If the same register is specified for Rn and Rm, the result is UNPREDICTABLE.
      assert(lsw->rn_num() != lsw->rm_num());
    }
    
    template<Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    check_rm_requirement_helper(InstLSW<RegLSW,
                                OFFSET,
                                direction,
                                data_width_enum,
                                desired_mode,
                                is_load,
                                T_usage> const * const lsw)
    {
      assert(lsw->rm_num() != CR_PC);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  check_rm_requirement() const
  {
    check_rm_requirement_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rd requirement
  
  namespace
  {
    // Used for pre-index & post-index addressing mode
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    check_rd_requirement_by_addressing_mode_helper(InstLSW<mode,
                                                   addressing_mode,
                                                   direction,
                                                   data_width_enum,
                                                   desired_mode,
                                                   is_load,
                                                   T_usage> const * const lsw)
    {
      // :ARM-DDI-0100E: p.A4-39: Wei 2003-Dec-03:
      //
      // In all instructions in LSW
      //
      // Operand restrictions
      //    If <addressing_mode> specifies base register writeback,
      //    and the same register is specified for <Rd> and <Rn>,
      //    the results are UNPREDICTABLE.
      //
      // and PRE_IDX & POST_IDX addressing mode does specified base register writeback.
      assert(lsw->rd_num() != lsw->rn_num());
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    check_rd_requirement_by_addressing_mode_helper(InstLSW<mode,
                                                   OFFSET,
                                                   direction,
                                                   data_width_enum,
                                                   desired_mode,
                                                   is_load,
                                                   T_usage> const * const /* lsw */)
    {
      // :NOTE: Wei 2004-Feb-07:
      //
      // OFFSET addressing mode doesn't specified base register writeback.
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  check_rd_requirement_by_addressing_mode() const
  {
    check_rd_requirement_by_addressing_mode_helper(this);
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
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    rn_helper(InstLSW<mode,
              addressing_mode,
              direction,
              data_width_enum,
              desired_mode,
              is_load,
              T_usage> const * const lsw)
    {
#if CHECK_UNPREDICTABLE
      lsw->check_rn_requirement();
#endif
      
      // Rn number in these modes can't be CR_PC
      return gp_core->CurReg(lsw->rn_num());
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    rn_helper(InstLSW<mode,
              OFFSET,
              direction,
              data_width_enum,
              desired_mode,
              is_load,
              T_usage> const * const lsw)
    {
#if CHECK_UNPREDICTABLE
      lsw->check_rn_requirement();
#endif
      
      // :ARM-DDI-0100E: p.A5-20: Wei 2003-Nov-6:
      //
      // In offset addressing, if R15 is specified as register Rn,
      // the value used is the address of the instruction plus 8.
      
      return ((CR_PC == lsw->rn_num()) ? (gp_core->ReadPC()) : (gp_core->CurReg(lsw->rn_num())));
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  rn() const
  {
    return rn_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Offset
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    offset_helper(InstLSW<ImmLSW,
                  addressing_mode,
                  direction,
                  data_width_enum,
                  desired_mode,
                  is_load,
                  T_usage> const * const lsw)
    {
      return lsw->m_inst.mImmLSW.mOff;
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    offset_helper(InstLSW<RegLSW,
                  addressing_mode,
                  direction,
                  data_width_enum,
                  desired_mode,
                  is_load,
                  T_usage> const * const lsw)
    {
      // :NOTE: Wei 2003-Dec-03:
      //
      // Because all kinds of Load Store Word or Unsigned Byte instructions of register form
      // claim Rm != CR_PC, thus we can avoid the special +8 statement for reading
      // CR_PC here.
#if CHECK_UNPREDICTABLE
      lsw->check_rm_requirement();
#endif
      
      // Rm number in these modes can't be CR_PC
      return gp_core->template PreformRegImmDPISftOper<DPI_OP_AND, NO_UPDATE_STATUS>
        (gp_core->CurReg(lsw->rm_num()),
         lsw->which_sft_type(),
         lsw->sft_imm());
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  offset() const
  {
#if CHECK_CALL_ONLY_ONE
    assert(false == m_call_offset_func);
    m_call_offset_func = true;
#endif
    
    return offset_helper(this);
  }
  
#if PRINT_CHUNK_CODE
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  offset_without_check() const
  {
    return offset_helper(this);
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // the load store address
  
  namespace
  {
    template<typename mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    address_helper(InstLSW<mode,
                   OFFSET,
                   direction,
                   data_width_enum,
                   desired_mode,
                   is_load,
                   T_usage> const * const lsw)
    {
      // :NOTE: Wei 2004-Feb-06:
      //
      // Offset addressing mode means no write-back
      
      return Operation<direction>(lsw->rn(), lsw->offset());
    }
    
    template<typename mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    address_helper(InstLSW<mode,
                   PRE_IDX,
                   direction,
                   data_width_enum,
                   desired_mode,
                   is_load,
                   T_usage> const * const lsw)
    {
      uint32_t const address = Operation<direction>(lsw->rn(), lsw->offset());
      
      // :NOTE: Wei 2004-Feb-06:
      //
      // Pre-index addressing mode means write-back
      gp_core->rCurReg(lsw->rn_num()) = address;
      
      return address;
    }
    
    template<typename mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline uint32_t
    address_helper(InstLSW<mode,
                   POST_IDX,
                   direction,
                   data_width_enum,
                   desired_mode,
                   is_load,
                   T_usage> const * const lsw)
    {
      uint32_t const address = lsw->rn();
      
      // :ARM-DDI-0100E: p.A5-28: Wei 2003-Nov-6:
      //
      // post-indexed always write-back
      gp_core->rCurReg(lsw->rn_num()) = Operation<direction>(lsw->rn(), lsw->offset());
      
      return address;
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  address() const
  {
#if CHECK_CALL_ONLY_ONE
    assert(false == m_call_address_func);
    m_call_address_func = true;
#endif
    
    return address_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // AssumeMode
  
  namespace
  {
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreMode
    assumeMode_helper(InstLSW<mode,
                      addressing_mode,
                      direction,
                      data_width_enum,
                      USER_MODE,
                      is_load,
                      T_usage> const * const /* lsw */)
    {
      return MODE_USR;
    }
    
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline CoreMode
    assumeMode_helper(InstLSW<mode,
                      addressing_mode,
                      direction,
                      data_width_enum,
                      CURRENT_MODE,
                      is_load,
                      T_usage> const * const /* lsw */)
    {
      return gp_core->CurrentMode();
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreMode
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  assume_mode() const
  {
    return assumeMode_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation Internal
  
  namespace
  {
    // LDRBT or LDRB
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DesiredMode desired_mode,
             UsageEnum T_usage>
    inline void
    operation_internal_helper(InstLSW<mode,
                              addressing_mode,
                              direction,
                              BYTE,
                              desired_mode,
                              LOAD,
                              T_usage> const * const lsw,
                              bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().m_instNameTag = INST_LDRB;
#endif
      
#if CHECK_UNPREDICTABLE
      lsw->check_rd_requirement_by_addressing_mode();
      
      // :ARM-DDI-0100E: p.A4-40 for LDRB: Wei 2004-Feb-05:
      // :ARM-DDI-0100E: p.A4-42 for LDRBT: Wei 2004-Feb-05:
      //
      // If register 15 is specified for <Rd>, the result is UNPREDICTABLE.
      assert(lsw->rd_num() != CR_PC);
#endif
      
      // :NOTE: Wei 2004-Feb-15:
      //
      // Because address() have a side-effect: the base register may be updated,
      // thus I shouldn't call address() more than once in operation_internal().
      uint32_t const address = lsw->address();
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mVaddr[lsw->rd_num()] = address;
      gp_core->CurrentInstRecallEntry().mCurrentRegNum = lsw->rd_num();
#endif
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if get_data() throws an exception,
      // 'gp_core->rCurReg(...)' will not be modified.
      //
      // Thus I have to use a temporary variable to store the result.
      
      uint32_t const temp = 
        static_cast<uint32_t>(gp_core->template get_data<BYTE, DATA>(address,
                                                                     lsw->assume_mode(),
                                                                     exception_occur));
      
      if (true == exception_occur)
      {
        return;
      }
      else
      {
        gp_core->rCurReg(lsw->rd_num()) = temp;
      }
      
      // :NOTE: Wei 2003-Dec-03:
      //
      // Because in this mode, Rd != CR_PC, I have to increment PC.
      gp_core->IncrePC();
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(lsw->rd_num());
#endif
    }
    
    // LDRT
    template<typename mode, AddressingMode addressing_mode, Direction direction, UsageEnum T_usage>
    inline void
    operation_internal_helper(InstLSW<mode,
                             addressing_mode,
                             direction,
                             WORD,
                             USER_MODE,
                             LOAD,
                             T_usage> const * const lsw,
                             bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().m_instNameTag = INST_LDR;
#endif
      
#if CHECK_UNPREDICTABLE
      lsw->check_rd_requirement_by_addressing_mode();
      
      // :ARM-DDI-0100E: p.A4-50: Wei 2004-Feb-05:
      //
      // If R15 is specified for <Rd>, the result is UNPREDICTABLE.
      assert(lsw->rd_num() != CR_PC);
#endif
      
      // :NOTE: Wei 2004-Feb-15:
      //
      // Because address() have a side-effect: the base register may be updated,
      // thus I shouldn't call address() more than once in operation_internal().
      uint32_t const address = lsw->address();
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if CheckAlign() throws an exception,
      // 'real_address' will not be modified.
      //
      // However, because of 'real_address' is a local variable,
      // thus I choose overwrite its value whether an exception occurs or not.
      uint32_t const real_address = gp_core->template CheckAlign<WORD>(address, exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mVaddr[lsw->rd_num()] = address;
      gp_core->CurrentInstRecallEntry().mCurrentRegNum = lsw->rd_num();
#endif
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if get_data() throws an exception,
      // 'value' will not be modified.
      //
      // However, because of 'value' is a local variable,
      // thus I choose overwrite its value whether an exception occurs or not.
      DataWidthTraits<WORD>::UnsignedType value =
        gp_core->template get_data<WORD, DATA>(real_address, lsw->assume_mode(), exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
      if (address & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK)
      {
        value = RotateRight(value, (address & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK) << 3);
      }
      
      gp_core->rCurReg(lsw->rd_num()) = value;
      
      // :NOTE: Wei 2003-Dec-03:
      //
      // Because in this mode, Rd != CR_PC, I have to increment PC.
      gp_core->IncrePC();
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(lsw->rd_num());
#endif
    }
    
    // LDR
    template<typename mode, AddressingMode addressing_mode, Direction direction, UsageEnum T_usage>
    inline void
    operation_internal_helper(InstLSW<mode,
                              addressing_mode,
                              direction,
                              WORD,
                              CURRENT_MODE,
                              LOAD,
                              T_usage> const * const lsw,
                              bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().m_instNameTag = INST_LDR;
#endif
      
#if CHECK_UNPREDICTABLE
      lsw->check_rd_requirement_by_addressing_mode();
#endif
      
      // :NOTE: Wei 2004-Feb-15:
      //
      // Because address() have a side-effect: the base register may be updated,
      // thus I shouldn't call address() more than once in operation_internal().
      uint32_t const address = lsw->address();
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if CheckAlign() throws an exception,
      // 'real_address' will not be modified.
      //
      // However, because of 'real_address' is a local variable,
      // thus I choose overwrite its value whether an exception occurs or not.
      uint32_t const real_address = gp_core->template CheckAlign<WORD>(address, exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mVaddr[lsw->rd_num()] = address;
      gp_core->CurrentInstRecallEntry().mCurrentRegNum = lsw->rd_num();
#endif
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if get_data() throws an exception,
      // 'value' will not be modified.
      //
      // However, because of 'value' is a local variable,
      // thus I choose overwrite its value whether an exception occurs or not.
      DataWidthTraits<WORD>::UnsignedType value =
        gp_core->template get_data<WORD, DATA>(real_address, lsw->assume_mode(), exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
      if (CR_PC == lsw->rd_num())
      {
#if CHECK_UNPREDICTABLE
        // :ARM-DDI-0100E: p.A4-39: Wei 2004-Feb-06:
        //
        // If register 15 is specified for <Rd>,
        // address[1:0] must be 0b00.
        // If not, the result is UNPREDICTABLE.
        assert(0 == (address & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK));
#endif
        
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3 || CONFIG_ARCH_V4
        gp_core->ModifyPC(value & 0xFFFFFFFC);
#elif CONFIG_ARCH_V5
        gp_core->ModifyPC(value & 0xFFFFFFFE);
        
        (value & 0x1)
          ? (gp_core->set_CPSR_T_bit())
          : (gp_core->clear_CPSR_T_bit());
#endif
        
        // :NOTE: Wei 2003-Dec-03:
        //
        // Because in this mode, Rd == CR_PC,
        // thus I shouldn't increment PC.
        
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
        if (address & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK)
        {
          value = RotateRight(value, (address & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK) << 3);
        }
        
        gp_core->rCurReg(lsw->rd_num()) = value;
        
        gp_core->IncrePC();
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(lsw->rd_num());
#endif
    }
    
    // STRB or STRBT
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DesiredMode desired_mode,
             UsageEnum T_usage>
    inline void
    operation_internal_helper(InstLSW<mode,
                             addressing_mode,
                             direction,
                             BYTE,
                             desired_mode,
                             STORE,
                             T_usage> const * const lsw,
                             bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().m_instNameTag = INST_STRB;
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(lsw->rd_num());
#endif
      
#if CHECK_UNPREDICTABLE
      lsw->check_rd_requirement_by_addressing_mode();
      
      // :ARM-DDI-0100E: p.A4-90 for STRB: Wei 2004-Feb-05:
      // :ARM-DDI-0100E: p.A4-92 for STRBT: Wei 2004-Feb-05:
      //
      // If register 15 is specified for <Rd>, the result is UNPREDICTABLE.
      assert(lsw->rd_num() != CR_PC);
#endif
      
      // :NOTE: Wei 2004-Feb-15:
      //
      // Because address() have a side-effect: the base register may be updated,
      // thus I shouldn't call address() more than once in operation_internal().
      uint32_t const address = lsw->address();
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mVaddr[lsw->rd_num()] = address;
      gp_core->CurrentInstRecallEntry().mCurrentRegNum = lsw->rd_num();
#endif
      
      gp_core->template put_data<BYTE>(
        address,
        static_cast<DataWidthTraits<BYTE>::UnsignedType>(
          gp_core->CurReg(lsw->rd_num()) & 0xFF
                                                         ),
        lsw->assume_mode(),
        exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
      gp_core->IncrePC();
    }
    
    // STR or STRT
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DesiredMode desired_mode,
             UsageEnum T_usage>
    inline void
    operation_internal_helper(InstLSW<mode,
                             addressing_mode,
                             direction,
                             WORD,
                             desired_mode,
                             STORE,
                             T_usage> const * const lsw,
                             bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().m_instNameTag = INST_STR;
      gp_core->CurrentInstRecallEntry().mRd = gp_core->CurReg(lsw->rd_num());
#endif
      
#if CHECK_UNPREDICTABLE
      lsw->check_rd_requirement_by_addressing_mode();
#endif
      
      DataWidthTraits<WORD>::UnsignedType store_value = gp_core->CurReg(lsw->rd_num());
      
      if (CR_PC == lsw->rd_num())
      {
        // :ARM-DDI-0100E: p.A2-7: Wei 2003-Dec-03:
        //
        // if (Rd == CR_PC),
        // STR can perform '+8' or '+12' in the following statement.
        // It is IMPLEMENTATION DEFINED.
        //
        // However, I cannot find a specified statement describe this
        // choice in the Intel StrongARM, thus I assume '+8' rather than
        // '+12'.
        store_value += 8;
      }
      
      // :NOTE: Wei 2004-Feb-15:
      //
      // Because address() have a side-effect: the base register may be updated,
      // thus I shouldn't call address() more than once in operation_internal().
      uint32_t const address = lsw->address();
      
      // :NOTE: Wei 2004-Jul-30:
      //
      // In the original throw exception version, if CheckAlign() throws an exception,
      // 'real_address' will not be modified.
      //
      // However, because of 'real_address' is a local variable,
      // thus I choose overwrite its value whether an exception occurs or not.
      uint32_t const real_address = gp_core->template CheckAlign<WORD>(address, exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mVaddr[lsw->rd_num()] = address;
      gp_core->CurrentInstRecallEntry().mCurrentRegNum = lsw->rd_num();
#endif
      
      gp_core->template put_data<WORD>(
        real_address,
        store_value,
        lsw->assume_mode(),
        exception_occur);
      
      if (true == exception_occur)
      {
        return;
      }
      
      gp_core->IncrePC();
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  operation_internal(bool &exception_occur) const
  {
    operation_internal_helper(this, exception_occur);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  namespace
  {
    // Used for pre-index & post-index addressing mode
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    operation_helper(InstLSW<mode,
                     addressing_mode,
                     direction,
                     data_width_enum,
                     desired_mode,
                     is_load,
                     T_usage> const * const lsw,
                     bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRdNum = lsw->rd_num();
      gp_core->CurrentInstRecallEntry().mRnNum = lsw->rn_num();
#endif
      
      // :ARM-DDI-0100E: p.A2-18: Wei 2003-Dec-09:
      //
      // If base register writeback is not specified, the base register value is unchanged.
      
      // :SA-1110 Developer's Manual: p.41: Wei 2003-Dec-09:
      //
      // Single data transfer instructions (LDR, STR) will abort with no registers modified.
      
      uint32_t const orig_rn_value = lsw->rn();
      
      lsw->operation_internal(exception_occur);
      
      if (true == exception_occur)
      {
        gp_core->rCurReg(lsw->rn_num()) = orig_rn_value;
      }
    }
    
    // Special case for offset addressing mode
    template<typename mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    operation_helper(InstLSW<mode,
                     OFFSET,
                     direction,
                     data_width_enum,
                     desired_mode,
                     is_load,
                     T_usage> const * const lsw,
                     bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mRdNum = lsw->rd_num();
      gp_core->CurrentInstRecallEntry().mRnNum = lsw->rn_num();
#endif
      
      // :NOTE: Wei 2004-Feb-06:
      //
      // Offset addressing mode means no write-back.
      // Thus I don't need to consider the restore of the original base register (Rn).
      
      lsw->operation_internal(exception_occur);
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  operation(bool &exception_occur) const
  {
    operation_helper(this, exception_occur);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // check modify pc
  
  namespace
  {
    // LDRBT or LDRB
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DesiredMode desired_mode,
             UsageEnum T_usage>
    inline ExecResultEnum
    check_modify_pc_helper(InstLSW<mode,
                           addressing_mode,
                           direction,
                           BYTE,
                           desired_mode,
                           LOAD,
                           T_usage> const * const /* lsw */)
    {
      // :NOTE: Wei 2004-Sep-20:
      //
      // Because the base register in all LSW instructions shouldn't be CR_PC,
      // thus I only need to consider the Rd register is CR_PC or not.
      
      // :ARM-DDI-0100E: p.A4-40 for LDRB: Wei 2004-Feb-05:
      // :ARM-DDI-0100E: p.A4-42 for LDRBT: Wei 2004-Feb-05:
      //
      // If register 15 is specified for <Rd>, the result is UNPREDICTABLE.
      
      return ER_NORMAL;
    }
    
    // LDRT
    template<typename mode, AddressingMode addressing_mode, Direction direction, UsageEnum T_usage>
    inline ExecResultEnum
    check_modify_pc_helper(InstLSW<mode,
                           addressing_mode,
                           direction,
                           WORD,
                           USER_MODE,
                           LOAD,
                           T_usage> const * const /* lsw */)
    {
      // :ARM-DDI-0100E: p.A4-50: Wei 2004-Feb-05:
      //
      // If R15 is specified for <Rd>, the result is UNPREDICTABLE.
      
      return ER_NORMAL;
    }
    
    // LDR
    template<typename mode, AddressingMode addressing_mode, Direction direction, UsageEnum T_usage>
    inline ExecResultEnum
    check_modify_pc_helper(InstLSW<mode,
                           addressing_mode,
                           direction,
                           WORD,
                           CURRENT_MODE,
                           LOAD,
                           T_usage> const * const lsw)
    {
      if (CR_PC == lsw->rd_num())
      {
        return ER_NORMAL_MODIFY_PC;
      }
      else
      {
        return ER_NORMAL;
      }
    }
    
    // STRB or STRBT or STR or STRT
    template<typename mode,
             AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             UsageEnum T_usage>
    inline ExecResultEnum
    check_modify_pc_helper(InstLSW<mode,
                           addressing_mode,
                           direction,
                           data_width_enum,
                           desired_mode,
                           STORE,
                           T_usage> const * const /* lsw */)
    {
      // :NOTE: Wei 2004-Sep-20:
      //
      // Because the base register in all LSW instructions shouldn't be CR_PC,
      // thus the STR variants in the LSW instructions shouldn't modify PC.
      
      return ER_NORMAL;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline ExecResultEnum
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  Process(Inst const inst)
  {
#if CHECK_CALL_ONLY_ONE
    m_call_address_func = false;
    m_call_offset_func = false;
#endif
    
    m_inst = inst;
    bool exception_occur = false;
    
#if CHECK_CHUNK_CODE
    assert(inst.mRawData == gp_core->get_curr_inst());
#endif
    
#if DUMP_CPU_INFO_MORE_DETAIL
    if (STORE == is_load)
    {
      if (true == gp_core->is_exec_in_chunk())
      {
        g_log_file << "--> <LSW> "
                   << std::endl;
      }
      
      gp_core->dump_cpu_info<false>();
    }
#endif
    
    operation(exception_occur);
    
    if (true == exception_occur)
    {
      gp_core->set_data_abort();
      
      return ER_EXCEPTION;
    }
    else
    {
      switch (T_usage)
      {
      case USAGE_NORMAL:
        return ER_NORMAL;
        
      case USAGE_THREADED_CODE:
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
        
        return check_modify_pc_helper(this);
        
      default:
        assert(!"Should not reach here.");
        return ER_NORMAL;
      }
    }
  }  
}
