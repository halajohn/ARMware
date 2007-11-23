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
#include "ChunkChain.hpp"
#include "Inst/CRT.hpp"
#include "Utils/Number.hpp"
#include "Gtk/MachineScreen.hpp"
#include "Compiler/MIR.hpp"

#include "Core.tcc"
#include "AddressSpace.tcc"
#include "Mmu.tcc"

#include "Global_TC.hpp"
#include "Global_DT.hpp"

#include "Inst/InstDPI.tcc"
#include "Inst/InstLSM.tcc"
#include "Inst/InstLSW.tcc"
#include "Inst/InstLSH.tcc"
#include "Inst/InstSWP.tcc"
#include "Inst/InstBranch.tcc"
#include "Inst/InstMulti.tcc"
#include "Inst/InstMRS.tcc"
#include "Inst/InstMSR.tcc"

#include "Inst/InstDPI_DT.tcc"
#include "Inst/InstLSM_DT.tcc"
#include "Inst/InstLSW_DT.tcc"
#include "Inst/InstLSH_DT.tcc"
#include "Inst/InstSWP_DT.tcc"
#include "Inst/InstBranch_DT.tcc"
#include "Inst/InstMulti_DT.tcc"
#include "Inst/InstMRS_DT.tcc"
#include "Inst/InstMSR_DT.tcc"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // dynamic translator version
  
