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

#ifndef Pattern_DAGLoadLabel_hpp
#define Pattern_DAGLoadLabel_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGLoadLabel
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_LOAD_LABEL == node->kind());
      
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
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          node->label(),
                                          node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
      }
      
      node->get_curr_emission_var<VAR_ATTACH_NORMAL>()->set_curr_dag_node(node);
      
      node->jump_to_next_emission_var<VAR_ATTACH_NORMAL>();
    }
  }
}

#endif

#endif
