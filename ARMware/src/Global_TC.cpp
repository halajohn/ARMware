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

#include "Global_TC.hpp"

#include "Inst/InstDPI.tcc"
#include "Inst/InstLSM.tcc"
#include "Inst/InstLSW.tcc"
#include "Inst/InstLSH.tcc"
#include "Inst/InstSWP.tcc"
#include "Inst/InstBranch.tcc"
#include "Inst/InstMulti.tcc"
#include "Inst/InstMRS.tcc"
#include "Inst/InstMSR.tcc"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // Threaded code version
  
#if ENABLE_THREADED_CODE
  // STM - threaded code version
    
  InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DA_TC;
  InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DA_w_TC;
  InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_DA_TC;
    
  InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DB_TC;
  InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_DB_w_TC;
  InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_DB_TC;
    
  InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IA_TC;
  InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IA_w_TC;
  InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_IA_TC;
    
  InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IB_TC;
  InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_1_IB_w_TC;
  InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE> g_STM_2_IB_TC;
  
  // LDM - threaded code version
    
  InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DA_TC;
  InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DA_w_TC;
  InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DA_TC;
  InstLSM<DOWN,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DA_w_TC;
    
  InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DB_TC;
  InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_DB_w_TC;
  InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DB_TC;
  InstLSM<DOWN, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_DB_w_TC;
    
  InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IA_TC;
  InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IA_w_TC;
  InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IA_TC;
  InstLSM<  UP,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IA_w_TC;
    
  InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IB_TC;
  InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_1_IB_w_TC;
  InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IB_TC;
  InstLSM<  UP, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE> g_LDM_23_IB_w_TC;
  
  // SWP - threaded code version
    
  InstSWP<WORD, USAGE_THREADED_CODE> g_SWP_TC;
  InstSWP<BYTE, USAGE_THREADED_CODE> g_SWPB_TC;
  
  // STR - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_post_up_TC;
  InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_offset_down_TC;
  InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_offset_up_TC;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_pre_down_TC;
  InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_imm_pre_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_post_up_TC;
  InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_offset_down_TC;
  InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_offset_up_TC;
  InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_pre_down_TC;
  InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STR_reg_pre_up_TC;
  
  // STRT - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_imm_post_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRT_reg_post_up_TC;
  
  // STRB - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_post_up_TC;
  InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_offset_down_TC;
  InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_offset_up_TC;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_pre_down_TC;
  InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_imm_pre_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_post_up_TC;
  InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_offset_down_TC;
  InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_offset_up_TC;
  InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_pre_down_TC;
  InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE> g_STRB_reg_pre_up_TC;
  
  // STRBT - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_imm_post_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE> g_STRBT_reg_post_up_TC;
  
  // LDR - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_post_up_TC;
  InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_offset_down_TC;
  InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_offset_up_TC;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_pre_down_TC;
  InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_imm_pre_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_post_up_TC;
  InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_offset_down_TC;
  InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_offset_up_TC;
  InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_pre_down_TC;
  InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDR_reg_pre_up_TC;
  
  // LDRT - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_imm_post_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRT_reg_post_up_TC;
  
  // LDRB - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_post_up_TC;
  InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_offset_down_TC;
  InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_offset_up_TC;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_pre_down_TC;
  InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_imm_pre_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_post_up_TC;
  InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_offset_down_TC;
  InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_offset_up_TC;
  InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_pre_down_TC;
  InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRB_reg_pre_up_TC;
  
  // LDRBT - threaded code version
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_imm_post_down_TC;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_imm_post_up_TC;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_reg_post_down_TC;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE> g_LDRBT_reg_post_up_TC;
  
  // STRH - threaded code version
    
  InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_post_down_TC;
  InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_post_up_TC;
  InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_offset_down_TC;
  InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_offset_up_TC;
  InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_pre_down_TC;
  InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_reg_pre_up_TC;
    
  InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_post_down_TC;
  InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_post_up_TC;
  InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_offset_down_TC;
  InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_offset_up_TC;
  InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_pre_down_TC;
  InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE> g_STRH_imm_pre_up_TC;
  
  // LDRH - threaded code version
    
  InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_post_down_TC;
  InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_post_up_TC;
  InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_offset_down_TC;
  InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_offset_up_TC;
  InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_pre_down_TC;
  InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_reg_pre_up_TC;
    
  InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_post_down_TC;
  InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_post_up_TC;
  InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_offset_down_TC;
  InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_offset_up_TC;
  InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_pre_down_TC;
  InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRH_imm_pre_up_TC;
  
  // LDRSB
    
  InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_post_down_TC;
  InstLSH<RegLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_post_up_TC;
  InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_offset_down_TC;
  InstLSH<RegLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_offset_up_TC;
  InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_pre_down_TC;
  InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_reg_pre_up_TC;
    
  InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_post_down_TC;
  InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_post_up_TC;
  InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_offset_down_TC;
  InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_offset_up_TC;
  InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_pre_down_TC;
  InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE> g_LDRSB_imm_pre_up_TC;
  
  // LDRSH
    
  InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_post_down_TC;
  InstLSH<RegLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_post_up_TC;
  InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_offset_down_TC;
  InstLSH<RegLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_offset_up_TC;
  InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_pre_down_TC;
  InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_reg_pre_up_TC;
    
  InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_post_down_TC;
  InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_post_up_TC;
  InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_offset_down_TC;
  InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_offset_up_TC;
  InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_pre_down_TC;
  InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE> g_LDRSH_imm_pre_up_TC;
  
  // DPI - threaded code version
    
  InstDPI<AND, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_AND_regimm_TC;
  InstDPI<AND, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_AND_regreg_TC;
  InstDPI<AND,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_AND_imm_TC;
  InstDPI<AND, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ANDS_regimm_TC;
  InstDPI<AND, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ANDS_regreg_TC;
  InstDPI<AND,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ANDS_imm_TC;
    
  InstDPI<EOR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_EOR_regimm_TC;
  InstDPI<EOR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_EOR_regreg_TC;
  InstDPI<EOR,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_EOR_imm_TC;
  InstDPI<EOR, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_EORS_regimm_TC;
  InstDPI<EOR, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_EORS_regreg_TC;
  InstDPI<EOR,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_EORS_imm_TC;
    
  InstDPI<SUB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SUB_regimm_TC;
  InstDPI<SUB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SUB_regreg_TC;
  InstDPI<SUB,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SUB_imm_TC;
  InstDPI<SUB, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SUBS_regimm_TC;
  InstDPI<SUB, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SUBS_regreg_TC;
  InstDPI<SUB,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SUBS_imm_TC;
    
  InstDPI<RSB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSB_regimm_TC;
  InstDPI<RSB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSB_regreg_TC;
  InstDPI<RSB,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSB_imm_TC;
  InstDPI<RSB, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSBS_regimm_TC;
  InstDPI<RSB, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSBS_regreg_TC;
  InstDPI<RSB,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSBS_imm_TC;
    
  InstDPI<ADD, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADD_regimm_TC;
  InstDPI<ADD, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADD_regreg_TC;
  InstDPI<ADD,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADD_imm_TC;
  InstDPI<ADD, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADDS_regimm_TC;
  InstDPI<ADD, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADDS_regreg_TC;
  InstDPI<ADD,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADDS_imm_TC;
    
  InstDPI<ADC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADC_regimm_TC;
  InstDPI<ADC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADC_regreg_TC;
  InstDPI<ADC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ADC_imm_TC;
  InstDPI<ADC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADCS_regimm_TC;
  InstDPI<ADC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADCS_regreg_TC;
  InstDPI<ADC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ADCS_imm_TC;
    
  InstDPI<SBC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SBC_regimm_TC;
  InstDPI<SBC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SBC_regreg_TC;
  InstDPI<SBC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_SBC_imm_TC;
  InstDPI<SBC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SBCS_regimm_TC;
  InstDPI<SBC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SBCS_regreg_TC;
  InstDPI<SBC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_SBCS_imm_TC;
    
  InstDPI<RSC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSC_regimm_TC;
  InstDPI<RSC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSC_regreg_TC;
  InstDPI<RSC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_RSC_imm_TC;
  InstDPI<RSC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSCS_regimm_TC;
  InstDPI<RSC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSCS_regreg_TC;
  InstDPI<RSC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_RSCS_imm_TC;
    
  InstDPI<ORR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ORR_regimm_TC;
  InstDPI<ORR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ORR_regreg_TC;
  InstDPI<ORR,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_ORR_imm_TC;
  InstDPI<ORR, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ORRS_regimm_TC;
  InstDPI<ORR, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ORRS_regreg_TC;
  InstDPI<ORR,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_ORRS_imm_TC;
    
  InstDPI<MOV, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MOV_regimm_TC;
  InstDPI<MOV, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MOV_regreg_TC;
  InstDPI<MOV,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MOV_imm_TC;
  InstDPI<MOV, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MOVS_regimm_TC;
  InstDPI<MOV, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MOVS_regreg_TC;
  InstDPI<MOV,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MOVS_imm_TC;
    
  InstDPI<BIC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_BIC_regimm_TC;
  InstDPI<BIC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_BIC_regreg_TC;
  InstDPI<BIC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_BIC_imm_TC;
  InstDPI<BIC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_BICS_regimm_TC;
  InstDPI<BIC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_BICS_regreg_TC;
  InstDPI<BIC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_BICS_imm_TC;
    
  InstDPI<MVN, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MVN_regimm_TC;
  InstDPI<MVN, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MVN_regreg_TC;
  InstDPI<MVN,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE> g_MVN_imm_TC;
  InstDPI<MVN, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MVNS_regimm_TC;
  InstDPI<MVN, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MVNS_regreg_TC;
  InstDPI<MVN,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_MVNS_imm_TC;
    
  InstDPI<TST, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TST_regimm_TC;
  InstDPI<TST, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TST_regreg_TC;
  InstDPI<TST,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TST_imm_TC;
    
  InstDPI<TEQ, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TEQ_regimm_TC;
  InstDPI<TEQ, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TEQ_regreg_TC;
  InstDPI<TEQ,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_TEQ_imm_TC;
    
  InstDPI<CMP, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMP_regimm_TC;
  InstDPI<CMP, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMP_regreg_TC;
  InstDPI<CMP,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMP_imm_TC;
    
  InstDPI<CMN, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMN_regimm_TC;
  InstDPI<CMN, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMN_regreg_TC;
  InstDPI<CMN,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE> g_CMN_imm_TC;
  
  // Branch
    
  InstBranch<ImmBranch,    LINK,    EXCHANGE, USAGE_THREADED_CODE> g_BLX_1_TC;
  InstBranch<RegBranch,    LINK,    EXCHANGE, USAGE_THREADED_CODE> g_BLX_2_TC;
  InstBranch<RegBranch, NO_LINK,    EXCHANGE, USAGE_THREADED_CODE> g_BX_TC;
  InstBranch<ImmBranch, NO_LINK, NO_EXCHANGE, USAGE_THREADED_CODE> g_B_TC;
  InstBranch<ImmBranch,    LINK, NO_EXCHANGE, USAGE_THREADED_CODE> g_BL_TC;
  
  // :NOTE: Wei 2004-Aug-23:
  //
  // Because C++'s 'pointer to member function' facility is slower than
  // normal function pointer in some compilers, at least in GNU C++ compiler.
  //
  // One more,
  // because I want to just call 'exec_xxx(Inst const inst)' in my 1st version
  // dynamic translator. However, the assembly level implementation of
  // 'pointer to member function' of C++ is not defined in the C++ standard.
  // (and indeed C++ standard shouldn't define this.)
  //
  // And after searching Google, almost every compiler vender defines their own
  // assembly implementation of their 'pointer to member function'.
  //
  // Thus, if I want to use 'pointer to member function' in my dynamic translator.
  // Not only hard to implement it, but also hard to achieve the portability.
  // Unless I write every vender's assembly implement in my dynamic translator.
  //
  // Because of these 2 reasons (preformance & portability),
  // I decide use 'normal function pointer' rather than 'pointer to member function'.
  
  // STM - threaded code version
  
  ExecResultEnum exec_STM_1_DA_TC(Inst const inst)   { return g_STM_1_DA_TC.Process(inst); }
  ExecResultEnum exec_STM_1_DA_w_TC(Inst const inst) { return g_STM_1_DA_w_TC.Process(inst); }
  ExecResultEnum exec_STM_2_DA_TC(Inst const inst)   { return g_STM_2_DA_TC.Process(inst); }
  
  ExecResultEnum exec_STM_1_DB_TC(Inst const inst)   { return g_STM_1_DB_TC.Process(inst); }
  ExecResultEnum exec_STM_1_DB_w_TC(Inst const inst) { return g_STM_1_DB_w_TC.Process(inst); }
  ExecResultEnum exec_STM_2_DB_TC(Inst const inst)   { return g_STM_2_DB_TC.Process(inst); }
  
  ExecResultEnum exec_STM_1_IA_TC(Inst const inst)   { return g_STM_1_IA_TC.Process(inst); }
  ExecResultEnum exec_STM_1_IA_w_TC(Inst const inst) { return g_STM_1_IA_w_TC.Process(inst); }
  ExecResultEnum exec_STM_2_IA_TC(Inst const inst)   { return g_STM_2_IA_TC.Process(inst); }
  
  ExecResultEnum exec_STM_1_IB_TC(Inst const inst)   { return g_STM_1_IB_TC.Process(inst); }
  ExecResultEnum exec_STM_1_IB_w_TC(Inst const inst) { return g_STM_1_IB_w_TC.Process(inst); }
  ExecResultEnum exec_STM_2_IB_TC(Inst const inst)   { return g_STM_2_IB_TC.Process(inst); }
  
  // LDM - threaded code version
  
  ExecResultEnum exec_LDM_1_DA_TC(Inst const inst)    { return g_LDM_1_DA_TC.Process(inst); }
  ExecResultEnum exec_LDM_1_DA_w_TC(Inst const inst)  { return g_LDM_1_DA_w_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_DA_TC(Inst const inst)   { return g_LDM_23_DA_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_DA_w_TC(Inst const inst) { return g_LDM_23_DA_w_TC.Process(inst); }
  
  ExecResultEnum exec_LDM_1_DB_TC(Inst const inst)    { return g_LDM_1_DB_TC.Process(inst); }
  ExecResultEnum exec_LDM_1_DB_w_TC(Inst const inst)  { return g_LDM_1_DB_w_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_DB_TC(Inst const inst)   { return g_LDM_23_DB_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_DB_w_TC(Inst const inst) { return g_LDM_23_DB_w_TC.Process(inst); }
    
  ExecResultEnum exec_LDM_1_IA_TC(Inst const inst)    { return g_LDM_1_IA_TC.Process(inst); }
  ExecResultEnum exec_LDM_1_IA_w_TC(Inst const inst)  { return g_LDM_1_IA_w_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_IA_TC(Inst const inst)   { return g_LDM_23_IA_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_IA_w_TC(Inst const inst) { return g_LDM_23_IA_w_TC.Process(inst); }
  
  ExecResultEnum exec_LDM_1_IB_TC(Inst const inst)    { return g_LDM_1_IB_TC.Process(inst); }
  ExecResultEnum exec_LDM_1_IB_w_TC(Inst const inst)  { return g_LDM_1_IB_w_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_IB_TC(Inst const inst)   { return g_LDM_23_IB_TC.Process(inst); }
  ExecResultEnum exec_LDM_23_IB_w_TC(Inst const inst) { return g_LDM_23_IB_w_TC.Process(inst); }
  
  // SWP - threaded code version
  
  ExecResultEnum exec_SWP_TC(Inst const inst)  { return g_SWP_TC.Process(inst); }
  ExecResultEnum exec_SWPB_TC(Inst const inst) { return g_SWPB_TC.Process(inst); }
  
  // STR - threaded code version
  
  ExecResultEnum exec_STR_imm_post_down_TC(Inst const inst)   { return g_STR_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_STR_imm_post_up_TC(Inst const inst)     { return g_STR_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_STR_imm_offset_down_TC(Inst const inst) { return g_STR_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_STR_imm_offset_up_TC(Inst const inst)   { return g_STR_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_STR_imm_pre_down_TC(Inst const inst)    { return g_STR_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_STR_imm_pre_up_TC(Inst const inst)      { return g_STR_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_STR_reg_post_down_TC(Inst const inst)   { return g_STR_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_STR_reg_post_up_TC(Inst const inst)     { return g_STR_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_STR_reg_offset_down_TC(Inst const inst) { return g_STR_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_STR_reg_offset_up_TC(Inst const inst)   { return g_STR_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_STR_reg_pre_down_TC(Inst const inst)    { return g_STR_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_STR_reg_pre_up_TC(Inst const inst)      { return g_STR_reg_pre_up_TC.Process(inst); }
  
  // STRT - threaded code version
  
  ExecResultEnum exec_STRT_imm_post_down_TC(Inst const inst) { return g_STRT_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRT_imm_post_up_TC(Inst const inst)   { return g_STRT_imm_post_up_TC.Process(inst); }
  
  ExecResultEnum exec_STRT_reg_post_down_TC(Inst const inst) { return g_STRT_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRT_reg_post_up_TC(Inst const inst)   { return g_STRT_reg_post_up_TC.Process(inst); }
  
  // STRB - threaded code version
  
  ExecResultEnum exec_STRB_imm_post_down_TC(Inst const inst)   { return g_STRB_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRB_imm_post_up_TC(Inst const inst)     { return g_STRB_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_STRB_imm_offset_down_TC(Inst const inst) { return g_STRB_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_STRB_imm_offset_up_TC(Inst const inst)   { return g_STRB_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_STRB_imm_pre_down_TC(Inst const inst)    { return g_STRB_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_STRB_imm_pre_up_TC(Inst const inst)      { return g_STRB_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_STRB_reg_post_down_TC(Inst const inst)   { return g_STRB_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRB_reg_post_up_TC(Inst const inst)     { return g_STRB_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_STRB_reg_offset_down_TC(Inst const inst) { return g_STRB_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_STRB_reg_offset_up_TC(Inst const inst)   { return g_STRB_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_STRB_reg_pre_down_TC(Inst const inst)    { return g_STRB_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_STRB_reg_pre_up_TC(Inst const inst)      { return g_STRB_reg_pre_up_TC.Process(inst); }
  
  // STRBT - threaded code version
  
  ExecResultEnum exec_STRBT_imm_post_down_TC(Inst const inst) { return g_STRBT_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRBT_imm_post_up_TC(Inst const inst)   { return g_STRBT_imm_post_up_TC.Process(inst); }
  
  ExecResultEnum exec_STRBT_reg_post_down_TC(Inst const inst) { return g_STRBT_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRBT_reg_post_up_TC(Inst const inst)   { return g_STRBT_reg_post_up_TC.Process(inst); }
  
  // LDR - threaded code version
  
  ExecResultEnum exec_LDR_imm_post_down_TC(Inst const inst)   { return g_LDR_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDR_imm_post_up_TC(Inst const inst)     { return g_LDR_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDR_imm_offset_down_TC(Inst const inst) { return g_LDR_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDR_imm_offset_up_TC(Inst const inst)   { return g_LDR_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDR_imm_pre_down_TC(Inst const inst)    { return g_LDR_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDR_imm_pre_up_TC(Inst const inst)      { return g_LDR_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_LDR_reg_post_down_TC(Inst const inst)   { return g_LDR_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDR_reg_post_up_TC(Inst const inst)     { return g_LDR_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDR_reg_offset_down_TC(Inst const inst) { return g_LDR_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDR_reg_offset_up_TC(Inst const inst)   { return g_LDR_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDR_reg_pre_down_TC(Inst const inst)    { return g_LDR_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDR_reg_pre_up_TC(Inst const inst)      { return g_LDR_reg_pre_up_TC.Process(inst); }
  
  // LDRT - threaded code version
  
  ExecResultEnum exec_LDRT_imm_post_down_TC(Inst const inst) { return g_LDRT_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRT_imm_post_up_TC(Inst const inst)   { return g_LDRT_imm_post_up_TC.Process(inst); }
  
  ExecResultEnum exec_LDRT_reg_post_down_TC(Inst const inst) { return g_LDRT_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRT_reg_post_up_TC(Inst const inst)   { return g_LDRT_reg_post_up_TC.Process(inst); }
  
  // LDRB - threaded code version
  
  ExecResultEnum exec_LDRB_imm_post_down_TC(Inst const inst)   { return g_LDRB_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRB_imm_post_up_TC(Inst const inst)     { return g_LDRB_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRB_imm_offset_down_TC(Inst const inst) { return g_LDRB_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRB_imm_offset_up_TC(Inst const inst)   { return g_LDRB_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRB_imm_pre_down_TC(Inst const inst)    { return g_LDRB_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRB_imm_pre_up_TC(Inst const inst)      { return g_LDRB_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_LDRB_reg_post_down_TC(Inst const inst)   { return g_LDRB_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRB_reg_post_up_TC(Inst const inst)     { return g_LDRB_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRB_reg_offset_down_TC(Inst const inst) { return g_LDRB_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRB_reg_offset_up_TC(Inst const inst)   { return g_LDRB_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRB_reg_pre_down_TC(Inst const inst)    { return g_LDRB_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRB_reg_pre_up_TC(Inst const inst)      { return g_LDRB_reg_pre_up_TC.Process(inst); }
  
  // LDRBT - threaded code version
  
  ExecResultEnum exec_LDRBT_imm_post_down_TC(Inst const inst) { return g_LDRBT_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRBT_imm_post_up_TC(Inst const inst)   { return g_LDRBT_imm_post_up_TC.Process(inst); }
  
  ExecResultEnum exec_LDRBT_reg_post_down_TC(Inst const inst) { return g_LDRBT_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRBT_reg_post_up_TC(Inst const inst)   { return g_LDRBT_reg_post_up_TC.Process(inst); }
  
  // STRH - threaded code version
  
  ExecResultEnum exec_STRH_imm_post_down_TC(Inst const inst)   { return g_STRH_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRH_imm_post_up_TC(Inst const inst)     { return g_STRH_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_STRH_imm_offset_down_TC(Inst const inst) { return g_STRH_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_STRH_imm_offset_up_TC(Inst const inst)   { return g_STRH_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_STRH_imm_pre_down_TC(Inst const inst)    { return g_STRH_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_STRH_imm_pre_up_TC(Inst const inst)      { return g_STRH_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_STRH_reg_post_down_TC(Inst const inst)   { return g_STRH_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_STRH_reg_post_up_TC(Inst const inst)     { return g_STRH_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_STRH_reg_offset_down_TC(Inst const inst) { return g_STRH_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_STRH_reg_offset_up_TC(Inst const inst)   { return g_STRH_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_STRH_reg_pre_down_TC(Inst const inst)    { return g_STRH_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_STRH_reg_pre_up_TC(Inst const inst)      { return g_STRH_reg_pre_up_TC.Process(inst); }
  
  // LDRH - threaded code version
  
  ExecResultEnum exec_LDRH_imm_post_down_TC(Inst const inst)   { return g_LDRH_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRH_imm_post_up_TC(Inst const inst)     { return g_LDRH_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRH_imm_offset_down_TC(Inst const inst) { return g_LDRH_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRH_imm_offset_up_TC(Inst const inst)   { return g_LDRH_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRH_imm_pre_down_TC(Inst const inst)    { return g_LDRH_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRH_imm_pre_up_TC(Inst const inst)      { return g_LDRH_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_LDRH_reg_post_down_TC(Inst const inst)   { return g_LDRH_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRH_reg_post_up_TC(Inst const inst)     { return g_LDRH_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRH_reg_offset_down_TC(Inst const inst) { return g_LDRH_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRH_reg_offset_up_TC(Inst const inst)   { return g_LDRH_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRH_reg_pre_down_TC(Inst const inst)    { return g_LDRH_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRH_reg_pre_up_TC(Inst const inst)      { return g_LDRH_reg_pre_up_TC.Process(inst); }
  
  // LDRSB - threaded code version
  
  ExecResultEnum exec_LDRSB_imm_post_down_TC(Inst const inst)   { return g_LDRSB_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_imm_post_up_TC(Inst const inst)     { return g_LDRSB_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_imm_offset_down_TC(Inst const inst) { return g_LDRSB_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_imm_offset_up_TC(Inst const inst)   { return g_LDRSB_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_imm_pre_down_TC(Inst const inst)    { return g_LDRSB_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_imm_pre_up_TC(Inst const inst)      { return g_LDRSB_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_LDRSB_reg_post_down_TC(Inst const inst)   { return g_LDRSB_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_reg_post_up_TC(Inst const inst)     { return g_LDRSB_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_reg_offset_down_TC(Inst const inst) { return g_LDRSB_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_reg_offset_up_TC(Inst const inst)   { return g_LDRSB_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_reg_pre_down_TC(Inst const inst)    { return g_LDRSB_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSB_reg_pre_up_TC(Inst const inst)      { return g_LDRSB_reg_pre_up_TC.Process(inst); }
  
  // LDRSH - threaded code version
  
  ExecResultEnum exec_LDRSH_imm_post_down_TC(Inst const inst)   { return g_LDRSH_imm_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_imm_post_up_TC(Inst const inst)     { return g_LDRSH_imm_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_imm_offset_down_TC(Inst const inst) { return g_LDRSH_imm_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_imm_offset_up_TC(Inst const inst)   { return g_LDRSH_imm_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_imm_pre_down_TC(Inst const inst)    { return g_LDRSH_imm_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_imm_pre_up_TC(Inst const inst)      { return g_LDRSH_imm_pre_up_TC.Process(inst); }
  
  ExecResultEnum exec_LDRSH_reg_post_down_TC(Inst const inst)   { return g_LDRSH_reg_post_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_reg_post_up_TC(Inst const inst)     { return g_LDRSH_reg_post_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_reg_offset_down_TC(Inst const inst) { return g_LDRSH_reg_offset_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_reg_offset_up_TC(Inst const inst)   { return g_LDRSH_reg_offset_up_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_reg_pre_down_TC(Inst const inst)    { return g_LDRSH_reg_pre_down_TC.Process(inst); }
  ExecResultEnum exec_LDRSH_reg_pre_up_TC(Inst const inst)      { return g_LDRSH_reg_pre_up_TC.Process(inst); }
  
  // DPI - threaded code version
  
  ExecResultEnum exec_AND_regimm_TC(Inst const inst)  { return g_AND_regimm_TC.Process(inst); }
  ExecResultEnum exec_AND_regreg_TC(Inst const inst)  { return g_AND_regreg_TC.Process(inst); }
  ExecResultEnum exec_AND_imm_TC(Inst const inst)     { return g_AND_imm_TC.Process(inst); }
  ExecResultEnum exec_ANDS_regimm_TC(Inst const inst) { return g_ANDS_regimm_TC.Process(inst); }
  ExecResultEnum exec_ANDS_regreg_TC(Inst const inst) { return g_ANDS_regreg_TC.Process(inst); }
  ExecResultEnum exec_ANDS_imm_TC(Inst const inst)    { return g_ANDS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_EOR_regimm_TC(Inst const inst)  { return g_EOR_regimm_TC.Process(inst); }
  ExecResultEnum exec_EOR_regreg_TC(Inst const inst)  { return g_EOR_regreg_TC.Process(inst); }
  ExecResultEnum exec_EOR_imm_TC(Inst const inst)     { return g_EOR_imm_TC.Process(inst); }
  ExecResultEnum exec_EORS_regimm_TC(Inst const inst) { return g_EORS_regimm_TC.Process(inst); }
  ExecResultEnum exec_EORS_regreg_TC(Inst const inst) { return g_EORS_regreg_TC.Process(inst); }
  ExecResultEnum exec_EORS_imm_TC(Inst const inst)    { return g_EORS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_SUB_regimm_TC(Inst const inst)  { return g_SUB_regimm_TC.Process(inst); }
  ExecResultEnum exec_SUB_regreg_TC(Inst const inst)  { return g_SUB_regreg_TC.Process(inst); }
  ExecResultEnum exec_SUB_imm_TC(Inst const inst)     { return g_SUB_imm_TC.Process(inst); }
  ExecResultEnum exec_SUBS_regimm_TC(Inst const inst) { return g_SUBS_regimm_TC.Process(inst); }
  ExecResultEnum exec_SUBS_regreg_TC(Inst const inst) { return g_SUBS_regreg_TC.Process(inst); }
  ExecResultEnum exec_SUBS_imm_TC(Inst const inst)    { return g_SUBS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_RSB_regimm_TC(Inst const inst)  { return g_RSB_regimm_TC.Process(inst); }
  ExecResultEnum exec_RSB_regreg_TC(Inst const inst)  { return g_RSB_regreg_TC.Process(inst); }
  ExecResultEnum exec_RSB_imm_TC(Inst const inst)     { return g_RSB_imm_TC.Process(inst); }
  ExecResultEnum exec_RSBS_regimm_TC(Inst const inst) { return g_RSBS_regimm_TC.Process(inst); }
  ExecResultEnum exec_RSBS_regreg_TC(Inst const inst) { return g_RSBS_regreg_TC.Process(inst); }
  ExecResultEnum exec_RSBS_imm_TC(Inst const inst)    { return g_RSBS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_ADD_regimm_TC(Inst const inst)  { return g_ADD_regimm_TC.Process(inst); }
  ExecResultEnum exec_ADD_regreg_TC(Inst const inst)  { return g_ADD_regreg_TC.Process(inst); }
  ExecResultEnum exec_ADD_imm_TC(Inst const inst)     { return g_ADD_imm_TC.Process(inst); }
  ExecResultEnum exec_ADDS_regimm_TC(Inst const inst) { return g_ADDS_regimm_TC.Process(inst); }
  ExecResultEnum exec_ADDS_regreg_TC(Inst const inst) { return g_ADDS_regreg_TC.Process(inst); }
  ExecResultEnum exec_ADDS_imm_TC(Inst const inst)    { return g_ADDS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_ADC_regimm_TC(Inst const inst)  { return g_ADC_regimm_TC.Process(inst); }
  ExecResultEnum exec_ADC_regreg_TC(Inst const inst)  { return g_ADC_regreg_TC.Process(inst); }
  ExecResultEnum exec_ADC_imm_TC(Inst const inst)     { return g_ADC_imm_TC.Process(inst); }
  ExecResultEnum exec_ADCS_regimm_TC(Inst const inst) { return g_ADCS_regimm_TC.Process(inst); }
  ExecResultEnum exec_ADCS_regreg_TC(Inst const inst) { return g_ADCS_regreg_TC.Process(inst); }
  ExecResultEnum exec_ADCS_imm_TC(Inst const inst)    { return g_ADCS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_SBC_regimm_TC(Inst const inst)  { return g_SBC_regimm_TC.Process(inst); }
  ExecResultEnum exec_SBC_regreg_TC(Inst const inst)  { return g_SBC_regreg_TC.Process(inst); }
  ExecResultEnum exec_SBC_imm_TC(Inst const inst)     { return g_SBC_imm_TC.Process(inst); }
  ExecResultEnum exec_SBCS_regimm_TC(Inst const inst) { return g_SBCS_regimm_TC.Process(inst); }
  ExecResultEnum exec_SBCS_regreg_TC(Inst const inst) { return g_SBCS_regreg_TC.Process(inst); }
  ExecResultEnum exec_SBCS_imm_TC(Inst const inst)    { return g_SBCS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_RSC_regimm_TC(Inst const inst)  { return g_RSC_regimm_TC.Process(inst); }
  ExecResultEnum exec_RSC_regreg_TC(Inst const inst)  { return g_RSC_regreg_TC.Process(inst); }
  ExecResultEnum exec_RSC_imm_TC(Inst const inst)     { return g_RSC_imm_TC.Process(inst); }
  ExecResultEnum exec_RSCS_regimm_TC(Inst const inst) { return g_RSCS_regimm_TC.Process(inst); }
  ExecResultEnum exec_RSCS_regreg_TC(Inst const inst) { return g_RSCS_regreg_TC.Process(inst); }
  ExecResultEnum exec_RSCS_imm_TC(Inst const inst)    { return g_RSCS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_ORR_regimm_TC(Inst const inst)  { return g_ORR_regimm_TC.Process(inst); }
  ExecResultEnum exec_ORR_regreg_TC(Inst const inst)  { return g_ORR_regreg_TC.Process(inst); }
  ExecResultEnum exec_ORR_imm_TC(Inst const inst)     { return g_ORR_imm_TC.Process(inst); }
  ExecResultEnum exec_ORRS_regimm_TC(Inst const inst) { return g_ORRS_regimm_TC.Process(inst); }
  ExecResultEnum exec_ORRS_regreg_TC(Inst const inst) { return g_ORRS_regreg_TC.Process(inst); }
  ExecResultEnum exec_ORRS_imm_TC(Inst const inst)    { return g_ORRS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_MOV_regimm_TC(Inst const inst)  { return g_MOV_regimm_TC.Process(inst); }
  ExecResultEnum exec_MOV_regreg_TC(Inst const inst)  { return g_MOV_regreg_TC.Process(inst); }
  ExecResultEnum exec_MOV_imm_TC(Inst const inst)     { return g_MOV_imm_TC.Process(inst); }
  ExecResultEnum exec_MOVS_regimm_TC(Inst const inst) { return g_MOVS_regimm_TC.Process(inst); }
  ExecResultEnum exec_MOVS_regreg_TC(Inst const inst) { return g_MOVS_regreg_TC.Process(inst); }
  ExecResultEnum exec_MOVS_imm_TC(Inst const inst)    { return g_MOVS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_BIC_regimm_TC(Inst const inst)  { return g_BIC_regimm_TC.Process(inst); }
  ExecResultEnum exec_BIC_regreg_TC(Inst const inst)  { return g_BIC_regreg_TC.Process(inst); }
  ExecResultEnum exec_BIC_imm_TC(Inst const inst)     { return g_BIC_imm_TC.Process(inst); }
  ExecResultEnum exec_BICS_regimm_TC(Inst const inst) { return g_BICS_regimm_TC.Process(inst); }
  ExecResultEnum exec_BICS_regreg_TC(Inst const inst) { return g_BICS_regreg_TC.Process(inst); }
  ExecResultEnum exec_BICS_imm_TC(Inst const inst)    { return g_BICS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_MVN_regimm_TC(Inst const inst)  { return g_MVN_regimm_TC.Process(inst); }
  ExecResultEnum exec_MVN_regreg_TC(Inst const inst)  { return g_MVN_regreg_TC.Process(inst); }
  ExecResultEnum exec_MVN_imm_TC(Inst const inst)     { return g_MVN_imm_TC.Process(inst); }
  ExecResultEnum exec_MVNS_regimm_TC(Inst const inst) { return g_MVNS_regimm_TC.Process(inst); }
  ExecResultEnum exec_MVNS_regreg_TC(Inst const inst) { return g_MVNS_regreg_TC.Process(inst); }
  ExecResultEnum exec_MVNS_imm_TC(Inst const inst)    { return g_MVNS_imm_TC.Process(inst); }
  
  ExecResultEnum exec_TST_regimm_TC(Inst const inst)  { return g_TST_regimm_TC.Process(inst); }
  ExecResultEnum exec_TST_regreg_TC(Inst const inst)  { return g_TST_regreg_TC.Process(inst); }
  ExecResultEnum exec_TST_imm_TC(Inst const inst)     { return g_TST_imm_TC.Process(inst); }
  
  ExecResultEnum exec_TEQ_regimm_TC(Inst const inst)  { return g_TEQ_regimm_TC.Process(inst); }
  ExecResultEnum exec_TEQ_regreg_TC(Inst const inst)  { return g_TEQ_regreg_TC.Process(inst); }
  ExecResultEnum exec_TEQ_imm_TC(Inst const inst)     { return g_TEQ_imm_TC.Process(inst); }
  
  ExecResultEnum exec_CMP_regimm_TC(Inst const inst)  { return g_CMP_regimm_TC.Process(inst); }
  ExecResultEnum exec_CMP_regreg_TC(Inst const inst)  { return g_CMP_regreg_TC.Process(inst); }
  ExecResultEnum exec_CMP_imm_TC(Inst const inst)     { return g_CMP_imm_TC.Process(inst); }
  
  ExecResultEnum exec_CMN_regimm_TC(Inst const inst)  { return g_CMN_regimm_TC.Process(inst); }
  ExecResultEnum exec_CMN_regreg_TC(Inst const inst)  { return g_CMN_regreg_TC.Process(inst); }
  ExecResultEnum exec_CMN_imm_TC(Inst const inst)     { return g_CMN_imm_TC.Process(inst); }
  
  // Branch - threaded code version
  
  ExecResultEnum exec_BLX_1_TC(Inst const inst) { return g_BLX_1_TC.Process(inst); }
  ExecResultEnum exec_BLX_2_TC(Inst const inst) { return g_BLX_2_TC.Process(inst); }
  ExecResultEnum exec_BX_TC(Inst const inst)    { return g_BX_TC.Process(inst); }
  ExecResultEnum exec_B_TC(Inst const inst)     { return g_B_TC.Process(inst); }
  ExecResultEnum exec_BL_TC(Inst const inst)    { return g_BL_TC.Process(inst); }
  
  // Multi - threaded code version
  
  ExecResultEnum exec_MUL_TC(Inst const inst)    { return g_MUL.Process(inst); }
  ExecResultEnum exec_MULS_TC(Inst const inst)   { return g_MULS.Process(inst); }
  ExecResultEnum exec_MLA_TC(Inst const inst)    { return g_MLA.Process(inst); }
  ExecResultEnum exec_MLAS_TC(Inst const inst)   { return g_MLAS.Process(inst); }
  ExecResultEnum exec_UMULL_TC(Inst const inst)  { return g_UMULL.Process(inst); }
  ExecResultEnum exec_UMULLS_TC(Inst const inst) { return g_UMULLS.Process(inst); }
  ExecResultEnum exec_UMLAL_TC(Inst const inst)  { return g_UMLAL.Process(inst); }
  ExecResultEnum exec_UMLALS_TC(Inst const inst) { return g_UMLALS.Process(inst); }
  ExecResultEnum exec_SMULL_TC(Inst const inst)  { return g_SMULL.Process(inst); }
  ExecResultEnum exec_SMULLS_TC(Inst const inst) { return g_SMULLS.Process(inst); }
  ExecResultEnum exec_SMLAL_TC(Inst const inst)  { return g_SMLAL.Process(inst); }
  ExecResultEnum exec_SMLALS_TC(Inst const inst) { return g_SMLALS.Process(inst); }
  
  // MRS - threaded code version
  
  ExecResultEnum exec_MRS_cpsr_TC(Inst const inst) { return g_MRS_cpsr.Process(inst); }
  ExecResultEnum exec_MRS_spsr_TC(Inst const inst) { return g_MRS_spsr.Process(inst); }
  
  // MSR - threaded code version
  
  ExecResultEnum exec_MSR_reg_cpsr_TC(Inst const inst) { return g_MSR_reg_cpsr.Process(inst); }
  ExecResultEnum exec_MSR_reg_spsr_TC(Inst const inst) { return g_MSR_reg_spsr.Process(inst); }
  ExecResultEnum exec_MSR_imm_cpsr_TC(Inst const inst) { return g_MSR_imm_cpsr.Process(inst); }
  ExecResultEnum exec_MSR_imm_spsr_TC(Inst const inst) { return g_MSR_imm_spsr.Process(inst); }
  
  // CDT - threaded code version
  
  ExecResultEnum exec_CDT_TC(Inst const inst) { return gp_core->exec_CDT(inst); }
  
  // CDP - threaded code version
  
  ExecResultEnum exec_CDP_TC(Inst const inst) { return gp_core->exec_CDP(inst); }
  
  // Undefined instruction - threaded code version
  
  ExecResultEnum exec_UND_TC(Inst const inst) { return gp_core->exec_UND(inst); }
  
  // SWI - threaded code version
  
  ExecResultEnum exec_SWI_TC(Inst const inst) { return gp_core->exec_SWI(inst); }
#endif
}
