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

#if ENABLE_DYNAMIC_TRANSLATOR

#include <iostream>

#include "../Chunk.hpp"
#include "../Core.hpp"
#include "../Log.hpp"

#include "Compiler.hpp"
#include "BitPosition.hpp"
#include "DAGNode.hpp"
#include "BasicBlock.hpp"
#include "MIR.hpp"
#include "ConditionBlock.hpp"

#include "x86/MachineDep.hpp"
#include "x86/LIR.hpp"
#include "x86/Pattern_DAGAdc.hpp"
#include "x86/Pattern_DAGAdd64.hpp"
#include "x86/Pattern_DAGAdd.hpp"
#include "x86/Pattern_DAGAnd.hpp"
#include "x86/Pattern_DAGASftRight.hpp"
#include "x86/Pattern_DAGCall.hpp"
#include "x86/Pattern_DAGCmpEQ.hpp"
#include "x86/Pattern_DAGCmp.hpp"
#include "x86/Pattern_DAGEor.hpp"
#include "x86/Pattern_DAGGoto.hpp"
#include "x86/Pattern_DAGGotoEQ.hpp"
#include "x86/Pattern_DAGGotoNE.hpp"
#include "x86/Pattern_DAGGotoCS.hpp"
#include "x86/Pattern_DAGGotoCC.hpp"
#include "x86/Pattern_DAGGotoMI.hpp"
#include "x86/Pattern_DAGGotoPL.hpp"
#include "x86/Pattern_DAGGotoVS.hpp"
#include "x86/Pattern_DAGGotoVC.hpp"
#include "x86/Pattern_DAGGotoHI.hpp"
#include "x86/Pattern_DAGGotoLS.hpp"
#include "x86/Pattern_DAGGotoGE.hpp"
#include "x86/Pattern_DAGGotoLT.hpp"
#include "x86/Pattern_DAGGotoGT.hpp"
#include "x86/Pattern_DAGGotoLE.hpp"
#include "x86/Pattern_DAGJump.hpp"
#include "x86/Pattern_DAGLoad.hpp"
#include "x86/Pattern_DAGLoadLabel.hpp"
#include "x86/Pattern_DAGLSftLeft.hpp"
#include "x86/Pattern_DAGLSftRight.hpp"
#include "x86/Pattern_DAGMov.hpp"
#include "x86/Pattern_DAGMul32.hpp"
#include "x86/Pattern_DAGNot.hpp"
#include "x86/Pattern_DAGOr.hpp"
#include "x86/Pattern_DAGProduceCBit.hpp"
#include "x86/Pattern_DAGSetCBit.hpp"
#include "x86/Pattern_DAGClearCBit.hpp"
#include "x86/Pattern_DAGRetrieve.hpp"
#include "x86/Pattern_DAGReturn.hpp"
#include "x86/Pattern_DAGRor.hpp"
#include "x86/Pattern_DAGRrx.hpp"
#include "x86/Pattern_DAGSbc.hpp"
#include "x86/Pattern_DAGSMul64.hpp"
#include "x86/Pattern_DAGSub.hpp"
#include "x86/Pattern_DAGTst.hpp"
#include "x86/Pattern_DAGUMul64.hpp"
#include "x86/Pattern_DAGVar.hpp"

#include "../Chunk.tcc"
#include "Variable.tcc"
#include "BasicBlock.tcc"
#include "BasicBlock_private.tcc"
#include "DAGNode.tcc"
#include "x86/LIR.tcc"

namespace ARMware
{
#if PRINT_CHUNK_CODE
  uint32_t BasicBlock::BB_SEQUENCE_IDX = 0;
#endif
  
  ///////////////////////////////// Private ///////////////////////////////////
  //============================== Operation ==================================
  
#if PRINT_CHUNK_CODE  
  void
  BasicBlock::dump_dag_tree(DAGNode * const node, uint32_t const level) const
  {
    for (uint32_t i = 0; i < level; ++i)
    {
      g_log_file << "  ";
    }
    
    node->dump_info();
    
    g_log_file << std::endl;
    
    std::deque<ChildInfo> const &children = node->children();
    
    for (std::deque<ChildInfo>::const_iterator iter = children.begin();
         iter != children.end();
         ++iter)
    {
      dump_dag_tree((*iter).node(), level + 1);
    }
  }
  
  void
  BasicBlock::dump_dominance_frontier()
  {
    for (std::vector<BasicBlock *>::const_iterator iter = m_dominance_frontier.begin();
         iter != m_dominance_frontier.end();
         ++iter)
    {
      g_log_file << (*iter)->sequence_idx()
                 << " ";
    }
  }
#endif
  
  inline DAGNode *
  BasicBlock::find_var_dag_node(Variable * const var)
  {
    assert(var != 0);
    
#if CHECK_CHUNK_CODE
    var->check_not_deleted();
#endif
    
    DAGNode *var_node = var->dag_node();
    
    if (0 == var_node)
    {
      // :NOTE: Wei 2004-Nov-8:
      //
      // Because I haven't define this variable in this basic block before,
      // thus there should be a new DAGVar node to represent this variable.
      //
      // And I don't set the dag_var pointer of this variable to this newly created
      // DAG node, because I want there should be no sharing on a DAGVar node.
      var_node = add_new_dag_node(new DAGNode(static_cast<DAGVar *>(0), var));
      
      var->set_attached_type(VAR_ATTACH_NORMAL);
    }
    
#if CHECK_CHUNK_CODE
    assert(true == var_node->is_usable());
    
    var_node->ensure_var_attached(var);
#endif
    
    return var_node;
  }
  
  namespace
  {
    template<VarAttachedTypeEnum T_attach>
    inline void
    add_child_node_internal(DAGNode * const node, DAGNode * const child_node)
    {
      node->children().push_back(ChildInfo(child_node, T_attach));
      
      child_node->add_ancestor<T_attach>(node);
    }
  }
  
