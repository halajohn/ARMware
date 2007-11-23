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
#include <iostream>
#include "../../Log.hpp"
#include "../Variable.hpp"
#endif

#include "LIR.hpp"
#include "Register.hpp"
#include "../../Chunk.hpp"
#include "NativeEmitter.hpp"
#include "../MIR.hpp"
#include "../Tuple.hpp"
#include "../Compiler.hpp"
#include "../BasicBlock.hpp"
#include "../BitPosition.hpp"
#include "../../ARMware.hpp"
#include "MachineDep.hpp"
#include "../../Core.hpp"

#include "LIR.tcc"
#include "LIR_private.tcc"
#include "NativeEmitter.tcc"
#include "../BasicBlock.tcc"
#include "../Variable.tcc"

namespace ARMware
{
  ///////////////////////////// Puclib ////////////////////////////////////////
  //========================== Life cycle =====================================
  
  LIR::~LIR()
  {
    std::for_each(m_operand.begin(), m_operand.end(), del_fun<OperandKind>());
    std::for_each(m_dest.begin(), m_dest.end(), del_fun<OperandKind>());
    
    std::for_each(m_memory_use_list.begin(), m_memory_use_list.end(), del_fun<OperandKind>());
    std::for_each(m_memory_def_list.begin(), m_memory_def_list.end(), del_fun<OperandKind>());
  }
  
  //========================== Operation ======================================
  
#if PRINT_CHUNK_CODE
  std::string const LIR::KIND_NAME[] = 
  {
    "PHI",
    "FRAME",
    
    "CALL",
    "PUSH_R",
    "PUSH_I",
    "RETURN",
    
    "MOV_I_2_R",
    "MOV_R_2_R",
    
    "MOV_LABEL_2_R",
    
    "MOV_FROM_M",
    "MOV_FROM_A",
    
    "LOAD_GLOBAL",
    "STORE_GLOBAL",
    
    "ADD_RR",
    "ADD_RI",
    
    "ADC_RR",
    "ADC_RI",
    
    "SUB_RR",
    "SUB_RI",
    
    "SBC_RR",
    "SBC_RI",
    
    "AND_RR",
    "AND_RI",
    
    "OR_RR",
    "OR_RI",
    
    "EOR_RR",
    "EOR_RI",
    
    "MUL32",
    "IMUL32",
    
    "NOT",
    
    "JMP_I",
    "JMP_R",
    
    "JZ",
    "JNZ",
    "JC",
    "JNC",
    "JS",
    "JNS",
    "JO",
    "JNO",
    "JGE",
    "JL",
    "JG",
    "JLE",
    
    "CMP_RR",
    "CMP_RI",
    
    "TST_RR",
    "TST_RI",
    
    "SET_S",
    "SET_Z",
    "SET_C",
    "SET_O",
    
    "LSFT_LEFT_RR",
    "LSFT_LEFT_RI",
    
    "LSFT_RIGHT_RR",
    "LSFT_RIGHT_RI",
    
    "ASFT_RIGHT_RR",
    "ASFT_RIGHT_RI",
    
    "ROR_RR",
    "ROR_RI",
    
    "RRX_RR",
    "RRX_RI",
    
    "PUSHF",
    "POPF",
    
    "LOAD_SP",
    "STORE_SP",
    "ADD_SP",
    
    "BT",
    "CMC"
  };
#endif
  
#if CHECK_CHUNK_CODE
  void
  LIR::check_bb_link_relationship() const
  {
    assert(this == mp_defined_bb->lir_tail());
    
    if (true == is_jmp_type())
    {
      switch (kind())
      {
      case LIR::CALL:
        assert(1 == mp_defined_bb->succ_bb().size());
        assert(true == mp_defined_bb->succ_bb().front().fall_throw());
        break;
        
      case LIR::RETURN:
        assert(0 == mp_defined_bb->succ_bb().size());
        break;
        
      case LIR::JMP_R:
        {
          for (std::vector<BasicBlock::SuccBBInfo>::const_iterator bb_iter = mp_defined_bb->succ_bb().begin();
               bb_iter != mp_defined_bb->succ_bb().end();
               ++bb_iter)
          {
            assert(false == (*bb_iter).fall_throw());
          }
          
          assert(1 == mp_defined_bb->assistant_bb().size());
          
          BasicBlock const * const assistant_bb = mp_defined_bb->assistant_bb().front();
          
          assert(0 == assistant_bb->mir_tail()->next_mir());
          
          for (MIR *curr_mir = assistant_bb->mir_head();
               curr_mir != 0;
               curr_mir = curr_mir->next_mir())
          {
            switch (curr_mir->label<MIR_DATA32>()->kind())
            {
            case Label::BASIC_BLOCK:
              {
                bool find = false;
                
                for (std::vector<BasicBlock::SuccBBInfo>::const_iterator bb_iter = mp_defined_bb->succ_bb().begin();
                     bb_iter != mp_defined_bb->succ_bb().end();
                     ++bb_iter)
                {
                  if ((*bb_iter).bb() == curr_mir->label<MIR_DATA32>()->bb())
                  {
                    find = true;
                  }
                  
                  if (true == find)
                  {
                    break;
                  }
                }
                
                assert(true == find);
              }
              break;
              
            case Label::MIR_INST:
              assert(MIR_DATA32 == curr_mir->kind());
              assert(0 == curr_mir->label<MIR_DATA32>()->mir());
              continue;
              
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
        }
        break;
        
      case LIR::JMP_I:
        assert(1 == mp_defined_bb->succ_bb().size());
        assert(false == mp_defined_bb->succ_bb().front().fall_throw());
        assert(mp_defined_bb->succ_bb().front().bb() == operand().front()->label()->bb());
        break;
        
      case LIR::JZ:
      case LIR::JNZ:
      case LIR::JC:
      case LIR::JNC:
      case LIR::JS:
      case LIR::JNS:
      case LIR::JO:
      case LIR::JNO:
      case LIR::JGE:
      case LIR::JL:
      case LIR::JG:
      case LIR::JLE:
        {
          bool find = false;
          
          assert(2 == mp_defined_bb->succ_bb().size());
          
          for (std::vector<BasicBlock::SuccBBInfo>::const_iterator bb_iter = mp_defined_bb->succ_bb().begin();
               bb_iter != mp_defined_bb->succ_bb().end();
               ++bb_iter)
          {
            if ((*bb_iter).bb() == operand().front()->label()->bb())
            {
              assert(false == (*bb_iter).fall_throw());
            }
            else
            {
              assert(false == find);
              assert(true == (*bb_iter).fall_throw());
              
              find = true;
            }
          }
          
          assert(true == find);
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    else
    {
      assert(1 == mp_defined_bb->succ_bb().size());
      assert(true == mp_defined_bb->succ_bb().front().fall_throw());
    }
  }
#endif
    
  void
  LIR::set_ssa_variable()
  {
    if (m_kind != PHI)
    {
      for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
           iter != m_operand.end();
           ++iter)
      {
        if (OperandKind::VARIABLE == (*iter)->kind())
        {
          set_ssa_variable_internal<USE, NON_MEMORY_VAR>(*iter);
        }
      }
      
      // :NOTE: Wei 2005-May-20:
      //
      // Extends to support SCCVN.
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_use_list.begin();
           iter != m_memory_use_list.end();
           ++iter)
      {
        assert(OperandKind::VARIABLE == (*iter)->kind());
        
        set_ssa_variable_internal<USE, MEMORY_VAR>(*iter);
      }
    }
    
    for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
         iter != m_dest.end();
         ++iter)
    {
      if (OperandKind::VARIABLE == (*iter)->kind())
      {
        set_ssa_variable_internal<DEF, NON_MEMORY_VAR>(*iter);
      }
    }
    
    // :NOTE: Wei 2005-May-20:
    //
    // Extends to support SCCVN.
    for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
         iter != m_memory_def_list.end();
         ++iter)
    {
      assert(OperandKind::VARIABLE == (*iter)->kind());
      
      set_ssa_variable_internal<DEF, MEMORY_VAR>(*iter);
    }
  }
  
  void
  LIR::pop_ssa_var()
  {
    for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
         iter != m_dest.end();
         ++iter)
    {
      if (OperandKind::VARIABLE == (*iter)->kind())
      {
        (*iter)->variable()->pop_ssa_var();
      }
    }
    
    // :NOTE: Wei 2005-May-20:
    //
    // Extends to support SCCVN.
    for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
         iter != m_memory_def_list.end();
         ++iter)
    {
      assert(OperandKind::VARIABLE == (*iter)->kind());
      
      (*iter)->variable()->pop_ssa_var();
    }
  }
  
