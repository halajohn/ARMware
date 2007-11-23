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

#ifndef Pattern_DAGLoad_hpp
#define Pattern_DAGLoad_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGLoad
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_LOAD == node->kind());
      
      assert(0 == (node->in_flags() | node->out_flags()));
      
      assert(node->attached_var_amount() != 0);
      
      Variable *src_var = 0;
      
      // :NOTE: Wei 2005-May-08:
      //
      // Try to get previous still valid attached variable.
      src_var = node->get_first_usable_attached_var<VAR_ATTACH_NORMAL>();
      
      if (src_var != 0)
      {
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          src_var,
                                          node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
      }
      else
      {
        ChildInfo const child = node->children().front();
        
        switch (child.node()->kind())
        {
        case DAG_VAR_CONST:
        case DAG_VAR_GLOBAL:
          assert(!"Should not reach here.");
          break;
          
        case DAG_VAR_TEMP:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRMov_from_M *>(0),
                                            child.peek_first_usable_attached_var(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
          
        default:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRMov_from_M *>(0),
                                            child.peek_first_usable_attached_var(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
        }
      }
      
      node->get_curr_emission_var<VAR_ATTACH_NORMAL>()->set_curr_dag_node(node);
      
      node->jump_to_next_emission_var<VAR_ATTACH_NORMAL>();
    }
  }
}

#endif

#endif
