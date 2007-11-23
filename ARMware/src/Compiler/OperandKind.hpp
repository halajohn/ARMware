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

#ifndef OperandKind_hpp
#define OperandKind_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include "Compiler.hpp"
#include "Label.hpp"
#include "../MemoryPool2.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Register;
  class Variable;
  class BasicBlock;
  
  typedef class OperandKind OperandKind;
  class OperandKind : public MemoryPool2<OperandKind, 256>
  {
  public:
    
    enum KindEnum
    {
      VARIABLE,
      IMM,
      LABEL,
      RAW_ADDR
    };
    typedef enum KindEnum KindEnum;
    
  private:
    
    KindEnum m_kind;
    
    union
    {
      Variable *mp_var;
      Label *mp_label;
      uint32_t m_value;
      void *m_addr;
    } u;
    
    Register *mp_reg;
    
    friend class MemoryPool2<OperandKind, 256>;
    
  public:
    
    // Life cycle
    
    inline void
    init()
    {
      mp_reg = 0;
    }
    
    OperandKind(Variable * const var)
      : m_kind(VARIABLE)
    {
      u.mp_var = var;
      
      init();
    }
    
    inline
    OperandKind(Label * const label)
      : m_kind(LABEL)
    {
      u.mp_label = label;
      
      init();
    }
    
    inline
    OperandKind(uint32_t const value)
      : m_kind(IMM)
    {
      u.m_value = value;
      
      init();
    }
    
    inline
    OperandKind(void *addr)
      : m_kind(RAW_ADDR)
    {
      u.m_addr = addr;
      
      init();
    }
    
    ~OperandKind();
    
    // Inquiry
    
    inline KindEnum
    kind() const
    { return m_kind; }
    
    inline Variable *
    variable() const
    {
      assert(VARIABLE == m_kind);
      
      return u.mp_var;
    }
    
    inline Label *
    label() const
    {
      assert(LABEL == m_kind);
      
      return u.mp_label;
    }
    
    inline uint32_t
    imm() const
    {
      assert(IMM == m_kind);
      
      return u.m_value;
    }
    
    inline void *
    addr() const
    {
      assert(RAW_ADDR == m_kind);
      
      return u.m_addr;
    }
    
    // Access
    
    inline void
    set_variable(Variable * const var)
    {
      assert(VARIABLE == m_kind);
      
      u.mp_var = var;
    }
    
    inline void
    set_expected_reg(Register * const reg)
    {
      assert(VARIABLE == m_kind);
      
      mp_reg = reg;
    }
  };
}

#endif

#endif
