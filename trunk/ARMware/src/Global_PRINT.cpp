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

#include "Core.hpp"
#include "AddressSpace.hpp"
#include "MachineType.hpp"
#include "OsTimer.hpp"
#include "Serial_1.hpp"
#include "Serial_2.hpp"
#include "Serial_3.hpp"
#include "GPIOControlBlock.hpp"
#include "RTC.hpp"
#include "IntController.hpp"

#if ENABLE_THREADED_CODE
#include "ChunkChain.hpp"
#endif

#include "Inst/CRT.hpp"

#include "Utils/Number.hpp"

#include "Gtk/MachineScreen.hpp"

#include "Core.tcc"
#include "AddressSpace.tcc"
#include "Mmu.tcc"

#include "Global_PRINT.hpp"

#include "Inst/InstDPI_PRINT.tcc"
#include "Inst/InstLSM_PRINT.tcc"
#include "Inst/InstLSW_PRINT.tcc"
#include "Inst/InstLSH_PRINT.tcc"
#include "Inst/InstSWP_PRINT.tcc"
#include "Inst/InstBranch_PRINT.tcc"
#include "Inst/InstMulti_PRINT.tcc"
#include "Inst/InstMRS_PRINT.tcc"
#include "Inst/InstMSR_PRINT.tcc"

namespace ARMware
{
#if PRINT_CHUNK_CODE
  // STM
  
  void exec_STM_1_DA_PRINT(Inst const inst)   { g_STM_1_DA.print_code(inst); }
  void exec_STM_1_DA_w_PRINT(Inst const inst) { g_STM_1_DA_w.print_code(inst); }
  void exec_STM_2_DA_PRINT(Inst const inst)   { g_STM_2_DA.print_code(inst); }
  
  void exec_STM_1_DB_PRINT(Inst const inst)   { g_STM_1_DB.print_code(inst); }
  void exec_STM_1_DB_w_PRINT(Inst const inst) { g_STM_1_DB_w.print_code(inst); }
  void exec_STM_2_DB_PRINT(Inst const inst)   { g_STM_2_DB.print_code(inst); }
  
  void exec_STM_1_IA_PRINT(Inst const inst)   { g_STM_1_IA.print_code(inst); }
  void exec_STM_1_IA_w_PRINT(Inst const inst) { g_STM_1_IA_w.print_code(inst); }
  void exec_STM_2_IA_PRINT(Inst const inst)   { g_STM_2_IA.print_code(inst); }
  
  void exec_STM_1_IB_PRINT(Inst const inst)   { g_STM_1_IB.print_code(inst); }
  void exec_STM_1_IB_w_PRINT(Inst const inst) { g_STM_1_IB_w.print_code(inst); }
  void exec_STM_2_IB_PRINT(Inst const inst)   { g_STM_2_IB.print_code(inst); }
  
  // LDM
  
  void exec_LDM_1_DA_PRINT(Inst const inst)    { g_LDM_1_DA.print_code(inst); }
  void exec_LDM_1_DA_w_PRINT(Inst const inst)  { g_LDM_1_DA_w.print_code(inst); }
  void exec_LDM_23_DA_PRINT(Inst const inst)   { g_LDM_23_DA.print_code(inst); }
  void exec_LDM_23_DA_w_PRINT(Inst const inst) { g_LDM_23_DA_w.print_code(inst); }
  
  void exec_LDM_1_DB_PRINT(Inst const inst)    { g_LDM_1_DB.print_code(inst); }
  void exec_LDM_1_DB_w_PRINT(Inst const inst)  { g_LDM_1_DB_w.print_code(inst); }
  void exec_LDM_23_DB_PRINT(Inst const inst)   { g_LDM_23_DB.print_code(inst); }
  void exec_LDM_23_DB_w_PRINT(Inst const inst) { g_LDM_23_DB_w.print_code(inst); }
  
  void exec_LDM_1_IA_PRINT(Inst const inst)    { g_LDM_1_IA.print_code(inst); }
  void exec_LDM_1_IA_w_PRINT(Inst const inst)  { g_LDM_1_IA_w.print_code(inst); }
  void exec_LDM_23_IA_PRINT(Inst const inst)   { g_LDM_23_IA.print_code(inst); }
  void exec_LDM_23_IA_w_PRINT(Inst const inst) { g_LDM_23_IA_w.print_code(inst); }
  
