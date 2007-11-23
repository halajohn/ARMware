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
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline CoreRegNum
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  RnNum() const
  {
    return static_cast<CoreRegNum>(mInst.mLSM.mRn);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Reg Enable
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline bool
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  RegEnable(CoreRegNum const reg_num) const
  {
    assert(static_cast<uint32_t>(reg_num) <= CR_15);
    
    if (static_cast<uint32_t>(1 << static_cast<uint32_t>(reg_num)) ==
        (mInst.mRawData & (1 << static_cast<uint32_t>(reg_num))))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Reg Enable Count
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  RegEnableCount() const
  {
    uint32_t x = mInst.mRawData;
    
    x &= 0xFFFF;
    x = (x & 0x5555) + ((x >> 1) & 0x5555);
    x = (x & 0x3333) + ((x >> 2) & 0x3333);
    x = (x & 0x0F0F) + ((x >> 4) & 0x0F0F);
    x = (x & 0x00FF) + ((x >> 8) & 0x00FF);
    
#if CHECK_UNPREDICTABLE
    // :ARM-DDI-0100E: p.A5-49: Wei 2004-Feb-09:
    //
    // If no bits are set, the result is UNPREDICTABLE.
    assert(x != 0);
#endif
    
    return x;
  }
  
#if CHECK_UNPREDICTABLE
  /////////////////////////////////////////////////////////////////////////////
  // Rn requirement
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  CheckRnRequirement() const
  {
    // :ARM-DDI-0100E: p.A2-17: Wei 2004-Feb-09:
    //
    // All instructions that allow this to be specified have UNPREDICTABLE results
    // if base register writeback is specified and the base register is the PC,
    // so only general-purpose registers other than the PC can legitimately be
    // modified in this way.
    //
    // :ARM-DDI-0100E: p.A4-30: Wei 2004-Feb-09:
    //
    // *All* of the LSM instructions specified:
    //
    // Using R15 as the base register <Rn> gives an UNPREDICTABLE result.
    assert(RnNum() != CR_PC);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Check base register writeback
  
  namespace
  {
    template<Direction direction,
             StartFrom start_from,
             DesiredBank desired_bank,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckBaseRegisterWriteBack_Helper(InstLSM<direction,
                                      start_from,
                                      desired_bank,
                                      NO_WRITE_BACK,
                                      is_load,
                                      T_usage> const * const /* lsm */)
    { }
    
    template<Direction direction,
             StartFrom start_from,
             DesiredBank desired_bank,
             UsageEnum T_usage>
    inline void
    CheckBaseRegisterWriteBack_Helper(InstLSM<direction,
                                      start_from,
                                      desired_bank,
                                      WRITE_BACK,
                                      LOAD,
                                      T_usage> const * const lsm)
    {
      // :ARM-DDI-0100E: p.A4-31: Wei 2004-Feb-01:
      //
      // If the base register <Rn> is specified in <registers>,
      // and base register writeback is specified,
      // the final value of <Rn> is UNPREDICTABLE.
      //
      // :NOTE: Wei 2004-Apr-26:
      //
      // LDM_1 & LDM_3 have this behavior.
      // LDM_2 doesn't (because all LDM_2 instructions are of NO_WRITE_BACK mode).
      assert(false == lsm->RegEnable(lsm->RnNum()));
    }
    
    template<Direction direction,
             StartFrom start_from,
             DesiredBank desired_bank,
             UsageEnum T_usage>
    inline void
    CheckBaseRegisterWriteBack_Helper(InstLSM<direction,
                                      start_from,
                                      desired_bank,
                                      WRITE_BACK,
                                      STORE,
                                      T_usage> const * const /* lsm */)
    { }
  }
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  CheckBaseRegisterWriteBack() const
  {
    CheckBaseRegisterWriteBack_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Check proper mode
  
  namespace
  {
    template<Direction direction, StartFrom start_from, IsWriteBack is_write_back, IsLoad is_load, UsageEnum T_usage>
    inline void
    CheckProperMode_Helper(InstLSM<direction,
                           start_from,
                           CURRENT_BANK,
                           is_write_back,
                           is_load,
                           T_usage> const * const /* lsm */)
    { }
    
    template<Direction direction,
             StartFrom start_from,
             IsWriteBack is_write_back,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    CheckProperMode_Helper(InstLSM<direction,
                           start_from,
                           USER_BANK,
                           is_write_back,
                           is_load,
                           T_usage> const * const /* lsm */)
    {
      // :ARM-DDI-0100E: p.A4-33: LDM(2): Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-35: LDM(3): Wei 2004-Feb-09:
      // :ARM-DDI-0100E: p.A4-87: LSM(2): Wei 2003-Nov-6:
      //
      // This form of LDM & STM is UNPREDICTABLE in User mode or System mode.
      assert(gp_core->CurrentMode() != MODE_USR);
      assert(gp_core->CurrentMode() != MODE_SYS);
    }
  }
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  CheckProperMode() const
  {
    CheckProperMode_Helper(this);
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // Rn
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  Rn() const
  {
#if CHECK_UNPREDICTABLE
    CheckRnRequirement();
#endif
    
    // :ARM-DDI-0100E: p.A4-33 for LDM(2): Wei 2004-May-24:
    // :ARM-DDI-0100E: p.A4-87 for STM(2): Wei 2004-May-24:
    //
    // The base register is read from the current processor mode registers, not the User mode registers.
    
    // Rn number shouldn't be CR_PC in all LSM instructions.
    return gp_core->CurReg(RnNum());
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Write back base register
  
  namespace
  {
    template<Direction direction,
             StartFrom start_from,
             DesiredBank desired_bank,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    WriteBackBase_Helper(InstLSM<direction,
                         start_from,
                         desired_bank,
                         NO_WRITE_BACK,
                         is_load,
                         T_usage> const * const /* lsm */)
    { }
    
    template<Direction direction,
             StartFrom start_from,
             DesiredBank desired_bank,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    WriteBackBase_Helper(InstLSM<direction,
                         start_from,
                         desired_bank,
                         WRITE_BACK,
                         is_load,
                         T_usage> const * const lsm)
    {
      gp_core->rCurReg(lsm->RnNum()) = ARMware::Operation<direction>(lsm->Rn(), lsm->RegEnableCount() << 2);
    }
  }
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  WriteBackBase() const
  {
    WriteBackBase_Helper(this);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Starting address
  
  namespace
  {
    template<DesiredBank desired_bank, IsWriteBack is_write_back, IsLoad is_load, UsageEnum T_usage>
    inline uint32_t
    StartAddress_Helper(InstLSM<UP,
                        BEFORE,
                        desired_bank,
                        is_write_back,
                        is_load,
                        T_usage> const * const lsm)
    {
      // IB
#if CHECK_LSM_END_ADDR
      lsm->mEndAddr = lsm->Rn() + (lsm->RegEnableCount() << 2);
#endif
      
      return lsm->Rn() + 4;
    }
    
    template<DesiredBank desired_bank, IsWriteBack is_write_back, IsLoad is_load, UsageEnum T_usage>
    inline uint32_t
    StartAddress_Helper(InstLSM<UP,
                        AFTER,
                        desired_bank,
                        is_write_back,
                        is_load,
                        T_usage> const * const lsm)
    {
      // IA
#if CHECK_LSM_END_ADDR
      lsm->mEndAddr = lsm->Rn() + (lsm->RegEnableCount() << 2) - 4;
#endif
      
      return lsm->Rn();
    }
    
    template<DesiredBank desired_bank, IsWriteBack is_write_back, IsLoad is_load, UsageEnum T_usage>
    inline uint32_t
    StartAddress_Helper(InstLSM<DOWN,
                        BEFORE,
                        desired_bank,
                        is_write_back,
                        is_load,
                        T_usage> const * const lsm)
    {
      // DB
#if CHECK_LSM_END_ADDR
      lsm->mEndAddr = lsm->Rn() - 4;
#endif
      
      return lsm->Rn() - (lsm->RegEnableCount() << 2);
    }
    
    template<DesiredBank desired_bank, IsWriteBack is_write_back, IsLoad is_load, UsageEnum T_usage>
    inline uint32_t
    StartAddress_Helper(InstLSM<DOWN,
                        AFTER,
                        desired_bank,
                        is_write_back,
                        is_load,
                        T_usage> const * const lsm)
    {
      // DA
#if CHECK_LSM_END_ADDR
      lsm->mEndAddr = lsm->Rn();
#endif
      
      return lsm->Rn() - (lsm->RegEnableCount() << 2) + 4;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Starting address
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline uint32_t
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  StartAddress() const
  {
    uint32_t const address = StartAddress_Helper(this);
    
    WriteBackBase();
    
    return address;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation internal
  
  // :NOTE: Wei 2004-May-27:
  //
  // When I implement the following LDM & STM instructions,
  // I found that I may do a optimization:
  //
  // int32_t temp = (inst.mRawData << 16);
  //
  // if (temp != 0)
  // {
  //   if (temp < 1)
  //   {
  //     ... the topmost bit is 1, and the corresponding register is enabled.
  //
  //     address -= 4; // because we are count from top to bottom.
  //   }
  //   temp <<= 1;
  // }
  //
  // However, use this optimization will not preserve the time order of memory access in LDM & STM.
  // Although some forms of LDM & STM are not guaranteed to preserve the time-order.
  // However, there are indeed some forms of LDM & STM that are guaranteed to preserve it.
  // (See ARM-DDI-0100E, p.A2-33 ~ p.A2-34)
  //
  // When we preform memory accesses to the memory-mapped I/O area,
  // the time-order between several memory accesses is important !
  //
  // Thus I choice to reject this optimization for code unifying and simplicity.
  
  // Another information...
  //
  // :Steve furber: p.110: Wei 2004-Jun-10:
  //
  // LDMFD r13!, {r0-r3, rc}^
  // ...
  // The CPSR is restored at the same time that the PC is loaded from memory,
  // which will always be the last item transfered from memory since the registers
  // are loaded in increasing order.
  namespace
  {
    // LDM(1)
    template<Direction direction,
             StartFrom start_from,
             IsWriteBack is_write_back,
             UsageEnum T_usage>
    inline void
    OperationInternal_Helper(InstLSM<direction,
                             start_from,
                             CURRENT_BANK,
                             is_write_back,
                             LOAD,
                             T_usage> const * const lsm,
                             uint32_t const /* orig_rn_value */, uint32_t address, bool &exception_occur)
    {
#if CHECK_UNPREDICTABLE
      lsm->CheckBaseRegisterWriteBack();
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_LDM_1;
#endif
      
      for (CoreRegNum i = CR_BEGIN; i != CR_15; ++i)
      {
        if (true == lsm->RegEnable(i))
        {
#if ENABLE_INST_RECALL
          gp_core->CurrentInstRecallEntry().mEnable[i] = true;
          gp_core->CurrentInstRecallEntry().mVaddr[i] = address;
          gp_core->CurrentInstRecallEntry().mCurrentRegNum = i;
#endif
          
          // :NOTE: Wei 2004-Jul-30:
          //
          // In the original throw exception version, if get_data() throws an exception,
          // 'gp_core->rCurReg(i)' will not be modified.
          //
          // Thus I have to use a temporary variable to store the result.
          uint32_t const temp = 
            gp_core->template get_data<WORD, DATA>(address,
                                                       gp_core->CurrentMode(),
                                                       exception_occur);
          
          if (true == exception_occur)
          {
            return;
          }
          else
          {
            gp_core->rCurReg(i) = temp;
          }
          
#if ENABLE_INST_RECALL
          gp_core->CurrentInstRecallEntry().mValue[i] = gp_core->CurReg(i);
#endif
          
          address += 4;
        }
      }
      
      if (true == lsm->RegEnable(CR_PC))
      {
#if ENABLE_INST_RECALL
        gp_core->CurrentInstRecallEntry().mEnable[CR_PC] = true;
        gp_core->CurrentInstRecallEntry().mVaddr[CR_PC] = address;
        gp_core->CurrentInstRecallEntry().mCurrentRegNum = CR_PC;
#endif
        
        // :NOTE: Wei 2004-Jul-30:
        //
        // In the original throw exception version, if get_data() throws an exception,
        // 'value' will not be modified.
        //
        // However, because of 'value' is a local variable,
        // thus I choose overwrite its value whether an exception occurs or not.
        uint32_t const value =
          gp_core->template get_data<WORD, DATA>(address,
                                                     gp_core->CurrentMode(),
                                                     exception_occur);
        
        if (true == exception_occur)
        {
          return;
        }
        
#if CHECK_LSM_END_ADDR
        address += 4;
#endif
        
#if ENABLE_INST_RECALL
        gp_core->CurrentInstRecallEntry().mValue[CR_PC] = value;
#endif
        
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3 || CONFIG_ARCH_V4
        gp_core->ModifyPC(value & 0xFFFFFFFC);
#elif CONFIG_ARCH_V5
        gp_core->ModifyPC(value & 0xFFFFFFFE);
        
        (value & 0x1)
          ? (gp_core->set_CPSR_T_bit())
          : (gp_core->clear_CPSR_T_bit());
#endif
        
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
        gp_core->IncrePC();
      }
      
#if CHECK_LSM_END_ADDR
      assert((address - 4) == lsm->mEndAddr);
#endif
    }
    
    template<Direction direction, StartFrom start_from, IsWriteBack is_write_back, UsageEnum T_usage>
    inline void
    OperationInternal_Helper(InstLSM<direction,
                             start_from,
                             USER_BANK,
                             is_write_back,
                             LOAD,
                             T_usage> const * const lsm,
                             uint32_t const /* orig_rn_value */, uint32_t address, bool &exception_occur)
    {
      if (false == lsm->RegEnable(CR_PC))
      {
        // LDM(2)
        
#if ENABLE_INST_RECALL
        gp_core->CurrentInstRecallEntry().mInstNameTag = INST_LDM_2;
#endif
        
        for (CoreRegNum i = CR_BEGIN; i != CR_15; ++i)
        {
          if (true == lsm->RegEnable(i))
          {
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mEnable[i] = true;
            gp_core->CurrentInstRecallEntry().mVaddr[i] = address;
            gp_core->CurrentInstRecallEntry().mCurrentRegNum = i;
#endif
            
            // :NOTE: Wei 2004-Jul-30:
            //
            // In the original throw exception version, if get_data() throws an exception,
            // 'gp_core->rUsrReg(i)' will not be modified.
            //
            // Thus I have to use a temporary variable to store the result.
            
            // Use 'user' register bank
            uint32_t const temp = 
              gp_core->template get_data<WORD, DATA>(address,
                                                         gp_core->CurrentMode(),
                                                         exception_occur);
            
            if (true == exception_occur)
            {
              return;
            }
            else
            {
              gp_core->rUsrReg(i) = temp;
            }
            
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mValue[i] = gp_core->UsrReg(i);
#endif
            
            address += 4;
          }
        }
        
#if CHECK_LSM_END_ADDR
        assert((address - 4) == lsm->mEndAddr);
#endif
        
        gp_core->IncrePC();
      }
      else
      {
        // LDM(3)
        
#if CHECK_UNPREDICTABLE
        lsm->CheckBaseRegisterWriteBack();
#endif
        
#if ENABLE_INST_RECALL
        gp_core->CurrentInstRecallEntry().mInstNameTag = INST_LDM_3;
#endif
        
        // :NOTE: Wei 2004-May-27:
        //
        // Remember the current mode when ldm_3 instruction is executed,
        // because of following code may changing it.
        CoreMode const current_mode = gp_core->CurrentMode();
        
        for (CoreRegNum i = CR_BEGIN; i != CR_15; ++i)
        {
          if (true == lsm->RegEnable(i))
          {
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mEnable[i] = true;
            gp_core->CurrentInstRecallEntry().mVaddr[i] = address;
            gp_core->CurrentInstRecallEntry().mCurrentRegNum = i;
#endif
            
            // :NOTE: Wei 2004-Jul-30:
            //
            // In the original throw exception version, if get_data() throws an exception,
            // 'gp_core->rCurReg(i)' will not be modified.
            //
            // Thus I have to use a temporary variable to store the result.
            
            uint32_t const temp = 
              gp_core->template get_data<WORD, DATA>(address,
                                                         current_mode,
                                                         exception_occur);
            
            if (true == exception_occur)
            {
              return;
            }
            else
            {
              gp_core->rCurReg(i) = temp;
            }
            
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mValue[i] = gp_core->CurReg(i);
#endif
            
            address += 4;
          }
        }
        
        // This may change the cpu mode.
        gp_core->restore_cpsr();
        
#if ENABLE_INST_RECALL
        gp_core->CurrentInstRecallEntry().mEnable[CR_PC] = true;
        gp_core->CurrentInstRecallEntry().mVaddr[CR_PC] = address;
        gp_core->CurrentInstRecallEntry().mCurrentRegNum = CR_PC;
#endif
        
        // :NOTE: Wei 2004-Jul-30:
        //
        // In the original throw exception version, if get_data() throws an exception,
        // 'value' will not be modified.
        //
        // However, because of 'value' is a local variable,
        // thus I choose overwrite its value whether an exception occurs or not.
        
        // use old cpu mode.
        uint32_t const value =
          gp_core->template get_data<WORD, DATA>(address,
                                                     current_mode,
                                                     exception_occur);
        
        if (true == exception_occur)
        {
          return;
        }
        
#if CHECK_LSM_END_ADDR
        address += 4;
#endif
        
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3 || (CONFIG_ARCH_V4 && !CONFIG_T_VARIANT)
        gp_core->ModifyPC(value & 0xFFFFFFFC);
#elif (CONFIG_ARCH_V4 && CONFIG_T_VARIANT) || CONFIG_ARCH_V5
        if (gp_core->IsCpsrTbitSet())
        {
          gp_core->ModifyPC(value & 0xFFFFFFFE);
        }
        else
        {
#if CHECK_UNPREDICTABLE
          // :ARMARM: Wei 2003-Nov-6:
          //
          // If the SPSR T bit is 0 and bit[1] of the value loaded into the PC is 1,
          // the results are UNPREDICTABLE because it is not possible to branch to
          // an ARM instruction at a non word-aligned address.
          assert(0 == (value & 0x2));
#endif
          gp_core->ModifyPC(value & 0xFFFFFFFC);
        }
#endif
        
#if CHECK_LSM_END_ADDR
        assert((address - 4) == lsm->mEndAddr);
#endif
        
#if ENABLE_INST_RECALL
        gp_core->CurrentInstRecallEntry().mValue[CR_PC] = gp_core->CurReg(CR_PC);
#endif
        
#if ENABLE_THREADED_CODE
        if (USAGE_NORMAL == T_usage)
        {
          // Non-sequential next PC
          gp_core->finalize_chunk();
        }
#endif
      }
    }
    
    // STM(1)
    template<Direction direction, StartFrom start_from, IsWriteBack is_write_back, UsageEnum T_usage>
    inline void
    OperationInternal_Helper(InstLSM<direction,
                             start_from,
                             CURRENT_BANK,
                             is_write_back,
                             STORE,
                             T_usage> const * const lsm,
                             uint32_t const orig_rn_value, uint32_t address, bool &exception_occur)
    {
#if CHECK_UNPREDICTABLE
      lsm->CheckBaseRegisterWriteBack();
#endif
      
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_STM_1;
#endif
      
      for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
      {
        if (true == lsm->RegEnable(i))
        {
          // :ARM-DDI-0100E: p.A4-85: Wei 2003-Nov-6:
          //
          // If <Rn> is specified as <registers> and base register writeback is specified:
          //  - If <Rn> is the lowest-numbered register specified in <register_list>,
          //    the original value of <Rn> is stored.
          //  - Otherwise, the stored value of <Rn> is UNPREDICTABLE.
          //
          // :NOTE: Wei 2003-Nov-6:
          //
          // However, because of performance, I choose to store the original value of Rn
          // in both case, not to test this special condition.
          //
          // :NOTE: Wei 2004-Apr-26:
          //
          // STM_1 has this behavior,
          // STM_2 doesn't.
          
          if (i == lsm->RnNum())
          {
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mEnable[i] = true;
            gp_core->CurrentInstRecallEntry().mVaddr[i] = address;
            gp_core->CurrentInstRecallEntry().mCurrentRegNum = i;
#endif
            
            gp_core->template put_data<WORD>(address,
                                             orig_rn_value,
                                             gp_core->CurrentMode(),
                                             exception_occur);
            
            if (true == exception_occur)
            {
              return;
            }
            
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mValue[i] = orig_rn_value;
#endif
          }
          else
          {
            // :NOTE: Wei 2003-Dec-03:
            //
            // Because all kinds of Load Store Multi instructions claim that
            // Rn != CR_PC, thus the only possible place that i == CR_PC is in this
            // 'else' block rather than the above 'if' block.
            //
            // :ARM-DDI-0100E: p.A2-7: Wei 2003-Dec-03:
            //
            // An exception to the above rule occurs when an STR or STM instruction
            // stores R15. Such instructions can store either the address of the
            // instruction plus 8 bytes, like other instructions that read R15, or
            // the instruction's own address plus 12 bytes. Whether the offset of 8
            // or the offset of 12 is used is IMPLEMENTATION DEFINED. An implementation
            // must use the same offset for all STR and STM instructions that store R15.
            // It cannot use 8 for some of them and 12 for others.
            //
            // However, I cannot find a specified statement describe this
            // choice in the Intel StrongARM, thus I assume '+8' rather than
            // '+12'.
            
            uint32_t const store_value = ((CR_PC == i)
                                          ? (gp_core->ReadPC())
                                          : (gp_core->CurReg(i)));
            
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mEnable[i] = true;
            gp_core->CurrentInstRecallEntry().mVaddr[i] = address;
            gp_core->CurrentInstRecallEntry().mCurrentRegNum = i;
#endif
            
            gp_core->template put_data<WORD>(address,
                                             store_value,
                                             gp_core->CurrentMode(),
                                             exception_occur);
            
            if (true == exception_occur)
            {
              return;
            }
            
#if ENABLE_INST_RECALL
            gp_core->CurrentInstRecallEntry().mValue[i] = store_value;
#endif
          }
          
          address += 4;
        }
      }
      
#if CHECK_LSM_END_ADDR
      assert((address - 4) == lsm->mEndAddr);
#endif
      
      gp_core->IncrePC();
    }
    
    // STM(2)
    template<Direction direction, StartFrom start_from, IsWriteBack is_write_back, UsageEnum T_usage>
    inline void
    OperationInternal_Helper(InstLSM<direction,
                             start_from,
                             USER_BANK,
                             is_write_back,
                             STORE,
                             T_usage> const * const lsm,
                             uint32_t const /* orig_rn_value */, uint32_t address, bool &exception_occur)
    {
#if ENABLE_INST_RECALL
      gp_core->CurrentInstRecallEntry().mInstNameTag = INST_STM_2;
#endif
      
      for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
      {
        if (true == lsm->RegEnable(i))
        {
          // :ARM-DDI-0100E: p.A2-7: Wei 2003-Dec-03:
          //
          // An exception to the above rule occurs when an STR or STM instruction
          // stores R15. Such instructions can store either the address of the
          // instruction plus 8 bytes, like other instructions that read R15, or
          // the instruction's own address plus 12 bytes. Whether the offset of 8
          // or the offset of 12 is used is IMPLEMENTATION DEFINED. An implementation
          // must use the same offset for all STR and STM instructions that store R15.
          // It cannot use 8 for some of them and 12 for others.
          //
          // However, I cannot find a specified statement describe this
          // choice in the Intel StrongARM, thus I assume '+8' rather than
          // '+12'.
          
          // :NOTE: Wei 2003-Nov-1:
          //
          // Use 'user' register bank
          uint32_t const store_value = ((CR_PC == i)
                                        ? (gp_core->ReadPC())
                                        : (gp_core->UsrReg(i)));
          
#if ENABLE_INST_RECALL
          gp_core->CurrentInstRecallEntry().mEnable[i] = true;
          gp_core->CurrentInstRecallEntry().mVaddr[i] = address;
          gp_core->CurrentInstRecallEntry().mCurrentRegNum = i;
#endif
          
          gp_core->template put_data<WORD>(address,
                                               store_value,
                                               gp_core->CurrentMode(),
                                               exception_occur);
          
          if (true == exception_occur)
          {
            return;
          }
          
#if ENABLE_INST_RECALL
          gp_core->CurrentInstRecallEntry().mEnable[i] = true;
          gp_core->CurrentInstRecallEntry().mVaddr[i] = address;
          gp_core->CurrentInstRecallEntry().mCurrentRegNum = i;
#endif
          
          address += 4;
        }
      }
      
#if CHECK_LSM_END_ADDR
      assert((address - 4) == lsm->mEndAddr);
#endif
      
      gp_core->IncrePC();
    }
  }
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  OperationInternal(uint32_t const orig_rn_value, uint32_t address, bool &exception_occur) const
  {
    OperationInternal_Helper(this, orig_rn_value, address, exception_occur);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  Operation(bool &exception_occur) const
  {
#if CHECK_UNPREDICTABLE
    CheckProperMode();
#endif
    
#if ENABLE_INST_RECALL
    for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
    {
      gp_core->CurrentInstRecallEntry().mEnable[i] = false;
    }
#endif
    
    // :ARM-DDI-0100E: p.A2-18: Wei 2004-Feb-09:
    //
    // When data abort...
    //
    // If the instruction loads more than one general-purpose register,
    // UNPREDICTABLE values are left in destination registers which are
    // neither the PC nor the base register of the instruction.
    //
    // :SA-1100 Developer's Manual: p.41: Wei 2004-May-13:
    //
    // Block data transfer instructions (LDM, STM) abort on the first access
    // that cannot complete. If write-back is set, the base is NOT updated.
    // If the instruction would normally have overwritten the base with data
    // (for example, an LDM instruction with the base in the transfer list),
    // the original value in the base register is restored.
    //
    // :NOTE: Wei 2004-Feb-09:
    //
    // Thus I have to save the original Rn value whether writeback is specified or not,
    // in order to restore the original Rn value in the case of the Rn is specified in
    // the enable registers list and I already modified Rn, and then Data abort.
    uint32_t const orig_rn_value = Rn();
    uint32_t const address = StartAddress();
    
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
    
    OperationInternal(orig_rn_value, real_address, exception_occur);
    
    if (true == exception_occur)
    {
      gp_core->rCurReg(RnNum()) = orig_rn_value;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // check modify pc
  
  namespace
  {
    // LDM(1)
    template<Direction direction,
             StartFrom start_from,
             IsWriteBack is_write_back,
             UsageEnum T_usage>
    inline ExecResultEnum
    check_modify_pc_helper(InstLSM<direction,
                           start_from,
                           CURRENT_BANK,
                           is_write_back,
                           LOAD,
                           T_usage> const * const lsm)
    {
      // :NOTE: Wei 2004-Sep-20:
      //
      // Because the base register of the LSM instructions shouldn't be CR_PC,
      // thus I only need to consider the enable-bits of the LDM variants include
      // CR_PC or not.
      if (true == lsm->RegEnable(CR_PC))
      {
        return ER_NORMAL_MODIFY_PC;
      }
      else
      {
        return ER_NORMAL;
      }
    }
    
    template<Direction direction, StartFrom start_from, IsWriteBack is_write_back, UsageEnum T_usage>
    inline ExecResultEnum
    check_modify_pc_helper(InstLSM<direction,
                           start_from,
                           USER_BANK,
                           is_write_back,
                           LOAD,
                           T_usage> const * const lsm)
    {
      if (false == lsm->RegEnable(CR_PC))
      {
        // LDM(2)
        
        return ER_NORMAL;
      }
      else
      {
        // LDM(3)
        
        return ER_NORMAL_MODIFY_PC;
      }
    }
    
    // STM
    template<Direction direction,
             StartFrom start_from,
             DesiredBank desired_bank,
             IsWriteBack is_write_back,
             UsageEnum T_usage>
    inline ExecResultEnum
    check_modify_pc_helper(InstLSM<direction,
                           start_from,
                           desired_bank,
                           is_write_back,
                           STORE,
                           T_usage> const * const /* lsm */)
    {
      // :NOTE: Wei 2004-Sep-20:
      //
      // Because the base register of the LSM instructions shouldn't be CR_PC,
      // thus the STM variants shouldn't modify PC.
      return ER_NORMAL;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Process
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline ExecResultEnum
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
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
        g_log_file << "--> <LSM> "
                   << std::endl;
      }
      
      if (CURRENT_BANK == desired_bank)
      {
        gp_core->dump_cpu_info<false>();
      }
      else
      {
        gp_core->dump_cpu_info<true>();
      }
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