#if ENABLE_DYNAMIC_TRANSLATOR
  ExecResultEnum check_periphery_DT() { return gp_core->check_periphery<false, false>(); }
  
  // STM - dynamic translator version
  
  void exec_STM_1_DA_DT(Inst const inst, bool const is_last)   { return g_STM_1_DA.gen_dt_code(inst, is_last); }
  void exec_STM_1_DA_w_DT(Inst const inst, bool const is_last) { return g_STM_1_DA_w.gen_dt_code(inst, is_last); }
  void exec_STM_2_DA_DT(Inst const inst, bool const is_last)   { return g_STM_2_DA.gen_dt_code(inst, is_last); }
  
  void exec_STM_1_DB_DT(Inst const inst, bool const is_last)   { return g_STM_1_DB.gen_dt_code(inst, is_last); }
  void exec_STM_1_DB_w_DT(Inst const inst, bool const is_last) { return g_STM_1_DB_w.gen_dt_code(inst, is_last); }
  void exec_STM_2_DB_DT(Inst const inst, bool const is_last)   { return g_STM_2_DB.gen_dt_code(inst, is_last); }
  
  void exec_STM_1_IA_DT(Inst const inst, bool const is_last)   { return g_STM_1_IA.gen_dt_code(inst, is_last); }
  void exec_STM_1_IA_w_DT(Inst const inst, bool const is_last) { return g_STM_1_IA_w.gen_dt_code(inst, is_last); }
  void exec_STM_2_IA_DT(Inst const inst, bool const is_last)   { return g_STM_2_IA.gen_dt_code(inst, is_last); }
  
  void exec_STM_1_IB_DT(Inst const inst, bool const is_last)   { return g_STM_1_IB.gen_dt_code(inst, is_last); }
  void exec_STM_1_IB_w_DT(Inst const inst, bool const is_last) { return g_STM_1_IB_w.gen_dt_code(inst, is_last); }
  void exec_STM_2_IB_DT(Inst const inst, bool const is_last)   { return g_STM_2_IB.gen_dt_code(inst, is_last); }
  
  // LDM - dynamic translator version
  
  void exec_LDM_1_DA_DT(Inst const inst, bool const is_last)    { return g_LDM_1_DA.gen_dt_code(inst, is_last); }
  void exec_LDM_1_DA_w_DT(Inst const inst, bool const is_last)  { return g_LDM_1_DA_w.gen_dt_code(inst, is_last); }
  void exec_LDM_23_DA_DT(Inst const inst, bool const is_last)   { return g_LDM_23_DA.gen_dt_code(inst, is_last); }
  void exec_LDM_23_DA_w_DT(Inst const inst, bool const is_last) { return g_LDM_23_DA_w.gen_dt_code(inst, is_last); }
  
  void exec_LDM_1_DB_DT(Inst const inst, bool const is_last)    { return g_LDM_1_DB.gen_dt_code(inst, is_last); }
  void exec_LDM_1_DB_w_DT(Inst const inst, bool const is_last)  { return g_LDM_1_DB_w.gen_dt_code(inst, is_last); }
  void exec_LDM_23_DB_DT(Inst const inst, bool const is_last)   { return g_LDM_23_DB.gen_dt_code(inst, is_last); }
  void exec_LDM_23_DB_w_DT(Inst const inst, bool const is_last) { return g_LDM_23_DB_w.gen_dt_code(inst, is_last); }
  
  void exec_LDM_1_IA_DT(Inst const inst, bool const is_last)    { return g_LDM_1_IA.gen_dt_code(inst, is_last); }
  void exec_LDM_1_IA_w_DT(Inst const inst, bool const is_last)  { return g_LDM_1_IA_w.gen_dt_code(inst, is_last); }
  void exec_LDM_23_IA_DT(Inst const inst, bool const is_last)   { return g_LDM_23_IA.gen_dt_code(inst, is_last); }
  void exec_LDM_23_IA_w_DT(Inst const inst, bool const is_last) { return g_LDM_23_IA_w.gen_dt_code(inst, is_last); }
  
  void exec_LDM_1_IB_DT(Inst const inst, bool const is_last)    { return g_LDM_1_IB.gen_dt_code(inst, is_last); }
  void exec_LDM_1_IB_w_DT(Inst const inst, bool const is_last)  { return g_LDM_1_IB_w.gen_dt_code(inst, is_last); }
  void exec_LDM_23_IB_DT(Inst const inst, bool const is_last)   { return g_LDM_23_IB.gen_dt_code(inst, is_last); }
  void exec_LDM_23_IB_w_DT(Inst const inst, bool const is_last) { return g_LDM_23_IB_w.gen_dt_code(inst, is_last); }
  
  // SWP - dynamic translator version
  
  void exec_SWP_DT(Inst const inst, bool const is_last)  { return g_SWP.gen_dt_code(inst, is_last); }
  void exec_SWPB_DT(Inst const inst, bool const is_last) { return g_SWPB.gen_dt_code(inst, is_last); }
  
  // STR - dynamic translator version
  
  void exec_STR_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_STR_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_STR_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_STR_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_STR_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_STR_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_STR_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_STR_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_STR_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_STR_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_STR_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_STR_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_STR_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_STR_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_STR_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_STR_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_STR_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_STR_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_STR_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_STR_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_STR_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_STR_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_STR_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_STR_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // STRT - dynamic translator version
  
  void exec_STRT_imm_post_down_DT(Inst const inst, bool const is_last) { return g_STRT_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_STRT_imm_post_up_DT(Inst const inst, bool const is_last)   { return g_STRT_imm_post_up.gen_dt_code(inst, is_last); }
  
  void exec_STRT_reg_post_down_DT(Inst const inst, bool const is_last) { return g_STRT_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_STRT_reg_post_up_DT(Inst const inst, bool const is_last)   { return g_STRT_reg_post_up.gen_dt_code(inst, is_last); }
  
  // STRB - dynamic translator version
  
  void exec_STRB_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_STRB_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_STRB_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_STRB_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_STRB_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_STRB_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_STRB_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_STRB_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_STRB_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_STRB_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_STRB_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_STRB_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_STRB_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_STRB_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_STRB_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_STRB_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_STRB_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_STRB_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_STRB_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_STRB_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_STRB_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_STRB_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_STRB_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_STRB_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // STRBT - dynamic translator version
  
  void exec_STRBT_imm_post_down_DT(Inst const inst, bool const is_last) { return g_STRBT_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_STRBT_imm_post_up_DT(Inst const inst, bool const is_last)   { return g_STRBT_imm_post_up.gen_dt_code(inst, is_last); }
  
  void exec_STRBT_reg_post_down_DT(Inst const inst, bool const is_last) { return g_STRBT_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_STRBT_reg_post_up_DT(Inst const inst, bool const is_last)   { return g_STRBT_reg_post_up.gen_dt_code(inst, is_last); }
  
  // LDR - dynamic translator version
  
  void exec_LDR_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_LDR_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_LDR_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_LDR_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_LDR_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_LDR_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDR_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDR_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDR_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDR_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDR_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDR_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_LDR_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_LDR_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_LDR_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_LDR_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_LDR_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_LDR_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDR_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDR_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDR_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDR_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDR_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDR_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // LDRT - dynamic translator version
  
  void exec_LDRT_imm_post_down_DT(Inst const inst, bool const is_last) { return g_LDRT_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRT_imm_post_up_DT(Inst const inst, bool const is_last)   { return g_LDRT_imm_post_up.gen_dt_code(inst, is_last); }
  
  void exec_LDRT_reg_post_down_DT(Inst const inst, bool const is_last) { return g_LDRT_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRT_reg_post_up_DT(Inst const inst, bool const is_last)   { return g_LDRT_reg_post_up.gen_dt_code(inst, is_last); }
  
  // LDRB - dynamic translator version
  
  void exec_LDRB_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRB_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRB_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRB_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRB_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRB_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRB_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRB_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRB_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRB_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRB_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRB_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_LDRB_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRB_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRB_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRB_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRB_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRB_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRB_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRB_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRB_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRB_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRB_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRB_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // LDRBT - dynamic translator version
  
  void exec_LDRBT_imm_post_down_DT(Inst const inst, bool const is_last) { return g_LDRBT_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRBT_imm_post_up_DT(Inst const inst, bool const is_last)   { return g_LDRBT_imm_post_up.gen_dt_code(inst, is_last); }
  
  void exec_LDRBT_reg_post_down_DT(Inst const inst, bool const is_last) { return g_LDRBT_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRBT_reg_post_up_DT(Inst const inst, bool const is_last)   { return g_LDRBT_reg_post_up.gen_dt_code(inst, is_last); }
  
  // STRH - dynamic translator version
  
  void exec_STRH_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_STRH_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_STRH_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_STRH_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_STRH_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_STRH_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_STRH_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_STRH_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_STRH_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_STRH_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_STRH_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_STRH_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_STRH_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_STRH_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_STRH_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_STRH_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_STRH_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_STRH_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_STRH_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_STRH_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_STRH_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_STRH_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_STRH_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_STRH_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // LDRH - dynamic translator version
  
  void exec_LDRH_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRH_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRH_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRH_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRH_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRH_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRH_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRH_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRH_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRH_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRH_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRH_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_LDRH_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRH_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRH_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRH_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRH_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRH_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRH_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRH_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRH_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRH_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRH_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRH_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // LDRSB - dynamic translator version
  
  void exec_LDRSB_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRSB_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRSB_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRSB_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRSB_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRSB_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRSB_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRSB_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRSB_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRSB_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRSB_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRSB_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_LDRSB_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRSB_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRSB_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRSB_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRSB_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRSB_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRSB_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRSB_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRSB_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRSB_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRSB_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRSB_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // LDRSH - dynamic translator version
  
  void exec_LDRSH_imm_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRSH_imm_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRSH_imm_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRSH_imm_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRSH_imm_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRSH_imm_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRSH_imm_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRSH_imm_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRSH_imm_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRSH_imm_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRSH_imm_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRSH_imm_pre_up.gen_dt_code(inst, is_last); }
  
  void exec_LDRSH_reg_post_down_DT(Inst const inst, bool const is_last)   { return g_LDRSH_reg_post_down.gen_dt_code(inst, is_last); }
  void exec_LDRSH_reg_post_up_DT(Inst const inst, bool const is_last)     { return g_LDRSH_reg_post_up.gen_dt_code(inst, is_last); }
  void exec_LDRSH_reg_offset_down_DT(Inst const inst, bool const is_last) { return g_LDRSH_reg_offset_down.gen_dt_code(inst, is_last); }
  void exec_LDRSH_reg_offset_up_DT(Inst const inst, bool const is_last)   { return g_LDRSH_reg_offset_up.gen_dt_code(inst, is_last); }
  void exec_LDRSH_reg_pre_down_DT(Inst const inst, bool const is_last)    { return g_LDRSH_reg_pre_down.gen_dt_code(inst, is_last); }
  void exec_LDRSH_reg_pre_up_DT(Inst const inst, bool const is_last)      { return g_LDRSH_reg_pre_up.gen_dt_code(inst, is_last); }
  
  // DPI - dynamic translator version
  
  void exec_AND_regimm_DT(Inst const inst, bool const is_last)  { return g_AND_regimm.gen_dt_code(inst, is_last); }
  void exec_AND_regreg_DT(Inst const inst, bool const is_last)  { return g_AND_regreg.gen_dt_code(inst, is_last); }
  void exec_AND_imm_DT(Inst const inst, bool const is_last)     { return g_AND_imm.gen_dt_code(inst, is_last); }
  void exec_ANDS_regimm_DT(Inst const inst, bool const is_last) { return g_ANDS_regimm.gen_dt_code(inst, is_last); }
  void exec_ANDS_regreg_DT(Inst const inst, bool const is_last) { return g_ANDS_regreg.gen_dt_code(inst, is_last); }
  void exec_ANDS_imm_DT(Inst const inst, bool const is_last)    { return g_ANDS_imm.gen_dt_code(inst, is_last); }
  
  void exec_EOR_regimm_DT(Inst const inst, bool const is_last)  { return g_EOR_regimm.gen_dt_code(inst, is_last); }
  void exec_EOR_regreg_DT(Inst const inst, bool const is_last)  { return g_EOR_regreg.gen_dt_code(inst, is_last); }
  void exec_EOR_imm_DT(Inst const inst, bool const is_last)     { return g_EOR_imm.gen_dt_code(inst, is_last); }
  void exec_EORS_regimm_DT(Inst const inst, bool const is_last) { return g_EORS_regimm.gen_dt_code(inst, is_last); }
  void exec_EORS_regreg_DT(Inst const inst, bool const is_last) { return g_EORS_regreg.gen_dt_code(inst, is_last); }
  void exec_EORS_imm_DT(Inst const inst, bool const is_last)    { return g_EORS_imm.gen_dt_code(inst, is_last); }
  
  void exec_SUB_regimm_DT(Inst const inst, bool const is_last)  { return g_SUB_regimm.gen_dt_code(inst, is_last); }
  void exec_SUB_regreg_DT(Inst const inst, bool const is_last)  { return g_SUB_regreg.gen_dt_code(inst, is_last); }
  void exec_SUB_imm_DT(Inst const inst, bool const is_last)     { return g_SUB_imm.gen_dt_code(inst, is_last); }
  void exec_SUBS_regimm_DT(Inst const inst, bool const is_last) { return g_SUBS_regimm.gen_dt_code(inst, is_last); }
  void exec_SUBS_regreg_DT(Inst const inst, bool const is_last) { return g_SUBS_regreg.gen_dt_code(inst, is_last); }
  void exec_SUBS_imm_DT(Inst const inst, bool const is_last)    { return g_SUBS_imm.gen_dt_code(inst, is_last); }
  
  void exec_RSB_regimm_DT(Inst const inst, bool const is_last)  { return g_RSB_regimm.gen_dt_code(inst, is_last); }
  void exec_RSB_regreg_DT(Inst const inst, bool const is_last)  { return g_RSB_regreg.gen_dt_code(inst, is_last); }
  void exec_RSB_imm_DT(Inst const inst, bool const is_last)     { return g_RSB_imm.gen_dt_code(inst, is_last); }
  void exec_RSBS_regimm_DT(Inst const inst, bool const is_last) { return g_RSBS_regimm.gen_dt_code(inst, is_last); }
  void exec_RSBS_regreg_DT(Inst const inst, bool const is_last) { return g_RSBS_regreg.gen_dt_code(inst, is_last); }
  void exec_RSBS_imm_DT(Inst const inst, bool const is_last)    { return g_RSBS_imm.gen_dt_code(inst, is_last); }
  
  void exec_ADD_regimm_DT(Inst const inst, bool const is_last)  { return g_ADD_regimm.gen_dt_code(inst, is_last); }
  void exec_ADD_regreg_DT(Inst const inst, bool const is_last)  { return g_ADD_regreg.gen_dt_code(inst, is_last); }
  void exec_ADD_imm_DT(Inst const inst, bool const is_last)     { return g_ADD_imm.gen_dt_code(inst, is_last); }
  void exec_ADDS_regimm_DT(Inst const inst, bool const is_last) { return g_ADDS_regimm.gen_dt_code(inst, is_last); }
  void exec_ADDS_regreg_DT(Inst const inst, bool const is_last) { return g_ADDS_regreg.gen_dt_code(inst, is_last); }
  void exec_ADDS_imm_DT(Inst const inst, bool const is_last)    { return g_ADDS_imm.gen_dt_code(inst, is_last); }
  
  void exec_ADC_regimm_DT(Inst const inst, bool const is_last)  { return g_ADC_regimm.gen_dt_code(inst, is_last); }
  void exec_ADC_regreg_DT(Inst const inst, bool const is_last)  { return g_ADC_regreg.gen_dt_code(inst, is_last); }
  void exec_ADC_imm_DT(Inst const inst, bool const is_last)     { return g_ADC_imm.gen_dt_code(inst, is_last); }
  void exec_ADCS_regimm_DT(Inst const inst, bool const is_last) { return g_ADCS_regimm.gen_dt_code(inst, is_last); }
  void exec_ADCS_regreg_DT(Inst const inst, bool const is_last) { return g_ADCS_regreg.gen_dt_code(inst, is_last); }
  void exec_ADCS_imm_DT(Inst const inst, bool const is_last)    { return g_ADCS_imm.gen_dt_code(inst, is_last); }
  
  void exec_SBC_regimm_DT(Inst const inst, bool const is_last)  { return g_SBC_regimm.gen_dt_code(inst, is_last); }
  void exec_SBC_regreg_DT(Inst const inst, bool const is_last)  { return g_SBC_regreg.gen_dt_code(inst, is_last); }
  void exec_SBC_imm_DT(Inst const inst, bool const is_last)     { return g_SBC_imm.gen_dt_code(inst, is_last); }
  void exec_SBCS_regimm_DT(Inst const inst, bool const is_last) { return g_SBCS_regimm.gen_dt_code(inst, is_last); }
  void exec_SBCS_regreg_DT(Inst const inst, bool const is_last) { return g_SBCS_regreg.gen_dt_code(inst, is_last); }
  void exec_SBCS_imm_DT(Inst const inst, bool const is_last)    { return g_SBCS_imm.gen_dt_code(inst, is_last); }
  
  void exec_RSC_regimm_DT(Inst const inst, bool const is_last)  { return g_RSC_regimm.gen_dt_code(inst, is_last); }
  void exec_RSC_regreg_DT(Inst const inst, bool const is_last)  { return g_RSC_regreg.gen_dt_code(inst, is_last); }
  void exec_RSC_imm_DT(Inst const inst, bool const is_last)     { return g_RSC_imm.gen_dt_code(inst, is_last); }
  void exec_RSCS_regimm_DT(Inst const inst, bool const is_last) { return g_RSCS_regimm.gen_dt_code(inst, is_last); }
  void exec_RSCS_regreg_DT(Inst const inst, bool const is_last) { return g_RSCS_regreg.gen_dt_code(inst, is_last); }
  void exec_RSCS_imm_DT(Inst const inst, bool const is_last)    { return g_RSCS_imm.gen_dt_code(inst, is_last); }
  
  void exec_ORR_regimm_DT(Inst const inst, bool const is_last)  { return g_ORR_regimm.gen_dt_code(inst, is_last); }
  void exec_ORR_regreg_DT(Inst const inst, bool const is_last)  { return g_ORR_regreg.gen_dt_code(inst, is_last); }
  void exec_ORR_imm_DT(Inst const inst, bool const is_last)     { return g_ORR_imm.gen_dt_code(inst, is_last); }
  void exec_ORRS_regimm_DT(Inst const inst, bool const is_last) { return g_ORRS_regimm.gen_dt_code(inst, is_last); }
  void exec_ORRS_regreg_DT(Inst const inst, bool const is_last) { return g_ORRS_regreg.gen_dt_code(inst, is_last); }
  void exec_ORRS_imm_DT(Inst const inst, bool const is_last)    { return g_ORRS_imm.gen_dt_code(inst, is_last); }
  
  void exec_MOV_regimm_DT(Inst const inst, bool const is_last)  { return g_MOV_regimm.gen_dt_code(inst, is_last); }
  void exec_MOV_regreg_DT(Inst const inst, bool const is_last)  { return g_MOV_regreg.gen_dt_code(inst, is_last); }
  void exec_MOV_imm_DT(Inst const inst, bool const is_last)     { return g_MOV_imm.gen_dt_code(inst, is_last); }
  void exec_MOVS_regimm_DT(Inst const inst, bool const is_last) { return g_MOVS_regimm.gen_dt_code(inst, is_last); }
  void exec_MOVS_regreg_DT(Inst const inst, bool const is_last) { return g_MOVS_regreg.gen_dt_code(inst, is_last); }
  void exec_MOVS_imm_DT(Inst const inst, bool const is_last)    { return g_MOVS_imm.gen_dt_code(inst, is_last); }
  
  void exec_BIC_regimm_DT(Inst const inst, bool const is_last)  { return g_BIC_regimm.gen_dt_code(inst, is_last); }
  void exec_BIC_regreg_DT(Inst const inst, bool const is_last)  { return g_BIC_regreg.gen_dt_code(inst, is_last); }
  void exec_BIC_imm_DT(Inst const inst, bool const is_last)     { return g_BIC_imm.gen_dt_code(inst, is_last); }
  void exec_BICS_regimm_DT(Inst const inst, bool const is_last) { return g_BICS_regimm.gen_dt_code(inst, is_last); }
  void exec_BICS_regreg_DT(Inst const inst, bool const is_last) { return g_BICS_regreg.gen_dt_code(inst, is_last); }
  void exec_BICS_imm_DT(Inst const inst, bool const is_last)    { return g_BICS_imm.gen_dt_code(inst, is_last); }
  
  void exec_MVN_regimm_DT(Inst const inst, bool const is_last)  { return g_MVN_regimm.gen_dt_code(inst, is_last); }
  void exec_MVN_regreg_DT(Inst const inst, bool const is_last)  { return g_MVN_regreg.gen_dt_code(inst, is_last); }
  void exec_MVN_imm_DT(Inst const inst, bool const is_last)     { return g_MVN_imm.gen_dt_code(inst, is_last); }
  void exec_MVNS_regimm_DT(Inst const inst, bool const is_last) { return g_MVNS_regimm.gen_dt_code(inst, is_last); }
  void exec_MVNS_regreg_DT(Inst const inst, bool const is_last) { return g_MVNS_regreg.gen_dt_code(inst, is_last); }
  void exec_MVNS_imm_DT(Inst const inst, bool const is_last)    { return g_MVNS_imm.gen_dt_code(inst, is_last); }
  
  void exec_TST_regimm_DT(Inst const inst, bool const is_last)  { return g_TST_regimm.gen_dt_code(inst, is_last); }
  void exec_TST_regreg_DT(Inst const inst, bool const is_last)  { return g_TST_regreg.gen_dt_code(inst, is_last); }
  void exec_TST_imm_DT(Inst const inst, bool const is_last)     { return g_TST_imm.gen_dt_code(inst, is_last); }
  
  void exec_TEQ_regimm_DT(Inst const inst, bool const is_last)  { return g_TEQ_regimm.gen_dt_code(inst, is_last); }
  void exec_TEQ_regreg_DT(Inst const inst, bool const is_last)  { return g_TEQ_regreg.gen_dt_code(inst, is_last); }
  void exec_TEQ_imm_DT(Inst const inst, bool const is_last)     { return g_TEQ_imm.gen_dt_code(inst, is_last); }
  
  void exec_CMP_regimm_DT(Inst const inst, bool const is_last)  { return g_CMP_regimm.gen_dt_code(inst, is_last); }
  void exec_CMP_regreg_DT(Inst const inst, bool const is_last)  { return g_CMP_regreg.gen_dt_code(inst, is_last); }
  void exec_CMP_imm_DT(Inst const inst, bool const is_last)     { return g_CMP_imm.gen_dt_code(inst, is_last); }
  
  void exec_CMN_regimm_DT(Inst const inst, bool const is_last)  { return g_CMN_regimm.gen_dt_code(inst, is_last); }
  void exec_CMN_regreg_DT(Inst const inst, bool const is_last)  { return g_CMN_regreg.gen_dt_code(inst, is_last); }
  void exec_CMN_imm_DT(Inst const inst, bool const is_last)     { return g_CMN_imm.gen_dt_code(inst, is_last); }
  
  // Branch - dynamic translator version
  
  void exec_BLX_1_DT(Inst const inst, bool const is_last) { return g_BLX_1.gen_dt_code(inst, is_last); }
  void exec_BLX_2_DT(Inst const inst, bool const is_last) { return g_BLX_2.gen_dt_code(inst, is_last); }
  void exec_BX_DT(Inst const inst, bool const is_last)    { return g_BX.gen_dt_code(inst, is_last); }
  void exec_B_DT(Inst const inst, bool const is_last)     { return g_B.gen_dt_code(inst, is_last); }
  void exec_BL_DT(Inst const inst, bool const is_last)    { return g_BL.gen_dt_code(inst, is_last); }
  
  // Multi - dynamic translator version
  
  void exec_MUL_DT(Inst const inst, bool const is_last)    { return g_MUL.gen_dt_code(inst, is_last); }
  void exec_MULS_DT(Inst const inst, bool const is_last)   { return g_MULS.gen_dt_code(inst, is_last); }
  void exec_MLA_DT(Inst const inst, bool const is_last)    { return g_MLA.gen_dt_code(inst, is_last); }
  void exec_MLAS_DT(Inst const inst, bool const is_last)   { return g_MLAS.gen_dt_code(inst, is_last); }
  void exec_UMULL_DT(Inst const inst, bool const is_last)  { return g_UMULL.gen_dt_code(inst, is_last); }
  void exec_UMULLS_DT(Inst const inst, bool const is_last) { return g_UMULLS.gen_dt_code(inst, is_last); }
  void exec_UMLAL_DT(Inst const inst, bool const is_last)  { return g_UMLAL.gen_dt_code(inst, is_last); }
  void exec_UMLALS_DT(Inst const inst, bool const is_last) { return g_UMLALS.gen_dt_code(inst, is_last); }
  void exec_SMULL_DT(Inst const inst, bool const is_last)  { return g_SMULL.gen_dt_code(inst, is_last); }
  void exec_SMULLS_DT(Inst const inst, bool const is_last) { return g_SMULLS.gen_dt_code(inst, is_last); }
  void exec_SMLAL_DT(Inst const inst, bool const is_last)  { return g_SMLAL.gen_dt_code(inst, is_last); }
  void exec_SMLALS_DT(Inst const inst, bool const is_last) { return g_SMLALS.gen_dt_code(inst, is_last); }
  
  // MRS - dynamic translator version
  
  void exec_MRS_cpsr_DT(Inst const inst, bool const is_last) { return g_MRS_cpsr.gen_dt_code(inst, is_last); }
  void exec_MRS_spsr_DT(Inst const inst, bool const is_last) { return g_MRS_spsr.gen_dt_code(inst, is_last); }
  
  // MSR - dynamic translator version
  
  void exec_MSR_reg_cpsr_DT(Inst const inst, bool const is_last) { return g_MSR_reg_cpsr.gen_dt_code(inst, is_last); }
  void exec_MSR_reg_spsr_DT(Inst const inst, bool const is_last) { return g_MSR_reg_spsr.gen_dt_code(inst, is_last); }
  void exec_MSR_imm_cpsr_DT(Inst const inst, bool const is_last) { return g_MSR_imm_cpsr.gen_dt_code(inst, is_last); }
  void exec_MSR_imm_spsr_DT(Inst const inst, bool const is_last) { return g_MSR_imm_spsr.gen_dt_code(inst, is_last); }
  
  // Undefined instruction - dynamic translator version
  
  void
  exec_UND_DT(Inst const /* inst */, bool const /* is_last */)
  {
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMov *>(0),
                                                 gp_chunk->find_const_var(Core::EXCEPTION_UND),
                                                 gp_chunk->find_global_var(Variable::EXCEPTION_TYPE)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                 gp_chunk->find_const_var(ER_EXCEPTION)));
  }
  
  // CDP - dynamic translator
  
  void
  exec_CDP_DT(Inst const inst, bool const is_last)
  {
    switch (CDPCpNum(inst))
    {
    case CP_0:
    case CP_1:
    case CP_2:
    case CP_3:
    case CP_4:
    case CP_5:
    case CP_6:
    case CP_7:
    case CP_8:
    case CP_9:
    case CP_10:
    case CP_11:
    case CP_12:
    case CP_13:
    case CP_14:
      // :ARM-DDI-0100E: p.A4-20: Wei 2004-May-12:
      //
      // If no coprocessors indicate that they can execute the instruction,
      // an Undefined Instruction exception is generated.
      exec_UND_DT(inst, is_last);
      break;
      
    case CP_15:
      // :ARM-DDI-0100E: p.B2-3: Wei 2004-May-12:
      //
      // The only defined System Control coprocessor instructions are:
      //  - MCR instructions to write an ARM register to a CP15 register
      //  - MRC instructions to read the value of a CP15 register into an ARM register.
      //
      // All CP15 CDP, LDC and STC instructions are UNDEFINED.
      exec_UND_DT(inst, is_last);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  // CDT - dynamic translator
  
  void
  exec_CDT_DT(Inst const inst, bool const is_last)
  {
    switch (CDTCpNum(inst))
    {
    case CP_0:
    case CP_1:
    case CP_2:
    case CP_3:
    case CP_4:
    case CP_5:
    case CP_6:
    case CP_7:
    case CP_8:
    case CP_9:
    case CP_10:
    case CP_11:
    case CP_12:
    case CP_13:
    case CP_14:
      // :ARM-DDI-0100E: p.A4-28: Wei 2004-May-12:
      //
      // If no coprocessors indicate that they can execute the instruction,
      // an Undefined Instruction exception is generated.
      exec_UND_DT(inst, is_last);
      break;
      
    case CP_15:
      // :ARM-DDI-0100E: p.B2-3: Wei 2004-May-12:
      //
      // The only defined System Control coprocessor instructions are:
      //  - MCR instructions to write an ARM register to a CP15 register
      //  - MRC instructions to read the value of a CP15 register into an ARM register.
      //
      // All CP15 CDP, LDC and STC instructions are UNDEFINED.
      exec_UND_DT(inst, is_last);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  // SWI - dynamic translator
  
  void
  exec_SWI_DT(Inst const /* inst */, bool const /* is_last */)
  {
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMov *>(0),
                                                 gp_chunk->find_const_var(Core::EXCEPTION_SWI),
                                                 gp_chunk->find_global_var(Variable::EXCEPTION_TYPE)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                 gp_chunk->find_const_var(ER_EXCEPTION)));
  }
#endif
}