  void exec_LDM_1_IB_PRINT(Inst const inst)    { g_LDM_1_IB.print_code(inst); }
  void exec_LDM_1_IB_w_PRINT(Inst const inst)  { g_LDM_1_IB_w.print_code(inst); }
  void exec_LDM_23_IB_PRINT(Inst const inst)   { g_LDM_23_IB.print_code(inst); }
  void exec_LDM_23_IB_w_PRINT(Inst const inst) { g_LDM_23_IB_w.print_code(inst); }
  
  // SWP
  
  void exec_SWP_PRINT(Inst const inst)  { g_SWP.print_code(inst); }
  void exec_SWPB_PRINT(Inst const inst) { g_SWPB.print_code(inst); }
  
  // STR
  
  void exec_STR_imm_post_down_PRINT(Inst const inst)   { g_STR_imm_post_down.print_code(inst); }
  void exec_STR_imm_post_up_PRINT(Inst const inst)     { g_STR_imm_post_up.print_code(inst); }
  void exec_STR_imm_offset_down_PRINT(Inst const inst) { g_STR_imm_offset_down.print_code(inst); }
  void exec_STR_imm_offset_up_PRINT(Inst const inst)   { g_STR_imm_offset_up.print_code(inst); }
  void exec_STR_imm_pre_down_PRINT(Inst const inst)    { g_STR_imm_pre_down.print_code(inst); }
  void exec_STR_imm_pre_up_PRINT(Inst const inst)      { g_STR_imm_pre_up.print_code(inst); }
  
  void exec_STR_reg_post_down_PRINT(Inst const inst)   { g_STR_reg_post_down.print_code(inst); }
  void exec_STR_reg_post_up_PRINT(Inst const inst)     { g_STR_reg_post_up.print_code(inst); }
  void exec_STR_reg_offset_down_PRINT(Inst const inst) { g_STR_reg_offset_down.print_code(inst); }
  void exec_STR_reg_offset_up_PRINT(Inst const inst)   { g_STR_reg_offset_up.print_code(inst); }
  void exec_STR_reg_pre_down_PRINT(Inst const inst)    { g_STR_reg_pre_down.print_code(inst); }
  void exec_STR_reg_pre_up_PRINT(Inst const inst)      { g_STR_reg_pre_up.print_code(inst); }
  
  // STRT
  
  void exec_STRT_imm_post_down_PRINT(Inst const inst) { g_STRT_imm_post_down.print_code(inst); }
  void exec_STRT_imm_post_up_PRINT(Inst const inst)   { g_STRT_imm_post_up.print_code(inst); }
  
  void exec_STRT_reg_post_down_PRINT(Inst const inst) { g_STRT_reg_post_down.print_code(inst); }
  void exec_STRT_reg_post_up_PRINT(Inst const inst)   { g_STRT_reg_post_up.print_code(inst); }
  
  // STRB
  
  void exec_STRB_imm_post_down_PRINT(Inst const inst)   { g_STRB_imm_post_down.print_code(inst); }
  void exec_STRB_imm_post_up_PRINT(Inst const inst)     { g_STRB_imm_post_up.print_code(inst); }
  void exec_STRB_imm_offset_down_PRINT(Inst const inst) { g_STRB_imm_offset_down.print_code(inst); }
  void exec_STRB_imm_offset_up_PRINT(Inst const inst)   { g_STRB_imm_offset_up.print_code(inst); }
  void exec_STRB_imm_pre_down_PRINT(Inst const inst)    { g_STRB_imm_pre_down.print_code(inst); }
  void exec_STRB_imm_pre_up_PRINT(Inst const inst)      { g_STRB_imm_pre_up.print_code(inst); }
  
  void exec_STRB_reg_post_down_PRINT(Inst const inst)   { g_STRB_reg_post_down.print_code(inst); }
  void exec_STRB_reg_post_up_PRINT(Inst const inst)     { g_STRB_reg_post_up.print_code(inst); }
  void exec_STRB_reg_offset_down_PRINT(Inst const inst) { g_STRB_reg_offset_down.print_code(inst); }
  void exec_STRB_reg_offset_up_PRINT(Inst const inst)   { g_STRB_reg_offset_up.print_code(inst); }
  void exec_STRB_reg_pre_down_PRINT(Inst const inst)    { g_STRB_reg_pre_down.print_code(inst); }
  void exec_STRB_reg_pre_up_PRINT(Inst const inst)      { g_STRB_reg_pre_up.print_code(inst); }
  
