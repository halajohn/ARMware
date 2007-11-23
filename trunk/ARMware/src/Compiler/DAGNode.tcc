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

#if PRINT_CHUNK_CODE
#include "../Log.hpp"
#endif

namespace ARMware
{
#if CHECK_CHUNK_CODE
  template<VarAttachedTypeEnum T_attach>
  bool
  DAGNode::ensure_var_attached_internal(Variable * const var) const
  {
    for (std::list<Variable *>::const_iterator iter = attached_var<T_attach>().begin();
         iter != attached_var<T_attach>().end();
         ++iter)
    {
      if ((*iter) == var)
      {
        return true;
      }
    }
    
    return false;
  }
#endif
  
#if PRINT_CHUNK_CODE
  template<VarAttachedTypeEnum T_attach>
  void
  DAGNode::dump_attached_var_internal() const
  {
    for (std::list<Variable *>::const_iterator iter = attached_var<T_attach>().begin();
         iter != attached_var<T_attach>().end();
         ++iter)
    {
      (*iter)->dump_info<false>();
      
      g_log_file << " ";
    }
  }
#endif
  
  template<VarAttachedTypeEnum T_attach>
  inline void
  DAGNode::cleanup_attached_var_internal()
  {
    for (std::list<Variable *>::const_iterator iter = attached_var<T_attach>().begin();
         iter != attached_var<T_attach>().end();
         ++iter)
    {
      switch ((*iter)->kind())
      {
      case Variable::CONST:
        assert(0 == (*iter)->dag_node());
        break;
        
      case Variable::GLOBAL:
        assert((*iter)->dag_node() != this);
        break;
        
      case Variable::TEMP:
        assert((false == (*iter)->is_act_as_global_var()) &&
               (this == (*iter)->dag_node()));
        
        gp_chunk->remove_temp_var(*iter);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    attached_var<T_attach>().clear();
  }
  
  template<VarAttachedTypeEnum T_attach>
  inline Variable *
  DAGNode::get_first_usable_attached_var()
  {
#if CHECK_CHUNK_CODE
    switch (T_attach)
    {
    case VAR_ATTACH_HI:
    case VAR_ATTACH_LO:
      assert((DAG_UMUL64 == m_kind) ||
             (DAG_SMUL64 == m_kind) ||
             (DAG_ADD64 == m_kind));
      
    case VAR_ATTACH_NORMAL:
      break;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
#endif
    
    assert(attached_var<T_attach>().size() != 0);
    
    switch (m_kind)
    {
    case DAG_VAR_TEMP:
    case DAG_VAR_CONST:
    case DAG_VAR_GLOBAL:
      assert(VAR_ATTACH_NORMAL == T_attach);
      assert(1 == attached_var<VAR_ATTACH_NORMAL>().size());
      
      return attached_var<VAR_ATTACH_NORMAL>().front();
      
    default:
      {
        for (std::list<Variable *>::iterator iter = attached_var<T_attach>().begin();
             iter != attached_var_iter<T_attach>();
             ++iter)
        {
          assert((*iter)->curr_dag_node() != 0);
          
          if (this == (*iter)->curr_dag_node())
          {
#if CHECK_CHUNK_CODE
            Variable * const found_var = *iter;
#endif
            
            // :NOTE: Wei 2005-May-08:
            //
            // According to the C++ standard:
            //
            // xIn a std::list, if you erase or insert elements anywhere in the list, all
            // iterators and references to objects in the list remain valid.  Except, of
            // course, for the element you erased.
            //
            // Thus, an Iterator that designates an element of a list<T> object remains
            // valid until the element is erased. 
            //
            // Thus, I will preform following move operation to move the one I found
            // to the beginning of this list, and I will not touch m_hi(lo)_attached_var_iter,
            // thus this iterator will still be valid.
            
            // :NOTE: Wei 2005-Apr-24:
            //
            // Move the one I found to the beginning of the attached variable list,
            // such that I can find it quicker in the future.
            attached_var<T_attach>().splice(attached_var<T_attach>().begin(),
                                            attached_var<T_attach>(),
                                            iter);
            
#if CHECK_CHUNK_CODE
            // :NOTE: Wei 2005-Apr-24:
            //
            // I have to check this out because the C++ standard (at least now)
            // states that iterators to the spliced element(s) are invalidated.
            //
            // Thus if I use 'iter' here, the result is undefined.
            //
            // However, there is one item in the current C++ standard library defect report.
            // It says:
            //
            // ---
            // The original proposed resolution said that iterators and references would remain "valid".
            // The new proposed resolution clarifies what that means.
            // Note that this only applies to the case of equal allocators.
            // ---
            //
            // This means if the two allocators that the two lists have are equal.
            // The the iterator to the spliced element will be remain valid.
            //
            // Note: two allocators are equal if storage allocated by allocator a1 can be
            // deallocated by allocator a2 and vice-versa.
            //
            // And in out case, because we are about to move element in the same container,
            // thus I can almost can sure that the iterator will be valid.
            assert((*iter) == found_var);
#endif
            
            return *iter;
          }
        }
        
        return 0;
      }
    }
  }
}

#endif
