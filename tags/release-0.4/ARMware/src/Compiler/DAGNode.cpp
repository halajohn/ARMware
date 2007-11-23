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

#include <cassert>
#include <iostream>

#include "../Chunk.hpp"

#include "DAGNode.hpp"
#include "Variable.hpp"
#include "BasicBlock.hpp"
#include "x86/LIR.hpp"

#include "DAGNode.tcc"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // Private ==================================================================
  
#if PRINT_CHUNK_CODE || CHECK_CHUNK_CODE
  uint32_t DAGNode::s_sequence_idx;
#endif
  
#if PRINT_CHUNK_CODE
  std::string const DAGNode::KIND_NAME[] =
  {
    "DAG_VAR_TEMP",
    "DAG_VAR_GLOBAL",
    "DAG_VAR_CONST",
    
    "DAG_CALL",
    "DAG_RETURN",
    "DAG_RETRIEVE",
    "DAG_ADD",
    "DAG_ADD64",
    "DAG_ADC",
    "DAG_MOV",
    "DAG_AND",
    "DAG_OR",
    "DAG_EOR",
    "DAG_SUB",
    "DAG_SBC",
    "DAG_TST",
    "DAG_CMP",
    "DAG_CMP_EQ",
    "DAG_NOT",
    "DAG_MUL32",
    "DAG_UMUL64",
    "DAG_SMUL64",
    "DAG_LSFT_LEFT",
    "DAG_LSFT_RIGHT",
    "DAG_ASFT_RIGHT",
    "DAG_RRX",
    "DAG_ROR",
    "DAG_GOTO",
    
    "DAG_GOTO_EQ",
    "DAG_GOTO_NE",
    "DAG_GOTO_CS",
    "DAG_GOTO_CC",
    "DAG_GOTO_MI",
    "DAG_GOTO_PL",
    "DAG_GOTO_VS",
    "DAG_GOTO_VC",
    "DAG_GOTO_HI",
    "DAG_GOTO_LS",
    "DAG_GOTO_GE",
    "DAG_GOTO_LT",
    "DAG_GOTO_GT",
    "DAG_GOTO_LE",
    
    "DAG_JUMP",
    "DAG_PRODUCE_CBIT",
    "DAG_SET_CBIT",
    "DAG_CLEAR_CBIT",
    "DAG_LOAD_LABEL",
    "DAG_LOAD"
  };
#endif
  
#if CHECK_CHUNK_CODE  
  void
  DAGNode::ensure_var_attached(Variable * const var) const
  {
    switch (m_kind)
    {
    case DAG_UMUL64:
    case DAG_SMUL64:
    case DAG_ADD64:
      if (true == ensure_var_attached_internal<VAR_ATTACH_HI>(var))
      {
        return;
      }
      
    default:
      if (true == ensure_var_attached_internal<VAR_ATTACH_NORMAL>(var))
      {
        return;
      }
      break;
    }
    
    assert(!"The desired variable isn't attached");
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  // Public == Operation ======================================================
    
#if PRINT_CHUNK_CODE
  void
  DAGNode::dump_attached_var() const
  {
    switch (m_kind)
    {
    case DAG_UMUL64:
    case DAG_SMUL64:
    case DAG_ADD64:
      dump_attached_var_internal<VAR_ATTACH_HI>();
      
    default:
      dump_attached_var_internal<VAR_ATTACH_LO>();
      break;
    }
  }
  
  void
  DAGNode::dump_info() const
  {
    g_log_file << "<"
               << std::dec
               << m_sequence_idx
               << "> "
               << KIND_NAME[m_kind]
               << " ["
               << ((m_in_flags & SIGN_FLAG) ? "N" : "-")
               << ((m_in_flags & ZERO_FLAG) ? "Z" : "-")
               << ((m_in_flags & CARRY_FLAG) ? "C" : "-")
               << ((m_in_flags & OVERFLOW_FLAG) ? "V" : "-")
               << "] ["
               << ((m_out_flags & SIGN_FLAG) ? "N" : "-")
               << ((m_out_flags & ZERO_FLAG) ? "Z" : "-")
               << ((m_out_flags & CARRY_FLAG) ? "C" : "-")
               << ((m_out_flags & OVERFLOW_FLAG) ? "V" : "-")
               << "] ";
    
    switch (m_kind)
    {
    case DAG_CALL:
    case DAG_RETURN:
    case DAG_RETRIEVE:
    case DAG_ADD:
    case DAG_ADD64:
    case DAG_ADC:
    case DAG_MOV:
    case DAG_AND:
    case DAG_OR:
    case DAG_EOR:
    case DAG_SUB:
    case DAG_SBC:
    case DAG_TST:
    case DAG_CMP:
    case DAG_CMP_EQ:
    case DAG_NOT:
    case DAG_MUL32:
    case DAG_UMUL64:
    case DAG_SMUL64:
    case DAG_LSFT_LEFT:
    case DAG_LSFT_RIGHT:
    case DAG_ASFT_RIGHT:
    case DAG_RRX:
    case DAG_ROR:
    case DAG_GOTO:
      
    case DAG_GOTO_EQ:
    case DAG_GOTO_NE:
    case DAG_GOTO_CS:
    case DAG_GOTO_CC:
    case DAG_GOTO_MI:
    case DAG_GOTO_PL:
    case DAG_GOTO_VS:
    case DAG_GOTO_VC:
    case DAG_GOTO_HI:
    case DAG_GOTO_LS:
    case DAG_GOTO_GE:
    case DAG_GOTO_LT:
    case DAG_GOTO_GT:
    case DAG_GOTO_LE:
      
    case DAG_JUMP:
    case DAG_PRODUCE_CBIT:
    case DAG_SET_CBIT:
    case DAG_CLEAR_CBIT:
    case DAG_LOAD_LABEL:
    case DAG_LOAD:
      
    case DAG_VAR_TEMP:
    case DAG_VAR_GLOBAL:
    case DAG_VAR_CONST:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    if (attached_var_amount() != 0)
    {
      // :NOTE: Wei 2004-Nov-10:
      //
      // Dump attached variables.
      g_log_file << " (";
      
      dump_attached_var();
      
      g_log_file << ")";
    }
  }
#endif
  
  // :NOTE: Wei 2004-Nov-26:
  //
  // I have to keep the order of the children,
  // so that the emission of LIR codes will be correct.
  void
  DAGNode::replace_child(DAGNode * const orig, ChildInfo const &after)
  {
    assert(orig != 0);
    
    for (std::deque<ChildInfo>::iterator iter = m_children.begin();
         iter != m_children.end();
         ++iter)
    {
      if ((*iter).node() == orig)
      {
        (*iter) = after;
        
        switch (after.attached_type())
        {
        case VAR_ATTACH_HI:
          after.node()->add_ancestor<VAR_ATTACH_HI>(this);
          break;
          
        case VAR_ATTACH_LO:
          after.node()->add_ancestor<VAR_ATTACH_LO>(this);
          break;
          
        case VAR_ATTACH_NORMAL:
          after.node()->add_ancestor<VAR_ATTACH_NORMAL>(this);
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        return;
      }
    }
    
    assert(!"replace_child() should really replace an existing child.");
  }
}

#endif
