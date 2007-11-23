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

#ifndef Pattern_DAGGotoMI_hpp
#define Pattern_DAGGotoMI_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGGotoMI
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_GOTO_MI == node->kind());
      
      assert(0 == node->out_flags());
      assert(CondCodeTranslationTraits<MI>::PRELOAD_STATUS_FLAG == node->in_flags());
      
      assert(0 == node->attached_var_amount());
      
      load_native_flags(curr_bb, CondCodeTranslationTraits<MI>::PRELOAD_STATUS_FLAG);
      
      curr_bb->append_lir<true>(new LIR(static_cast<LIRJs *>(0), node->label()));
    }
  }
}

#endif

#endif
