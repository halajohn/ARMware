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

#ifndef DefinedBBPair_hpp
#define DefinedBBPair_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include "../MemoryPool2.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class BasicBlock;
  
  typedef class DefinedBBPair DefinedBBPair;
  class DefinedBBPair : public MemoryPool2<DefinedBBPair, 1024>
  {
  private:
    
    BasicBlock * const mp_defined_bb;
    uint32_t m_defined_count;
    
    friend class MemoryPool2<DefinedBBPair, 1024>;
    
  public:
    
    // Life cycle
    
    DefinedBBPair(BasicBlock * const defined_bb)
      : mp_defined_bb(defined_bb),
        m_defined_count(1)
    { }
    
    // Inquiry
    
    inline BasicBlock *
    defined_bb() const
    { return mp_defined_bb; }
    
    inline uint32_t
    defined_count() const
    { return m_defined_count; }
    
    // Operation
    
    inline void
    increment_defined_count()
    { ++m_defined_count; }
    
    inline void
    decrement_defined_count()
    { --m_defined_count; }
  };
}

#endif

#endif
