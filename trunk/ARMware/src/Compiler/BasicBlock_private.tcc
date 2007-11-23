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

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // Private == Operation =====================================================
  
#if CHECK_CHUNK_CODE || PRINT_CHUNK_CODE
  namespace
  {
    template<CheckAncestorNumberTiming T_timing>
    void
    check_ref_cnt(DAGNode * const node, uint32_t const level)
    {
      if (0 == level)
      {
        assert(0 == node->global_attached_var_ref_cnt());
      }
      
      std::deque<ChildInfo> const &children = node->children();
      
      for (std::deque<ChildInfo>::const_iterator iter = children.begin();
           iter != children.end();
           ++iter)
      {
        check_ref_cnt<T_timing>((*iter).node(), level + 1);
      }
    }
  }
  
  template<CheckAncestorNumberTiming T_timing>
  void
  BasicBlock::dump_or_check_on_tree() const
  {
    std::vector<DAGNode *> m_top_node;
    
    for (std::list<DAGNode *>::const_iterator iter = m_all_dag_nodes.begin();
         iter != m_all_dag_nodes.end();
         ++iter)
    {
      if (true == (*iter)->is_usable())
      {
        if (0 == (*iter)->ancestor_number())
        {
          m_top_node.push_back(*iter);
        }
      }
    }
    
#if PRINT_CHUNK_CODE
    switch (T_timing)
    {
    case BEFORE_DEAD_CODE_ELIMINATION:
      g_log_file << "--- before dead code elimination ---"
                 << std::endl;
      break;
      
    case AFTER_CONSTANT_FOLDING:
      g_log_file << "--- after constant folding ---"
                 << std::endl;
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
#endif
    
    for (std::vector<DAGNode *>::const_iterator iter = m_top_node.begin();
         iter != m_top_node.end();
         ++iter)
    {
#if PRINT_CHUNK_CODE
      dump_dag_tree(*iter, 0);
#endif
      
#if CHECK_CHUNK_CODE
      check_ref_cnt<T_timing>(*iter, 0);
#endif
    }
    
#if PRINT_CHUNK_CODE
    g_log_file << std::endl;
#endif
  }
#endif
  
  inline BasicBlock::ConstantFoldingType
  BasicBlock::determine_constant_folding_type(DAGNode * const node)
  {
#if CHECK_CHUNK_CODE
    if (0 == node->ancestor_number())
    {
      assert(true == node->is_needed());
    }
#endif
    
    if (true == node->is_needed())
    {
      return NEED_MOVE;
    }
    else
    {
      return NO_NEED_MOVE;
    }
  }
  
  template<typename T_dag_type>
  inline void
  BasicBlock::constant_folding_internal_type_cc(DAGNode * const node,
                                                ChildInfo const &child_0,
                                                ChildInfo const &child_1)
  {
    //      o
    //   |     |
    // const const
    
    // :NOTE: Wei 2004-Nov-14:
    //
    // Create a new CONST variable.
    
    // :NOTE: Wei 2004-Nov-26:
    //
    // The order of the arguments are child_0 first, and child_1 later.
    Variable * const const_var =
      gp_chunk->find_const_var(T_dag_type::constant_folding_operation(
                                 child_0.peek_first_usable_attached_var()->const_num(),
                                 child_1.peek_first_usable_attached_var()->const_num()));
    
#if PRINT_CHUNK_CODE
    g_log_file << "constant folding (process 2 constant var): ";
    node->dump_info();
    g_log_file << std::endl;
#endif
    
    if (NEED_MOVE == determine_constant_folding_type(node))
    {
      // :NOTE: Wei 2004-Nov-14:
      //
      //         o                mov
      //      |      |     ->      |
      //    const  const         const
      
      node->set_kind(DAG_MOV);
      
      node->children().resize(1);
      child_0.node()->set_const_var(const_var);
      
      // :NOTE: Wei 2005-Apr-26:
      //
      // Because there are no child nodes of child_1, thus the template argument of
      // remove_dead_node() is not important.
      remove_dead_node<false, true, true>(child_1.node());
    }
    else
    {
      // :NOTE: Wei 2004-Nov-14:
      //
      //         o               const
      //      |      |     ->
      //    const  const
      
      remove_dead_node<false, true, true>(node);
      remove_dead_node<false, true, true>(child_0.node());
      remove_dead_node<false, true, true>(child_1.node());
      
      for (std::list<DAGNode *>::const_iterator iter = node->ancestor<VAR_ATTACH_NORMAL>().begin();
           iter != node->ancestor<VAR_ATTACH_NORMAL>().end();
           ++iter)
      {
        DAGNode * const const_node = add_new_dag_node(new DAGNode(static_cast<DAGVar *>(0), const_var));
        
        ChildInfo const child_info = ChildInfo(const_node, VAR_ATTACH_NORMAL);
        
        (*iter)->replace_child(node, child_info);
      }
    }
  }
  
  template<typename T_dag_type>
  inline void
  BasicBlock::constant_folding_internal_type_xcc(DAGNode * const node,
                                                 ChildInfo const &child_0,
                                                 ChildInfo const &child_1,
                                                 ChildInfo const &child_0_child_0,
                                                 ChildInfo const &child_0_child_1)
  {
    //          o                    o
    //      |        |           |       |
    //      o      const  ->     x     const
    //   |      |
    //   x    const
    
    assert(0 == child_0.node()->global_attached_var_def_cnt());
    
#if PRINT_CHUNK_CODE
    g_log_file << "constant folding: ";
    node->dump_info();
    g_log_file << std::endl;
#endif
    
    child_1.node()->
      set_const_var(gp_chunk->find_const_var(
                      T_dag_type::constant_folding_operation(
                        child_1.peek_first_usable_attached_var()->const_num(),
                        child_0_child_1.peek_first_usable_attached_var()->const_num())));
    
    node->replace_child(child_0.node(), child_0_child_0);
    
    // :NOTE: Wei 2005-Apr-26:
    //
    // I don't want to change the global_attached_var_ref_cnt of child_0_child_0,
    // because 'node' will use child_0_child_0, thus the ref_cnt of child_0_child_0
    // should be the same after this constant folding operation.
    //
    // Thus I pass false as the template argument of remove_dead_node().
    remove_dead_node<true, false, true>(child_0.node());
    
    // :NOTE: Wei 2005-Apr-26:
    //
    // Because there are no child nodes of child_1, thus the template argument of
    // remove_dead_node() is not important.
    remove_dead_node<true, true, true>(child_0_child_1.node());
  }
    
  template<typename T_dag_type>
  inline bool
  BasicBlock::constant_folding_internal(DAGNode * const node)
  {
    ChildInfo const child_0 = node->children().front();
    ChildInfo const child_1 = node->children().back();
    
    if ((child_0.node()->ancestor_number() != 1) ||
        (child_1.node()->ancestor_number() != 1))
    {
      return false;
    }
    
    switch (child_0.node()->kind())
    {
    case DAG_VAR_CONST:
      // :NOTE: Wei 2004-Nov-14:
      //
      //         o
      //      |      |
      //    const
      {
        switch (child_1.node()->kind())
        {
        case DAG_VAR_CONST:
          // :NOTE: Wei 2004-Nov-14:
          //
          // Type 1:
          //
          //         o
          //      |      |
          //    const  const
          switch (T_dag_type::KIND)
          {
          case DAG_ADD:
          case DAG_AND:
          case DAG_EOR:
          case DAG_OR:
          case DAG_SUB:
          case DAG_LSFT_LEFT:
            // :NOTE: Wei 2004-Nov-14:
            //
            // Process 2 constant.
            constant_folding_internal_type_cc<T_dag_type>(node, child_0, child_1);
            return true;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          break;
          
        case T_dag_type::KIND:
          // :NOTE: Wei 2004-Nov-14:
          //
          //         o
          //      |      |
          //    const    o
          if ((child_1.node()->in_flags() | child_1.node()->out_flags()) == 0)
          {
            ChildInfo const child_1_child_0 = child_1.node()->children().front();
            ChildInfo const child_1_child_1 = child_1.node()->children().back();
            
            if ((child_1_child_0.node()->ancestor_number() != 1) ||
                (child_1_child_1.node()->ancestor_number() != 1))
            {
              return false;
            }
            
            if (DAG_VAR_CONST == child_1_child_0.node()->kind())
            {
              if (DAG_VAR_CONST == child_1_child_1.node()->kind())
              {
                if (0 == child_1.node()->global_attached_var_def_cnt())
                {
                  // :NOTE: Wei 2004-Nov-14:
                  //
                  // Type 2:
                  //
                  //         o
                  //      |      |
                  //    const    o
                  //          |     |
                  //        const const
                  switch (T_dag_type::KIND)
                  {
                  case DAG_ADD:
                  case DAG_AND:
                  case DAG_EOR:
                  case DAG_OR:
                  case DAG_SUB:
                  case DAG_LSFT_LEFT:
                    {
                      // :NOTE: Wei 2004-Nov-14:
                      //
                      // Process 3 constant value.
                      
                      // :NOTE: Wei 2004-Nov-14:
                      //
                      // Create a new CONST variable.
                      
                      // :NOTE: Wei 2004-Nov-26:
                      //
                      // The order of the arguments are very important !!!
                      Variable * const const_var =
                        gp_chunk->find_const_var(T_dag_type::constant_folding_operation(
                                                   child_0.peek_first_usable_attached_var()->const_num(),
                                                   T_dag_type::constant_folding_operation(
                                                     child_1_child_0.peek_first_usable_attached_var()->const_num(),
                                                     child_1_child_1.peek_first_usable_attached_var()->const_num())));
                      
#if PRINT_CHUNK_CODE
                      g_log_file << "constant folding (process 3 constant var): ";
                      node->dump_info();
                      g_log_file << std::endl;
#endif
                      
                      if (NEED_MOVE == determine_constant_folding_type(node))
                      {
                        // :NOTE: Wei 2004-Nov-14:
                        //
                        //         o                    mov
                        //      |      |                 |
                        //    const    o         ->    const
                        //          |     |
                        //        const const
                        
                        node->set_kind(DAG_MOV);
                        
                        node->children().resize(1);
                        child_0.node()->set_const_var(const_var);
                        
                        remove_dead_node<true, true, true>(child_1.node());
                        remove_dead_node<true, true, true>(child_1_child_0.node());
                        remove_dead_node<true, true, true>(child_1_child_1.node());
                      }
                      else
                      {
                        // :NOTE: Wei 2004-Nov-14:
                        //
                        //         o                   const
                        //      |      |
                        //    const    o         ->
                        //          |     |
                        //        const const
                        
                        remove_dead_node<true, true, true>(node);
                        remove_dead_node<true, true, true>(child_0.node());
                        remove_dead_node<true, true, true>(child_1.node());
                        remove_dead_node<true, true, true>(child_1_child_0.node());
                        remove_dead_node<true, true, true>(child_1_child_1.node());
                        
                        for (std::list<DAGNode *>::const_iterator iter = node->ancestor<VAR_ATTACH_NORMAL>().begin();
                             iter != node->ancestor<VAR_ATTACH_NORMAL>().end();
                             ++iter)
                        {
                          DAGNode * const const_node = add_new_dag_node(new DAGNode(static_cast<DAGVar *>(0), const_var));
                          
                          ChildInfo const child_info = ChildInfo(const_node, VAR_ATTACH_NORMAL);
                          
                          (*iter)->replace_child(node, child_info);
                        }
                      }
                      
                      return true;
                    }
                    break;
                    
                  default:
                    assert(!"Should not reach here.");
                    break;
                  }
                }
              }
              else
              {
                // :NOTE: Wei 2004-Nov-14:
                //
                //         o                   o
                //      |      |            |     |
                //    const    o      ->  const   x
                //          |     |
                //        const   x
                if ((1 == child_1_child_1.node()->attached_var_amount()) &&
                    (1 == child_1.node()->attached_var_amount()) &&
                    (0 == child_1.node()->global_attached_var_def_cnt()) &&
                    (child_1.node()->template attached_var<VAR_ATTACH_NORMAL>().front() == 
                     child_1_child_1.node()->template attached_var<VAR_ATTACH_NORMAL>().front()))
                {
                  switch (T_dag_type::KIND)
                  {
                  case DAG_ADD:
                  case DAG_AND:
                  case DAG_OR:
#if PRINT_CHUNK_CODE
                    g_log_file << "constant folding: ";
                    node->dump_info();
                    g_log_file << std::endl;
#endif
                    
                    child_0.node()->
                      set_const_var(gp_chunk->find_const_var(
                                      T_dag_type::constant_folding_operation(
                                        child_0.peek_first_usable_attached_var()->const_num(),
                                        child_1_child_0.peek_first_usable_attached_var()->const_num())));
                    
                    node->replace_child(child_1.node(), child_1_child_1);
                    
                    remove_dead_node<true, false, true>(child_1.node());
                    remove_dead_node<true, true, true>(child_1_child_0.node());
                    break;
                    
                  case DAG_SUB:
                  case DAG_LSFT_LEFT:
                    assert(!"I don't support this yet.");
                    break;
                    
                  case DAG_EOR:
                    // :NOTE: Wei 2004-Nov-26:
                    //
                    // Because EOR doesn't obey 'distributive law',
                    // (a ¡ò b) ¡ò c != a ¡ò (b ¡ò c)
                    // thus I can merge these 2 constants.
                    break;
                    
                  default:
                    assert(!"Should not reach here.");
                    break;
                  }
                }
              }
            }
            else if (DAG_VAR_CONST == child_1_child_1.node()->kind())
            {
              // :NOTE: Wei 2004-Nov-14:
              //
              // Type 4:
              //
              //         o                      o
              //      |      |               |     |
              //    const    o        ->   const   x
              //          |     |
              //          x   const
              if ((1 == child_1_child_0.node()->attached_var_amount()) &&
                  (1 == child_1.node()->attached_var_amount()) &&
                  (0 == child_1.node()->global_attached_var_def_cnt()) &&
                  (child_1.node()->template attached_var<VAR_ATTACH_NORMAL>().front() == 
                   child_1_child_0.node()->template attached_var<VAR_ATTACH_NORMAL>().front()))
              {
                switch (T_dag_type::KIND)
                {
                case DAG_ADD:
                case DAG_AND:
                case DAG_OR:
#if PRINT_CHUNK_CODE
                  g_log_file << "constant folding: ";
                  node->dump_info();
                  g_log_file << std::endl;
#endif
                  
                  child_0.node()->
                    set_const_var(gp_chunk->find_const_var(
                                    T_dag_type::constant_folding_operation(
                                      child_0.peek_first_usable_attached_var()->const_num(),
                                      child_1_child_1.peek_first_usable_attached_var()->const_num())));
                  
                  node->replace_child(child_1.node(), child_1_child_0);
                  
                  remove_dead_node<true, false, true>(child_1.node());
                  remove_dead_node<true, true, true>(child_1_child_1.node());
                  break;
                  
                case DAG_SUB:
                case DAG_LSFT_LEFT:
                  assert(!"I don't support this yet.");
                  break;
                  
                case DAG_EOR:
                  // :NOTE: Wei 2004-Nov-26:
                  //
                  // Because EOR doesn't obey 'distributive law',
                  // (a ¡ò b) ¡ò c != a ¡ò (b ¡ò c)
                  // thus I can merge these 2 constants.
                  break;
                  
                default:
                  assert(!"Should not reach here.");
                  break;
                }
              }
            }
          }
          break;
          
        default:
          break;
        }
      }
      break;
      
    case T_dag_type::KIND:
      {
        // :NOTE: Wei 2004-Nov-14:
        //
        //         o
        //      |     |
        //      o
        if ((child_0.node()->in_flags() | child_0.node()->out_flags()) == 0)
        {
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            {
              // :NOTE: Wei 2004-Nov-14:
              //
              //         o
              //      |      |
              //      o    const
              ChildInfo const child_0_child_0 = child_0.node()->children().front();
              ChildInfo const child_0_child_1 = child_0.node()->children().back();
              
              if ((child_0_child_0.node()->ancestor_number() != 1) ||
                  (child_0_child_1.node()->ancestor_number() != 1))
              {
                return false;
              }
              
              if (DAG_VAR_CONST == child_0_child_0.node()->kind())
              {
                if (DAG_VAR_CONST == child_0_child_1.node()->kind())
                {
                  if (0 == child_0.node()->global_attached_var_def_cnt())
                  {
                    switch (T_dag_type::KIND)
                    {
                    case DAG_ADD:
                    case DAG_AND:
                    case DAG_OR:
                    case DAG_SUB:
                    case DAG_LSFT_LEFT:
                      {
                        // :NOTE: Wei 2004-Nov-14:
                        //
                        // Type 5:
                        //
                        //          o
                        //      |       |
                        //      o     const
                        //   |      |
                        // const  const
                        
                        // :NOTE: Wei 2004-Nov-14:
                        //
                        // Process 3 constant value.
                        
                        Variable * const const_var =
                          gp_chunk->find_const_var(T_dag_type::constant_folding_operation(
                                                     T_dag_type::constant_folding_operation(
                                                       child_0_child_0.peek_first_usable_attached_var()->const_num(),
                                                       child_0_child_1.peek_first_usable_attached_var()->const_num()),
                                                     child_1.peek_first_usable_attached_var()->const_num()));
                        
#if PRINT_CHUNK_CODE
                        g_log_file << "constant folding (process 3 constant var): ";
                        node->dump_info();
                        g_log_file << std::endl;
#endif
                        
                        if (NEED_MOVE == determine_constant_folding_type(node))
                        {
                          // :NOTE: Wei 2004-Nov-14:
                          //
                          //          o                  mov
                          //      |        |              |
                          //      o      const    ->    const
                          //   |      |
                          // const  const
                          
                          node->set_kind(DAG_MOV);
                          
                          node->children().pop_front();
                          child_1.node()->set_const_var(const_var);
                          
                          remove_dead_node<true, true, true>(child_0.node());
                          remove_dead_node<true, true, true>(child_0_child_0.node());
                          remove_dead_node<true, true, true>(child_0_child_1.node());
                        }
                        else
                        {
                          // :NOTE: Wei 2004-Nov-14:
                          //
                          //          o                 const
                          //      |        |
                          //      o      const    ->
                          //   |      |
                          // const  const
                          
                          remove_dead_node<true, true, true>(node);
                          remove_dead_node<true, true, true>(child_0.node());
                          remove_dead_node<true, true, true>(child_1.node());
                          remove_dead_node<true, true, true>(child_0_child_0.node());
                          remove_dead_node<true, true, true>(child_0_child_1.node());
                          
                          for (std::list<DAGNode *>::const_iterator iter = node->ancestor<VAR_ATTACH_NORMAL>().begin();
                               iter != node->ancestor<VAR_ATTACH_NORMAL>().end();
                               ++iter)
                          {
                            DAGNode * const const_node = add_new_dag_node(new DAGNode(static_cast<DAGVar *>(0), const_var));
                            
                            ChildInfo const child_info = ChildInfo(const_node, VAR_ATTACH_NORMAL);
                            
                            (*iter)->replace_child(node, child_info);
                          }
                        }
                        
                        return true;
                      }
                      break;
                      
                    default:
                      assert(!"Should not reach here.");
                      break;
                    }
                  }
                }
                else
                {
                  if ((1 == child_0_child_1.node()->attached_var_amount()) &&
                      (1 == child_0.node()->attached_var_amount()) &&
                      (0 == child_0.node()->global_attached_var_def_cnt()) &&
                      (child_0.node()->template attached_var<VAR_ATTACH_NORMAL>().front() == 
                       child_0_child_1.node()->template attached_var<VAR_ATTACH_NORMAL>().front()))
                  {
                    switch (T_dag_type::KIND)
                    {
                    case DAG_ADD:
                    case DAG_AND:
                    case DAG_OR:
                      // :NOTE: Wei 2004-Nov-14:
                      //
                      //          o                  o
                      //      |        |          |     |
                      //      o      const  ->    x   const
                      //   |      |
                      // const    x
#if PRINT_CHUNK_CODE
                      g_log_file << "constant folding: ";
                      node->dump_info();
                      g_log_file << std::endl;
#endif
                      
                      child_1.node()->
                        set_const_var(gp_chunk->find_const_var(
                                        T_dag_type::constant_folding_operation(
                                          child_1.peek_first_usable_attached_var()->const_num(),
                                          child_0_child_0.peek_first_usable_attached_var()->const_num())));
                      
                      node->replace_child(child_0.node(), child_0_child_1);
                      
                      remove_dead_node<true, false, true>(child_0.node());
                      remove_dead_node<true, true, true>(child_0_child_0.node());
                      break;
                      
                    case DAG_SUB:
                    case DAG_LSFT_LEFT:
                      assert(!"I don't support this yet.");
                      break;
                      
                    case DAG_EOR:
                      // :NOTE: Wei 2004-Nov-26:
                      //
                      // Because EOR doesn't obey 'distributive law',
                      // (a ¡ò b) ¡ò c != a ¡ò (b ¡ò c)
                      // thus I can merge these 2 constants.
                      break;
                      
                    default:
                      assert(!"Should not reach here.");
                      break;
                    }
                    
                    return true;
                  }
                }
              }
              else if (DAG_VAR_CONST == child_0_child_1.node()->kind())
              {
                if ((1 == child_0_child_0.node()->attached_var_amount()) &&
                    (1 == child_0.node()->attached_var_amount()) &&
                    (0 == child_0.node()->global_attached_var_def_cnt()) &&
                    (child_0.node()->template attached_var<VAR_ATTACH_NORMAL>().front() == 
                     child_0_child_0.node()->template attached_var<VAR_ATTACH_NORMAL>().front()))
                {
                  // :NOTE: Wei 2004-Nov-14:
                  //
                  //          o
                  //      |        |
                  //      o      const
                  //   |      |
                  //   x    const
                  switch (T_dag_type::KIND)
                  {
                  case DAG_ADD:
                  case DAG_AND:
                  case DAG_OR:
                    constant_folding_internal_type_xcc<T_dag_type>(node,
                                                                   child_0,
                                                                   child_1,
                                                                   child_0_child_0,
                                                                   child_0_child_1);
                    return true;
                    
                  case DAG_SUB:
                    // :NOTE: Wei 2004-Nov-26:
                    //
                    // In this case, (x - const) - const = x - (const + const)
                    constant_folding_internal_type_xcc<DAGAdd>(node,
                                                               child_0,
                                                               child_1,
                                                               child_0_child_0,
                                                               child_0_child_1);
                    return true;
                    
                  case DAG_EOR:
                    // :NOTE: Wei 2004-Nov-26:
                    //
                    // Because EOR doesn't obey 'distributive law',
                    // (a ¡ò b) ¡ò c != a ¡ò (b ¡ò c)
                    // thus I can merge these 2 constants.
                    break;
                    
                  case DAG_LSFT_LEFT:
                    assert(!"I don't support this yet.");
                    break;
                    
                  default:
                    assert(!"Should not reach here.");
                    break;
                  }
                }
              }
            }
            break;
            
          default:
            break;
          }
        }
      }
      break;
      
    default:
      break;
    }
    
    return false;
  }
  
  template<typename T_dag_node>
  inline std::vector<DAGNode *> &
  BasicBlock::determine_dag_node_table()
  {
    switch (T_dag_node::KIND)
    {
    case DAG_MOV:
      return m_dag_node_MOV_table;
      
    case DAG_NOT:
      return m_dag_node_NOT_table;
      
    case DAG_LOAD:
      return m_dag_node_LOAD_table;
      
    case DAG_ADD:
      return m_dag_node_ADD_table;
      
    case DAG_AND:
      return m_dag_node_AND_table;
      
    case DAG_OR:
      return m_dag_node_OR_table;
      
    case DAG_EOR:
      return m_dag_node_EOR_table;
      
    case DAG_MUL32:
      return m_dag_node_MUL32_table;
      
    case DAG_TST:
      return m_dag_node_TST_table;
      
    case DAG_UMUL64:
      return m_dag_node_UMUL64_table;
      
    case DAG_SMUL64:
      return m_dag_node_SMUL64_table;
      
    case DAG_SUB:
      return m_dag_node_SUB_table;
      
    case DAG_LSFT_LEFT:
      return m_dag_node_LSFT_LEFT_table;
      
    case DAG_LSFT_RIGHT:
      return m_dag_node_LSFT_RIGHT_table;
      
    case DAG_ASFT_RIGHT:
      return m_dag_node_ASFT_RIGHT_table;
      
    case DAG_ROR:
      return m_dag_node_ROR_table;
      
    case DAG_CMP:
      return m_dag_node_CMP_table;
      
    case DAG_ADD64:
      return m_dag_node_ADD64_table;
      
    default:
      assert(!"Should not reach here.");
      return m_dag_node_ADD_table;
    }
  }
  
  template<typename T_dag_node>
  inline DAGNode *
  BasicBlock::find_dag_node(DAGNode * const child)
  {
    assert(child != 0);
    
    std::vector<DAGNode *> &dag_node_table = determine_dag_node_table<T_dag_node>();
    
    for (std::vector<DAGNode *>::const_iterator iter = dag_node_table.begin();
         iter != dag_node_table.end();
         ++iter)
    {
      assert(1 == ((*iter)->children()).size());
      
      if ((*iter)->children().front().node() == child)
      {
        // :NOTE: Wei 2004-Nov-8:
        //
        // We should only add no-condition-flag-calculation DAG node
        // into 'dag_node_table', thus we should only find
        // this kind of DAG node in the 'dag_node_table'.
        assert(0 == ((*iter)->in_flags() | (*iter)->out_flags()));
        
        return *iter;
      }
    }
    
    // :NOTE: Wei 2004-Nov-8:
    //
    // Can't find such a DAG node.
    return 0;
  }
  
  template<typename T_dag_node, bool operand_exchangable>
  inline DAGNode *
  BasicBlock::find_dag_node(std::vector<Variable *> &child_var,
                            DAGNode * const child_1,
                            DAGNode * const child_2)
  {
    assert(child_1 != 0);
    assert(child_2 != 0);
    
    std::vector<DAGNode *> &dag_node_table = determine_dag_node_table<T_dag_node>();
    
    for (std::vector<DAGNode *>::const_iterator iter = dag_node_table.begin();
         iter != dag_node_table.end();
         ++iter)
    {
      if (((*iter)->children().front().node() == child_1) &&
          ((*iter)->children().back().node() == child_2))
      {
        assert(0 == ((*iter)->in_flags() | (*iter)->out_flags()));
        
        return *iter;
      }
      
      if (true == operand_exchangable)
      {
        if (((*iter)->children().front().node() == child_2) &&
            ((*iter)->children().back().node() == child_1))
        {
          // :NOTE: Wei 2005-Apr-22:
          //
          // I have to change the order of the operand variables of the original mir,
          // because I will use the order of the operand varables of the mir to 
          // increase the ref cnt of the ChildInfo & child node.
          
          Variable * const temp = child_var.front();
          
          child_var.front() = child_var.back();
          child_var.back() = temp;
          
          // :NOTE: Wei 2004-Nov-8:
          //
          // We should only add no-condition-flag-calculation DAG node
          // into 'dag_node_table', thus we should only find
          // this kind of DAG node in the 'dag_node_table'.
          assert(0 == ((*iter)->in_flags() | (*iter)->out_flags()));
          
          return *iter;
        }
      }
    }
    
    // :NOTE: Wei 2004-Nov-8:
    //
    // Can't find such a DAG node.
    return 0;
  }
  
  template<typename T_dag_node, bool operand_exchangable>
  inline DAGNode *
  BasicBlock::find_dag_node(std::vector<Variable *> &child_var,
                            DAGNode * const child_1,
                            DAGNode * const child_2,
                            DAGNode * const child_3,
                            DAGNode * const child_4)
  {
    assert(child_1 != 0);
    assert(child_2 != 0);
    assert(child_3 != 0);
    assert(child_4 != 0);
    
    std::vector<DAGNode *> &dag_node_table = determine_dag_node_table<T_dag_node>();
    
    for (std::vector<DAGNode *>::const_iterator iter = dag_node_table.begin();
         iter != dag_node_table.end();
         ++iter)
    {
      // :NOTE: Wei 2004-Nov-9:
      //
      // For now, the operands of all of the MIR instructions of this type
      // are exchangable, thus I don't need to similiar checking like
      // the upper one:
      //
      //   template<typename T_dag_node, bool operand_exchangable>
      //   inline DAGNode *
      //   BasicBlock::find_dag_node(DAGNode * const child_1,
      //                             DAGNode * const child_2)
      
      if ((((*iter)->children())[0].node() == child_1) &&
          (((*iter)->children())[1].node() == child_2) &&
          (((*iter)->children())[2].node() == child_3) &&
          (((*iter)->children())[3].node() == child_4))
      {
        // :NOTE: Wei 2004-Nov-8:
        //
        // We should only add no-condition-flag-calculation DAG node
        // into 'dag_node_table', thus we should only find
        // this kind of DAG node in the 'dag_node_table'.
        assert(0 == ((*iter)->in_flags() | (*iter)->out_flags()));
        
        return *iter;
      }
      
      if (true == operand_exchangable)
      {
        if ((((*iter)->children())[0].node() == child_3) &&
            (((*iter)->children())[1].node() == child_4) &&
            (((*iter)->children())[2].node() == child_1) &&
            (((*iter)->children())[3].node() == child_2))
        {
          Variable * const temp0 = child_var[0];
          Variable * const temp1 = child_var[1];
          
          child_var[0] = child_var[2];
          child_var[1] = child_var[3];
          child_var[2] = temp0;
          child_var[3] = temp1;
          
          assert(0 == ((*iter)->in_flags() | (*iter)->out_flags()));
          
          return *iter;
        }
      }
    }
    
    // :NOTE: Wei 2004-Nov-8:
    //
    // Can't find such a DAG node.
    return 0;
  }
  
  namespace
  {
    inline void
    increase_child_node_global_attached_var_ref_cnt(DAGNode * const node,
                                                    std::vector<Variable *> const &child_var)
    {
      assert(child_var.size() == node->children().size());
      
      std::vector<Variable *>::const_iterator var_iter = child_var.begin();
      std::deque<ChildInfo>::iterator child_info_iter = node->children().begin();
      
      for (; var_iter != child_var.end(); ++var_iter, ++child_info_iter)
      {
        switch ((*var_iter)->kind())
        {
        case Variable::CONST:
        case Variable::TEMP:
          // :NOTE: Wei 2005-Apr-21:
          //
          // Because I only need the last definition of a temporary variable with
          // (is_act_as_global_variable()==true) property.
          //
          // Thus I will not increse the m_global_attached_var_ref_cnt of the corresponding
          // child node to reflect that if this child node is not the last definition of this
          // special kind temporary variable, then I have possibility to treat it as a dead node.
          break;
          
        case Variable::GLOBAL:
          (*child_info_iter).inc_global_attached_var_ref_cnt();
          
          (*child_info_iter).node()->inc_global_attached_var_ref_cnt();
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
    }
  }
  
  inline void
  BasicBlock::link_nodes(DAGNode * const node)
  {
    uint32_t prev_dag_node_slot_idx;
    
    if (mp_dag_node_tail != 0)
    {
      assert(mp_dag_node_tail->prev_next_dag_node_pair().size() != 0);
      
#if CHECK_CHUNK_CODE
      std::vector<DAGNode::DAGNodePair>::const_iterator last_iter = mp_dag_node_tail->prev_next_dag_node_pair().end();
      
      --last_iter;
      
      for (std::vector<DAGNode::DAGNodePair>::const_iterator iter = mp_dag_node_tail->prev_next_dag_node_pair().begin();
           iter != last_iter;
           ++iter)
      {
        assert((*iter).mp_next_dag_node != 0);
      }
#endif
      
      assert(0 == mp_dag_node_tail->prev_next_dag_node_pair().back().mp_next_dag_node);
      
      mp_dag_node_tail->prev_next_dag_node_pair().back().mp_next_dag_node = node;
      mp_dag_node_tail->prev_next_dag_node_pair().back().m_next_dag_node_slot_idx = node->prev_next_dag_node_pair().size();
      
      prev_dag_node_slot_idx = mp_dag_node_tail->prev_next_dag_node_pair().size() - 1;
      
      assert(mp_dag_node_head != 0);
    }
    else
    {
      assert(0 == mp_dag_node_head);
      
      mp_dag_node_head = node;
      prev_dag_node_slot_idx = 0;
    }
    
    node->prev_next_dag_node_pair().push_back(DAGNode::DAGNodePair(mp_dag_node_tail,
                                                                   prev_dag_node_slot_idx,
                                                                   0,
                                                                   0));
    
    mp_dag_node_tail = node;
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_CALL_0_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_CALL_0 == T_mir::KIND);
    
    // :NOTE: Wei 2004-Oct-12:
    //
    // There should only be a 'CALL' node in a basic block, and that 'CALL'
    // node should be the last one, too.
    //
    // Thus, I don't need to search an existed and matched 'CALL' node in a
    // basic block.
    assert(mir == mp_mir_tail);
    
    link_nodes(add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0),
                                            mir->label<T_mir::KIND>())));
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_CALL_1_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_CALL_1 == T_mir::KIND);
    
    // :NOTE: Wei 2004-Oct-12:
    //
    // There should only be a 'CALL' node in a basic block, and that 'CALL'
    // node should be the last one, too.
    //
    // Thus, I don't need to search an existed and matched 'CALL' node in a
    // basic block.
    assert(mir == mp_mir_tail);
    
    DAGNode * const call_node =
      add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0),
                                   mir->label<T_mir::KIND>()));
    
    std::vector<Variable *> const &child_var = mir->operand_variable();
    
    add_child_node(call_node, find_var_dag_node(child_var.front()), child_var.front());
    
    increase_child_node_global_attached_var_ref_cnt(call_node, child_var);
    
    link_nodes(call_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_LOAD_LABEL_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_LOAD_LABEL == T_mir::KIND);
    
    assert(0 == (mir->out_flags() | mir->in_flags()));
    
    DAGNode * const node =
      add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0),
                                   mir->label<MIR_LOAD_LABEL>()));
    
    link_nodes(node);
    
    mir->template dest_variable<VAR_ATTACH_NORMAL>()->
      update_attached_type_and_dag_node(VAR_ATTACH_NORMAL, node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_RETURN_type_mir_into_dag_internal(MIR * const mir)
  {
    assert((MIR_JUMP == T_mir::KIND) || (MIR_RETURN == T_mir::KIND));
    
    // :NOTE: Wei 2004-Oct-12:
    //
    // There should only be a such node in a basic block, and that such
    // node should be the last one, too.
    //
    // Thus, I don't need to search an existed and matched such node in a
    // basic block.
    assert(mir == mp_mir_tail);
    
    DAGNode * const return_node =
      add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
    
    std::vector<Variable *> const &child_var = mir->operand_variable();
    
    add_child_node(return_node, find_var_dag_node(child_var.front()), child_var.front());
    
    increase_child_node_global_attached_var_ref_cnt(return_node, child_var);
    
    link_nodes(return_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_RETRIEVE_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_RETRIEVE == T_mir::KIND);
    
    // :NOTE: Wei 2004-Oct-12:
    //
    // There should only be a 'RETRIEVE' node in a basic block, and that 'RETRIEVE'
    // node should be the first one, too.
    //
    // Thus, I don't need to search an existed and matched 'RETRIEVE' node in a
    // basic block.
    assert(mir == mp_mir_head);
    
    DAGNode * const retrieve_node =
      add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0),
                                   mir->pop_count()));
    
    // :NOTE: Wei 2004-Oct-17:
    //
    // We should always retrieve the return value of a function into a temporary
    // variable.
