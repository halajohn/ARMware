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

#ifndef InstLSM_hpp
#define InstLSM_hpp

#include "../ARMware.hpp"

namespace ARMware
{
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  struct InstLSM
  {
    // Attribute
    
    Inst mInst;
    
#if CHECK_LSM_END_ADDR
    mutable uint32_t mEndAddr;
#endif
    
    // Operation
    
    CoreRegNum RnNum() const;
    
    uint32_t Rn() const;
    uint32_t RegEnableCount() const;
    
    uint32_t StartAddress() const;
    
    bool     RegEnable(CoreRegNum const reg_num) const;
    
    void     WriteBackBase() const;
    
    void     OperationInternal(uint32_t const orig_rn_value,
                               uint32_t address,
                               bool &exception_occur) const;
    
    void     Operation(bool &exception_occur) const;
    
#if CHECK_UNPREDICTABLE
    void     CheckRnRequirement() const;
    void     CheckProperMode() const;
    void     CheckBaseRegisterWriteBack() const;
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
