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

#ifndef InstMSR_hpp
#define InstMSR_hpp

#include "../ARMware.hpp"
#include "Inst.hpp"

namespace ARMware
{
  enum FieldNameEnum
  {
    CONTROL,
    EXTENSION,
    STATUS,
    FLAGS,
    ALL
  };
  typedef enum FieldNameEnum FieldNameEnum;
  
  template<typename mode, UseWhichStatus use_which_status>
  struct InstMSR
  {
    // Attribute
    
    Inst m_inst;
    
    // Operation
    
    uint32_t Imm() const;
    uint32_t RotImm() const;
    CoreRegNum rm_num() const;
    
#if CHECK_UNPREDICTABLE  
    void CheckProperMode() const;
    void CheckBits() const;
#endif
    
    uint32_t Operand() const;
    uint32_t &desired_status_reg() const;
    
    template<FieldNameEnum field_name_enum>
    void UpdateField(uint32_t const operand);
    
    void change_core_mode_if_necessary() const;
    void Operation();
    
    // Operation
    
    ExecResultEnum Process(Inst const inst);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    void gen_dt_code(Inst const inst, bool const is_last);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    DTFunc_t dt_func;
#endif
#endif
    
#if PRINT_CHUNK_CODE
    void print_fields() const;
    void print_code(Inst const inst);
#endif
  };
  
  template<FieldNameEnum field_name_enum>
  struct FieldTraits;
  
  template<>
  struct FieldTraits<CONTROL>
  {
    static uint32_t const INST_BITMASK = (1 << 16);
    static uint32_t const STATUS_REG_BITMASK = 0xFF;
  };
  
  template<>
  struct FieldTraits<EXTENSION>
  {
    static uint32_t const INST_BITMASK = (1 << 17);
    static uint32_t const STATUS_REG_BITMASK = 0xFF00;
  };
  
  template<>
  struct FieldTraits<STATUS>
  {
    static uint32_t const INST_BITMASK = (1 << 18);
    static uint32_t const STATUS_REG_BITMASK = 0xFF0000;
  };
  
  template<>
  struct FieldTraits<FLAGS>
  {
    static uint32_t const INST_BITMASK = (1 << 19);
    static uint32_t const STATUS_REG_BITMASK = 0xFF000000;
  };
  
  template<>
  struct FieldTraits<ALL>
  {
    static uint32_t const INST_BITMASK = (FieldTraits<CONTROL>::INST_BITMASK |
                                          FieldTraits<EXTENSION>::INST_BITMASK |
                                          FieldTraits<STATUS>::INST_BITMASK |
                                          FieldTraits<FLAGS>::INST_BITMASK);
  };
}

#endif
