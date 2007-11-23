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

#include "../Chunk.hpp"
#include "ChildInfo.hpp"
#include "DAGNode.hpp"
#include "BasicBlock.hpp"
#include "x86/LIR.hpp"

#include "DAGNode.tcc"

namespace ARMware
{
  class Variable;
  
  /////////////////////////////////////////////////////////////////////////////
  // Public == Operation ======================================================
  
  Variable *
  ChildInfo::peek_first_usable_attached_var() const
  {
    switch (m_attached_type)
    {
    case VAR_ATTACH_HI:
      return mp_child->peek_first_usable_attached_var<VAR_ATTACH_HI>();
      
    case VAR_ATTACH_LO:
      return mp_child->peek_first_usable_attached_var<VAR_ATTACH_LO>();
      
    case VAR_ATTACH_NORMAL:
      return mp_child->peek_first_usable_attached_var<VAR_ATTACH_NORMAL>();
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  std::list<Variable *> const &
  ChildInfo::child_attached_var() const
  {
    switch (m_attached_type)
    {
    case VAR_ATTACH_HI:
      return mp_child->attached_var<VAR_ATTACH_HI>();
      
    case VAR_ATTACH_LO:
    case VAR_ATTACH_NORMAL:
      return mp_child->attached_var<VAR_ATTACH_LO>();
      
    default:
      assert(!"Should not reach here.");
      return mp_child->attached_var<VAR_ATTACH_LO>();
    }
  }
}

#endif
