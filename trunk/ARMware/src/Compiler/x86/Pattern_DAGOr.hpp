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

#ifndef Pattern_DAGOr_hpp
#define Pattern_DAGOr_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGOr
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_OR == node->kind());
      
      // :NOTE: Wei 2005-Mar-09:
      //
      // The out flags of the OR instruction should _not_ include CARRY & OVERFLOW.
      assert(0 == (node->out_flags() & (CARRY_FLAG | OVERFLOW_FLAG)));
      assert(0 == node->in_flags());
      
      assert(node->attached_var_amount() != 0);
      
      Variable *src_var = 0;
      
      if (0 == node->out_flags())
      {
        // :NOTE: Wei 2005-May-08:
        //
        // Try to get previous still valid attached variable.
        src_var = node->get_first_usable_attached_var<VAR_ATTACH_NORMAL>();
      }
      else
      {
        assert(node->attached_var<VAR_ATTACH_NORMAL>().begin() ==
               node->attached_var_iter<VAR_ATTACH_NORMAL>());
      }
      
      if (src_var != 0)
      {
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          src_var,
                                          node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
      }
      else
      {
        std::deque<ChildInfo> const &children = node->children();
        
        ChildInfo const &child_0 = children.front();
        ChildInfo const &child_1 = children.back();
        
        LIR *main_lir
#if CHECK_CHUNK_CODE
          = 0
#endif
          ;
        
        switch (child_0.node()->kind())
        {
        case DAG_VAR_CONST:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            assert(!"Or of 2 constants. This shouldn't be happened, because I have already implement OR constant folding.");
            main_lir = 0;
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_1.peek_first_usable_attached_var(),
                                                         child_0.peek_first_usable_attached_var()->const_num(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
            
          default:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_1.peek_first_usable_attached_var(),
                                                         child_0.peek_first_usable_attached_var()->const_num(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
          }
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_0.peek_first_usable_attached_var(),
                                                         child_1.peek_first_usable_attached_var()->const_num(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_0.peek_first_usable_attached_var(),
                                                         child_1.peek_first_usable_attached_var(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
            
          default:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_0.peek_first_usable_attached_var(),
                                                         child_1.peek_first_usable_attached_var(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
          }
          break;
          
        default:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_0.peek_first_usable_attached_var(),
                                                         child_1.peek_first_usable_attached_var()->const_num(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_1.peek_first_usable_attached_var(),
                                                         child_0.peek_first_usable_attached_var(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
            
          default:
            main_lir = curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                         child_0.peek_first_usable_attached_var(),
                                                         child_1.peek_first_usable_attached_var(),
                                                         node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
            break;
          }
          break;
        }
        
        if (node->out_flags() != 0)
        {
          assert(main_lir != 0);
          
          main_lir->set_cond_flags_involved();
        }
        
        store_native_flags(curr_bb, node->out_flags());
      }
      
      node->get_curr_emission_var<VAR_ATTACH_NORMAL>()->set_curr_dag_node(node);
      
      node->jump_to_next_emission_var<VAR_ATTACH_NORMAL>();
    }
  }
}

#endif

#endif
