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

#ifndef Global_TC_hpp
#define Global_TC_hpp

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
#if ENABLE_THREADED_CODE
  // STM - threaded code version
    
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DA_TC;
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DA_w_TC;
  extern InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_DA_TC;
  
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DB_TC;
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DB_w_TC;
  extern InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_DB_TC;
  
  extern InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IA_TC;
  extern InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IA_w_TC;
  extern InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_IA_TC;
  
  extern InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IB_TC;
  extern InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IB_w_TC;
  extern InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_IB_TC;
  
  // LDM - threaded code version
    
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DA_TC;
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DA_w_TC;
  extern InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DA_TC;
  extern InstLSM<DOWN,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DA_w_TC;
    
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DB_TC;
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DB_w_TC;
  extern InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DB_TC;
  extern InstLSM<DOWN, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DB_w_TC;
    
  extern InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IA_TC;
  extern InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IA_w_TC;
  extern InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IA_TC;
  extern InstLSM<  UP,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IA_w_TC;
    
  extern InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IB_TC;
  extern InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IB_w_TC;
  extern InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IB_TC;
  extern InstLSM<  UP, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IB_w_TC;
  
  // SWP - threaded code version
    
  extern InstSWP<WORD, USAGE_THREADED_CODE> g_SWP_TC;
  extern InstSWP<BYTE, USAGE_THREADED_CODE> g_SWPB_TC;
  
  // STR - threaded code version
  
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_post_up_TC;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_offset_down_TC;
  extern InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_offset_up_TC;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_pre_down_TC;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_pre_up_TC;
  
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_post_up_TC;
  extern InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_offset_down_TC;
  extern InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_offset_up_TC;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_pre_down_TC;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_pre_up_TC;
  
  // STRT - threaded code version
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_imm_post_up_TC;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_reg_post_up_TC;
  
  // STRB - threaded code version
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_post_up_TC;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_offset_down_TC;
  extern InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_offset_up_TC;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_pre_down_TC;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_pre_up_TC;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_post_up_TC;
  extern InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_offset_down_TC;
  extern InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_offset_up_TC;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_pre_down_TC;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_pre_up_TC;
  
  // STRBT - threaded code version
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_imm_post_up_TC;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_reg_post_up_TC;
  
  // LDR - threaded code version
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_post_up_TC;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_offset_down_TC;
  extern InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_offset_up_TC;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_pre_down_TC;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_pre_up_TC;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_post_up_TC;
  extern InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_offset_down_TC;
  extern InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_offset_up_TC;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_pre_down_TC;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_pre_up_TC;
  
  // LDRT - threaded code version
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_imm_post_up_TC;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_reg_post_up_TC;
  
  // LDRB - threaded code version
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_post_up_TC;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_offset_down_TC;
  extern InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_offset_up_TC;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_pre_down_TC;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_pre_up_TC;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_post_up_TC;
  extern InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_offset_down_TC;
  extern InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_offset_up_TC;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_pre_down_TC;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_pre_up_TC;
  
  // LDRBT - threaded code version
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_imm_post_down_TC;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_imm_post_up_TC;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_reg_post_down_TC;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_reg_post_up_TC;
  
  // STRH - threaded code version
    
  extern InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_post_down_TC;
  extern InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_post_up_TC;
  extern InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_offset_down_TC;
  extern InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_offset_up_TC;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_pre_down_TC;
  extern InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_pre_up_TC;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_post_down_TC;
  extern InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_post_up_TC;
  extern InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_offset_down_TC;
  extern InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_offset_up_TC;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_pre_down_TC;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_pre_up_TC;
  
  // LDRH - threaded code version
    
  extern InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_post_down_TC;
  extern InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_post_up_TC;
  extern InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_offset_down_TC;
  extern InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_offset_up_TC;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_pre_down_TC;
  extern InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_pre_up_TC;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_post_down_TC;
  extern InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_post_up_TC;
  extern InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_offset_down_TC;
  extern InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_offset_up_TC;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_pre_down_TC;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_pre_up_TC;
  
  // LDRSB
    
  extern InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_post_down_TC;
  extern InstLSH<RegLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_post_up_TC;
  extern InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_offset_down_TC;
  extern InstLSH<RegLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_offset_up_TC;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_pre_down_TC;
  extern InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_pre_up_TC;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_post_down_TC;
  extern InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_post_up_TC;
  extern InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_offset_down_TC;
  extern InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_offset_up_TC;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_pre_down_TC;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_pre_up_TC;
  
  // LDRSH
    
  extern InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_post_down_TC;
  extern InstLSH<RegLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_post_up_TC;
  extern InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_offset_down_TC;
  extern InstLSH<RegLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_offset_up_TC;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_pre_down_TC;
  extern InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_pre_up_TC;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_post_down_TC;
  extern InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_post_up_TC;
  extern InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_offset_down_TC;
  extern InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_offset_up_TC;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_pre_down_TC;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_pre_up_TC;
  
  // DPI - threaded code version
    
  extern InstDPI<AND, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_AND_regimm_TC;
  extern InstDPI<AND, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_AND_regreg_TC;
  extern InstDPI<AND,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_AND_imm_TC;
  extern InstDPI<AND, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ANDS_regimm_TC;
  extern InstDPI<AND, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ANDS_regreg_TC;
  extern InstDPI<AND,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ANDS_imm_TC;
    
  extern InstDPI<EOR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_EOR_regimm_TC;
  extern InstDPI<EOR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_EOR_regreg_TC;
  extern InstDPI<EOR,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_EOR_imm_TC;
  extern InstDPI<EOR, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_EORS_regimm_TC;
  extern InstDPI<EOR, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_EORS_regreg_TC;
  extern InstDPI<EOR,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_EORS_imm_TC;
    
  extern InstDPI<SUB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SUB_regimm_TC;
  extern InstDPI<SUB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SUB_regreg_TC;
  extern InstDPI<SUB,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SUB_imm_TC;
  extern InstDPI<SUB, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SUBS_regimm_TC;
  extern InstDPI<SUB, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SUBS_regreg_TC;
  extern InstDPI<SUB,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SUBS_imm_TC;
    
  extern InstDPI<RSB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSB_regimm_TC;
  extern InstDPI<RSB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSB_regreg_TC;
  extern InstDPI<RSB,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSB_imm_TC;
  extern InstDPI<RSB, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSBS_regimm_TC;
  extern InstDPI<RSB, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSBS_regreg_TC;
  extern InstDPI<RSB,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSBS_imm_TC;
    
  extern InstDPI<ADD, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADD_regimm_TC;
  extern InstDPI<ADD, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADD_regreg_TC;
  extern InstDPI<ADD,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADD_imm_TC;
  extern InstDPI<ADD, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADDS_regimm_TC;
  extern InstDPI<ADD, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADDS_regreg_TC;
  extern InstDPI<ADD,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADDS_imm_TC;
    
  extern InstDPI<ADC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADC_regimm_TC;
  extern InstDPI<ADC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADC_regreg_TC;
  extern InstDPI<ADC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADC_imm_TC;
  extern InstDPI<ADC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADCS_regimm_TC;
  extern InstDPI<ADC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADCS_regreg_TC;
  extern InstDPI<ADC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADCS_imm_TC;
    
  extern InstDPI<SBC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SBC_regimm_TC;
  extern InstDPI<SBC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SBC_regreg_TC;
  extern InstDPI<SBC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SBC_imm_TC;
  extern InstDPI<SBC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SBCS_regimm_TC;
  extern InstDPI<SBC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SBCS_regreg_TC;
  extern InstDPI<SBC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SBCS_imm_TC;
    
  extern InstDPI<RSC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSC_regimm_TC;
  extern InstDPI<RSC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSC_regreg_TC;
  extern InstDPI<RSC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSC_imm_TC;
  extern InstDPI<RSC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSCS_regimm_TC;
  extern InstDPI<RSC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSCS_regreg_TC;
  extern InstDPI<RSC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSCS_imm_TC;
    
  extern InstDPI<ORR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ORR_regimm_TC;
  extern InstDPI<ORR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ORR_regreg_TC;
  extern InstDPI<ORR,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ORR_imm_TC;
  extern InstDPI<ORR, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ORRS_regimm_TC;
  extern InstDPI<ORR, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ORRS_regreg_TC;
  extern InstDPI<ORR,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ORRS_imm_TC;
    
  extern InstDPI<MOV, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MOV_regimm_TC;
  extern InstDPI<MOV, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MOV_regreg_TC;
  extern InstDPI<MOV,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MOV_imm_TC;
  extern InstDPI<MOV, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MOVS_regimm_TC;
  extern InstDPI<MOV, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MOVS_regreg_TC;
  extern InstDPI<MOV,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MOVS_imm_TC;
    
  extern InstDPI<BIC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_BIC_regimm_TC;
  extern InstDPI<BIC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_BIC_regreg_TC;
  extern InstDPI<BIC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_BIC_imm_TC;
  extern InstDPI<BIC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_BICS_regimm_TC;
  extern InstDPI<BIC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_BICS_regreg_TC;
  extern InstDPI<BIC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_BICS_imm_TC;
    
  extern InstDPI<MVN, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MVN_regimm_TC;
  extern InstDPI<MVN, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MVN_regreg_TC;
  extern InstDPI<MVN,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MVN_imm_TC;
  extern InstDPI<MVN, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MVNS_regimm_TC;
  extern InstDPI<MVN, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MVNS_regreg_TC;
  extern InstDPI<MVN,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MVNS_imm_TC;
    
  extern InstDPI<TST, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TST_regimm_TC;
  extern InstDPI<TST, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TST_regreg_TC;
  extern InstDPI<TST,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TST_imm_TC;
    
  extern InstDPI<TEQ, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TEQ_regimm_TC;
  extern InstDPI<TEQ, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TEQ_regreg_TC;
  extern InstDPI<TEQ,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TEQ_imm_TC;
    
  extern InstDPI<CMP, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMP_regimm_TC;
  extern InstDPI<CMP, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMP_regreg_TC;
  extern InstDPI<CMP,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMP_imm_TC;
    
  extern InstDPI<CMN, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMN_regimm_TC;
  extern InstDPI<CMN, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMN_regreg_TC;
  extern InstDPI<CMN,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMN_imm_TC;
  
  // Branch
  
  extern InstBranch<ImmBranch,    LINK,    EXCHANGE, USAGE_THREADED_CODE> g_BLX_1_TC;
  extern InstBranch<RegBranch,    LINK,    EXCHANGE, USAGE_THREADED_CODE> g_BLX_2_TC;
  extern InstBranch<RegBranch, NO_LINK,    EXCHANGE, USAGE_THREADED_CODE> g_BX_TC;
  extern InstBranch<ImmBranch, NO_LINK, NO_EXCHANGE, USAGE_THREADED_CODE> g_B_TC;
  extern InstBranch<ImmBranch,    LINK, NO_EXCHANGE, USAGE_THREADED_CODE> g_BL_TC;
  
  // STM - threaded code version
  
  extern ExecResultEnum exec_STM_1_DA_TC(Inst const inst);
  extern ExecResultEnum exec_STM_1_DA_w_TC(Inst const inst);
  extern ExecResultEnum exec_STM_2_DA_TC(Inst const inst);
  
  extern ExecResultEnum exec_STM_1_DB_TC(Inst const inst);
  extern ExecResultEnum exec_STM_1_DB_w_TC(Inst const inst);
  extern ExecResultEnum exec_STM_2_DB_TC(Inst const inst);
  
  extern ExecResultEnum exec_STM_1_IA_TC(Inst const inst);
  extern ExecResultEnum exec_STM_1_IA_w_TC(Inst const inst);
  extern ExecResultEnum exec_STM_2_IA_TC(Inst const inst);
  
  extern ExecResultEnum exec_STM_1_IB_TC(Inst const inst);
  extern ExecResultEnum exec_STM_1_IB_w_TC(Inst const inst);
  extern ExecResultEnum exec_STM_2_IB_TC(Inst const inst);
  
  // LDM - threaded code version
  
  extern ExecResultEnum exec_LDM_1_DA_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_1_DA_w_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_DA_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_DA_w_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDM_1_DB_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_1_DB_w_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_DB_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_DB_w_TC(Inst const inst);
    
  extern ExecResultEnum exec_LDM_1_IA_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_1_IA_w_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_IA_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_IA_w_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDM_1_IB_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_1_IB_w_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_IB_TC(Inst const inst);
  extern ExecResultEnum exec_LDM_23_IB_w_TC(Inst const inst);
  
  // SWP - threaded code version
  
  extern ExecResultEnum exec_SWP_TC(Inst const inst);
  extern ExecResultEnum exec_SWPB_TC(Inst const inst);
  
  // STR - threaded code version
  
  extern ExecResultEnum exec_STR_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STR_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_STR_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_STR_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_STR_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_STR_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_STR_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STR_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_STR_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_STR_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_STR_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_STR_reg_pre_up_TC(Inst const inst);
  
  // STRT - threaded code version
  
  extern ExecResultEnum exec_STRT_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRT_imm_post_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_STRT_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRT_reg_post_up_TC(Inst const inst);
  
  // STRB - threaded code version
  
  extern ExecResultEnum exec_STRB_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_STRB_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRB_reg_pre_up_TC(Inst const inst);
  
  // STRBT - threaded code version
  
  extern ExecResultEnum exec_STRBT_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRBT_imm_post_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_STRBT_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRBT_reg_post_up_TC(Inst const inst);
  
  // LDR - threaded code version
  
  extern ExecResultEnum exec_LDR_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDR_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDR_reg_pre_up_TC(Inst const inst);
  
  // LDRT - threaded code version
  
  extern ExecResultEnum exec_LDRT_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRT_imm_post_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDRT_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRT_reg_post_up_TC(Inst const inst);
  
  // LDRB - threaded code version
  
  extern ExecResultEnum exec_LDRB_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDRB_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRB_reg_pre_up_TC(Inst const inst);
  
  // LDRBT - threaded code version
  
  extern ExecResultEnum exec_LDRBT_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRBT_imm_post_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDRBT_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRBT_reg_post_up_TC(Inst const inst);
  
  // STRH - threaded code version
  
  extern ExecResultEnum exec_STRH_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_STRH_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_STRH_reg_pre_up_TC(Inst const inst);
  
  // LDRH - threaded code version
  
  extern ExecResultEnum exec_LDRH_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDRH_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRH_reg_pre_up_TC(Inst const inst);
  
  // LDRSB - threaded code version
  
  extern ExecResultEnum exec_LDRSB_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDRSB_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSB_reg_pre_up_TC(Inst const inst);
  
  // LDRSH - threaded code version
  
  extern ExecResultEnum exec_LDRSH_imm_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_imm_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_imm_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_imm_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_imm_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_imm_pre_up_TC(Inst const inst);
  
  extern ExecResultEnum exec_LDRSH_reg_post_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_reg_post_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_reg_offset_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_reg_offset_up_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_reg_pre_down_TC(Inst const inst);
  extern ExecResultEnum exec_LDRSH_reg_pre_up_TC(Inst const inst);
  
  // DPI - threaded code version
  
  extern ExecResultEnum exec_AND_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_AND_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_AND_imm_TC(Inst const inst);
  extern ExecResultEnum exec_ANDS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_ANDS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_ANDS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_EOR_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_EOR_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_EOR_imm_TC(Inst const inst);
  extern ExecResultEnum exec_EORS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_EORS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_EORS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_SUB_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_SUB_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_SUB_imm_TC(Inst const inst);
  extern ExecResultEnum exec_SUBS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_SUBS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_SUBS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_RSB_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_RSB_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_RSB_imm_TC(Inst const inst);
  extern ExecResultEnum exec_RSBS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_RSBS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_RSBS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_ADD_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_ADD_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_ADD_imm_TC(Inst const inst);
  extern ExecResultEnum exec_ADDS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_ADDS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_ADDS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_ADC_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_ADC_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_ADC_imm_TC(Inst const inst);
  extern ExecResultEnum exec_ADCS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_ADCS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_ADCS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_SBC_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_SBC_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_SBC_imm_TC(Inst const inst);
  extern ExecResultEnum exec_SBCS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_SBCS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_SBCS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_RSC_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_RSC_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_RSC_imm_TC(Inst const inst);
  extern ExecResultEnum exec_RSCS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_RSCS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_RSCS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_ORR_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_ORR_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_ORR_imm_TC(Inst const inst);
  extern ExecResultEnum exec_ORRS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_ORRS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_ORRS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_MOV_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_MOV_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_MOV_imm_TC(Inst const inst);
  extern ExecResultEnum exec_MOVS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_MOVS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_MOVS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_BIC_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_BIC_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_BIC_imm_TC(Inst const inst);
  extern ExecResultEnum exec_BICS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_BICS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_BICS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_MVN_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_MVN_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_MVN_imm_TC(Inst const inst);
  extern ExecResultEnum exec_MVNS_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_MVNS_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_MVNS_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_TST_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_TST_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_TST_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_TEQ_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_TEQ_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_TEQ_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_CMP_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_CMP_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_CMP_imm_TC(Inst const inst);
  
  extern ExecResultEnum exec_CMN_regimm_TC(Inst const inst);
  extern ExecResultEnum exec_CMN_regreg_TC(Inst const inst);
  extern ExecResultEnum exec_CMN_imm_TC(Inst const inst);
  
  // Branch - threaded code version
  
  extern ExecResultEnum exec_BLX_1_TC(Inst const inst);
  extern ExecResultEnum exec_BLX_2_TC(Inst const inst);
  extern ExecResultEnum exec_BX_TC(Inst const inst);
  extern ExecResultEnum exec_B_TC(Inst const inst);
  extern ExecResultEnum exec_BL_TC(Inst const inst);
  
  // Multi - threaded code version
  
  extern ExecResultEnum exec_MUL_TC(Inst const inst);
  extern ExecResultEnum exec_MULS_TC(Inst const inst);
  extern ExecResultEnum exec_MLA_TC(Inst const inst);
  extern ExecResultEnum exec_MLAS_TC(Inst const inst);
  extern ExecResultEnum exec_UMULL_TC(Inst const inst);
  extern ExecResultEnum exec_UMULLS_TC(Inst const inst);
  extern ExecResultEnum exec_UMLAL_TC(Inst const inst);
  extern ExecResultEnum exec_UMLALS_TC(Inst const inst);
  extern ExecResultEnum exec_SMULL_TC(Inst const inst);
  extern ExecResultEnum exec_SMULLS_TC(Inst const inst);
  extern ExecResultEnum exec_SMLAL_TC(Inst const inst);
  extern ExecResultEnum exec_SMLALS_TC(Inst const inst);
  
  // MRS - threaded code version
  
  extern ExecResultEnum exec_MRS_cpsr_TC(Inst const inst);
  extern ExecResultEnum exec_MRS_spsr_TC(Inst const inst);
  
  // MSR - threaded code version
  
  extern ExecResultEnum exec_MSR_reg_cpsr_TC(Inst const inst);
  extern ExecResultEnum exec_MSR_reg_spsr_TC(Inst const inst);
  extern ExecResultEnum exec_MSR_imm_cpsr_TC(Inst const inst);
  extern ExecResultEnum exec_MSR_imm_spsr_TC(Inst const inst);
  
  // CDT - threaded code version
  
  extern ExecResultEnum exec_CDT_TC(Inst const inst);
  
  // CDP - threaded code version
  
  extern ExecResultEnum exec_CDP_TC(Inst const inst);
  
  // Undefined instruction - threaded code version
  
  extern ExecResultEnum exec_UND_TC(Inst const inst);
  
  // SWI - threaded code version
  
  extern ExecResultEnum exec_SWI_TC(Inst const inst);
#endif
}

#endif