#if CHECK_CHUNK_CODE
  void
  LIR::check_ssa_property(BasicBlock const * const bb)
  {
    // :NOTE: Wei 2005-Jan-19:
    //
    // Because I will close the checking for a variable when we meet
    // the next ssa variable for the same type,
    // (Ex: I will close the checking for A0 when I meet A1)
    // thus I must put the checking for USE before the DEF in the
    // following codes.
    
    if (m_kind != PHI)
    {
      // :NOTE: Wei 2005-Jan-25:
      //
      // I can't check the USE of variables here.
      // because I maybe meet the USE of variables in the PHI instruction
      // before its definition.
      for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
           iter != m_operand.end();
           ++iter)
      {
        if (OperandKind::VARIABLE == (*iter)->kind())
        {
          (*iter)->variable()->check_ssa_property<USE>(bb);
        }
      }
      
      // :NOTE: Wei 2005-May-20:
      //
      // Extends to support SCCVN.
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_use_list.begin();
           iter != m_memory_use_list.end();
           ++iter)
      {
        assert(OperandKind::VARIABLE == (*iter)->kind());
        
        (*iter)->variable()->check_ssa_property<USE>(bb);
      }
    }
    
    for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
         iter != m_dest.end();
         ++iter)
    {
      if (OperandKind::VARIABLE == (*iter)->kind())
      {
        (*iter)->variable()->check_ssa_property<DEF>(bb);
      }
    }
    
    // :NOTE: Wei 2005-May-20:
    //
    // Extends to support SCCVN.
    for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
         iter != m_memory_def_list.end();
         ++iter)
    {
      assert(OperandKind::VARIABLE == (*iter)->kind());
      
      (*iter)->variable()->check_ssa_property<DEF>(bb);
    }
  }
  
  inline std::vector<OperandKind *> &
  LIR::find_list(Variable const * const var)
  {
    switch (var->kind())
    {
    case Variable::GLOBAL:
    case Variable::TEMP:
      return m_dest;
      
    case Variable::MEMORY:
      return m_memory_def_list;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  void
  LIR::check_var_is_dest(Variable const * const var)
  {
    std::vector<OperandKind *> &list = find_list(var);
    
    for (std::vector<OperandKind *>::const_iterator iter = list.begin();
         iter != list.end();
         ++iter)
    {
      if ((*iter)->variable() == var)
      {
        return;
      }
    }
    
    assert(!"Should not reach here.");
  }
  
  void
  LIR::check_live_interval_preallocated_reg()
  {
    switch (m_kind)
    {
    case LSFT_LEFT_RR:
    case LSFT_RIGHT_RR:
    case ASFT_RIGHT_RR:
    case ROR_RR:
    case RRX_RR:
      assert(Register::ECX ==
             m_operand.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      break;
      
    case MUL32:
    case IMUL32:
      assert(Register::EAX ==
             m_operand.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      
      assert(Register::EDX ==
             m_dest.front()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      
      assert(Register::EAX ==
             m_dest.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      break;
      
    case ADD_RR:
    case ADC_RR:
    case AND_RR:
    case SUB_RR:
    case SBC_RR:
    case OR_RR:
    case EOR_RR:
    case ADD_RI:
    case ADC_RI:
    case AND_RI:
    case SUB_RI:
    case SBC_RI:
    case OR_RI:
    case EOR_RI:
    case LSFT_LEFT_RI:
    case LSFT_RIGHT_RI:
    case ASFT_RIGHT_RI:
    case ROR_RI:
    case RRX_RI:
    case NOT:
    case MOV_I_2_R:
    case MOV_R_2_R:
      
    case MOV_LABEL_2_R:
      
    case MOV_FROM_M:
    case MOV_FROM_A:
      
    case LOAD_GLOBAL:
    case STORE_GLOBAL:
      
    case SET_S:
    case SET_Z:
    case SET_C:
    case SET_O:
    case CALL:
    case CMP_RR:
    case TST_RR:
    case CMP_RI:
    case TST_RI:
    case BT:
    case JMP_R:
    case PUSH_R:
    case LOAD_SP:
    case STORE_SP:
    case ADD_SP:
    case RETURN:
    case POPF:
    case CMC:
    case PUSH_I:
    case PUSHF:
    case JMP_I:
    case JZ:
    case JNZ:
    case JC:
    case JNC:
    case JS:
    case JNS:
    case JO:
    case JNO:
    case JGE:
    case JL:
    case JG:
    case JLE:
    default:
      break;
    }
  }
    
  void
  LIR::check_label_linked() const
  {
    switch (m_kind)
    {
    case CALL:
    case MOV_LABEL_2_R:
    case JMP_I:
    case JZ:
    case JNZ:
    case JC:
    case JNC:
    case JS:
    case JNS:
    case JO:
    case JNO:
    case JGE:
    case JL:
    case JG:
    case JLE:
      assert(OperandKind::LABEL == m_operand.front()->kind());
      assert(true == m_operand.front()->label()->is_finish_linking());
      break;
      
    case PHI:
    case FRAME:
    case ADD_RR:
    case ADC_RR:
    case AND_RR:
    case SUB_RR:
    case SBC_RR:
    case OR_RR:
    case EOR_RR:
    case LSFT_LEFT_RR:
    case LSFT_RIGHT_RR:
    case ASFT_RIGHT_RR:
    case ROR_RR:
    case RRX_RR:
    case ADD_RI:
    case ADC_RI:
    case AND_RI:
    case SUB_RI:
    case SBC_RI:
    case OR_RI:
    case EOR_RI:
    case LSFT_LEFT_RI:
    case LSFT_RIGHT_RI:
    case ASFT_RIGHT_RI:
    case ROR_RI:
    case RRX_RI:
    case NOT:
    case MOV_I_2_R:
    case MOV_R_2_R:
      
    case MOV_FROM_M:
    case MOV_FROM_A:
      
    case LOAD_GLOBAL:
    case STORE_GLOBAL:
      
    case LOAD_SP:
    case SET_S:
    case SET_Z:
    case SET_C:
    case SET_O:
    case MUL32:
    case IMUL32:
    case CMP_RR:
    case TST_RR:
    case CMP_RI:
    case TST_RI:
    case BT:
    case JMP_R:
    case PUSH_R:
    case STORE_SP:
    case RETURN:
    case POPF:
    case CMC:
    case PUSH_I:
    case PUSHF:
    case ADD_SP:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
#endif
    
  void
  LIR::computing_def_use(std::vector<Variable *> &def_var, std::vector<Variable *> &use_var)
  {
    for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
         iter != m_dest.end();
         ++iter)
    {
      if (OperandKind::VARIABLE == (*iter)->kind())
      {
        def_var.push_back((*iter)->variable());
      }
    }
    
    for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
         iter != m_operand.end();
         ++iter)
    {
      if (OperandKind::VARIABLE == (*iter)->kind())
      {
        use_var.push_back((*iter)->variable());
      }
    }
  }
  
  bool
  LIR::is_jmp_type() const
  {
    switch (m_kind)
    {
    case PHI:
    case FRAME:
    case ADD_RR:
    case ADC_RR:
    case AND_RR:
    case SUB_RR:
    case SBC_RR:
    case OR_RR:
    case EOR_RR:
    case LSFT_LEFT_RR:
    case LSFT_RIGHT_RR:
    case ASFT_RIGHT_RR:
    case ROR_RR:
    case RRX_RR:
    case ADD_RI:
    case ADC_RI:
    case AND_RI:
    case SUB_RI:
    case SBC_RI:
    case OR_RI:
    case EOR_RI:
    case LSFT_LEFT_RI:
    case LSFT_RIGHT_RI:
    case ASFT_RIGHT_RI:
    case ROR_RI:
    case RRX_RI:
    case MUL32:
    case NOT:
    case MOV_I_2_R:
    case MOV_R_2_R:
      
    case MOV_LABEL_2_R:
      
    case MOV_FROM_M:
    case MOV_FROM_A:
      
    case LOAD_GLOBAL:
    case STORE_GLOBAL:
      
    case LOAD_SP:
    case STORE_SP:
    case ADD_SP:
    case SET_S:
    case SET_Z:
    case SET_C:
    case SET_O:
    case IMUL32:
    case CMP_RR:
    case TST_RR:
    case CMP_RI:
    case TST_RI:
    case BT:
    case PUSH_R:
    case POPF:
    case CMC:
    case PUSH_I:
    case PUSHF:
      return false;
      
    case CALL:
    case RETURN:
    case JMP_R:
    case JMP_I:
    case JZ:
    case JNZ:
    case JC:
    case JNC:
    case JS:
    case JNS:
    case JO:
    case JNO:
    case JGE:
    case JL:
    case JG:
    case JLE:
      return true;
      
    default:
      assert(!"Should not reach here.");
      return true;
    }
  }
  
  ValueNumberingTypeEnum
  LIR::value_numbering_type() const
  {
    switch (m_kind)
    {
    case LSFT_LEFT_RR:
    case LSFT_RIGHT_RR:
    case ASFT_RIGHT_RR:
    case ROR_RR:
    case RRX_RR:
      
    case LSFT_LEFT_RI:
    case LSFT_RIGHT_RI:
    case ASFT_RIGHT_RI:
    case ROR_RI:
    case RRX_RI:
      
    case SUB_RR:
    case SBC_RR:
      
    case SUB_RI:
    case SBC_RI:
      
    case ADD_RR:
    case ADC_RR:
    case AND_RR:
    case OR_RR:
    case EOR_RR:
      
    case ADD_RI:
    case ADC_RI:
    case AND_RI:
    case OR_RI:
    case EOR_RI:
      
    case MUL32:
    case IMUL32:
      
    case NOT:
    case MOV_R_2_R:
      return SCALAR_OPERATION_PERFORM_SEARCHING;
      
    case MOV_I_2_R:
    case SET_S:
    case SET_Z:
    case SET_C:
    case SET_O:
    case MOV_FROM_A:
    case LOAD_SP:
    case MOV_LABEL_2_R:
    case MOV_FROM_M:
      return SCALAR_OPERATION_NO_PERFORM_SEARCHING;
      
    case FRAME:
      return MEMORY_OPERATION_FRAME;
      
    case CALL:
      return MEMORY_OPERATION_JSR;
      
    case LOAD_GLOBAL:
      return MEMORY_OPERATION_LOAD;
      
    case STORE_GLOBAL:
      return MEMORY_OPERATION_STORE;
      
    case STORE_SP:
      
    case PUSHF:
    case POPF:
      
    case PUSH_R:
    case PUSH_I:
      
    case ADD_SP:
      
    case JMP_R:
    case JMP_I:
    case JZ:
    case JNZ:
    case JC:
    case JNC:
    case JS:
    case JNS:
    case JO:
    case JNO:
    case JGE:
    case JL:
    case JG:
    case JLE:
      
    case CMP_RR:
    case TST_RR:
    case CMP_RI:
    case TST_RI:
      
    case BT:
    case CMC:
      
    case RETURN:
      return NOT_INSIDE_VALUE_NUMBERING;
      
    default:
      assert(!"Should not reach here.");
      return NOT_INSIDE_VALUE_NUMBERING;
    }
  }
  
  void
  LIR::join_live_interval()
  {
    switch (m_kind)
    {
    case PHI:
      // :NOTE: Wei 2005-Feb-26:
      //
      // The live interval of all the phi operands and the definition should join together.
#if CHECK_CHUNK_CODE
      {
        assert(1 == m_dest.size());
        
        Variable const * const join = m_dest.front()->variable()->join_var();
        
        for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
             iter != m_operand.end();
             ++iter)
        {
          assert((*iter)->variable()->join_var() == join);
        }
      }
#endif
      break;
      
    case ADD_RR:
    case ADC_RR:
    case AND_RR:
    case SUB_RR:
    case SBC_RR:
    case OR_RR:
    case EOR_RR:
    case LSFT_LEFT_RR:
    case LSFT_RIGHT_RR:
    case ASFT_RIGHT_RR:
    case ROR_RR:
    case RRX_RR:
      // :NOTE: Wei 2005-May-13:
      //
      // If I met this kind of codes:
      //
      // ADD R1-0, R2, R1-1
      //
      // I want to join the live interval of R1-0 and R1-1,
      // not R1-0 and R2.
      {
        Variable const * const dest_var = m_dest.front()->variable();
        LiveInterval * const dest_live = dest_var->join_var()->live_interval();
        
        if (dest_live != 0)
        {
          // :NOTE: Wei 2005-Feb-28:
          //
          // If dest_live == 0, this means this LIR has no use, I should not emit the native code for this LIR,
          // thus I don't need to join the live intervals of this LIR.
          
          Variable const *src_var = m_operand.front()->variable();
          
          for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
               iter != m_operand.end();
               ++iter)
          {
            if ((*iter)->variable()->original_ssa_var() == dest_var->original_ssa_var())
            {
              src_var = (*iter)->variable();
              
              break;
            }
          }
          
          gp_chunk->join_live_interval(dest_live,
                                       src_var->join_var()->live_interval());
        }
      }
      break;
      
    case MOV_R_2_R:
    case ADD_RI:
    case ADC_RI:
    case AND_RI:
    case SUB_RI:
    case SBC_RI:
    case OR_RI:
    case EOR_RI:
    case LSFT_LEFT_RI:
    case LSFT_RIGHT_RI:
    case ASFT_RIGHT_RI:
    case ROR_RI:
    case RRX_RI:
    case NOT:
      {
        LiveInterval * const live = m_dest.front()->variable()->join_var()->live_interval();
        
        if (live != 0)
        {
          // :NOTE: Wei 2005-Feb-28:
          //
          // If live == 0, this means this LIR has no use, I should not emit the native code for this LIR,
          // thus I don't need to join the live intervals of this LIR.
          gp_chunk->join_live_interval(live,
                                       m_operand.front()->variable()->join_var()->live_interval());
        }
      }
      break;
      
    case FRAME:
      
    case MUL32:
    case IMUL32:
      
    case MOV_I_2_R:
    case MOV_LABEL_2_R:
      
    case MOV_FROM_M:
    case MOV_FROM_A:
      
    case LOAD_GLOBAL:
    case STORE_GLOBAL:
      
    case LOAD_SP:
    case STORE_SP:
    case ADD_SP:
      
    case SET_S:
    case SET_Z:
    case SET_C:
    case SET_O:
      
    case CMP_RR:
    case TST_RR:
    case CMP_RI:
    case TST_RI:
      
    case CALL:
    case RETURN:
      
    case BT:
    case CMC:
      
    case PUSH_R:
    case PUSH_I:
      
    case PUSHF:
    case POPF:
      
    case JMP_R:
    case JMP_I:
      
    case JZ:
    case JNZ:
    case JC:
    case JNC:
    case JS:
    case JNS:
    case JO:
    case JNO:
    case JGE:
    case JL:
    case JG:
    case JLE:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  void
  LIR::determine_AVAIL_AVLOC(BasicBlock * const bb)
  {
    assert(m_kind != PHI);
    
    switch (value_numbering_type())
    {
    case SCALAR_OPERATION_PERFORM_SEARCHING:
    case SCALAR_OPERATION_NO_PERFORM_SEARCHING:
      for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
           iter != m_dest.end();
           ++iter)
      {
        assert(OperandKind::VARIABLE == (*iter)->kind());
        
        bb->set_AVAIL_AVLOC((*iter)->variable()->scc_value_number());
      }
      break;
      
    case MEMORY_OPERATION_LOAD:
      assert(OperandKind::VARIABLE == m_dest.front()->kind());
      
      bb->set_AVAIL_AVLOC(m_dest.front()->variable()->scc_value_number());
      
    case MEMORY_OPERATION_FRAME:
    case MEMORY_OPERATION_JSR:
    case MEMORY_OPERATION_STORE:
      bb->set_AVAIL_AVLOC(m_memory_operation_idx);
      break;
      
    case NOT_INSIDE_VALUE_NUMBERING:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  void
  LIR::emit_native_code(std::vector<uint8_t> &native_code_buffer)
  {
    switch (m_kind)
    {
    case FRAME:
      break;
      
    case ADD_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RR<NativeOperTraits_ADD, true>(native_code_buffer,
                                                                m_operand.front()->variable(),
                                                                m_operand.back()->variable(),
                                                                m_dest.front()->variable());
      break;
      
    case ADC_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RR<NativeOperTraits_ADC, true>(native_code_buffer,
                                                                m_operand.front()->variable(),
                                                                m_operand.back()->variable(),
                                                                m_dest.front()->variable());
      break;
      
    case AND_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RR<NativeOperTraits_AND, true>(native_code_buffer,
                                                                m_operand.front()->variable(),
                                                                m_operand.back()->variable(),
                                                                m_dest.front()->variable());
      break;
      
    case OR_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RR<NativeOperTraits_OR, true>(native_code_buffer,
                                                               m_operand.front()->variable(),
                                                               m_operand.back()->variable(),
                                                               m_dest.front()->variable());
      break;
      
    case EOR_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
        
      emit_two_operand_operation_RR<NativeOperTraits_EOR, true>(native_code_buffer,
                                                                m_operand.front()->variable(),
                                                                m_operand.back()->variable(),
                                                                m_dest.front()->variable());
      break;
      
    case SUB_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RR<NativeOperTraits_SUB, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->variable(),
                                                                 m_dest.front()->variable());
      break;
      
    case SBC_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RR<NativeOperTraits_SBC, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->variable(),
                                                                 m_dest.front()->variable());
      break;
      
    case LSFT_LEFT_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
        
      assert(Register::ECX ==
             m_operand.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      
      emit_shift_operation_RR<NativeOperTraits_LSFT_LEFT>(native_code_buffer,
                                                          m_operand.front()->variable(),
                                                          m_dest.front()->variable());
      break;
      
    case LSFT_RIGHT_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      assert(Register::ECX ==
             m_operand.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      
      emit_shift_operation_RR<NativeOperTraits_LSFT_RIGHT>(native_code_buffer,
                                                           m_operand.front()->variable(),
                                                           m_dest.front()->variable());
      break;
      
    case ASFT_RIGHT_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      assert(Register::ECX ==
             m_operand.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      
      emit_shift_operation_RR<NativeOperTraits_ASFT_RIGHT>(native_code_buffer,
                                                           m_operand.front()->variable(),
                                                           m_dest.front()->variable());
      break;
      
    case ROR_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      assert(Register::ECX ==
             m_operand.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      
      emit_shift_operation_RR<NativeOperTraits_ROR>(native_code_buffer,
                                                    m_operand.front()->variable(),
                                                    m_dest.front()->variable());
      break;
      
    case RRX_RR:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      assert(Register::ECX ==
             m_operand.back()->variable()->join_var()->live_interval()->preallocated_reg()->kind());
      
      emit_shift_operation_RR<NativeOperTraits_RRX>(native_code_buffer,
                                                    m_operand.front()->variable(),
                                                    m_dest.front()->variable());
      break;
      
    case CMP_RR:
      assert(0 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_without_dest_RR<NativeOperTraits_CMP>(native_code_buffer,
                                                                       m_operand.front()->variable(),
                                                                       m_operand.back()->variable());
      break;
      
    case TST_RR:
      assert(0 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_without_dest_RR<NativeOperTraits_TST>(native_code_buffer,
                                                                       m_operand.front()->variable(),
                                                                       m_operand.back()->variable());
      break;
      
    case ADD_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_ADD, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->imm(),
                                                                 m_dest.front()->variable());
      break;
      
    case ADC_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_ADC, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->imm(),
                                                                 m_dest.front()->variable());
      break;
      
    case AND_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_AND, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->imm(),
                                                                 m_dest.front()->variable());
      break;
      
    case SUB_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_SUB, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->imm(),
                                                                 m_dest.front()->variable());
      break;
      
    case SBC_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_SBC, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->imm(),
                                                                 m_dest.front()->variable());
      break;
      
    case OR_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_OR, false>(native_code_buffer,
                                                                m_operand.front()->variable(),
                                                                m_operand.back()->imm(),
                                                                m_dest.front()->variable());
      break;
      
    case EOR_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_EOR, false>(native_code_buffer,
                                                                 m_operand.front()->variable(),
                                                                 m_operand.back()->imm(),
                                                                 m_dest.front()->variable());
      break;
      
    case LSFT_LEFT_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_LSFT_LEFT, true>(native_code_buffer,
                                                                      m_operand.front()->variable(),
                                                                      m_operand.back()->imm(),
                                                                      m_dest.front()->variable());
      break;
      
    case LSFT_RIGHT_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_LSFT_RIGHT, true>(native_code_buffer,
                                                                       m_operand.front()->variable(),
                                                                       m_operand.back()->imm(),
                                                                       m_dest.front()->variable());
      break;
      
    case ASFT_RIGHT_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_ASFT_RIGHT, true>(native_code_buffer,
                                                                       m_operand.front()->variable(),
                                                                       m_operand.back()->imm(),
                                                                       m_dest.front()->variable());
      break;
      
    case ROR_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_ROR, true>(native_code_buffer,
                                                                m_operand.front()->variable(),
                                                                m_operand.back()->imm(),
                                                                m_dest.front()->variable());
      break;
      
    case RRX_RI:
      assert(1 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_RI<NativeOperTraits_RRX, true>(native_code_buffer,
                                                                m_operand.front()->variable(),
                                                                m_operand.back()->imm(),
                                                                m_dest.front()->variable());
      break;
      
    case CMP_RI:
      assert(0 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_without_dest_RI<NativeOperTraits_CMP>(native_code_buffer,
                                                                       m_operand.front()->variable(),
                                                                       m_operand.back()->imm());
      break;
      
    case TST_RI:
      assert(0 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_two_operand_operation_without_dest_RI<NativeOperTraits_TST>(native_code_buffer,
                                                                       m_operand.front()->variable(),
                                                                       m_operand.back()->imm());
      break;
      
    case MOV_R_2_R:
      {
        assert(1 == m_dest.size());
        assert(1 == m_operand.size());
        
        Variable const * const first_operand = m_operand.front()->variable();
        Variable const * const dest = m_dest.front()->variable();
        
        assert(first_operand->join_var()->live_interval() != 0);
        
        Register const * const first_operand_reg = first_operand->join_var()->live_interval()->preallocated_reg();
        
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
          
          if (first_operand_reg != 0)
          {
            if (first_operand_reg->kind() == dest_reg->kind())
            {
              return;
            }
            else
            {
              mov_r_r(native_code_buffer,
                      first_operand_reg->native_represent(),
                      dest_reg->native_represent());
            }
          }
          else
          {
            switch (first_operand->kind())
            {
            case Variable::GLOBAL:
              mov_global_r(native_code_buffer,
                           first_operand->mem_loc_var()->memory_addr(),
                           dest_reg->native_represent());
              break;
              
            case Variable::TEMP:
              mov_stack_r(native_code_buffer,
                          first_operand->mem_loc_var()->stack_offset(),
                          dest_reg->native_represent());
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
        }
        else
        {
          if (first_operand_reg != 0)
          {
            switch (dest->kind())
            {
            case Variable::GLOBAL:
              mov_r_global(native_code_buffer,
                           first_operand_reg->native_represent(),
                           dest->mem_loc_var()->memory_addr());
              break;
                
            case Variable::TEMP:
              mov_r_stack(native_code_buffer,
                          first_operand_reg->native_represent(),
                          dest->mem_loc_var()->stack_offset());
              break;
                
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          else
          {
            // :NOTE: Wei 2005-Mar-15:
            //
            // Move from memory to memory.
            mov_memory_memory(native_code_buffer, first_operand, dest);
          }
        }
      }
      break;
      
    case MOV_I_2_R:
      {
        assert(1 == m_dest.size());
        assert(1 == m_operand.size());
        
        Variable const * const dest = m_dest.front()->variable();
          
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
            
          mov_imm_r(native_code_buffer,
                    m_operand.front()->imm(),
                    dest_reg->native_represent());
        }
        else
        {
          switch (dest->kind())
          {
          case Variable::GLOBAL:
            mov_imm_global(native_code_buffer,
                           m_operand.front()->imm(),
                           dest->mem_loc_var()->memory_addr());
            break;
              
          case Variable::TEMP:
            mov_imm_stack(native_code_buffer,
                          m_operand.front()->imm(),
                          dest->mem_loc_var()->stack_offset());
            break;
              
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
      break;
      
    case MOV_FROM_A:
      {
        assert(1 == m_dest.size());
        
        Variable const * const dest = m_dest.front()->variable();
        
        // :NOTE: Wei 2005-Mar-14:
        //
        // I think the value I move from EAX register has to be used by some later instruction.
        assert(dest->join_var()->live_interval() != 0);
        
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
          
          if (Register::EAX == dest_reg->kind())
          {
            return;
          }
          else
          {
            mov_r_r(native_code_buffer,
                    Register::EAX_NATIVE_REP,
                    dest_reg->native_represent());
          }
        }
        else
        {
          switch (dest->kind())
          {
          case Variable::GLOBAL:
            mov_r_global(native_code_buffer,
                         Register::EAX_NATIVE_REP,
                         dest->mem_loc_var()->memory_addr());
            break;
            
          case Variable::TEMP:
            mov_r_stack(native_code_buffer,
                        Register::EAX_NATIVE_REP,
                        dest->mem_loc_var()->stack_offset());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
      break;
      
    case MOV_LABEL_2_R:
      {
        assert(1 == m_dest.size());
        assert(1 == m_operand.size());
        
        Variable const * const dest = m_dest.front()->variable();
        
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
          
          mov_label_r(native_code_buffer,
                      m_operand.front()->label(),
                      dest_reg->native_represent());
        }
        else
        {
          // :NOTE: Wei 2005-Mar-15:
          //
          // Move imm to memory.
          switch (dest->kind())
          {
          case Variable::GLOBAL:
            mov_label_global(native_code_buffer,
                             m_operand.front()->label(),
                             dest->mem_loc_var()->memory_addr());
            break;
            
          case Variable::TEMP:
            mov_label_stack(native_code_buffer,
                            m_operand.front()->label(),
                            dest->mem_loc_var()->stack_offset());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
      break;
      
    case LOAD_GLOBAL:
      {
        assert(1 == m_dest.size());
        assert(0 == m_operand.size());
        
        Variable const * const dest = m_dest.front()->variable();
        
        // :NOTE: Wei 2005-May-13:
        //
        // If the destination variable of LOAD_GLOBAL isn't allocated to a register,
        // then I don't need to emit the native codes for LOAD_GLOBAL.
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
          
          assert(dest->mem_loc_var() == dest);
          
          mov_global_r(native_code_buffer,
                       dest->memory_addr(),
                       dest_reg->native_represent());
        }
      }
      break;
      
    case STORE_GLOBAL:
      {
        Variable const * const var = m_operand.front()->variable();
        
        assert(Variable::GLOBAL == var->kind());
        assert(var->join_var()->live_interval() != 0);
        
        Register const * const reg = var->join_var()->live_interval()->preallocated_reg();
        
        if (reg != 0)
        {
          assert(var->mem_loc_var() == var);
          
          // :NOTE: Wei 2005-Mar-16:
          //
          // This global variable is in a register, so I spill it out.
          mov_r_global(native_code_buffer,
                       reg->native_represent(),
                       var->memory_addr());
        }
      }
      break;
      
    case MOV_FROM_M:
      assert(1 == m_dest.size());
      assert(1 == m_operand.size());
      
      {
        Variable const * const dest = m_dest.front()->variable();
        Variable const * const operand = m_operand.front()->variable();
        
        assert(operand->join_var()->live_interval() != 0);
        
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
          
          if (operand->join_var()->live_interval()->preallocated_reg() != 0)
          {
            mov_from_memory_r_r(native_code_buffer,
                                dest_reg->native_represent(),
                                operand->join_var()->live_interval()->preallocated_reg()->native_represent());
          }
          else
          {
            // :NOTE: Wei 2005-Mar-15:
            //
            // Move from memory to register.
            Register::RegNativeRep const scratch_reg =
              Register::convert_reg_kind_to_its_native_rep(
                Register::get_one_scratch_reg(0, dest_reg->kind()));
            
            if (scratch_reg != dest_reg->native_represent())
            {
              mov_r_stack(native_code_buffer,
                          scratch_reg,
                          gp_chunk->scratch_reg_stack_offset());
            }
            
            switch (operand->kind())
            {
            case Variable::GLOBAL:
              mov_global_r(native_code_buffer,
                           operand->mem_loc_var()->memory_addr(),
                           scratch_reg);
              break;
              
            case Variable::TEMP:
              mov_stack_r(native_code_buffer,
                          operand->mem_loc_var()->stack_offset(),
                          scratch_reg);
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
            
            mov_from_memory_r_r(native_code_buffer,
                                dest_reg->native_represent(),
                                scratch_reg);
            
            if (scratch_reg != dest_reg->native_represent())
            {
              mov_stack_r(native_code_buffer,
                          gp_chunk->scratch_reg_stack_offset(),
                          scratch_reg);
            }
          }
        }
        else
        {
          // :NOTE: Wei 2005-Mar-15:
          //
          // Move from memory to memory.
          if (operand->join_var()->live_interval()->preallocated_reg() != 0)
          {
            Register const * const operand_reg =
              operand->join_var()->live_interval()->preallocated_reg();
            
            Register::RegNativeRep const scratch_reg =
              Register::convert_reg_kind_to_its_native_rep(
                Register::get_one_scratch_reg(operand_reg->kind()));
            
            mov_r_stack(native_code_buffer,
                        scratch_reg,
                        gp_chunk->scratch_reg_stack_offset());
            
            mov_from_memory_r_r(native_code_buffer,
                                scratch_reg,
                                operand_reg->native_represent());
            
            switch (dest->kind())
            {
            case Variable::GLOBAL:
              mov_r_global(native_code_buffer,
                           scratch_reg,
                           dest->mem_loc_var()->memory_addr());
              break;
              
            case Variable::TEMP:
              mov_r_stack(native_code_buffer,
                          scratch_reg,
                          dest->mem_loc_var()->stack_offset());
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
            
            mov_stack_r(native_code_buffer,
                        gp_chunk->scratch_reg_stack_offset(),
                        scratch_reg);
          }
          else
          {
            Register::RegNativeRep const scratch_reg =
              Register::convert_reg_kind_to_its_native_rep(
                Register::get_one_scratch_reg());
            
            mov_r_stack(native_code_buffer,
                        scratch_reg,
                        gp_chunk->scratch_reg_stack_offset());
            
            switch (operand->kind())
            {
            case Variable::GLOBAL:
              mov_global_r(native_code_buffer,
                           operand->mem_loc_var()->memory_addr(),
                           scratch_reg);
              break;
              
            case Variable::TEMP:
              mov_stack_r(native_code_buffer,
                          operand->mem_loc_var()->stack_offset(),
                          scratch_reg);
              break;
                
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
            
            mov_from_memory_r_r(native_code_buffer,
                                scratch_reg,
                                scratch_reg);
            
            switch (dest->kind())
            {
            case Variable::GLOBAL:
              mov_r_global(native_code_buffer,
                           scratch_reg,
                           dest->mem_loc_var()->memory_addr());
              break;
              
            case Variable::TEMP:
              mov_r_stack(native_code_buffer,
                          scratch_reg,
                          dest->mem_loc_var()->stack_offset());
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
            
            mov_stack_r(native_code_buffer,
                        gp_chunk->scratch_reg_stack_offset(),
                        scratch_reg);
          }
        }
      }
      break;
      
    case NOT:
      assert(1 == m_dest.size());
      assert(1 == m_operand.size());
      
      {
        Variable const * const operand = m_operand.front()->variable();
        Variable const * const dest = m_dest.front()->variable();
        
        assert(operand->join_var()->live_interval() != 0);
        
        Register const * const operand_reg = operand->join_var()->live_interval()->preallocated_reg();
        
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
          
          if (operand_reg != 0)
          {
            // :NOTE: Wei 2005-Mar-15:
            //
            // Register -> Register
            if (operand_reg->kind() != dest_reg->kind())
            {
              mov_r_r(native_code_buffer,
                      operand_reg->native_represent(),
                      dest_reg->native_represent());
            }
          }
          else
          {
            switch (operand->kind())
            {
            case Variable::GLOBAL:
              mov_global_r(native_code_buffer,
                           operand->mem_loc_var()->memory_addr(),
                           dest_reg->native_represent());
              break;
              
            case Variable::TEMP:
              mov_stack_r(native_code_buffer,
                          operand->mem_loc_var()->stack_offset(),
                          dest_reg->native_represent());
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          
          not_r(native_code_buffer, dest_reg->native_represent());
        }
        else
        {
          if (operand_reg != 0)
          {
            // :NOTE: Wei 2005-Mar-15:
            //
            // Register -> Memory
            switch (dest->kind())
            {
            case Variable::GLOBAL:
              mov_r_global(native_code_buffer,
                           operand_reg->native_represent(),
                           dest->mem_loc_var()->memory_addr());
              
              not_global(native_code_buffer, dest->mem_loc_var()->memory_addr());
              break;
              
            case Variable::TEMP:
              mov_r_stack(native_code_buffer,
                          operand_reg->native_represent(),
                          dest->mem_loc_var()->stack_offset());
              
              not_stack(native_code_buffer, dest->mem_loc_var()->stack_offset());
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          else
          {
            mov_memory_memory(native_code_buffer, operand, dest);
            
            switch (dest->kind())
            {
            case Variable::GLOBAL:
              not_global(native_code_buffer, dest->mem_loc_var()->memory_addr());
              break;
              
            case Variable::TEMP:
              not_stack(native_code_buffer, dest->mem_loc_var()->stack_offset());
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
        }
      }
      break;
      
    case MUL32:
      assert(2 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_mul_operation<NativeOperTraits_MUL32>(native_code_buffer,
#ifndef NDEBUG
                                                 m_dest.front()->variable(),
                                                 m_dest.back()->variable(),
#endif
                                                 m_operand.front()->variable()
#ifndef NDEBUG
                                                 , m_operand.back()->variable()
#endif
                                                 );
      break;
      
    case IMUL32:
      assert(2 == m_dest.size());
      assert(2 == m_operand.size());
      
      emit_mul_operation<NativeOperTraits_IMUL32>(native_code_buffer,
#ifndef NDEBUG
                                                  m_dest.front()->variable(),
                                                  m_dest.back()->variable(),
#endif
                                                  m_operand.front()->variable()
#ifndef NDEBUG
                                                  , m_operand.back()->variable()
#endif
                                                  );
      break;
      
    case LOAD_SP:
      assert(1 == m_dest.size());
      assert(0 == m_operand.size());
      
      {
        Variable const * const dest = m_dest.front()->variable();
        
        if (dest->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
          
          mov_from_memory_sp_r(native_code_buffer,
                               dest_reg->native_represent());
        }
        else
        {
          Register::RegNativeRep const scratch_reg =
            Register::convert_reg_kind_to_its_native_rep(
              Register::get_one_scratch_reg());
          
          mov_r_stack(native_code_buffer,
                      scratch_reg,
                      gp_chunk->scratch_reg_stack_offset());
          
          mov_from_memory_sp_r(native_code_buffer,
                               scratch_reg);
          
          switch (dest->kind())
          {
          case Variable::GLOBAL:
            mov_r_global(native_code_buffer,
                         scratch_reg,
                         dest->mem_loc_var()->memory_addr());
            break;
            
          case Variable::TEMP:
            mov_r_stack(native_code_buffer,
                        scratch_reg,
                        dest->mem_loc_var()->stack_offset());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          mov_stack_r(native_code_buffer,
                      gp_chunk->scratch_reg_stack_offset(),
                      scratch_reg);
        }
      }
      break;
      
    case STORE_SP:
      assert(0 == m_dest.size());
      assert(1 == m_operand.size());
      
      {
        Variable const * const operand = m_operand.front()->variable();
        
        if (operand->join_var()->live_interval()->preallocated_reg() != 0)
        {
          Register const * const operand_reg = operand->join_var()->live_interval()->preallocated_reg();
          
          mov_to_memory_r_sp(native_code_buffer,
                             operand_reg->native_represent());
        }
        else
        {
          Register::RegNativeRep const scratch_reg =
            Register::convert_reg_kind_to_its_native_rep(
              Register::get_one_scratch_reg());
          
          mov_r_stack(native_code_buffer,
                      scratch_reg,
                      gp_chunk->scratch_reg_stack_offset());
          
          switch (operand->kind())
          {
          case Variable::GLOBAL:
            mov_global_r(native_code_buffer,
                         operand->mem_loc_var()->memory_addr(),
                         scratch_reg);
            break;
            
          case Variable::TEMP:
            mov_stack_r(native_code_buffer,
                        operand->mem_loc_var()->stack_offset(),
                        scratch_reg);
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          mov_to_memory_r_sp(native_code_buffer,
                             scratch_reg);
          
          mov_stack_r(native_code_buffer,
                      gp_chunk->scratch_reg_stack_offset(),
                      scratch_reg);
        }
      }
      break;
      
    case ADD_SP:
      two_operand_operation_r_imm<false>(native_code_buffer,
                                         NativeOperTraits_ADD::I_MR_to_MR,
                                         NativeOperTraits_ADD::I_MR_to_MR_OPCODE_2,
                                         m_operand.front()->imm(),
                                         Register::ESP_NATIVE_REP);
      break;
      
    case SET_S:
      assert(0 == m_operand.size());
      assert(1 == m_dest.size());
      
      emit_set_byte_operation<NativeOperTraits_SETS>(native_code_buffer, m_dest.front()->variable());
      break;
      
    case SET_Z:
      assert(0 == m_operand.size());
      assert(1 == m_dest.size());
      
      emit_set_byte_operation<NativeOperTraits_SETZ>(native_code_buffer, m_dest.front()->variable());
      break;
      
    case SET_C:
      assert(0 == m_operand.size());
      assert(1 == m_dest.size());
      
      emit_set_byte_operation<NativeOperTraits_SETC>(native_code_buffer, m_dest.front()->variable());
      break;
      
    case SET_O:
      assert(0 == m_operand.size());
      assert(1 == m_dest.size());
      
      emit_set_byte_operation<NativeOperTraits_SETO>(native_code_buffer, m_dest.front()->variable());
      break;
      
    case BT:
      {
        native_code_buffer.push_back(0x0F);
        native_code_buffer.push_back(0xBA);
        
        Variable const * const operand = m_operand.front()->variable();
        
        assert(operand->join_var()->live_interval() != 0);
        
        Register const * const operand_reg = operand->join_var()->live_interval()->preallocated_reg();
        
        if (operand_reg != 0)
        {
          addressing_mode_r_r(native_code_buffer, 0x04, operand_reg->native_represent());
          
          assert(OperandKind::IMM == m_operand.back()->kind());
          assert(m_operand.back()->imm() <= 31);
          
          native_code_buffer.push_back(static_cast<uint8_t>(m_operand.back()->imm()));
        }
        else
        {
          switch (operand->kind())
          {
          case Variable::GLOBAL:
            addressing_mode_bit_test_global(native_code_buffer,
                                            operand->mem_loc_var()->memory_addr(),
                                            m_operand.back()->imm());
            break;
            
          case Variable::TEMP:
            addressing_mode_bit_test_stack(native_code_buffer,
                                           operand->mem_loc_var()->stack_offset(),
                                           m_operand.back()->imm());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
      break;
      
    case PUSH_R:
      {
        native_code_buffer.push_back(0xFF);
        
        Variable const * const operand = m_operand.front()->variable();
        
        assert(operand->join_var()->live_interval() != 0);
        
        Register const * const operand_reg = operand->join_var()->live_interval()->preallocated_reg();
        
        if (operand_reg != 0)
        {
          addressing_mode_r_r(native_code_buffer, 0x06, operand_reg->native_represent());
        }
        else
        {
          switch (operand->kind())
          {
          case Variable::GLOBAL:
            addressing_mode_global_r(native_code_buffer, operand->mem_loc_var()->memory_addr(), 0x06);
            break;
            
          case Variable::TEMP:
            addressing_mode_stack_r(native_code_buffer, operand->mem_loc_var()->stack_offset(), 0x06);
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
      break;
      
    case PUSH_I:
      {
        native_code_buffer.push_back(0x68);
        
        uint32_t const imm = m_operand.front()->imm();
        
        native_code_buffer.push_back(static_cast<uint8_t>(imm));
        native_code_buffer.push_back(static_cast<uint8_t>(imm >> 8));
        native_code_buffer.push_back(static_cast<uint8_t>(imm >> 16));
        native_code_buffer.push_back(static_cast<uint8_t>(imm >> 24));
      }
      break;
      
    case PUSHF:
      native_code_buffer.push_back(0x9C);
      break;
      
    case POPF:
      native_code_buffer.push_back(0x9D);
      break;
      
    case CMC:
      native_code_buffer.push_back(0xF5);
      break;
      
    case CALL:
      {
        Label * const function_label = m_operand.front()->label();
        
        native_code_buffer.push_back(0xE8);
        
        uint32_t const offset = native_code_buffer.size();
        
        native_code_buffer.push_back(0);
        native_code_buffer.push_back(0);
        native_code_buffer.push_back(0);
        native_code_buffer.push_back(0);
        
        assert(Label::ROUTINE == function_label->kind());
        
        function_label->add_back_patch_info(Label::RELATIVE_TO_NEXT, offset);
      }
      break;
      
    case RETURN:      
      // :NOTE: Wei 2005-Mar-19:
      //
      // Because I am about to return to the interpreter,
      // thus I need to emit the epilogue codes here.
      emit_native_epilogue_codes(native_code_buffer);
      
      native_code_buffer.push_back(0xC3);
      break;
      
    case JMP_R:
      {
        native_code_buffer.push_back(0xFF);
        
        Variable const * const operand = m_operand.front()->variable();
        
        assert(operand->join_var()->live_interval() != 0);
        
        Register const * const operand_reg = operand->join_var()->live_interval()->preallocated_reg();
        
        if (operand_reg != 0)
        {
          addressing_mode_r_r(native_code_buffer, 0x04, operand_reg->native_represent());
        }
        else
        {
          switch (operand->kind())
          {
          case Variable::GLOBAL:
            addressing_mode_global_r(native_code_buffer, operand->mem_loc_var()->memory_addr(), 0x04);
            break;
            
          case Variable::TEMP:
            addressing_mode_stack_r(native_code_buffer, operand->mem_loc_var()->stack_offset(), 0x04);
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
      break;
      
    case JMP_I:
      jmp_to_label(native_code_buffer, m_operand.front()->label());
      break;
      
    case JZ:
      jmp(native_code_buffer, 0x84, m_operand.front()->label());
      break;
      
    case JNZ:
      jmp(native_code_buffer, 0x85, m_operand.front()->label());
      break;
      
    case JC:
      jmp(native_code_buffer, 0x82, m_operand.front()->label());
      break;
      
    case JNC:
      jmp(native_code_buffer, 0x83, m_operand.front()->label());
      break;
      
    case JS:
      jmp(native_code_buffer, 0x88, m_operand.front()->label());
      break;
      
    case JNS:
      jmp(native_code_buffer, 0x89, m_operand.front()->label());
      break;
      
    case JO:
      jmp(native_code_buffer, 0x80, m_operand.front()->label());
      break;
      
    case JNO:
      jmp(native_code_buffer, 0x81, m_operand.front()->label());
      break;
      
    case JGE:
      jmp(native_code_buffer, 0x8D, m_operand.front()->label());
      break;
      
    case JL:
      jmp(native_code_buffer, 0x8C, m_operand.front()->label());
      break;
      
    case JG:
      jmp(native_code_buffer, 0x8F, m_operand.front()->label());
      break;
      
    case JLE:
      jmp(native_code_buffer, 0x8E, m_operand.front()->label());
      break;
      
    case PHI:
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}

#endif
