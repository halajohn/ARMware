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

#ifndef Pattern_DAGCall_hpp
#define Pattern_DAGCall_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGCall
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_CALL == node->kind());
      
      assert(0 == (node->in_flags() | node->out_flags()));
      assert(0 == node->attached_var_amount());
      
      std::deque<ChildInfo> const &children = node->children();
      
      for (std::deque<ChildInfo>::const_iterator iter = children.begin();
           iter != children.end();
           ++iter)
      {
        switch ((*iter).peek_first_usable_attached_var()->kind())
        {
        case Variable::GLOBAL:
        case Variable::TEMP:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRPush *>(0),
                                            (*iter).peek_first_usable_attached_var()));
          break;
          
        case Variable::CONST:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRPush *>(0),
                                            (*iter).peek_first_usable_attached_var()->const_num()));
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      
      curr_bb->append_lir<true>(new LIR(static_cast<LIRCall *>(0), node->label()));
    }
  }
}

#endif

#endif
