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

#ifndef ARMInstInfo_hpp
#define ARMInstInfo_hpp

#include "MemoryPool2.hpp"
#include "ARMware.hpp"

#include "Inst/Inst.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Core;
  class MIR;
  
  typedef class ARMInstInfo ARMInstInfo;
  class ARMInstInfo
  {
  private:
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Used in threaded code executing.
    typedef ExecResultEnum (*FunctionPtr_TC)(Inst const);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    // :NOTE: Wei 2004-Nov-4:
    //
    // Used in dynamic recompiled code generating.
    typedef void (*FunctionPtr_DT)(Inst const, bool const);
#endif
    
#if PRINT_CHUNK_CODE
    // :NOTE: Wei 2004-Nov-4:
    //
    // Used in ARM instruction printing.
    typedef void (*FunctionPtr_PRINT)(Inst const);
#endif
    
  public:
    
    struct ArgList
    {
      // Attribute
      
      FunctionPtr_TC const mp_func_TC;
#if ENABLE_DYNAMIC_TRANSLATOR
      FunctionPtr_DT const mp_func_DT;
#endif
#if PRINT_CHUNK_CODE
      FunctionPtr_PRINT const mp_func_PRINT;
#endif
      
      // Life cycle
      
      ArgList(FunctionPtr_TC const func_TC
#if ENABLE_DYNAMIC_TRANSLATOR
              , FunctionPtr_DT const func_DT
#endif
#if PRINT_CHUNK_CODE
              ,FunctionPtr_PRINT const func_PRINT
#endif
              )
        : mp_func_TC(func_TC)
#if ENABLE_DYNAMIC_TRANSLATOR
          , mp_func_DT(func_DT)
#endif
#if PRINT_CHUNK_CODE
          , mp_func_PRINT(func_PRINT)
#endif
      { }
    };
    typedef struct ArgList ArgList;
    
  private:
    
    Inst m_inst;
    
#if ENABLE_DYNAMIC_TRANSLATOR
    // :NOTE: Wei 2004-Nov-4:
    //
    // The first MIR instruction of this ARM instruction,
    // maybe the MIR for the condition code braching.
    MIR *mp_first_mir;
    
    // :NOTE: Wei 2005-Apr-1:
    //
    // The first normal MIR instruction of this ARM instruction.
    MIR *mp_first_normal_mir;
#endif
    
    // :NOTE: Wei 2005-Mar-24:
    //
    // I don't need to delete mp_arg_list in the destructor,
    // because each ArgList object is not dynamic.
    ArgList const * const mp_arg_list;
    
  public:
    
    // Life cycle
    
    inline
    ARMInstInfo(Inst const inst, ArgList const * const arg_list)
      : m_inst(inst),
#if ENABLE_DYNAMIC_TRANSLATOR
        // :NOTE: Wei 2004-Nov-4:
        //
        // We need to set the value of 'mp_first_mir' & 'mp_first_normal_mir' to 0,
        // becuase Chunk::append_mir() will use this 0 to decide
        // whether we have already set the first MIR instruction
        // of this ARM instruction or not.
        mp_first_mir(0),
        mp_first_normal_mir(0),
#endif
        mp_arg_list(arg_list)
    { }
    
    // Access
    
#if ENABLE_DYNAMIC_TRANSLATOR
    inline void
    set_first_mir(MIR * const mir)
    {
      assert(0 == mp_first_mir);
      
      mp_first_mir = mir;
    }
    
    inline void
    set_first_normal_mir(MIR * const mir)
    {
      assert(0 == mp_first_normal_mir);
      
      mp_first_normal_mir = mir;
    }
#endif
    
    // Inquiry
    
    inline FunctionPtr_TC
    func_TC() const
    { return mp_arg_list->mp_func_TC; }
    
#if ENABLE_DYNAMIC_TRANSLATOR
    inline FunctionPtr_DT
    func_DT() const
    { return mp_arg_list->mp_func_DT; }
#endif
    
#if PRINT_CHUNK_CODE
    inline FunctionPtr_PRINT
    func_PRINT() const
    { return mp_arg_list->mp_func_PRINT; }
#endif
    
    inline Inst
    inst() const
    { return m_inst; }
    
#if ENABLE_DYNAMIC_TRANSLATOR
    inline MIR *
    first_mir() const
    { return mp_first_mir; }
    
    inline MIR *
    first_normal_mir() const
    { return mp_first_normal_mir; }
#endif
  };
  
  // :NOTE: Wei 2004-Sep-30:
  //
  // Smaller chunk will use more often.
  struct ARMInstInfo_10  : public MemoryPool2<ARMInstInfo_10,  9128> { ARMInstInfo m_infos[ 10]; };
  struct ARMInstInfo_20  : public MemoryPool2<ARMInstInfo_20,  4096> { ARMInstInfo m_infos[ 20]; };
  struct ARMInstInfo_30  : public MemoryPool2<ARMInstInfo_30,  4096> { ARMInstInfo m_infos[ 30]; };
  struct ARMInstInfo_40  : public MemoryPool2<ARMInstInfo_40,  1024> { ARMInstInfo m_infos[ 40]; };
  struct ARMInstInfo_50  : public MemoryPool2<ARMInstInfo_50,  1024> { ARMInstInfo m_infos[ 50]; };
  struct ARMInstInfo_60  : public MemoryPool2<ARMInstInfo_60,   512> { ARMInstInfo m_infos[ 60]; };
  struct ARMInstInfo_70  : public MemoryPool2<ARMInstInfo_70,   512> { ARMInstInfo m_infos[ 70]; };
  struct ARMInstInfo_80  : public MemoryPool2<ARMInstInfo_80,   512> { ARMInstInfo m_infos[ 80]; };
  struct ARMInstInfo_90  : public MemoryPool2<ARMInstInfo_90,   512> { ARMInstInfo m_infos[ 90]; };
  struct ARMInstInfo_100 : public MemoryPool2<ARMInstInfo_100,  128> { ARMInstInfo m_infos[100]; };
  struct ARMInstInfo_110 : public MemoryPool2<ARMInstInfo_110,  128> { ARMInstInfo m_infos[110]; };
  struct ARMInstInfo_120 : public MemoryPool2<ARMInstInfo_120,  128> { ARMInstInfo m_infos[120]; };
  struct ARMInstInfo_130 : public MemoryPool2<ARMInstInfo_130,  128> { ARMInstInfo m_infos[130]; };
  struct ARMInstInfo_140 : public MemoryPool2<ARMInstInfo_140,  128> { ARMInstInfo m_infos[140]; };
  struct ARMInstInfo_150 : public MemoryPool2<ARMInstInfo_150,   64> { ARMInstInfo m_infos[150]; };
  struct ARMInstInfo_160 : public MemoryPool2<ARMInstInfo_160,   64> { ARMInstInfo m_infos[160]; };
  struct ARMInstInfo_170 : public MemoryPool2<ARMInstInfo_170,   64> { ARMInstInfo m_infos[170]; };
  struct ARMInstInfo_180 : public MemoryPool2<ARMInstInfo_180,   64> { ARMInstInfo m_infos[180]; };
  struct ARMInstInfo_190 : public MemoryPool2<ARMInstInfo_190,   64> { ARMInstInfo m_infos[190]; };
  struct ARMInstInfo_200 : public MemoryPool2<ARMInstInfo_200,   64> { ARMInstInfo m_infos[200]; };
  
  // STM
  
  extern ARMInstInfo::ArgList g_STM_1_DA_arglist;
  extern ARMInstInfo::ArgList g_STM_1_DA_w_arglist;
  extern ARMInstInfo::ArgList g_STM_2_DA_arglist;
  
  extern ARMInstInfo::ArgList g_STM_1_DB_arglist;
  extern ARMInstInfo::ArgList g_STM_1_DB_w_arglist;
  extern ARMInstInfo::ArgList g_STM_2_DB_arglist;
  
  extern ARMInstInfo::ArgList g_STM_1_IA_arglist;
  extern ARMInstInfo::ArgList g_STM_1_IA_w_arglist;
  extern ARMInstInfo::ArgList g_STM_2_IA_arglist;
  
  extern ARMInstInfo::ArgList g_STM_1_IB_arglist;
  extern ARMInstInfo::ArgList g_STM_1_IB_w_arglist;
  extern ARMInstInfo::ArgList g_STM_2_IB_arglist;
  
  // LDM
  
  extern ARMInstInfo::ArgList g_LDM_1_DA_arglist;
  extern ARMInstInfo::ArgList g_LDM_1_DA_w_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_DA_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_DA_w_arglist;

  extern ARMInstInfo::ArgList g_LDM_1_DB_arglist;
  extern ARMInstInfo::ArgList g_LDM_1_DB_w_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_DB_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_DB_w_arglist;

  extern ARMInstInfo::ArgList g_LDM_1_IA_arglist;
  extern ARMInstInfo::ArgList g_LDM_1_IA_w_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_IA_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_IA_w_arglist;
  
  extern ARMInstInfo::ArgList g_LDM_1_IB_arglist;
  extern ARMInstInfo::ArgList g_LDM_1_IB_w_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_IB_arglist;
  extern ARMInstInfo::ArgList g_LDM_23_IB_w_arglist;
    
  // SWP
    
  extern ARMInstInfo::ArgList g_SWP_arglist;
  extern ARMInstInfo::ArgList g_SWPB_arglist;
    
  // STR
    
  extern ARMInstInfo::ArgList g_STR_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_STR_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_STR_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_STR_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_STR_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_STR_imm_pre_up_arglist;

  extern ARMInstInfo::ArgList g_STR_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_STR_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_STR_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_STR_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_STR_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_STR_reg_pre_up_arglist;
  
  // STRT
  
  extern ARMInstInfo::ArgList g_STRT_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRT_imm_post_up_arglist;

  extern ARMInstInfo::ArgList g_STRT_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRT_reg_post_up_arglist;
    
  // STRB
    
  extern ARMInstInfo::ArgList g_STRB_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRB_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_STRB_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_STRB_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_STRB_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_STRB_imm_pre_up_arglist;

  extern ARMInstInfo::ArgList g_STRB_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRB_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_STRB_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_STRB_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_STRB_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_STRB_reg_pre_up_arglist;
    
  // STRBT
    
  extern ARMInstInfo::ArgList g_STRBT_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRBT_imm_post_up_arglist;

  extern ARMInstInfo::ArgList g_STRBT_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRBT_reg_post_up_arglist;
    
  // LDR
    
  extern ARMInstInfo::ArgList g_LDR_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDR_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDR_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDR_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDR_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDR_imm_pre_up_arglist;

  extern ARMInstInfo::ArgList g_LDR_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDR_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDR_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDR_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDR_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDR_reg_pre_up_arglist;
    
  // LDRT
    
  extern ARMInstInfo::ArgList g_LDRT_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRT_imm_post_up_arglist;

  extern ARMInstInfo::ArgList g_LDRT_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRT_reg_post_up_arglist;
    
  // LDRB
    
  extern ARMInstInfo::ArgList g_LDRB_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRB_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRB_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRB_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRB_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRB_imm_pre_up_arglist;

  extern ARMInstInfo::ArgList g_LDRB_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRB_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRB_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRB_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRB_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRB_reg_pre_up_arglist;
    
  // LDRBT
    
  extern ARMInstInfo::ArgList g_LDRBT_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRBT_imm_post_up_arglist;

  extern ARMInstInfo::ArgList g_LDRBT_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRBT_reg_post_up_arglist;
    
  // STRH
    
  extern ARMInstInfo::ArgList g_STRH_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRH_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_STRH_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_STRH_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_STRH_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_STRH_reg_pre_up_arglist;

  extern ARMInstInfo::ArgList g_STRH_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_STRH_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_STRH_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_STRH_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_STRH_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_STRH_imm_pre_up_arglist;
    
  // LDRH
    
  extern ARMInstInfo::ArgList g_LDRH_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRH_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRH_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRH_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRH_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRH_reg_pre_up_arglist;

  extern ARMInstInfo::ArgList g_LDRH_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRH_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRH_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRH_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRH_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRH_imm_pre_up_arglist;
    
  // LDRSB
    
  extern ARMInstInfo::ArgList g_LDRSB_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_reg_pre_up_arglist;

  extern ARMInstInfo::ArgList g_LDRSB_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSB_imm_pre_up_arglist;
    
  // LDRSH
    
  extern ARMInstInfo::ArgList g_LDRSH_reg_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_reg_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_reg_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_reg_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_reg_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_reg_pre_up_arglist;

  extern ARMInstInfo::ArgList g_LDRSH_imm_post_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_imm_post_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_imm_offset_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_imm_offset_up_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_imm_pre_down_arglist;
  extern ARMInstInfo::ArgList g_LDRSH_imm_pre_up_arglist;
    
  // DPI
    
  extern ARMInstInfo::ArgList g_AND_regimm_arglist;
  extern ARMInstInfo::ArgList g_AND_regreg_arglist;
  extern ARMInstInfo::ArgList g_AND_imm_arglist;
  extern ARMInstInfo::ArgList g_ANDS_regimm_arglist;
  extern ARMInstInfo::ArgList g_ANDS_regreg_arglist;
  extern ARMInstInfo::ArgList g_ANDS_imm_arglist;

  extern ARMInstInfo::ArgList g_EOR_regimm_arglist;
  extern ARMInstInfo::ArgList g_EOR_regreg_arglist;
  extern ARMInstInfo::ArgList g_EOR_imm_arglist;
  extern ARMInstInfo::ArgList g_EORS_regimm_arglist;
  extern ARMInstInfo::ArgList g_EORS_regreg_arglist;
  extern ARMInstInfo::ArgList g_EORS_imm_arglist;

  extern ARMInstInfo::ArgList g_SUB_regimm_arglist;
  extern ARMInstInfo::ArgList g_SUB_regreg_arglist;
  extern ARMInstInfo::ArgList g_SUB_imm_arglist;
  extern ARMInstInfo::ArgList g_SUBS_regimm_arglist;
  extern ARMInstInfo::ArgList g_SUBS_regreg_arglist;
  extern ARMInstInfo::ArgList g_SUBS_imm_arglist;

  extern ARMInstInfo::ArgList g_RSB_regimm_arglist;
  extern ARMInstInfo::ArgList g_RSB_regreg_arglist;
  extern ARMInstInfo::ArgList g_RSB_imm_arglist;
  extern ARMInstInfo::ArgList g_RSBS_regimm_arglist;
  extern ARMInstInfo::ArgList g_RSBS_regreg_arglist;
  extern ARMInstInfo::ArgList g_RSBS_imm_arglist;

  extern ARMInstInfo::ArgList g_ADD_regimm_arglist;
  extern ARMInstInfo::ArgList g_ADD_regreg_arglist;
  extern ARMInstInfo::ArgList g_ADD_imm_arglist;
  extern ARMInstInfo::ArgList g_ADDS_regimm_arglist;
  extern ARMInstInfo::ArgList g_ADDS_regreg_arglist;
  extern ARMInstInfo::ArgList g_ADDS_imm_arglist;

  extern ARMInstInfo::ArgList g_ADC_regimm_arglist;
  extern ARMInstInfo::ArgList g_ADC_regreg_arglist;
  extern ARMInstInfo::ArgList g_ADC_imm_arglist;
  extern ARMInstInfo::ArgList g_ADCS_regimm_arglist;
  extern ARMInstInfo::ArgList g_ADCS_regreg_arglist;
  extern ARMInstInfo::ArgList g_ADCS_imm_arglist;

  extern ARMInstInfo::ArgList g_SBC_regimm_arglist;
  extern ARMInstInfo::ArgList g_SBC_regreg_arglist;
  extern ARMInstInfo::ArgList g_SBC_imm_arglist;
  extern ARMInstInfo::ArgList g_SBCS_regimm_arglist;
  extern ARMInstInfo::ArgList g_SBCS_regreg_arglist;
  extern ARMInstInfo::ArgList g_SBCS_imm_arglist;

  extern ARMInstInfo::ArgList g_RSC_regimm_arglist;
  extern ARMInstInfo::ArgList g_RSC_regreg_arglist;
  extern ARMInstInfo::ArgList g_RSC_imm_arglist;
  extern ARMInstInfo::ArgList g_RSCS_regimm_arglist;
  extern ARMInstInfo::ArgList g_RSCS_regreg_arglist;
  extern ARMInstInfo::ArgList g_RSCS_imm_arglist;

  extern ARMInstInfo::ArgList g_ORR_regimm_arglist;
  extern ARMInstInfo::ArgList g_ORR_regreg_arglist;
  extern ARMInstInfo::ArgList g_ORR_imm_arglist;
  extern ARMInstInfo::ArgList g_ORRS_regimm_arglist;
  extern ARMInstInfo::ArgList g_ORRS_regreg_arglist;
  extern ARMInstInfo::ArgList g_ORRS_imm_arglist;

  extern ARMInstInfo::ArgList g_MOV_regimm_arglist;
  extern ARMInstInfo::ArgList g_MOV_regreg_arglist;
  extern ARMInstInfo::ArgList g_MOV_imm_arglist;
  extern ARMInstInfo::ArgList g_MOVS_regimm_arglist;
  extern ARMInstInfo::ArgList g_MOVS_regreg_arglist;
  extern ARMInstInfo::ArgList g_MOVS_imm_arglist;

  extern ARMInstInfo::ArgList g_BIC_regimm_arglist;
  extern ARMInstInfo::ArgList g_BIC_regreg_arglist;
  extern ARMInstInfo::ArgList g_BIC_imm_arglist;
  extern ARMInstInfo::ArgList g_BICS_regimm_arglist;
  extern ARMInstInfo::ArgList g_BICS_regreg_arglist;
  extern ARMInstInfo::ArgList g_BICS_imm_arglist;

  extern ARMInstInfo::ArgList g_MVN_regimm_arglist;
  extern ARMInstInfo::ArgList g_MVN_regreg_arglist;
  extern ARMInstInfo::ArgList g_MVN_imm_arglist;
  extern ARMInstInfo::ArgList g_MVNS_regimm_arglist;
  extern ARMInstInfo::ArgList g_MVNS_regreg_arglist;
  extern ARMInstInfo::ArgList g_MVNS_imm_arglist;

  extern ARMInstInfo::ArgList g_TST_regimm_arglist;
  extern ARMInstInfo::ArgList g_TST_regreg_arglist;
  extern ARMInstInfo::ArgList g_TST_imm_arglist;

  extern ARMInstInfo::ArgList g_TEQ_regimm_arglist;
  extern ARMInstInfo::ArgList g_TEQ_regreg_arglist;
  extern ARMInstInfo::ArgList g_TEQ_imm_arglist;

  extern ARMInstInfo::ArgList g_CMP_regimm_arglist;
  extern ARMInstInfo::ArgList g_CMP_regreg_arglist;
  extern ARMInstInfo::ArgList g_CMP_imm_arglist;

  extern ARMInstInfo::ArgList g_CMN_regimm_arglist;
  extern ARMInstInfo::ArgList g_CMN_regreg_arglist;
  extern ARMInstInfo::ArgList g_CMN_imm_arglist;
    
  // Branch
    
  extern ARMInstInfo::ArgList g_BLX_1_arglist;
  extern ARMInstInfo::ArgList g_BLX_2_arglist;
  extern ARMInstInfo::ArgList g_BX_arglist;
  extern ARMInstInfo::ArgList g_B_arglist;
  extern ARMInstInfo::ArgList g_BL_arglist;
    
  // Multi
    
  extern ARMInstInfo::ArgList g_MUL_arglist;
  extern ARMInstInfo::ArgList g_MULS_arglist;
  extern ARMInstInfo::ArgList g_MLA_arglist;
  extern ARMInstInfo::ArgList g_MLAS_arglist;
  extern ARMInstInfo::ArgList g_UMULL_arglist;
  extern ARMInstInfo::ArgList g_UMULLS_arglist;
  extern ARMInstInfo::ArgList g_UMLAL_arglist;
  extern ARMInstInfo::ArgList g_UMLALS_arglist;
  extern ARMInstInfo::ArgList g_SMULL_arglist;
  extern ARMInstInfo::ArgList g_SMULLS_arglist;
  extern ARMInstInfo::ArgList g_SMLAL_arglist;
  extern ARMInstInfo::ArgList g_SMLALS_arglist;
  
  // MRS
  
  extern ARMInstInfo::ArgList g_MRS_cpsr_arglist;
  extern ARMInstInfo::ArgList g_MRS_spsr_arglist;
  
  // MSR
  
  extern ARMInstInfo::ArgList g_MSR_reg_cpsr_arglist;
  extern ARMInstInfo::ArgList g_MSR_reg_spsr_arglist;
  extern ARMInstInfo::ArgList g_MSR_imm_cpsr_arglist;
  extern ARMInstInfo::ArgList g_MSR_imm_spsr_arglist;
  
  // CDT
  
  extern ARMInstInfo::ArgList g_CDT_arglist;
  
  // CDP
  
  extern ARMInstInfo::ArgList g_CDP_arglist;
  
  // CRT<LOAD>
  
  extern ARMInstInfo::ArgList g_CRT_LOAD_arglist;
  
  // CRT<STORE>
  
  extern ARMInstInfo::ArgList g_CRT_STORE_arglist;
  
  // SWI
  
  extern ARMInstInfo::ArgList g_SWI_arglist;
  
  // UND
  
  extern ARMInstInfo::ArgList g_UND_arglist;
}

#endif
