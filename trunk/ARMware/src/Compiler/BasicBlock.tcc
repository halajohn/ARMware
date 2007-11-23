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

#include "../Log.hpp"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // Private == Operation =====================================================
  
  template<DefUseEnum T_type>
  inline void
  BasicBlock::set_def_use(Variable * const var)
  {
    assert(var != 0);
    
#if CHECK_CHUNK_CODE
    var->check_not_deleted();
#endif
    
    uint32_t vector_idx;
    uint32_t bit_position;
    
    calculate_bit_position(var->idx(), vector_idx, bit_position);
    
    switch (T_type)
    {
    case USE:
      assert(m_use.size() > vector_idx);
      
      m_use[vector_idx] |= bit_position;
      break;
      
    case DEF:
      assert(m_use.size() > vector_idx);
      assert(m_def.size() > vector_idx);
      
      m_def[vector_idx] |= bit_position;
      
      // :NOTE: Wei 2005-Jan-06:
      //
      // Ex: USE[x=z; x=x+1; y=1;] = {z} (x not in USE)
      //     DEF[x=z; x=x+1; y=1;] = {x, y}
      m_use[vector_idx] &= ~bit_position;
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
    
  template<ConsiderMemoryVarEnum T_consider>
  void
  BasicBlock::calculate_def_use(uint32_t const var_number)
  {
    m_def.clear();
    m_use.clear();
    m_in.clear();
    m_out.clear();
    
    m_def.resize(var_number);
    m_use.resize(var_number);
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // The following 2 are used later in 'calculate_in_out()'.
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // initialize IN(X) to 0 for all basic blocks X
    m_in.resize(var_number);
    m_out.resize(var_number);
    
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
    
    // :NOTE: Wei 2005-Jan-06:
    //
    // According to the 'Advanced Compiler Techniques',
    // the calculation of USE is from the last LIR to the first LIR.
    //
    // Ex: USE[x=z; x=x+1; y=1;] = {z} (x not in USE)
    //     DEF[x=z; x=x+1; y=1;] = {x, y}
    assert(mp_lir_tail != 0);
    
    LIR *end_lir;
    
    switch (T_consider)
    {
    case CONSIDER_MEMORY_VAR:
      end_lir = 0;
      break;
      
    case NO_CONSIDER_MEMORY_VAR:
      end_lir = mp_lir_scalar_phi_head->prev_lir();
      break;
      
    default:
      assert(!"Should not reach here.");
      end_lir = 0;
      break;
    }
    
    LIR *curr_lir = mp_lir_tail;
    
    while (curr_lir != end_lir)
    {
      curr_lir->determine_def_use<T_consider>(this);
      
      curr_lir = curr_lir->prev_lir();
    }
  }
  
  template<bool T_root_bb>
  bool
  BasicBlock::calculate_AVAIL_in_out()
  {
    // :NOTE: Wei 2004-Oct-23:
    //
    // old_OUT = AVOUT(X)
    std::vector<uint32_t> old_out(m_out);
    
#if CHECK_CHUNK_CODE
    if (true == T_root_bb)
    {
      assert(0 == m_pred_bb.size());
    } 
#endif
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // Reset AVIN(X) to 1, except mp_root_bb.
    //
    // This one is different from the one in the live variable analysis.
    // Because the AVIN(X) calculation here is to AND the AVOUT from the predecessor basic blocks.
    // However, in live variable analysis, the OUT(X) is to OR the IN(X) from the predecessor ones.
    // Thus I have to set initial AVIN(X) here to all 1,
    // and set initial OUT(X) in live variable analysis to all 0.
    for (std::vector<uint32_t>::iterator iter = m_in.begin();
         iter != m_in.end();
         ++iter)
    {
      if (true == T_root_bb)
      {
        (*iter) = 0;
      }
      else
      {
        (*iter) = static_cast<uint32_t>(~0);
      }
    }
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // AVIN(X) = And(AVOUT(Y)) for all predecessor Y of X.
    for (std::vector<BasicBlock *>::const_iterator iter = m_pred_bb.begin();
         iter != m_pred_bb.end();
         ++iter)
    {
      std::vector<uint32_t>::const_iterator out_iter = (*iter)->out().begin();
      
      for (std::vector<uint32_t>::iterator in_iter = m_in.begin();
           in_iter != m_in.end();
           ++in_iter, ++out_iter)
      {
        (*in_iter) &= (*out_iter);
      }
    }
    
    // :NOTE: Wei 2005-May-24:
    //
    // Before I modify m_in of each basic block in the later fully static redundancy elimination stage,
    // I need to save the original value to m_orig_in.
    m_orig_in = m_in;
    
    // :NOTE: Wei 2004-Oct-23:
    //
    // AVOUT(X) = AVIN(X) Union AVLOC(X)
    
    std::vector<uint32_t>::const_iterator in_iter = m_in.begin();
    std::vector<uint32_t>::const_iterator def_iter = m_def.begin();
    
    std::vector<uint32_t>::iterator out_iter = m_out.begin();
    
    do
    {
      (*out_iter) = ((*in_iter) | (*def_iter));
      
      ++in_iter;
      ++def_iter;
      ++out_iter;
    }
    while (in_iter != m_in.end());
    
    assert(def_iter == m_def.end());
    assert(out_iter == m_out.end());
    
    return ((old_out != m_out) ? true : false);
  }
  
  template<bool T_clear_childs_ancestor, bool T_minus_child_ref_cnt, bool T_preform_unlink>
  inline void
  BasicBlock::remove_dead_node(DAGNode * const node)
  {
    assert(true == node->is_usable());
    
#if PRINT_CHUNK_CODE
    g_log_file << "remove dead node: ";
    node->dump_info();
    g_log_file << std::endl;
#endif
    
    node->cleanup_attached_var();
    
    if (true == T_clear_childs_ancestor)
    {
      std::deque<ChildInfo> const &children = node->children();
      
      for (std::deque<ChildInfo>::const_iterator iter = children.begin();
           iter != children.end();
           ++iter)
      {
        assert(true == (*iter).node()->is_usable());
        
        if (true == T_minus_child_ref_cnt)
        {
          (*iter).node()->minus_global_attached_var_ref_cnt((*iter).global_attached_var_ref_cnt());
        }
        
        switch ((*iter).attached_type())
        {
        case VAR_ATTACH_HI:
          (*iter).node()->template remove_ancestor<VAR_ATTACH_HI>(node);
          break;
          
        case VAR_ATTACH_LO:
          (*iter).node()->template remove_ancestor<VAR_ATTACH_LO>(node);
          break;
          
        case VAR_ATTACH_NORMAL:
          (*iter).node()->template remove_ancestor<VAR_ATTACH_NORMAL>(node);
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
    }
    
    if (true == T_preform_unlink)
    {
      unlink_dag_node(node);
    }
    
    node->clear_usable();
  }
  
  template<bool T_need_load_store>
  LIR *
  BasicBlock::append_lir(LIR * const lir)
  {
    assert(lir != 0);
    
    if (true == m_next_lir_is_leader)
    {
      if (0 == m_pending_need_link_bb.size())
      {
        m_pending_need_link_bb.push_back(this);
      }
      
      BasicBlock * const new_bb = gp_chunk->get_new_basicblock();
      
      m_pending_need_link_bb.push_back(new_bb);
      
      mp_real_bb = new_bb;
      
      mp_real_bb->set_translation_status(TRANSLATION_TO_LIR);
      
      m_next_lir_is_leader = false;
    }
    
    lir->template before_append_lir<T_need_load_store>(mp_real_bb);
    
    mp_real_bb->append_lir_internal(lir);
    
    if (true == lir->is_jmp_type())
    {
      m_next_lir_is_leader = true;
    }
    else
    {
      lir->template after_append_lir<T_need_load_store>(mp_real_bb);
    }
    
    if (mp_back_patch_bb_label != 0)
    {
      mp_back_patch_bb_label->replace_bb_label_bb(lir->defined_bb());
      
      mp_back_patch_bb_label = 0;
    }
    
    return lir;
  }
  
#if PRINT_CHUNK_CODE
  template<bool T_ssa_enabled, bool T_use_global_idx, bool T_print_memory_operation_idx>
  void
  BasicBlock::dump_lir_info() const
  {
    if (true == T_ssa_enabled)
    {
      g_log_file << std::dec
                 << m_sequence_idx
                 << ": ";
      
      for (std::vector<SuccBBInfo>::const_iterator iter = m_succ_bb.begin();
           iter != m_succ_bb.end();
           ++iter)
      {
        g_log_file << (*iter).bb()->sequence_idx()
                   << " ";
      }
      
      g_log_file << std::endl;
    }
    else
    {
      g_log_file << "--- LIR codes ---"
                 << std::endl;
    }
    
    assert(mp_lir_head != 0);
    
    LIR *curr_lir = mp_lir_head;
    uint32_t inst_idx = 0;
    
    do
    {
      g_log_file << std::dec;
      
      if (false == T_use_global_idx)
      {
        g_log_file << inst_idx;
      }
      else
      {
        g_log_file << curr_lir->idx();
      }
      
      g_log_file << ": ";
      
      curr_lir->dump_info<T_ssa_enabled, T_print_memory_operation_idx>();
      
      ++inst_idx;
      
      curr_lir = curr_lir->next_lir();
    }
    while (curr_lir != 0);
    
    g_log_file << std::endl;
  }
#endif
}

#endif
