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

#ifndef ChildInfo_hpp
#define ChildInfo_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>

#include "Compiler.hpp"

namespace ARMware
{
  class DAGNode;
  class Variable;
  
  typedef class ChildInfo ChildInfo;
  class ChildInfo
  {
  private:
    
    // Attribute
    
    // :NOTE: Wei 2004-Nov-24:
    //
    // I can't add 'const' attribute to the following 2 fields,
    // because I use std::deque to store each ChildInfo in each DAGNode,
    // and I may use std::deque::erase() to remove one ChildInfo in DAGNode,
    // and std::deque::erase() will use default assignment operator of ChildInfo
    // to copy 之後的 ChildInfo structure 往前一格.
    //
    // Thus, if I add 'const' attribute to these 2 fields,
    // then the default assignment operator will not work.
    DAGNode * /* const */ mp_child;
    VarAttachedTypeEnum /* const */ m_attached_type;
    
    uint32_t m_global_attached_var_ref_cnt;
    
  public:
    
    // Life cycle
    
    // :NOTE: Wei 2004-Nov-24:
    //
    // Because I may use std::deque::resize() to shrink the 'm_children' field
    // of each DAGNode.
    //
    // However, because std::deque::resize() may be used to enlarge a std::deque,
    // thus, std::deque::resize() needs the default constructor of its element,
    // and because I provide a user-defined constructor, thus the C++ compiler will
    // not provide a default constructor for me, I have to create one by my self.
    inline
    ChildInfo()
    {
      assert(!"Should not reach here.");
    }
    
    inline
    ChildInfo(DAGNode * const child,
              VarAttachedTypeEnum const attached_type,
              uint32_t const ref_cnt = 0)
      : mp_child(child),
        m_attached_type(attached_type),
        m_global_attached_var_ref_cnt(ref_cnt)
    { }
    
    inline
    ChildInfo(ChildInfo const &info)
    {
      m_attached_type = info.attached_type();
      mp_child = info.node();
      m_global_attached_var_ref_cnt = info.global_attached_var_ref_cnt();
    }
    
    // Inquiry
    
    inline DAGNode *
    node() const
    { return mp_child; }
    
    inline VarAttachedTypeEnum
    attached_type() const
    { return m_attached_type; }
    
    inline uint32_t
    global_attached_var_ref_cnt() const
    { return m_global_attached_var_ref_cnt; }
    
    // Access
    
    inline void
    set_node(DAGNode * const node)
    {
      assert(node != 0);
      
      mp_child = node;
    }
    
    inline void
    set_attached_type(VarAttachedTypeEnum const type)
    { m_attached_type = type; }
    
    inline void
    inc_global_attached_var_ref_cnt()
    { ++m_global_attached_var_ref_cnt; }
    
    // Operator
    
    inline ChildInfo &
    operator=(ChildInfo const &info)
    {
      m_attached_type = info.attached_type();
      mp_child = info.node();
      m_global_attached_var_ref_cnt = info.global_attached_var_ref_cnt();
      
      return *this;
    }
    
    // Operation
    
    Variable *peek_first_usable_attached_var() const;
    
    std::list<Variable *> const &child_attached_var() const;
  };
}

#endif

#endif
