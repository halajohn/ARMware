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

#ifndef ConditionBlock_hpp
#define ConditionBlock_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include "../MemoryPool2.hpp"

namespace ARMware
{
  typedef class ConditionBlock ConditionBlock;
  class ConditionBlock : public MemoryPool2<ConditionBlock, 128>
  {
  private:
    
    InstCond const m_cond;
    
    friend class MemoryPool2<ConditionBlock, 128>;
    
  public:
    
    // Life cycle
    
    inline
    ConditionBlock(InstCond const cond)
      : m_cond(cond)
    { }
    
    // Inquiry
    
    inline InstCond
    cond() const
    { return m_cond; }
  };
}

#endif

#endif