  // STRBT
  
  void exec_STRBT_imm_post_down_PRINT(Inst const inst) { g_STRBT_imm_post_down.print_code(inst); }
  void exec_STRBT_imm_post_up_PRINT(Inst const inst)   { g_STRBT_imm_post_up.print_code(inst); }
  
  void exec_STRBT_reg_post_down_PRINT(Inst const inst) { g_STRBT_reg_post_down.print_code(inst); }
  void exec_STRBT_reg_post_up_PRINT(Inst const inst)   { g_STRBT_reg_post_up.print_code(inst); }
  
  // LDR
  
  void exec_LDR_imm_post_down_PRINT(Inst const inst)   { g_LDR_imm_post_down.print_code(inst); }
  void exec_LDR_imm_post_up_PRINT(Inst const inst)     { g_LDR_imm_post_up.print_code(inst); }
  void exec_LDR_imm_offset_down_PRINT(Inst const inst) { g_LDR_imm_offset_down.print_code(inst); }
  void exec_LDR_imm_offset_up_PRINT(Inst const inst)   { g_LDR_imm_offset_up.print_code(inst); }
  void exec_LDR_imm_pre_down_PRINT(Inst const inst)    { g_LDR_imm_pre_down.print_code(inst); }
  void exec_LDR_imm_pre_up_PRINT(Inst const inst)      { g_LDR_imm_pre_up.print_code(inst); }
  
  void exec_LDR_reg_post_down_PRINT(Inst const inst)   { g_LDR_reg_post_down.print_code(inst); }
  void exec_LDR_reg_post_up_PRINT(Inst const inst)     { g_LDR_reg_post_up.print_code(inst); }
  void exec_LDR_reg_offset_down_PRINT(Inst const inst) { g_LDR_reg_offset_down.print_code(inst); }
  void exec_LDR_reg_offset_up_PRINT(Inst const inst)   { g_LDR_reg_offset_up.print_code(inst); }
  void exec_LDR_reg_pre_down_PRINT(Inst const inst)    { g_LDR_reg_pre_down.print_code(inst); }
  void exec_LDR_reg_pre_up_PRINT(Inst const inst)      { g_LDR_reg_pre_up.print_code(inst); }
  
  // LDRT
  
  void exec_LDRT_imm_post_down_PRINT(Inst const inst) { g_LDRT_imm_post_down.print_code(inst); }
  void exec_LDRT_imm_post_up_PRINT(Inst const inst)   { g_LDRT_imm_post_up.print_code(inst); }
  
  void exec_LDRT_reg_post_down_PRINT(Inst const inst) { g_LDRT_reg_post_down.print_code(inst); }
  void exec_LDRT_reg_post_up_PRINT(Inst const inst)   { g_LDRT_reg_post_up.print_code(inst); }
  
  // LDRB
  
  void exec_LDRB_imm_post_down_PRINT(Inst const inst)   { g_LDRB_imm_post_down.print_code(inst); }
  void exec_LDRB_imm_post_up_PRINT(Inst const inst)     { g_LDRB_imm_post_up.print_code(inst); }
  void exec_LDRB_imm_offset_down_PRINT(Inst const inst) { g_LDRB_imm_offset_down.print_code(inst); }
  void exec_LDRB_imm_offset_up_PRINT(Inst const inst)   { g_LDRB_imm_offset_up.print_code(inst); }
  void exec_LDRB_imm_pre_down_PRINT(Inst const inst)    { g_LDRB_imm_pre_down.print_code(inst); }
  void exec_LDRB_imm_pre_up_PRINT(Inst const inst)      { g_LDRB_imm_pre_up.print_code(inst); }
  
