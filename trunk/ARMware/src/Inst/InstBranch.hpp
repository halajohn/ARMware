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

#ifndef InstBranch_hpp
#define InstBranch_hpp

namespace ARMware
{
  template<typename mode,
           IsLink is_link,
           ExchangeToThumb exchange_to_thumb,
           UsageEnum T_usage>
  struct InstBranch
  {
    // Attribute
    
    Inst mInst;
    
    // Operation
    
    CoreRegNum RmNum() const;
    
    uint32_t Imm() const;
    uint32_t Rm() const;
    
#if CHECK_UNPREDICTABLE
    void CheckRmRequirement() const;
    void CheckInstBits() const;
#endif
    
    void PreformLink() const;
    void PreformExchange() const;
    
    uint32_t NewPCAddon() const;
    uint32_t NewPC() const;
    
    void Operation() const;
    
    // Operation
    
    ExecResultEnum Process(Inst const inst);
    
#if ENABLE_DYNAMIC_TRANSLATOR || ENABLE_THREADED_CODE
    uint32_t dest_offset() const;
    uint32_t dest_imm() const;
#endif
    
#if ENABLE_DYNAMIC_TRANSLATOR
    inline void
    feed_inst(Inst const inst)
    { mInst = inst; }
    
    void gen_dt_code(Inst const inst, bool const is_last);
#endif
    
#if PRINT_CHUNK_CODE
    void print_code(Inst const inst);
#endif
  };
}

#endif
