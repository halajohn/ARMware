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

#ifndef Pattern_DAGReturn_hpp
#define Pattern_DAGReturn_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGReturn
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_RETURN == node->kind());
      
      assert(0 == (node->in_flags() | node->out_flags()));
      
      assert(0 == node->attached_var_amount());
      assert(1 == node->children().size());
      
      ChildInfo const &child = node->children().front();
      
      Variable * const tmp_a = gp_chunk->get_new_temp();
      
      tmp_a->set_preallocated_reg(new Register(Register::EAX, true));
      
      switch (child.node()->kind())
      {
      case DAG_VAR_CONST:
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          child.peek_first_usable_attached_var()->const_num(),
                                          tmp_a));
        break;
        
      case DAG_VAR_TEMP:
      case DAG_VAR_GLOBAL:
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          child.peek_first_usable_attached_var(),
                                          tmp_a));
        break;
        
      default:
        // :NOTE: Wei 2004-Nov-11:
        //
        // The child node of a DAGReturn node should have at least one variable
        // attached, otherwise the DAGReturn node wouldn't link this node as
        // its child node.
        assert(child.node()->attached_var_amount() >= 1);
        
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          child.peek_first_usable_attached_var(),
                                          tmp_a));
        break;
      }
      
      curr_bb->append_lir<true>(new LIR(static_cast<LIRReturn *>(0)));
    }
  }
}

#endif

#endif