  void exec_LDRB_reg_post_down_PRINT(Inst const inst)   { g_LDRB_reg_post_down.print_code(inst); }
  void exec_LDRB_reg_post_up_PRINT(Inst const inst)     { g_LDRB_reg_post_up.print_code(inst); }
  void exec_LDRB_reg_offset_down_PRINT(Inst const inst) { g_LDRB_reg_offset_down.print_code(inst); }
  void exec_LDRB_reg_offset_up_PRINT(Inst const inst)   { g_LDRB_reg_offset_up.print_code(inst); }
  void exec_LDRB_reg_pre_down_PRINT(Inst const inst)    { g_LDRB_reg_pre_down.print_code(inst); }
  void exec_LDRB_reg_pre_up_PRINT(Inst const inst)      { g_LDRB_reg_pre_up.print_code(inst); }
  
  // LDRBT
  
  void exec_LDRBT_imm_post_down_PRINT(Inst const inst) { g_LDRBT_imm_post_down.print_code(inst); }
  void exec_LDRBT_imm_post_up_PRINT(Inst const inst)   { g_LDRBT_imm_post_up.print_code(inst); }
  
  void exec_LDRBT_reg_post_down_PRINT(Inst const inst) { g_LDRBT_reg_post_down.print_code(inst); }
  void exec_LDRBT_reg_post_up_PRINT(Inst const inst)   { g_LDRBT_reg_post_up.print_code(inst); }
  
  // STRH
  
  void exec_STRH_imm_post_down_PRINT(Inst const inst)   { g_STRH_imm_post_down.print_code(inst); }
  void exec_STRH_imm_post_up_PRINT(Inst const inst)     { g_STRH_imm_post_up.print_code(inst); }
  void exec_STRH_imm_offset_down_PRINT(Inst const inst) { g_STRH_imm_offset_down.print_code(inst); }
  void exec_STRH_imm_offset_up_PRINT(Inst const inst)   { g_STRH_imm_offset_up.print_code(inst); }
  void exec_STRH_imm_pre_down_PRINT(Inst const inst)    { g_STRH_imm_pre_down.print_code(inst); }
  void exec_STRH_imm_pre_up_PRINT(Inst const inst)      { g_STRH_imm_pre_up.print_code(inst); }
  
  void exec_STRH_reg_post_down_PRINT(Inst const inst)   { g_STRH_reg_post_down.print_code(inst); }
  void exec_STRH_reg_post_up_PRINT(Inst const inst)     { g_STRH_reg_post_up.print_code(inst); }
  void exec_STRH_reg_offset_down_PRINT(Inst const inst) { g_STRH_reg_offset_down.print_code(inst); }
  void exec_STRH_reg_offset_up_PRINT(Inst const inst)   { g_STRH_reg_offset_up.print_code(inst); }
  void exec_STRH_reg_pre_down_PRINT(Inst const inst)    { g_STRH_reg_pre_down.print_code(inst); }
  void exec_STRH_reg_pre_up_PRINT(Inst const inst)      { g_STRH_reg_pre_up.print_code(inst); }
  
  // LDRH
  
  void exec_LDRH_imm_post_down_PRINT(Inst const inst)   { g_LDRH_imm_post_down.print_code(inst); }
  void exec_LDRH_imm_post_up_PRINT(Inst const inst)     { g_LDRH_imm_post_up.print_code(inst); }
  void exec_LDRH_imm_offset_down_PRINT(Inst const inst) { g_LDRH_imm_offset_down.print_code(inst); }
  void exec_LDRH_imm_offset_up_PRINT(Inst const inst)   { g_LDRH_imm_offset_up.print_code(inst); }
  void exec_LDRH_imm_pre_down_PRINT(Inst const inst)    { g_LDRH_imm_pre_down.print_code(inst); }
  void exec_LDRH_imm_pre_up_PRINT(Inst const inst)      { g_LDRH_imm_pre_up.print_code(inst); }
  
  void exec_LDRH_reg_post_down_PRINT(Inst const inst)   { g_LDRH_reg_post_down.print_code(inst); }
  void exec_LDRH_reg_post_up_PRINT(Inst const inst)     { g_LDRH_reg_post_up.print_code(inst); }
  void exec_LDRH_reg_offset_down_PRINT(Inst const inst) { g_LDRH_reg_offset_down.print_code(inst); }
  void exec_LDRH_reg_offset_up_PRINT(Inst const inst)   { g_LDRH_reg_offset_up.print_code(inst); }
  void exec_LDRH_reg_pre_down_PRINT(Inst const inst)    { g_LDRH_reg_pre_down.print_code(inst); }
  void exec_LDRH_reg_pre_up_PRINT(Inst const inst)      { g_LDRH_reg_pre_up.print_code(inst); }
  
