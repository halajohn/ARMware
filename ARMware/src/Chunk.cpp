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

// :NOTE: Wei 2005-May-13:
//
// The optimizations I have already made are:
//
// * redundant condition code calculation elimination
// * conditional execuating instruction grouping
// * redundant jump elimination
// * dead code elimination
// * constant folding
// * LOCAL & GLOBAL Common Subexpression Elimination
// * redundany memory operation elimination
// * algebraic canonicalization
// * SSA form based linear scan register allocation

// :TODO: Wei 2005-May-24:
//
// The future plan should include:
//
// * Partial redundancy elimination
//
// * Consider in what situation the OS will change the PAGE TABLE,
//   if I can figure out the exact time, can I compile the whole ARM codes into
//   native codes? including the ARM Load & Store instructions (Ex: LSM, LDM... etc)
//
// * Redesign the whole MIR and LIR.
//   Because I do the SSA transformation and value numbering in the LIR form,
//   however, I think these 2 or more optimization should be preform in the MIR form.
//   I should cut complex MIR instruction into more simple MIR operation, and preform
//   those optimization technology. And try to maintain the SSA form after each optimization.

#if ENABLE_DYNAMIC_TRANSLATOR
#include <algorithm>
#include <queue>
#endif

#include "Log.hpp"
#include "ARMware.hpp"
#include "Chunk.hpp"
#include "Core.hpp"
#include "Compiler/MIR.hpp"
#include "Compiler/BasicBlock.hpp"
#include "Compiler/DAGNode.hpp"
#include "Compiler/ConditionBlock.hpp"
#include "Compiler/BitPosition.hpp"
#include "Compiler/LiveInterval.hpp"
#include "Compiler/Tuple.hpp"
#include "Compiler/x86/LIR.hpp"
#include "Compiler/x86/Register.hpp"
#include "Compiler/x86/MachineDep.hpp"
#include "Compiler/x86/NativeEmitter.hpp"

#include "Utils/Number.hpp"

#include "Compiler/BasicBlock.tcc"
#include "Compiler/Variable.tcc"
#include "Compiler/LiveInterval.tcc"
#include "Compiler/x86/LIR.tcc"
#include "Chunk.tcc"

namespace ARMware
{
#if ENABLE_DYNAMIC_TRANSLATOR || ENABLE_THREADED_CODE
  inline void
  Chunk::clean_tc_buffer()
  {
    if (mp_tc_buffer != 0)
    {
      switch (m_length)
      {
      case   0:
      case   1:
      case   2:
      case   3:
      case   4:
      case   5:
      case   6:
      case   7:
      case   8:
      case   9:
        ARMInstInfo_10::operator delete(mp_tc_buffer);
        break;
        
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
      case 16:
      case 17:
      case 18:
      case 19:
        ARMInstInfo_20::operator delete(mp_tc_buffer);
        break;

      case 20:
      case 21:
      case 22:
      case 23:
      case 24:
      case 25:
      case 26:
      case 27:
      case 28:
      case 29:
        ARMInstInfo_30::operator delete(mp_tc_buffer);
        break;

      case 30:
      case 31:
      case 32:
      case 33:
      case 34:
      case 35:
      case 36:
      case 37:
      case 38:
      case 39:
        ARMInstInfo_40::operator delete(mp_tc_buffer);
        break;

      case 40:
      case 41:
      case 42:
      case 43:
      case 44:
      case 45:
      case 46:
      case 47:
      case 48:
      case 49:
        ARMInstInfo_50::operator delete(mp_tc_buffer);
        break;

      case 50:
      case 51:
      case 52:
      case 53:
      case 54:
      case 55:
      case 56:
      case 57:
      case 58:
      case 59:
        ARMInstInfo_60::operator delete(mp_tc_buffer);
        break;

      case 60:
      case 61:
      case 62:
      case 63:
      case 64:
      case 65:
      case 66:
      case 67:
      case 68:
      case 69:
        ARMInstInfo_70::operator delete(mp_tc_buffer);
        break;

      case 70:
      case 71:
      case 72:
      case 73:
      case 74:
      case 75:
      case 76:
      case 77:
      case 78:
      case 79:
        ARMInstInfo_80::operator delete(mp_tc_buffer);
        break;

      case 80:
      case 81:
      case 82:
      case 83:
      case 84:
      case 85:
      case 86:
      case 87:
      case 88:
      case 89:
        ARMInstInfo_90::operator delete(mp_tc_buffer);
        break;

      case 90:
      case 91:
      case 92:
      case 93:
      case 94:
      case 95:
      case 96:
      case 97:
      case 98:
      case 99:
        ARMInstInfo_100::operator delete(mp_tc_buffer);
        break;

      case 100:
      case 101:
      case 102:
      case 103:
      case 104:
      case 105:
      case 106:
      case 107:
      case 108:
      case 109:
        ARMInstInfo_110::operator delete(mp_tc_buffer);
        break;

      case 110:
      case 111:
      case 112:
      case 113:
      case 114:
      case 115:
      case 116:
      case 117:
      case 118:
      case 119:
        ARMInstInfo_120::operator delete(mp_tc_buffer);
        break;

      case 120:
      case 121:
      case 122:
      case 123:
      case 124:
      case 125:
      case 126:
      case 127:
      case 128:
      case 129:
        ARMInstInfo_130::operator delete(mp_tc_buffer);
        break;

      case 130:
      case 131:
      case 132:
      case 133:
      case 134:
      case 135:
      case 136:
      case 137:
      case 138:
      case 139:
        ARMInstInfo_140::operator delete(mp_tc_buffer);
        break;

      case 140:
      case 141:
      case 142:
      case 143:
      case 144:
      case 145:
      case 146:
      case 147:
      case 148:
      case 149:
        ARMInstInfo_150::operator delete(mp_tc_buffer);
        break;

      case 150:
      case 151:
      case 152:
      case 153:
      case 154:
      case 155:
      case 156:
      case 157:
      case 158:
      case 159:
        ARMInstInfo_160::operator delete(mp_tc_buffer);
        break;

      case 160:
      case 161:
      case 162:
      case 163:
      case 164:
      case 165:
      case 166:
      case 167:
      case 168:
      case 169:
        ARMInstInfo_170::operator delete(mp_tc_buffer);
        break;

      case 170:
      case 171:
      case 172:
      case 173:
      case 174:
      case 175:
      case 176:
      case 177:
      case 178:
      case 179:
        ARMInstInfo_180::operator delete(mp_tc_buffer);
        break;

      case 180:
      case 181:
      case 182:
      case 183:
      case 184:
      case 185:
      case 186:
      case 187:
      case 188:
      case 189:
        ARMInstInfo_190::operator delete(mp_tc_buffer);
        break;

      case 190:
      case 191:
      case 192:
      case 193:
      case 194:
      case 195:
      case 196:
      case 197:
      case 198:
      case 199:
        ARMInstInfo_200::operator delete(mp_tc_buffer);
        break;
        
      default:
        ::operator delete(mp_tc_buffer);
        break;
      }
    }
  }
  
  //////////////////////////////////// Public /////////////////////////////////
  //================================ Life cycle ===============================
  
  Chunk::~Chunk()
  {
    // :NOTE: Wei 2004-Aug-12:
    //
    // Clean up the threaded code buffer.
    
    clean_tc_buffer();
  }
#endif

#if ENABLE_DYNAMIC_TRANSLATOR
  // Global object pointer
  
  Chunk *gp_chunk;
  
  ///////////////////////////// Private ///////////////////////////////////////
  //========================== Operation ======================================
  
  template<InstCond T_cond>
  inline void
  Chunk::check_and_break_cond_block(Label * const label, bool &finish)
  {
    MIR * const temp = break_cond_block<T_cond>(label->mir());
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // If we insert a new MIRGotoXX before the jump target MIR,
    // then the real jump target MIR should be the new MIRGotoXX,
    // thus we will update the jump target MIR, too.
    if (temp != label->mir())
    {
      label->set_mir_inst(temp);
      
      finish = true;
    }
  }
  
