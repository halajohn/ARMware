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

#include "Variable.hpp"
#include "x86/LIR.hpp"

#if ENABLE_DYNAMIC_TRANSLATOR

#include <algorithm>

#if PRINT_CHUNK_CODE
#include <cassert>

#include <iostream>
#include "../Log.hpp"
#endif

#endif

#include "../Core.hpp"
#include "DAGNode.hpp"
#include "../ARMware.hpp"
#include "x86/Register.hpp"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // Public == Operation ======================================================
  
#if PRINT_CHUNK_CODE
  std::string const Variable::GLOBAL_VAR_NAME[] =
  {
    "CR_0",
    "CR_1",
    "CR_2",
    "CR_3",
    "CR_4",
    "CR_5",
    "CR_6",
    "CR_7",
    "CR_8",
    "CR_9",
    "CR_10",
    "CR_11",
    "CR_12",
    "CR_13",
    "CR_14",
    "CR_15",
    
    "CPSR",
    "SPSR",
    
    "EXCEPTION_TYPE",
    "DEVICE_TIMER_INCREMENT"
  };
#endif
  
#if ENABLE_DYNAMIC_TRANSLATOR
  Variable::~Variable()
  {
    std::for_each(m_defined_bb.begin(), m_defined_bb.end(), del_fun<DefinedBBPair>());
    
    mp_dag_node = 0;
    
    if (mp_live_interval != 0)
    {
      delete mp_live_interval;
    }
  }
  
  void
  Variable::update_attached_type_and_dag_node(VarAttachedTypeEnum type,
                                              DAGNode * const node)
  {
    assert(node != 0);
    assert(mp_dag_node != node);
    
    if (mp_dag_node != 0)
    {
      mp_dag_node->remove_attached_var(this);
    }
    
    m_attached_type = type;
    mp_dag_node = node;
    
    switch (m_attached_type)
    {
    case VAR_ATTACH_HI:
      assert((DAG_UMUL64 == node->kind()) ||
             (DAG_SMUL64 == node->kind()) ||
             (DAG_ADD64 == node->kind()));
      
      node->add_attached_var<VAR_ATTACH_HI>(this);
      break;
      
    case VAR_ATTACH_LO:
      assert((DAG_UMUL64 == node->kind()) ||
             (DAG_SMUL64 == node->kind()) ||
             (DAG_ADD64 == node->kind()));
      
      node->add_attached_var<VAR_ATTACH_LO>(this);
      break;
      
    case VAR_ATTACH_NORMAL:
      assert((DAG_UMUL64 != node->kind()) &&
             (DAG_SMUL64 != node->kind()) &&
             (DAG_ADD64 != node->kind()));
      
      node->add_attached_var<VAR_ATTACH_NORMAL>(this);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  uint32_t
  Variable::memory_addr() const
  {
    assert(GLOBAL == m_kind);
    
    return reinterpret_cast<uint32_t>(gp_core->get_global_var_mem_addr(u.m_GLOBAL.m_global_var));
  }
  
  void
  Variable::pop_ssa_var()
  {
    std::vector<Variable *> &vect = mp_original_ssa_var->var_stack();
    
    assert(vect.size() > 0);
    
    vect.pop_back();
  }
  
#if CHECK_CHUNK_CODE
  void
  Variable::check_defined_lir()
  {
    for (std::list<LIR *>::const_iterator iter = m_defined_lir.begin();
         iter != m_defined_lir.end();
         ++iter)
    {
      (*iter)->check_var_is_dest(this);
    }
  }
  
  void
  Variable::check_not_deleted()
  {
    assert(0 == mp_next_free_node);
  }
#endif
#endif
}
