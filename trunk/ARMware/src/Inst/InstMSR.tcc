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
  // 8-bit immediate field
  
  namespace
  {
    template<UseWhichStatus use_which_status>
    inline uint32_t
    Imm_Helper(InstMSR<RegMSR, use_which_status> const * const /* msr */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<UseWhichStatus use_which_status>
    inline uint32_t
    Imm_Helper(InstMSR<ImmMSR, use_which_status> const * const msr)
    {
      return msr->m_inst.mImmMSR.mImm;
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline uint32_t
  InstMSR<mode, use_which_status>::
  Imm() const
  {
    return Imm_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // rotate_imm field
  
  namespace
  {
    template<UseWhichStatus use_which_status>
    inline uint32_t
    RotImm_Helper(InstMSR<RegMSR, use_which_status> const * const /* msr */)
    {
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<UseWhichStatus use_which_status>
    inline uint32_t
    RotImm_Helper(InstMSR<ImmMSR, use_which_status> const * const msr)
    {
      return msr->m_inst.mImmMSR.mRotImm;
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline uint32_t
  InstMSR<mode, use_which_status>::
  RotImm() const
  {
    return RotImm_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Rm
  
  namespace
  {
    template<UseWhichStatus use_which_status>
    inline CoreRegNum
    rm_num_helper(InstMSR<RegMSR, use_which_status> const * const msr)
    {
      return static_cast<CoreRegNum>(msr->m_inst.mRegMSR.mRm);
    }
    
    template<UseWhichStatus use_which_status>
    inline CoreRegNum
    rm_num_helper(InstMSR<ImmMSR, use_which_status> const * const /* msr */)
    {
      assert(!"Should not reach here.");
      return static_cast<CoreRegNum>(0);
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline CoreRegNum
  InstMSR<mode, use_which_status>::
  rm_num() const
  {
    return rm_num_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // FieldTratis
  
#if CHECK_UNPREDICTABLE  
  namespace
  {
    template<typename mode>
    inline void
    CheckSettingCondition(InstMSR<mode, USE_CURRENT_SPSR> const * const /* msr */,
                          FieldTraits<CONTROL> const &/* field */,
                          uint32_t const /* operand */)
    {
    }
    
    template<typename mode>
    inline void
    CheckSettingCondition(InstMSR<mode, USE_CPSR> const * const /* msr */,
                          FieldTraits<CONTROL> const &/* field */,
                          uint32_t const operand)
    {
      // :ARM-DDI-0100E: p.A4-65: Wei 2004-Feb-10:
      //
      // The MSR instruction must not be used to alter the T bit
      // in the CPSR. If such an attempt is made, the results are UNPREDICTABLE.
      assert((gp_core->Cpsr() & Core::TBIT_MASK) == (operand & Core::TBIT_MASK));
      
      // :ARM-DDI-0100E: p.A2-19: Wei 2004-Apr-252:
      //
      // The F bit can only be changed from a privileged mode.
      if ((operand & Core::FBIT_MASK) != (gp_core->Cpsr() & Core::FBIT_MASK))
      {
        assert(MODE_USR != gp_core->CurrentMode());
      }
    }
    
    template<typename mode, UseWhichStatus use_which_status>
    inline void
    CheckSettingCondition(InstMSR<mode, use_which_status> const * const /* msr */,
                          FieldTraits<EXTENSION> const &/* field */,
                          uint32_t const /* operand */)
    {
    }
    
    template<typename mode, UseWhichStatus use_which_status>
    inline void
    CheckSettingCondition(InstMSR<mode, use_which_status> const * const /* msr */,
                          FieldTraits<STATUS> const &/* field */,
                          uint32_t const /* operand */)
    {
    }
    
    template<typename mode, UseWhichStatus use_which_status>
    inline void
    CheckSettingCondition(InstMSR<mode, use_which_status> const * const /* msr */,
                          FieldTraits<FLAGS> const &/* field */,
                          uint32_t const /* operand */)
    {
    }
    
    template<typename mode, UseWhichStatus use_which_status>
    inline void
    CheckSettingCondition(InstMSR<mode, use_which_status> const * const /* msr */,
                          FieldTraits<ALL> const &/* field */,
                          uint32_t const /* operand */)
    {
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // some form of MSR have not to be in some mode.
  
  namespace
  {
    template<typename mode>
    inline void
    CheckProperMode_Helper(InstMSR<mode, USE_CPSR> const * const /* msr */)
    { }
    
    template<typename mode>
    inline void
    CheckProperMode_Helper(InstMSR<mode, USE_CURRENT_SPSR> const * const /* msr */)
    {
      // :ARM-DDI-0100E: p.A4-65: Wei 2004-Feb-11:
      //
      // Accessing the SPSR when in User mode is UNPREDICTABLE.
      // Accessing the SPSR when in System mode is UNPREDICTABLE.
      assert(gp_core->CurrentMode() != MODE_USR);
      assert(gp_core->CurrentMode() != MODE_SYS);
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline void
  InstMSR<mode, use_which_status>::
  CheckProperMode() const
  {
    CheckProperMode_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  namespace
  {
    template<UseWhichStatus use_which_status>
    inline void
    CheckBits_Helper(InstMSR<ImmMSR, use_which_status> const * const msr)
    {
      assert(0xF000 == (msr->m_inst.mRawData & 0xF000));
      
      // :Steve furber: p.135: Wei 2004-Jun-10:
      //
      // When an immediate operand is used only the flags (PSR[31:24])
      // may be selected for update.
      assert(FieldTraits<FLAGS>::INST_BITMASK == (msr->m_inst.mRawData & FieldTraits<ALL>::INST_BITMASK));
    }
    
    template<UseWhichStatus use_which_status>
    inline void
    CheckBits_Helper(InstMSR<RegMSR, use_which_status> const * const msr)
    {
      assert(0xF000 == (msr->m_inst.mRawData & 0xF000));
      assert(0 == (msr->m_inst.mRawData & 0xFF0));
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline void
  InstMSR<mode, use_which_status>::
  CheckBits() const
  {
    CheckBits_Helper(this);
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // Operand
  
  namespace
  {
    template<UseWhichStatus use_which_status>
    inline uint32_t
    Operand_Helper(InstMSR<ImmMSR, use_which_status> const * const msr)
    {
      return RotateRight(msr->Imm(), msr->RotImm() << 1);
    }
    
    template<UseWhichStatus use_which_status>
    inline uint32_t
    Operand_Helper(InstMSR<RegMSR, use_which_status> const * const msr)
    {
      return ((CR_PC == msr->rm_num())
              ? (gp_core->ReadPC())
              : (gp_core->CurReg(msr->rm_num())));
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline uint32_t
  InstMSR<mode, use_which_status>::
  Operand() const
  {
    return Operand_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // MSR additional requirement
  
  namespace
  {
    template<typename mode, FieldNameEnum field_name_enum>
    inline bool
    CheckAdditionalRequirement(InstMSR<mode, USE_CURRENT_SPSR> const * const /* msr */,
                               FieldTraits<field_name_enum> const &/* field */)
    {
      return true;
    }
    
    // Used for CONTROL, EXTENSION, STATUS
    
    template<typename mode, FieldNameEnum field_name_enum>
    inline bool
    CheckAdditionalRequirement(InstMSR<mode, USE_CPSR> const * const /* msr */,
                               FieldTraits<field_name_enum> const &/* field */)
    {
      return ((MODE_USR == gp_core->CurrentMode())
              ? false
              : true);
    }
    
    template<typename mode>
    inline bool
    CheckAdditionalRequirement(InstMSR<mode, USE_CPSR> const * const /* msr */,
                               FieldTraits<FLAGS> const &/* field */)
    {
      return true;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // use which status register
  
  namespace
  {
    template<typename mode>
    inline uint32_t &
    desired_status_reg_helper(InstMSR<mode, USE_CPSR> const * const /* msr */)
    {
      return gp_core->rCpsr();
    }
    
    template<typename mode>
    inline uint32_t &
    desired_status_reg_helper(InstMSR<mode, USE_CURRENT_SPSR> const * const /* msr */)
    {
      return gp_core->rCurSpsr();
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline uint32_t &
  InstMSR<mode, use_which_status>::
  desired_status_reg() const
  {
    return desired_status_reg_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // When I change the content of CPSR, I have to change the core mode, too.
  
  namespace
  {
    template<typename mode>
    inline void
    change_core_mode_if_necessary_helper(InstMSR<mode, USE_CPSR> const * const /* msr */)
    {
      gp_core->change_reg_bank(static_cast<CoreMode>(gp_core->Cpsr() & Core::MODE_MASK));
    }
    
    template<typename mode>
    inline void
    change_core_mode_if_necessary_helper(InstMSR<mode, USE_CURRENT_SPSR> const * const /* msr */)
    { }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline void
  InstMSR<mode, use_which_status>::
  change_core_mode_if_necessary() const
  {
    change_core_mode_if_necessary_helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // UpdateField
  
  template<typename mode, UseWhichStatus use_which_status>
  template<FieldNameEnum msr_field_name>
  inline void
  InstMSR<mode, use_which_status>::
  UpdateField(uint32_t const operand)
  {
    if (true == CheckAdditionalRequirement(this, FieldTraits<msr_field_name>()))
    {
#if CHECK_UNPREDICTABLE
      CheckSettingCondition(this, FieldTraits<msr_field_name>(), operand);
#endif
      
      desired_status_reg() &= ~FieldTraits<msr_field_name>::STATUS_REG_BITMASK;
      desired_status_reg() |= (operand & FieldTraits<msr_field_name>::STATUS_REG_BITMASK);
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<typename mode, UseWhichStatus use_which_status>
  inline void
  InstMSR<mode, use_which_status>::
  Operation()
  {
#if CHECK_UNPREDICTABLE
    CheckBits();
    CheckProperMode();
#endif
    
#if ENABLE_INST_RECALL
    gp_core->CurrentInstRecallEntry().mInstNameTag = INST_MSR;
#endif
    
    uint32_t const operand = Operand();
    
    switch (m_inst.mRawData & FieldTraits<ALL>::INST_BITMASK)
    {
    case 0: // 0000
      break;
      
    case FieldTraits<CONTROL>::INST_BITMASK: // 0001
      UpdateField<CONTROL>(operand);
      break;
      
    case FieldTraits<EXTENSION>::INST_BITMASK: // 0010
      UpdateField<EXTENSION>(operand);
      break;
      
    case (FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 0011
      UpdateField<CONTROL>(operand);
      UpdateField<EXTENSION>(operand);
      break;
      
    case FieldTraits<STATUS>::INST_BITMASK: // 0100
      UpdateField<STATUS>(operand);
      break;
      
    case (FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 0101
      UpdateField<CONTROL>(operand);
      UpdateField<STATUS>(operand);
      break;
      
    case (FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK): // 0110
      UpdateField<EXTENSION>(operand);
      UpdateField<STATUS>(operand);
      break;
      
    case (FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 0111
      UpdateField<CONTROL>(operand);
      UpdateField<EXTENSION>(operand);
      UpdateField<STATUS>(operand);
      break;
      
    case FieldTraits<FLAGS>::INST_BITMASK: // 1000
      UpdateField<FLAGS>(operand);
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1001
      UpdateField<CONTROL>(operand);
      UpdateField<FLAGS>(operand);
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK): // 1010
      UpdateField<EXTENSION>(operand);
      UpdateField<FLAGS>(operand);
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1011
      UpdateField<CONTROL>(operand);
      UpdateField<EXTENSION>(operand);
      UpdateField<FLAGS>(operand);
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK): // 1100
      UpdateField<STATUS>(operand);
      UpdateField<FLAGS>(operand);
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1101
      UpdateField<CONTROL>(operand);
      UpdateField<STATUS>(operand);
      UpdateField<FLAGS>(operand);
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK): // 1110
      UpdateField<EXTENSION>(operand);
      UpdateField<STATUS>(operand);
      UpdateField<FLAGS>(operand);
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1111
      UpdateField<CONTROL>(operand);
      UpdateField<EXTENSION>(operand);
      UpdateField<STATUS>(operand);
      UpdateField<FLAGS>(operand);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    // :Steve furber: p.135: Wei 2004-Jun-10:
    //
    // The mode change takes effect only after the MSR has been executed;
    // the intermediate working has no effect on the mode until the result is copied back
    // into the CPSR.
    change_core_mode_if_necessary();
    
    gp_core->IncrePC();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Process
  
  template<typename mode, UseWhichStatus use_which_status>
  inline ExecResultEnum
  InstMSR<mode, use_which_status>::
  Process(Inst const inst)
  {
    m_inst = inst;
    
#if CHECK_CHUNK_CODE
    assert(inst.mRawData == gp_core->get_curr_inst());
#endif
    
#if DUMP_CPU_INFO_MORE_DETAIL
    if (true == gp_core->is_exec_in_chunk())
    {
      g_log_file << "--> <MSR before> "
                 << std::endl;
    }
    
    gp_core->dump_cpu_info<false>();
#endif
    
    Operation();
    
#if DUMP_CPU_INFO_MORE_DETAIL
    if (true == gp_core->is_exec_in_chunk())
    {
      g_log_file << "--> <MSR end> "
                 << std::endl;
    }
    
    gp_core->dump_cpu_info<false>();
#endif
    
    // :NOTE: Wei 2004-Aug-09:
    //
    // Doesn't have ability to generate an exception.
    //
    // MSR instructions shouldn't modify PC.
    return ER_NORMAL;
  }
}