  // LDRSB
  
  void exec_LDRSB_imm_post_down_PRINT(Inst const inst)   { g_LDRSB_imm_post_down.print_code(inst); }
  void exec_LDRSB_imm_post_up_PRINT(Inst const inst)     { g_LDRSB_imm_post_up.print_code(inst); }
  void exec_LDRSB_imm_offset_down_PRINT(Inst const inst) { g_LDRSB_imm_offset_down.print_code(inst); }
  void exec_LDRSB_imm_offset_up_PRINT(Inst const inst)   { g_LDRSB_imm_offset_up.print_code(inst); }
  void exec_LDRSB_imm_pre_down_PRINT(Inst const inst)    { g_LDRSB_imm_pre_down.print_code(inst); }
  void exec_LDRSB_imm_pre_up_PRINT(Inst const inst)      { g_LDRSB_imm_pre_up.print_code(inst); }
  
  void exec_LDRSB_reg_post_down_PRINT(Inst const inst)   { g_LDRSB_reg_post_down.print_code(inst); }
  void exec_LDRSB_reg_post_up_PRINT(Inst const inst)     { g_LDRSB_reg_post_up.print_code(inst); }
  void exec_LDRSB_reg_offset_down_PRINT(Inst const inst) { g_LDRSB_reg_offset_down.print_code(inst); }
  void exec_LDRSB_reg_offset_up_PRINT(Inst const inst)   { g_LDRSB_reg_offset_up.print_code(inst); }
  void exec_LDRSB_reg_pre_down_PRINT(Inst const inst)    { g_LDRSB_reg_pre_down.print_code(inst); }
  void exec_LDRSB_reg_pre_up_PRINT(Inst const inst)      { g_LDRSB_reg_pre_up.print_code(inst); }
  
  // LDRSH
  
  void exec_LDRSH_imm_post_down_PRINT(Inst const inst)   { g_LDRSH_imm_post_down.print_code(inst); }
  void exec_LDRSH_imm_post_up_PRINT(Inst const inst)     { g_LDRSH_imm_post_up.print_code(inst); }
  void exec_LDRSH_imm_offset_down_PRINT(Inst const inst) { g_LDRSH_imm_offset_down.print_code(inst); }
  void exec_LDRSH_imm_offset_up_PRINT(Inst const inst)   { g_LDRSH_imm_offset_up.print_code(inst); }
  void exec_LDRSH_imm_pre_down_PRINT(Inst const inst)    { g_LDRSH_imm_pre_down.print_code(inst); }
  void exec_LDRSH_imm_pre_up_PRINT(Inst const inst)      { g_LDRSH_imm_pre_up.print_code(inst); }
  
  void exec_LDRSH_reg_post_down_PRINT(Inst const inst)   { g_LDRSH_reg_post_down.print_code(inst); }
  void exec_LDRSH_reg_post_up_PRINT(Inst const inst)     { g_LDRSH_reg_post_up.print_code(inst); }
  void exec_LDRSH_reg_offset_down_PRINT(Inst const inst) { g_LDRSH_reg_offset_down.print_code(inst); }
  void exec_LDRSH_reg_offset_up_PRINT(Inst const inst)   { g_LDRSH_reg_offset_up.print_code(inst); }
  void exec_LDRSH_reg_pre_down_PRINT(Inst const inst)    { g_LDRSH_reg_pre_down.print_code(inst); }
  void exec_LDRSH_reg_pre_up_PRINT(Inst const inst)      { g_LDRSH_reg_pre_up.print_code(inst); }
  
  // DPI
  
