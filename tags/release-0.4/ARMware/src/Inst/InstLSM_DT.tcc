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

namespace ARMware
{
#if ENABLE_DYNAMIC_TRANSLATOR  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  gen_dt_code(Inst const inst, bool const is_last)
  {
    mInst = inst;
    
    switch (is_load)
    {
    case STORE:
      switch (direction)
      {
      case DOWN:
        switch (start_from)
        {
        case AFTER:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_DA_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_DA_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_2_DA_TC);
              break;
              
            case WRITE_BACK:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'store-user-write-back' variants in LSM instructions.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case BEFORE:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_DB_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_DB_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_2_DB_TC);
              break;
              
            case WRITE_BACK:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'store-user-write-back' variants in LSM instructions.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      case UP:
        switch (start_from)
        {
        case AFTER:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_IA_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_IA_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_2_IA_TC);
              break;
              
            case WRITE_BACK:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'store-user-write-back' variants in LSM instructions.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case BEFORE:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_IB_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_1_IB_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_STM_2_IB_TC);
              break;
              
            case WRITE_BACK:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'store-user-write-back' variants in LSM instructions.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      default: assert(!"Should not reach here."); break;
      }
      break;
      
    case LOAD:
      switch (direction)
      {
      case DOWN:
        switch (start_from)
        {
        case AFTER:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_DA_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_DA_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_DA_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_DA_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case BEFORE:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_DB_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_DB_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_DB_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_DB_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      case UP:
        switch (start_from)
        {
        case AFTER:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_IA_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_IA_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_IA_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_IA_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case BEFORE:
          switch (desired_bank)
          {
          case CURRENT_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_IB_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_1_IB_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case USER_BANK:
            switch (is_write_back)
            {
            case NO_WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_IB_TC);
              break;
              
            case WRITE_BACK:
              dt_func = reinterpret_cast<DTFunc_t>(exec_LDM_23_IB_w_TC);
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      default: assert(!"Should not reach here."); break;
      }
      break;
      
    default: assert(!"Should not reach here."); break;
    }
    
    Label * const routine_label =
      gp_chunk->add_new_label(new Label(static_cast<LabelRoutine *>(0), dt_func));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(
      new MIR(static_cast<MIRCall_1 *>(0),
              routine_label,
              gp_chunk->find_const_var(mInst.mRawData)));
    
    Variable * const tmp1 = gp_chunk->get_new_temp();
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRRetrieve *>(0), 1, tmp1));
    
    Label * const table_base_label = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0)));
    
    MIR * const jmp_mir = gp_chunk->insert_switch_statement_DT(tmp1, table_base_label);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Create jump table.
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL:
    Label * const label_ER_NORMAL = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0)));
    
    MIR * const table_base_mir =
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                   label_ER_NORMAL));
    
    table_base_label->set_mir_inst(table_base_mir);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_EXCEPTION:
    Label * const label_ER_EXCEPTION = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_EXCEPTION));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL_CONT_EXEC_IN_CHUNK:
    //
    // This value is not possible for LSH, thus we set the value of this label to 0.
    Label * const label_ER_NORMAL_CONT_EXEC_IN_CHUNK =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_NORMAL_CONT_EXEC_IN_CHUNK));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL_MODIFY_PC:
    Label * const label_ER_NORMAL_MODIFY_PC = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_NORMAL_MODIFY_PC));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_IO_IRQ_OCCUR:
    //
    // This value is not possible for LSH, thus we set the value of this label to 0.
    Label * const label_ER_IO_IRQ_OCCUR = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_IO_IRQ_OCCUR));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_IO_FIQ_OCCUR:
    //
    // This value is not possible for LSH, thus we set the value of this label to 0.
    Label * const label_ER_IO_FIQ_OCCUR = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_IO_FIQ_OCCUR));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_CHUNK_DISAPPEAR:
    Label * const label_ER_CHUNK_DISAPPEAR = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_CHUNK_DISAPPEAR));
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // End of the basic block for the jump table.
    gp_chunk->next_mir_is_leader();
    
    jmp_mir->add_jump_target_label(label_ER_NORMAL);
    jmp_mir->add_jump_target_label(label_ER_NORMAL_MODIFY_PC);
    jmp_mir->add_jump_target_label(label_ER_EXCEPTION);
    jmp_mir->add_jump_target_label(label_ER_CHUNK_DISAPPEAR);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Jump target implementation.
    
    // :NOTE: Wei 2004-Oct-03:
    //
    // The possible value returned from 'LSM' are:
    //
    // (1) ER_EXCEPTION
    // (2) ER_CHUNK_DISAPPEAR
    // (3) ER_NORMAL
    // (4) ER_NORMAL_MODIFY_PC
    //
    // if (1), (2) or (4), then ARMware will return to the interpreter,
    // otherwise, (3) will continue execuating in this chunk.
        
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_EXCEPTION
    
    label_ER_EXCEPTION->set_mir_inst(gp_chunk->append_mir<MIR_FOR_NORMAL>(
                                       new MIR(static_cast<MIRReturn *>(0),
                                               gp_chunk->find_const_var(ER_EXCEPTION))));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_NORNAL_MODIFY_PC
    
    label_ER_NORMAL_MODIFY_PC->set_mir_inst(gp_chunk->append_mir<MIR_FOR_NORMAL>(
                                              new MIR(static_cast<MIRReturn *>(0),
                                                      gp_chunk->find_const_var(ER_NORMAL_MODIFY_PC))));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_CHUNK_DISAPPEAR
    
    label_ER_CHUNK_DISAPPEAR->set_mir_inst(
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                   gp_chunk->find_const_var(ER_CHUNK_DISAPPEAR))));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_NORMAL.
    
    if (true == is_last)
    {
      label_ER_NORMAL->set_mir_inst(
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                     gp_chunk->find_const_var(ER_NORMAL))));
    }
    else
    {
      label_ER_NORMAL->reset_to_arm_first_inst(gp_chunk->next_arm_inst_info());
      
      gp_chunk->add_pending_back_patch_label(label_ER_NORMAL);
    }
  }
#endif
}
