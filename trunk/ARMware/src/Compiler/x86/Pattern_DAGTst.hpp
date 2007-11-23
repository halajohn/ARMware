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

#ifndef Pattern_DAGTst_hpp
#define Pattern_DAGTst_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGTst
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_TST == node->kind());
      
      assert(0 == (node->out_flags() & (CARRY_FLAG | OVERFLOW_FLAG)));
      assert(0 == node->in_flags());
      assert(0 == node->attached_var_amount());
      
      std::deque<ChildInfo> const &children = node->children();
      
      ChildInfo const &child_0 = children.front();
      ChildInfo const &child_1 = children.back();
      
      switch (child_0.node()->kind())
      {
      case DAG_VAR_CONST:
        switch (child_1.node()->kind())
        {
        case DAG_VAR_CONST:
          assert(!"The 1st operand of the TST instruction in x86 can't be a constant value.");
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_1.peek_first_usable_attached_var(),
                                            child_0.peek_first_usable_attached_var()->const_num()));
          break;
          
        default:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_1.peek_first_usable_attached_var(),
                                            child_0.peek_first_usable_attached_var()->const_num()));
          break;
        }
        break;
        
      case DAG_VAR_TEMP:
      case DAG_VAR_GLOBAL:
        switch (child_1.node()->kind())
        {
        case DAG_VAR_CONST:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var()->const_num()));
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var()));
          break;
          
        default:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var()));
          break;
        }
        break;
        
      default:
        switch (child_1.node()->kind())
        {
        case DAG_VAR_CONST:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var()->const_num()));
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_1.peek_first_usable_attached_var(),
                                            child_0.peek_first_usable_attached_var()));
          break;
          
        default:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var()));
          break;
        }
        break;
      }
      
      store_native_flags(curr_bb, node->out_flags());
    }
  }
}

#endif

#endif