  void exec_AND_regimm_PRINT(Inst const inst)  { g_AND_regimm.print_code(inst); }
  void exec_AND_regreg_PRINT(Inst const inst)  { g_AND_regreg.print_code(inst); }
  void exec_AND_imm_PRINT(Inst const inst)     { g_AND_imm.print_code(inst); }
  void exec_ANDS_regimm_PRINT(Inst const inst) { g_ANDS_regimm.print_code(inst); }
  void exec_ANDS_regreg_PRINT(Inst const inst) { g_ANDS_regreg.print_code(inst); }
  void exec_ANDS_imm_PRINT(Inst const inst)    { g_ANDS_imm.print_code(inst); }
  
  void exec_EOR_regimm_PRINT(Inst const inst)  { g_EOR_regimm.print_code(inst); }
  void exec_EOR_regreg_PRINT(Inst const inst)  { g_EOR_regreg.print_code(inst); }
  void exec_EOR_imm_PRINT(Inst const inst)     { g_EOR_imm.print_code(inst); }
  void exec_EORS_regimm_PRINT(Inst const inst) { g_EORS_regimm.print_code(inst); }
  void exec_EORS_regreg_PRINT(Inst const inst) { g_EORS_regreg.print_code(inst); }
  void exec_EORS_imm_PRINT(Inst const inst)    { g_EORS_imm.print_code(inst); }
  
  void exec_SUB_regimm_PRINT(Inst const inst)  { g_SUB_regimm.print_code(inst); }
  void exec_SUB_regreg_PRINT(Inst const inst)  { g_SUB_regreg.print_code(inst); }
  void exec_SUB_imm_PRINT(Inst const inst)     { g_SUB_imm.print_code(inst); }
  void exec_SUBS_regimm_PRINT(Inst const inst) { g_SUBS_regimm.print_code(inst); }
  void exec_SUBS_regreg_PRINT(Inst const inst) { g_SUBS_regreg.print_code(inst); }
  void exec_SUBS_imm_PRINT(Inst const inst)    { g_SUBS_imm.print_code(inst); }
  
  void exec_RSB_regimm_PRINT(Inst const inst)  { g_RSB_regimm.print_code(inst); }
  void exec_RSB_regreg_PRINT(Inst const inst)  { g_RSB_regreg.print_code(inst); }
  void exec_RSB_imm_PRINT(Inst const inst)     { g_RSB_imm.print_code(inst); }
  void exec_RSBS_regimm_PRINT(Inst const inst) { g_RSBS_regimm.print_code(inst); }
  void exec_RSBS_regreg_PRINT(Inst const inst) { g_RSBS_regreg.print_code(inst); }
  void exec_RSBS_imm_PRINT(Inst const inst)    { g_RSBS_imm.print_code(inst); }
  
  void exec_ADD_regimm_PRINT(Inst const inst)  { g_ADD_regimm.print_code(inst); }
  void exec_ADD_regreg_PRINT(Inst const inst)  { g_ADD_regreg.print_code(inst); }
  void exec_ADD_imm_PRINT(Inst const inst)     { g_ADD_imm.print_code(inst); }
  void exec_ADDS_regimm_PRINT(Inst const inst) { g_ADDS_regimm.print_code(inst); }
  void exec_ADDS_regreg_PRINT(Inst const inst) { g_ADDS_regreg.print_code(inst); }
  void exec_ADDS_imm_PRINT(Inst const inst)    { g_ADDS_imm.print_code(inst); }
  
  void exec_ADC_regimm_PRINT(Inst const inst)  { g_ADC_regimm.print_code(inst); }
  void exec_ADC_regreg_PRINT(Inst const inst)  { g_ADC_regreg.print_code(inst); }
  void exec_ADC_imm_PRINT(Inst const inst)     { g_ADC_imm.print_code(inst); }
  void exec_ADCS_regimm_PRINT(Inst const inst) { g_ADCS_regimm.print_code(inst); }
  void exec_ADCS_regreg_PRINT(Inst const inst) { g_ADCS_regreg.print_code(inst); }
  void exec_ADCS_imm_PRINT(Inst const inst)    { g_ADCS_imm.print_code(inst); }
  
  void exec_SBC_regimm_PRINT(Inst const inst)  { g_SBC_regimm.print_code(inst); }
  void exec_SBC_regreg_PRINT(Inst const inst)  { g_SBC_regreg.print_code(inst); }
  void exec_SBC_imm_PRINT(Inst const inst)     { g_SBC_imm.print_code(inst); }
  void exec_SBCS_regimm_PRINT(Inst const inst) { g_SBCS_regimm.print_code(inst); }
  void exec_SBCS_regreg_PRINT(Inst const inst) { g_SBCS_regreg.print_code(inst); }
  void exec_SBCS_imm_PRINT(Inst const inst)    { g_SBCS_imm.print_code(inst); }
  
