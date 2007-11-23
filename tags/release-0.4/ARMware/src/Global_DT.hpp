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

#ifndef Global_DT_hpp
#define Global_DT_hpp

#include "ARMware.hpp"

#include "Inst/Inst.hpp"

#include "Inst/CDP.hpp"
#include "Inst/CDT.hpp"
#include "Inst/InstDPI.hpp"
#include "Inst/InstLSM.hpp"
#include "Inst/InstLSW.hpp"
#include "Inst/InstLSH.hpp"
#include "Inst/InstSWP.hpp"
#include "Inst/InstBranch.hpp"
#include "Inst/InstMulti.hpp"
#include "Inst/InstMRS.hpp"
#include "Inst/InstMSR.hpp"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // dynamic translator version
  
#if ENABLE_DYNAMIC_TRANSLATOR
  extern ExecResultEnum check_periphery_DT();
  
  // STM - dynamic translator version
  
  extern void exec_STM_1_DA_DT(Inst const inst, bool const is_last);
  extern void exec_STM_1_DA_w_DT(Inst const inst, bool const is_last);
  extern void exec_STM_2_DA_DT(Inst const inst, bool const is_last);
  
  extern void exec_STM_1_DB_DT(Inst const inst, bool const is_last);
  extern void exec_STM_1_DB_w_DT(Inst const inst, bool const is_last);
  extern void exec_STM_2_DB_DT(Inst const inst, bool const is_last);
  
  extern void exec_STM_1_IA_DT(Inst const inst, bool const is_last);
  extern void exec_STM_1_IA_w_DT(Inst const inst, bool const is_last);
  extern void exec_STM_2_IA_DT(Inst const inst, bool const is_last);
  
  extern void exec_STM_1_IB_DT(Inst const inst, bool const is_last);
  extern void exec_STM_1_IB_w_DT(Inst const inst, bool const is_last);
  extern void exec_STM_2_IB_DT(Inst const inst, bool const is_last);
  
  // LDM - dynamic translator version
  
  extern void exec_LDM_1_DA_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_1_DA_w_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_DA_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_DA_w_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDM_1_DB_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_1_DB_w_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_DB_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_DB_w_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDM_1_IA_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_1_IA_w_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_IA_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_IA_w_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDM_1_IB_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_1_IB_w_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_IB_DT(Inst const inst, bool const is_last);
  extern void exec_LDM_23_IB_w_DT(Inst const inst, bool const is_last);
  
  // SWP - dynamic translator version
  
  extern void exec_SWP_DT(Inst const inst, bool const is_last);
  extern void exec_SWPB_DT(Inst const inst, bool const is_last);
  
  // STR - dynamic translator version
  
  extern void exec_STR_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STR_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_STR_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_STR_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_STR_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_STR_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_STR_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STR_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_STR_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_STR_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_STR_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_STR_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // STRT - dynamic translator version
  
  extern void exec_STRT_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRT_imm_post_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_STRT_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRT_reg_post_up_DT(Inst const inst, bool const is_last);
  
  // STRB - dynamic translator version
  
  extern void exec_STRB_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_STRB_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRB_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // STRBT - dynamic translator version
  
  extern void exec_STRBT_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRBT_imm_post_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_STRBT_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRBT_reg_post_up_DT(Inst const inst, bool const is_last);
  
  // LDR - dynamic translator version
  
  extern void exec_LDR_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDR_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDR_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // LDRT - dynamic translator version
  
  extern void exec_LDRT_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRT_imm_post_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDRT_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRT_reg_post_up_DT(Inst const inst, bool const is_last);
  
  // LDRB - dynamic translator version
  
  extern void exec_LDRB_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDRB_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRB_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // LDRBT - dynamic translator version
  
  extern void exec_LDRBT_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRBT_imm_post_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDRBT_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRBT_reg_post_up_DT(Inst const inst, bool const is_last);
  
  // STRH - dynamic translator version
  
  extern void exec_STRH_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_STRH_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_STRH_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // LDRH - dynamic translator version
  
  extern void exec_LDRH_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDRH_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRH_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // LDRSB - dynamic translator version
  
  extern void exec_LDRSB_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDRSB_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSB_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // LDRSH - dynamic translator version
  
  extern void exec_LDRSH_imm_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_imm_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_imm_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_imm_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_imm_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_imm_pre_up_DT(Inst const inst, bool const is_last);
  
  extern void exec_LDRSH_reg_post_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_reg_post_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_reg_offset_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_reg_offset_up_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_reg_pre_down_DT(Inst const inst, bool const is_last);
  extern void exec_LDRSH_reg_pre_up_DT(Inst const inst, bool const is_last);
  
  // DPI - dynamic translator version
  
  extern void exec_AND_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_AND_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_AND_imm_DT(Inst const inst, bool const is_last);
  extern void exec_ANDS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_ANDS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_ANDS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_EOR_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_EOR_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_EOR_imm_DT(Inst const inst, bool const is_last);
  extern void exec_EORS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_EORS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_EORS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_SUB_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_SUB_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_SUB_imm_DT(Inst const inst, bool const is_last);
  extern void exec_SUBS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_SUBS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_SUBS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_RSB_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_RSB_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_RSB_imm_DT(Inst const inst, bool const is_last);
  extern void exec_RSBS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_RSBS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_RSBS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_ADD_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_ADD_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_ADD_imm_DT(Inst const inst, bool const is_last);
  extern void exec_ADDS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_ADDS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_ADDS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_ADC_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_ADC_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_ADC_imm_DT(Inst const inst, bool const is_last);
  extern void exec_ADCS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_ADCS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_ADCS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_SBC_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_SBC_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_SBC_imm_DT(Inst const inst, bool const is_last);
  extern void exec_SBCS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_SBCS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_SBCS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_RSC_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_RSC_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_RSC_imm_DT(Inst const inst, bool const is_last);
  extern void exec_RSCS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_RSCS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_RSCS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_ORR_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_ORR_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_ORR_imm_DT(Inst const inst, bool const is_last);
  extern void exec_ORRS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_ORRS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_ORRS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_MOV_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_MOV_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_MOV_imm_DT(Inst const inst, bool const is_last);
  extern void exec_MOVS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_MOVS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_MOVS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_BIC_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_BIC_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_BIC_imm_DT(Inst const inst, bool const is_last);
  extern void exec_BICS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_BICS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_BICS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_MVN_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_MVN_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_MVN_imm_DT(Inst const inst, bool const is_last);
  extern void exec_MVNS_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_MVNS_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_MVNS_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_TST_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_TST_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_TST_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_TEQ_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_TEQ_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_TEQ_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_CMP_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_CMP_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_CMP_imm_DT(Inst const inst, bool const is_last);
  
  extern void exec_CMN_regimm_DT(Inst const inst, bool const is_last);
  extern void exec_CMN_regreg_DT(Inst const inst, bool const is_last);
  extern void exec_CMN_imm_DT(Inst const inst, bool const is_last);
  
  // Branch - dynamic translator version
  
  extern void exec_BLX_1_DT(Inst const inst, bool const is_last);
  extern void exec_BLX_2_DT(Inst const inst, bool const is_last);
  extern void exec_BX_DT(Inst const inst, bool const is_last);
  extern void exec_B_DT(Inst const inst, bool const is_last);
  extern void exec_BL_DT(Inst const inst, bool const is_last);
  
  // Multi - dynamic translator version
  
  extern void exec_MUL_DT(Inst const inst, bool const is_last);
  extern void exec_MULS_DT(Inst const inst, bool const is_last);
  extern void exec_MLA_DT(Inst const inst, bool const is_last);
  extern void exec_MLAS_DT(Inst const inst, bool const is_last);
  extern void exec_UMULL_DT(Inst const inst, bool const is_last);
  extern void exec_UMULLS_DT(Inst const inst, bool const is_last);
  extern void exec_UMLAL_DT(Inst const inst, bool const is_last);
  extern void exec_UMLALS_DT(Inst const inst, bool const is_last);
  extern void exec_SMULL_DT(Inst const inst, bool const is_last);
  extern void exec_SMULLS_DT(Inst const inst, bool const is_last);
  extern void exec_SMLAL_DT(Inst const inst, bool const is_last);
  extern void exec_SMLALS_DT(Inst const inst, bool const is_last);
  
  // MRS - dynamic translator version
  
  extern void exec_MRS_cpsr_DT(Inst const inst, bool const is_last);
  extern void exec_MRS_spsr_DT(Inst const inst, bool const is_last);
  
  // MSR - dynamic translator version
  
  extern void exec_MSR_reg_cpsr_DT(Inst const inst, bool const is_last);
  extern void exec_MSR_reg_spsr_DT(Inst const inst, bool const is_last);
  extern void exec_MSR_imm_cpsr_DT(Inst const inst, bool const is_last);
  extern void exec_MSR_imm_spsr_DT(Inst const inst, bool const is_last);
  
  // Undefined instruction - dynamic translator version
  
  extern void exec_UND_DT(Inst const inst, bool const is_last);
  extern void exec_CDP_DT(Inst const inst, bool const is_last);
  extern void exec_CDT_DT(Inst const inst, bool const is_last);
  extern void exec_SWI_DT(Inst const inst, bool const is_last);
#endif
}

#endif