  inline void
  Chunk::back_patching_pending_label()
  {
    Label *curr_pending = mp_pending_back_patching_label_head;
    
    if (curr_pending != 0)
    {
      // :NOTE: Wei 2004-Oct-05:
      //
      // If there are any label needs to be back-patched.
      
      // :NOTE: Wei 2004-Oct-05:
      //
      // Mark 'mp_pending_tail' is the last one.
      mp_pending_back_patching_label_tail->set_next_need_back_patch_label(0);
      
      while (curr_pending != 0)
      {
        bool finish = false;
        
        // :NOTE: Wei 2004-Nov-5:
        //
        // We only need to back-patch the labels with LabelARMInst type.
        assert((Label::ARM_FIRST_INST == curr_pending->kind()) ||
               (Label::ARM_NORMAL_INST == curr_pending->kind()));
        
        curr_pending->back_patch_to_mir_inst();
        
        // :NOTE: Wei 2004-Nov-4:
        //
        // Only the branch target needs to check whether we need to insert
        // a Goto_xx MIR instruction in the middle of a condition block.
        if (true == curr_pending->whether_need_to_check_cond_block())
        {
          if (curr_pending->mir()->cond_block() != 0)
          {
            switch (curr_pending->mir()->cond_block()->cond())
            {
            case EQ: check_and_break_cond_block<EQ>(curr_pending, finish); break;
            case NE: check_and_break_cond_block<NE>(curr_pending, finish); break;
            case CS: check_and_break_cond_block<CS>(curr_pending, finish); break;
            case CC: check_and_break_cond_block<CC>(curr_pending, finish); break;
            case MI: check_and_break_cond_block<MI>(curr_pending, finish); break;
            case PL: check_and_break_cond_block<PL>(curr_pending, finish); break;
            case VS: check_and_break_cond_block<VS>(curr_pending, finish); break;
            case VC: check_and_break_cond_block<VC>(curr_pending, finish); break;
            case HI: check_and_break_cond_block<HI>(curr_pending, finish); break;
            case LS: check_and_break_cond_block<LS>(curr_pending, finish); break;
            case GE: check_and_break_cond_block<GE>(curr_pending, finish); break;
            case LT: check_and_break_cond_block<LT>(curr_pending, finish); break;
            case GT: check_and_break_cond_block<GT>(curr_pending, finish); break;
            case LE: check_and_break_cond_block<LE>(curr_pending, finish); break;
              
            case AL:
              assert(!"We should not have a AL condition block.");
              break;
              
            case NV:
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
        }
        
        if (false == finish)
        {
          // :NOTE: Wei 2004-Nov-5:
          //
          // I don't need to consider the condition block,
          // that is to say, I don't need to consider inserting a new MIR instruction
          // into the current chunk.
          //
          // However, I have to consider whether I need to create a new basic block
          // on the jump target.
          
          // :NOTE: Wei 2004-Nov-5:
          //
          // If the jump target is the first MIR, thus there is a basic block
          // already.
          if ((mp_mir_head != curr_pending->mir()) &&
              (curr_pending->mir()->prev_mir()->bb() == curr_pending->mir()->bb()))
          {
            // :NOTE: Wei 2004-Nov-5:
            //
            // I need to create a new basic block.
            
            // :NOTE: Wei 2004-Nov-4:
            //
            // Remember the old basic block first.
            BasicBlock * const old_bb = curr_pending->mir()->bb();
            
            // :NOTE: Wei 2004-Nov-3:
            //
            // Start a new basic block.
            BasicBlock * const new_bb = new BasicBlock(curr_pending->mir());
            m_bb_table.push_back(new_bb);
            
            curr_pending->mir()->set_bb(new_bb);
            
            // :NOTE: Wei 2004-Nov-4:
            //
            // Change the basic block pointers of the following MIRs.
            MIR *curr_mir = curr_pending->mir()->next_mir();
            
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
        }
        
        curr_pending = curr_pending->next_back_patch_label();
      }
    }
  }
  
  inline void
  Chunk::init_dynamic_compiler()
  {
    assert(0 == mp_mir_head);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Reset the current ARM inst to the first one.
    mp_curr_arm_inst_info = mp_first_arm_inst_info;
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Used in deciding whether we are finish translation or not.
    m_arm_idx = 0;
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Trigger the examining of the condition codes and condition blocks.
    m_flags_no_change = false;
    
    // :NOTE: Wei 2004-Aug-31:
    //
    // Treat the first instruction as a leader of a basic block.
    m_next_mir_is_leader = true;
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // We don't have any label that are the target of the Goto_xx MIR
    // instruction which are the first one in a condition block.
    mp_cond_block_back_patching_label = 0;
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // We don't have any condition block for now.
    mp_cond_block = 0;
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Initialize the link list of the back patching labels.
    mp_pending_back_patching_label_head = 0;
    
    mp_worklist_bb = 0;
    
#if PRINT_CHUNK_CODE
    BasicBlock::BB_SEQUENCE_IDX = 0;
#endif
  }
  
  inline void
  Chunk::back_patch_cond_block_back_patching_add_mir()
  {
    assert(0 == mp_cond_block_back_patching_add_mir->operand_variable().back());
    
    uint32_t const arm_inst_interval =
      static_cast<uint32_t>(mp_curr_arm_inst_info -
                            mp_cond_block_back_patching_add_mir->belong_to_which_arm_inst());
    
    // :NOTE: Wei 2005-Apr-2:
    //
    // The reason why I multiply 4 is because each ARM instruction occupies 4 bytes,
    // thus the PC should be incremented by 4, 8, 12, ...
    Variable * const cond_block_length_var = find_const_var(arm_inst_interval << 2);
    
    mp_cond_block_back_patching_add_mir->set_operand_variable(1, cond_block_length_var);
  }
  
  inline void
  Chunk::translate_chunk_to_MIR()
  {
    init_dynamic_compiler();
    
    do
    {
#if PRINT_CHUNK_CODE
      // :NOTE: Wei 2004-Oct-30:
      //
      // Print the original ARM codes.
      g_log_file << std::dec
                 << m_arm_idx
                 << ": ";
      (mp_curr_arm_inst_info->func_PRINT())(mp_curr_arm_inst_info->inst());
#endif
      
#if CHECK_CHUNK_CODE
      m_finish_setting_cond_block = false;
#endif
      
      switch (mp_curr_arm_inst_info->inst().mRawData & 0xF0000000)
      {
      case EQ: translate_cond_code<EQ>(); break;
      case NE: translate_cond_code<NE>(); break;
      case CS: translate_cond_code<CS>(); break;
      case CC: translate_cond_code<CC>(); break;
      case MI: translate_cond_code<MI>(); break;
      case PL: translate_cond_code<PL>(); break;
      case VS: translate_cond_code<VS>(); break;
      case VC: translate_cond_code<VC>(); break;
      case HI: translate_cond_code<HI>(); break;
      case LS: translate_cond_code<LS>(); break;
      case GE: translate_cond_code<GE>(); break;
      case LT: translate_cond_code<LT>(); break;
      case GT: translate_cond_code<GT>(); break;
      case LE: translate_cond_code<LE>(); break;
      case AL: translate_cond_code<AL>(); break;
        
      case NV:
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2
        continue;
#elif CONFIG_ARCH_V3 || CONFIG_ARCH_V4
        assert(!"Should not reach here.");
#elif CONFIG_ARCH_V5
        // :TODO: Wei 2004-Sep-19:
        //
        // see Core::pass_cond_nv()
        assert(!"Should not reach here.");
#endif
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      (mp_curr_arm_inst_info->func_DT())(mp_curr_arm_inst_info->inst(),
                                         ((m_arm_idx + 1) == m_length) ? true : false);
      
      ++mp_curr_arm_inst_info;
      ++m_arm_idx;
    }
    while (m_arm_idx != m_length);
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // If there is still one condition block needs to be closed,
    // then we will add another new MIRReturn instruction at the end of this
    // chunk.
    if (mp_cond_block_back_patching_label != 0)
    {
      // :NOTE: Wei 2004-Oct-04:
      //
      // If this chunk is ended by the normal situation -
      // execuating all instructions in this chunk.
      //
      // Then this chunk will return 'ER_NORMAL'.
      MIR * const mir = new MIR(static_cast<MIRReturn *>(0), find_const_var(ER_NORMAL));
      
      // :NOTE: Wei 2005-May-26:
      //
      // Becuase I don't apply append_mir() to this MIR,
      // thus I have to manually add this mir into m_all_mir.
      m_all_mir.push_back(mir);
      
      // :NOTE: Wei 2004-Nov-4:
      //
      // This new MIR should not belong to any condition block.
      mir->set_cond_block(0);
      
      // :NOTE: Wei 2004-Nov-4:
      //
      // Because the end of the original chunk should be a MIRReturn
      // instruction, thus this new MIRReturn instruction should
      // belongs to its own newly created basic block.
      BasicBlock * const bb = new BasicBlock(mir);
      m_bb_table.push_back(bb);
      
      mir->set_bb(bb);
      
      // :NOTE: Wei 2004-Nov-4:
      //
      // Add to the end of the MIR link lists.
      mir->set_prev_mir(mp_mir_tail);
      mp_mir_tail->set_next_mir(mir);
      
      mp_mir_tail = mir;
      
      // :NOTE: Wei 2004-Nov-5:
      //
      // Patch the label which it needs to.
      mp_cond_block_back_patching_label->reset_to_mir_inst(mir);
      
      // :NOTE: Wei 2005-Apr-2:
      //
      // Patch the condition block addition
      back_patch_cond_block_back_patching_add_mir();
    }
    
    // :NOTE: Wei 2004-Oct-05:
    //
    // Indicate the end.
    mp_mir_tail->set_next_mir(0);
    
    back_patching_pending_label();
  }
  
  namespace
  {
    // :NOTE: Wei 2005-May-13:
    //
    // Jump to jump: Convert this to a single jump
    //
    //           goto 100
    //           ...
    //    100    goto 300
    //
    // transfers to
    //
    //           goto 300
    template<MIRKindEnum T_kind>
    inline MIR *
    find_label_real_target_mir(MIR * const mir)
    {
      assert(T_kind == mir->kind());
      
      Label *curr_label = mir->label<T_kind>();
      
      assert(Label::MIR_INST == curr_label->kind());
      
      MIR *target_mir = curr_label->mir();
      
#if PRINT_CHUNK_CODE
      MIR *orig_mir = target_mir;
#endif
      
      while (1)
      {
        if (T_kind == target_mir->kind())
        {
          curr_label = target_mir->label<T_kind>();
          
          assert(Label::MIR_INST == curr_label->kind());
          
          target_mir = curr_label->mir();
        }
        else
        {
          break;
        }
      }
      
#if PRINT_CHUNK_CODE
      if (orig_mir != target_mir)
      {
        g_log_file << "<opt> eliminate redundant jump: "
                   << mir->idx()
                   << ", "
                   << orig_mir->idx()
                   << "->"
                   << target_mir->idx()
                   << std::endl;
      }
#endif
      
      return target_mir;
    }
    
    template<MIRKindEnum T_kind>
    inline void
    linking_goto_type_mir(BasicBlock * const prev_bb,
                          BasicBlock * const new_bb,
                          MIR * const prev_mir)
    {
      assert(Label::MIR_INST == prev_mir->label<T_kind>()->kind());
      
#if CHECK_CHUNK_CODE
      switch (T_kind)
      {
      case MIR_GOTO:
        assert(0 == prev_mir->in_flags());
        break;
        
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
        assert(prev_mir->in_flags() != 0);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
#endif
      
      MIR * const target_mir = find_label_real_target_mir<T_kind>(prev_mir);
      
      prev_mir->label<T_kind>()->set_mir_inst(target_mir);
      
      switch (T_kind)
      {
      case MIR_GOTO:
        prev_bb->add_succ_bb(false, prev_mir->label<T_kind>()->mir()->bb());
        break;
        
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
        prev_bb->add_succ_true_false_bb(prev_mir->label<T_kind>()->mir()->bb(),
                                        new_bb);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
  }
  
  void
  Chunk::eliminate_redundant_cond_code_calculation_and_link_bb()
  {
    assert(mp_mir_head != 0);
    
    MIR *curr_mir = mp_mir_head;
    MIR *prev_mir;
    
    BasicBlock *prev_bb = 0;
    BasicBlock *new_bb;
    
    mp_root_bb = mp_mir_head->bb();
    
    // :NOTE: Wei 2005-Apr-27:
    //
    // For now, I don't think about what will happened when the last MIR instruction involves
    // the condition code calculations, thus I will raise an assert for now.
    //
    // If the last MIR instuction may involve the condition code calculations, then
    // I have to think about it, and change some codes maybe, and delete the assertion below.
    assert(0 == (mp_mir_tail->in_flags() | mp_mir_tail->out_flags()));
    
    do
    {
      new_bb = curr_mir->bb();
      prev_mir = curr_mir->prev_mir();
      
      if ((0 == prev_mir) || (prev_mir->bb() != new_bb))
      {
        // :NOTE: Wei 2004-Nov-6:
        //
        // Starting a new basic block.
        assert(curr_mir->bb()->mir_head() == curr_mir);
        
        if (prev_bb != 0)
        {
          // :NOTE: Wei 2004-Nov-6:
          //
          // We have an old basic block, thus preform the basic block linking.
          prev_bb->set_mir_tail(prev_mir);
          
          // :NOTE: Wei 2005-May-03:
          //
          // Mark the end of MIR instructions of a basic block.
          prev_bb->mir_tail()->set_next_mir(0);
          
          switch (prev_mir->kind())
          {
          case MIR_CALL_0:
          case MIR_CALL_1:
            prev_bb->add_succ_bb(true, new_bb);
            break;
            
          case MIR_RETURN:
            break;
            
          case MIR_CMP_EQ:
            assert(Label::MIR_INST == prev_mir->label<MIR_CMP_EQ>()->kind());
            
            prev_bb->add_succ_true_false_bb(
              prev_mir->label<MIR_CMP_EQ>()->mir()->bb(),
              new_bb);
            break;
            
          case MIR_GOTO: linking_goto_type_mir<MIR_GOTO>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_EQ: linking_goto_type_mir<MIR_GOTO_EQ>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_NE: linking_goto_type_mir<MIR_GOTO_NE>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_CS: linking_goto_type_mir<MIR_GOTO_CS>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_CC: linking_goto_type_mir<MIR_GOTO_CC>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_MI: linking_goto_type_mir<MIR_GOTO_MI>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_PL: linking_goto_type_mir<MIR_GOTO_PL>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_VS: linking_goto_type_mir<MIR_GOTO_VS>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_VC: linking_goto_type_mir<MIR_GOTO_VC>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_HI: linking_goto_type_mir<MIR_GOTO_HI>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_LS: linking_goto_type_mir<MIR_GOTO_LS>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_GE: linking_goto_type_mir<MIR_GOTO_GE>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_LT: linking_goto_type_mir<MIR_GOTO_LT>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_GT: linking_goto_type_mir<MIR_GOTO_GT>(prev_bb, new_bb, prev_mir); break;
          case MIR_GOTO_LE: linking_goto_type_mir<MIR_GOTO_LE>(prev_bb, new_bb, prev_mir); break;
            
          case MIR_JUMP:
            {
              prev_bb->add_assistant_bb(prev_mir->label<MIR_JUMP>()->mir()->bb());
              
              std::vector<Label *> const * const jump_table = prev_mir->jump_target_table();
              
              for (std::vector<Label *>::const_iterator iter = (*jump_table).begin();
                   iter != (*jump_table).end();
                   ++iter)
              {
                prev_bb->add_succ_bb(false, (*iter)->mir()->bb());
              }
            }
            break;
            
          case MIR_DATA32:
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
            prev_bb->add_succ_bb(true, new_bb);
            break;
            
          case MIR_LOAD_LABEL:
          case MIR_LOAD:
            // :NOTE: Wei 2005-Mar-22:
            //
            // The possibility of these MIR instructions been the last one of a basic block is 0.
          default:
            assert(!"Should not reach here.");
            break;
          }
          
#if PRINT_CHUNK_CODE
          g_log_file << prev_bb->sequence_idx()
                     << ": ";
          
          prev_bb->dump_info();
#endif
        }
        
        prev_bb = new_bb;
        
        mp_n_bit_mir = 0;
        mp_z_bit_mir = 0;
        mp_c_bit_mir = 0;
        mp_v_bit_mir = 0;
      }
      
      // :NOTE: Wei 2004-Nov-7:
      //
      // Eliminate redundant condition code calculation.
      switch (curr_mir->in_flags())
      {
      case SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
        mp_n_bit_mir = 0;
        mp_z_bit_mir = 0;
        mp_c_bit_mir = 0;
        mp_v_bit_mir = 0;
        break;
        
      case SIGN_FLAG | ZERO_FLAG | CARRY_FLAG:
        mp_n_bit_mir = 0;
        mp_z_bit_mir = 0;
        mp_c_bit_mir = 0;
        break;
        
      case SIGN_FLAG | ZERO_FLAG | OVERFLOW_FLAG:
        mp_n_bit_mir = 0;
        mp_z_bit_mir = 0;
        mp_v_bit_mir = 0;
        break;
        
      case SIGN_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
        mp_n_bit_mir = 0;
        mp_c_bit_mir = 0;
        mp_v_bit_mir = 0;
        break;
        
      case ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
        mp_z_bit_mir = 0;
        mp_c_bit_mir = 0;
        mp_v_bit_mir = 0;
        break;
        
      case SIGN_FLAG | ZERO_FLAG:
        mp_n_bit_mir = 0;
        mp_z_bit_mir = 0;
        break;
        
      case SIGN_FLAG | CARRY_FLAG:
        mp_n_bit_mir = 0;
        mp_c_bit_mir = 0;
        break;
        
      case SIGN_FLAG | OVERFLOW_FLAG:
        mp_n_bit_mir = 0;
        mp_v_bit_mir = 0;
        break;
        
      case ZERO_FLAG | CARRY_FLAG:
        mp_z_bit_mir = 0;
        mp_c_bit_mir = 0;
        break;
        
      case ZERO_FLAG | OVERFLOW_FLAG:
        mp_z_bit_mir = 0;
        mp_v_bit_mir = 0;
        break;
        
      case CARRY_FLAG | OVERFLOW_FLAG:
        mp_c_bit_mir = 0;
        mp_v_bit_mir = 0;
        break;
                
      case SIGN_FLAG:
        mp_n_bit_mir = 0;
        break;
        
      case ZERO_FLAG:
        mp_z_bit_mir = 0;
        break;
        
      case CARRY_FLAG:
        mp_c_bit_mir = 0;
        break;
        
      case OVERFLOW_FLAG:
        mp_v_bit_mir = 0;
        break;
        
      case 0:
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      switch (curr_mir->out_flags())
      {
      case SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
        
      case SIGN_FLAG | ZERO_FLAG | CARRY_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        break;
        
      case SIGN_FLAG | ZERO_FLAG | OVERFLOW_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
        
      case SIGN_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
        
      case ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
        
      case SIGN_FLAG | ZERO_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        break;
        
      case SIGN_FLAG | CARRY_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        break;
        
      case SIGN_FLAG | OVERFLOW_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
        
      case ZERO_FLAG | CARRY_FLAG:
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        break;
        
      case ZERO_FLAG | OVERFLOW_FLAG:
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
        
      case CARRY_FLAG | OVERFLOW_FLAG:
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
                
      case SIGN_FLAG:
        if (mp_n_bit_mir != 0)
        {
          mp_n_bit_mir->clear_n_bit();
        }
        mp_n_bit_mir = curr_mir;
        break;
        
      case ZERO_FLAG:
        if (mp_z_bit_mir != 0)
        {
          mp_z_bit_mir->clear_z_bit();
        }
        mp_z_bit_mir = curr_mir;
        break;
        
      case CARRY_FLAG:
        if (mp_c_bit_mir != 0)
        {
          mp_c_bit_mir->clear_c_bit();
        }
        mp_c_bit_mir = curr_mir;
        break;
        
      case OVERFLOW_FLAG:
        if (mp_v_bit_mir != 0)
        {
          mp_v_bit_mir->clear_v_bit();
        }
        mp_v_bit_mir = curr_mir;
        break;
        
      case 0:
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      curr_mir = curr_mir->next_mir();
    }
    while (curr_mir != 0);
    
#if PRINT_CHUNK_CODE
    assert(prev_bb != 0);
    
    g_log_file << prev_bb->sequence_idx()
               << ": ";
    
    prev_bb->dump_info();
#endif
    
    assert(mp_mir_tail->bb() == prev_bb);
    
    switch (mp_mir_tail->kind())
    {
    case MIR_GOTO:
      linking_goto_type_mir<MIR_GOTO>(prev_bb, new_bb, mp_mir_tail); break;
      break;
      
    case MIR_RETURN:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    // :NOTE: Wei 2004-Nov-8:
    //
    // Setting the last MIR of the last basic block.
    prev_bb->set_mir_tail(mp_mir_tail);
    
    // :NOTE: Wei 2005-May-03:
    //
    // Mark the end of MIR instructions of a basic block.
    prev_bb->mir_tail()->set_next_mir(0);
  }
  
  inline void
  Chunk::cleanup_variable(Variable * const head)
  {
    Variable *curr_var = head;
    Variable *next_var;
    
    while (curr_var != 0)
    {
      assert(0 == curr_var->var_stack().size());
      
      next_var = curr_var->next_link_var();
      
      delete curr_var;
      
      curr_var = next_var;
    }
  }
  
  inline void
  Chunk::cleanup_compiler_intermedia_product()
  {
    // :NOTE: Wei 2004-Oct-20:
    //
    // Cleanup basic block.
    std::for_each(m_bb_table.begin(), m_bb_table.end(), del_fun<BasicBlock>());
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // Cleanup global variable
    cleanup_variable(mp_global_var_head);
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // Cleanup temp variable table.
    cleanup_variable(mp_temp_var_head);
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // Cleanup memory variable table.
    cleanup_variable(mp_memory_var_head);
    
    // :NOTE: Wei 2005-May-26:
    //
    // Cleanup const variable hash table.
    m_const_table.flush_all();
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // Cleanup MIR list.
    std::for_each(m_all_mir.begin(), m_all_mir.end(), del_fun<MIR>());
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // Cleanup label.
    std::for_each(m_label_table.begin(), m_label_table.end(), del_fun<Label>());
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Cleanup condition block.
    std::for_each(m_cond_block_table.begin(), m_cond_block_table.end(), del_fun<ConditionBlock>());
    
    // :NOTE: Wei 2005-May-22:
    //
    // Cleanup operation table.
    for (uint32_t i = 0; i != OPERATION_TABLE_SIZE; ++i)
    {
      for (std::list<Tuple *>::const_iterator iter = m_operation_table[i].begin();
           iter != m_operation_table[i].end();
           ++iter)
      {
        delete (*iter);
      }
    }
    
    // :NOTE: Wei 2005-May-26:
    //
    // Cleanup threaded code buffer, because I don't need it at all.
    clean_tc_buffer();
    
    mp_tc_buffer = 0;
  }
  
  // :NOTE: Wei 2004-Oct-18:
  //
  // The algorithms to build dominator tree:
  //
  // (1) Purdom-Moore [1972]:
  //
  // for all v in (V - r) do
  //    remove v from G
  //    R(v) = unreachable vertices
  //    for all u in R(v) do
  //       Dom(u) = Dom(u) | {v}
  //    done
  // done
  //
  // This algorithm is very simple.
  // We remove a node, A, from the graph at a time,
  // and to see whether there are nodes become unreachable from the rood node.
  // If such a node, B, exists, then the dominator of node B will include
  // node A.
  // Because we have to go through node A to reach node B, otherwise,
  // node B will unable to reach (i.e. unreachable).
  //
  // Disadvantages: very slow in pratice.
  //
  // (2) Allen and Cocke [1972]:
  //
  // Disadvantages: very slow in pratice (but better than Purdom-Moore).
  // 
  // (3) Lengauer-Tarjan [1979]:
  //
  // It is a too complex algorithm to describe accurately here.
  // (Shame... I don't understand it very well, too)
  //
  // Tarjan defines a quantity called the semi-dominator and computes
  // these value in a bottom-up walk of the depth-first search tree.
  // Having these values, he can easily compute the actual dominators.
  
  // :NOTE: Wei 2004-Oct-19:
  //
  // Use Lengauer-Tarjan's sophisticated algorithm to compute the dominator tree.
  
  inline void
  Chunk::build_dfs_spanning_tree(BasicBlock * const bb)
  {
    // :NOTE: Wei 2004-Oct-19:
    //
    // Depth first number is counted from 1.
    //
    // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
    //
    // semi(v) := n := n + 1;
    bb->set_df_number(++m_df_number);
    
    // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
    //
    // vertex(n) := v;
    m_vertex.push_back(bb);
    
    bb->set_dfs_prev_number_bb(mp_prev_dfs_searching_bb);
    
    mp_prev_dfs_searching_bb = bb;
    
    // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
    //
    // for each w belongs to succ(v) do
    std::vector<BasicBlock::SuccBBInfo> &succ = bb->succ_bb();
    
    for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = succ.begin();
         iter != succ.end();
         ++iter)
    {
      // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
      //
      // if semi(w) = 0 then
      if (0 == (*iter).bb()->df_number())
      {
        // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
        //
        // parent(w) := v;
        (*iter).bb()->set_dfs_parent(bb);
        
        // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
        //
        // DFS(w);
        build_dfs_spanning_tree((*iter).bb());
      }
      // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
      //
      // fi
      
      // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
      //
      // add v to pred(w);
      (*iter).bb()->add_pred_bb(bb);
    }
    // :Lengauer-Tarjan [1979]: p.128: Wei 2004-Oct-20:
    //
    // od
  }
  
  inline void
  Chunk::compress(uint32_t const v)
  {
    // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
    //
    // comment this procedure assumes ancestor(v) != 0
    assert(m_ancestor[v] != 0);
    
    // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
    //
    // if ancestor(ancestor(v)) != 0 then
    if (m_ancestor[m_ancestor[v]] != 0)
    {
      // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
      //
      // compress(ancestor(v));
      compress(m_ancestor[v]);
      
      // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
      //
      // if semi(label(ancestor(v))) < semi(label(v)) then
      if (m_semi_dominator[m_label[m_ancestor[v]]] <
          m_semi_dominator[m_label[v]])
      {
        // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
        //
        // label(v) := label(ancestor(v)) fi;
        m_label[v] = m_label[m_ancestor[v]];
      }
      
      // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
      //
      // ancestor(v) := ancestor(ancestor(v)) fi;
      m_ancestor[v] = m_ancestor[m_ancestor[v]];
    }
  }
  
  inline uint32_t
  Chunk::eval(uint32_t const v)
  {
#if USE_BALANCE_DOM_ALGO
    // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
    //
    // if ancestor(v) = 0 then
    //   return v;
    // else
    //   compress(v);
    //   return label(v);
    // fi
    if (0 == m_ancestor[v])
    {
      assert((v <= m_df_number) && (v >= 1));
      
      return v;
    }
    else
    {
      compress(v);
      
      assert((m_label[v] <= m_df_number) && (m_label[v] >= 1));
      
      return m_label[v];
    }
#else
    // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
    //
    // if ancestor(v) = 0 then
    if (0 == m_ancestor[v])
    {
      assert((m_label[v] <= m_df_number) && (m_label[v] >= 1));
      
      // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
      //
      // return label(v);
      return m_label[v];
    }
    else
    {
      // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
      //
      // else
      //   compress(v);
      compress(v);
      
      // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
      //
      //   if semi(label(ancestor(v))) >= semi(label(v)) then
      if (m_semi_dominator[m_label[m_ancestor[v]]] >=
          m_semi_dominator[m_label[v]])
      {
        assert((m_label[v] <= m_df_number) && (m_label[v] >= 1));
        
        // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
        //
        // return label(v);
        return m_label[v];
      }
      else
      {
        assert((m_label[m_ancestor[v]] <= m_df_number) &&
               (m_label[m_ancestor[v]] >= 1));
        
        // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
        //
        // else
        //   return label(ancestor(v)) fi fi;
        return m_label[m_ancestor[v]];
      }
    }
#endif
  }
  
  inline void
  Chunk::link(uint32_t const v, uint32_t const w)
  {
#if USE_BALANCE_DOM_ALGO
    // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
    //
    // ancestor(w) := v;
    m_ancestor[w] = v;
#else
    // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
    //
    // comment this procedure assumes for convenience that
    // size(0) = label(0) = semi(0) = 0;
    assert(0 == m_size[0]);
    assert(0 == m_label[0]);
    assert(0 == m_semi_dominator[0]);
    
    // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
    //
    // s := w
    uint32_t s = w;
    
    // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
    //
    // while semi(label(w)) < semi(label(child(s))) do
    while (m_semi_dominator[m_label[w]] <
           m_semi_dominator[m_label[m_child[s]]])
    {
      // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
      //
      // if size(s) + size(child(child(s))) >= 2 * size(child(s)) then
      if (m_size[s] + m_size[m_child[m_child[s]]] >=
          (2 * m_size[m_child[s]]))
      {
        // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
        //
        // ancestor(child(s)) := s; <-- original 'parent(child(s)) := s' is wrong.
        // child(s) := child(child(s))
        m_ancestor[m_child[s]] = s;
        m_child[s] = m_child[m_child[s]];
      }
      else
      {
        // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
        //
        // else
        //   size(child(s)) := size(s);
        //   s := ancestor(s) := child(s) fi od; <-- original 's := parent(s) := child(s) fi od;' is wrong.
        m_size[m_child[s]] = m_size[s];
        m_ancestor[s] = m_child[s];
        s = m_ancestor[s];
      }
    }
    
    // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
    //
    // label(s) := label(w);
    m_label[s] = m_label[w];
    
    // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
    //
    // size(v) := size(v) + size(w);
    m_size[v] += m_size[w];
    
    // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
    //
    // if size(v) < 2 * size(w) then
    if (m_size[v] < (2 * m_size[w]))
    {
      // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
      //
      // s, child(v) := child(v), s fi;
      uint32_t const temp = s;
      
      s = m_child[v];
      m_child[v] = temp;
    }
    
    // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
    //
    // while s != 0 do
    while (s != 0)
    {
      // :Lengauer-Tarjan [1979]: p.132: Wei 2004-Oct-20:
      //
      // ancestor(s) := v; <-- original 'parent(s) := v' is wrong.
      // s := child(s) od
      m_ancestor[s] = v;
      s = m_child[s];
    }
#endif
  }
  
  inline void
  Chunk::build_dominator_tree()
  {
    // :NOTE: Wei 2004-Oct-19:
    //
    // Reset 'm_df_number' to 0.
    m_df_number = 0;
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // We count the df number from 1, thus we need to set the first element
    // of m_vertex to 0.
    m_vertex.push_back(0);
    
    // :NOTE: Wei 2004-Oct-19:
    //
    // Indicate this is the root node, thus it's number is 1,
    // and the root node doesn't have any previous nodes whose number is
    // smaller than the root node.
    mp_prev_dfs_searching_bb = 0;
    
    build_dfs_spanning_tree(mp_root_bb);
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // Initialize some arrays in the Chunk class.
    //
    // At this point, 'm_df_number' will be the number of nodes in the
    // depth-first spanning tree.
    //
    // However, because the df number is counted from 1,
    // thus we need to allocate 'm_df_number + 1' elements for the following
    // arrays.
    m_semi_dominator.resize(m_df_number + 1);
    m_dominator.resize(m_df_number + 1);
    m_ancestor.resize(m_df_number + 1);
    m_label.resize(m_df_number + 1);
    m_parent.resize(m_df_number + 1);
    m_child.resize(m_df_number + 1);
    m_size.resize(m_df_number + 1);
    
    // :Lengauer-Tarjan [1979]: p.127: Wei 2004-Oct-20:
    //
    // After w is numbered but before its semidominator is computed,
    // semi(v) is the number of w.
    //
    // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
    //
    // Initially ancestor(v} = 0 and label(v) = v for each vertex v.
    //
    // :Lengauer-Tarjan [1979]: p.131: Wei 2004-Oct-20:
    //
    // Initially size(v) = 1 and child(v) = 0 for all vertices v.
    
    uint32_t i = 0;
    
    for (std::vector<uint32_t>::iterator semi_iter = m_semi_dominator.begin(),
           label_iter = m_label.begin();
         semi_iter != m_semi_dominator.end();
         ++semi_iter, ++label_iter)
    {
      (*semi_iter) = i;
      (*label_iter) = i;
      
      ++i;
    }
    
    for (std::vector<uint32_t>::iterator ancestor_iter = m_ancestor.begin(),
           child_iter = m_child.begin(),
           size_iter = m_size.begin();
         ancestor_iter != m_ancestor.end();
         ++ancestor_iter, ++child_iter, ++size_iter)
    {
      (*ancestor_iter) = 0;
      (*child_iter) = 0;
      (*size_iter) = 1;
    }
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // We need m_size[0] == 0, see Lengauer-Tarjan [1979], page 132,
    // the comment of the LINK procedure.
    m_size[0] = 0;
    
    std::vector<uint32_t>::iterator parent_iter = m_parent.begin();
    std::vector<BasicBlock *>::iterator vertex_iter = m_vertex.begin();
    
    if (m_df_number > 1)
    {
      // :NOTE: Wei 2004-Oct-20:
      //
      // The df number is started from index 1.
      // However, the root node doesn't have a parent in a depth-first
      // search spanning tree.
      //
      // Thus I start from index 2.
      parent_iter += 2;
      vertex_iter += 2;
      
      do
      {
        (*parent_iter) = (*vertex_iter)->dfs_parent()->df_number();
        
        ++parent_iter;
        ++vertex_iter;
      }
      while (parent_iter != m_parent.end());
    }
    
    // :NOTE: Wei 2004-Oct-20:
    //
    // Indicate the root node doesn't have a parent in a depth-first
    // search spanning tree.
    m_parent[1] = 0;
    
#if CHECK_CHUNK_CODE
    if (m_df_number > 1)
    {
      parent_iter = m_parent.begin();
      vertex_iter = m_vertex.begin();
      
      parent_iter += 2;
      vertex_iter += 2;
      i = 2;
      
      do
      {
        assert(((*parent_iter) <= m_df_number) && ((*parent_iter) >= 1));
        assert((*vertex_iter)->df_number() == i);
        
        ++parent_iter;
        ++vertex_iter;
        ++i;
      }
      while (parent_iter != m_parent.end());
    }
    
    assert(1 == m_vertex[1]->df_number());
#endif
    
    // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
    //
    // for i := n by -1 until 2 do
    //   w = vertex(i);
    
    // :NOTE: Wei 2004-Oct-19:
    //
    // After build_dfs_spanning_tree(), 'mp_prev_dfs_searching_bb' should
    // points to the last dfs number basic block.
    BasicBlock *curr_bb = mp_prev_dfs_searching_bb;
    
    assert(curr_bb != 0);
    
#if CHECK_CHUNK_CODE
    uint32_t old_w = curr_bb->df_number() + 1;
#endif
    
    while (curr_bb != mp_root_bb)
    {
      uint32_t const w = curr_bb->df_number();
      
#if CHECK_CHUNK_CODE
      assert(w == old_w - 1);
      
      old_w = w;
#endif
      
      // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
      //
      // step2:
      //   for each v belongs to pred(w) do
      std::vector<BasicBlock *> const &pred_bb = curr_bb->pred_bb();
      
      for (std::vector<BasicBlock *>::const_iterator iter = pred_bb.begin();
           iter != pred_bb.end();
           ++iter)
      {
        // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
        //
        // u := eval(v)
        uint32_t const u = eval((*iter)->df_number());
        
        // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
        //
        // if semi(u) < semi(w) then semi(w) := semi(u) fi od;
        if (m_semi_dominator[u] < m_semi_dominator[w])
        {
          m_semi_dominator[w] = m_semi_dominator[u];
        }
      }
      
      // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
      //
      // add w to bucket(vertex(semi(w)));
      m_vertex[m_semi_dominator[w]]->add_to_bucket(curr_bb);
      
      // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
      //
      // link(parent(w), w);
      link(m_parent[w], w);
      
      // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
      //
      // step3:
      //   for each v belongs to bucket(parent(w)) do
      //     delete v from bucket(parent(w));
      BasicBlock *v_bb = m_vertex[m_parent[w]]->remove_from_bucket();
      
      while (v_bb != 0)
      {
        uint32_t const v = v_bb->df_number();
        
        // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
        //
        // u := eval(v);
        uint32_t const u = eval(v);
        
        // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
        //
        //   if semi(u) < semi(v) then
        //     dom(v) := u
        //   else
        //     dom(v) := parent(w)
        //   fi
        //  od
        // od;
        if (m_semi_dominator[u] < m_semi_dominator[v])
        {
          m_dominator[v] = u;
        }
        else
        {
          m_dominator[v] = m_parent[w];
        }
        
        v_bb = m_vertex[m_parent[w]]->remove_from_bucket();
      }
      
      curr_bb = curr_bb->dfs_prev_number_bb();
    }
    
    // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
    //
    // step4:
    //   for i := 2 until n do
    //     w := vertex(i);
    if (m_df_number > 1)
    {
      vertex_iter = m_vertex.begin();
      vertex_iter += 2;
      
      do
      {
        // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
        //
        // if dom(w) != vertex(semi(w)) then dom(w) := dom(dom(w)) fi od;
        if (m_dominator[(*vertex_iter)->df_number()] !=
            m_semi_dominator[(*vertex_iter)->df_number()])
        {
          m_dominator[(*vertex_iter)->df_number()] =
            m_dominator[m_dominator[(*vertex_iter)->df_number()]];
        }
        
        ++vertex_iter;
      }
      while (vertex_iter != m_vertex.end());
    }
    
    // :Lengauer-Tarjan [1979]: p.129: Wei 2004-Oct-20:
    //
    // dom(r) := 0;
    m_dominator[1] = 0;
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // Link each basic block with its dominator one.
    if (m_df_number > 1)
    {
      std::vector<uint32_t>::const_iterator dominator_iter = m_dominator.begin();
      
      dominator_iter += 2;
      i = 2;
      
      do
      {
        m_vertex[i]->set_idom_bb(m_vertex[(*dominator_iter)]);
        m_vertex[(*dominator_iter)]->add_be_dominanced(m_vertex[i]);
        
        ++dominator_iter;
        ++i;
      }
      while (dominator_iter != m_dominator.end());
    }
    
#if PRINT_CHUNK_CODE
    g_log_file << std::dec
               << m_vertex[1]->df_number()
               << "("
               << m_vertex[1]->sequence_idx()
               << "): dom: "
               << m_dominator[1]
               << ": ";
    
    for (std::vector<BasicBlock *>::const_iterator iter = m_vertex[1]->dominanced_by_me().begin();
         iter != m_vertex[1]->dominanced_by_me().end();
         ++iter)
    {
      g_log_file << (*iter)->df_number()
                 << " ";
    }
    
    g_log_file << ": ";
    
    for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = m_vertex[1]->succ_bb().begin();
         iter != m_vertex[1]->succ_bb().end();
         ++iter)
    {
      g_log_file << (*iter).bb()->df_number()
                 << "("
                 << (*iter).bb()->sequence_idx()
                 << ") ";
    }
    
    g_log_file << std::endl;
    
    if (m_df_number > 1)
    {
      std::vector<uint32_t>::const_iterator dominator_iter = m_dominator.begin();
      std::vector<uint32_t>::const_iterator semi_dominator_iter = m_semi_dominator.begin();
      
      dominator_iter += 2;
      semi_dominator_iter += 2;
      i = 2;
      
      while (dominator_iter != m_dominator.end())
      {
        g_log_file << m_vertex[i]->df_number()
                   << "("
                   << m_vertex[i]->sequence_idx()
                   << "): dom: "
                   << (*dominator_iter)
                   << ": ";
        
        for (std::vector<BasicBlock *>::const_iterator iter = m_vertex[i]->dominanced_by_me().begin();
             iter != m_vertex[i]->dominanced_by_me().end();
             ++iter)
        {
          g_log_file << (*iter)->df_number()
                     << " ";
        }
        
        g_log_file << ": ";
        
        for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = m_vertex[i]->succ_bb().begin();
             iter != m_vertex[i]->succ_bb().end();
             ++iter)
        {
          g_log_file << (*iter).bb()->df_number()
                     << "("
                     << (*iter).bb()->sequence_idx()
                     << ") ";
        }
        
        g_log_file << std::endl;
        
#if CHECK_CHUNK_CODE
        assert(((*dominator_iter) <= m_df_number) && ((*dominator_iter) >= 1) && ((*dominator_iter) != i));
        assert(((*semi_dominator_iter) <= m_df_number) && ((*semi_dominator_iter) >= 1));
#endif
        
        ++dominator_iter;
        ++semi_dominator_iter;
        ++i;
      }
    }
    
    g_log_file << std::endl;
#endif
  }
  
  // :NOTE: Wei 2004-Dec-30:
  //
  // The following algorithm is from
  // 'An Efficient Method of Computing Static Single Assignment Form'.
  // by Ron Cytron.
  inline void
  Chunk::compute_dominance_frontier(BasicBlock * const bb)
  {
    // :NOTE: Wei 2004-Dec-22:
    //
    // for each child c of bb in the dominator tree do
    for (std::vector<BasicBlock *>::iterator iter_c = bb->dominanced_by_me().begin();
         iter_c != bb->dominanced_by_me().end();
         ++iter_c)
    {
      // :NOTE: Wei 2004-Dec-22:
      //
      // compute DF[c]
      //
      // :NOTE: Wei 2004-Dec-30:
      //
      // This step must do first,
      // becuase the algorithm explains that operate each basic blocks from the bottom up
      // in the dominator tree.
      //
      // Thus I have to recursive call compute_dominance_frontier() here.
      compute_dominance_frontier(*iter_c);
      
      // :NOTE: Wei 2004-Dec-22:
      //
      // for each element w of DF[c] do
      for (std::vector<BasicBlock *>::iterator iter_w = (*iter_c)->dominance_frontier().begin();
           iter_w != (*iter_c)->dominance_frontier().end();
           ++iter_w)
      {
        // :NOTE: Wei 2004-Dec-22:
        //
        // if bb does not dominate w
        if ((*iter_w)->idom_bb() != bb)
        {
          // :NOTE: Wei 2004-Dec-22:
          //
          // S U= {w}
          add_to_vector_without_duplicate<BasicBlock>(bb->dominance_frontier(), *iter_w);
        }
      }
    }
    
    // :NOTE: Wei 2004-Dec-22:
    //
    // for each node y in succ(bb) do
    for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter_y = bb->succ_bb().begin();
         iter_y != bb->succ_bb().end();
         ++iter_y)
    {
      // :NOTE: Wei 2004-Dec-22:
      //
      // if n does not dominate y
      if ((*iter_y).bb()->idom_bb() != bb)
      {
        // :NOTE: Wei 2004-Dec-22:
        //
        // S U= {y}
        add_to_vector_without_duplicate<BasicBlock>(bb->dominance_frontier(), (*iter_y).bb());
      }
    }
  }
  
  inline void
  Chunk::compute_iterated_dominance_frontier_for_var(Variable * const var)
  {
    // :NOTE: Wei 2004-Dec-24:
    //
    // IDF(S) = {}
    // workList = {}
    std::vector<BasicBlock *> worklist;
    
    // :NOTE: Wei 2004-Dec-24:
    //
    // foreach n belongs to S do
    for (std::vector<DefinedBBPair *>::iterator iter = var->defined_bb().begin();
         iter != var->defined_bb().end();
         ++iter)
    {
      // :NOTE: Wei 2004-Dec-24:
      //
      // worklist U= {n}
      
      add_to_vector_without_duplicate<BasicBlock>(worklist, (*iter)->defined_bb());
    }
    
    // :NOTE: Wei 2004-Dec-30:
    //
    // The algorithm explains that the original basic block set contains
    // those basic blocks which defines 'var' and the entry block.
    // Thus I have to add the entry block to the 'defined_bb' field of 'var'.
    add_to_vector_without_duplicate<BasicBlock>(worklist, mp_root_bb);
    
    // :NOTE: Wei 2004-Dec-24:
    //
    // while worklist != {} do
    while (worklist.size() != 0)
    {
      // :NOTE: Wei 2004-Dec-24:
      //
      // take n from worklist
      BasicBlock * const n = worklist.back();
      
      worklist.pop_back();
      
      // :NOTE: Wei 2004-Dec-24:
      //
      // foreach c belongs to DF(n) do
      for (std::vector<BasicBlock *>::iterator iter_c = n->dominance_frontier().begin();
           iter_c != n->dominance_frontier().end();
           ++iter_c)
      {
        // :NOTE: Wei 2004-Dec-24:
        //
        // if c not belongs to IDF(S) then
        for (std::vector<BasicBlock *>::const_iterator iter_2 = var->iterated_dominance_frontier().begin();
             iter_2 != var->iterated_dominance_frontier().end();
             ++iter_2)
        {
          if ((*iter_c) == (*iter_2))
          {
            goto done;
          }
        }
        
        // :NOTE: Wei 2004-Dec-24:
        //
        // IDF(S) U= {c}
        // worklist U= {c}
        add_to_vector_without_duplicate<BasicBlock>(var->iterated_dominance_frontier(), *iter_c);
        add_to_vector_without_duplicate<BasicBlock>(worklist, *iter_c);
      }
      
    done:
      
      continue;
    }
  }
  
  inline void
  Chunk::insert_phi_function()
  {
    // :NOTE: Wei 2005-Jan-14:
    //
    // Because I am sure that in each basic block, the temporary variables
    // are already in normal form.
    // However, I will use the temporary variable in switch table structure,
    // thus, in the global view, the temporary variables are not in the normal form.
    //
    // That is to say,
    // I still need to insert the phi function for the temporary variables.
    
    for (std::vector<Variable *>::iterator iter = m_used_var_table.begin();
         iter != m_used_var_table.end();
         ++iter)
    {
      assert((*iter) != 0);
      
      if (true == (*iter)->is_live_global())
      {
        compute_iterated_dominance_frontier_for_var(*iter);
        
#if PRINT_CHUNK_CODE
        if ((*iter)->iterated_dominance_frontier().size() == 0)
        {
          (*iter)->dump_info<false>();
          
          g_log_file << " lives global, but its iterated dominance frontier is 0."
                     << std::endl;
        }
#endif
        
        for (std::vector<BasicBlock *>::iterator bb_iter = (*iter)->iterated_dominance_frontier().begin();
             bb_iter != (*iter)->iterated_dominance_frontier().end();
             ++bb_iter)
        {
          // :NOTE: Wei 2004-Dec-24:
          //
          // Insert a phi-node (pruned-SSA)
          if (true == ((*bb_iter)->is_var_live_in(*iter)))
          {
#if PRINT_CHUNK_CODE
            g_log_file << "Insert phi for ";
            (*iter)->dump_info<false>();
            g_log_file << " at "
                       << (*bb_iter)->sequence_idx();
            g_log_file << std::endl;
#endif
            
            (*bb_iter)->add_phi_node(*iter);
          }
#if PRINT_CHUNK_CODE
          else
          {
            (*iter)->dump_info<false>();
            g_log_file << " lives global, but it is not live at bb: "
                       << (*bb_iter)->sequence_idx()
                       << std::endl;
          }
#endif
        }
      }
    }
    
#if PRINT_CHUNK_CODE
    g_log_file << std::endl;
#endif
  }
  
  inline void
  Chunk::rename_variable()
  {
    // :NOTE: Wei 2004-Dec-30:
    //
    // foreach global name i
    //   stack[i] = {}
    // end
    
    // :NOTE: Wei 2004-Dec-31:
    //
    // Call Rename(ENTRY)
    mp_root_bb->rename_variable();
  }
  
  template<Variable::KindEnum T_kind>
  inline void
  Chunk::number_var_and_fill_table_internal(Variable * const head, uint32_t &idx)
  {
    // :NOTE: Wei 2005-Mar-19:
    //
    // Each platform has its own stack arrangement.
    // For example, the SP pointer on x86 points to the top stack,
    // however, on other platforms, the SP pointer may point to the one right upper
    // than the top stack.
    //
    // So... on x86, the stack offset of the first stack variable should be 1 rather
    // than 0, thus 'STACK_VAR_OFFSET_START' below should be 1.
    uint32_t i = STACK_VAR_OFFSET_START;
    
    for (Variable *curr_var = head;
         curr_var != 0;
         curr_var = curr_var->next_link_var(), ++idx)
    {
      assert(T_kind == curr_var->kind());
      
#if CHECK_CHUNK_CODE
      curr_var->check_defined_lir();
#endif
      
      m_used_var_table.push_back(curr_var);
      
      curr_var->set_idx(idx);
      
      switch (T_kind)
      {
      case Variable::GLOBAL:
      case Variable::MEMORY:
        break;
        
      case Variable::TEMP:
        // :NOTE: Wei 2005-Mar-01:
        //
        // Used for determine the stack offset.
        curr_var->set_temp_idx(i++);
        break;
        
      case Variable::CONST:
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
#if CHECK_CHUNK_CODE
    if (Variable::TEMP == T_kind)
    {
      assert(i == (m_temp_var_number + 1));
    }
#endif
  }
  
  template<ConsiderMemoryVarEnum T_consider>
  inline void
  Chunk::number_var_and_fill_table()
  {
    m_used_var_table.clear();
    
    // :NOTE: Wei 2005-Jan-05:
    //
    // Number the variables (including global & temporary ones),
    // so that we can use the integer type to represent 'use', 'def', 'in' and 'out'.
    //
    // :NOTE: Wei 2005-Jan-05:
    //
    // And this operation may change the original number of every temp
    // variables (When PRINT_CHUNK_CODE turn on).
    uint32_t idx = 0;
    
    number_var_and_fill_table_internal<Variable::GLOBAL>(mp_global_var_head, idx);
    number_var_and_fill_table_internal<Variable::TEMP>(mp_temp_var_head, idx);
    
    if (CONSIDER_MEMORY_VAR == T_consider)
    {
      number_var_and_fill_table_internal<Variable::MEMORY>(mp_memory_var_head, idx);
    }
  }
  
#if CHECK_CHUNK_CODE
  inline void
  Chunk::check_all_bb_not_in_worklist()
  {
    for (std::vector<BasicBlock *>::const_iterator iter = m_bb_table.begin();
         iter != m_bb_table.end();
         ++iter)
    {
      if (0 == (*iter)->lir_head())
      {
        continue;
      }
      else
      {
        assert(false == (*iter)->in_worklist());
      }
    }
  }
#endif
  
  template<ConsiderMemoryVarEnum T_consider>
  inline void
  Chunk::build_worklist(BasicBlock * const bb,
                        uint32_t const num_of_bytes_needed_for_varlist)
  {
    if (false == bb->in_worklist())
    {
      // :NOTE: Wei 2004-Dec-28:
      //
      // Compute def-use for each basic block.
      bb->calculate_def_use<T_consider>(num_of_bytes_needed_for_varlist);
      
      bb->set_next_worklist_bb(mp_worklist_bb);
      bb->set_in_worklist(true);
      
      mp_worklist_bb = bb;
      
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = bb->succ_bb().begin();
           iter != bb->succ_bb().end();
           ++iter)
      {
        build_worklist<T_consider>((*iter).bb(), num_of_bytes_needed_for_varlist);
      }
    }
  }
  
  inline void
  Chunk::build_AVAIL_worklist(BasicBlock * const bb,
                              uint32_t const num_of_bytes_needed_for_varlist)
  {
    if (false == bb->in_worklist())
    {
      // :NOTE: Wei 2004-Dec-28:
      //
      // Compute def-use for each basic block.
      bb->calculate_AVAIL_AVLOC(num_of_bytes_needed_for_varlist);
      
      bb->set_next_worklist_bb(mp_worklist_bb);
      bb->set_in_worklist(true);
      
      mp_worklist_bb = bb;
      
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = bb->succ_bb().begin();
           iter != bb->succ_bb().end();
           ++iter)
      {
        build_AVAIL_worklist((*iter).bb(), num_of_bytes_needed_for_varlist);
      }
    }
  }
  
  template<ConsiderMemoryVarEnum T_consider>
  inline void
  Chunk::live_variable_analysis()
  {
    // :NOTE: Wei 2004-Oct-23:
    //
    // number the temp variable for the following stage.
    number_var_and_fill_table<T_consider>();
    
    uint32_t const num_of_bytes_needed_for_varlist =
      (m_used_var_table.size() + 31) >> 5; // divided by 32
    
    // :NOTE: Wei 2004-Oct-25:
    //
    // Build work list first.
    
#if CHECK_CHUNK_CODE
    check_all_bb_not_in_worklist();
#endif
    
    mp_worklist_bb = 0;
    
    build_worklist<T_consider>(mp_root_bb, num_of_bytes_needed_for_varlist);
    
    while (mp_worklist_bb != 0)
    {
      BasicBlock * const curr_bb = mp_worklist_bb;
      
      mp_worklist_bb = mp_worklist_bb->next_worklist_bb();
      
      // :NOTE: Wei 2004-Oct-25:
      //
      // Indicate that this basic block has already been removed from the
      // work list.
      curr_bb->set_in_worklist(false);
      
      if (true == curr_bb->calculate_in_out())
      {
        for (std::vector<BasicBlock *>::iterator iter = curr_bb->pred_bb().begin();
             iter != curr_bb->pred_bb().end();
             ++iter)
        {
          if (false == (*iter)->in_worklist())
          {
            (*iter)->set_next_worklist_bb(mp_worklist_bb);
            (*iter)->set_in_worklist(true);
            
            mp_worklist_bb = (*iter);
          }
        }
      }
    }
    
#if PRINT_CHUNK_CODE_MORE_DETAIL
    dump_live_var_info();
#endif
  }
  
  void
  Chunk::determine_live_global_var()
  {
    uint32_t const var_number = m_used_var_table.size();
    
    for (std::vector<BasicBlock *>::const_iterator iter = m_bb_table.begin();
         iter != m_bb_table.end();
         ++iter)
    {
      if (0 == (*iter)->lir_head())
      {
        continue;
      }
      else
      {
#if PRINT_CHUNK_CODE_MORE_DETAIL
        g_log_file << "checking live var info for bb: "
                   << (*iter)->sequence_idx()
                   << std::endl;
#endif
        
        for (uint32_t i = 0; i < var_number; ++i)
        {
          uint32_t const vector_idx = (i >> 5);
          
          assert(((*iter)->in()).size() > vector_idx);
          
          if ((((*iter)->in())[vector_idx] & (1 << (i - (vector_idx << 5)))) != 0)
          {
            assert(m_used_var_table[i] != 0);
            
#if PRINT_CHUNK_CODE_MORE_DETAIL
            m_used_var_table[i]->dump_info<false>();
            g_log_file << " lives global."
                       << std::endl;
#endif
            
            m_used_var_table[i]->set_live_global();
          }
        }
      }
    }
  }
  
  inline void
  Chunk::numbering_lir_by_bfs()
  {
    assert(mp_root_bb != 0);
    uint32_t idx = 0;
    std::queue<BasicBlock *> bb_queue;
    
    bb_queue.push(mp_root_bb);
    mp_root_bb->set_already_added_to_the_bfs_list();
    
    while (false == bb_queue.empty())
    {
      BasicBlock * const bb = bb_queue.front();
      
      bb_queue.pop();
      
      assert(true == bb->is_already_added_to_the_bfs_list());
      
      for (LIR *curr_lir = bb->lir_scalar_phi_head();
           curr_lir != 0;
           curr_lir = curr_lir->next_lir())
      {
        curr_lir->set_idx(idx++);
        
        m_all_lir_by_bfs.push_back(curr_lir);
      }
      
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = bb->succ_bb().begin();
           iter != bb->succ_bb().end();
           ++iter)
      {
        if (false == (*iter).bb()->is_already_added_to_the_bfs_list())
        {
          bb_queue.push((*iter).bb());
          
          (*iter).bb()->set_already_added_to_the_bfs_list();
        }
      }
    }
  }
  
#if PRINT_CHUNK_CODE
  void
  Chunk::dump_mir_info() const
  {
    // :NOTE: Wei 2004-Nov-1:
    //
    // Number MIR instructions.
    MIR *curr_mir = mp_mir_head;
    uint32_t inst_idx = 0;
    
    do
    {
      curr_mir->set_idx(inst_idx);
      
      ++inst_idx;
      curr_mir = curr_mir->next_mir();
    }
    while (curr_mir != 0);
    
    // :NOTE: Wei 2004-Nov-1:
    //
    // Dump MIR info.
    curr_mir = mp_mir_head;
    
    g_log_file << std::endl;
    
    do
    {
      g_log_file << std::dec
                << curr_mir->idx()
                << ": ";
      curr_mir->dump_info();
      
      curr_mir = curr_mir->next_mir();
    }
    while (curr_mir != 0);
    
    g_log_file << std::endl;
  }
    
  void
  Chunk::dump_iterated_dominance_frontier_for_var(Variable * const var)
  {
    for (std::vector<BasicBlock *>::const_iterator iter = var->iterated_dominance_frontier().begin();
         iter != var->iterated_dominance_frontier().end();
         ++iter)
    {
      g_log_file << (*iter)->df_number()
                 << " ";
    }
    
    g_log_file << std::endl;
  }
#endif
  
#if PRINT_CHUNK_CODE
  template<bool T_ssa_form, bool T_use_global_idx, bool T_print_memory_operation_idx>
  void
  Chunk::print_lir_code() const
  {
    for (std::vector<BasicBlock *>::const_iterator iter = m_bb_table.begin();
         iter != m_bb_table.end();
         ++iter)
    {
      if (0 == (*iter)->lir_head())
      {
        continue;
      }
      else
      {
        (*iter)->dump_lir_info<T_ssa_form, T_use_global_idx, T_print_memory_operation_idx>();
      }
    }
  }
  
  inline void
  Chunk::dump_live_var_info_internal(std::vector<uint32_t> const &vect) const
  {
    for (uint32_t i = 0; i < m_used_var_table.size(); ++i)
    {
      uint32_t const vector_idx = (i >> 5);
      
      assert(vect.size() > vector_idx);
      
      if ((vect[vector_idx] & (1 << (i - (vector_idx << 5)))) != 0)
      {
        m_used_var_table[i]->dump_info<true>();
        
        g_log_file << " ";
      }
    }
  }
  
  void
  Chunk::dump_live_var_info() const
  {
    for (std::vector<BasicBlock *>::const_iterator iter = m_bb_table.begin();
         iter != m_bb_table.end();
         ++iter)
    {
      if (0 == (*iter)->lir_head())
      {
        continue;
      }
      else
      {
        g_log_file << (*iter)->sequence_idx()
                   << std::endl;
        
        g_log_file << "in: ";
        
        dump_live_var_info_internal((*iter)->in());
        
        g_log_file << std::endl
                   << "out: ";
        
        dump_live_var_info_internal((*iter)->out());
        
        g_log_file << std::endl;
      }
    }
  }
#endif
  
#if CHECK_CHUNK_CODE
  void
  Chunk::check_ssa_form(BasicBlock * const bb)
  {
    if (false == bb->already_check_ssa_form())
    {
      // :NOTE: Wei 2005-Jan-17:
      //
      // SSA properties:
      //
      // 1) Each variable has 1 definition at most.
      // 2) A definition of a variable dominates all uses.
      // 3) A preorder DFS over the CFG visits variable definitions before uses.
      // 4) Each use is reached by one definition
      
      bb->check_ssa_form();
      
      bb->set_already_check_ssa_form();
      
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = bb->succ_bb().begin();
           iter != bb->succ_bb().end();
           ++iter)
      {
        check_ssa_form((*iter).bb());
      }
    }
  }
#endif
  
  inline void
  Chunk::convert_to_ssa_form()
  {
    // :NOTE: Wei 2004-Oct-19:
    //
    // Convert the LIR codes to the SSA form.
    
    // :NOTE: Wei 2004-Oct-21:
    //
    // Stage 1: build dominator tree
    build_dominator_tree();
    
    compute_dominance_frontier(mp_root_bb);
    
#if PRINT_CHUNK_CODE
    g_log_file << "====== dominance frontier ======"
               << std::endl;
    
    for (uint32_t i = 1; i <= m_df_number; ++i)
    {
      g_log_file << i
                 << "("
                 << m_vertex[i]->sequence_idx()
                 << "): ";
      
      m_vertex[i]->dump_dominance_frontier();
      
      g_log_file << std::endl;
    }
    
    g_log_file << std::endl;
#endif
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // Stage 2: Live Variable Analysis
    //          (worklist algorithm)
    //
    // :NOTE: Wei  2005-Jan-25:
    //
    // From Preston Briggs' article:
    // "Pratical Improvements to the Construction and
    //  Destruction of Static Single Assignment Form".
    // It says:
    // "if SSA form is used to support finding live ranges during
    //  register allocation, then the pruned form should be used
    //  - phi-functions represent merging of live ranges, and
    //  unnecessary merging detracts from the quality of allocation."
    //
    // And my purpose to build a SSA form is used in register allocation,
    // thus I should build a pruned-SSA form, thus I should preform a
    // global live variable analysis first.
    live_variable_analysis<CONSIDER_MEMORY_VAR>();
    
    // :NOTE: Wei 2005-Jan-04:
    //
    // Stage 3: Determine which variable are live global.
    determine_live_global_var();
    
#if PRINT_CHUNK_CODE
    g_log_file << std::endl;
#endif
    
    // :NOTE: Wei 2004-Dec-21:
    //
    // Stage 4: insert Phi-function (pruned SSA)
    insert_phi_function();
    
    // :NOTE: Wei 2004-Dec-24:
    //
    // Stage 5: Rename variable
    rename_variable();
    
#if PRINT_CHUNK_CODE
    print_lir_code<true, false, false>();
#endif
    
#if CHECK_CHUNK_CODE
    check_ssa_form(mp_root_bb);
#endif
  }
  
  namespace
  {
    inline uint32_t
    hash_function_for_tuple(Tuple const * const tuple)
    {
      uint32_t value = static_cast<uint32_t>(tuple->kind());
      
      for (std::vector<ValueNumber>::const_iterator iter = tuple->operands_value_number().begin();
           iter != tuple->operands_value_number().end();
           ++iter)
      {
        switch ((*iter).kind())
        {
        case ValueNumber::VAR:
          value ^= reinterpret_cast<uint32_t>((*iter).var());
          break;
          
        case ValueNumber::CONST:
          value ^= (*iter).value();
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      
      // :NOTE: Wei 2005-May-20:
      //
      // Simply from (value % OPERATION_TABLE_SIZE)
      return value & (Chunk::OPERATION_TABLE_SIZE - 1);
    }
  }
  
  inline Tuple *
  Chunk::search_operation_table(Tuple const * const tuple)
  {
    uint32_t const hash_table_idx = hash_function_for_tuple(tuple);
    
    for (std::list<Tuple *>::const_iterator iter = m_operation_table[hash_table_idx].begin();
         iter != m_operation_table[hash_table_idx].end();
         ++iter)
    {
      if ((*tuple) == (**iter))
      {
        return *iter;
      }
    }
    
    return 0;
  }
  
  inline bool
  Chunk::value_numbering_one_bb(BasicBlock * const bb)
  {
    bool changed = false;
    
    // :NOTE: Wei 2005-May-17:
    //
    // for all phi nodes in basic block BLOCK {
    for (LIR *curr_lir = bb->lir_head();
         curr_lir != bb->lir_normal_head();
         curr_lir = curr_lir->next_lir())
    {
      assert(bb->lir_normal_head()->kind() != LIR::PHI);
      assert(LIR::PHI == curr_lir->kind());
      
      // :NOTE: Wei 2005-May-17:
      //
      // ignore phi args not numbered yet;
      
      bool all_same = true;
      Variable *value_number = 0;
      
      // :NOTE: Wei 2005-May-17:
      //
      // if numbered args have same value number ssa_v
      for (std::vector<OperandKind *>::const_iterator iter = curr_lir->operand().begin();
           iter != curr_lir->operand().end();
           ++iter)
      {
        Variable * const var = (*iter)->variable();
        
        if (var->scc_value_number() != 0)
        {
          if (value_number != 0)
          {
            if (var->scc_value_number() != value_number)
            {
              all_same = false;
              
              break;
            }
          }
          else
          {
            value_number = var->scc_value_number();
          }
        }
      }
      
      assert(1 == curr_lir->dest().size());
      
      Variable * const phi_var = curr_lir->dest().front()->variable();
      
      Variable *final_value_number;
      
      if ((true == all_same) && (value_number != 0))
      {
        // :NOTE: Wei 2005-May-17:
        //
        // value_table[ssa_phi] = ssa_v;
        final_value_number = value_number;
      }
      else
      {
        // :NOTE: Wei 2005-May-17:
        //
        // value_table[ssa_phi] = ssa_phi;
        final_value_number = phi_var;
      }
      
      if (true == phi_var->set_scc_value_number(final_value_number))
      {
        changed = true;
      }
    }
    
    // :NOTE: 2005-May-17:
    //
    // for all scalar op in basic block BLOCK in execution order { /* op : opcode r1 r2 => r3 */
    for (LIR *curr_lir = bb->lir_normal_head();
         curr_lir != 0;
         curr_lir = curr_lir->next_lir())
    {
      // :NOTE: Wei 2005-May-17:
      //
      // There are 2 versions of SCCVN redundant operation elimination:
      //
      // 1) just take care of scalar operations:
      //    { author = "Loren Taylor Simpson",
      //      title = "Value-Driven Redundancy Elimination" }
      //
      // 2) take care of both scalar & memory operations:
      //    { author = "Li Xu",
      //      title = "Program Redundancy Analysis and Optimization to Improve Memory Performance" }
      //
      // When I read these 2 papers, I have one question:
      // How can I determine the operand's initial value number when I met this kind of instructions below:
      //
      // ADD r1, r2 => r3
      //
      // If I don't meet with r1 & r2 before.
      //
      // And I finally conclude 2 conclusion:
      //
      // 1) in the "just taka care of scalar operations" case,
      //    if I don't meet the operand before, then the initial value number of that operand
      //    should be the SSA name of that operand.
      //
      // 2) in the "take care of both scalar & memory operations" case,
      //    I will not get into the situation where I haven't meet that operand and I want to use it.
      //    That is because in this case, I will load every possible global variables before it uses,
      //    and the algorithm will handle these LOAD opertions correctly.
      
      switch (curr_lir->value_numbering_type())
      {
      case SCALAR_OPERATION_PERFORM_SEARCHING:
        assert(curr_lir->dest().size() != 0);
        assert(OperandKind::VARIABLE == curr_lir->dest().front()->kind());
        
        {
          // :NOTE: Wei 2005-May-17:
          //
          // use tuple <opcode, value_table[r1], value_table[r2]> to lookup operation_table;
          
          Tuple * const tuple = new Tuple(curr_lir);
          Tuple * const result = search_operation_table(tuple);
          
          // :NOTE: Wei 2005-May-19:
          //
          // if there is a match with result v
          if (result != 0)
          {
            assert(result->equivalent_operation().size() != 0);
            
            if (true == result->add_equivalent_operation(curr_lir))
            {
              curr_lir->set_equivalent_operation(result);
            }
            
            // :NOTE: Wei 2005-May-19:
            //
            // value_table[r3] = v;
            assert(result->results_value_number().size() == curr_lir->dest().size());
            
            std::vector<Variable *>::const_iterator tuple_iter = result->results_value_number().begin();
            
            for (std::vector<OperandKind *>::const_iterator iter = curr_lir->dest().begin();
                 iter != curr_lir->dest().end();
                 ++iter, ++tuple_iter)
            {
              if (true == (*iter)->variable()->set_scc_value_number(*tuple_iter))
              {
                changed = true;
              }
            }
            
            // :NOTE: Wei 2005-May-19:
            //
            // Because m_operation_table[] has already have this tuple,
            // thus I have to delete it to avoid the memory leak.
            delete tuple;
          }
          // :NOTE: Wei 2005-May-19:
          //
          // else
          else
          {
            // :NOTE: Wei 2005-May-19:
            //
            // value_table[r3] = r3;
            //
            // add tuple <opcode, value_table[r1], value_table[r2]>
            //   to operation_table with result r3;
            assert(0 == tuple->results_value_number().size());
            
            for (std::vector<OperandKind *>::const_iterator iter = curr_lir->dest().begin();
                 iter != curr_lir->dest().end();
                 ++iter)
            {
              if (true == (*iter)->variable()->set_scc_value_number((*iter)->variable()))
              {
                changed = true;
              }
              
              tuple->results_value_number().push_back((*iter)->variable());
            }
            
            curr_lir->set_equivalent_operation(tuple);
            
#if CHECK_CHUNK_CODE
            bool add_result =
#endif
              tuple->add_equivalent_operation(curr_lir);
            
#if CHECK_CHUNK_CODE
            assert(true == add_result);
#endif
            
            m_operation_table[hash_function_for_tuple(tuple)].push_back(tuple);
          }
        }
        break;
        
      case SCALAR_OPERATION_NO_PERFORM_SEARCHING:
        for (std::vector<OperandKind *>::const_iterator iter = curr_lir->dest().begin();
             iter != curr_lir->dest().end();
             ++iter)
        {
          if (true == (*iter)->variable()->set_scc_value_number((*iter)->variable()))
          {
            changed = true;
          }
        }
        break;
        
      case MEMORY_OPERATION_LOAD:
        assert(false == curr_lir->is_cond_flags_involved());
        
        {
          // :NOTE: Wei 2005-May-20:
          //
          // For a load op1: xLD ra -> rv [M-use1]
          // the algorithm constructs a tuple <Norm(xLD), Value(ra)>
          // and use it to search operation table.
          Tuple * const tuple = new Tuple(curr_lir);
          Tuple * const result = search_operation_table(tuple);
          
          assert(1 == curr_lir->dest().size());
          
          if (result != 0)
          {
            assert(result->equivalent_operation().size() != 0);
            
            if (true == result->add_equivalent_operation(curr_lir))
            {
              curr_lir->set_memory_operation_idx(result->memory_operation_idx());
              curr_lir->set_equivalent_operation(result);
            }
            
            assert(result->results_value_number().size() == curr_lir->dest().size());
            
            // :NOTE: Wei 2005-May-20:
            //
            // value_table[rv] = v, where v is the result value of result.
            if (true == curr_lir->dest().front()->variable()->
                set_scc_value_number(result->results_value_number().front()))
            {
              changed = true;
            }
            
            delete tuple;
          }
          else
          {
            curr_lir->set_memory_operation_idx(gp_chunk->memory_operation_number());
            
            // :NOTE: Wei 2005-May-20:
            //
            // value_table[rv] = rv
            // and add tuple to the operation table with result value as rv
            assert(0 == tuple->results_value_number().size());
            
            if (true == curr_lir->dest().front()->variable()->
                set_scc_value_number(curr_lir->dest().front()->variable()))
            {
              changed = true;
            }
            
            tuple->results_value_number().push_back(curr_lir->dest().front()->variable());
            
            curr_lir->set_equivalent_operation(tuple);
            
            tuple->set_memory_operation_idx(curr_lir->memory_operation_idx());
            
#if CHECK_CHUNK_CODE
            bool add_result =
#endif
              tuple->add_equivalent_operation(curr_lir);
            
#if CHECK_CHUNK_CODE
            assert(true == add_result);
#endif
            
            m_operation_table[hash_function_for_tuple(tuple)].push_back(tuple);
          }
        }
        break;
        
      case MEMORY_OPERATION_STORE:
        assert(false == curr_lir->is_cond_flags_involved());
        
        {
          // :NOTE: Wei 2005-May-20:
          //
          // For a store op1: xST ra rv M-use[au ...] M-def[ad ...]
          // the algorithm constructs a tuple <Norm(xST), Value(ra)>
          // and use it to search operation table.
          Tuple * const tuple = new Tuple(curr_lir);
          Tuple * const result = search_operation_table(tuple);
          
          assert(0 == curr_lir->dest().size());
          assert(1 == curr_lir->operand().size());
          assert(1 == curr_lir->memory_use_list().size());
          assert(1 == curr_lir->memory_def_list().size());
          
          if (result != 0)
          {
            assert(result->equivalent_operation().size() != 0);
            
            if (true == result->add_equivalent_operation(curr_lir))
            {
              curr_lir->set_memory_operation_idx(result->memory_operation_idx());
              curr_lir->set_equivalent_operation(result);
            }
            
            // :NOTE: Wei 2005-May-20:
            //
            // value_table[ad] = value_table[au]
            if (true == curr_lir->memory_def_list().front()->variable()->
                set_scc_value_number(curr_lir->memory_use_list().front()->variable()->scc_value_number()))
            {
              changed = true;
            }
            
            delete tuple;
          }
          else
          {
            // :NOTE: Wei 2005-May-20:
            //
            // for each ad belongs to M-def[]
            //   it sets value_table[ad] = ad
            //
            // add tuple to operation table, with result value Value(rv)
            if (true == curr_lir->memory_def_list().front()->variable()->
                set_scc_value_number(curr_lir->memory_def_list().front()->variable()))
            {
              changed = true;
            }
            
            // :NOTE: Wei 2005-May-22:
            //
            // Although I have already added M-use[] into the tuple,
            // however, after reading the whole paper and a note, in the bottom of a page
            // in section 3.4 mentiond xST:
            // "If the matching entry is a store, use Value(M-def) instead, ...
            //  This also applies to operation table lookup for stores."
            // , in it. I think a xST tuple in the operation table should use M-def list rather than M-use list.
            //
            // Thus I have to replace the M-use entry already in the tuple with the M-def entry.
            tuple->operands_value_number().back().set_var(curr_lir->memory_def_list().front()->variable()->scc_value_number());
            
            // :NOTE: Wei 2005-May-20:
            //
            // I don't need to add Value(rv) as the result value number for this tuple.
            // Because I have already added in the construction stage for this tuple.
            //
            // See Tuple::Tuple() constructor.
            
            Tuple const * const already_added = search_operation_table(tuple);
            
            if (0 == already_added)
            {
              curr_lir->set_memory_operation_idx(gp_chunk->memory_operation_number());
              
              curr_lir->set_equivalent_operation(tuple);
              
              tuple->set_memory_operation_idx(curr_lir->memory_operation_idx());
              
#if CHECK_CHUNK_CODE
              bool add_result =
#endif
                tuple->add_equivalent_operation(curr_lir);
              
#if CHECK_CHUNK_CODE
              assert(true == add_result);
#endif
              
              m_operation_table[hash_function_for_tuple(tuple)].push_back(tuple);
            }
            else
            {
              delete tuple;
            }
          }
        }
        break;
        
      case MEMORY_OPERATION_JSR:
      case MEMORY_OPERATION_FRAME:
        assert(false == curr_lir->is_cond_flags_involved());
        
        curr_lir->set_memory_operation_idx(gp_chunk->memory_operation_number());
        
        // :NOTE: Wei 2005-May-20:
        //
        // 1) JSR:
        // for any b belongs to M-def, value_table[b] = b
        //
        // 2) FRAME:
        // for any a belongs to M-def, value_table[a] = a
        for (std::vector<OperandKind *>::const_iterator iter = curr_lir->memory_def_list().begin();
             iter != curr_lir->memory_def_list().end();
             ++iter)
        {
          assert(OperandKind::VARIABLE == (*iter)->kind());
          
          if (true == (*iter)->variable()->set_scc_value_number((*iter)->variable()))
          {
            changed = true;
          }
        }
        break;
        
      case NOT_INSIDE_VALUE_NUMBERING:
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    // :NOTE: Wei 2005-May-21:
    //
    // Finish value numbering, reset the postorder_idx = 0 to indicate it.
    bb->set_postorder_idx(0);
    
    return changed;
  }
  
  inline void
  Chunk::value_numbering_group_bb(BasicBlock * const bb)
  {
    bool changed;
    
    // :NOTE: Wei 2005-May-13:
    //
    // do {
    do
    {
      // :NOTE: Wei 2005-May-13:
      //
      // changed = false;
      changed = false;
      
      BasicBlock *curr_bb = bb;
      
      // :NOTE: Wei 2005-May-13:
      //
      // for each basic block BLOCK in SCC {
      while (curr_bb != 0)
      {
        // :NOTE: Wei 2005-May-13:
        //
        // ValueNumberOneBasicBlock(BLOCK);
        bool const bb_change = value_numbering_one_bb(curr_bb);
        
        // :NOTE: Wei 2005-May-13:
        //
        // if (value_table has changed during numbering)
        if (true == bb_change)
        {
          // :NOTE: Wei 2005-May-13:
          //
          // changed = true;
          changed = true;
        }
        
        curr_bb = curr_bb->next_scc_bb();
      }
    }
    // :NOTE: Wei 2005-May-13:
    //
    // } while (changed);
    while (true == changed);
  }
  
  namespace
  {
    inline bool
    sort_bb_by_postorder_idx(BasicBlock * const a, BasicBlock * const b)
    {
      return (a->postorder_idx() < b->postorder_idx()) ? true : false;
    }
  }
  
  // :NOTE: Wei 2005-May-18:
  //
  // I use Tarjan's algorithm.
  inline void
  Chunk::find_scc_component(BasicBlock * const bb)
  {
    // :NOTE: Wei 2005-May-13:
    //
    // node.DFSnum <- nextDFSnum++
    bb->set_scc_dfs_num(m_scc_dfs_num);
    
    // :NOTE: Wei 2005-May-13:
    //
    // node.low = node.DFSnum
    bb->set_scc_low_num(m_scc_dfs_num);
    
    ++m_scc_dfs_num;
    
    // :NOTE: Wei 2005-May-13:
    //
    // node.visited = TRUE
    bb->set_scc_visited();
    
    // :NOTE: Wei 2005-May-13:
    //
    // PUSH(node)
    m_scc_stack.push_back(bb);
    
    // :NOTE: Wei 2005-May-13:
    //
    // for each o belongs to {operands of node}
    for (std::vector<BasicBlock::SuccBBInfo>::iterator iter = bb->succ_bb().begin();
         iter != bb->succ_bb().end();
         ++iter)
    {
      // :NOTE: Wei 2005-May-13:
      //
      // if not o.visited
      if (false == (*iter).bb()->is_scc_visited())
      {
        // :NOTE: Wei 2005-May-13:
        //
        // DFS(o)
        find_scc_component((*iter).bb());
        
        // :NOTE: Wei 2005-May-13:
        //
        // node.low = MIN(node.low, o.low);
        bb->set_scc_low_num(min(bb->scc_low_num(), (*iter).bb()->scc_low_num()));
      }
      else
      {
        // :NOTE: Wei 2005-May-13:
        //
        // if o belongs to stack
        for (std::vector<BasicBlock *>::const_iterator s_iter = m_scc_stack.begin();
             s_iter != m_scc_stack.end();
             ++s_iter)
        {
          if ((*s_iter) == (*iter).bb())
          {
            // :NOTE: Wei 2005-May-13:
            //
            // node.low <- MIN(o.DFSnum, node.low)
            bb->set_scc_low_num(min(bb->scc_low_num(), (*iter).bb()->scc_dfs_num()));
            
            break;
          }
        }
      }
    }
    
    // :NOTE: Wei 2005-May-13:
    //
    // if node.low == node.DFSnum
    if (bb->scc_low_num() == bb->scc_dfs_num())
    {
      // :NOTE: Wei 2005-May-13:
      //
      // SCC <- empty
      std::vector<BasicBlock *> scc;
      
      BasicBlock *found_bb;
      
      // :NOTE: Wei 2005-May-13:
      //
      // do
      do
      {
        // :NOTE: Wei 2005-May-13:
        //
        // x <- POP()
        found_bb = m_scc_stack.back();
        
        m_scc_stack.pop_back();
        
        // :NOTE: Wei 2005-May-13:
        //
        // SCC <- SCC U {x}
        scc.push_back(found_bb);
      }
      // :NOTE: Wei 2005-May-13:
      //
      // while x != node
      while (found_bb != bb);
      
      assert(scc.size() != 0);
      
      if (scc.size() >= 1)
      {
        std::sort(scc.begin(), scc.end(), sort_bb_by_postorder_idx);
        
#if CHECK_CHUNK_CODE
        {
          std::vector<BasicBlock *>::const_iterator iter = scc.begin();
          
          ++iter;
          
          for (; iter != scc.end(); ++iter)
          {
            assert((*iter)->postorder_idx() > (*(iter - 1))->postorder_idx());
          }
        }
#endif
        
        std::vector<BasicBlock *>::const_reverse_iterator end_iter = scc.rend();
        
        --end_iter;
        
        for (std::vector<BasicBlock *>::const_reverse_iterator iter = scc.rbegin();
             iter != end_iter;
             ++iter)
        {
          // :NOTE: Wei 2005-May-22:
          //
          // That's why I need to use std::vector rather than std::list.
          // Because I use (iter + 1).
          // If an iterator can perform such that (iter + 1) operation,
          // that kind of iterator will be called "random access iterator".
          //
          // A Random Access Iterator is an iterator that provides both increment and decrement
          // (just like a Bidirectional Iterator), and that also provides constant-time methods
          // for moving forward and backward in arbitrary-sized steps.
          //
          // The iterator of std::list is just a bidirectional iterator, not a random access iterator.
          // So I can not perform (iter + 1) to it.
          // However, the iterator of std::vector is a random access iterator,
          // so that I can perform (iter + 1) to it.
          //
          // The reason why bidirectional iterator doesn't provide moving forward or backward function
          // is because:
          //
          // in the C++ standard 24.1/8:
          //
          // "All the categories of iterators require only those functions that
          // are realizable for a given category in constant time (amortized)."
          //
          // If an operation of a kind of iterator can not be implemented in "constant time", (i.e. O(1))
          // then that operation should not be provided.
          //
          // For std::list, the best complexity we can achieve for moving forward or backward is in linear
          // time. (i.e. O(n))
          (*iter)->set_next_scc_bb(*(iter + 1));
        }
      }
    }
  }
  
  inline void
  Chunk::scc_value_numbering(BasicBlock * const bb)
  {
    if (0 == bb->next_scc_bb())
    {
      (void)value_numbering_one_bb(bb);
    }
    else
    {
      value_numbering_group_bb(bb);
    }
  }
  
  inline void
  Chunk::preform_sccvn()
  {
#if CHECK_CHUNK_CODE
    {
      BasicBlock *curr_bb = mp_postorder_bb_head;
      
      while (curr_bb != 0)
      {
        assert(curr_bb->postorder_idx() != 0);
        
        curr_bb = curr_bb->next_postorder_bb();
      }
    }
#endif
    
    BasicBlock *curr_bb = mp_postorder_bb_tail;
    
    assert(curr_bb != 0);
    
    while (curr_bb != 0)
    {
      if (curr_bb->postorder_idx() != 0)
      {
        scc_value_numbering(curr_bb);
      }
      
      curr_bb = curr_bb->prev_postorder_bb();
    }
  }
  
  void
  Chunk::build_postorder_list(BasicBlock * const bb)
  {
    // :NOTE: Wei 2005-May-22:
    //
    // set postorder_idx to not non-zero to indicate that this bb has already been visited.
    // Through this facility to avoid the unlimited loops caused by back edges.
    bb->set_postorder_idx(1);
    
    for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = bb->succ_bb().begin();
         iter != bb->succ_bb().end();
         ++iter)
    {
      if (0 == (*iter).bb()->postorder_idx())
      {
        build_postorder_list((*iter).bb());
      }
    }
    
    // :NOTE: Wei 2005-May-21:
    //
    // The postorder idx starts from 1, so that I can use it to determine whether
    // this basic block be visited or not.
    bb->set_postorder_idx(++m_postorder_idx);
    
    if (0 == mp_postorder_bb_head)
    {
      mp_postorder_bb_head = bb;
      
      bb->set_prev_postorder_bb(0);
    }
    else
    {
      bb->set_prev_postorder_bb(mp_postorder_bb_tail);
      
      mp_postorder_bb_tail->set_next_postorder_bb(bb);
    }
    
    mp_postorder_bb_tail = bb;
    
    mp_postorder_bb_tail->set_next_postorder_bb(0);
  }
  
  inline void
  Chunk::AVAIL_analysis()
  {
    number_var_and_fill_table<NO_CONSIDER_MEMORY_VAR>();
    
    uint32_t const num_of_bytes_needed_for_varlist =
      (m_used_var_table.size() + m_memory_operation_number + 31) >> 5; // divided by 32
    
    // :NOTE: Wei 2004-Oct-25:
    //
    // Build work list first.
    
#if CHECK_CHUNK_CODE
    check_all_bb_not_in_worklist();
#endif
    
    mp_worklist_bb = 0;
    
    build_AVAIL_worklist(mp_root_bb, num_of_bytes_needed_for_varlist);
    
    while (mp_worklist_bb != 0)
    {
      BasicBlock * const curr_bb = mp_worklist_bb;
      
      mp_worklist_bb = mp_worklist_bb->next_worklist_bb();
      
      // :NOTE: Wei 2004-Oct-25:
      //
      // Indicate that this basic block has already been removed from the
      // work list.
      curr_bb->set_in_worklist(false);
      
      bool result;
      
      if (curr_bb == mp_root_bb)
      {
        result = curr_bb->calculate_AVAIL_in_out<true>();
      }
      else
      {
        result = curr_bb->calculate_AVAIL_in_out<false>();
      }
      
      if (true == result)
      {
        // :NOTE: Wei 2005-May-23:
        //
        // Here, I use succ_bb() rather than pred_bb().
        for (std::vector<BasicBlock::SuccBBInfo>::iterator iter = curr_bb->succ_bb().begin();
             iter != curr_bb->succ_bb().end();
             ++iter)
        {
          if (false == (*iter).bb()->in_worklist())
          {
            (*iter).bb()->set_next_worklist_bb(mp_worklist_bb);
            (*iter).bb()->set_in_worklist(true);
            
            mp_worklist_bb = (*iter).bb();
          }
        }
      }
    }
    
#if PRINT_CHUNK_CODE
    print_lir_code<true, false, true>();
#endif
  }
  
  // :NOTE: Wei 2005-May-24:
  //
  // Because I still want the whole LIRs be in SSA form after this fully static redundancy elimination stage.
  // Thus I modify the original algorithm to feet my needs.
  //
  // 1) I mantain a link list in each Tuple stored in the operation table,
  //    that link list records those LIRs which are equivalent to this Tuple.
  //
  // 2) When I need to replace a LOAD with a MOV,
  //    (NOTE: I can not just remove the LOAD, becuase I still want to be in SSA form.
  //     And I don't want to spend much time to scan the following LIRs to update the removable variable,
  //     thus I use a MOV to replace a LOAD rather than removing it.)
  //    I will scan the link list stored in the corresponding Tuple.
  //
  //    Because the LIRs stored in that link list is in the reverse postorder,
  //    so that, if I scan that list backwards, and find the first corresponding LIR of a basic block
  //    which doesn't has that memory operation lived in the entry (i.e. m_in).
  //    than that memory operation is what the current LIR be redundant with.
  //    And I will update the current LIR according to that LIR.
  //
  //    The reason why I need to do this is because not the first corresponding LIR is the real target.
  //    Ex:
  //              1 o
  /*                 \   */
  //                2 o
  /*                 / \ */
  //              3 o   o 4
  //
  // If there are 3 LIRs which is redundant in node 2, 3 & 4,
  // and all these 3 LIRs are corresponding to the one in node 1.
  // However, I can NOT replace the memory operation LIR in node 2, because that memory operation
  // is not lived in the entry of node 2.
  // Thus, the LIRs in node 3 & 4 can only be replaced according to the one in node 2,
  // not the one in node 1.
  //
  // Thus I have to do the above scanning and find the real target LIR to be the pivot.
  inline void
  Chunk::fully_static_redundancy_elimination()
  {
    for (BasicBlock *curr_bb = mp_postorder_bb_tail;
         curr_bb != 0;
         curr_bb = curr_bb->prev_postorder_bb())
    {
      if (0 == curr_bb->lir_head())
      {
        continue;
      }
      else
      {
        uint32_t vector_idx;
        uint32_t bit_position;
        
#if CHECK_CHUNK_CODE
        bool all_redundant;
#endif
        
        LIR *next_lir;
        
        for (LIR *curr_lir = curr_bb->lir_normal_head();
             curr_lir != 0;
             curr_lir = next_lir)
        {
          // :NOTE: Wei 2005-May-29:
          //
          // I have to record the next LIR now,
          // because I may delete the curr_lir in MEMORy_OPERATION_STORE elimination.
          next_lir = curr_lir->next_lir();
          
          switch (curr_lir->value_numbering_type())
          {
          case SCALAR_OPERATION_PERFORM_SEARCHING:
#if CHECK_CHUNK_CODE
            all_redundant = true;
#endif
            
            for (std::vector<OperandKind *>::const_iterator iter = curr_lir->dest().begin();
                 iter != curr_lir->dest().end();
                 ++iter)
            {
              // :NOTE: Wei 2005-May-26:
              //
              // The current architecture of my compiler can not support SCALAR type
              // Common Subexpression Elimination.
              //
              // The reason why I can't, to see the documentations I receive from several
              // professors. (I should save these docs in /MyData/CDRW_Backup/Document/)
              
              assert(OperandKind::VARIABLE == (*iter)->kind());
              
              calculate_bit_position((*iter)->variable()->scc_value_number()->idx(), vector_idx, bit_position);
              
#if 0
              assert(curr_bb->in().size() > vector_idx);
              
              // :NOTE: Wei 2005-May-23:
              //
              // If scalar operation s computes rv belongs to AVIN(X),
              // s is redundant and removed.
              if (curr_bb->in()[vector_idx] & bit_position)
              {
                // :NOTE: Wei 2005-May-18:
                //
                // I have to make a variant here to consider the condition flags calculation.
                // If an expresstion involved condition flags calculation, then this expresstion should not
                // be replaced by MOV instruction.
                if (true == curr_lir->is_cond_flags_involved())
                {
                  continue;
                }
                
                // :NOTE: Wei 2005-May-24:
                //
                // Now, I can only handle the SCALAR operation with only one result.
                // i.e. SMUL64, UMUL64, ADD64 not handled yet.
                // Because these 2 results operations need to add new LIR instruction to the basic block.
                assert(1 == curr_lir->dest().size());
                
#if CHECK_CHUNK_CODE
                assert(true == all_redundant);
#endif
                
                assert((*iter)->variable()->scc_value_number() != (*iter)->variable());
                
                assert(curr_lir->operand().size() != 0);
                assert(1 == curr_lir->dest().size());
                
                assert(curr_lir->equivalent_operation() != 0);
                
                LIR *target_lir = 0;
                bool find_me = false;
                
                for (std::vector<LIR *>::const_reverse_iterator equ_iter =
                       curr_lir->equivalent_operation()->equivalent_operation().rbegin();
                     equ_iter != curr_lir->equivalent_operation()->equivalent_operation().rend();
                     ++equ_iter)
                {
                  if ((*equ_iter) == curr_lir)
                  {
                    assert(false == find_me);
                    
                    find_me = true;
                    
                    continue;
                  }
                  
                  if ((true == find_me) &&
                      (true == (*equ_iter)->defined_bb()->is_dominated_by_me(curr_lir->defined_bb())))
                  {
                    target_lir = *equ_iter;
                  }
                }
                
                if (target_lir != 0)
                {
                  // :NOTE: Wei 2005-May-24:
                  //
                  // If the source variable are preallocated to a specified register,
                  // then I shall not to replace the current LIR.
                  // Becasue I may extends that variable's life and may be overlap to another
                  // variable's live time, and that variable also need to be placed in the same register.
                  //
                  // :WARNING: Wei 2005-May-24:
                  //
                  // I think this is a design error that I put this kind of optimization (global redundancy
                  // elimination) in the LIR form.
                  // I should put this optimization into the MIR form.
                  if (target_lir->dest().front()->variable()->preallocated_reg() != 0)
                  {
                    continue;
                  }
                  
#if PRINT_CHUNK_CODE
                  g_log_file << "Replace redundant SCALAR operation: ";
                  target_lir->dest().front()->variable()->dump_info<true>();
                  g_log_file << " -> ";
                  (*iter)->variable()->dump_info<true>();
                  g_log_file << std::endl;
#endif
                  
                  for (std::vector<OperandKind *>::const_iterator oper_iter = curr_lir->operand().begin();
                       oper_iter != curr_lir->operand().end();
                       ++oper_iter)
                  {
                    delete *oper_iter;
                  }
                  
                  curr_lir->operand().clear();
                  curr_lir->operand().push_back(new OperandKind(target_lir->dest().front()->variable()));
                  
                  curr_lir->set_kind(LIR::MOV_R_2_R);
                }
                else
                {
                  // :NOTE: Wei 2005-May-25:
                  //
                  // I can't find the target LIR, then this means I can not replace this SCALAR operation with a
                  // MOV instruction.
                }
              }
              // :NOTE: Wei 2005-May-23:
              //
              // otherwise, add rv to AVIN(X)
              else
              {
#endif // #if 0
                
#if CHECK_CHUNK_CODE
                all_redundant = false;
#endif
                
                curr_bb->in()[vector_idx] |= bit_position;
                
#if 0
              }
#endif // #if 0
            }
            break;
            
          case SCALAR_OPERATION_NO_PERFORM_SEARCHING:
            for (std::vector<OperandKind *>::const_iterator iter = curr_lir->dest().begin();
                 iter != curr_lir->dest().end();
                 ++iter)
            {
              assert(OperandKind::VARIABLE == (*iter)->kind());
              assert((*iter)->variable() == (*iter)->variable()->scc_value_number());
              
              calculate_bit_position((*iter)->variable()->idx(), vector_idx, bit_position);
              
              assert(curr_bb->in().size() > vector_idx);
              assert(0 == (curr_bb->in()[vector_idx] & bit_position));
              
              curr_bb->in()[vector_idx] |= bit_position;
            }
            break;
            
            // :NOTE: Wei 2005-May-23:
            //
            // If memory operation with ID m belongs to AVIN(X),
            // m is redundant and removed, otherwise, add m to AVIN(X).
            // Furthermore, for load, add result value rv to AVIN.
          case MEMORY_OPERATION_LOAD:
            {
              calculate_bit_position(gp_chunk->used_var_table().size() +
                                     curr_lir->memory_operation_idx(),
                                     vector_idx,
                                     bit_position);
              
              assert(curr_bb->in().size() > vector_idx);
              
              assert(1 == curr_lir->dest().size());
              assert(OperandKind::VARIABLE == curr_lir->dest().front()->kind());
              
              // :NOTE: Wei 2005-May-23:
              //
              // If memory operation with ID m belongs to AVIN(X),
              // m is redundant and removed.
              if (curr_bb->in()[vector_idx] & bit_position)
              {
                assert(curr_lir->dest().front()->variable()->scc_value_number() !=
                       curr_lir->dest().front()->variable());
                
                assert(0 == curr_lir->operand().size());
                assert(1 == curr_lir->dest().size());
                assert(curr_lir->equivalent_operation() != 0);
                
                LIR *target_lir = 0;
                bool find_me = false;
                
                for (std::vector<LIR *>::const_reverse_iterator iter =
                       curr_lir->equivalent_operation()->equivalent_operation().rbegin();
                     iter != curr_lir->equivalent_operation()->equivalent_operation().rend();
                     ++iter)
                {
                  if ((*iter) == curr_lir)
                  {
                    assert(false == find_me);
                    
                    find_me = true;
                    
                    continue;
                  }
                  
                  if ((true == find_me) &&
                      (true == (*iter)->defined_bb()->is_dominated_by_me(curr_lir->defined_bb())))
                  {
                    target_lir = *iter;
                  }
                }
                
                if (target_lir != 0)
                {
#if PRINT_CHUNK_CODE
                  g_log_file << "Replace redundant LOAD operation: ";
#endif
                  
                  switch (target_lir->kind())
                  {
                  case LIR::LOAD_GLOBAL:
#if PRINT_CHUNK_CODE
                    target_lir->dest().front()->variable()->dump_info<true>();
#endif
                    
                    curr_lir->operand().push_back(new OperandKind(target_lir->dest().front()->variable()));
                    break;
                    
                  case LIR::STORE_GLOBAL:
#if PRINT_CHUNK_CODE
                    target_lir->operand().front()->variable()->dump_info<true>();
#endif
                    
                    curr_lir->operand().push_back(new OperandKind(target_lir->operand().front()->variable()));
                    break;
                    
                  case LIR::MOV_R_2_R:
#if PRINT_CHUNK_CODE
                    target_lir->dest().front()->variable()->dump_info<true>();
#endif

                    curr_lir->operand().push_back(new OperandKind(target_lir->dest().front()->variable()));
                    break;
                    
                  default:
                    assert(!"Should not reach here.");
                    break;
                  }
                  
#if PRINT_CHUNK_CODE
                  g_log_file << " -> ";
                  curr_lir->dest().front()->variable()->dump_info<true>();
                  g_log_file << std::endl;
#endif
                  
                  curr_lir->set_kind(LIR::MOV_R_2_R);
                }
                else
                {
                  // :NOTE: Wei 2005-May-25:
                  //
                  // I can't find the target LIR, then this means I can not replace this LOAD operation with a
                  // MOV instruction.
                }
              }
              // :NOTE: Wei 2005-May-23:
              //
              // Otherwise, add m to AVIN(X).
              // Furthermore, for load, add result value rv to AVIN.
              else
              {
                // :NOTE: Wei 2005-May-23:
                //
                // I cann't assertion the following statement, because of the following condition:
                //
                //          o
                /*         / \      */
                //      1 o   o 2
                //
                // Node 1 has: LOAD A-0 [A]
                // Node 2 has: LOAD A-1 [A]
                //
                // And Node 1 and 2 don't have any expression can change the memory state of A,
                // then A-0 and A-1 will have the same value in this SCCVN algorithm.
                //
                // However, in the elimination stage, this algorithm will detect that nither statements
                // can be removed.
                //
                // This, because of the above situation, 2 equivalent LOAD expressions could be both still alive.
                //
                // assert(curr_lir->dest().front()->variable() == curr_lir->dest().front()->variable()->scc_value_number());
                
                assert(curr_bb->in().size() > vector_idx);
                
                curr_bb->in()[vector_idx] |= bit_position;
                
                calculate_bit_position(curr_lir->dest().front()->variable()->idx(),
                                       vector_idx,
                                       bit_position);
                
                assert(0 == (curr_bb->in()[vector_idx] & bit_position));
                
                curr_bb->in()[vector_idx] |= bit_position;
              }
            }
            break;
            
          case MEMORY_OPERATION_STORE:
            {
              calculate_bit_position(gp_chunk->used_var_table().size() +
                                     curr_lir->memory_operation_idx(),
                                     vector_idx,
                                     bit_position);
              
              assert(curr_bb->in().size() > vector_idx);
              
              // :NOTE: Wei 2005-May-23:
              //
              // If memory operation with ID m belongs to AVIN(X),
              // m is redundant and removed.
              if (curr_bb->in()[vector_idx] & bit_position)
              {
                assert(1 == curr_lir->operand().size());
                assert(OperandKind::VARIABLE == curr_lir->operand().front()->kind());
                
#if CHECK_CHUNK_CODE
                assert(curr_lir->memory_use_list().size() == curr_lir->memory_def_list().size());
                
                for (std::vector<OperandKind *>::const_iterator use_iter = curr_lir->memory_use_list().begin(),
                       def_iter = curr_lir->memory_def_list().begin();
                     use_iter != curr_lir->memory_use_list().end();
                     ++use_iter, ++def_iter)
                {
                  assert(OperandKind::VARIABLE == (*use_iter)->kind());
                  assert(OperandKind::VARIABLE == (*def_iter)->kind());
                  
                  assert((*use_iter)->variable()->scc_value_number() ==
                         (*def_iter)->variable()->scc_value_number());
                }
#endif
                
#if PRINT_CHUNK_CODE
                g_log_file << "Remove STORE operation: ";
                curr_lir->operand().front()->variable()->dump_info<true>();
                g_log_file << std::endl;
#endif
                
                if (curr_lir->prev_lir() != 0)
                {
                  curr_lir->prev_lir()->set_next_lir(curr_lir->next_lir());
                }
                
                if (curr_lir->next_lir() != 0)
                {
                  curr_lir->next_lir()->set_prev_lir(curr_lir->prev_lir());
                }
                
                if (curr_lir == curr_bb->lir_normal_head())
                {
                  curr_bb->set_lir_normal_head(curr_lir->next_lir());
                  
                  assert(curr_bb->lir_normal_head() != 0);
                }
                
                if (curr_lir == curr_bb->lir_scalar_phi_head())
                {
                  curr_bb->set_lir_scalar_phi_head(curr_lir->next_lir());
                  
                  assert(curr_bb->lir_scalar_phi_head() != 0);
                }
                
                if (curr_lir == curr_bb->lir_head())
                {
                  curr_bb->set_lir_head(curr_lir->next_lir());
                  
                  assert(curr_bb->lir_head() != 0);
                }
                
                if (curr_lir == curr_bb->lir_tail())
                {
                  curr_bb->set_lir_tail(curr_lir->prev_lir());
                  
                  assert(curr_bb->lir_tail() != 0);
                  assert(curr_bb->lir_tail()->kind() != LIR::PHI);
                }
                
                // :NOTE: Wei 2005-May-27:
                //
                // Because this LIR will not be in the LIR list,
                // thus I have to delete it here to prevent memory leak.
                delete curr_lir;
              }
              // :NOTE: Wei 2005-May-23:
              //
              // Otherwise, add m to AVIN(X).
              else
              {
                curr_bb->in()[vector_idx] |= bit_position;
              }
            }
            break;
            
          case MEMORY_OPERATION_JSR:
          case MEMORY_OPERATION_FRAME:
            calculate_bit_position(gp_chunk->used_var_table().size() + curr_lir->memory_operation_idx(),
                                   vector_idx,
                                   bit_position);
            
            assert(curr_bb->in().size() > vector_idx);
            assert(0 == (curr_bb->in()[vector_idx] & bit_position));
            
            curr_bb->in()[vector_idx] |= bit_position;
            break;
            
          case NOT_INSIDE_VALUE_NUMBERING:
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
    }
  }
  
  inline void
  Chunk::generating_move_for_phi_operand()
  {
    // :NOTE: Wei 2005-Jan-26:
    //
    // for all blocks b do
    for (std::vector<BasicBlock *>::const_iterator bb_iter = m_bb_table.begin();
         bb_iter != m_bb_table.end();
         ++bb_iter)
    {
      if (0 == (*bb_iter)->lir_head())
      {
        continue;
      }
      else
      {
        if (LIR::PHI == (*bb_iter)->lir_scalar_phi_head()->kind())
        {
          assert((*bb_iter)->lir_normal_head() != (*bb_iter)->lir_head());
          
          // :NOTE: Wei 2005-Jan-26:
          //
          // This variable is used for computing the order of phi-operand.
          uint32_t i = 0;
          
          // :NOTE: Wei 2005-Jan-26:
          //
          // for all predecessors p of b do
          for (std::vector<BasicBlock *>::iterator iter = (*bb_iter)->pred_bb().begin();
               iter != (*bb_iter)->pred_bb().end();
               ++iter)
          {
            BasicBlock *target;
            
            // :NOTE: Wei 2005-Jan-26:
            //
            // if b.no_of_predecessors > 1 and p.no_of_successors > 1 then
            if (((*bb_iter)->pred_bb().size() > 1) && ((*iter)->succ_bb().size() > 1))
            {
              // :NOTE: Wei 2005-Jan-26:
              //
              // This is so-called 'critical edge'.
              
              // :NOTE: Wei 2005-Jan-26:
              //
              // insert a new block n between p and b
              BasicBlock * const new_bb = new BasicBlock(0);
              
              m_bb_table.push_back(new_bb);
              
              bool is_fall_throw = true;
              
              // :NOTE: Wei 2005-Mar-21:
              //
              // If the previous basic block has multiple success basic blocks,
              // then the last LIR instruction should be a jump type instruction.
              assert(true == (*iter)->lir_tail()->is_jmp_type());
              
              assert(0 == (*iter)->lir_tail()->dest().size());
              assert(1 == (*iter)->lir_tail()->operand().size());
              
              OperandKind * const operand = (*iter)->lir_tail()->operand().front();
              
              if (OperandKind::LABEL == operand->kind())
              {
                // :NOTE: Wei 2005-Mar-21:
                //
                // The only jumpping LIR whose operand kind is not BASIC_BLOCK is CALL,
                // and I think the last instruction of the previous basic block in this condition
                // should not be a CALL.
                assert(Label::BASIC_BLOCK == operand->label()->kind());
                
                if (operand->label()->bb() == (*bb_iter))
                {
                  // :NOTE: Wei 2005-Mar-22:
                  //
                  // This is a jumpping relationship, thus I will copy the last jump lir instruction
                  // to the new basic block.
                  Label * const label =
                    add_new_label(new Label(static_cast<LabelBB *>(0), *bb_iter));
                  
                  new_bb->append_lir<true>(new LIR(static_cast<LIRJmp *>(0), label));
                  
                  is_fall_throw = false;
                }
              }
              else
              {
                assert(OperandKind::VARIABLE == operand->kind());
                
                // :NOTE: Wei 2005-Mar-21:
                //
                // The only jumpping LIR whose operand kind is not LABEL is JMP_R.
                Label * const label =
                  add_new_label(new Label(static_cast<LabelBB *>(0), *bb_iter));
                
                new_bb->append_lir<true>(new LIR(static_cast<LIRJmp *>(0), label));
                
                is_fall_throw = false;
              }
              
              new_bb->add_succ_bb(is_fall_throw, *bb_iter);
              new_bb->add_pred_bb(*iter);
              
              (*iter)->replace_succ_bb_and_patch_last_jmp_label_if_possible(*bb_iter, new_bb);
              
              // :NOTE: Wei 2005-Mar-21:
              //
              // After this line, the basic block where (*iter) points changes to 'new_bb',
              // thus I must _not_ use (*iter) anymore after this line.
              (*bb_iter)->replace_pred_bb((*iter), new_bb);
              
              target = new_bb;
            }
            else
            {
              // :NOTE: Wei 2005-Jan-26:
              //
              // n <- p
              target = (*iter);
            }
            
            // :NOTE: Wei 2005-Jan-26:
            //
            // for each phi-function of b do
            for (LIR *curr_lir = (*bb_iter)->lir_scalar_phi_head();
                 curr_lir != (*bb_iter)->lir_normal_head();
                 curr_lir = curr_lir->next_lir())
            {
              assert((*bb_iter)->lir_normal_head()->kind() != LIR::PHI);
              assert(LIR::PHI == curr_lir->kind());
              
              // :NOTE: Wei 2005-Jan-26:
              //
              // i <- new RegMove(phi.opd(p)) // the phi-operand corresponding to p
              // append i to n
              
              Variable *new_var;
              
              switch ((curr_lir->operand())[i]->variable()->kind())
              {
              case Variable::GLOBAL:
                new_var = add_new_var<Variable::GLOBAL>(new Variable(static_cast<VarGlobal *>(0),
                                                                     (curr_lir->operand())[i]->variable()->global_var()));
                break;
                
              case Variable::TEMP:
                new_var = get_new_temp();
                break;
                
              case Variable::CONST:
              default:
                assert(!"Should not reach here.");
                new_var = 0;
                break;
              }
              
              // :NOTE: Wei 2005-Feb-10:
              //
              // Set the original SSA variable, and set the SSA number optionally.
              new_var->set_original_ssa_var((curr_lir->operand())[i]->variable()->original_ssa_var());
              
              target->insert_lir_before_the_last_jmp(new LIR(static_cast<LIRMov *>(0),
                                                             (curr_lir->operand())[i]->variable(),
                                                             new_var));
              
              // :Note: Wei 2005-Jan-26:
              //
              // phi.opd(p) <- i
              curr_lir->replace_PHI_operand(i, new_var);
              
              // :NOTE: Wei 2005-Jan-26:
              //
              // join i with phi
              
              assert(1 == curr_lir->dest().size());
              
              new_var->set_join_var(curr_lir->dest().front()->variable());
              
              // :NOTE: Wei 2005-May-19:
              //
              // This is especially needed for PHI nodes of TEMP variables,
              // because each TEMP variable has their own stack space,
              // however, these TEMP variables group should have the same stack space.
              new_var->set_mem_loc_var(curr_lir->dest().front()->variable());
            }
            
            assert(target->lir_head() != 0);
            
            ++i;
          }
        }
      }
    }
  }
  
  inline void
  Chunk::add_live_range(Variable * const var, BasicBlock * const bb, uint32_t const end_idx)
  {
    uint32_t range_start;
    
    // :NOTE: Wei 2005-Jan-27:
    //
    // if (b.first.n <= i.n <= b.last.n) then
    //   range <- [i.n, end[
    // else
    //   range <- [b.first.n, end[
    if ((bb->lir_normal_head()->idx() <= var->defined_lir().front()->idx()) &&
        (bb->lir_tail()->idx() >= var->defined_lir().front()->idx()))
    {
      range_start = var->defined_lir().front()->idx();
    }
    else
    {
      range_start = bb->lir_normal_head()->idx();
    }
    
    // :NOTE: Wei 2005-Jan-27:
    //
    // add range to interval[i.n] // merging adjacent ranges if possible
    var->add_live_range(range_start, end_idx);
  }
  
  inline void
  Chunk::computing_live_interval(BasicBlock * const bb)
  {
    // :NOTE: Wei 2005-Jan-26:
    //
    // for each block b do
    if (false == bb->is_computing_live_interval())
    {
      uint32_t vector_idx;
      uint32_t bit_position;
      
      // :NOTE: Wei 2005-Feb-16:
      //
      // ‰æùÁÖß Linear Scan Register Allocation in the Context of SSA Form and Register Constraints ?ôÁ? paper,
      // ??algorithm ??
      //
      // live <- {}
      // for each successor s of b do
      //   live <- live U s.liveIn
      //
      // ‰ΩÜÂ??∫Ë©≤ paper ?áË®≠ live variable analysis ?™ÊõøÊØè‰???basic block Ë®àÁ? live in,
      // ?Ä‰ª•È?Ë¶Å‰??¢Á? for loop ‰æÜË?ÁÆóÊ?‰∏Ä??basic block ??live out.
      //
      // ‰ΩÜÊòØ?ëÁ? live variable analysis ?¥Êé•Â∞±Ë?ÁÆóÊ???basic block ??live in ‰ª•Â? live out,
      // ?Ä‰ª•Â∞±‰∏çÈ?Ë¶Å‰?Ëø∞Á? for loop ‰∫?
      
      // :TODO: Wei 2005-Feb-16:
      //
      // ‰æùÁÖß Linear Scan Register Allocation in the Context of SSA Form and Register Constraints ?ôÁ? paper,
      // ÂÆÉË™™ SSA form ?Ø‰ª•Á∞°Â? live variable analysis ?ÑË?ÁÆ?
      // ?åÁ∞°?ñÁ??πÊ??®Ë©≤ÁØ?paper ??reference 9:
      // Adding Static Single Assignment Form and a Graph Coloring Register Allocator to the Java HotSpot Client Compiler.
      // ?åÂú®?ëÁ? compiler ?? ?ÄË¶ÅÂÖ©Ê¨°Á? live variable analysis,
      // ?ÜÂà•?®Ë??õÊ? SSA form ?Ñ‰??ç‰ª•?ä‰?Âæ? ?Ä‰ª•Ë??ªÁ??ãË©≤ÁØ?paper,
      // Â¶ÇÊ??üÁ??âÁ∞°?ñÁ??πÊ?, Â∞?implement it!
      std::vector<uint32_t> live(bb->out());
      
      assert(live.size() == bb->out().size());
      
      // :NOTE: Wei 2005-Jan-26:
      //
      // for each successor s of b do
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = bb->succ_bb().begin();
           iter != bb->succ_bb().end();
           ++iter)
      {
        // :NOTE: Wei 2005-Jan-27:
        //
        // for each phi-function in s do
        for (LIR *curr_lir = (*iter).bb()->lir_scalar_phi_head();
             curr_lir != (*iter).bb()->lir_normal_head();
             curr_lir = curr_lir->next_lir())
        {
          assert((*iter).bb()->lir_normal_head()->kind() != LIR::PHI);
          assert(LIR::PHI == curr_lir->kind());
          
          // :NOTE: Wei 2005-Mar-25:
          //
          // Change the join variable of the PHI function, because the variable which has the
          // smallest LIR number between the destination one and the operand ones should not
          // be the destination.
          //
          // :NOTE: Wei 2005-Mar-24:
          //
          // If this basic block has multiple predecessor,
          // then I will meet each PHI function multiple times,
          // however, I only need to modify the join variable of the PHI function
          // at the first time, thus I will check whether the join variable of the destination
          // is the same with itself to determine whether it is the first time or not.
          //
          // Because at first, I will set all PHI operands to join with the PHI destination.
          Variable * const dest_var = curr_lir->dest().front()->variable();
          
          if (dest_var->join_var() == dest_var)
          {
            Variable *new_join_var = dest_var;
            
            // :NOTE: Wei 2005-Mar-24:
            //
            // To find a PHI operand whose starting point is first.
            for (std::vector<OperandKind *>::const_iterator operand_iter = curr_lir->operand().begin();
                 operand_iter != curr_lir->operand().end();
                 ++operand_iter)
            {
              Variable * const var = (*operand_iter)->variable();
              
              assert(0 == var->live_interval());
              assert(new_join_var->defined_lir().front()->idx() != var->defined_lir().front()->idx());
              
              if (var->defined_lir().front()->idx() < new_join_var->defined_lir().front()->idx())
              {
                new_join_var = var;
              }
            }
            
            assert(new_join_var != dest_var);
            
            dest_var->set_join_var(new_join_var);
            
            if (dest_var->live_interval() != 0)
            {
              assert(0 == dest_var->live_interval()->preallocated_reg());
              
              new_join_var->set_live_interval(dest_var->live_interval());
              
              new_join_var->live_interval()->set_attached_var(new_join_var);
              
              dest_var->set_live_interval(0);
            }
            
            for (std::vector<OperandKind *>::const_iterator operand_iter = curr_lir->operand().begin();
                 operand_iter != curr_lir->operand().end();
                 ++operand_iter)
            {
              (*operand_iter)->variable()->set_join_var(new_join_var);
            }
          }
          
          // :NOTE: Wei 2005-Jan-27:
          //
          // live <- live U {phi.opd(b)}
          
#if CHECK_CHUNK_CODE
          calculate_bit_position(curr_lir->phi_def_var()->idx(), vector_idx, bit_position);
          
          assert(live.size() > vector_idx);
          assert(0 == (live[vector_idx] & bit_position));
#endif
          
          uint32_t const i = (*iter).bb()->pred_bb_idx(bb);
          
          calculate_bit_position((curr_lir->operand())[i]->variable()->idx(), vector_idx, bit_position);
          
#if CHECK_CHUNK_CODE
          assert(live.size() > vector_idx);
          assert(0 == (live[vector_idx] & bit_position));
#endif
          
          live[vector_idx] |= bit_position;
        }
      }
      
      // :NOTE: Wei 2005-Jan-27:
      //
      // for each instruction i in live do ADDRANGE(i, b, b.last.n+1)
      for (std::vector<Variable *>::const_iterator iter = m_used_var_table.begin();
           iter != m_used_var_table.end();
           ++iter)
      {
        calculate_bit_position((*iter)->idx(), vector_idx, bit_position);
        
        assert(live.size() > vector_idx);
        
        if ((live[vector_idx] & bit_position) != 0)
        {
#if CHECK_CHUNK_CODE
          assert(1 == (*iter)->defined_lir().size());
          
          if (1 == (*iter)->defined_lir().front()->dest().size())
          {
            assert((*iter) == (*iter)->defined_lir().front()->dest().front()->variable());
          }
          else
          {
            bool find = false;
            
            for (std::vector<OperandKind *>::const_iterator dest_iter = (*iter)->defined_lir().front()->dest().begin();
                 dest_iter != (*iter)->defined_lir().front()->dest().end();
                 ++dest_iter)
            {
              if ((*dest_iter)->variable() == (*iter))
              {
                assert(false == find);
                
                find = true;
              }
            }
            
            assert(true == find);
          }
#endif
          
          add_live_range((*iter), bb, bb->lir_tail()->idx() + 1);
        }
      }
      
      // :NOTE: Wei 2005-Jan-27:
      //
      // for all instructions i in b in reverse order do
      //
      // :NOTE: Wei 2005-Feb-15:
      //
      // Here, I should exclude PHI instructions.
      std::vector<Variable *> def_var;
      std::vector<Variable *> use_var;
      LIR const * const stop_lir = bb->lir_normal_head()->prev_lir();
      
      for (LIR *curr_lir = bb->lir_tail();
           curr_lir != stop_lir;
           curr_lir = curr_lir->prev_lir())
      {
        def_var.clear();
        use_var.clear();
        
        curr_lir->computing_def_use(def_var, use_var);
        
        // :NOTE: Wei 2005-Jan-27:
        //
        // live <- live - {i}
        
        if (def_var.size() != 0)
        {
          for (std::vector<Variable *>::const_iterator iter = def_var.begin();
               iter != def_var.end();
               ++iter)
          {
            calculate_bit_position((*iter)->idx(), vector_idx, bit_position);
            
            assert(live.size() > vector_idx);
            
            live[vector_idx] &= ~bit_position;
          }
        }
        
        // :NOTE: Wei 2005-Jan-27:
        //
        // for each operand opd of i do
        for (std::vector<Variable *>::const_iterator iter = use_var.begin();
             iter != use_var.end();
             ++iter)
        {
          // :NOTE: Wei 2005-Jan-27:
          //
          // if opd not belongs to live then
          calculate_bit_position((*iter)->idx(), vector_idx, bit_position);
          
          assert(live.size() > vector_idx);
          
          if (0 == (live[vector_idx] & bit_position))
          {
            // :NOTE: Wei 2005-Jan-27:
            //
            // live <- live U {opd}
            
            live[vector_idx] |= bit_position;
            
            // :NOTE: Wei 2005-Jan-27:
            //
            // ADDRANGE(opd, b, i.n)
            
            add_live_range((*iter), bb, curr_lir->idx());
          }
        }
      }
      
      bb->set_computing_live_interval();
      
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = bb->succ_bb().begin();
           iter != bb->succ_bb().end();
           ++iter)
      {
        computing_live_interval((*iter).bb());
      }
    }
  }
  
#if CHECK_CHUNK_CODE
  inline void
  Chunk::check_live_interval_and_lir_idx(LIR * const curr_lir, Variable * const var)
  {
    curr_lir->check_live_interval_preallocated_reg();
    
    if (var->live_interval() != 0)
    {
      if (LIR::PHI == curr_lir->kind())
      {
        // :NOTE: Wei 2005-Feb-24:
        //
        // Because I join the new generated variable (preparing_for_linear_scan() stage 1)
        // with the phi variable, thus the start point of the live interval of the phi variable
        // should be smaller than the instruction index of phi instruction.
        assert(curr_lir->idx() > var->live_interval()->start_point_idx());
        
        // :NOTE: Wei 2005-Feb-24:
        //
        // The start point should be the definition point of one phi operand.
        for (std::vector<OperandKind *>::const_iterator iter = curr_lir->operand().begin();
             iter != curr_lir->operand().end();
             ++iter)
        {
          // :NOTE: Wei 2005-Feb-24:
          //
          // Because each phi operand is joined with phi variable,
          // thus the live interval of them (phi operands) must be 0.
          assert(0 == (*iter)->variable()->live_interval());
          
          assert(1 == (*iter)->variable()->defined_lir().size());
          
          if ((*iter)->variable()->defined_lir().front()->idx() == var->live_interval()->start_point_idx())
          {
            return;
          }
        }
        
        assert(!"Should not reach here.");
      }
      else
      {        
        assert(curr_lir->idx() == var->live_interval()->start_point_idx());
        
        // :NOTE: Wei 2005-Feb-24:
        //
        // The start point should be also the definition point.
        for (std::vector<OperandKind *>::const_iterator iter = curr_lir->dest().begin();
             iter != curr_lir->dest().end();
             ++iter)
        {
          if ((*iter)->variable() == var)
          {
            return;
          }
        }
        
        assert(!"Should not reach here.");
      }
    }
  }
#endif
  
  inline bool
  Chunk::live_interval_is_compatible_internal(LiveInterval * const live1,
                                              LiveInterval * const live2)
  {
    // :NOTE: Wei 2005-Feb-25:
    //
    // x must be in a specific register and
    // the interval of y does not overlap any other interval to which x.reg
    // has been assigned (or vice versa).
    // More formally:
    // x.reg >= 0 and !(exist an interval iv: iv.reg = x.reg & interval[y.n] overlaps iv) or
    // y.reg >= 0 and !(exist an interval iv: iv.reg = y.reg & interval[x.n] overlaps iv)
    if ((live1->preallocated_reg() != 0) && (0 == live2->preallocated_reg()))
    {
      for (LiveInterval *curr = mp_unhandled_head;
           curr != 0;
           curr = curr->next_unhandled())
      {
        if ((curr != live1) && (curr != live2))
        {
          if ((curr->preallocated_reg() != 0) &&
              (curr->preallocated_reg()->kind() == live1->preallocated_reg()->kind()))
          {
            if (true == (live2->overlap(curr)))
            {
              return false;
            }
          }
        }
      }
      
      return true;
    }
    
    return false;
  }
  
  namespace
  {
#if CHECK_CHUNK_CODE
    inline void
    check_preallocated_reg_and_excluded_reg_overlap(LiveInterval const * const live)
    {
      if (live->preallocated_reg() != 0)
      {
        for (std::vector<Register::KindEnum>::const_iterator iter = live->excluded_reg().begin();
             iter != live->excluded_reg().end();
             ++iter)
        {
          assert(live->preallocated_reg()->kind() != (*iter));
        }
      }
    }
#endif
    
    inline bool
    is_preallocated_reg_and_excluded_reg_overlap(LiveInterval const * const live1,
                                                 LiveInterval const * const live2)
    {
      if (live1->preallocated_reg() != 0)
      {
        for (std::vector<Register::KindEnum>::const_iterator iter = live2->excluded_reg().begin();
             iter != live2->excluded_reg().end();
             ++iter)
        {
          if (live1->preallocated_reg()->kind() == (*iter))
          {
            return true;
          }
        }
      }
      
      return false;
    }
  }
  
  inline bool
  Chunk::live_interval_is_compatible(LiveInterval * const live1,
                                     LiveInterval * const live2)
  {
    // :NOTE: Wei 2005-Feb-25:
    //
    // 1) both do not have to be in specific registers, or
    if ((0 == live1->preallocated_reg()) &&
        (0 == live2->preallocated_reg()))
    {
      return true;
    }
    
    // :NOTE: Wei 2005-Apr-6:
    //
    // This is my extension to the register allocation.
    if (true == is_preallocated_reg_and_excluded_reg_overlap(live1, live2))
    {
      return false;
    }
    
    if (true == is_preallocated_reg_and_excluded_reg_overlap(live2, live1))
    {
      return false;
    }
    
    // :NOTE: Wei 2005-Mar-03:
    //
    // 2) both have to be in the same specific register, or...
    if ((live1->preallocated_reg() != 0) &&
        (live2->preallocated_reg() != 0) &&
        (live1->preallocated_reg()->kind() == live2->preallocated_reg()->kind()))
    {
      return true;
    }
    
    // :NOTE: Wei 2005-Feb-25:
    //
    // x must be in a specific register and
    // the interval of y does not overlap any other interval to which x.reg
    // has been assigned (or vice versa).
    // More formally:
    // x.reg >= 0 and !(exist an interval iv: iv.reg = x.reg & interval[y.n] overlaps iv) or
    // y.reg >= 0 and !(exist an interval iv: iv.reg = y.reg & interval[x.n] overlaps iv)
    if (true == live_interval_is_compatible_internal(live1, live2))
    {
      return true;
    }
    
    return live_interval_is_compatible_internal(live2, live1);
  }
  
  inline void
  Chunk::join_live_interval_internal(LiveInterval * const live1,
                                     LiveInterval * const live2)
  {
    // :NOTE: Wei 2005-Feb-25:
    //
    // interval[REP(y).n] <- i U j
    live1->move_merged_range_to_raw_range();
    
    for (std::vector<LiveRange>::const_iterator iter = live2->merged_range().begin();
         iter != live2->merged_range().end();
         ++iter)
    {
      live1->add_live_range((*iter).m_start, (*iter).m_end);
    }
    
    live1->merge_ranges<true>();
    live1->clear_bitset();
    
    // :NOTE: Wei 2005-Feb-25:
    //
    // drop interval[REP(x).n]
    //
    // I don't need to delete live2 here,
    // because each live interval is attached to a variable,
    // and when I delete variables, I will also delete the live interval attached to it,
    // if that variable has one.
    if (mp_unhandled_head == live2)
    {
      assert(live2->next_unhandled() != 0);
      
      mp_unhandled_head = live2->next_unhandled();
    }
    
    if (mp_unhandled_tail == live2)
    {
      assert(live2->prev_unhandled() != 0);
      
      mp_unhandled_tail = live2->prev_unhandled();
    }
    
    if (live2->prev_unhandled() != 0)
    {
      live2->prev_unhandled()->set_next_unhandled(live2->next_unhandled());
    }
    
    if (live2->next_unhandled() != 0)
    {
      live2->next_unhandled()->set_prev_unhandled(live2->prev_unhandled());
    }
    
#if PRINT_CHUNK_CODE_MORE_DETAIL
    live2->attached_var()->dump_info<true>();
    g_log_file << " joins to ";
    live1->attached_var()->dump_info<true>();
    g_log_file << std::endl;
#endif
    
    // x.join <- REP(y)
    live2->attached_var()->set_join_var(live1->attached_var()->join_var());
  }
  
  void
  Chunk::join_live_interval(LiveInterval *live1,
                            LiveInterval *live2)
  {
    assert(live1 != 0);
    assert(live2 != 0);
    
#if CHECK_CHUNK_CODE
    check_preallocated_reg_and_excluded_reg_overlap(live1);
    check_preallocated_reg_and_excluded_reg_overlap(live2);
#endif
    
    // :NOTE: Wei 2005-Feb-25:
    //
    // if i and j = {} and x and y are compatible then
    if ((false == live1->overlap(live2)) &&
        (true == live_interval_is_compatible(live1, live2)))
    {
      assert(((0 == live1->preallocated_reg()) && (0 == live2->preallocated_reg())) ||
             (((live1->preallocated_reg() != 0) && (live2->preallocated_reg() != 0)) &&
              (live1->preallocated_reg()->kind() == live2->preallocated_reg()->kind())) ||
             ((0 == live1->preallocated_reg()) && (live2->preallocated_reg() != 0)) ||
             ((live1->preallocated_reg() != 0) && (0 == live2->preallocated_reg())));
      
      assert(false == is_preallocated_reg_and_excluded_reg_overlap(live1, live2));
      assert(false == is_preallocated_reg_and_excluded_reg_overlap(live2, live1));
      
      // :NOTE: Wei 2005-Feb-25:
      //
      // I should not break the increasing point order of m_unhandled.
      if (live1->start_point_idx() > live2->start_point_idx())
      {
        LiveInterval * const temp = live1;
        
        live1 = live2;
        live2 = temp;
      }
      
      join_live_interval_internal(live1, live2);
      
      // :NOTE: Wei 2005-Mar-22:
      //
      // Set the preallocated reg if needed.
      if (live2->preallocated_reg() != 0)
      {
        if (live1->preallocated_reg() != 0)
        {
          assert(live1->preallocated_reg() != live2->preallocated_reg());
          assert(live1->preallocated_reg()->kind() == live2->preallocated_reg()->kind());
          
          // :NOTE: Wei 2005-Mar-22:
          //
          // I have to delete it here, because the life time of a Register object is bounded to
          // its attached LiveInterval, thus I have to delete it here to avoid the memory leak.
          if (true == live1->preallocated_reg()->is_fixed())
          {
            live2->clear_preallocated_reg<true>();
          }
          else
          {
            delete live1->preallocated_reg();
            
            live1->set_preallocated_reg(live2->preallocated_reg());
            
            live2->clear_preallocated_reg<false>();
          }
        }
        else
        {
          live1->set_preallocated_reg(live2->preallocated_reg());
          
          live2->clear_preallocated_reg<false>();
        }
      }
      
      // :NOTE: Wei 2005-Apr-04:
      //
      // Merge the excluded registers from live2 to live1.
      std::vector<uint32_t> regs(Register::bitset_size());
      
      for (std::vector<Register::KindEnum>::const_iterator iter = live1->excluded_reg().begin();
           iter != live1->excluded_reg().end();
           ++iter)
      {
        assert(false == Register::check_available(regs, *iter));
        
        Register::set_available(regs, *iter);
      }
      
      for (std::vector<Register::KindEnum>::const_iterator iter = live2->excluded_reg().begin();
           iter != live2->excluded_reg().end();
           ++iter)
      {
        if (false == Register::check_available(regs, *iter))
        {
          live1->add_excluded_reg(*iter);
          
          Register::set_available(regs, *iter);
        }
      }
    }
  }
  
  // :NOTE: Wei 2005-Jan-26:
  //
  // The following algorithm is originated from
  // "Linear scan register allocation in the context of SSA form and register constraints'.
  inline void
  Chunk::preparing_for_linear_scan()
  {
    // :NOTE: Wei 2005-Jan-26:
    //
    // Stage 1: Generating moves for phi-operands
    generating_move_for_phi_operand();
    
    // :NOTE: Wei 2005-Jan-25:
    //
    // Stage 2: Numbering each LIR according to its BFS order.
    numbering_lir_by_bfs();
    
#if PRINT_CHUNK_CODE
    print_lir_code<true, true, false>();
#endif
    
    // :NOTE: Wei 2005-Feb-04:
    //
    // Stage 3: Global liveness analysis
    live_variable_analysis<NO_CONSIDER_MEMORY_VAR>();
    
    // :NOTE: Wei 2005-Jan-26:
    //
    // Stage 4: Computing live intervals
    computing_live_interval(mp_root_bb);
    
    for (std::vector<Variable *>::iterator iter = m_used_var_table.begin();
         iter != m_used_var_table.end();
         ++iter)
    {
      // :NOTE: Wei 2005-Mar-25:
      //
      // Every variable should be joined with itself, except for the destination
      // and almost all of the operands of the PHI function.
      if ((*iter)->join_var() == (*iter))
      {
        // :NOTE: Wei 2005-Apr-04:
        //
        // If this variable doesn't have a live interval attached to it,
        // then this must be the following 2 reasons:
        //
        // 1) If this variable be defined just once (and only once),
        //    then this means there is no use of this variable,
        //    but I still allocate a live interval for it to let it join the register
        //    allocation.
        //
        // 2) If this variable doesn't belong to any LIR
        //    then this means the Basic Block of this Variable is a dead basic block,
        //    and this variable will not join any following stages.
        //    Thus I simply pass it throw.
        //    (I think this is a kind of 'Dead Code Elimination')
        if (0 == (*iter)->live_interval())
        {
          if (1 == (*iter)->defined_lir().size())
          {
            LIR const * const lir = (*iter)->defined_lir().front();
            
            (*iter)->add_live_range(lir->idx(), lir->idx());
          }
          else
          {
            assert(0 == (*iter)->defined_lir().size());
            assert(0 == (*iter)->defined_bb().size());
            
            continue;
          }
        }
        
        assert((*iter)->live_interval() != 0);
        
        (*iter)->live_interval()->merge_ranges<false>();
      }
#if CHECK_CHUNK_CODE
      else
      {
        // :NOTE: Wei 2005-Mar-25:
        //
        // This variable should be the destination of a PHI function,
        // or be one of the PHI operands of a PHI function.
        assert(0 == (*iter)->live_interval());
      }
#endif
    }
    
    // :NOTE: Wei 2005-Feb-23:
    //
    // Collect live intervals
    for (std::list<LIR *>::iterator curr_lir_iter = m_all_lir_by_bfs.begin();
         curr_lir_iter != m_all_lir_by_bfs.end();
         ++curr_lir_iter)
    {
      // :NOTE: Wei 2005-Feb-04:
      //
      // The LiveInterval vector should be sorted by increasing start point order.
      for (std::vector<OperandKind *>::const_iterator iter = (*curr_lir_iter)->dest().begin();
           iter != (*curr_lir_iter)->dest().end();
           ++iter)
      {
        if (OperandKind::VARIABLE == (*iter)->kind())
        {
#if CHECK_CHUNK_CODE
          check_live_interval_and_lir_idx(*curr_lir_iter, (*iter)->variable());
#endif
          
          if ((*iter)->variable()->live_interval() != 0)
          {
            LiveInterval * const interval = (*iter)->variable()->live_interval();
            
            if (0 == mp_unhandled_head)
            {
              interval->set_prev_unhandled(0);
              
              mp_unhandled_head = interval;
            }
            else
            {
              interval->set_prev_unhandled(mp_unhandled_tail);
              
              mp_unhandled_tail->set_next_unhandled(interval);
            }
            
            mp_unhandled_tail = interval;
            
            mp_unhandled_tail->set_next_unhandled(0);
            
#if PRINT_CHUNK_CODE_MORE_DETAIL
            mp_unhandled_tail->dump_info();
            g_log_file << std::endl;
#endif
          }
        }
      }
    }
    
#if PRINT_CHUNK_CODE_MORE_DETAIL
    g_log_file << "=================="
               << std::endl;
#endif
    
    // :NOTE: Wei 2005-Feb-26:
    //
    // Join live intervals
    for (std::list<LIR *>::iterator curr_lir_iter = m_all_lir_by_bfs.begin();
         curr_lir_iter != m_all_lir_by_bfs.end();
         ++curr_lir_iter)
    {
      (*curr_lir_iter)->join_live_interval();
    }
    
#if CHECK_CHUNK_CODE
    LiveInterval *prev = 0;
    
    // :NOTE: Wei 2005-Feb-28:
    //
    // unhandled interval is sorted by increasing start point.
    for (LiveInterval *curr = mp_unhandled_head;
         curr != 0;
         curr = curr->next_unhandled())
    {
      if (0 == prev)
      {
        prev = curr;
      }
      else
      {
        assert(curr->start_point_idx() >= prev->start_point_idx());
      }
      
#if PRINT_CHUNK_CODE_MORE_DETAIL
      curr->dump_info();
      
      g_log_file << std::endl;
#endif
    }
#endif
  }
  
  namespace
  {
    template<bool T_add_unlimit>
    inline void
    compute_live_weight(std::vector<Register::RegisterWeight> &weight,
                        LiveInterval * const live1,
                        LiveInterval * const live2)
    {
      if (true == live1->overlap(live2))
      {
        uint32_t const idx = Register::array_index(live1->preallocated_reg()->kind());
        
        if (Register::UNLIMITED_VALUE == weight[idx].m_weight)
        {
          return;
        }
        else
        {
          if (true == T_add_unlimit)
          {
            weight[idx].m_weight = Register::UNLIMITED_VALUE;
          }
          else
          {
            weight[idx].m_weight += live1->attached_var()->access_times();
            
            // :NOTE: Wei 2005-Mar-01:
            //
            // Make sure that the incremented value should less than the unlimited value I set.
            assert(weight[idx].m_weight < Register::UNLIMITED_VALUE);
          }
        }
      }
    }
  }
  
  inline void
  Chunk::linear_scan_register_allocation_real()
  {
    std::list<LiveInterval *> active;
    std::list<LiveInterval *> inactive;
    std::list<LiveInterval *> handled;
    std::vector<uint32_t> free(Register::bitset_size());
    std::vector<Register::RegisterWeight> weight;
    
    Register::set_init_available_set(free);
    
    // :NOTE: Wei 2005-Feb-16:
    //
    // while unhandled != {} do
    while (mp_unhandled_head != 0)
    {
      // :NOTE: Wei 2005-Feb-16:
      //
      // cur <- pick and remove the first interval from unhandled
      LiveInterval * const cur = mp_unhandled_head;
      
      mp_unhandled_head = mp_unhandled_head->next_unhandled();
      
#if PRINT_CHUNK_CODE_MORE_DETAIL
      g_log_file << "alloc ";
      
      cur->dump_info();
      
      g_log_file << " free=0x"
                 << std::hex
                 << free.front();
#endif
      
#if CHECK_CHUNK_CODE
      for (std::list<LiveInterval *>::const_iterator iter = active.begin();
           iter != active.end();
           ++iter)
      {
        assert((*iter)->preallocated_reg() != 0);
        
        assert(false == Register::check_available(free, (*iter)->preallocated_reg()));
      }
#endif
      
      // :NOTE: Wei 2005-Feb-16:
      //
      // check for active intervals that expired
      //
      // :NOTE: Wei 2005-Feb-16:
      //
      // for each interval i in active do
      std::list<LiveInterval *>::iterator iter = active.begin();
      
      while (iter != active.end())
      {
        // :NOTE: Wei 2005-Feb-20:
        //
        // if i ends before cur.beg then
        if ((*iter)->end_point_idx() <= cur->start_point_idx())
        {
          if (((*iter)->start_point_idx() == (*iter)->end_point_idx()) &&
              ((*iter)->end_point_idx() == cur->start_point_idx()))
          {
            // :NOTE: Wei 2005-Feb-28:
            //
            // Do nothing.
          }
          else
          {
            // :NOTE: Wei 2005-Feb-20:
            //
            // move i to handled and add i.reg to free
            Register::set_available(free, (*iter)->preallocated_reg());
            
            handled.push_back(*iter);
            iter = active.erase(iter);
            
            continue;
          }
        }
        // :NOTE: Wei 2005-Feb-21:
        //
        // else if i does not overlap cur.beg then
        else if (false == (*iter)->overlap(cur->start_point_idx()))
        {
          // :NOTE: Wei 2005-Feb-21:
          //
          // move i to inactive and add i.reg to free
          Register::set_available(free, (*iter)->preallocated_reg());
          
          inactive.push_back(*iter);
          iter = active.erase(iter);
          
          continue;
        }
        
        ++iter;
      }
      
      // :NOTE: Wei 2005-Feb-21:
      //
      // check for inactive intervals that expired or become reactivated
      //
      // :NOTE: Wei 2005-Feb-21:
      //
      // for each interval i in inactive do
      iter = inactive.begin();
      
      while (iter != inactive.end())
      {
        // :NOTE: Wei 2005-Feb-20:
        //
        // if i ends before cur.beg then
        if ((*iter)->end_point_idx() <= cur->start_point_idx())
        {
          if (((*iter)->start_point_idx() == (*iter)->end_point_idx()) &&
              ((*iter)->end_point_idx() == cur->start_point_idx()))
          {
            // :NOTE: Wei 2005-Feb-28:
            //
            // Do nothing.
          }
          else
          {
            // :NOTE: Wei 2005-Feb-20:
            //
            // move i to handled
            handled.push_back(*iter);
            iter = inactive.erase(iter);
            
            continue;
          }
        }
        // :NOTE: Wei 2005-Feb-21:
        //
        // else if i overlaps cur.beg then
        else if (true == (*iter)->overlap(cur->start_point_idx()))
        {
          // :NOTE: Wei 2005-Feb-21:
          //
          // move i to active and remove i.reg from free
          Register::clear_available(free, (*iter)->preallocated_reg());
          
          assert((*iter)->preallocated_reg() != 0);
          
          active.push_back(*iter);
          iter = inactive.erase(iter);
          
          continue;
        }
        
        ++iter;
      }
      
#if CHECK_CHUNK_CODE
      if (cur->preallocated_reg() != 0)
      {
        assert(true == cur->preallocated_reg()->is_fixed());
        
        assert(true == Register::check_available(free, cur->preallocated_reg()));
      }
#endif
      
      // :NOTE: Wei 2005-Feb-28:
      //
      // collect available registers in f
      //
      // f <- free
      std::vector<uint32_t> f = free;
      
#if PRINT_CHUNK_CODE_MORE_DETAIL
      g_log_file << " f=0x"
                 << std::hex
                 << f.front();
#endif
      
      // :NOTE: Wei 2005-Feb-21:
      //
      // for each interval i in inactive that overlaps cur do f <- f - {i.reg}
      for (iter = inactive.begin(); iter != inactive.end(); ++iter)
      {
        if (true == (*iter)->overlap(cur))
        {
#if PRINT_CHUNK_CODE_MORE_DETAIL
          g_log_file << ", kick(in) ";
          (*iter)->dump_info();
#endif
          
          Register::clear_available(f, (*iter)->preallocated_reg());
        }
      }
      
#if CHECK_CHUNK_CODE
      if (cur->preallocated_reg() != 0)
      {
        assert(true == cur->preallocated_reg()->is_fixed());
        
        assert(true == Register::check_available(f, cur->preallocated_reg()));
      }
#endif
      
      // :NOTE: Wei 2005-Feb-28:
      //
      // for each fixed interval i in unhandled that overlaps cur do f <- f - {i.reg}
      for (LiveInterval *unhandled = mp_unhandled_head;
           unhandled != 0;
           unhandled = unhandled->next_unhandled())
      {
        if (unhandled->preallocated_reg() != 0)
        {
          if (true == unhandled->overlap(cur))
          {
#if CHECK_CHUNK_CODE
            if (cur->preallocated_reg() != 0)
            {
              assert(true == cur->preallocated_reg()->is_fixed());
              
              assert(cur->preallocated_reg()->kind() != unhandled->preallocated_reg()->kind());
            }
#endif
            
#if PRINT_CHUNK_CODE_MORE_DETAIL
            g_log_file << ", kick(un) ";
            unhandled->dump_info();
#endif
            
            Register::clear_available(f, unhandled->preallocated_reg());
          }
        }
      }
      
#if CHECK_CHUNK_CODE
      if (cur->preallocated_reg() != 0)
      {
        assert(true == cur->preallocated_reg()->is_fixed());
        
        assert(true == Register::check_available(f, cur->preallocated_reg()));
      }
#endif
      
      // :NOTE: Wei 2005-Apr-6:
      //
      // This is my linear scan extension.
      // Because some x86 instruction eats r8, thus doesn't eat ESI or EDI,
      // thus I have to take care of this situation.
      for (std::vector<Register::KindEnum>::const_iterator iter = cur->excluded_reg().begin();
           iter != cur->excluded_reg().end();
           ++iter)
      {
        Register::clear_available(f, *iter);
      }
      
      // :NOTE: Wei 2005-Feb-28:
      //
      // select a register from f
      //
      // if f = {} then
      if (true == Register::is_empty(f))
      {
        // :NOTE: Wei 2005-Feb-28:
        //
        // ASSIGNMEMLOC(cur)
        
        // :NOTE: Wei 2005-Feb-28:
        //
        // for all registers r do w[r] <- 0 // clear register weights
        Register::register_init(weight);
        
        // :NOTE: Wei 2005-Feb-28:
        //
        // for all intervals i in active, inactive and (fixed) unhandled do
        //   if i overlaps cur then w[i.reg] <- w[i.reg] + i.weight // if fixed i.weight = oo
        for (iter = active.begin(); iter != active.end(); ++iter)
        {
          assert((*iter)->preallocated_reg() != 0);
          
          if (true == (*iter)->preallocated_reg()->is_fixed())
          {
            compute_live_weight<true>(weight, *iter, cur);
          }
          else
          {
            compute_live_weight<false>(weight, *iter, cur);
          }
        }
        
        for (iter = inactive.begin(); iter != inactive.end(); ++iter)
        {
          assert((*iter)->preallocated_reg() != 0);
          
          if (true == (*iter)->preallocated_reg()->is_fixed())
          {
            compute_live_weight<true>(weight, *iter, cur);
          }
          else
          {
            compute_live_weight<false>(weight, *iter, cur);
          }
        }
        
        for (LiveInterval *curr = mp_unhandled_head;
             curr != 0;
             curr = curr->next_unhandled())
        {
          if (curr->preallocated_reg() != 0)
          {
            assert(true == curr->preallocated_reg()->is_fixed());
            
            compute_live_weight<true>(weight, curr, cur);
          }
        }
        
        // :NOTE: Wei 2005-Apr-6:
        //
        // This is my linear scan extension.
        // Because some x86 instruction eats r8, thus doesn't eat ESI or EDI,
        // thus I have to take care of this situation.
        for (std::vector<Register::KindEnum>::const_iterator excluded_iter = cur->excluded_reg().begin();
             excluded_iter != cur->excluded_reg().end();
             ++excluded_iter)
        {
          uint32_t const idx = Register::array_index(*excluded_iter);
          
          weight[idx].m_weight = Register::UNLIMITED_VALUE;
        }
        
        // :NOTE: Wei 2005-Mar-01:
        //
        // find r such that w[r] is a minimum
        std::sort(weight.begin(), weight.end());
        
        // :NOTE: Wei 2005-Mar-01:
        //
        // if cur.weight < w[r] then
        assert(cur->attached_var()->access_times() < Register::UNLIMITED_VALUE);
        
#if PRINT_CHUNK_CODE_MORE_DETAIL
        g_log_file << ": "
                   << cur->attached_var()->access_times()
                   << ": ";
        
        for (std::vector<Register::RegisterWeight>::const_iterator weight_iter = weight.begin();
             weight_iter != weight.end();
             ++weight_iter)
        {
          g_log_file << " ";
          (*weight_iter).dump_info();
          g_log_file << " ";
        }
        
        g_log_file.flush();
#endif
        
        if (cur->attached_var()->access_times() < weight.front().m_weight)
        {
          // :NOTE: Wei 2005-Mar-01:
          //
          //   assign a memory location to cur and move cur to handled
          
#if PRINT_CHUNK_CODE_MORE_DETAIL
          g_log_file << " -> mem";
#endif
          
          assert(0 == cur->preallocated_reg());
          
          handled.push_back(cur);
        }
        // :NOTE: Wei 2005-Mar-01:
        //
        // else // assign memory locations to the intervals occupied by r
        else
        {          
          // :NOTE: Wei 2005-Mar-01:
          //
          //   move all active or inactive intervals to which r was assigned to handled
          //   assign memory locations to them
          iter = active.begin();
          
          while (iter != active.end())
          {
            assert((*iter)->preallocated_reg() != 0);
            
            if ((*iter)->preallocated_reg()->kind() == weight.front().m_kind)
            {
              assert(false == (*iter)->preallocated_reg()->is_fixed());
              
              (*iter)->clear_preallocated_reg<true>();
              
              handled.push_back(*iter);
              iter = active.erase(iter);
              
              continue;
            }
            
            ++iter;
          }
          
          iter = inactive.begin();
          
          while (iter != inactive.end())
          {
            assert((*iter)->preallocated_reg() != 0);
            
            if ((*iter)->preallocated_reg()->kind() == weight.front().m_kind)
            {
              // :NOTE: Wei 2005-Mar-27:
              //
              // If an inactive interval doesn't overlap 'cur',
              // then that interval should not be preallocated to a specified register.
              if (false == (*iter)->preallocated_reg()->is_fixed())
              {
                (*iter)->clear_preallocated_reg<true>();
                
                handled.push_back(*iter);
                iter = inactive.erase(iter);
                
                continue;
              }
#if CHECK_CHUNK_CODE
              else
              {
                assert(false == (*iter)->overlap(cur));
              }
#endif
            }
            
            ++iter;
          }
          
          assert(0 == cur->preallocated_reg());
          
          // :NOTE: Wei 2005-Mar-01:
          //
          //   cur.reg <- r
          cur->set_preallocated_reg(Register::create_reg(weight.front().m_kind));
          
          assert(cur->preallocated_reg() != 0);
          
#if PRINT_CHUNK_CODE_MORE_DETAIL
          g_log_file << " -> scratch others";
          cur->preallocated_reg()->dump_info();
#endif
          
          // :NOTE: Wei 2005-Mar-01:
          //
          //   move cur to active
          active.push_back(cur);
          
          Register::clear_available(free, cur->preallocated_reg());
        }
      }
      // :NOTE: Wei 2005-Feb-28:
      //
      // else
      //   if cur.reg < 0 then
      //     cur.reg <- any register in f
      //   free <- free - {cur.reg}
      //   move cur to active
      else
      {
        if (0 == cur->preallocated_reg())
        {
          cur->set_preallocated_reg(Register::create_free_reg(f));
        }
        
#if PRINT_CHUNK_CODE_MORE_DETAIL
        g_log_file << " -> ";
        cur->preallocated_reg()->dump_info();
#endif
        
        Register::clear_available(free, cur->preallocated_reg());
        
        assert(cur->preallocated_reg() != 0);
        
        active.push_back(cur);
      }
      
#if PRINT_CHUNK_CODE_MORE_DETAIL
      g_log_file << std::endl;
#endif
      
#if CHECK_CHUNK_CODE
      if (cur->preallocated_reg() != 0)
      {
        for (std::vector<Register::KindEnum>::const_iterator iter = cur->excluded_reg().begin();
             iter != cur->excluded_reg().end();
             ++iter)
        {
          assert(cur->preallocated_reg()->kind() != (*iter));
        }
      }
#endif
    }
  }
  
  inline void
  Chunk::linear_scan_register_allocation()
  {
    // :NOTE: Wei 2005-Jan-26:
    //
    // Preparing the IR for linear scan
    preparing_for_linear_scan();
    
    // :NOTE: Wei 2004-Oct-25:
    //
    // linear scan register allocation
    linear_scan_register_allocation_real();
  }
    
  //////////////////////////////////// Public /////////////////////////////////
  //================================ Operation ================================
  
  BasicBlock *
  Chunk::get_new_basicblock()
  {
    BasicBlock * const new_bb = new BasicBlock(0);
    
    m_bb_table.push_back(new_bb);
    
    return new_bb;
  }
  
  void
  Chunk::check_end_a_cond_block(uint32_t const changed_cond)
  {
    assert(changed_cond != 0);
    
    switch (m_old_cond)
    {
    case EQ:
      if (changed_cond & CondCodeTranslationTraits<EQ>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case NE:
      if (changed_cond & CondCodeTranslationTraits<NE>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case CS:
      if (changed_cond & CondCodeTranslationTraits<CS>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case CC:
      if (changed_cond & CondCodeTranslationTraits<CC>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case MI:
      if (changed_cond & CondCodeTranslationTraits<MI>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case PL:
      if (changed_cond & CondCodeTranslationTraits<PL>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case VS:
      if (changed_cond & CondCodeTranslationTraits<VS>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case VC:
      if (changed_cond & CondCodeTranslationTraits<VC>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case HI:
      if (changed_cond & CondCodeTranslationTraits<HI>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case LS:
      if (changed_cond & CondCodeTranslationTraits<LS>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case GE:
      if (changed_cond & CondCodeTranslationTraits<GE>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case LT:
      if (changed_cond & CondCodeTranslationTraits<LT>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case GT:
      if (changed_cond & CondCodeTranslationTraits<GT>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case LE:
      if (changed_cond & CondCodeTranslationTraits<LE>::PRELOAD_STATUS_FLAG)
      {
        m_flags_no_change = false;
      }
      break;
      
    case AL:
      // :NOTE: Wei 2004-Nov-2:
      //
      // Because AL doesn't belongs to any condition block,
      // thus we don't need to break any condition block.
      break;
      
    case NV:
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  void
  Chunk::emit_native_code_real(BasicBlock * const bb)
  {
    if (false == bb->is_finish_emit_native_code())
    {
      bb->set_native_buffer_offset(m_native_code_buffer.size());
      
      // :NOTE: Wei 2005-May-04:
      //
      // If the basic block isn't reached from any other basic blocks,
      // then that basic block will not generate any LIR instructions,
      // thus the following 'for loop' will not emit any native codes.
      // That's to say, I implement a simple dead basic blocks elimination.
      for (LIR *curr_lir = bb->lir_normal_head();
           curr_lir != 0;
           curr_lir = curr_lir->next_lir())
      {
        curr_lir->set_native_buffer_offset(m_native_code_buffer.size());
        
        curr_lir->emit_native_code(m_native_code_buffer);
      }
      
#if CHECK_CHUNK_CODE
      bool find = false;
#endif
      
      std::vector<BasicBlock::SuccBBInfo> &succ = bb->succ_bb();
      
      // :NOTE: Wei 2005-Mar-03:
      //
      // Make sure at most one fall throw successive basic block,
      // And that one basic block is in front of any other ones.
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = succ.begin();
           iter != succ.end();
           ++iter)
      {
#if CHECK_CHUNK_CODE
        if (true == find)
        {
          assert(false == (*iter).fall_throw());
        }
        else
        {
#endif
          if (true == (*iter).fall_throw())
          {
            assert(iter == succ.begin());
            
            // :NOTE: Wei 2005-Mar-23:
            //
            // If the successive fall throw basic block has already finished the native code
            // emission, then I have to append a JMP native instruction to the last of the
            // current basic block.
            if (true == (*iter).bb()->is_finish_emit_native_code())
            {
              // :NOTE: Wei 2005-Mar-22:
              //
              // The fall throw basic block can not be append right after the current one,
              // so I have to append a new JMP native instructino in the current one.
              jmp_to_label(m_native_code_buffer,
                           add_new_label(new Label(static_cast<LabelBB *>(0), (*iter).bb())));
            }
            
#if CHECK_CHUNK_CODE
            find = true;
#endif
          }
#if CHECK_CHUNK_CODE
        }
#endif
      }
      
      for (std::vector<BasicBlock *>::const_iterator iter = bb->assistant_bb().begin();
           iter != bb->assistant_bb().end();
           ++iter)
      {
#if CHECK_CHUNK_CODE
        // :NOTE: Wei 2005-Mar-21:
        //
        // If this basic block has assistant basic blocks, then it should not have one
        // fall throw basic block.
        for (std::vector<BasicBlock::SuccBBInfo>::const_iterator check_iter = succ.begin();
             check_iter != succ.end();
             ++check_iter)
        {
          assert(false == (*check_iter).fall_throw());
        }
#endif
        
        (*iter)->set_native_buffer_offset(m_native_code_buffer.size());
        
        assert(0 == (*iter)->mir_tail()->next_mir());
        
        for (MIR *curr_mir = (*iter)->mir_head();
             curr_mir != 0;
             curr_mir = curr_mir->next_mir())
        {
          curr_mir->emit_native_code(m_native_code_buffer);
        }
      }
      
      bb->set_finish_emit_native_code();
      
      for (std::vector<BasicBlock::SuccBBInfo>::const_iterator iter = succ.begin();
           iter != succ.end();
           ++iter)
      {
        emit_native_code_real((*iter).bb());
      }
    }
  }
  
  inline void
  Chunk::emit_native_code(BasicBlock * const bb)
  {
    assert(0 == m_native_code_buffer.size());
    
    m_native_code_buffer.reserve(gp_core->average_native_code_buffer_size() * m_length);
    
    emit_native_prologue_codes(m_native_code_buffer);
    
    // :NOTE: Wei 2005-Mar-04:
    //
    // Make sure that I have already emitted the prologue part of the native codes.
    assert(m_native_code_buffer.size() != 0);
    
    emit_native_code_real(bb);
    
    gp_core->calculate_average_native_code_buffer_size(m_native_code_buffer.size(), m_length);
  }
  
  uint32_t
  Chunk::stack_size() const
  {
    return ((m_temp_var_number + NEEDED_SCRATCH_REG_NUMBER) << 2);
  }
  
  void
  Chunk::link_labels()
  {
    for (std::vector<Label *>::const_iterator iter = m_label_table.begin();
         iter != m_label_table.end();
         ++iter)
    {
      if ((*iter)->back_patch_info().size() != 0)
      {
        uint32_t target_addr
#if CHECK_CHUNK_CODE
          = 0
#endif
        ;
        
        switch ((*iter)->kind())
        {
        case Label::BASIC_BLOCK:
#if CHECK_CHUNK_CODE
          if ((*iter)->bb()->lir_head() != 0)
          {
            assert((*iter)->bb()->native_buffer_offset() == (*iter)->bb()->lir_normal_head()->native_buffer_offset());
          }
          
          assert((*iter)->bb()->native_buffer_offset() < m_native_code_buffer.size());
#endif
          
          target_addr = reinterpret_cast<uint32_t>(&(m_native_code_buffer[(*iter)->bb()->native_buffer_offset()]));
          break;
          
        case Label::ROUTINE:
          target_addr = reinterpret_cast<uint32_t>((*iter)->routine());
          break;
          
        case Label::MIR_INST:
          // :NOTE: Wei 2005-Mar-21:
          //
          // This should be an unused MIR_DATA32 entry.
        case Label::ARM_FIRST_INST:
        case Label::ARM_NORMAL_INST:
        default:
          assert(!"Should not reach here.");
          target_addr = 0;
          break;
        }
        
        assert(target_addr != 0);
        assert((*iter)->back_patch_info().size() != 0);
        
        uint32_t real_value
#if CHECK_CHUNK_CODE
          = 0
#endif
          ;
        
        for (std::vector<Label::PatchInfo>::const_iterator patch_iter = (*iter)->back_patch_info().begin();
             patch_iter != (*iter)->back_patch_info().end();
             ++patch_iter)
        {
          switch ((*patch_iter).type())
          {
          case Label::ABSOLUTE:
#if PRINT_CHUNK_CODE_MORE_DETAIL
            g_log_file << "absolute target addr => 0x"
                       << std::hex
                       << target_addr
                       << std::endl;
#endif
            
            real_value = target_addr;
            break;
            
          case Label::RELATIVE_TO_NEXT:
            assert((*patch_iter).offset() < m_native_code_buffer.size());
            assert(((*patch_iter).offset() + 4) <= m_native_code_buffer.size());
            
#if PRINT_CHUNK_CODE_MORE_DETAIL
            g_log_file << "relative target addr => 0x"
                       << std::hex
                       << target_addr
                       << " - 0x"
                       << reinterpret_cast<uint32_t>((&(m_native_code_buffer[(*patch_iter).offset() + 4])))
                       << " = 0x";
#endif
            
            real_value = target_addr - reinterpret_cast<uint32_t>((&(m_native_code_buffer[(*patch_iter).offset() + 4])));
            
#if PRINT_CHUNK_CODE_MORE_DETAIL
            g_log_file << real_value
                       << std::endl;
#endif
            break;
            
          default:
            assert(!"Should not reach here.");
            real_value = 0;
            break;
          }
          
          assert(((*patch_iter).offset() + 0) < m_native_code_buffer.size());
          assert(((*patch_iter).offset() + 1) < m_native_code_buffer.size());
          assert(((*patch_iter).offset() + 2) < m_native_code_buffer.size());
          assert(((*patch_iter).offset() + 3) < m_native_code_buffer.size());
          
          assert(0 == m_native_code_buffer[(*patch_iter).offset() + 0]);
          assert(0 == m_native_code_buffer[(*patch_iter).offset() + 1]);
          assert(0 == m_native_code_buffer[(*patch_iter).offset() + 2]);
          assert(0 == m_native_code_buffer[(*patch_iter).offset() + 3]);
          
          m_native_code_buffer[(*patch_iter).offset() + 0] = static_cast<uint8_t>(real_value >> 0);
          m_native_code_buffer[(*patch_iter).offset() + 1] = static_cast<uint8_t>(real_value >> 8);
          m_native_code_buffer[(*patch_iter).offset() + 2] = static_cast<uint8_t>(real_value >> 16);
          m_native_code_buffer[(*patch_iter).offset() + 3] = static_cast<uint8_t>(real_value >> 24);
        }
      }
#if CHECK_CHUNK_CODE
      else
      {
        switch ((*iter)->kind())
        {
        case Label::BASIC_BLOCK:
        case Label::ROUTINE:
          break;
          
        case Label::MIR_INST:
          // :NOTE: Wei 2005-Mar-21:
          //
          // This should be an unused MIR_DATA32 entry.
          assert(0 == (*iter)->mir());
          break;
          
        case Label::ARM_FIRST_INST:
        case Label::ARM_NORMAL_INST:
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
#endif
      
#if CHECK_CHUNK_CODE
      (*iter)->set_finish_linking();
#endif
    }
  }
  
#if CHECK_CHUNK_CODE
  void
  Chunk::check_all_used_labels_linked()
  {
    for (std::vector<BasicBlock *>::const_iterator iter = m_bb_table.begin();
         iter != m_bb_table.end();
         ++iter)
    {
      if (0 == (*iter)->lir_head())
      {
        continue;
      }
      else
      {
        (*iter)->check_label_linked();
      }
    }
  }
  
  void
  Chunk::check_bb_link_relationship()
  {
    for (std::vector<BasicBlock *>::const_iterator iter = m_bb_table.begin();
         iter != m_bb_table.end();
         ++iter)
    {
      if (0 == (*iter)->lir_head())
      {
        if (MIR_DATA32 == (*iter)->mir_head()->kind())
        {
          for (MIR *curr_mir = (*iter)->mir_head();
               curr_mir != 0;
               curr_mir = curr_mir->next_mir())
          {
            assert(MIR_DATA32 == curr_mir->kind());
            
            if (Label::MIR_INST == curr_mir->label<MIR_DATA32>()->kind())
            {
              assert(0 == curr_mir->label<MIR_DATA32>()->mir());
            }
          }
        }
        else
        {
#if PRINT_CHUNK_CODE
          g_log_file << "<opt> dead basic block elimination: "
                     << (*iter)->sequence_idx()
                     << std::endl;
#endif
        }
        
        continue;
      }
      
      for (LIR *curr_lir = (*iter)->lir_head();
           curr_lir != (*iter)->lir_tail();
           curr_lir = curr_lir->next_lir())
      {
        assert(false == curr_lir->is_jmp_type());
      }
      
      (*iter)->lir_tail()->check_bb_link_relationship();
    }
  }
#endif
  
  void
  Chunk::gen_dt_code()
  {
    assert(ST_THREADED_CODE == m_status);
    
    // :NOTE: Wei 2004-Nov-3:
    //
    // Make sure the first 15 entries of ARMware::Variable::GlobalVarEnum
    // is the same with the first 15 entries of ARMware::CoreRegNum.
    assert(Variable::CR_0  == static_cast<Variable::GlobalVarEnum>(CR_0));
    assert(Variable::CR_1  == static_cast<Variable::GlobalVarEnum>(CR_1));
    assert(Variable::CR_2  == static_cast<Variable::GlobalVarEnum>(CR_2));
    assert(Variable::CR_3  == static_cast<Variable::GlobalVarEnum>(CR_3));
    assert(Variable::CR_4  == static_cast<Variable::GlobalVarEnum>(CR_4));
    assert(Variable::CR_5  == static_cast<Variable::GlobalVarEnum>(CR_5));
    assert(Variable::CR_6  == static_cast<Variable::GlobalVarEnum>(CR_6));
    assert(Variable::CR_7  == static_cast<Variable::GlobalVarEnum>(CR_7));
    assert(Variable::CR_8  == static_cast<Variable::GlobalVarEnum>(CR_8));
    assert(Variable::CR_9  == static_cast<Variable::GlobalVarEnum>(CR_9));
    assert(Variable::CR_10 == static_cast<Variable::GlobalVarEnum>(CR_10));
    assert(Variable::CR_11 == static_cast<Variable::GlobalVarEnum>(CR_11));
    assert(Variable::CR_12 == static_cast<Variable::GlobalVarEnum>(CR_12));
    assert(Variable::CR_13 == static_cast<Variable::GlobalVarEnum>(CR_13));
    assert(Variable::CR_14 == static_cast<Variable::GlobalVarEnum>(CR_14));
    assert(Variable::CR_15 == static_cast<Variable::GlobalVarEnum>(CR_15));
    
    gp_chunk = this;
    
    translate_chunk_to_MIR();
    
#if PRINT_CHUNK_CODE
    dump_mir_info();
#endif
    
    eliminate_redundant_cond_code_calculation_and_link_bb();
    
    m_LIR_number = 0;
    
    assert(mp_mir_head->bb() == mp_root_bb);
    
    // :NOTE: Wei 2005-Mar-08:
    //
    // Because all MIR are added, and the basic block boundarys are cutted,
    // thus I can convert each label into the Label::BASIC_BLOCK form.
    //
    // The final compiling stage will use this basic block information stored
    // in these labels to back patch the final native code.
    for (std::vector<Label *>::const_iterator iter = m_label_table.begin();
         iter != m_label_table.end();
         ++iter)
    {
      switch ((*iter)->kind())
      {
      case Label::MIR_INST:
        if ((*iter)->mir() != 0)
        {
          (*iter)->convert_to_basicblock_label();
        }
        break;
        
      case Label::ROUTINE:
        break;
        
      case Label::BASIC_BLOCK:
      case Label::ARM_FIRST_INST:
      case Label::ARM_NORMAL_INST:
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    mp_root_bb->translate_MIR_to_LIR();
    
#if CHECK_CHUNK_CODE
    check_bb_link_relationship();
#endif
    
    // :NOTE: Wei 2005-May-20:
    //
    // The following stage is needed by SCCVN.
    {
      BasicBlock * const new_root_bb = new BasicBlock(0);
      m_bb_table.push_back(new_root_bb);
      
      LIR * const frame_lir = new LIR(static_cast<LIRFrame *>(0));
      
      new_root_bb->append_lir<true>(frame_lir);
      
      new_root_bb->add_succ_bb(true, mp_root_bb);
      
      mp_root_bb = new_root_bb;
    }
    
    convert_to_ssa_form();
    
    build_postorder_list(mp_root_bb);
    
    find_scc_component(mp_root_bb);
    
    preform_sccvn();
    
    AVAIL_analysis();
    
    fully_static_redundancy_elimination();
    
    linear_scan_register_allocation();
    
#if CHECK_CHUNK_CODE
    check_bb_link_relationship();
#endif
    
#if PRINT_CHUNK_CODE
    print_lir_code<true, true, false>();
#endif
    
    // :NOTE: Wei 2004-Dec-24:
    //
    // last stage: emit native codes.
    
    emit_native_code(mp_root_bb);
    
    link_labels();
    
#if CHECK_CHUNK_CODE
    check_all_used_labels_linked();
#endif
    
    cleanup_compiler_intermedia_product();
    
#if CHECK_CHUNK_CODE
    BasicBlock::check_delete_all_into_memory_pool();
    DAGNode::check_delete_all_into_memory_pool();
    MIR::check_delete_all_into_memory_pool();
    LIR::check_delete_all_into_memory_pool();
    Label::check_delete_all_into_memory_pool();
    LiveInterval::check_delete_all_into_memory_pool();
    Register::check_delete_all_into_memory_pool();
    OperandKind::check_delete_all_into_memory_pool();
    Variable::check_delete_all_into_memory_pool();
    Tuple::check_delete_all_into_memory_pool();
    DefinedBBPair::check_delete_all_into_memory_pool();
    ConditionBlock::check_delete_all_into_memory_pool();
#endif
    
    set_status(Chunk::ST_DYNAMIC);
  }
  
  // :NOTE: Wei 2004-Nov-5:
  //
  // @value_var the switch statment value.
  // @table_base_label the label of the jump table relative to the
  // switch statement.
  MIR *
  Chunk::insert_switch_statement_DT(Variable * const value_var,
                                    Label * const table_base_label)
  {
    Variable * const tmp1 = get_new_temp();
    
    append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRLoadLabel *>(0),
                               table_base_label,
                               tmp1));
    
    Variable * const tmp2 = get_new_temp();
    
    append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMul32 *>(0),
                               value_var,
                               find_const_var(4),
                               tmp2));
    
    Variable * const tmp3 = get_new_temp();
    
    append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                               tmp1,
                               tmp2,
                               tmp3));
    
    Variable * const tmp4 = get_new_temp();
    
    append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRLoad *>(0),
                               tmp3,
                               tmp4));
    
    MIR * const jmp_inst = append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRJump *>(0),
                                                              tmp4,
                                                              table_base_label));
    
    return jmp_inst;
  }
  
  MIR *
  Chunk::incre_PC_DT()
  {
    // :NOTE: Wei 2004-Nov-3:
    //
    // Update the value of PC, and I will eliminate useless updating
    // in the 'dead code elimination' stage later on.
    return append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                                              find_global_var(Variable::CR_PC),
                                              find_const_var(4),
                                              find_global_var(Variable::CR_PC)));
  }
#endif // #if ENABLE_DYNAMIC_TRANSLATOR
}