  void exec_RSC_regimm_PRINT(Inst const inst)  { g_RSC_regimm.print_code(inst); }
  void exec_RSC_regreg_PRINT(Inst const inst)  { g_RSC_regreg.print_code(inst); }
  void exec_RSC_imm_PRINT(Inst const inst)     { g_RSC_imm.print_code(inst); }
  void exec_RSCS_regimm_PRINT(Inst const inst) { g_RSCS_regimm.print_code(inst); }
  void exec_RSCS_regreg_PRINT(Inst const inst) { g_RSCS_regreg.print_code(inst); }
  void exec_RSCS_imm_PRINT(Inst const inst)    { g_RSCS_imm.print_code(inst); }
  
  void exec_ORR_regimm_PRINT(Inst const inst)  { g_ORR_regimm.print_code(inst); }
  void exec_ORR_regreg_PRINT(Inst const inst)  { g_ORR_regreg.print_code(inst); }
  void exec_ORR_imm_PRINT(Inst const inst)     { g_ORR_imm.print_code(inst); }
  void exec_ORRS_regimm_PRINT(Inst const inst) { g_ORRS_regimm.print_code(inst); }
  void exec_ORRS_regreg_PRINT(Inst const inst) { g_ORRS_regreg.print_code(inst); }
  void exec_ORRS_imm_PRINT(Inst const inst)    { g_ORRS_imm.print_code(inst); }
  
  void exec_MOV_regimm_PRINT(Inst const inst)  { g_MOV_regimm.print_code(inst); }
  void exec_MOV_regreg_PRINT(Inst const inst)  { g_MOV_regreg.print_code(inst); }
  void exec_MOV_imm_PRINT(Inst const inst)     { g_MOV_imm.print_code(inst); }
  void exec_MOVS_regimm_PRINT(Inst const inst) { g_MOVS_regimm.print_code(inst); }
  void exec_MOVS_regreg_PRINT(Inst const inst) { g_MOVS_regreg.print_code(inst); }
  void exec_MOVS_imm_PRINT(Inst const inst)    { g_MOVS_imm.print_code(inst); }
  
  void exec_BIC_regimm_PRINT(Inst const inst)  { g_BIC_regimm.print_code(inst); }
  void exec_BIC_regreg_PRINT(Inst const inst)  { g_BIC_regreg.print_code(inst); }
  void exec_BIC_imm_PRINT(Inst const inst)     { g_BIC_imm.print_code(inst); }
  void exec_BICS_regimm_PRINT(Inst const inst) { g_BICS_regimm.print_code(inst); }
  void exec_BICS_regreg_PRINT(Inst const inst) { g_BICS_regreg.print_code(inst); }
  void exec_BICS_imm_PRINT(Inst const inst)    { g_BICS_imm.print_code(inst); }
  
  void exec_MVN_regimm_PRINT(Inst const inst)  { g_MVN_regimm.print_code(inst); }
  void exec_MVN_regreg_PRINT(Inst const inst)  { g_MVN_regreg.print_code(inst); }
  void exec_MVN_imm_PRINT(Inst const inst)     { g_MVN_imm.print_code(inst); }
  void exec_MVNS_regimm_PRINT(Inst const inst) { g_MVNS_regimm.print_code(inst); }
  void exec_MVNS_regreg_PRINT(Inst const inst) { g_MVNS_regreg.print_code(inst); }
  void exec_MVNS_imm_PRINT(Inst const inst)    { g_MVNS_imm.print_code(inst); }
  
  void exec_TST_regimm_PRINT(Inst const inst)  { g_TST_regimm.print_code(inst); }
  void exec_TST_regreg_PRINT(Inst const inst)  { g_TST_regreg.print_code(inst); }
  void exec_TST_imm_PRINT(Inst const inst)     { g_TST_imm.print_code(inst); }
  
