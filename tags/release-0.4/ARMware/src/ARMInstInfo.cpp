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

#include "ARMInstInfo.hpp"

#include "Global.hpp"
#include "Global_TC.hpp"
#include "Global_DT.hpp"
#include "Global_PRINT.hpp"

#include "Compiler/MIR.hpp"

#include "Global_TC.tcc"
#include "Global_DT.tcc"
#include "Global_PRINT.tcc"

namespace ARMware
{
  // STM
  
  ARMInstInfo::ArgList g_STM_1_DA_arglist   (exec_STM_1_DA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_DA_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_DA_PRINT
#endif  
                                             );
  ARMInstInfo::ArgList g_STM_1_DA_w_arglist (exec_STM_1_DA_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_DA_w_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_DA_w_PRINT
#endif
                                             );
  ARMInstInfo::ArgList g_STM_2_DA_arglist   (exec_STM_2_DA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_2_DA_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_2_DA_PRINT
#endif  
                                             );
  
  ARMInstInfo::ArgList g_STM_1_DB_arglist   (exec_STM_1_DB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_DB_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_DB_PRINT
#endif  
                                             );
  ARMInstInfo::ArgList g_STM_1_DB_w_arglist (exec_STM_1_DB_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_DB_w_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_DB_w_PRINT
#endif
                                             );
  ARMInstInfo::ArgList g_STM_2_DB_arglist   (exec_STM_2_DB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_2_DB_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_2_DB_PRINT
#endif  
                                             );
  
  ARMInstInfo::ArgList g_STM_1_IA_arglist   (exec_STM_1_IA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_IA_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_IA_PRINT
#endif  
                                             );
  ARMInstInfo::ArgList g_STM_1_IA_w_arglist (exec_STM_1_IA_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_IA_w_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_IA_w_PRINT
#endif
                                             );
  ARMInstInfo::ArgList g_STM_2_IA_arglist   (exec_STM_2_IA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_2_IA_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_2_IA_PRINT
#endif  
                                             );
  
  ARMInstInfo::ArgList g_STM_1_IB_arglist   (exec_STM_1_IB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_IB_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_IB_PRINT
#endif  
                                             );
  ARMInstInfo::ArgList g_STM_1_IB_w_arglist (exec_STM_1_IB_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_1_IB_w_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_1_IB_w_PRINT
#endif
                                             );
  ARMInstInfo::ArgList g_STM_2_IB_arglist   (exec_STM_2_IB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                             , exec_STM_2_IB_DT
#endif
#if PRINT_CHUNK_CODE
                                             , exec_STM_2_IB_PRINT
#endif  
                                             );
  
  // LDM
  