  inline void
  BasicBlock::add_child_node(DAGNode * const node,
                             DAGNode * const child_node,
                             Variable * const child_var) const
  {
    switch (child_node->kind())
    {
    case DAG_UMUL64:
    case DAG_SMUL64:
    case DAG_ADD64:
      switch (child_var->attached_type())
      {
      case VAR_ATTACH_HI:
        add_child_node_internal<VAR_ATTACH_HI>(node, child_node);
        break;
        
      case VAR_ATTACH_LO:
        add_child_node_internal<VAR_ATTACH_LO>(node, child_node);
        break;
        
      case VAR_ATTACH_NORMAL:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    default:
      assert(VAR_ATTACH_NORMAL == child_var->attached_type());
      
      add_child_node_internal<VAR_ATTACH_NORMAL>(node, child_node);
      break;
    }
  }
  
  inline void
  BasicBlock::unlink_dag_node(DAGNode * const node)
  {
#if CHECK_CHUNK_CODE
    uint32_t i = 0;
#endif
    
    for (std::vector<DAGNode::DAGNodePair>::const_iterator iter = node->prev_next_dag_node_pair().begin();
         iter != node->prev_next_dag_node_pair().end();
         ++iter
#if CHECK_CHUNK_CODE
           , ++i
#endif
         )
    {
#if CHECK_CHUNK_CODE
      if ((*iter).mp_prev_dag_node != 0)
      {
        assert(((*iter).mp_prev_dag_node->prev_next_dag_node_pair())[(*iter).m_prev_dag_node_slot_idx].mp_next_dag_node == node);
        assert(((*iter).mp_prev_dag_node->prev_next_dag_node_pair())[(*iter).m_prev_dag_node_slot_idx].m_next_dag_node_slot_idx == i);
      }
      
      if ((*iter).mp_next_dag_node != 0)
      {
        assert(((*iter).mp_next_dag_node->prev_next_dag_node_pair())[(*iter).m_next_dag_node_slot_idx].mp_prev_dag_node == node);
        assert(((*iter).mp_next_dag_node->prev_next_dag_node_pair())[(*iter).m_next_dag_node_slot_idx].m_prev_dag_node_slot_idx == i);
      }
#endif
      
      if ((*iter).mp_prev_dag_node != 0)
      {
        ((*iter).mp_prev_dag_node->prev_next_dag_node_pair())[
          (*iter).m_prev_dag_node_slot_idx
         ].mp_next_dag_node = (*iter).mp_next_dag_node;
        
        ((*iter).mp_prev_dag_node->prev_next_dag_node_pair())[
          (*iter).m_prev_dag_node_slot_idx
         ].m_next_dag_node_slot_idx = (*iter).m_next_dag_node_slot_idx;
      }
      else
      {
        assert(iter == node->prev_next_dag_node_pair().begin());
        assert(node == mp_dag_node_head);
        
        mp_dag_node_head = (*iter).mp_next_dag_node;
      }
      
      if ((*iter).mp_next_dag_node != 0)
      {
        ((*iter).mp_next_dag_node->prev_next_dag_node_pair())[
          (*iter).m_next_dag_node_slot_idx
         ].mp_prev_dag_node = (*iter).mp_prev_dag_node;
        
        ((*iter).mp_next_dag_node->prev_next_dag_node_pair())[
          (*iter).m_next_dag_node_slot_idx
         ].m_prev_dag_node_slot_idx = (*iter).m_prev_dag_node_slot_idx;
      }
      else
      {
        assert((iter + 1) == node->prev_next_dag_node_pair().end());
        assert(node == mp_dag_node_tail);
        
        mp_dag_node_tail = (*iter).mp_prev_dag_node;
      }
    }
  }
  
  namespace
  {
    inline bool
    can_get_var_unique_value(ChildInfo const &info)
    {
      for (std::list<Variable *>::const_iterator iter = info.child_attached_var().begin();
           iter != info.child_attached_var().end();
           ++iter)
      {
        switch ((*iter)->kind())
        {
        case Variable::TEMP:
#if CHECK_CHUNK_CODE
          assert(((*iter)->dag_node() == info.node()) ||
                 (0 == (*iter)->dag_node()));
          
          if (0 == (*iter)->dag_node())
          {
            assert(true == (*iter)->is_act_as_global_var());
            assert(1 == info.child_attached_var().size());
          }
#endif
          break;
          
        case Variable::GLOBAL:
          if (0 == (*iter)->dag_node())
          {
          }
          else
          {
            return false;
          }
          break;
          
        case Variable::CONST:
          assert(1 == info.child_attached_var().size());
          assert(0 == (*iter)->dag_node());
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      
      return true;
    }
  }
  
  inline bool
  BasicBlock::dead_code_elimination_internal(DAGNode * const node)
  {
    // :NOTE: Wei 2004-Nov-12:
    //
    // Dead code elimination:
    // ----------------------
    // Roots in DAG tree with only 'unused' variables.
    
    switch (node->kind())
    {
    case DAG_CALL:
    case DAG_RETURN:
    case DAG_RETRIEVE:
    case DAG_CMP_EQ:
    case DAG_JUMP:
    case DAG_GOTO:
    case DAG_GOTO_EQ:
    case DAG_GOTO_NE:
    case DAG_GOTO_CS:
    case DAG_GOTO_CC:
    case DAG_GOTO_MI:
    case DAG_GOTO_PL:
    case DAG_GOTO_VS:
    case DAG_GOTO_VC:
    case DAG_GOTO_HI:
    case DAG_GOTO_LS:
    case DAG_GOTO_GE:
    case DAG_GOTO_LT:
    case DAG_GOTO_GT:
    case DAG_GOTO_LE:
      break;
      
    case DAG_MOV:
      // :NOTE: Wei 2004-Nov-12:
      //
      // If a DAGMov node which doesn't be needed,
      // then it is a dead code, too.
      
      assert(1 == node->children().size());
      
      if (false == node->is_needed())
      {
        ChildInfo const child_info = node->children().front();
        
        if (true == can_get_var_unique_value(child_info))
        {
          std::list<DAGNode *> &ancestor = node->ancestor<VAR_ATTACH_NORMAL>();
          
          for (std::list<DAGNode *>::const_iterator iter = ancestor.begin();
               iter != ancestor.end();
               ++iter)
          {
            // :NOTE: Wei 2004-Nov-26:
            //
            // I have to keep the order of the operands.
            (*iter)->replace_child(node, child_info);
          }
          
          child_info.node()->add_global_attached_var_ref_cnt(
            ancestor.size() * child_info.global_attached_var_ref_cnt());
          
          remove_dead_node<true, true, true>(node);
          
          return true;
        }
      }
      break;
      
    default:
      // :NOTE: Wei 2004-Nov-12:
      //
      // The rule of the dead code elimination:
      // Roots in DAG tree with only 'unused' variables.
      //
      // :NOTE: Wei 2004-Nov-12:
      //
      // Only nodes don't envolve condition flags calculation may be dead codes.
      if ((0 == (node->in_flags() | node->out_flags())) &&
          (0 == node->ancestor_number()))
      {
        if (false == node->is_needed())
        {
          remove_dead_node<true, true, true>(node);
          
          return true;
        }
      }
      break;
    }
    
    return false;
  }
  
  inline bool
  BasicBlock::dead_code_elimination_real()
  {
    bool finish = true;
    
    for (std::list<DAGNode *>::const_iterator iter = m_all_dag_nodes.begin();
         iter != m_all_dag_nodes.end();
         ++iter)
    {
      if (true == (*iter)->is_usable())
      {
        if (true == dead_code_elimination_internal(*iter))
        {
          assert(false == (*iter)->is_usable());
          
          finish = false;
        }
      }
    }
    
    return finish;
  }
  
  inline void
  BasicBlock::dead_code_elimination()
  {
    while (false == dead_code_elimination_real());
  }
  
  inline bool
  BasicBlock::constant_folding_real(DAGNode * const node)
  {
    if (0 == (node->in_flags() | node->out_flags()))
    {
      switch (node->kind())
      {
      case DAG_NOT:
        {
          assert(1 == node->children().size());
          
          ChildInfo const &child_info = node->children().front();
          DAGNode * const child = child_info.node();
          
          if (DAG_VAR_CONST == child->kind())
          {
            assert(1 == child->ancestor_number());
            
            // :NOTE: Wei 2004-Nov-14:
            //
            //     not
            //      |
            //    const
            Variable * const not_var =
              gp_chunk->find_const_var(~(child_info.peek_first_usable_attached_var()->const_num()));
            
#if PRINT_CHUNK_CODE
            g_log_file << "constant folding: ";
            node->dump_info();
            g_log_file << std::endl;
#endif
            
            // :NOTE: Wei 2005-Apr-15:
            //
            // I don't need to consider whether node->ancestor().size() == 0 or not,
            // because if a node which ancestor number is 0 and all attached variables are temp,
            // then that node will be eliminated in the dead code elimination stage.
            //
            //              |  all temp    |  not all temp
            // ----------------------------------------------
            //  ancestor=0  | xxxxxxxxx    |  need move node
            //  ancestor!=0 | single const |  need move node
            // ----------------------------------------------
            if (NEED_MOVE == determine_constant_folding_type(node))
            {
              // :NOTE: Wei 2004-Nov-14:
              //
              //     not         mov
              //      |     ->    |
              //    const       ~const
              node->set_kind(DAG_MOV);
              
              child->set_const_var(not_var);
            }
            else
            {
              // :NOTE: Wei 2004-Nov-14:
              //
              //     not        ~const
              //      |     ->
              //    const
              remove_dead_node<false, true, true>(node);
              remove_dead_node<false, true, true>(child);
              
              for (std::list<DAGNode *>::const_iterator iter = node->ancestor<VAR_ATTACH_NORMAL>().begin();
                   iter != node->ancestor<VAR_ATTACH_NORMAL>().end();
                   ++iter)
              {
                DAGNode * const const_node = add_new_dag_node(new DAGNode(static_cast<DAGVar *>(0), not_var));
                
                ChildInfo const child_info = ChildInfo(const_node, VAR_ATTACH_NORMAL);
                
                (*iter)->replace_child(node, child_info);
              }
            }
            
            return true;
          }
        }
        break;
        
      case DAG_ADD:
        return constant_folding_internal<DAGAdd>(node);
        
      case DAG_SUB:
        return constant_folding_internal<DAGSub>(node);
        
      case DAG_EOR:
        return constant_folding_internal<DAGEor>(node);
        
      case DAG_OR:
        return constant_folding_internal<DAGOr>(node);
        
      case DAG_AND:
        return constant_folding_internal<DAGAnd>(node);
        
      case DAG_LSFT_LEFT:
        return constant_folding_internal<DAGLSftLeft>(node);
        
      default:
        break;
      }
    }
    
    return false;
  }
  
  inline void
  BasicBlock::constant_folding()
  {
    bool finish;
    
    do
    {
      finish = true;
      
      for (std::list<DAGNode *>::const_iterator iter = m_all_dag_nodes.begin();
           iter != m_all_dag_nodes.end();
           ++iter)
      {
        if (true == (*iter)->is_usable())
        {
          bool const modify = constant_folding_real(*iter);
          
          if (true == modify)
          {
            finish = false;
          }
        }
      }
    }
    while (false == finish);
  }
  
  inline void
  BasicBlock::emission_lir_for_each_node_internal(DAGNode * const node)
  {
    switch (node->kind())
    {
    case DAG_VAR_TEMP:
    case DAG_VAR_GLOBAL:
    case DAG_VAR_CONST:
      Pattern_DAGVar::emit_lir_code(this, node);
      break;
      
    case DAG_CALL:
      Pattern_DAGCall::emit_lir_code(this, node);
      break;
      
    case DAG_RETURN:
      Pattern_DAGReturn::emit_lir_code(this, node);
      break;
      
    case DAG_RETRIEVE:
      Pattern_DAGRetrieve::emit_lir_code(this, node);
      break;
      
    case DAG_ADD:
      Pattern_DAGAdd::emit_lir_code(this, node);
      break;
      
    case DAG_ADD64:
      Pattern_DAGAdd64::emit_lir_code(this, node);
      break;
      
    case DAG_ADC:
      Pattern_DAGAdc::emit_lir_code(this, node);
      break;
      
    case DAG_MOV:
      Pattern_DAGMov::emit_lir_code(this, node);
      break;
      
    case DAG_AND:
      Pattern_DAGAnd::emit_lir_code(this, node);
      break;
      
    case DAG_OR:
      Pattern_DAGOr::emit_lir_code(this, node);
      break;
      
    case DAG_EOR:
      Pattern_DAGEor::emit_lir_code(this, node);
      break;
      
    case DAG_SUB:
      Pattern_DAGSub::emit_lir_code(this, node);
      break;
      
    case DAG_SBC:
      Pattern_DAGSbc::emit_lir_code(this, node);
      break;
      
    case DAG_TST:
      Pattern_DAGTst::emit_lir_code(this, node);
      break;
      
    case DAG_CMP:
      Pattern_DAGCmp::emit_lir_code(this, node);
      break;
      
    case DAG_CMP_EQ:
      Pattern_DAGCmpEQ::emit_lir_code(this, node);
      break;
      
    case DAG_NOT:
      Pattern_DAGNot::emit_lir_code(this, node);
      break;
      
    case DAG_MUL32:
      Pattern_DAGMul32::emit_lir_code(this, node);
      break;
      
    case DAG_UMUL64:
      Pattern_DAGUMul64::emit_lir_code(this, node);
      break;
      
    case DAG_SMUL64:
      Pattern_DAGSMul64::emit_lir_code(this, node);
      break;
      
    case DAG_LSFT_LEFT:
      Pattern_DAGLSftLeft::emit_lir_code(this, node);
      break;
      
    case DAG_LSFT_RIGHT:
      Pattern_DAGLSftRight::emit_lir_code(this, node);
      break;
      
    case DAG_ASFT_RIGHT:
      Pattern_DAGASftRight::emit_lir_code(this, node);
      break;
      
    case DAG_RRX:
      Pattern_DAGRrx::emit_lir_code(this, node);
      break;
      
    case DAG_ROR:
      Pattern_DAGRor::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO:
      Pattern_DAGGoto::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_EQ:
      Pattern_DAGGotoEQ::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_NE:
      Pattern_DAGGotoNE::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_CS:
      Pattern_DAGGotoCS::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_CC:
      Pattern_DAGGotoCC::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_MI:
      Pattern_DAGGotoMI::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_PL:
      Pattern_DAGGotoPL::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_VS:
      Pattern_DAGGotoVS::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_VC:
      Pattern_DAGGotoVC::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_HI:
      Pattern_DAGGotoHI::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_LS:
      Pattern_DAGGotoLS::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_GE:
      Pattern_DAGGotoGE::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_LT:
      Pattern_DAGGotoLT::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_GT:
      Pattern_DAGGotoGT::emit_lir_code(this, node);
      break;
      
    case DAG_GOTO_LE:
      Pattern_DAGGotoLE::emit_lir_code(this, node);
      break;
      
    case DAG_JUMP:
      Pattern_DAGJump::emit_lir_code(this, node);
      break;
      
    case DAG_PRODUCE_CBIT:
      Pattern_DAGProduceCBit::emit_lir_code(this, node);
      break;
      
    case DAG_SET_CBIT:
      Pattern_DAGSetCBit::emit_lir_code(this, node);
      break;
      
    case DAG_CLEAR_CBIT:
      Pattern_DAGClearCBit::emit_lir_code(this, node);
      break;
      
    case DAG_LOAD_LABEL:
      Pattern_DAGLoadLabel::emit_lir_code(this, node);
      break;
      
    case DAG_LOAD:
      Pattern_DAGLoad::emit_lir_code(this, node);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  inline void
  BasicBlock::emission_lir_for_each_node()
  {
    for (DAGNode *curr_node = mp_dag_node_head;
         curr_node != 0;
         curr_node = curr_node->get_next_emission_node())
    {
      assert(true == curr_node->is_usable());
      
#if CHECK_CHUNK_CODE
      if (curr_node->attached_var_amount() != 0)
      {
        assert(curr_node->prev_next_dag_node_pair().size() == curr_node->attached_var<VAR_ATTACH_LO>().size());
      }
#endif
      
      emission_lir_for_each_node_internal(curr_node);
    }
  }
  
  inline void
  BasicBlock::add_mir_into_dag(MIR * const mir)
  {
    switch (mir->kind())
    {
    case MIR_CALL_0:       add_CALL_0_type_mir_into_dag_internal<MIRCall_0>(mir); break;
    case MIR_CALL_1:       add_CALL_1_type_mir_into_dag_internal<MIRCall_1>(mir); break;
    case MIR_RETURN:       add_RETURN_type_mir_into_dag_internal<MIRReturn>(mir); break;
    case MIR_RETRIEVE:     add_RETRIEVE_type_mir_into_dag_internal<MIRRetrieve>(mir); break;
    case MIR_ADD:          add_ADD_type_mir_into_dag_internal<MIRAdd>(mir); break;
    case MIR_ADD64:        add_ADD64_type_mir_into_dag_internal<MIRAdd64>(mir); break;
    case MIR_ADC:          add_ADC_type_mir_into_dag_internal<MIRAdc>(mir); break;
    case MIR_MOV:          add_MOV_type_mir_into_dag_internal<MIRMov>(mir); break;
    case MIR_AND:          add_ADD_type_mir_into_dag_internal<MIRAnd>(mir); break;
    case MIR_OR:           add_ADD_type_mir_into_dag_internal<MIROr>(mir); break;
    case MIR_EOR:          add_ADD_type_mir_into_dag_internal<MIREor>(mir); break;
    case MIR_SUB:          add_ADD_type_mir_into_dag_internal<MIRSub>(mir); break;
    case MIR_SBC:          add_ADC_type_mir_into_dag_internal<MIRSbc>(mir); break;
    case MIR_TST:          add_ADD_type_mir_into_dag_internal<MIRTst>(mir); break;
    case MIR_CMP:          add_ADD_type_mir_into_dag_internal<MIRCmp>(mir); break;
    case MIR_CMP_EQ:       add_CMP_EQ_type_mir_into_dag_internal<MIRCmpEQ>(mir); break;
    case MIR_NOT:          add_MOV_type_mir_into_dag_internal<MIRNot>(mir); break;
    case MIR_MUL32:        add_ADD_type_mir_into_dag_internal<MIRMul32>(mir); break;
    case MIR_UMUL64:       add_UMUL64_type_mir_into_dag_internal<MIRUMul64>(mir); break;
    case MIR_SMUL64:       add_UMUL64_type_mir_into_dag_internal<MIRSMul64>(mir); break;
    case MIR_LSFT_LEFT:    add_ADD_type_mir_into_dag_internal<MIRLSftLeft>(mir); break;
    case MIR_LSFT_RIGHT:   add_ADD_type_mir_into_dag_internal<MIRLSftRight>(mir); break;
    case MIR_ASFT_RIGHT:   add_ADD_type_mir_into_dag_internal<MIRASftRight>(mir); break;
    case MIR_RRX:          add_ADC_type_mir_into_dag_internal<MIRRrx>(mir); break;
    case MIR_ROR:          add_ADD_type_mir_into_dag_internal<MIRRor>(mir); break;
    case MIR_GOTO:         add_GOTO_type_mir_into_dag_internal<MIRGoto>(mir); break;
    case MIR_GOTO_EQ:      add_GOTO_type_mir_into_dag_internal<MIRGotoEQ>(mir); break;
    case MIR_GOTO_NE:      add_GOTO_type_mir_into_dag_internal<MIRGotoNE>(mir); break;
    case MIR_GOTO_CS:      add_GOTO_type_mir_into_dag_internal<MIRGotoCS>(mir); break;
    case MIR_GOTO_CC:      add_GOTO_type_mir_into_dag_internal<MIRGotoCC>(mir); break;
    case MIR_GOTO_MI:      add_GOTO_type_mir_into_dag_internal<MIRGotoMI>(mir); break;
    case MIR_GOTO_PL:      add_GOTO_type_mir_into_dag_internal<MIRGotoPL>(mir); break;
    case MIR_GOTO_VS:      add_GOTO_type_mir_into_dag_internal<MIRGotoVS>(mir); break;
    case MIR_GOTO_VC:      add_GOTO_type_mir_into_dag_internal<MIRGotoVC>(mir); break;
    case MIR_GOTO_HI:      add_GOTO_type_mir_into_dag_internal<MIRGotoHI>(mir); break;
    case MIR_GOTO_LS:      add_GOTO_type_mir_into_dag_internal<MIRGotoLS>(mir); break;
    case MIR_GOTO_GE:      add_GOTO_type_mir_into_dag_internal<MIRGotoGE>(mir); break;
    case MIR_GOTO_LT:      add_GOTO_type_mir_into_dag_internal<MIRGotoLT>(mir); break;
    case MIR_GOTO_GT:      add_GOTO_type_mir_into_dag_internal<MIRGotoGT>(mir); break;
    case MIR_GOTO_LE:      add_GOTO_type_mir_into_dag_internal<MIRGotoLE>(mir); break;
    case MIR_JUMP:         add_RETURN_type_mir_into_dag_internal<MIRJump>(mir); break;
    case MIR_PRODUCE_CBIT: add_PRODUCE_CBIT_type_mir_into_dag_internal<MIRProduceCBit>(mir); break;
    case MIR_CLEAR_CBIT:   add_CLEAR_CBIT_type_mir_into_dag_internal<MIRClearCBit>(mir); break;
    case MIR_LOAD_LABEL:   add_LOAD_LABEL_type_mir_into_dag_internal<MIRLoadLabel>(mir); break;
    case MIR_LOAD:         add_MOV_type_mir_into_dag_internal<MIRLoad>(mir); break;
      
    case MIR_DATA32:
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
    
  template<VarAttachedTypeEnum T_attach>
  inline void
  BasicBlock::split_node(DAGNode * const node)
  {
#if 0
    std::list<DAGNode *> &ancestor = node->ancestor<T_attach>();
    
    assert(node->attached_var<T_attach>().size() != 0);
    
    for (std::list<DAGNode *>::const_iterator iter = ancestor.begin();
         iter != ancestor.end();
         ++iter)
    {
      DAGNode * const var_node =
        add_new_dag_node(new DAGNode(static_cast<DAGVar *>(0),
                                     node->peek_first_usable_attached_var<T_attach>()));
      
      ChildInfo const child_info = ChildInfo(var_node, VAR_ATTACH_NORMAL);
      
      (*iter)->replace_child(node, child_info);
    }
#endif
  }
    
  inline void
  BasicBlock::translate_dag_into_lir()
  {
#if PRINT_CHUNK_CODE || CHECK_CHUNK_CODE
    dump_or_check_on_tree<BEFORE_DEAD_CODE_ELIMINATION>();
#endif
    
    dead_code_elimination();
    
    constant_folding();
    
#if CHECK_CHUNK_CODE
    dump_or_check_on_tree<AFTER_CONSTANT_FOLDING>();
#endif
    
    emission_lir_for_each_node();
    
    if (mp_back_patch_bb_label != 0)
    {
      assert(m_pending_need_link_bb.size() != 0);
      
      mp_back_patch_bb_label->reset_to_bb(determine_back_patch_bb());
      
      mp_back_patch_bb_label = 0;
    }
    
    assert(mp_lir_head != 0);
    
#if PRINT_CHUNK_CODE
    dump_lir_info<false, false, false>();
#endif
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  BasicBlock::~BasicBlock()
  {
    // :NOTE: Wei 2004-Dec-15:
    //
    // Cleanup LIR codes.
    if (mp_lir_head != 0)
    {
      LIR *curr_lir = mp_lir_head;
      LIR *prev_lir;
      
      do
      {
        prev_lir = curr_lir;
        curr_lir = curr_lir->next_lir();
        
        delete prev_lir;
      }
      while (curr_lir != 0);
    }
  }
  
  //============================== Operation ==================================
  
#if CHECK_CHUNK_CODE
  void
  BasicBlock::check_label_linked() const
  {
    for (LIR *curr_lir = mp_lir_normal_head;
         curr_lir != 0;
         curr_lir = curr_lir->next_lir())
    {
      curr_lir->check_label_linked();
    }
  }
#endif
  
  namespace
  {
    void
    check_variable_initial_state(Variable *var)
    {
      for (;var != 0; var = var->next_link_var())
      {
        assert(0 == var->dag_node());
      }
    }
  }
  
  inline BasicBlock *
  BasicBlock::determine_back_patch_bb()
  {
    assert(mp_back_patch_bb_label != 0);
    
    BasicBlock *back_patch_bb = 0;
    
    std::vector<SuccBBInfo> const &succ_bb = mp_lir_tail->defined_bb()->succ_bb();
    
#if CHECK_CHUNK_CODE
    if (false == mp_lir_tail->is_jmp_type())
    {
      assert(1 == succ_bb.size());
      assert(true == succ_bb.front().fall_throw());
    }
#endif
    
    for (std::vector<SuccBBInfo>::const_iterator iter = succ_bb.begin();
         iter != succ_bb.end();
         ++iter)
    {
      if (true == (*iter).fall_throw())
      {
        back_patch_bb = (*iter).bb();
        
        break;
      }
    }
    
    assert(back_patch_bb != 0);
    
    return back_patch_bb;
  }
  
  void
  BasicBlock::relink_possible_added_bb()
  {
    if (m_pending_need_link_bb.size() != 0)
    {
      std::vector<BasicBlock *>::iterator back_iter = m_pending_need_link_bb.end();
      --back_iter;
      
      assert((*back_iter) != this);
      
      (*back_iter)->succ_bb() = m_succ_bb;
      
      m_succ_bb.clear();
      
#if CHECK_CHUNK_CODE
      (*back_iter)->lir_tail()->check_bb_link_relationship();
#endif
      
      for (std::vector<BasicBlock *>::iterator iter = m_pending_need_link_bb.begin();
           iter != back_iter;
           ++iter)
      {
        assert(0 == (*iter)->succ_bb().size());
        
        switch ((*iter)->lir_tail()->kind())
        {
        case LIR::RETURN:
          break;
          
        case LIR::JMP_R:
          assert(!"Should not reach here.");
          break;
          
        case LIR::CALL:
          (*iter)->add_succ_bb(true, *(iter + 1));
          break;
          
        case LIR::JMP_I:
          (*iter)->add_succ_bb(false, (*iter)->lir_tail()->operand().front()->label()->bb());
          break;
          
        case LIR::JZ:
        case LIR::JNZ:
        case LIR::JC:
        case LIR::JNC:
        case LIR::JS:
        case LIR::JNS:
        case LIR::JO:
        case LIR::JNO:
        case LIR::JGE:
        case LIR::JL:
        case LIR::JG:
        case LIR::JLE:
          (*iter)->add_succ_true_false_bb(
            (*iter)->lir_tail()->operand().front()->label()->bb(),
            *(iter + 1));
          break;
          
        default:
          assert(false == (*iter)->lir_tail()->is_jmp_type());
          
          (*iter)->add_succ_bb(true, *(iter + 1));
          break;
        }
      }
    }
  }
  
  void
  BasicBlock::translate_MIR_to_LIR_internal()
  {
#if CHECK_CHUNK_CODE
    check_variable_initial_state(gp_chunk->global_var_head());
    check_variable_initial_state(gp_chunk->temp_var_head());
#endif
    
    assert(mp_mir_head != 0);
    assert(0 == mp_mir_tail->next_mir());
    
#if PRINT_CHUNK_CODE || CHECK_CHUNK_CODE
    DAGNode::reset_static_sequence_idx();
#endif
    
    for (MIR *curr_mir = mp_mir_head;
         curr_mir != 0;
         curr_mir = curr_mir->next_mir())
    {
      add_mir_into_dag(curr_mir);
    }
    
    m_translation_status = TRANSLATION_DONE;
    
    translate_dag_into_lir();
    
    // :NOTE: Wei 2004-Nov-9:
    //
    // Cleanup.
    for (std::list<DAGNode *>::const_iterator iter = m_all_dag_nodes.begin();
         iter != m_all_dag_nodes.end();
         ++iter)
    {
      delete *iter;
    }
    
    relink_possible_added_bb();
  }
  
  void
  BasicBlock::translate_MIR_to_LIR()
  {
    switch (m_translation_status)
    {
    case TRANSLATION_NONE:
      translate_MIR_to_LIR_internal();
      break;
      
    case TRANSLATION_TO_LIR:
      m_translation_status = TRANSLATION_DONE;
      break;
      
    case TRANSLATION_DONE:
      return;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    for (std::vector<SuccBBInfo>::const_iterator iter = m_succ_bb.begin();
         iter != m_succ_bb.end();
         ++iter)
    {
      (*iter).bb()->translate_MIR_to_LIR();
    }
  }
  
  inline void
  BasicBlock::add_phi_node_internal(LIR * const phi, LIR *lir_head)
  {
    phi->set_next_lir(lir_head);
    phi->set_prev_lir(lir_head->prev_lir());
    
    if (lir_head->prev_lir() != 0)
    {
      lir_head->prev_lir()->set_next_lir(phi);
    }
    
    lir_head->set_prev_lir(phi);
    
    if (mp_lir_head == lir_head)
    {
      mp_lir_head = phi;
    }
  }
  
  void
  BasicBlock::add_phi_node(Variable * const var)
  {
    assert(var != 0);
    
#if CHECK_CHUNK_CODE
    var->check_not_deleted();
#endif
    
    LIR * const phi = new LIR(static_cast<LIRPhi *>(0), var, m_pred_bb.size());
    
    assert(phi != 0);
    
    phi->set_defined_bb(this);
    
#if CHECK_CHUNK_CODE
    var->add_defined_bb(this);
#endif
    
    switch (var->kind())
    {
    case Variable::GLOBAL:
    case Variable::TEMP:
      add_phi_node_internal(phi, mp_lir_normal_head);
      
      if (mp_lir_scalar_phi_head == mp_lir_normal_head)
      {
        mp_lir_scalar_phi_head = phi;
      }
      break;
      
    case Variable::MEMORY:
      add_phi_node_internal(phi, mp_lir_scalar_phi_head);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  void
  BasicBlock::append_lir_internal(LIR * const lir)
  {
    assert(lir != 0);
    
    if (0 == mp_lir_head)
    {
      mp_lir_head = lir;
      mp_lir_scalar_phi_head = lir;
      mp_lir_normal_head = lir;
      
      mp_lir_head->set_prev_lir(0);
    }
    else
    {
      mp_lir_tail->set_next_lir(lir);
      
      lir->set_prev_lir(mp_lir_tail);
    }
    
    mp_lir_tail = lir;
    
    mp_lir_tail->set_next_lir(0);
    
    lir->set_defined_bb(this);
  }
  
  void
  BasicBlock::insert_lir_before_the_last_jmp(LIR * const lir)
  {
    assert(lir->kind() != LIR::PHI);
    
    if ((0 == mp_lir_head) || (false == mp_lir_tail->is_jmp_type()))
    {
      // :NOTE: Wei 2005-Feb-22:
      //
      // This means I can append the lir instruction in the back of this basic block
      // safely.
      append_lir_internal(lir);
    }
    else
    {
#if CHECK_CHUNK_CODE
      if (mp_lir_tail->prev_lir() != 0)
      {
        assert(false == mp_lir_tail->prev_lir()->is_jmp_type());
      }
#endif
      
      lir->set_prev_lir(mp_lir_tail->prev_lir());
      lir->set_next_lir(mp_lir_tail);
      
      if (mp_lir_tail->prev_lir() != 0)
      {
        mp_lir_tail->prev_lir()->set_next_lir(lir);
      }
      
      mp_lir_tail->set_prev_lir(lir);
      
      if (mp_lir_normal_head == mp_lir_tail)
      {
        mp_lir_normal_head = lir;
      }
      
      if (mp_lir_scalar_phi_head == mp_lir_tail)
      {
        mp_lir_scalar_phi_head = lir;
      }
      
      if (mp_lir_head == mp_lir_tail)
      {
        mp_lir_head = lir;
      }
    }
  }
  
  bool
  BasicBlock::calculate_in_out()
  {
    // :NOTE: Wei 2004-Oct-23:
    //
    // old_IN = IN(X)
    std::vector<uint32_t> old_in(m_in);
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // Reset OUT(X) to 0.
    for (std::vector<uint32_t>::iterator iter = m_out.begin();
         iter != m_out.end();
         ++iter)
    {
      (*iter) = 0;
    }
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // OUT(X) = Union(IN(Y)) for all successors Y of X.
    for (std::vector<SuccBBInfo>::const_iterator iter = m_succ_bb.begin();
         iter != m_succ_bb.end();
         ++iter)
    {
      std::vector<uint32_t>::const_iterator in_iter = (*iter).bb()->in().begin();
      
      for (std::vector<uint32_t>::iterator out_iter = m_out.begin();
           out_iter != m_out.end();
           ++out_iter, ++in_iter)
      {
        (*out_iter) |= (*in_iter);
      }
    }
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // IN(X) = USE(X) Union (OUT(X) - DEF(X));
    
    std::vector<uint32_t>::const_iterator out_iter = m_out.begin();
    std::vector<uint32_t>::const_iterator def_iter = m_def.begin();
    std::vector<uint32_t>::const_iterator use_iter = m_use.begin();
    
    std::vector<uint32_t>::iterator in_iter = m_in.begin();
    
    do
    {
      (*in_iter) = ((*use_iter) | ((*out_iter) & (~(*def_iter))));
      
      ++out_iter;
      ++def_iter;
      ++use_iter;
      ++in_iter;
    }
    while (out_iter != m_out.end());
    
    assert(def_iter == m_def.end());
    assert(use_iter == m_use.end());
    assert(in_iter == m_in.end());
    
    return ((old_in != m_in) ? true : false);
  }
    
  void
  BasicBlock::set_AVAIL_AVLOC(Variable * const var)
  {
    assert(var != 0);
    
#if CHECK_CHUNK_CODE
    var->check_not_deleted();
#endif
    
    uint32_t vector_idx;
    uint32_t bit_position;
    
    calculate_bit_position(var->idx(), vector_idx, bit_position);
    
    assert(m_def.size() > vector_idx);
    
    m_def[vector_idx] |= bit_position;
  }
  
  void
  BasicBlock::set_AVAIL_AVLOC(uint32_t const memory_operation_idx)
  {
    uint32_t vector_idx;
    uint32_t bit_position;
    
    calculate_bit_position(gp_chunk->used_var_table().size() + memory_operation_idx,
                           vector_idx,
                           bit_position);
    
    assert(m_def.size() > vector_idx);
    
    m_def[vector_idx] |= bit_position;
  }
  
  void
  BasicBlock::calculate_AVAIL_AVLOC(uint32_t const var_number)
  {
    m_def.clear();
    
    m_in.clear();
    m_out.clear();
    
    m_def.resize(var_number);
    
    m_in.resize(var_number);
    m_out.resize(var_number);
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // initialize OUT(X) to all 1 for all basic blocks X
    for (std::vector<uint32_t>::iterator iter = m_out.begin();
         iter != m_out.end();
         ++iter)
    {
      (*iter) = static_cast<uint32_t>(~0);
    }
    
#if CHECK_CHUNK_CODE
    // :NOTE: Wei 2004-Oct-23:
    //
    // Make sure that all elements in std::vector<> are initialized to 0.
    for (std::vector<uint32_t>::const_iterator iter = m_def.begin();
         iter != m_def.end();
         ++iter)
    {
      assert(0 == (*iter));
    }
#endif
    
    LIR *curr_lir = mp_lir_normal_head;
    
    while (curr_lir != 0)
    {
      curr_lir->determine_AVAIL_AVLOC(this);
      
      curr_lir = curr_lir->next_lir();
    }
  }
  
  bool
  BasicBlock::is_var_live_in(Variable * const var)
  {
    assert(var != 0);
    
#if CHECK_CHUNK_CODE
    var->check_not_deleted();
#endif
    
    uint32_t vector_idx;
    uint32_t bit_position;
    
    calculate_bit_position(var->idx(), vector_idx, bit_position);
    
    assert(m_in.size() > vector_idx);
    
    return (m_in[vector_idx] & bit_position) ? true : false;
  }
  
  uint32_t
  BasicBlock::pred_bb_idx(BasicBlock * const bb)
  {
    uint32_t i = 0;
    
    for (std::vector<BasicBlock *>::iterator iter = m_pred_bb.begin();
         iter != m_pred_bb.end();
         ++iter)
    {
      if ((*iter) == bb)
      {
        return i;
      }
      
      ++i;
    }
    
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  namespace
  {
    inline void
    replace_mir_data32_bb_label_bb(MIR const * const mir,
                                   BasicBlock * const old_bb,
                                   BasicBlock * const new_bb)
    {
      assert(MIR_DATA32 == mir->kind());
      
      Label * const label = mir->label<MIR_DATA32>();
      
      if (Label::BASIC_BLOCK == label->kind())
      {
        if (label->bb() == old_bb)
        {
          label->replace_bb_label_bb(new_bb);
        }
      }
#if CHECK_CHUNK_CODE
      else
      {
        assert(Label::MIR_INST == label->kind());
        assert(0 == label->mir());
      }
#endif
    }
  }
  
  void
  BasicBlock::replace_succ_bb_and_patch_last_jmp_label_if_possible(BasicBlock * const old_bb,
                                                                   BasicBlock * const new_bb)
  {
    assert(old_bb != this);
    assert(new_bb != this);
    assert(old_bb != new_bb);
    
    assert(true == mp_lir_tail->is_jmp_type());
    assert(1 == mp_lir_tail->operand().size());
    
    for (std::vector<SuccBBInfo>::iterator iter = m_succ_bb.begin();
         iter != m_succ_bb.end();
         ++iter)
    {
      if ((*iter).bb() == old_bb)
      {
        (*iter).set_bb(new_bb);
        
        // :NOTE: Wei 2005-Mar-09:
        //
        // Replace the jump instruction's target label if needed.
        OperandKind * const operand = mp_lir_tail->operand().front();
        
        if (OperandKind::LABEL == operand->kind())
        {
          Label * const label = operand->label();
          
          assert(Label::BASIC_BLOCK == label->kind());
          
          if (label->bb() == old_bb)
          {
            label->replace_bb_label_bb(new_bb);
          }
        }
        else
        {
          assert(OperandKind::VARIABLE == operand->kind());
          assert(LIR::JMP_R == mp_lir_tail->kind());
          assert(1 == m_assistant_bb.size());
          
          // :NOTE: Wei 2005-Mar-31:
          //
          // Replace jump table entry label.
          BasicBlock * const bb = m_assistant_bb.front();
          
          assert(0 == bb->mir_tail()->next_mir());
          
          for (MIR *curr_mir = bb->mir_head();
               curr_mir != 0;
               curr_mir = curr_mir->next_mir())
          {
            replace_mir_data32_bb_label_bb(curr_mir, old_bb, new_bb);
          }
        }
        
        return;
      }
    }
    
    assert(!"Should not reach here.");
  }
  
  inline void
  BasicBlock::fill_ssa_phi_function_operand(BasicBlock * const bb)
  {
    uint32_t const i = pred_bb_idx(bb);
    
    for (LIR *curr_lir = mp_lir_head;
         curr_lir != mp_lir_normal_head;
         curr_lir = curr_lir->next_lir())
    {
      assert(mp_lir_normal_head->kind() != LIR::PHI);
      assert(LIR::PHI == curr_lir->kind());
      
      curr_lir->set_PHI_operand(i);
    }
  }
  
  void
  BasicBlock::rename_variable()
  {
    // :NOTE: Wei 2004-Dec-31:
    //
    // for each Phi-function in bb, x <- Phi(...)
    //   rename x as NewName(x)
    // endfor
    //
    // for each Non-Phi-function in bb, x <- y op z
    //   rename y as top(stack[y])
    //   rename z as top(stack[z])
    //   rename x as NewName(x)
    // endfor
    
    // :NOTE: Wei 2005-Jan-06:
    //
    // USE must be front of DEF.
    for (LIR *curr_lir = mp_lir_head;
         curr_lir != 0;
         curr_lir = curr_lir->next_lir())
    {
      curr_lir->set_ssa_variable();
    }
    
    // :NOTE: Wei 2004-Dec-31:
    //
    // for each successor of bb in the CFG
    //   rename appropriate Phi parameters.
    // endfor
    for (std::vector<SuccBBInfo>::const_iterator iter = m_succ_bb.begin();
         iter != m_succ_bb.end();
         ++iter)
    {
      (*iter).bb()->fill_ssa_phi_function_operand(this);
    }
    
    // :NOTE: Wei 2004-Dec-31:
    //
    // for each successor s of bb in dominator tree
    //   rename(s)
    // endfor
    for (std::vector<BasicBlock *>::iterator iter = m_be_dominanced_by_me.begin();
         iter != m_be_dominanced_by_me.end();
         ++iter)
    {
      (*iter)->rename_variable();
    }
    
    // :NOTE: Wei 2004-Dec-31:
    //
    // for each assignment x in bb do
    //   pop(stack[x])
    // endfor
    for (LIR *curr_lir = mp_lir_head;
         curr_lir != 0;
         curr_lir = curr_lir->next_lir())
    {
      curr_lir->pop_ssa_var();
    }
  }
  
  bool
  BasicBlock::is_dominated_by_me(BasicBlock const * const bb)
  {
    // :NOTE: Wei 2005-Jan-19:
    //
    // Because basicblock dominates itself,
    // thus I start the checking from 'bb', not 'bb->idom_bb()'.
    for (BasicBlock const *dom_bb = bb;
         dom_bb != 0;
         dom_bb = dom_bb->idom_bb())
    {
      if (dom_bb == this)
      {
        return true;
      }
    }
    
    return false;
  }
  
#if CHECK_CHUNK_CODE
  void
  BasicBlock::check_ssa_form()
  {
#if PRINT_CHUNK_CODE
    g_log_file << "SSA checking... "
               << m_sequence_idx
               << std::endl;
#endif
    
    for (LIR *curr_lir = mp_lir_head;
         curr_lir != 0;
         curr_lir = curr_lir->next_lir())
    {
      curr_lir->check_ssa_property(this);
    }
  }
#endif
  
#if PRINT_CHUNK_CODE  
  uint32_t
  BasicBlock::start_inst_idx() const
  {
    return mp_mir_head->idx();
  }
  
  void
  BasicBlock::dump_info() const
  {
    g_log_file << "start inst index: "
               << std::dec
               << mp_mir_head->idx();
    
    for (std::vector<SuccBBInfo>::const_iterator iter = m_succ_bb.begin();
         iter != m_succ_bb.end();
         ++iter)
    {
      g_log_file << ", "
                 << (*iter).bb()->start_inst_idx();
    }
    
    g_log_file << std::endl;
  }
#endif
}

#endif
