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

#ifndef Pattern_DAGRetrieve_hpp
#define Pattern_DAGRetrieve_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGRetrieve
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_RETRIEVE == node->kind());
      
      assert(0 == (node->in_flags() | node->out_flags()));
      
      assert(node->attached_var_amount() <= 1);
      
      if (node->pop_count() != 0)
      {
        curr_bb->append_lir<true>(new LIR(static_cast<LIRAddSP *>(0), 
                                          (node->pop_count() << 2)));
      }
      
      if (node->attached_var_amount() != 0)
      {
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov_from_A *>(0),
                                          node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
        
        node->get_curr_emission_var<VAR_ATTACH_NORMAL>()->set_curr_dag_node(node);
        
        node->jump_to_next_emission_var<VAR_ATTACH_NORMAL>();
      }
    }
  }
}

#endif

#endif