  ARMInstInfo::ArgList g_LDM_1_DA_arglist    (exec_LDM_1_DA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_DA_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_DA_PRINT
#endif   
                                              );
  ARMInstInfo::ArgList g_LDM_1_DA_w_arglist  (exec_LDM_1_DA_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_DA_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_DA_w_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_LDM_23_DA_arglist   (exec_LDM_23_DA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_DA_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_DA_PRINT
#endif  
                                              );
  ARMInstInfo::ArgList g_LDM_23_DA_w_arglist (exec_LDM_23_DA_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_DA_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_DA_w_PRINT
#endif
                                              );
  
  ARMInstInfo::ArgList g_LDM_1_DB_arglist    (exec_LDM_1_DB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_DB_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_DB_PRINT
#endif   
                                              );
  ARMInstInfo::ArgList g_LDM_1_DB_w_arglist  (exec_LDM_1_DB_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_DB_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_DB_w_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_LDM_23_DB_arglist   (exec_LDM_23_DB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_DB_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_DB_PRINT
#endif  
                                              );
  ARMInstInfo::ArgList g_LDM_23_DB_w_arglist (exec_LDM_23_DB_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_DB_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_DB_w_PRINT
#endif
                                              );
  
  ARMInstInfo::ArgList g_LDM_1_IA_arglist    (exec_LDM_1_IA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_IA_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_IA_PRINT
#endif   
                                              );
  ARMInstInfo::ArgList g_LDM_1_IA_w_arglist  (exec_LDM_1_IA_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_IA_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_IA_w_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_LDM_23_IA_arglist   (exec_LDM_23_IA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_IA_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_IA_PRINT
#endif  
                                              );
  ARMInstInfo::ArgList g_LDM_23_IA_w_arglist (exec_LDM_23_IA_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_IA_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_IA_w_PRINT
#endif
                                              );
  
  ARMInstInfo::ArgList g_LDM_1_IB_arglist    (exec_LDM_1_IB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_IB_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_IB_PRINT
#endif   
                                              );
  ARMInstInfo::ArgList g_LDM_1_IB_w_arglist  (exec_LDM_1_IB_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_1_IB_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_1_IB_w_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_LDM_23_IB_arglist   (exec_LDM_23_IB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_IB_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_IB_PRINT
#endif  
                                              );
  ARMInstInfo::ArgList g_LDM_23_IB_w_arglist (exec_LDM_23_IB_w_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_LDM_23_IB_w_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_LDM_23_IB_w_PRINT
#endif
                                              );
  
  // SWP
  
  ARMInstInfo::ArgList g_SWP_arglist  (exec_SWP_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                       , exec_SWP_DT
#endif
#if PRINT_CHUNK_CODE
                                       , exec_SWP_PRINT
#endif
                                       );
  ARMInstInfo::ArgList g_SWPB_arglist (exec_SWPB_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                       , exec_SWPB_DT
#endif
#if PRINT_CHUNK_CODE
                                       , exec_SWPB_PRINT
#endif
                                       );
  
  // STR
  
  ARMInstInfo::ArgList g_STR_imm_post_down_arglist   (exec_STR_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_imm_post_down_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_STR_imm_post_up_arglist     (exec_STR_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_imm_post_up_PRINT
#endif    
                                                      );
  ARMInstInfo::ArgList g_STR_imm_offset_down_arglist (exec_STR_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_imm_offset_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_STR_imm_offset_up_arglist   (exec_STR_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_imm_offset_up_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_STR_imm_pre_down_arglist    (exec_STR_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_imm_pre_down_PRINT
#endif   
                                                      );
  ARMInstInfo::ArgList g_STR_imm_pre_up_arglist      (exec_STR_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_imm_pre_up_PRINT
#endif     
                                                      );
  
  ARMInstInfo::ArgList g_STR_reg_post_down_arglist   (exec_STR_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_reg_post_down_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_STR_reg_post_up_arglist     (exec_STR_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_reg_post_up_PRINT
#endif    
                                                      );
  ARMInstInfo::ArgList g_STR_reg_offset_down_arglist (exec_STR_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_reg_offset_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_STR_reg_offset_up_arglist   (exec_STR_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_reg_offset_up_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_STR_reg_pre_down_arglist    (exec_STR_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_reg_pre_down_PRINT
#endif   
                                                      );
  ARMInstInfo::ArgList g_STR_reg_pre_up_arglist      (exec_STR_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STR_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STR_reg_pre_up_PRINT
#endif     
                                                      );
  
  // STRT
  
  ARMInstInfo::ArgList g_STRT_imm_post_down_arglist (exec_STRT_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_STRT_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_STRT_imm_post_down_PRINT
#endif
                                                     );
  ARMInstInfo::ArgList g_STRT_imm_post_up_arglist   (exec_STRT_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_STRT_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_STRT_imm_post_up_PRINT
#endif  
                                                     );
  
  ARMInstInfo::ArgList g_STRT_reg_post_down_arglist (exec_STRT_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_STRT_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_STRT_reg_post_down_PRINT
#endif
                                                     );
  ARMInstInfo::ArgList g_STRT_reg_post_up_arglist   (exec_STRT_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_STRT_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_STRT_reg_post_up_PRINT
#endif  
                                                     );
  
  // STRB
  
  ARMInstInfo::ArgList g_STRB_imm_post_down_arglist   (exec_STRB_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_imm_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRB_imm_post_up_arglist     (exec_STRB_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_imm_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_STRB_imm_offset_down_arglist (exec_STRB_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_imm_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_STRB_imm_offset_up_arglist   (exec_STRB_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_imm_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRB_imm_pre_down_arglist    (exec_STRB_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_imm_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_STRB_imm_pre_up_arglist      (exec_STRB_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_imm_pre_up_PRINT
#endif     
                                                       );
  
  ARMInstInfo::ArgList g_STRB_reg_post_down_arglist   (exec_STRB_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_reg_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRB_reg_post_up_arglist     (exec_STRB_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_reg_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_STRB_reg_offset_down_arglist (exec_STRB_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_reg_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_STRB_reg_offset_up_arglist   (exec_STRB_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_reg_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRB_reg_pre_down_arglist    (exec_STRB_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_reg_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_STRB_reg_pre_up_arglist      (exec_STRB_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRB_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRB_reg_pre_up_PRINT
#endif     
                                                       );
  
  // STRBT
  
  ARMInstInfo::ArgList g_STRBT_imm_post_down_arglist (exec_STRBT_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STRBT_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STRBT_imm_post_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_STRBT_imm_post_up_arglist   (exec_STRBT_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STRBT_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STRBT_imm_post_up_PRINT
#endif  
                                                      );
  
  ARMInstInfo::ArgList g_STRBT_reg_post_down_arglist (exec_STRBT_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STRBT_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STRBT_reg_post_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_STRBT_reg_post_up_arglist   (exec_STRBT_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_STRBT_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_STRBT_reg_post_up_PRINT
#endif  
                                                      );
  
  // LDR
  
  ARMInstInfo::ArgList g_LDR_imm_post_down_arglist   (exec_LDR_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_imm_post_down_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_LDR_imm_post_up_arglist     (exec_LDR_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_imm_post_up_PRINT
#endif    
                                                      );
  ARMInstInfo::ArgList g_LDR_imm_offset_down_arglist (exec_LDR_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_imm_offset_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_LDR_imm_offset_up_arglist   (exec_LDR_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_imm_offset_up_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_LDR_imm_pre_down_arglist    (exec_LDR_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_imm_pre_down_PRINT
#endif   
                                                      );
  ARMInstInfo::ArgList g_LDR_imm_pre_up_arglist      (exec_LDR_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_imm_pre_up_PRINT
#endif     
                                                      );
  
  ARMInstInfo::ArgList g_LDR_reg_post_down_arglist   (exec_LDR_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_reg_post_down_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_LDR_reg_post_up_arglist     (exec_LDR_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_reg_post_up_PRINT
#endif    
                                                      );
  ARMInstInfo::ArgList g_LDR_reg_offset_down_arglist (exec_LDR_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_reg_offset_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_LDR_reg_offset_up_arglist   (exec_LDR_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_reg_offset_up_PRINT
#endif  
                                                      );
  ARMInstInfo::ArgList g_LDR_reg_pre_down_arglist    (exec_LDR_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_reg_pre_down_PRINT
#endif   
                                                      );
  ARMInstInfo::ArgList g_LDR_reg_pre_up_arglist      (exec_LDR_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDR_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDR_reg_pre_up_PRINT
#endif     
                                                      );
  
  // LDRT
  
  ARMInstInfo::ArgList g_LDRT_imm_post_down_arglist (exec_LDRT_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_LDRT_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_LDRT_imm_post_down_PRINT
#endif
                                                     );
  ARMInstInfo::ArgList g_LDRT_imm_post_up_arglist   (exec_LDRT_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_LDRT_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_LDRT_imm_post_up_PRINT
#endif  
                                                     );
  
  ARMInstInfo::ArgList g_LDRT_reg_post_down_arglist (exec_LDRT_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_LDRT_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_LDRT_reg_post_down_PRINT
#endif
                                                     );
  ARMInstInfo::ArgList g_LDRT_reg_post_up_arglist   (exec_LDRT_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                     , exec_LDRT_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                     , exec_LDRT_reg_post_up_PRINT
#endif  
                                                     );
  
  // LDRB
  
  ARMInstInfo::ArgList g_LDRB_imm_post_down_arglist   (exec_LDRB_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_imm_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRB_imm_post_up_arglist     (exec_LDRB_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_imm_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_LDRB_imm_offset_down_arglist (exec_LDRB_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_imm_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_LDRB_imm_offset_up_arglist   (exec_LDRB_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_imm_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRB_imm_pre_down_arglist    (exec_LDRB_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_imm_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_LDRB_imm_pre_up_arglist      (exec_LDRB_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_imm_pre_up_PRINT
#endif     
                                                       );
  
  ARMInstInfo::ArgList g_LDRB_reg_post_down_arglist   (exec_LDRB_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_reg_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRB_reg_post_up_arglist     (exec_LDRB_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_reg_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_LDRB_reg_offset_down_arglist (exec_LDRB_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_reg_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_LDRB_reg_offset_up_arglist   (exec_LDRB_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_reg_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRB_reg_pre_down_arglist    (exec_LDRB_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_reg_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_LDRB_reg_pre_up_arglist      (exec_LDRB_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRB_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRB_reg_pre_up_PRINT
#endif     
                                                       );
  
  // LDRBT
  
  ARMInstInfo::ArgList g_LDRBT_imm_post_down_arglist (exec_LDRBT_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDRBT_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDRBT_imm_post_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_LDRBT_imm_post_up_arglist   (exec_LDRBT_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDRBT_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDRBT_imm_post_up_PRINT
#endif  
                                                      );
  
  ARMInstInfo::ArgList g_LDRBT_reg_post_down_arglist (exec_LDRBT_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDRBT_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDRBT_reg_post_down_PRINT
#endif
                                                      );
  ARMInstInfo::ArgList g_LDRBT_reg_post_up_arglist   (exec_LDRBT_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                      , exec_LDRBT_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                      , exec_LDRBT_reg_post_up_PRINT
#endif  
                                                      );
  
  // STRH
  
  ARMInstInfo::ArgList g_STRH_imm_post_down_arglist   (exec_STRH_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_imm_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRH_imm_post_up_arglist     (exec_STRH_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_imm_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_STRH_imm_offset_down_arglist (exec_STRH_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_imm_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_STRH_imm_offset_up_arglist   (exec_STRH_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_imm_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRH_imm_pre_down_arglist    (exec_STRH_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_imm_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_STRH_imm_pre_up_arglist      (exec_STRH_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_imm_pre_up_PRINT
#endif     
                                                       );
  
  ARMInstInfo::ArgList g_STRH_reg_post_down_arglist   (exec_STRH_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_reg_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRH_reg_post_up_arglist     (exec_STRH_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_reg_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_STRH_reg_offset_down_arglist (exec_STRH_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_reg_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_STRH_reg_offset_up_arglist   (exec_STRH_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_reg_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_STRH_reg_pre_down_arglist    (exec_STRH_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_reg_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_STRH_reg_pre_up_arglist      (exec_STRH_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_STRH_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_STRH_reg_pre_up_PRINT
#endif     
                                                       );
  
  // LDRH
  
  ARMInstInfo::ArgList g_LDRH_imm_post_down_arglist   (exec_LDRH_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_imm_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRH_imm_post_up_arglist     (exec_LDRH_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_imm_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_LDRH_imm_offset_down_arglist (exec_LDRH_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_imm_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_LDRH_imm_offset_up_arglist   (exec_LDRH_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_imm_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRH_imm_pre_down_arglist    (exec_LDRH_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_imm_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_LDRH_imm_pre_up_arglist      (exec_LDRH_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_imm_pre_up_PRINT
#endif     
                                                       );
  
  ARMInstInfo::ArgList g_LDRH_reg_post_down_arglist   (exec_LDRH_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_reg_post_down_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRH_reg_post_up_arglist     (exec_LDRH_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_reg_post_up_PRINT
#endif    
                                                       );
  ARMInstInfo::ArgList g_LDRH_reg_offset_down_arglist (exec_LDRH_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_reg_offset_down_PRINT
#endif
                                                       );
  ARMInstInfo::ArgList g_LDRH_reg_offset_up_arglist   (exec_LDRH_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_reg_offset_up_PRINT
#endif  
                                                       );
  ARMInstInfo::ArgList g_LDRH_reg_pre_down_arglist    (exec_LDRH_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_reg_pre_down_PRINT
#endif   
                                                       );
  ARMInstInfo::ArgList g_LDRH_reg_pre_up_arglist      (exec_LDRH_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                       , exec_LDRH_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                       , exec_LDRH_reg_pre_up_PRINT
#endif     
                                                       );
  
  // LDRSB
  
  ARMInstInfo::ArgList g_LDRSB_imm_post_down_arglist   (exec_LDRSB_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_imm_post_down_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSB_imm_post_up_arglist     (exec_LDRSB_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_imm_post_up_PRINT
#endif    
                                                        );
  ARMInstInfo::ArgList g_LDRSB_imm_offset_down_arglist (exec_LDRSB_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_imm_offset_down_PRINT
#endif
                                                        );
  ARMInstInfo::ArgList g_LDRSB_imm_offset_up_arglist   (exec_LDRSB_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_imm_offset_up_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSB_imm_pre_down_arglist    (exec_LDRSB_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_imm_pre_down_PRINT
#endif   
                                                        );
  ARMInstInfo::ArgList g_LDRSB_imm_pre_up_arglist      (exec_LDRSB_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_imm_pre_up_PRINT
#endif     
                                                        );
  
  ARMInstInfo::ArgList g_LDRSB_reg_post_down_arglist   (exec_LDRSB_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_reg_post_down_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSB_reg_post_up_arglist     (exec_LDRSB_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_reg_post_up_PRINT
#endif    
                                                        );
  ARMInstInfo::ArgList g_LDRSB_reg_offset_down_arglist (exec_LDRSB_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_reg_offset_down_PRINT
#endif
                                                        );
  ARMInstInfo::ArgList g_LDRSB_reg_offset_up_arglist   (exec_LDRSB_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_reg_offset_up_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSB_reg_pre_down_arglist    (exec_LDRSB_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_reg_pre_down_PRINT
#endif   
                                                        );
  ARMInstInfo::ArgList g_LDRSB_reg_pre_up_arglist      (exec_LDRSB_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSB_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSB_reg_pre_up_PRINT
#endif     
                                                        );
  
  // LDRSH
  
  ARMInstInfo::ArgList g_LDRSH_imm_post_down_arglist   (exec_LDRSH_imm_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_imm_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_imm_post_down_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSH_imm_post_up_arglist     (exec_LDRSH_imm_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_imm_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_imm_post_up_PRINT
#endif    
                                                        );
  ARMInstInfo::ArgList g_LDRSH_imm_offset_down_arglist (exec_LDRSH_imm_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_imm_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_imm_offset_down_PRINT
#endif
                                                        );
  ARMInstInfo::ArgList g_LDRSH_imm_offset_up_arglist   (exec_LDRSH_imm_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_imm_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_imm_offset_up_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSH_imm_pre_down_arglist    (exec_LDRSH_imm_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_imm_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_imm_pre_down_PRINT
#endif   
                                                        );
  ARMInstInfo::ArgList g_LDRSH_imm_pre_up_arglist      (exec_LDRSH_imm_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_imm_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_imm_pre_up_PRINT
#endif     
                                                        );
  
  ARMInstInfo::ArgList g_LDRSH_reg_post_down_arglist   (exec_LDRSH_reg_post_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_reg_post_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_reg_post_down_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSH_reg_post_up_arglist     (exec_LDRSH_reg_post_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_reg_post_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_reg_post_up_PRINT
#endif    
                                                        );
  ARMInstInfo::ArgList g_LDRSH_reg_offset_down_arglist (exec_LDRSH_reg_offset_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_reg_offset_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_reg_offset_down_PRINT
#endif
                                                        );
  ARMInstInfo::ArgList g_LDRSH_reg_offset_up_arglist   (exec_LDRSH_reg_offset_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_reg_offset_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_reg_offset_up_PRINT
#endif  
                                                        );
  ARMInstInfo::ArgList g_LDRSH_reg_pre_down_arglist    (exec_LDRSH_reg_pre_down_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_reg_pre_down_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_reg_pre_down_PRINT
#endif   
                                                        );
  ARMInstInfo::ArgList g_LDRSH_reg_pre_up_arglist      (exec_LDRSH_reg_pre_up_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                                        , exec_LDRSH_reg_pre_up_DT
#endif
#if PRINT_CHUNK_CODE
                                                        , exec_LDRSH_reg_pre_up_PRINT
#endif     
                                                        );
  
  // DPI
  
  ARMInstInfo::ArgList g_AND_regimm_arglist  (exec_AND_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_AND_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_AND_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_AND_regreg_arglist  (exec_AND_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_AND_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_AND_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_AND_imm_arglist     (exec_AND_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_AND_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_AND_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_ANDS_regimm_arglist (exec_ANDS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ANDS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ANDS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ANDS_regreg_arglist (exec_ANDS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ANDS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ANDS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ANDS_imm_arglist    (exec_ANDS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ANDS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ANDS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_EOR_regimm_arglist  (exec_EOR_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_EOR_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_EOR_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_EOR_regreg_arglist  (exec_EOR_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_EOR_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_EOR_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_EOR_imm_arglist     (exec_EOR_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_EOR_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_EOR_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_EORS_regimm_arglist (exec_EORS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_EORS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_EORS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_EORS_regreg_arglist (exec_EORS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_EORS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_EORS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_EORS_imm_arglist    (exec_EORS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_EORS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_EORS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_SUB_regimm_arglist  (exec_SUB_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SUB_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SUB_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_SUB_regreg_arglist  (exec_SUB_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SUB_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SUB_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_SUB_imm_arglist     (exec_SUB_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SUB_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SUB_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_SUBS_regimm_arglist (exec_SUBS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SUBS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SUBS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_SUBS_regreg_arglist (exec_SUBS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SUBS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SUBS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_SUBS_imm_arglist    (exec_SUBS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SUBS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SUBS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_RSB_regimm_arglist  (exec_RSB_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSB_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSB_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_RSB_regreg_arglist  (exec_RSB_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSB_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSB_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_RSB_imm_arglist     (exec_RSB_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSB_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSB_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_RSBS_regimm_arglist (exec_RSBS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSBS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSBS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_RSBS_regreg_arglist (exec_RSBS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSBS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSBS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_RSBS_imm_arglist    (exec_RSBS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSBS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSBS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_ADD_regimm_arglist  (exec_ADD_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADD_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADD_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_ADD_regreg_arglist  (exec_ADD_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADD_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADD_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_ADD_imm_arglist     (exec_ADD_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADD_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADD_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_ADDS_regimm_arglist (exec_ADDS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADDS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADDS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ADDS_regreg_arglist (exec_ADDS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADDS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADDS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ADDS_imm_arglist    (exec_ADDS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADDS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADDS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_ADC_regimm_arglist  (exec_ADC_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADC_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADC_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_ADC_regreg_arglist  (exec_ADC_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADC_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADC_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_ADC_imm_arglist     (exec_ADC_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADC_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADC_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_ADCS_regimm_arglist (exec_ADCS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADCS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADCS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ADCS_regreg_arglist (exec_ADCS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADCS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADCS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ADCS_imm_arglist    (exec_ADCS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ADCS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ADCS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_SBC_regimm_arglist  (exec_SBC_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SBC_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SBC_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_SBC_regreg_arglist  (exec_SBC_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SBC_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SBC_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_SBC_imm_arglist     (exec_SBC_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SBC_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SBC_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_SBCS_regimm_arglist (exec_SBCS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SBCS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SBCS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_SBCS_regreg_arglist (exec_SBCS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SBCS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SBCS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_SBCS_imm_arglist    (exec_SBCS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_SBCS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_SBCS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_RSC_regimm_arglist  (exec_RSC_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSC_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSC_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_RSC_regreg_arglist  (exec_RSC_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSC_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSC_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_RSC_imm_arglist     (exec_RSC_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSC_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSC_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_RSCS_regimm_arglist (exec_RSCS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSCS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSCS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_RSCS_regreg_arglist (exec_RSCS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSCS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSCS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_RSCS_imm_arglist    (exec_RSCS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_RSCS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_RSCS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_ORR_regimm_arglist  (exec_ORR_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ORR_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ORR_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_ORR_regreg_arglist  (exec_ORR_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ORR_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ORR_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_ORR_imm_arglist     (exec_ORR_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ORR_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ORR_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_ORRS_regimm_arglist (exec_ORRS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ORRS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ORRS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ORRS_regreg_arglist (exec_ORRS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ORRS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ORRS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_ORRS_imm_arglist    (exec_ORRS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_ORRS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_ORRS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_MOV_regimm_arglist  (exec_MOV_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MOV_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MOV_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_MOV_regreg_arglist  (exec_MOV_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MOV_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MOV_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_MOV_imm_arglist     (exec_MOV_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MOV_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MOV_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_MOVS_regimm_arglist (exec_MOVS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MOVS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MOVS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_MOVS_regreg_arglist (exec_MOVS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MOVS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MOVS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_MOVS_imm_arglist    (exec_MOVS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MOVS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MOVS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_BIC_regimm_arglist  (exec_BIC_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_BIC_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_BIC_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_BIC_regreg_arglist  (exec_BIC_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_BIC_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_BIC_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_BIC_imm_arglist     (exec_BIC_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_BIC_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_BIC_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_BICS_regimm_arglist (exec_BICS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_BICS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_BICS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_BICS_regreg_arglist (exec_BICS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_BICS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_BICS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_BICS_imm_arglist    (exec_BICS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_BICS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_BICS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_MVN_regimm_arglist  (exec_MVN_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MVN_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MVN_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_MVN_regreg_arglist  (exec_MVN_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MVN_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MVN_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_MVN_imm_arglist     (exec_MVN_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MVN_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MVN_imm_PRINT
#endif    
                                              );
  ARMInstInfo::ArgList g_MVNS_regimm_arglist (exec_MVNS_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MVNS_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MVNS_regimm_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_MVNS_regreg_arglist (exec_MVNS_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MVNS_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MVNS_regreg_PRINT
#endif
                                              );
  ARMInstInfo::ArgList g_MVNS_imm_arglist    (exec_MVNS_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_MVNS_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_MVNS_imm_PRINT
#endif   
                                              );
  
  ARMInstInfo::ArgList g_TST_regimm_arglist  (exec_TST_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_TST_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_TST_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_TST_regreg_arglist  (exec_TST_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_TST_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_TST_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_TST_imm_arglist     (exec_TST_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_TST_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_TST_imm_PRINT
#endif    
                                              );
  
  ARMInstInfo::ArgList g_TEQ_regimm_arglist  (exec_TEQ_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_TEQ_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_TEQ_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_TEQ_regreg_arglist  (exec_TEQ_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_TEQ_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_TEQ_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_TEQ_imm_arglist     (exec_TEQ_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_TEQ_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_TEQ_imm_PRINT
#endif    
                                              );
  
  ARMInstInfo::ArgList g_CMP_regimm_arglist  (exec_CMP_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_CMP_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_CMP_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_CMP_regreg_arglist  (exec_CMP_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_CMP_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_CMP_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_CMP_imm_arglist     (exec_CMP_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_CMP_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_CMP_imm_PRINT
#endif    
                                              );
  
  ARMInstInfo::ArgList g_CMN_regimm_arglist  (exec_CMN_regimm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_CMN_regimm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_CMN_regimm_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_CMN_regreg_arglist  (exec_CMN_regreg_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_CMN_regreg_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_CMN_regreg_PRINT
#endif 
                                              );
  ARMInstInfo::ArgList g_CMN_imm_arglist     (exec_CMN_imm_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                              , exec_CMN_imm_DT
#endif
#if PRINT_CHUNK_CODE
                                              , exec_CMN_imm_PRINT
#endif    
                                              );
  
  // Branch
  
  ARMInstInfo::ArgList g_BLX_1_arglist (exec_BLX_1_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                        , exec_BLX_1_DT
#endif
#if PRINT_CHUNK_CODE
                                        , exec_BLX_1_PRINT
#endif
                                        );
  ARMInstInfo::ArgList g_BLX_2_arglist (exec_BLX_2_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                        , exec_BLX_2_DT
#endif
#if PRINT_CHUNK_CODE
                                        , exec_BLX_2_PRINT
#endif
                                        );
  ARMInstInfo::ArgList g_BX_arglist    (exec_BX_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                        , exec_BX_DT
#endif
#if PRINT_CHUNK_CODE
                                        , exec_BX_PRINT
#endif   
                                        );
  ARMInstInfo::ArgList g_B_arglist     (exec_B_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                        , exec_B_DT
#endif
#if PRINT_CHUNK_CODE
                                        , exec_B_PRINT
#endif    
                                        );
  ARMInstInfo::ArgList g_BL_arglist    (exec_BL_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                        , exec_BL_DT
#endif
#if PRINT_CHUNK_CODE
                                        , exec_BL_PRINT
#endif   
                                        );
  
  // Multi
  
  ARMInstInfo::ArgList g_MUL_arglist    (exec_MUL_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_MUL_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_MUL_PRINT
#endif   
                                         );
  ARMInstInfo::ArgList g_MULS_arglist   (exec_MULS_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_MULS_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_MULS_PRINT
#endif  
                                         );
  ARMInstInfo::ArgList g_MLA_arglist    (exec_MLA_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_MLA_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_MLA_PRINT
#endif   
                                         );
  ARMInstInfo::ArgList g_MLAS_arglist   (exec_MLAS_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_MLAS_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_MLAS_PRINT
#endif  
                                         );
  ARMInstInfo::ArgList g_UMULL_arglist  (exec_UMULL_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_UMULL_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_UMULL_PRINT
#endif 
                                         );
  ARMInstInfo::ArgList g_UMULLS_arglist (exec_UMULLS_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_UMULLS_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_UMULLS_PRINT
#endif
                                         );
  ARMInstInfo::ArgList g_UMLAL_arglist  (exec_UMLAL_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_UMLAL_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_UMLAL_PRINT
#endif 
                                         );
  ARMInstInfo::ArgList g_UMLALS_arglist (exec_UMLALS_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_UMLALS_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_UMLALS_PRINT
#endif
                                         );
  ARMInstInfo::ArgList g_SMULL_arglist  (exec_SMULL_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_SMULL_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_SMULL_PRINT
#endif 
                                         );
  ARMInstInfo::ArgList g_SMULLS_arglist (exec_SMULLS_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_SMULLS_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_SMULLS_PRINT
#endif
                                         );
  ARMInstInfo::ArgList g_SMLAL_arglist  (exec_SMLAL_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_SMLAL_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_SMLAL_PRINT
#endif 
                                         );
  ARMInstInfo::ArgList g_SMLALS_arglist (exec_SMLALS_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                         , exec_SMLALS_DT
#endif
#if PRINT_CHUNK_CODE
                                         , exec_SMLALS_PRINT
#endif
                                         );
  
  // MRS
  
  ARMInstInfo::ArgList g_MRS_cpsr_arglist (exec_MRS_cpsr_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                           , exec_MRS_cpsr_DT
#endif
#if PRINT_CHUNK_CODE
                                           , exec_MRS_cpsr_PRINT
#endif
                                           );
  ARMInstInfo::ArgList g_MRS_spsr_arglist (exec_MRS_spsr_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                           , exec_MRS_spsr_DT
#endif
#if PRINT_CHUNK_CODE
                                           , exec_MRS_spsr_PRINT
#endif
                                           );
  
  // MSR
  
  ARMInstInfo::ArgList g_MSR_imm_cpsr_arglist (exec_MSR_imm_cpsr_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                               , exec_MSR_imm_cpsr_DT
#endif
#if PRINT_CHUNK_CODE
                                               , exec_MSR_imm_cpsr_PRINT
#endif
                                               );
  ARMInstInfo::ArgList g_MSR_imm_spsr_arglist (exec_MSR_imm_spsr_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                               , exec_MSR_imm_spsr_DT
#endif
#if PRINT_CHUNK_CODE
                                               , exec_MSR_imm_spsr_PRINT
#endif
                                               );
  
  ARMInstInfo::ArgList g_MSR_reg_cpsr_arglist (exec_MSR_reg_cpsr_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                               , exec_MSR_reg_cpsr_DT
#endif
#if PRINT_CHUNK_CODE
                                               , exec_MSR_reg_cpsr_PRINT
#endif
                                               );
  ARMInstInfo::ArgList g_MSR_reg_spsr_arglist (exec_MSR_reg_spsr_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                               , exec_MSR_reg_spsr_DT
#endif
#if PRINT_CHUNK_CODE
                                               , exec_MSR_reg_spsr_PRINT
#endif
                                               );
  
  // CDT
  
  ARMInstInfo::ArgList g_CDT_arglist(exec_CDT_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                     , exec_CDT_DT
#endif
#if PRINT_CHUNK_CODE
                                     , exec_CDT_PRINT
#endif
                                     );
  
  // CDP
  
  ARMInstInfo::ArgList g_CDP_arglist(exec_CDP_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                     , exec_CDP_DT
#endif
#if PRINT_CHUNK_CODE
                                     , exec_CDP_PRINT
#endif
                                     );
  
  // CRT<LOAD>
  
  ARMInstInfo::ArgList g_CRT_LOAD_arglist(exec_CRT_TC<LOAD>
#if ENABLE_DYNAMIC_TRANSLATOR
                                          , exec_CRT_DT<LOAD>
#endif
#if PRINT_CHUNK_CODE
                                          , exec_CRT_PRINT<LOAD>
#endif
                                          );
  
  // CRT<STORE>
  
  ARMInstInfo::ArgList g_CRT_STORE_arglist(exec_CRT_TC<STORE>
#if ENABLE_DYNAMIC_TRANSLATOR
                                           , exec_CRT_DT<STORE>
#endif
#if PRINT_CHUNK_CODE
                                           , exec_CRT_PRINT<STORE>
#endif
                                           );
  
  // SWI
  
  ARMInstInfo::ArgList g_SWI_arglist(exec_SWI_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                     , exec_SWI_DT
#endif
#if PRINT_CHUNK_CODE
                                     , exec_SWI_PRINT
#endif
                                     );
  
  // UND
  
  ARMInstInfo::ArgList g_UND_arglist(exec_UND_TC
#if ENABLE_DYNAMIC_TRANSLATOR
                                     , exec_UND_DT
#endif
#if PRINT_CHUNK_CODE
                                     , exec_UND_PRINT
#endif
                                     );
}
