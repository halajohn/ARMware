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

#ifndef InstDPI_hpp
#define InstDPI_hpp

#if ENABLE_DYNAMIC_TRANSLATOR
#include "../Compiler/Compiler.hpp"
#endif

namespace ARMware
{
  class MIR;
  class Label;
  class Variable;
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_Usage,
           typename update_status_category_id = typename opcode::UpdateStatusCategoryId,
           typename operand_number_category_id = typename opcode::OperandNumberCategoryId>
  struct InstDPI
  {
    // Attribute
    
    Inst m_inst;
    
#if ENABLE_COND_CODES_ASM
    uint32_t m_eflags;
#else  
    uint32_t op1;
    uint32_t op2;
    uint32_t result;
#endif
    
#if ENABLE_THREADED_CODE || ENABLE_DYNAMIC_TRANSLATOR
    bool m_modify_pc;
#endif
    
    // Operation
    
    uint32_t Imm() const;
    uint32_t RotImm() const;
    uint32_t SftImm() const;
    
    CoreRegNum RdNum() const;
    CoreRegNum RnNum() const;
    CoreRegNum RmNum() const;
    CoreRegNum RsNum() const;
    
    uint32_t Rn() const;
    uint32_t Rm() const;
    uint32_t SftOperand() const;
    
    void update_CPSR_flags() const;
    void Finalize() const;
    
    SftType WhichSftType() const;
    
#if CHECK_UNPREDICTABLE
    void CheckRnRequirement() const;
    void CheckRmRequirement() const;
    void CheckRdRequirementByMode() const;
    void CheckRdRequirementByOpcode() const;
    void CheckRsRequirement() const;
#endif
    
    void OperationInternal();
    void OperationInstRecall();
    void Operation();
    
    // Operation
    
    ExecResultEnum Process(Inst const inst);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    Variable *gen_shifter_operand(Label ** const after_switch_label);
    
    MIR *produce_main_mir(Variable * const rn_var,
                          Variable * const sft_operand);
    
    void gen_dt_code(Inst const inst, bool const is_last);
#endif
    
#if PRINT_CHUNK_CODE
    void print_code(Inst const inst);
#endif
  };
}

#endif
