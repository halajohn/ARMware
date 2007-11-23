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
  template<MIRAppendType T_type>
  MIR *
  Chunk::append_mir(MIR * const mir)
  {
#if CHECK_CHUNK_CODE
    // :NOTE: Wei 2004-Nov-4:
    //
    // The setting of the condition block have to come first.
    assert(true == m_finish_setting_cond_block);
#endif
    
    // :NOTE: Wei 2004-Nov-2:
    //
    // Set the information of the current condition block.
    mir->set_cond_block(mp_cond_block);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // If we haven't set the first MIR instruction of the current
    // ARM instruction, then set it now.
    switch (T_type)
    {
    case MIR_FOR_NORMAL:
      if (0 == mp_curr_arm_inst_info->first_mir())
      {
        mp_curr_arm_inst_info->set_first_mir(mir);
      }
      
      if (0 == mp_curr_arm_inst_info->first_normal_mir())
      {
        mp_curr_arm_inst_info->set_first_normal_mir(mir);
      }
      break;
      
    case MIR_FOR_COND_CODE:
      assert(0 == mp_curr_arm_inst_info->first_normal_mir());
      
      if (0 == mp_curr_arm_inst_info->first_mir())
      {
        mp_curr_arm_inst_info->set_first_mir(mir);
      }
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    mir->set_belong_to_which_arm_inst(mp_curr_arm_inst_info);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Determine the current basic block.
    if (true == m_next_mir_is_leader)
    {
      mp_curr_bb = new BasicBlock(mir);
      
      // :NOTE: Wei 2004-Nov-5:
      //
      // Record this new basic block, so that we can free it in the destructor.
      m_bb_table.push_back(mp_curr_bb);
      
      m_next_mir_is_leader = false;
    }
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Set the basic block for the current MIR instruction.
    mir->set_bb(mp_curr_bb);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Chain the current MIR into the MIR link lists.
    if (0 == mp_mir_head)
    {
      // :NOTE: Wei 2004-Nov-4:
      //
      // The previous one of the first MIR instruction should be 0.
      mir->set_prev_mir(0);
      
      mp_mir_head = mir;
    }
    else
    {
      mir->set_prev_mir(mp_mir_tail);
      
      mp_mir_tail->set_next_mir(mir);
    }
    
    mp_mir_tail = mir;
    
    m_all_mir.push_back(mir);
    
    switch (mir->kind())
    {
    case MIR_CALL_0:
    case MIR_CALL_1:
    case MIR_RETURN:
    case MIR_CMP_EQ:
    case MIR_GOTO:
      
    case MIR_GOTO_EQ:
    case MIR_GOTO_NE:
    case MIR_GOTO_CS:
    case MIR_GOTO_CC:
    case MIR_GOTO_MI:
    case MIR_GOTO_PL:
    case MIR_GOTO_VS:
    case MIR_GOTO_VC:
    case MIR_GOTO_HI:
    case MIR_GOTO_LS:
    case MIR_GOTO_GE:
    case MIR_GOTO_LT:
    case MIR_GOTO_GT:
    case MIR_GOTO_LE:
      
    case MIR_JUMP:
      m_next_mir_is_leader = true;
      break;
      
    case MIR_RETRIEVE:
    case MIR_ADD:
    case MIR_ADD64:
    case MIR_ADC:
    case MIR_MOV:
    case MIR_AND:
    case MIR_OR:
    case MIR_EOR:
    case MIR_SUB:
    case MIR_SBC:
    case MIR_TST:
    case MIR_CMP:
    case MIR_NOT:
    case MIR_MUL32:
    case MIR_UMUL64:
    case MIR_SMUL64:
    case MIR_LSFT_LEFT:
    case MIR_LSFT_RIGHT:
    case MIR_ASFT_RIGHT:
    case MIR_RRX:
    case MIR_ROR:
    case MIR_PRODUCE_CBIT:
    case MIR_CLEAR_CBIT:
    case MIR_DATA32:
    case MIR_LOAD_LABEL:
    case MIR_LOAD:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    return mir;
  }
  
  template<InstCond T_cond>
  inline void
  Chunk::translate_cond_code()
  {
    // :NOTE: Wei 2004-Nov-4:
    //
    // The only possible place where m_flags_no_change is true
    // is in the middle of a condition block.
    if ((true == m_flags_no_change) && (T_cond == m_old_cond))
    {
      // :NOTE: Wei 2004-Nov-4:
      //
      // We shouldn't have a condition block of type AL, otherwise we should
      // have one.
      assert((AL == m_old_cond) ? (0 == mp_cond_block) : (mp_cond_block != 0));
      
#if CHECK_CHUNK_CODE
      m_finish_setting_cond_block = true;
#endif
      
      // :NOTE: Wei 2004-Oct-30:
      //
      // We don't need to emit any MIR codes to handle this condition code,
      // because it is the same with the previous one (i.e. in the same condition block).
      goto done;
    }
    else
    {
      // :NOTE: Wei 2004-Oct-30:
      //
      // The previous condition block has been closed.
      //
      // The possible reasons are:
      //
      // 1) The previous ARM instruction has changed the condition code,
      //    thus 'm_flags_no_change == false'.
      // 2) The condition code of the current ARM instruction is different from
      //    the previous one, thus 'm_old_cond != T_cond'.
      
      // :NOTE: Wei 2004-Oct-30:
      //
      // If we have a previous MIR instruction that needs to be back-patched the
      // destination jump label because of the condition block, then back-patched it now.
      if (mp_cond_block_back_patching_label != 0)
      {
        mp_cond_block_back_patching_label->set_arm_first_inst(mp_curr_arm_inst_info);
        
        // :NOTE: Wei 2004-Oct-30:
        //
        // Because this label still needs to be converted to point to the MIR instruction
        // instead of pointing to the ARM instruction, thus we need to add this label
        // to the back-patching list.
        add_pending_back_patch_label(mp_cond_block_back_patching_label);
        
        // :NOTE: Wei 2005-Apr-2:
        //
        // Patch the condition block addition
        back_patch_cond_block_back_patching_add_mir();
      }
      
      if (T_cond != AL)
      {
        // :NOTE: Wei 2004-Nov-2:
        //
        // Close the previous condition block, and start a new one.
        //
        // :NOTE: Wei 2004-Nov-2:
        //
        // This operation must be done before the first call to 'append_mir',
        // because 'append_mir' needs the information of the
        // current condition block.
        mp_cond_block = new ConditionBlock(T_cond);
        
#if CHECK_CHUNK_CODE
        m_finish_setting_cond_block = true;
#endif
        
        // :NOTE: Wei 2004-Nov-5:
        //
        // Record this condition block, so that we can free it in the destructor.
        m_cond_block_table.push_back(mp_cond_block);
        
        Label * const normal_mir_label =
          add_new_label(new Label(static_cast<LabelARMNormalInst *>(0), mp_curr_arm_inst_info));
        
        MIR * const first_goto_mir = append_mir<MIR_FOR_COND_CODE>(
          new MIR(static_cast<typename CondCodeTranslationTraits<T_cond>::MIRType *>(0),
                  normal_mir_label));
        
        add_pending_back_patch_label(normal_mir_label);
        
        first_goto_mir->set_in_out_flags(CondCodeTranslationTraits<T_cond>::PRELOAD_STATUS_FLAG, 0);
        
        mp_cond_block_back_patching_add_mir = append_mir<MIR_FOR_COND_CODE>(
          new MIR(static_cast<MIRAdd *>(0),
                  find_global_var(Variable::CR_PC),
                  0,
                  find_global_var(Variable::CR_PC)));
        
        // :NOTE: Wei 2004-Oct-30:
        //
        // Because the jump target here should always be on the ARM instruction boundry,
        // thus I use 'LabelARMInst' here.
        // And back patching them later on.
        mp_cond_block_back_patching_label =
          add_new_label(new Label(static_cast<LabelARMFirstInst *>(0)));
        
        (void)append_mir<MIR_FOR_COND_CODE>(new MIR(static_cast<MIRGoto *>(0), mp_cond_block_back_patching_label));
      }
      else
      {
        // :NOTE: Wei 2004-Oct-30:
        //
        // Close the previous condition block, but don't start a new one.
        mp_cond_block = 0;
        
#if CHECK_CHUNK_CODE
        m_finish_setting_cond_block = true;
#endif
        
        // :NOTE: Wei 2004-Oct-30:
        //
        // If the current condition code is AL, then we won't have any label to back-patch,
        // thus set 'mp_cond_block_back_patching_label' to 0.
        mp_cond_block_back_patching_label = 0;
      }
      
      m_flags_no_change = true;
      m_old_cond = T_cond;
    }
    
  done:
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Increment Core::m_device_timer_increment.
    append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                               find_global_var(Variable::DEVICE_TIMER_INCREMENT),
                               find_const_var(1),
                               find_global_var(Variable::DEVICE_TIMER_INCREMENT)));
  }
  
  template<InstCond T_cond>
  inline MIR *
  Chunk::break_cond_block(MIR * const mir)
  {
    assert((T_cond != AL) || !"We should not have a AL condition block.");
    assert((T_cond != NV) || !"Should not reach here.");
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // If the target MIR is already the desired GOTO_xx MIR,
    // then we don't need to add another one, just return.
    if (CondCodeTranslationTraits<T_cond>::MIRType::KIND == mir->kind())
    {
      // :NOTE: Wei 2005-Apr-2:
      //
      // Because the MIRGotoXX instruction is always used in the condition code translation,
      // thus if I see a MIRGotoXX instruction, then I can sure that the following 2 instruction
      // will be the MIR_ADD & MIR_GOTO.
      assert(mir->next_mir() != 0);
      assert(MIR_ADD == mir->next_mir()->kind());
      assert(mir->next_mir()->next_mir() != 0);
      assert(MIR_GOTO == mir->next_mir()->next_mir()->kind());
      
      // :NOTE: Wei 2004-Nov-5:
      //
      // Because we don't add any new MIR instruction,
      // thus we return the original one.
      return mir;
    }
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Find the end of the current condition block.
    MIR *curr_mir = mir->next_mir();
    
    while ((curr_mir != 0) && (curr_mir->cond_block() == mir->cond_block()))
    {
      curr_mir = curr_mir->next_mir();
    }
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // We must find one, otherwise this is a design error.
    assert(curr_mir != 0);
    
#if PRINT_CHUNK_CODE
    g_log_file << "<opt> breaking condition block."
               << std::endl;
#endif
    
    Label * const label = add_new_label(
      new Label(static_cast<LabelMIRInst *>(0), curr_mir));
    
    MIR * const first_goto_mir =
      new MIR(static_cast<typename CondCodeTranslationTraits<T_cond>::MIRType *>(0),
              add_new_label(new Label(static_cast<LabelARMNormalInst *>(0), mp_curr_arm_inst_info)));
    
    first_goto_mir->set_in_out_flags(CondCodeTranslationTraits<T_cond>::PRELOAD_STATUS_FLAG, 0);
    first_goto_mir->set_cond_block(mir->cond_block());
    
    MIR * const add_mir = new MIR(static_cast<MIRAdd *>(0),
                                  find_global_var(Variable::CR_PC),
                                  0,
                                  find_global_var(Variable::CR_PC));
    
    BasicBlock * const new_cond_bb = new BasicBlock(add_mir);
    m_bb_table.push_back(new_cond_bb);
    
    add_mir->set_bb(new_cond_bb);
    add_mir->set_cond_block(mir->cond_block());
    
    MIR * const second_goto_mir = new MIR(static_cast<MIRGoto *>(0), label);
    
    second_goto_mir->set_bb(new_cond_bb);
    second_goto_mir->set_cond_block(mir->cond_block());
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // Insert that new MIRGotoXX instruction into this chunk.
    if (mir->prev_mir()->bb() == mir->bb())
    {
      // :NOTE: Wei 2004-Nov-5:
      //
      // We are breaking a basic block,
      // thus the new MIRGotoXX instruction will follow the previous MIR's
      // basic block, however, we have to create a new basic block for 
      // the following MIRs.
      first_goto_mir->set_bb(mir->bb());
      
      BasicBlock * const new_bb = new BasicBlock(mir);
      m_bb_table.push_back(new_bb);
      
      // :NOTE: Wei 2004-Nov-4:
      //
      // Remember the old basic block first.
      BasicBlock * const old_bb = mir->bb();
      
      // :NOTE: Wei 2004-Nov-5:
      //
      // And set the new basic block later.
      mir->set_bb(new_bb);
      
      // :NOTE: Wei 2004-Nov-4:
      //
      // Change the basic block pointers of the following MIRs.
      MIR *curr_mir = mir->next_mir();
      
      while (curr_mir->bb() == old_bb)
      {
        curr_mir->set_bb(new_bb);
        
        curr_mir = curr_mir->next_mir();
        
        if (0 == curr_mir)
        {
          break;
        }
      }
    }
    else
    {
      // :NOTE: Wei 2004-Nov-5:
      //
      // We are insert a new MIR in the basic block boundry,
      // thus this new MIRGotoXX instruction will be in its own newly created
      // basic block.
      BasicBlock * const new_bb = new BasicBlock(first_goto_mir);
      m_bb_table.push_back(new_bb);
      
      first_goto_mir->set_bb(new_bb);
    }
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // Insert the newly created MIRs into the MIR link list.
    first_goto_mir->set_prev_mir(mir->prev_mir());
    mir->prev_mir()->set_next_mir(first_goto_mir);
    
    first_goto_mir->set_next_mir(add_mir);
    
    add_mir->set_prev_mir(first_goto_mir);
    add_mir->set_next_mir(second_goto_mir);
    
    second_goto_mir->set_prev_mir(add_mir);
    second_goto_mir->set_next_mir(mir);
    
    mir->set_prev_mir(second_goto_mir);
    
    // :NOTE: Wei 2005-May-26:
    //
    // Becuase I don't apply append_mir() to this MIR,
    // thus I have to manually add this mir into m_all_mir.
    m_all_mir.push_back(first_goto_mir);
    m_all_mir.push_back(add_mir);
    m_all_mir.push_back(second_goto_mir);
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // Return the newly created MIR.
    return first_goto_mir;
  }
#endif
}
