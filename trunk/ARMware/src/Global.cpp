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
#include "Global.hpp"
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
  // Global object
  
  // :NOTE:Wei 2004-Aug-24:
  //
  // Global objects in C++ are normally bad.
  // That is because global objects' constructors & destructors are executed
  // before & after main() function.
  //
  // Thus, if an exception has occured in global objects' constructors or destructors,
  // that exception will not be catched, and the program will be terminated.
  //
  // However, the constructors of the following instruction objects don't throw
  // any exceptions. Thus they are safe to be global objects.
  
  // STM
  
  InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DA;
  InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DA_w;
  InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_DA;
    
  InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DB;
  InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_DB_w;
  InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_DB;
    
  InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IA;
  InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IA_w;
  InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_IA;
    
  InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IB;
  InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL> g_STM_1_IB_w;
  InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL> g_STM_2_IB;
  
  // LDM
    
  InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DA;
  InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DA_w;
  InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DA;
  InstLSM<DOWN,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DA_w;
    
  InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DB;
  InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_DB_w;
  InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DB;
  InstLSM<DOWN, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_DB_w;
    
  InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IA;
  InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IA_w;
  InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IA;
  InstLSM<  UP,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IA_w;
    
  InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IB;
  InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_1_IB_w;
  InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IB;
  InstLSM<  UP, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL> g_LDM_23_IB_w;
    
  // SWP
    
  InstSWP<WORD, USAGE_NORMAL> g_SWP;
  InstSWP<BYTE, USAGE_NORMAL> g_SWPB;
    
  // STR
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_post_up;
  InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_offset_down;
  InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_offset_up;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_pre_down;
  InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_imm_pre_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_post_up;
  InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_offset_down;
  InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_offset_up;
  InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_pre_down;
  InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL> g_STR_reg_pre_up;
    
  // STRT
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_imm_post_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_NORMAL> g_STRT_reg_post_up;
    
  // STRB
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_post_up;
  InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_offset_down;
  InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_offset_up;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_pre_down;
  InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_imm_pre_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_post_up;
  InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_offset_down;
  InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_offset_up;
  InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_pre_down;
  InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL> g_STRB_reg_pre_up;
    
  // STRBT
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_imm_post_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_NORMAL> g_STRBT_reg_post_up;
    
  // LDR
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_post_up;
  InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_offset_down;
  InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_offset_up;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_pre_down;
  InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_imm_pre_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_post_up;
  InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_offset_down;
  InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_offset_up;
  InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_pre_down;
  InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDR_reg_pre_up;
    
  // LDRT
    
  InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_imm_post_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRT_reg_post_up;
    
  // LDRB
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_post_up;
  InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_offset_down;
  InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_offset_up;
  InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_pre_down;
  InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_imm_pre_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_post_up;
  InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_offset_down;
  InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_offset_up;
  InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_pre_down;
  InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL> g_LDRB_reg_pre_up;
    
  // LDRBT
    
  InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_imm_post_down;
  InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_imm_post_up;
    
  InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_reg_post_down;
  InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL> g_LDRBT_reg_post_up;
    
  // STRH
    
  InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_post_down;
  InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_post_up;
  InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_offset_down;
  InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_offset_up;
  InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_pre_down;
  InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_reg_pre_up;
    
  InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_post_down;
  InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_post_up;
  InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_offset_down;
  InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_offset_up;
  InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_pre_down;
  InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL> g_STRH_imm_pre_up;
    
  // LDRH
    
  InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_post_down;
  InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_post_up;
  InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_offset_down;
  InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_offset_up;
  InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_pre_down;
  InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_reg_pre_up;
    
  InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_post_down;
  InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_post_up;
  InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_offset_down;
  InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_offset_up;
  InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_pre_down;
  InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRH_imm_pre_up;
    
  // LDRSB
    
  InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_post_down;
  InstLSH<RegLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_post_up;
  InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_offset_down;
  InstLSH<RegLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_offset_up;
  InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_pre_down;
  InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_reg_pre_up;
    
  InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_post_down;
  InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_post_up;
  InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_offset_down;
  InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_offset_up;
  InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_pre_down;
  InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL> g_LDRSB_imm_pre_up;
    
  // LDRSH
    
  InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_post_down;
  InstLSH<RegLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_post_up;
  InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_offset_down;
  InstLSH<RegLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_offset_up;
  InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_pre_down;
  InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_reg_pre_up;
    
  InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_post_down;
  InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_post_up;
  InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_offset_down;
  InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_offset_up;
  InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_pre_down;
  InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL> g_LDRSH_imm_pre_up;
    
  // DPI
    
  InstDPI<AND, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_AND_regimm;
  InstDPI<AND, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_AND_regreg;
  InstDPI<AND,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_AND_imm;
  InstDPI<AND, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ANDS_regimm;
  InstDPI<AND, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ANDS_regreg;
  InstDPI<AND,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ANDS_imm;
    
  InstDPI<EOR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_EOR_regimm;
  InstDPI<EOR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_EOR_regreg;
  InstDPI<EOR,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_EOR_imm;
  InstDPI<EOR, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_EORS_regimm;
  InstDPI<EOR, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_EORS_regreg;
  InstDPI<EOR,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_EORS_imm;
    
  InstDPI<SUB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SUB_regimm;
  InstDPI<SUB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SUB_regreg;
  InstDPI<SUB,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SUB_imm;
  InstDPI<SUB, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SUBS_regimm;
  InstDPI<SUB, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SUBS_regreg;
  InstDPI<SUB,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SUBS_imm;
    
  InstDPI<RSB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSB_regimm;
  InstDPI<RSB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSB_regreg;
  InstDPI<RSB,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSB_imm;
  InstDPI<RSB, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSBS_regimm;
  InstDPI<RSB, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSBS_regreg;
  InstDPI<RSB,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSBS_imm;
    
  InstDPI<ADD, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADD_regimm;
  InstDPI<ADD, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADD_regreg;
  InstDPI<ADD,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADD_imm;
  InstDPI<ADD, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADDS_regimm;
  InstDPI<ADD, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADDS_regreg;
  InstDPI<ADD,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADDS_imm;
    
  InstDPI<ADC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADC_regimm;
  InstDPI<ADC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADC_regreg;
  InstDPI<ADC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ADC_imm;
  InstDPI<ADC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADCS_regimm;
  InstDPI<ADC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADCS_regreg;
  InstDPI<ADC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ADCS_imm;
    
  InstDPI<SBC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SBC_regimm;
  InstDPI<SBC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SBC_regreg;
  InstDPI<SBC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_SBC_imm;
  InstDPI<SBC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SBCS_regimm;
  InstDPI<SBC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SBCS_regreg;
  InstDPI<SBC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_SBCS_imm;
    
  InstDPI<RSC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSC_regimm;
  InstDPI<RSC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSC_regreg;
  InstDPI<RSC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_RSC_imm;
  InstDPI<RSC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSCS_regimm;
  InstDPI<RSC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSCS_regreg;
  InstDPI<RSC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_RSCS_imm;
    
  InstDPI<ORR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ORR_regimm;
  InstDPI<ORR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ORR_regreg;
  InstDPI<ORR,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_ORR_imm;
  InstDPI<ORR, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ORRS_regimm;
  InstDPI<ORR, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ORRS_regreg;
  InstDPI<ORR,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_ORRS_imm;
    
  InstDPI<MOV, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MOV_regimm;
  InstDPI<MOV, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MOV_regreg;
  InstDPI<MOV,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MOV_imm;
  InstDPI<MOV, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MOVS_regimm;
  InstDPI<MOV, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MOVS_regreg;
  InstDPI<MOV,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MOVS_imm;
    
  InstDPI<BIC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_BIC_regimm;
  InstDPI<BIC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_BIC_regreg;
  InstDPI<BIC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_BIC_imm;
  InstDPI<BIC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_BICS_regimm;
  InstDPI<BIC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_BICS_regreg;
  InstDPI<BIC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_BICS_imm;
    
  InstDPI<MVN, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MVN_regimm;
  InstDPI<MVN, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MVN_regreg;
  InstDPI<MVN,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL> g_MVN_imm;
  InstDPI<MVN, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MVNS_regimm;
  InstDPI<MVN, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MVNS_regreg;
  InstDPI<MVN,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_MVNS_imm;
    
  InstDPI<TST, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TST_regimm;
  InstDPI<TST, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TST_regreg;
  InstDPI<TST,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TST_imm;
    
  InstDPI<TEQ, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TEQ_regimm;
  InstDPI<TEQ, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TEQ_regreg;
  InstDPI<TEQ,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_TEQ_imm;
    
  InstDPI<CMP, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMP_regimm;
  InstDPI<CMP, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMP_regreg;
  InstDPI<CMP,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMP_imm;
    
  InstDPI<CMN, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMN_regimm;
  InstDPI<CMN, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMN_regreg;
  InstDPI<CMN,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL> g_CMN_imm;
    
  // Branch
    
  InstBranch<ImmBranch,    LINK,    EXCHANGE, USAGE_NORMAL> g_BLX_1;
  InstBranch<RegBranch,    LINK,    EXCHANGE, USAGE_NORMAL> g_BLX_2;
  InstBranch<RegBranch, NO_LINK,    EXCHANGE, USAGE_NORMAL> g_BX;
  InstBranch<ImmBranch, NO_LINK, NO_EXCHANGE, USAGE_NORMAL> g_B;
  InstBranch<ImmBranch,    LINK, NO_EXCHANGE, USAGE_NORMAL> g_BL;
    
  // Multi
    
  InstMulti<NORMAL,   SIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS> g_MUL;
  InstMulti<NORMAL,   SIGNED, NO_ACCUMULATE,    UPDATE_STATUS> g_MULS;
  InstMulti<NORMAL,   SIGNED,    ACCUMULATE, NO_UPDATE_STATUS> g_MLA;
  InstMulti<NORMAL,   SIGNED,    ACCUMULATE,    UPDATE_STATUS> g_MLAS;
  InstMulti<  LONG, UNSIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS> g_UMULL;
  InstMulti<  LONG, UNSIGNED, NO_ACCUMULATE,    UPDATE_STATUS> g_UMULLS;
  InstMulti<  LONG, UNSIGNED,    ACCUMULATE, NO_UPDATE_STATUS> g_UMLAL;
  InstMulti<  LONG, UNSIGNED,    ACCUMULATE,    UPDATE_STATUS> g_UMLALS;
  InstMulti<  LONG,   SIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS> g_SMULL;
  InstMulti<  LONG,   SIGNED, NO_ACCUMULATE,    UPDATE_STATUS> g_SMULLS;
  InstMulti<  LONG,   SIGNED,    ACCUMULATE, NO_UPDATE_STATUS> g_SMLAL;
  InstMulti<  LONG,   SIGNED,    ACCUMULATE,    UPDATE_STATUS> g_SMLALS;
    
  // MRS
    
  InstMRS<        USE_CPSR> g_MRS_cpsr;
  InstMRS<USE_CURRENT_SPSR> g_MRS_spsr;
    
  // MSR
    
  InstMSR<RegMSR,         USE_CPSR> g_MSR_reg_cpsr;
  InstMSR<RegMSR, USE_CURRENT_SPSR> g_MSR_reg_spsr;
  InstMSR<ImmMSR,         USE_CPSR> g_MSR_imm_cpsr;
  InstMSR<ImmMSR, USE_CURRENT_SPSR> g_MSR_imm_spsr;
}
