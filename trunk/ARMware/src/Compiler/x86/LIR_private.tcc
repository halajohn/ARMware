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

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  template<DefUseEnum T_type, VariableTypeEnum T_var_type>
  inline void
  LIR::set_ssa_variable_internal(OperandKind * const operand)
  {
    assert(OperandKind::VARIABLE == operand->kind());
    assert(operand->variable() != 0);
    
    Variable * const new_var = operand->variable()->template get_ssa_var<T_type>();
    
    if (m_kind != PHI)
    {
      // :NOTE: Wei 2005-Feb-28:
      //
      // Used in the 'linear scan register allocation' stage.
      // and accesses in a f-function are neglected.
      new_var->increment_access_times();
    }
    
    switch (T_type)
    {
    case DEF:
      set_ssa_def_variable<T_var_type>(operand->variable(), new_var);
      
      // :NOTE: Wei 2005-Feb-28:
      //
      // Full throw.
      
    case USE:
      operand->set_variable(new_var);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}

#endif
