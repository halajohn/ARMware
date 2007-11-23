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

#ifndef Global_hpp
#define Global_hpp

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
  // STM
  
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DA;
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DA_w;
  extern InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_DA;
    
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DB;
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DB_w;
  extern InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_DB;
    
  extern InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IA;
  extern InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IA_w;
  extern InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_IA;
    
  extern InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IB;
  extern InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IB_w;
  extern InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_IB;
  
  // LDM
    
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DA;
  extern InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DA_w;
  extern InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DA;
  extern InstLSM<DOWN,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DA_w;
    
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DB;
  extern InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DB_w;
  extern InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DB;
  extern InstLSM<DOWN, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DB_w;
  
  extern InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IA;
  extern InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IA_w;
  extern InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IA;
  extern InstLSM<  UP,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IA_w;
    
  extern InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IB;
  extern InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IB_w;
  extern InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IB;
  extern InstLSM<  UP, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IB_w;
    
  // SWP
    
  extern InstSWP<WORD, USAGE_NORMAL> g_SWP;
  extern InstSWP<BYTE, USAGE_NORMAL> g_SWPB;
    
  // STR
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_post_up;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_offset_down;
  extern InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_offset_up;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_pre_down;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_pre_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_post_up;
  extern InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_offset_down;
  extern InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_offset_up;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_pre_down;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_pre_up;
    
  // STRT
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_imm_post_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_reg_post_up;
    
  // STRB
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_post_up;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_offset_down;
  extern InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_offset_up;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_pre_down;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_pre_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_post_up;
  extern InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_offset_down;
  extern InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_offset_up;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_pre_down;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_pre_up;
    
  // STRBT
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_imm_post_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_reg_post_up;
    
  // LDR
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_post_up;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_offset_down;
  extern InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_offset_up;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_pre_down;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_pre_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_post_up;
  extern InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_offset_down;
  extern InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_offset_up;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_pre_down;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_pre_up;
    
  // LDRT
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_imm_post_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_reg_post_up;
    
  // LDRB
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_post_up;
  extern InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_offset_down;
  extern InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_offset_up;
  extern InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_pre_down;
  extern InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_pre_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_post_up;
  extern InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_offset_down;
  extern InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_offset_up;
  extern InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_pre_down;
  extern InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_pre_up;
    
  // LDRBT
    
  extern InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_imm_post_down;
  extern InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_imm_post_up;
    
  extern InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_reg_post_down;
  extern InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_reg_post_up;
    
  // STRH
    
  extern InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_post_down;
  extern InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_post_up;
  extern InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_offset_down;
  extern InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_offset_up;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_pre_down;
  extern InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_pre_up;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_post_down;
  extern InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_post_up;
  extern InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_offset_down;
  extern InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_offset_up;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_pre_down;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_pre_up;
    
  // LDRH
    
  extern InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_post_down;
  extern InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_post_up;
  extern InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_offset_down;
  extern InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_offset_up;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_pre_down;
  extern InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_pre_up;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_post_down;
  extern InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_post_up;
  extern InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_offset_down;
  extern InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_offset_up;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_pre_down;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_pre_up;
    
  // LDRSB
    
  extern InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_post_down;
  extern InstLSH<RegLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_post_up;
  extern InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_offset_down;
  extern InstLSH<RegLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_offset_up;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_pre_down;
  extern InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_pre_up;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_post_down;
  extern InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_post_up;
  extern InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_offset_down;
  extern InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_offset_up;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_pre_down;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_pre_up;
    
  // LDRSH
    
  extern InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_post_down;
  extern InstLSH<RegLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_post_up;
  extern InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_offset_down;
  extern InstLSH<RegLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_offset_up;
  extern InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_pre_down;
  extern InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_pre_up;
    
  extern InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_post_down;
  extern InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_post_up;
  extern InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_offset_down;
  extern InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_offset_up;
  extern InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_pre_down;
  extern InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_pre_up;
    
  // DPI
    
  extern InstDPI<AND, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_AND_regimm;
  extern InstDPI<AND, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_AND_regreg;
  extern InstDPI<AND,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_AND_imm;
  extern InstDPI<AND, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ANDS_regimm;
  extern InstDPI<AND, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ANDS_regreg;
  extern InstDPI<AND,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ANDS_imm;
    
  extern InstDPI<EOR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_EOR_regimm;
  extern InstDPI<EOR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_EOR_regreg;
  extern InstDPI<EOR,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_EOR_imm;
  extern InstDPI<EOR, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_EORS_regimm;
  extern InstDPI<EOR, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_EORS_regreg;
  extern InstDPI<EOR,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_EORS_imm;
    
  extern InstDPI<SUB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SUB_regimm;
  extern InstDPI<SUB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SUB_regreg;
  extern InstDPI<SUB,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SUB_imm;
  extern InstDPI<SUB, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SUBS_regimm;
  extern InstDPI<SUB, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SUBS_regreg;
  extern InstDPI<SUB,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SUBS_imm;
    
  extern InstDPI<RSB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSB_regimm;
  extern InstDPI<RSB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSB_regreg;
  extern InstDPI<RSB,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSB_imm;
  extern InstDPI<RSB, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSBS_regimm;
  extern InstDPI<RSB, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSBS_regreg;
  extern InstDPI<RSB,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSBS_imm;
    
  extern InstDPI<ADD, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADD_regimm;
  extern InstDPI<ADD, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADD_regreg;
  extern InstDPI<ADD,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADD_imm;
  extern InstDPI<ADD, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADDS_regimm;
  extern InstDPI<ADD, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADDS_regreg;
  extern InstDPI<ADD,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADDS_imm;
    
  extern InstDPI<ADC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADC_regimm;
  extern InstDPI<ADC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADC_regreg;
  extern InstDPI<ADC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADC_imm;
  extern InstDPI<ADC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADCS_regimm;
  extern InstDPI<ADC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADCS_regreg;
  extern InstDPI<ADC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADCS_imm;
    
  extern InstDPI<SBC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SBC_regimm;
  extern InstDPI<SBC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SBC_regreg;
  extern InstDPI<SBC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SBC_imm;
  extern InstDPI<SBC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SBCS_regimm;
  extern InstDPI<SBC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SBCS_regreg;
  extern InstDPI<SBC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SBCS_imm;
    
  extern InstDPI<RSC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSC_regimm;
  extern InstDPI<RSC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSC_regreg;
  extern InstDPI<RSC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSC_imm;
  extern InstDPI<RSC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSCS_regimm;
  extern InstDPI<RSC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSCS_regreg;
  extern InstDPI<RSC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSCS_imm;
    
  extern InstDPI<ORR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ORR_regimm;
  extern InstDPI<ORR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ORR_regreg;
  extern InstDPI<ORR,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ORR_imm;
  extern InstDPI<ORR, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ORRS_regimm;
  extern InstDPI<ORR, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ORRS_regreg;
  extern InstDPI<ORR,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ORRS_imm;
    
  extern InstDPI<MOV, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MOV_regimm;
  extern InstDPI<MOV, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MOV_regreg;
  extern InstDPI<MOV,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MOV_imm;
  extern InstDPI<MOV, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MOVS_regimm;
  extern InstDPI<MOV, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MOVS_regreg;
  extern InstDPI<MOV,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MOVS_imm;
    
  extern InstDPI<BIC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_BIC_regimm;
  extern InstDPI<BIC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_BIC_regreg;
  extern InstDPI<BIC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_BIC_imm;
  extern InstDPI<BIC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_BICS_regimm;
  extern InstDPI<BIC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_BICS_regreg;
  extern InstDPI<BIC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_BICS_imm;
    
  extern InstDPI<MVN, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MVN_regimm;
  extern InstDPI<MVN, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MVN_regreg;
  extern InstDPI<MVN,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MVN_imm;
  extern InstDPI<MVN, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MVNS_regimm;
  extern InstDPI<MVN, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MVNS_regreg;
  extern InstDPI<MVN,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MVNS_imm;
    
  extern InstDPI<TST, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TST_regimm;
  extern InstDPI<TST, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TST_regreg;
  extern InstDPI<TST,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TST_imm;
    
  extern InstDPI<TEQ, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TEQ_regimm;
  extern InstDPI<TEQ, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TEQ_regreg;
  extern InstDPI<TEQ,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TEQ_imm;
    
  extern InstDPI<CMP, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMP_regimm;
  extern InstDPI<CMP, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMP_regreg;
  extern InstDPI<CMP,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMP_imm;
    
  extern InstDPI<CMN, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMN_regimm;
  extern InstDPI<CMN, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMN_regreg;
  extern InstDPI<CMN,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMN_imm;
    
  // Branch
    
  extern InstBranch<ImmBranch,    LINK,    EXCHANGE, USAGE_NORMAL> g_BLX_1;
  extern InstBranch<RegBranch,    LINK,    EXCHANGE, USAGE_NORMAL> g_BLX_2;
  extern InstBranch<RegBranch, NO_LINK,    EXCHANGE, USAGE_NORMAL> g_BX;
  extern InstBranch<ImmBranch, NO_LINK, NO_EXCHANGE, USAGE_NORMAL> g_B;
  extern InstBranch<ImmBranch,    LINK, NO_EXCHANGE, USAGE_NORMAL> g_BL;
  
  // Multi
  
  extern InstMulti<NORMAL,   SIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS> g_MUL;
  extern InstMulti<NORMAL,   SIGNED, NO_ACCUMULATE,    UPDATE_STATUS> g_MULS;
  extern InstMulti<NORMAL,   SIGNED,    ACCUMULATE, NO_UPDATE_STATUS> g_MLA;
  extern InstMulti<NORMAL,   SIGNED,    ACCUMULATE,    UPDATE_STATUS> g_MLAS;
  extern InstMulti<  LONG, UNSIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS> g_UMULL;
  extern InstMulti<  LONG, UNSIGNED, NO_ACCUMULATE,    UPDATE_STATUS> g_UMULLS;
  extern InstMulti<  LONG, UNSIGNED,    ACCUMULATE, NO_UPDATE_STATUS> g_UMLAL;
  extern InstMulti<  LONG, UNSIGNED,    ACCUMULATE,    UPDATE_STATUS> g_UMLALS;
  extern InstMulti<  LONG,   SIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS> g_SMULL;
  extern InstMulti<  LONG,   SIGNED, NO_ACCUMULATE,    UPDATE_STATUS> g_SMULLS;
  extern InstMulti<  LONG,   SIGNED,    ACCUMULATE, NO_UPDATE_STATUS> g_SMLAL;
  extern InstMulti<  LONG,   SIGNED,    ACCUMULATE,    UPDATE_STATUS> g_SMLALS;
    
  // MRS
    
  extern InstMRS<        USE_CPSR> g_MRS_cpsr;
  extern InstMRS<USE_CURRENT_SPSR> g_MRS_spsr;
    
  // MSR
    
  extern InstMSR<RegMSR,         USE_CPSR> g_MSR_reg_cpsr;
  extern InstMSR<RegMSR, USE_CURRENT_SPSR> g_MSR_reg_spsr;
  extern InstMSR<ImmMSR,         USE_CPSR> g_MSR_imm_cpsr;
  extern InstMSR<ImmMSR, USE_CURRENT_SPSR> g_MSR_imm_spsr;
}

#endif
