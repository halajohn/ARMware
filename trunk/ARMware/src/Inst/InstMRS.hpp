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

#ifndef InstMRS_hpp
#define InstMRS_hpp

#include "../ARMware.hpp"

namespace ARMware
{
  template<UseWhichStatus use_which_status>
  struct InstMRS
  {
    // Attribute
    
    Inst m_inst;
    
    CoreRegNum rd_num() const;
    
#if CHECK_UNPREDICTABLE
    void check_rd_require() const;
    void check_proper_core_mode() const;
#endif
    
    uint32_t desired_status_value() const;
    void operation();
    
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
