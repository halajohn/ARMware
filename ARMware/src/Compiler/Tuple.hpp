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

#ifndef Tuple_hpp
#define Tuple_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include "../MemoryPool2.hpp"
#include "x86/LIR.hpp"

namespace ARMware
{
  typedef struct ValueNumber ValueNumber;
  class ValueNumber
  {
  public:
    
    enum KindEnum
    {
      VAR,
      CONST
    };
    typedef enum KindEnum KindEnum;
    
  private:
    
    KindEnum m_kind;
    
    union
    {
      Variable *mp_var;
      uint32_t m_value;
    } u;
    
  public:
    
    // Life cycle
    
    // :NOTE: Wei 2005-May-20:
    //
    // The following 2 constructors are so called "conversion constructor".
    inline
    ValueNumber(Variable * const var)
      : m_kind(VAR)
    {
      assert(var != 0);
      
      u.mp_var = var;
    }
    
    inline
    ValueNumber(uint32_t const value)
      : m_kind(CONST)
    {
      u.m_value = value;
    }
    
    // Inquiry
    
    inline KindEnum
    kind() const
    { return m_kind; }
    
    inline Variable *
    var() const
    {
      assert(VAR == m_kind);
      
      return u.mp_var;
    }
    
    inline uint32_t
    value() const
    {
      assert(CONST == m_kind);
      
      return u.m_value;
    }
    
    // Access
    
    inline void
    set_var(Variable * const var)
    {
      assert(var != 0);
      assert(VAR == m_kind);
      
      u.mp_var = var;
    }
    
    // Operator
    