  void exec_TEQ_regimm_PRINT(Inst const inst)  { g_TEQ_regimm.print_code(inst); }
  void exec_TEQ_regreg_PRINT(Inst const inst)  { g_TEQ_regreg.print_code(inst); }
  void exec_TEQ_imm_PRINT(Inst const inst)     { g_TEQ_imm.print_code(inst); }
  
  void exec_CMP_regimm_PRINT(Inst const inst)  { g_CMP_regimm.print_code(inst); }
  void exec_CMP_regreg_PRINT(Inst const inst)  { g_CMP_regreg.print_code(inst); }
  void exec_CMP_imm_PRINT(Inst const inst)     { g_CMP_imm.print_code(inst); }
  
  void exec_CMN_regimm_PRINT(Inst const inst)  { g_CMN_regimm.print_code(inst); }
  void exec_CMN_regreg_PRINT(Inst const inst)  { g_CMN_regreg.print_code(inst); }
  void exec_CMN_imm_PRINT(Inst const inst)     { g_CMN_imm.print_code(inst); }
  
  // Branch
  
  void exec_BLX_1_PRINT(Inst const inst) { g_BLX_1.print_code(inst); }
  void exec_BLX_2_PRINT(Inst const inst) { g_BLX_2.print_code(inst); }
  void exec_BX_PRINT(Inst const inst)    { g_BX.print_code(inst); }
  void exec_B_PRINT(Inst const inst)     { g_B.print_code(inst); }
  void exec_BL_PRINT(Inst const inst)    { g_BL.print_code(inst); }
  
  // Multi
  
  void exec_MUL_PRINT(Inst const inst)    { g_MUL.print_code(inst); }
  void exec_MULS_PRINT(Inst const inst)   { g_MULS.print_code(inst); }
  void exec_MLA_PRINT(Inst const inst)    { g_MLA.print_code(inst); }
  void exec_MLAS_PRINT(Inst const inst)   { g_MLAS.print_code(inst); }
  void exec_UMULL_PRINT(Inst const inst)  { g_UMULL.print_code(inst); }
  void exec_UMULLS_PRINT(Inst const inst) { g_UMULLS.print_code(inst); }
  void exec_UMLAL_PRINT(Inst const inst)  { g_UMLAL.print_code(inst); }
  void exec_UMLALS_PRINT(Inst const inst) { g_UMLALS.print_code(inst); }
  void exec_SMULL_PRINT(Inst const inst)  { g_SMULL.print_code(inst); }
  void exec_SMULLS_PRINT(Inst const inst) { g_SMULLS.print_code(inst); }
  void exec_SMLAL_PRINT(Inst const inst)  { g_SMLAL.print_code(inst); }
  void exec_SMLALS_PRINT(Inst const inst) { g_SMLALS.print_code(inst); }
  
  // MRS
  
  void exec_MRS_cpsr_PRINT(Inst const inst) { g_MRS_cpsr.print_code(inst); }
  void exec_MRS_spsr_PRINT(Inst const inst) { g_MRS_spsr.print_code(inst); }
  
  // MSR
  
  void exec_MSR_reg_cpsr_PRINT(Inst const inst) { g_MSR_reg_cpsr.print_code(inst); }
  void exec_MSR_reg_spsr_PRINT(Inst const inst) { g_MSR_reg_spsr.print_code(inst); }
  void exec_MSR_imm_cpsr_PRINT(Inst const inst) { g_MSR_imm_cpsr.print_code(inst); }
  void exec_MSR_imm_spsr_PRINT(Inst const inst) { g_MSR_imm_spsr.print_code(inst); }
  
  // UND
  
  void
  exec_UND_PRINT(Inst const /* inst */)
  {
    g_log_file << "UND" << std::endl;
  }
  
  // CDP
  
  void
  exec_CDP_PRINT(Inst const inst)
  {
    g_log_file << "CDP"
              << CondName(inst)
              << std::endl;
  }
  
  // CDT
  
  void
  exec_CDT_PRINT(Inst const inst)
  {
    g_log_file << "CDT"
              << CondName(inst)
              << std::endl;
  }
  
  // SWI
  
  void
  exec_SWI_PRINT(Inst const inst)
  {
    g_log_file << "SWI"
              << CondName(inst)
              << std::endl;
  }
#endif
}
