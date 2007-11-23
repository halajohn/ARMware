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

#ifndef Pattern_DAGProduceCBit_hpp
#define Pattern_DAGProduceCBit_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGProduceCBit
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_PRODUCE_CBIT == node->kind());
      
      assert(0 == node->in_flags());
      assert(0 == (node->out_flags() & (SIGN_FLAG | ZERO_FLAG | OVERFLOW_FLAG)));
      
      assert(0 == node->attached_var_amount());
      
      if (CARRY_FLAG == node->out_flags())
      {
        std::deque<ChildInfo> const &children = node->children();
        
        ChildInfo const &child_0 = children.front();
        ChildInfo const &child_1 = children.back();
        
        switch (child_0.node()->kind())
        {
        case DAG_VAR_CONST:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            if ((child_0.peek_first_usable_attached_var()->const_num() &
                 (1 << child_1.peek_first_usable_attached_var()->const_num())) != 0)
            {
              // :NOTE: Wei 2004-Nov-14:
              //
              //         o ProduceCBit     o DAGSetCBit
              //      |      |     ->
              //    const  const
              
              curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                                gp_chunk->find_global_var(Variable::CPSR),
                                                (1 << NativeCBit::CORRESPONDING_ARM_BIT_POSITION),
                                                gp_chunk->find_global_var(Variable::CPSR)));
            }
            else
            {
              // :NOTE: Wei 2004-Nov-14:
              //
              //         o ProduceCBit     o DAGClearCBit
              //      |      |     ->
              //    const  const
              
              curr_bb->append_lir<true>(new LIR(static_cast<LIRAnd *>(0),
                                                gp_chunk->find_global_var(Variable::CPSR),
                                                static_cast<uint32_t>(~(1 << NativeCBit::CORRESPONDING_ARM_BIT_POSITION)),
                                                gp_chunk->find_global_var(Variable::CPSR)));
            }
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            assert(!"Should not reach here.");
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            curr_bb->append_lir<true>(new LIR(static_cast<LIRBt *>(0),
                                              child_0.peek_first_usable_attached_var(),
                                              child_1.peek_first_usable_attached_var()->const_num()));
            
            store_native_flags(curr_bb, CARRY_FLAG);
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            assert(!"Should not reach here.");
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          break;
          
        default:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            curr_bb->append_lir<true>(new LIR(static_cast<LIRBt *>(0),
                                              child_0.peek_first_usable_attached_var(),
                                              child_1.peek_first_usable_attached_var()->const_num()));
            
            store_native_flags(curr_bb, CARRY_FLAG);
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            assert(!"Should not reach here.");
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
          break;
        }
      }
    }
  }
}

#endif

#endif