    inline bool
    operator==(ValueNumber const &value_number) const
    {
      if (m_kind == value_number.kind())
      {
        switch (m_kind)
        {
        case VAR:
          if (u.mp_var == value_number.var())
          {
            return true;
          }
          break;
          
        case CONST:
          if (u.m_value == value_number.value())
          {
            return true;
          }
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      
      return false;
    }
    
    inline bool
    operator!=(ValueNumber const &value_number) const
    {
      return !operator==(value_number);
    }
  };
  
  typedef class Tuple Tuple;
  class Tuple : public MemoryPool2<Tuple, 256>
  {
  private:
    
    LIR::KindEnum m_kind;
    
    std::vector<ValueNumber> m_operands_value_number;
    std::vector<Variable *> m_results_value_number;
    
    uint32_t m_memory_operation_idx;
    std::vector<LIR *> m_equivalent_operation;
    
    friend class MemoryPool2<Tuple, 256>;
    
  public:
    
    // Life cycle
    
    inline
    Tuple(LIR const * const lir)
    {
      switch (lir->kind())
      {
      case LIR::LOAD_GLOBAL:
        m_kind = LIR::LOAD_GLOBAL;
        
        // :NOTE: Wei 2005-May-20:
        //
        // In Li Xu's paper: Program Redundancy Analysis and Optimization to Improve Memory Performance,
        // it says that the tuple created for xLD ra=>rv [M-use] is
        // <Norm(xLD), Value(ra)>
        //
        // However, in my implementation, I don't have an address variable to indicate the address of the
        // source. But the Value() operation in this paper means equivalent value, so that in this paper,
        // it can know whether 2 xLD operations load value from the same memory address or not through this
        // Value() operation.
        //
        // Thus, taking an observation of this phenomenon, although in my implementation, I don't have a
        // field to indicate the loading address, however, I will use the original_ssa_var() attribute of
        // each global variable to indicate whether the loading addresses are the same or not.
        //
        // [NOTE] in the SSA form, the loaded destination variables are different between 2 xLD operations,
        // however, if they both load the same global variable, the original_ssa_var() attributes of these
        // 2 xLD will be the same.
        m_operands_value_number.push_back(lir->dest().front()->variable()->original_ssa_var());
        
        // :NOTE: Wei 2005-May-20:
        //
        // Although in this paper, it says that the tuple for a xLD operations is
        // <Norm(xLD), Value(ra)>, however, finish reading this paper, I found the tuple for a xLD
        // operation should really be <Norm(xLD), Value(ra), Value[M-use]>.
        m_operands_value_number.push_back(lir->memory_use_list().front()->variable()->scc_value_number());
        break;
        
      case LIR::STORE_GLOBAL:
        // :NOTE: Wei 2005-May-20:
        //
        // In Li Xu's paper, it says that all xLD and xST operations should have a normal form,
        // so that when I search xLD or xST operations in the operation table,
        // I should find both of them.
        // And this facility in Li Xu's paper is achieved by a Norm() operation.
        //
        // And my way to achieve this facility is to just set the m_kind field of a xST tuple to
        // be the same with a xLD operation.
        m_kind = LIR::LOAD_GLOBAL;
        
        m_operands_value_number.push_back(lir->operand().front()->variable()->original_ssa_var());
        
        // :NOTE: Wei 2005-May-20:
        //
        // In Li Xu's paper, he says that the tuple for a xST operation is:
        // <Norm(xST), Value(ra)>, and uses it as a lookup key into the operation table.
        // For any matching op2 with result value as Value(rv), the algorithm checks whether Value[M-use1]
        // == Value[M-use2]
        //
        // Thus, I think the real tuple for a xST operation should like the one for a xLD operation:
        // <Norm(xST), Value(ra), Value[M-use]>.
        m_operands_value_number.push_back(lir->memory_use_list().front()->variable()->scc_value_number());
        
        // :NOTE: Wei 2005-May-20:
        //
        // In Li Xu's paper, it says:
        // "For a store op1, "xST ra rv M-use1 M-def1", the algorithm constructs the tuple
        // <Norm(xST), Value(ra)> and uses it as a lookup key into the operation table.
        // For any matching op2 with result value as Value(rv),...
        //
        // Thus this means that when I want to perform a searching in the operation table
        // for a xST operation, then the result value should be considered.
        m_results_value_number.push_back(lir->operand().front()->variable()->scc_value_number());
        break;
        
      default:
        m_kind = lir->kind();
        
        for (std::vector<OperandKind *>::const_iterator iter = lir->operand().begin();
             iter != lir->operand().end();
             ++iter)
        {
          switch ((*iter)->kind())
          {
          case OperandKind::VARIABLE:
            assert((*iter)->variable()->scc_value_number() != 0);
            
            m_operands_value_number.push_back((*iter)->variable()->scc_value_number());
            break;
            
          case OperandKind::IMM:
            m_operands_value_number.push_back((*iter)->imm());
            break;
            
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
        break;
      }
    }
    
    // Inquiry
    
    inline uint32_t
    memory_operation_idx() const
    { return m_memory_operation_idx; }
    
    inline std::vector<LIR *> const &
    equivalent_operation() const
    { return m_equivalent_operation; }
    
    inline LIR::KindEnum
    kind() const
    { return m_kind; }
    
    inline std::vector<ValueNumber> const &
    operands_value_number() const
    { return m_operands_value_number; }
    
    inline std::vector<Variable *> const &
    results_value_number() const
    { return m_results_value_number; }
    
    // Access
    
    inline void
    set_memory_operation_idx(uint32_t const idx)
    { m_memory_operation_idx = idx; }
    
    inline bool
    add_equivalent_operation(LIR * const lir)
    {
      assert(lir != 0);
      
      for (std::vector<LIR *>::const_iterator iter = m_equivalent_operation.begin();
           iter != m_equivalent_operation.end();
           ++iter)
      {
        if ((*iter) == lir)
        {
          return false;
        }
      }
      
      m_equivalent_operation.push_back(lir);
      
      return true;
    }
    
    inline std::vector<ValueNumber> &
    operands_value_number()
    { return m_operands_value_number; }
    
    inline std::vector<Variable *> &
    results_value_number()
    { return m_results_value_number; }
    
    // Operator
    
    inline bool
    operator==(Tuple const &tuple) const
    {
      if (m_kind == tuple.kind())
      {
        assert(m_operands_value_number.size() == tuple.operands_value_number().size());
        
        switch (m_kind)
        {
        case LIR::LSFT_LEFT_RR:
        case LIR::LSFT_RIGHT_RR:
        case LIR::ASFT_RIGHT_RR:
        case LIR::ROR_RR:
        case LIR::RRX_RR:
          
        case LIR::LSFT_LEFT_RI:
        case LIR::LSFT_RIGHT_RI:
        case LIR::ASFT_RIGHT_RI:
        case LIR::ROR_RI:
        case LIR::RRX_RI:
          
        case LIR::SUB_RR:
        case LIR::SBC_RR:
          
        case LIR::SUB_RI:
        case LIR::SBC_RI:
          if ((m_operands_value_number.front() == tuple.operands_value_number().front()) &&
              (m_operands_value_number.back() == tuple.operands_value_number().back()))
          {
            return true;
          }
          break;
          
        case LIR::ADD_RR:
        case LIR::ADC_RR:
        case LIR::AND_RR:
        case LIR::OR_RR:
        case LIR::EOR_RR:
          
        case LIR::ADD_RI:
        case LIR::ADC_RI:
        case LIR::AND_RI:
        case LIR::OR_RI:
        case LIR::EOR_RI:
          
        case LIR::MUL32:
        case LIR::IMUL32:
          if (((m_operands_value_number.front() == tuple.operands_value_number().front()) &&
               (m_operands_value_number.back() == tuple.operands_value_number().back())) ||
              ((m_operands_value_number.front() == tuple.operands_value_number().back()) &&
               (m_operands_value_number.back() == tuple.operands_value_number().front())))
          {
            return true;
          }
          break;
          
        case LIR::NOT:
        case LIR::MOV_I_2_R:
        case LIR::MOV_R_2_R:
          if (m_operands_value_number.front() == tuple.operands_value_number().front())
          {
            return true;
          }
          break;
          
        case LIR::LOAD_GLOBAL:
          if ((m_operands_value_number.front() == tuple.operands_value_number().front()) &&
              (m_operands_value_number.back() == tuple.operands_value_number().back()))
          {
            if (m_results_value_number.size() != 0)
            {
              assert(m_results_value_number.size() == tuple.results_value_number().size());
              
              // :NOTE: Wei 2005-May-20:
              //
              // In Li Xu's paper, it says:
              // "For a store op1, "xST ra rv M-use1 M-def1", the algorithm constructs the tuple
              // <Norm(xST), Value(ra)> and uses it as a lookup key into the operation table.
              // For any matching op2 with result value as Value(rv),...
              //
              // Thus this means that when I want to perform a searching in the operation table
              // for a xST operation, then the result value should be considered.
              if (m_results_value_number.front() == tuple.results_value_number().front())
              {
                return true;
              }
              else
              {
                return false;
              }
            }
            
            return true;
          }
          break;
          
        case LIR::MOV_FROM_A:
        case LIR::MOV_LABEL_2_R:
        case LIR::MOV_FROM_M:
          
        case LIR::LOAD_SP:
        case LIR::STORE_SP:
        case LIR::ADD_SP:
          
        case LIR::PUSHF:
        case LIR::POPF:
          
        case LIR::PUSH_R:
        case LIR::PUSH_I:
          
        case LIR::JMP_R:
        case LIR::JMP_I:
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
          
        case LIR::CMP_RR:
        case LIR::TST_RR:
        case LIR::CMP_RI:
        case LIR::TST_RI:
          
        case LIR::SET_S:
        case LIR::SET_Z:
        case LIR::SET_C:
        case LIR::SET_O:
        case LIR::BT:
        case LIR::CMC:
          
        case LIR::CALL:
        case LIR::RETURN:
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      
      return false;
    }
  };
}

#endif

#endif
