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

#ifndef InstLSW_hpp
#define InstLSW_hpp

namespace ARMware
{
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  struct InstLSW
  {
    // Attribute
    
    Inst m_inst;
    
#if CHECK_CALL_ONLY_ONE
    mutable bool m_call_address_func;
    mutable bool m_call_offset_func;
#endif
    
    // Operation
    
    CoreRegNum rn_num() const;
    CoreRegNum rd_num() const;
    CoreRegNum rm_num() const;
    
    uint32_t rn() const;
    uint32_t offset() const;
    
#if PRINT_CHUNK_CODE
    uint32_t offset_without_check() const;
#endif
    
    uint32_t address() const;
    CoreMode assume_mode() const;
    
    SftType  which_sft_type() const;
    uint32_t sft_imm() const;
    
    void operation_internal(bool &exception_occur) const;
    void operation(bool &exception_occur) const;
    
#if CHECK_UNPREDICTABLE
    void check_rn_requirement() const;
    void check_rm_requirement() const;
    void check_rd_requirement_by_addressing_mode() const;
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
    void print_sft_field() const;
    void print_code(Inst const inst);
#endif
  };
}

#endif
