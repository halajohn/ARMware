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

#ifndef InstMulti_hpp
#define InstMulti_hpp

namespace ARMware
{
  template<IsLong is_long,
           IsSigned is_signed,
           IsAccumulate is_accumulate,
           IsUpdateStatus is_update_status>
  struct InstMulti
  {
    // Attribute
    
    Inst mInst;
    
    uint64_t mResult;
    uint32_t mHiResult;
    uint32_t mLoResult;
    
    // Operation
    
    CoreRegNum RmNum() const;
    CoreRegNum RsNum() const;
    CoreRegNum RdNum() const;
    CoreRegNum RnNum() const;
    CoreRegNum RdHiNum() const;
    CoreRegNum RdLoNum() const;
    
    void UpdateStatusRegister() const;
    void Finalize() const;
    
    void OperationInternal();
    void Operation();
    
#if CHECK_UNPREDICTABLE
    void CheckRequirement() const;
#endif
    
    // Operation
    
    ExecResultEnum Process(Inst const inst);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    void gen_dt_code(Inst const inst, bool const is_last);
#endif
    
#if PRINT_CHUNK_CODE
    void print_code(Inst const inst);
#endif
  };
}

#endif
