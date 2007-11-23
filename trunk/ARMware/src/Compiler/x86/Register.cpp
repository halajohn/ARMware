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

#include "Register.hpp"

#include "../../Log.hpp"

namespace ARMware
{
  std::vector<Register::RegisterWeight> Register::REG_WEIGHT;
  
#if PRINT_CHUNK_CODE
  void
  Register::dump_info() const
  {
    switch (m_kind)
    {
    case EAX: g_log_file << "EAX"; break;
    case EBX: g_log_file << "EBX"; break;
    case ECX: g_log_file << "ECX"; break;
    case EDX: g_log_file << "EDX"; break;
    case ESI: g_log_file << "ESI"; break;
    case EDI: g_log_file << "EDI"; break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  void
  Register::RegisterWeight::dump_info() const
  {
    switch (m_kind)
    {
    case EAX: g_log_file << "EAX"; break;
    case EBX: g_log_file << "EBX"; break;
    case ECX: g_log_file << "ECX"; break;
    case EDX: g_log_file << "EDX"; break;
    case ESI: g_log_file << "ESI"; break;
    case EDI: g_log_file << "EDI"; break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    g_log_file << "= "
               << std::dec
               << m_weight;
  }
#endif
}

#endif
