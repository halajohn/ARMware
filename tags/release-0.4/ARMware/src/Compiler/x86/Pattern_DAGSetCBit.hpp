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

#ifndef Pattern_DAGSetCBit_hpp
#define Pattern_DAGSetCBit_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGSetCBit
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_SET_CBIT == node->kind());
      
      assert(0 == node->in_flags());
      assert(0 == (node->out_flags() & (SIGN_FLAG | ZERO_FLAG | OVERFLOW_FLAG)));
      
      assert(0 == node->attached_var_amount());
      
      if (CARRY_FLAG == node->out_flags())
      {
        curr_bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                          gp_chunk->find_global_var(Variable::CPSR),
                                          (1 << NativeCBit::CORRESPONDING_ARM_BIT_POSITION),
                                          gp_chunk->find_global_var(Variable::CPSR)));
      }
    }
  }
}

#endif

#endif
