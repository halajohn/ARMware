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

#ifndef Global_PRINT_hpp
#define Global_PRINT_hpp

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
#if PRINT_CHUNK_CODE
  // STM
  
  extern void exec_STM_1_DA_PRINT(Inst const inst);
  extern void exec_STM_1_DA_w_PRINT(Inst const inst);
  extern void exec_STM_2_DA_PRINT(Inst const inst);
  
  extern void exec_STM_1_DB_PRINT(Inst const inst);
  extern void exec_STM_1_DB_w_PRINT(Inst const inst);
  extern void exec_STM_2_DB_PRINT(Inst const inst);
  
  extern void exec_STM_1_IA_PRINT(Inst const inst);
  extern void exec_STM_1_IA_w_PRINT(Inst const inst);
  extern void exec_STM_2_IA_PRINT(Inst const inst);
  
  extern void exec_STM_1_IB_PRINT(Inst const inst);
  extern void exec_STM_1_IB_w_PRINT(Inst const inst);
  extern void exec_STM_2_IB_PRINT(Inst const inst);
  
  // LDM
  
  extern void exec_LDM_1_DA_PRINT(Inst const inst);
  extern void exec_LDM_1_DA_w_PRINT(Inst const inst);
  extern void exec_LDM_23_DA_PRINT(Inst const inst);
  extern void exec_LDM_23_DA_w_PRINT(Inst const inst);
  
  extern void exec_LDM_1_DB_PRINT(Inst const inst);
  extern void exec_LDM_1_DB_w_PRINT(Inst const inst);
  extern void exec_LDM_23_DB_PRINT(Inst const inst);
  extern void exec_LDM_23_DB_w_PRINT(Inst const inst);
  
  extern void exec_LDM_1_IA_PRINT(Inst const inst);
  extern void exec_LDM_1_IA_w_PRINT(Inst const inst);
  extern void exec_LDM_23_IA_PRINT(Inst const inst);
  extern void exec_LDM_23_IA_w_PRINT(Inst const inst);
  
  extern void exec_LDM_1_IB_PRINT(Inst const inst);
  extern void exec_LDM_1_IB_w_PRINT(Inst const inst);
  extern void exec_LDM_23_IB_PRINT(Inst const inst);
  extern void exec_LDM_23_IB_w_PRINT(Inst const inst);
  
  // SWP
  
  extern void exec_SWP_PRINT(Inst const inst);
  extern void exec_SWPB_PRINT(Inst const inst);
  
  // STR
  
  extern void exec_STR_imm_post_down_PRINT(Inst const inst);
  extern void exec_STR_imm_post_up_PRINT(Inst const inst);
  extern void exec_STR_imm_offset_down_PRINT(Inst const inst);
  extern void exec_STR_imm_offset_up_PRINT(Inst const inst);
  extern void exec_STR_imm_pre_down_PRINT(Inst const inst);
  extern void exec_STR_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_STR_reg_post_down_PRINT(Inst const inst);
  extern void exec_STR_reg_post_up_PRINT(Inst const inst);
  extern void exec_STR_reg_offset_down_PRINT(Inst const inst);
  extern void exec_STR_reg_offset_up_PRINT(Inst const inst);
  extern void exec_STR_reg_pre_down_PRINT(Inst const inst);
  extern void exec_STR_reg_pre_up_PRINT(Inst const inst);
  
  // STRT
  
  extern void exec_STRT_imm_post_down_PRINT(Inst const inst);
  extern void exec_STRT_imm_post_up_PRINT(Inst const inst);
  
  extern void exec_STRT_reg_post_down_PRINT(Inst const inst);
  extern void exec_STRT_reg_post_up_PRINT(Inst const inst);
  
  // STRB
  
  extern void exec_STRB_imm_post_down_PRINT(Inst const inst);
  extern void exec_STRB_imm_post_up_PRINT(Inst const inst);
  extern void exec_STRB_imm_offset_down_PRINT(Inst const inst);
  extern void exec_STRB_imm_offset_up_PRINT(Inst const inst);
  extern void exec_STRB_imm_pre_down_PRINT(Inst const inst);
  extern void exec_STRB_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_STRB_reg_post_down_PRINT(Inst const inst);
  extern void exec_STRB_reg_post_up_PRINT(Inst const inst);
  extern void exec_STRB_reg_offset_down_PRINT(Inst const inst);
  extern void exec_STRB_reg_offset_up_PRINT(Inst const inst);
  extern void exec_STRB_reg_pre_down_PRINT(Inst const inst);
  extern void exec_STRB_reg_pre_up_PRINT(Inst const inst);
  
  // STRBT
  
  extern void exec_STRBT_imm_post_down_PRINT(Inst const inst);
  extern void exec_STRBT_imm_post_up_PRINT(Inst const inst);
  
  extern void exec_STRBT_reg_post_down_PRINT(Inst const inst);
  extern void exec_STRBT_reg_post_up_PRINT(Inst const inst);
  
  // LDR
  
  extern void exec_LDR_imm_post_down_PRINT(Inst const inst);
  extern void exec_LDR_imm_post_up_PRINT(Inst const inst);
  extern void exec_LDR_imm_offset_down_PRINT(Inst const inst);
  extern void exec_LDR_imm_offset_up_PRINT(Inst const inst);
  extern void exec_LDR_imm_pre_down_PRINT(Inst const inst);
  extern void exec_LDR_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_LDR_reg_post_down_PRINT(Inst const inst);
  extern void exec_LDR_reg_post_up_PRINT(Inst const inst);
  extern void exec_LDR_reg_offset_down_PRINT(Inst const inst);
  extern void exec_LDR_reg_offset_up_PRINT(Inst const inst);
  extern void exec_LDR_reg_pre_down_PRINT(Inst const inst);
  extern void exec_LDR_reg_pre_up_PRINT(Inst const inst);
  
  // LDRT
  
  extern void exec_LDRT_imm_post_down_PRINT(Inst const inst);
  extern void exec_LDRT_imm_post_up_PRINT(Inst const inst);
  
  extern void exec_LDRT_reg_post_down_PRINT(Inst const inst);
  extern void exec_LDRT_reg_post_up_PRINT(Inst const inst);
  
  // LDRB
  
  extern void exec_LDRB_imm_post_down_PRINT(Inst const inst);
  extern void exec_LDRB_imm_post_up_PRINT(Inst const inst);
  extern void exec_LDRB_imm_offset_down_PRINT(Inst const inst);
  extern void exec_LDRB_imm_offset_up_PRINT(Inst const inst);
  extern void exec_LDRB_imm_pre_down_PRINT(Inst const inst);
  extern void exec_LDRB_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_LDRB_reg_post_down_PRINT(Inst const inst);
  extern void exec_LDRB_reg_post_up_PRINT(Inst const inst);
  extern void exec_LDRB_reg_offset_down_PRINT(Inst const inst);
  extern void exec_LDRB_reg_offset_up_PRINT(Inst const inst);
  extern void exec_LDRB_reg_pre_down_PRINT(Inst const inst);
  extern void exec_LDRB_reg_pre_up_PRINT(Inst const inst);
  
  // LDRBT
  
  extern void exec_LDRBT_imm_post_down_PRINT(Inst const inst);
  extern void exec_LDRBT_imm_post_up_PRINT(Inst const inst);
  
  extern void exec_LDRBT_reg_post_down_PRINT(Inst const inst);
  extern void exec_LDRBT_reg_post_up_PRINT(Inst const inst);
  
  // STRH
  
  extern void exec_STRH_imm_post_down_PRINT(Inst const inst);
  extern void exec_STRH_imm_post_up_PRINT(Inst const inst);
  extern void exec_STRH_imm_offset_down_PRINT(Inst const inst);
  extern void exec_STRH_imm_offset_up_PRINT(Inst const inst);
  extern void exec_STRH_imm_pre_down_PRINT(Inst const inst);
  extern void exec_STRH_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_STRH_reg_post_down_PRINT(Inst const inst);
  extern void exec_STRH_reg_post_up_PRINT(Inst const inst);
  extern void exec_STRH_reg_offset_down_PRINT(Inst const inst);
  extern void exec_STRH_reg_offset_up_PRINT(Inst const inst);
  extern void exec_STRH_reg_pre_down_PRINT(Inst const inst);
  extern void exec_STRH_reg_pre_up_PRINT(Inst const inst);
  
  // LDRH
  
  extern void exec_LDRH_imm_post_down_PRINT(Inst const inst);
  extern void exec_LDRH_imm_post_up_PRINT(Inst const inst);
  extern void exec_LDRH_imm_offset_down_PRINT(Inst const inst);
  extern void exec_LDRH_imm_offset_up_PRINT(Inst const inst);
  extern void exec_LDRH_imm_pre_down_PRINT(Inst const inst);
  extern void exec_LDRH_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_LDRH_reg_post_down_PRINT(Inst const inst);
  extern void exec_LDRH_reg_post_up_PRINT(Inst const inst);
  extern void exec_LDRH_reg_offset_down_PRINT(Inst const inst);
  extern void exec_LDRH_reg_offset_up_PRINT(Inst const inst);
  extern void exec_LDRH_reg_pre_down_PRINT(Inst const inst);
  extern void exec_LDRH_reg_pre_up_PRINT(Inst const inst);
  
  // LDRSB
  
  extern void exec_LDRSB_imm_post_down_PRINT(Inst const inst);
  extern void exec_LDRSB_imm_post_up_PRINT(Inst const inst);
  extern void exec_LDRSB_imm_offset_down_PRINT(Inst const inst);
  extern void exec_LDRSB_imm_offset_up_PRINT(Inst const inst);
  extern void exec_LDRSB_imm_pre_down_PRINT(Inst const inst);
  extern void exec_LDRSB_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_LDRSB_reg_post_down_PRINT(Inst const inst);
  extern void exec_LDRSB_reg_post_up_PRINT(Inst const inst);
  extern void exec_LDRSB_reg_offset_down_PRINT(Inst const inst);
  extern void exec_LDRSB_reg_offset_up_PRINT(Inst const inst);
  extern void exec_LDRSB_reg_pre_down_PRINT(Inst const inst);
  extern void exec_LDRSB_reg_pre_up_PRINT(Inst const inst);
  
  // LDRSH
  
  extern void exec_LDRSH_imm_post_down_PRINT(Inst const inst);
  extern void exec_LDRSH_imm_post_up_PRINT(Inst const inst);
  extern void exec_LDRSH_imm_offset_down_PRINT(Inst const inst);
  extern void exec_LDRSH_imm_offset_up_PRINT(Inst const inst);
  extern void exec_LDRSH_imm_pre_down_PRINT(Inst const inst);
  extern void exec_LDRSH_imm_pre_up_PRINT(Inst const inst);
  
  extern void exec_LDRSH_reg_post_down_PRINT(Inst const inst);
  extern void exec_LDRSH_reg_post_up_PRINT(Inst const inst);
  extern void exec_LDRSH_reg_offset_down_PRINT(Inst const inst);
  extern void exec_LDRSH_reg_offset_up_PRINT(Inst const inst);
  extern void exec_LDRSH_reg_pre_down_PRINT(Inst const inst);
  extern void exec_LDRSH_reg_pre_up_PRINT(Inst const inst);
  
  // DPI
  
  extern void exec_AND_regimm_PRINT(Inst const inst);
  extern void exec_AND_regreg_PRINT(Inst const inst);
  extern void exec_AND_imm_PRINT(Inst const inst);
  extern void exec_ANDS_regimm_PRINT(Inst const inst);
  extern void exec_ANDS_regreg_PRINT(Inst const inst);
  extern void exec_ANDS_imm_PRINT(Inst const inst);
  
  extern void exec_EOR_regimm_PRINT(Inst const inst);
  extern void exec_EOR_regreg_PRINT(Inst const inst);
  extern void exec_EOR_imm_PRINT(Inst const inst);
  extern void exec_EORS_regimm_PRINT(Inst const inst);
  extern void exec_EORS_regreg_PRINT(Inst const inst);
  extern void exec_EORS_imm_PRINT(Inst const inst);
  
  extern void exec_SUB_regimm_PRINT(Inst const inst);
  extern void exec_SUB_regreg_PRINT(Inst const inst);
  extern void exec_SUB_imm_PRINT(Inst const inst);
  extern void exec_SUBS_regimm_PRINT(Inst const inst);
  extern void exec_SUBS_regreg_PRINT(Inst const inst);
  extern void exec_SUBS_imm_PRINT(Inst const inst);
  
  extern void exec_RSB_regimm_PRINT(Inst const inst);
  extern void exec_RSB_regreg_PRINT(Inst const inst);
  extern void exec_RSB_imm_PRINT(Inst const inst);
  extern void exec_RSBS_regimm_PRINT(Inst const inst);
  extern void exec_RSBS_regreg_PRINT(Inst const inst);
  extern void exec_RSBS_imm_PRINT(Inst const inst);
  
  extern void exec_ADD_regimm_PRINT(Inst const inst);
  extern void exec_ADD_regreg_PRINT(Inst const inst);
  extern void exec_ADD_imm_PRINT(Inst const inst);
  extern void exec_ADDS_regimm_PRINT(Inst const inst);
  extern void exec_ADDS_regreg_PRINT(Inst const inst);
  extern void exec_ADDS_imm_PRINT(Inst const inst);
  
  extern void exec_ADC_regimm_PRINT(Inst const inst);
  extern void exec_ADC_regreg_PRINT(Inst const inst);
  extern void exec_ADC_imm_PRINT(Inst const inst);
  extern void exec_ADCS_regimm_PRINT(Inst const inst);
  extern void exec_ADCS_regreg_PRINT(Inst const inst);
  extern void exec_ADCS_imm_PRINT(Inst const inst);
  
  extern void exec_SBC_regimm_PRINT(Inst const inst);
  extern void exec_SBC_regreg_PRINT(Inst const inst);
  extern void exec_SBC_imm_PRINT(Inst const inst);
  extern void exec_SBCS_regimm_PRINT(Inst const inst);
  extern void exec_SBCS_regreg_PRINT(Inst const inst);
  extern void exec_SBCS_imm_PRINT(Inst const inst);
  
  extern void exec_RSC_regimm_PRINT(Inst const inst);
  extern void exec_RSC_regreg_PRINT(Inst const inst);
  extern void exec_RSC_imm_PRINT(Inst const inst);
  extern void exec_RSCS_regimm_PRINT(Inst const inst);
  extern void exec_RSCS_regreg_PRINT(Inst const inst);
  extern void exec_RSCS_imm_PRINT(Inst const inst);
  
  extern void exec_ORR_regimm_PRINT(Inst const inst);
  extern void exec_ORR_regreg_PRINT(Inst const inst);
  extern void exec_ORR_imm_PRINT(Inst const inst);
  extern void exec_ORRS_regimm_PRINT(Inst const inst);
  extern void exec_ORRS_regreg_PRINT(Inst const inst);
  extern void exec_ORRS_imm_PRINT(Inst const inst);
  
  extern void exec_MOV_regimm_PRINT(Inst const inst);
  extern void exec_MOV_regreg_PRINT(Inst const inst);
  extern void exec_MOV_imm_PRINT(Inst const inst);
  extern void exec_MOVS_regimm_PRINT(Inst const inst);
  extern void exec_MOVS_regreg_PRINT(Inst const inst);
  extern void exec_MOVS_imm_PRINT(Inst const inst);
  
  extern void exec_BIC_regimm_PRINT(Inst const inst);
  extern void exec_BIC_regreg_PRINT(Inst const inst);
  extern void exec_BIC_imm_PRINT(Inst const inst);
  extern void exec_BICS_regimm_PRINT(Inst const inst);
  extern void exec_BICS_regreg_PRINT(Inst const inst);
  extern void exec_BICS_imm_PRINT(Inst const inst);
  
  extern void exec_MVN_regimm_PRINT(Inst const inst);
  extern void exec_MVN_regreg_PRINT(Inst const inst);
  extern void exec_MVN_imm_PRINT(Inst const inst);
  extern void exec_MVNS_regimm_PRINT(Inst const inst);
  extern void exec_MVNS_regreg_PRINT(Inst const inst);
  extern void exec_MVNS_imm_PRINT(Inst const inst);
  
  extern void exec_TST_regimm_PRINT(Inst const inst);
  extern void exec_TST_regreg_PRINT(Inst const inst);
  extern void exec_TST_imm_PRINT(Inst const inst);
  
  extern void exec_TEQ_regimm_PRINT(Inst const inst);
  extern void exec_TEQ_regreg_PRINT(Inst const inst);
  extern void exec_TEQ_imm_PRINT(Inst const inst);
  
  extern void exec_CMP_regimm_PRINT(Inst const inst);
  extern void exec_CMP_regreg_PRINT(Inst const inst);
  extern void exec_CMP_imm_PRINT(Inst const inst);
  
  extern void exec_CMN_regimm_PRINT(Inst const inst);
  extern void exec_CMN_regreg_PRINT(Inst const inst);
  extern void exec_CMN_imm_PRINT(Inst const inst);
  
  // Branch
  
  extern void exec_BLX_1_PRINT(Inst const inst);
  extern void exec_BLX_2_PRINT(Inst const inst);
  extern void exec_BX_PRINT(Inst const inst);
  extern void exec_B_PRINT(Inst const inst);
  extern void exec_BL_PRINT(Inst const inst);
  
  // Multi
  
  extern void exec_MUL_PRINT(Inst const inst);
  extern void exec_MULS_PRINT(Inst const inst);
  extern void exec_MLA_PRINT(Inst const inst);
  extern void exec_MLAS_PRINT(Inst const inst);
  extern void exec_UMULL_PRINT(Inst const inst);
  extern void exec_UMULLS_PRINT(Inst const inst);
  extern void exec_UMLAL_PRINT(Inst const inst);
  extern void exec_UMLALS_PRINT(Inst const inst);
  extern void exec_SMULL_PRINT(Inst const inst);
  extern void exec_SMULLS_PRINT(Inst const inst);
  extern void exec_SMLAL_PRINT(Inst const inst);
  extern void exec_SMLALS_PRINT(Inst const inst);
  
  // MRS
  
  extern void exec_MRS_cpsr_PRINT(Inst const inst);
  extern void exec_MRS_spsr_PRINT(Inst const inst);
  
  // MSR
  
  extern void exec_MSR_reg_cpsr_PRINT(Inst const inst);
  extern void exec_MSR_reg_spsr_PRINT(Inst const inst);
  extern void exec_MSR_imm_cpsr_PRINT(Inst const inst);
  extern void exec_MSR_imm_spsr_PRINT(Inst const inst);
  
  // UND
  
  extern void exec_UND_PRINT(Inst const inst);
  extern void exec_CDP_PRINT(Inst const inst);
  extern void exec_CDT_PRINT(Inst const inst);
  extern void exec_SWI_PRINT(Inst const inst);
#endif
}

#endif
