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

#ifndef InstLSH_hpp
#define InstLSH_hpp

namespace ARMware
{
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_Usage>
  struct InstLSH
  {
    // Attribute
    
    Inst mInst;
    
    // Operation
    
    CoreRegNum RnNum() const;
    CoreRegNum RmNum() const;
    CoreRegNum RdNum() const;
    
    uint32_t Rn() const;
    uint32_t Offset() const;
    uint32_t Address() const;
    
    void OperationInternal(bool &exception_occur) const;
    void OperationInternalByAddressingMode(bool &exception_occur) const;
    void Operation(bool &exception_occur) const;
    
#if CHECK_UNPREDICTABLE
    void CheckRnRequirement() const;
    void CheckRmRequirement() const;
    void CheckRdRequirement() const;
#endif
    
    // Operation
    
    ExecResultEnum Process(Inst const inst);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    void gen_dt_code(Inst const inst, bool const is_last);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    DTFunc_t dt_func;
#endif
#endif
    
#if PRINT_CHUNK_CODE
    void print_code(Inst const inst);
#endif
  };
}

#endif
