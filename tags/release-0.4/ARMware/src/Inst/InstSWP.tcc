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
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline CoreRegNum
  InstSWP<data_width_enum, T_usage>::
  RdNum() const
  {
    return static_cast<CoreRegNum>(mInst.mSWP.mRd);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline CoreRegNum
  InstSWP<data_width_enum, T_usage>::
  RmNum() const
  {
    return static_cast<CoreRegNum>(mInst.mSWP.mRm);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline CoreRegNum
  InstSWP<data_width_enum, T_usage>::
  RnNum() const
  {
    return static_cast<CoreRegNum>(mInst.mSWP.mRn);
  }
  
#if CHECK_UNPREDICTABLE
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline void
  InstSWP<data_width_enum, T_usage>::
  check_rn_require() const
  {
    assert(RnNum() != CR_PC);
    assert(RnNum() != RmNum());
    assert(RnNum() != RdNum());
  }
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline void
  InstSWP<data_width_enum, T_usage>::
  check_rm_require() const
  {
    assert(RmNum() != CR_PC);
  }
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline void
  InstSWP<data_width_enum, T_usage>::
  check_rd_require() const
  {
    assert(RdNum() != CR_PC);
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline uint32_t
  InstSWP<data_width_enum, T_usage>::
  Rn() const
  {
#if CHECK_UNPREDICTABLE
    check_rn_require();
#endif
    
    return gp_core->CurReg(RnNum());
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline uint32_t
  InstSWP<data_width_enum, T_usage>::
  Rm() const
  {
#if CHECK_UNPREDICTABLE
    check_rm_require();
#endif
    
    return gp_core->CurReg(RmNum());
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  namespace
  {
    template<UsageEnum T_usage>
    inline uint32_t
    Rotate_Helper(InstSWP<BYTE, T_usage> const * const /* swp */,
                  uint32_t const temp,
                  uint32_t const /* address */)
    {
      return temp;
    }
    
    template<UsageEnum T_usage>
    inline uint32_t
    Rotate_Helper(InstSWP<WORD, T_usage> const * const /* swp */,
                  uint32_t const temp,
                  uint32_t const address)
    {
      return RotateRight(temp, (address & DataWidthTraits<WORD>::AlignNeeds::IGNORE_BITMASK) << 3);
    }
  }
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline uint32_t
  InstSWP<data_width_enum, T_usage>::
  Rotate(uint32_t const temp, uint32_t const address) const
  {
    return Rotate_Helper(this, temp, address);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Operation
  
  // SWP or SWPB
  //
  // When aborting...
  //
  // :SA-1110 Developer's Manual: p.41: Wei 2003-Dec-09:
  //
  // The swap instruction is aborted as though it had not executed.
  //
  // :ARM-DDI-0100E: p.A4-103: Wei 2004-Feb-11:
  //
  // If a data abort is signaled on either the load access or the store access,
  // the loaded value is not written to <Rd>. If a data abort is signaled on
  // the load access, the store access does not occur.
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline void
  InstSWP<data_width_enum, T_usage>::
  Operation(bool &exception_occur) const
  {
#if CHECK_UNPREDICTABLE
    check_rd_require();
#endif
    
    assert(0 == (mInst.mRawData & 0xF00));
    
#if ENABLE_INST_RECALL
    gp_core->CurrentInstRecallEntry().mInstNameTag = INST_SWP;
#endif
    
    uint32_t const address = Rn();
    
    // :NOTE: Wei 2004-Jul-30:
    //
    // In the original throw exception version, if CheckAlign() throws an exception,
    // 'real_address' will not be modified.
    //
    // However, because of 'real_address' is a local variable,
    // thus I choose overwrite its value whether an exception occurs or not.
    uint32_t const real_address = gp_core->template CheckAlign<data_width_enum>(address, exception_occur);
    
    if (true == exception_occur)
    {
      return;
    }
    
    // :NOTE: Wei 2004-Jul-30:
    //
    // In the original throw exception version, if get_data() throws an exception,
    // 'temp' will not be modified.
    //
    // However, because of 'temp' is a local variable,
    // thus I choose overwrite its value whether an exception occurs or not.
    typename DataWidthTraits<data_width_enum>::UnsignedType temp =
      gp_core->template get_data<data_width_enum, DATA>(real_address, gp_core->CurrentMode(), exception_occur);
    
    if (true == exception_occur)
    {
      return;
    }
    
    temp = Rotate(temp, address);
    
    uint32_t const store_value = Rm();
    
    gp_core->template put_data<data_width_enum>(
      address,
      static_cast<typename DataWidthTraits<data_width_enum>::UnsignedType>(store_value),
      gp_core->CurrentMode(), exception_occur);
    
    if (true == exception_occur)
    {
      return;
    }
    
    gp_core->rCurReg(RdNum()) = temp;
    
    gp_core->IncrePC();
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Process
  
  template<DataWidthEnum data_width_enum, UsageEnum T_usage>
  inline ExecResultEnum
  InstSWP<data_width_enum, T_usage>::
  Process(Inst const inst)
  {
    mInst = inst;
    bool exception_occur = false;
    
#if CHECK_CHUNK_CODE
    assert(inst.mRawData == gp_core->get_curr_inst());
#endif
    
#if DUMP_CPU_INFO_MORE_DETAIL
    if (true == gp_core->is_exec_in_chunk())
    {
      g_log_file << "--> <SWP> "
                 << std::endl;
    }
    
    gp_core->dump_cpu_info<false>();
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
        if (0 == gp_core->curr_chunk())
        {
          return ER_CHUNK_DISAPPEAR;
        }
      }
      
      // :NOTE: Wei 2004-Sep-20:
      //
      // All Rd register in SWP instructions shouldn't be CR_PC.
      return ER_NORMAL;
    }
  }  
}