#if CHECK_CHUNK_CODE
    if (mir->dest_variable<VAR_ATTACH_NORMAL>() != 0)
    {
      assert(Variable::TEMP == mir->dest_variable<VAR_ATTACH_NORMAL>()->kind());
    }
#endif
    
    // :NOTE: Wei 2004-Oct-12:
    //
    // Update symbol table entry.
    link_nodes(retrieve_node);
    
    if (mir->dest_variable<VAR_ATTACH_NORMAL>() != 0)
    {
      mir->template dest_variable<VAR_ATTACH_NORMAL>()->
        update_attached_type_and_dag_node(VAR_ATTACH_NORMAL, retrieve_node);
    }
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_ADD_type_mir_into_dag_internal(MIR * const mir)
  {
    assert((MIR_ADD == T_mir::KIND) ||
           (MIR_AND == T_mir::KIND) ||
           (MIR_OR == T_mir::KIND) ||
           (MIR_EOR == T_mir::KIND) ||
           (MIR_SUB == T_mir::KIND) ||
           (MIR_MUL32 == T_mir::KIND) ||
           (MIR_LSFT_LEFT == T_mir::KIND) ||
           (MIR_LSFT_RIGHT == T_mir::KIND) ||
           (MIR_ASFT_RIGHT == T_mir::KIND) ||
           (MIR_ROR == T_mir::KIND) ||
           (MIR_TST == T_mir::KIND) ||
           (MIR_CMP == T_mir::KIND));
    
    assert(0 == mir->in_flags());
    
    DAGNode *operation_node = 0;
    
    std::vector<Variable *> &child_var = mir->operand_variable();
    std::vector<DAGNode *> child_node;
    
    for (std::vector<Variable *>::const_iterator iter = child_var.begin();
         iter != child_var.end();
         ++iter)
    {
      child_node.push_back(find_var_dag_node(*iter));
    }
    
    if (0 == mir->out_flags())
    {
      // :NOTE: Wei 2004-Nov-26:
      //
      // The order of the children have to the same with the order in MIR,
      // because I will use that order in the LIR emission stage.
      //
      // Ex: In the SUB, LSFT_LEFT, LSFT_RIGHT, ASFT_RIGHT, ROR, CMP operations,
      // the order of the operands are very important.
      operation_node =
        find_dag_node<typename T_mir::DAGNodeType, T_mir::OPERAND_EXCHANGABLE>(child_var,
                                                                               child_node.front(),
                                                                               child_node.back());
    }
    else
    {
      // :NOTE: Wei 2004-Nov-8:
      //
      // Because all condition code calculations can not be treated as common subexpressions,
      // thus I have to preserve each MIR who envolves condition flag calculations.
      //
      // Thus, if the current MIR envolves condition flags calculation,
      // then I will create a new DAG node for that MIR operation.
      // And set the corresponding condition flags in the DAG node.
    }
    
    if (0 == operation_node)
    {
      operation_node = add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
      
      std::vector<Variable *>::const_iterator var_iter = child_var.begin();
      std::vector<DAGNode *>::const_iterator node_iter = child_node.begin();
      
      for (; var_iter != child_var.end(); ++var_iter, ++node_iter)
      {
        add_child_node(operation_node, *node_iter, *var_iter);
      }
      
      if (mir->out_flags() != 0)
      {
        operation_node->set_out_flags(mir->out_flags());
      }
      else
      {
        determine_dag_node_table<typename T_mir::DAGNodeType>().push_back(operation_node);
      }
    }
    
    increase_child_node_global_attached_var_ref_cnt(operation_node, child_var);
    
    // :NOTE: Wei 2004-Nov-26:
    //
    // The order of the children have to the same with the order in MIR,
    // because I will use that order in the LIR emission stage.
    //
    // Ex: In the SUB, LSFT_LEFT, LSFT_RIGHT, ASFT_RIGHT, ROR, CMP operations,
    // the order of the operands are very important.
    
    link_nodes(operation_node);
    
    switch (T_mir::KIND)
    {
    case MIR_TST:
    case MIR_CMP:
      break;
      
    case MIR_ADD:
    case MIR_AND:
    case MIR_OR:
    case MIR_EOR:
    case MIR_SUB:
    case MIR_MUL32:
    case MIR_LSFT_LEFT:
    case MIR_LSFT_RIGHT:
    case MIR_ASFT_RIGHT:
    case MIR_ROR:
      mir->template dest_variable<VAR_ATTACH_NORMAL>()->
        update_attached_type_and_dag_node(VAR_ATTACH_NORMAL, operation_node);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_ADC_type_mir_into_dag_internal(MIR * const mir)
  {
    assert((MIR_RRX == T_mir::KIND) ||
           (MIR_ADC == T_mir::KIND) ||
           (MIR_SBC == T_mir::KIND));
    
    assert(mir->in_flags() != 0);
    
    DAGNode * const operation_node =
      add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
    
    operation_node->set_in_out_flags(mir->in_flags(), mir->out_flags());
    
    std::vector<Variable *> const &child_var = mir->operand_variable();
    
    add_child_node(operation_node, find_var_dag_node(child_var.front()), child_var.front());
    add_child_node(operation_node, find_var_dag_node(child_var.back()), child_var.back());
    
    increase_child_node_global_attached_var_ref_cnt(operation_node, child_var);
    
    link_nodes(operation_node);
    
    mir->template dest_variable<VAR_ATTACH_NORMAL>()->
      update_attached_type_and_dag_node(VAR_ATTACH_NORMAL, operation_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_MOV_type_mir_into_dag_internal(MIR * const mir)
  {
    assert((MIR_MOV == T_mir::KIND) ||
           (MIR_NOT == T_mir::KIND) ||
           (MIR_LOAD == T_mir::KIND));
    
    assert(0 == mir->in_flags());
    
    DAGNode *operation_node = 0;
    
    std::vector<Variable *> const &child_var = mir->operand_variable();
    std::vector<DAGNode *> child_node;
    
    child_node.push_back(find_var_dag_node(child_var.front()));
    
    if (0 == mir->out_flags())
    {
      operation_node = find_dag_node<typename T_mir::DAGNodeType>(child_node.front());
    }
    
    if (0 == operation_node)
    {
      operation_node = add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
      
      add_child_node(operation_node, child_node.front(), child_var.front());
      
      if (mir->out_flags() != 0)
      {
        operation_node->set_out_flags(mir->out_flags());
      }
      else
      {
        determine_dag_node_table<typename T_mir::DAGNodeType>().push_back(operation_node);
      }
    }
    
    increase_child_node_global_attached_var_ref_cnt(operation_node, child_var);
    
    link_nodes(operation_node);
    
    mir->template dest_variable<VAR_ATTACH_NORMAL>()->
      update_attached_type_and_dag_node(VAR_ATTACH_NORMAL, operation_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_UMUL64_type_mir_into_dag_internal(MIR * const mir)
  {
    assert((MIR_UMUL64 == T_mir::KIND) ||
           (MIR_SMUL64 == T_mir::KIND));
    
    assert(0 == mir->in_flags());
    
    DAGNode *operation_node = 0;
    
    std::vector<Variable *> &child_var = mir->operand_variable();
    std::vector<DAGNode *> child_node;
    
    for (std::vector<Variable *>::const_iterator iter = child_var.begin();
         iter != child_var.end();
         ++iter)
    {
      child_node.push_back(find_var_dag_node(*iter));
    }
    
    if (0 == mir->out_flags())
    {
      operation_node =
        find_dag_node<typename T_mir::DAGNodeType, T_mir::OPERAND_EXCHANGABLE>(child_var,
                                                                               child_node.front(),
                                                                               child_node.back());
    }
    
    if (0 == operation_node)
    {
      operation_node = add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
      
      std::vector<Variable *>::const_iterator var_iter = child_var.begin();
      std::vector<DAGNode *>::const_iterator node_iter = child_node.begin();
      
      for (; var_iter != child_var.end(); ++var_iter, ++node_iter)
      {
        add_child_node(operation_node, *node_iter, *var_iter);
      }
      
      if (mir->out_flags() != 0)
      {
        operation_node->set_out_flags(mir->out_flags());
      }
      else
      {
        determine_dag_node_table<typename T_mir::DAGNodeType>().push_back(operation_node);
      }
    }
    
    increase_child_node_global_attached_var_ref_cnt(operation_node, child_var);
    
    link_nodes(operation_node);
    
    mir->template dest_variable<VAR_ATTACH_HI>()->
      update_attached_type_and_dag_node(VAR_ATTACH_HI, operation_node);
    
    mir->template dest_variable<VAR_ATTACH_LO>()->
      update_attached_type_and_dag_node(VAR_ATTACH_LO, operation_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_ADD64_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_ADD64 == T_mir::KIND);
    
    assert(0 == mir->in_flags());
    
    DAGNode *operation_node = 0;
    
    std::vector<Variable *> &child_var = mir->operand_variable();
    std::vector<DAGNode *> child_node;
    
    for (std::vector<Variable *>::const_iterator iter = child_var.begin();
         iter != child_var.end();
         ++iter)
    {
      child_node.push_back(find_var_dag_node(*iter));
    }
    
    if (0 == mir->out_flags())
    {
      operation_node =
        find_dag_node<typename T_mir::DAGNodeType, T_mir::OPERAND_EXCHANGABLE>(child_var,
                                                                               child_node[0],
                                                                               child_node[1],
                                                                               child_node[2],
                                                                               child_node[3]);
    }
    
    if (0 == operation_node)
    {
      operation_node = add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
      
      std::vector<Variable *>::const_iterator var_iter = child_var.begin();
      std::vector<DAGNode *>::const_iterator node_iter = child_node.begin();
      
      for (; var_iter != child_var.end(); ++var_iter, ++node_iter)
      {
        add_child_node(operation_node, *node_iter, *var_iter);
      }
      
      if (mir->out_flags() != 0)
      {
        operation_node->set_out_flags(mir->out_flags());
      }
      else
      {
        determine_dag_node_table<typename T_mir::DAGNodeType>().push_back(operation_node);
      }
    }
    
    increase_child_node_global_attached_var_ref_cnt(operation_node, child_var);
    
    link_nodes(operation_node);
    
    mir->template dest_variable<VAR_ATTACH_HI>()->
      update_attached_type_and_dag_node(VAR_ATTACH_HI, operation_node);
    
    mir->template dest_variable<VAR_ATTACH_LO>()->
      update_attached_type_and_dag_node(VAR_ATTACH_LO, operation_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_CMP_EQ_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_CMP_EQ == T_mir::KIND);
    
    // :NOTE: Wei 2004-Oct-12:
    //
    // There should only be a 'CMP_EQ' node in a basic block, and that 'CMP_EQ'
    // node should be the last one, too.
    //
    // Thus, I don't need to search an existed and matched 'CMP_EQ' node in a
    // basic block.
    assert(mir == mp_mir_tail);
    
    DAGNode * const cmp_eq_node =
      add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0),
                                   mir->label<T_mir::KIND>()));
    
    std::vector<Variable *> const &child_var = mir->operand_variable();
    std::vector<DAGNode *> child_node;
    
    for (std::vector<Variable *>::const_iterator iter = child_var.begin();
         iter != child_var.end();
         ++iter)
    {
      child_node.push_back(find_var_dag_node(*iter));
    }
    
    std::vector<Variable *>::const_iterator var_iter = child_var.begin();
    std::vector<DAGNode *>::const_iterator node_iter = child_node.begin();
    
    for (; var_iter != child_var.end(); ++var_iter, ++node_iter)
    {
      add_child_node(cmp_eq_node, *node_iter, *var_iter);
    }
    
    increase_child_node_global_attached_var_ref_cnt(cmp_eq_node, child_var);
    
    link_nodes(cmp_eq_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_GOTO_type_mir_into_dag_internal(MIR * const mir)
  {
    assert((MIR_GOTO == T_mir::KIND) ||
           (MIR_GOTO_EQ == T_mir::KIND) ||
           (MIR_GOTO_NE == T_mir::KIND) ||
           (MIR_GOTO_CS == T_mir::KIND) ||
           (MIR_GOTO_CC == T_mir::KIND) ||
           (MIR_GOTO_MI == T_mir::KIND) ||
           (MIR_GOTO_PL == T_mir::KIND) ||
           (MIR_GOTO_VS == T_mir::KIND) ||
           (MIR_GOTO_VC == T_mir::KIND) ||
           (MIR_GOTO_HI == T_mir::KIND) ||
           (MIR_GOTO_LS == T_mir::KIND) ||
           (MIR_GOTO_GE == T_mir::KIND) ||
           (MIR_GOTO_LT == T_mir::KIND) ||
           (MIR_GOTO_GT == T_mir::KIND) ||
           (MIR_GOTO_LE == T_mir::KIND));
    
    // :NOTE: Wei 2004-Oct-12:
    //
    // There should only be one 'GOTO' node in a basic block, and that 'GOTO'
    // node should be the last one, too.
    //
    // Thus, I don't need to search an existed and matched 'GOTO' node in a
    // basic block.
    assert(mir == mp_mir_tail);
    
    assert(0 == mir->out_flags());
    
    DAGNode * const goto_node =
      add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0),
                                   mir->label<T_mir::KIND>()));
    
    if (T_mir::KIND != MIR_GOTO)
    {
      goto_node->set_in_flags(mir->in_flags());
    }
    else
    {
      assert(0 == mir->in_flags());
    }
    
    link_nodes(goto_node);
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_PRODUCE_CBIT_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_PRODUCE_CBIT == T_mir::KIND);
    
    assert(0 == mir->in_flags());
    
    if ((mir->out_flags() & CARRY_FLAG) != 0)
    {
      DAGNode * const operation_node =
        add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
      
      operation_node->set_out_flags(mir->out_flags());
      
      std::vector<Variable *> const &child_var = mir->operand_variable();
      std::vector<DAGNode *> child_node;
      
      for (std::vector<Variable *>::const_iterator iter = child_var.begin();
           iter != child_var.end();
           ++iter)
      {
        child_node.push_back(find_var_dag_node(*iter));
      }
      
      std::vector<Variable *>::const_iterator var_iter = child_var.begin();
      std::vector<DAGNode *>::const_iterator node_iter = child_node.begin();
      
      for (; var_iter != child_var.end(); ++var_iter, ++node_iter)
      {
        add_child_node(operation_node, *node_iter, *var_iter);
      }
      
      increase_child_node_global_attached_var_ref_cnt(operation_node, child_var);
      
      link_nodes(operation_node);
    }
  }
  
  template<typename T_mir>
  inline void
  BasicBlock::add_CLEAR_CBIT_type_mir_into_dag_internal(MIR * const mir)
  {
    assert(MIR_CLEAR_CBIT == T_mir::KIND);
    assert(0 == mir->in_flags());
    
    if ((mir->out_flags() & CARRY_FLAG) != 0)
    {
      DAGNode * const operation_node =
        add_new_dag_node(new DAGNode(static_cast<typename T_mir::DAGNodeType *>(0)));
      
      operation_node->set_out_flags(mir->out_flags());
      
      link_nodes(operation_node);
    }
  }
}

#endif
