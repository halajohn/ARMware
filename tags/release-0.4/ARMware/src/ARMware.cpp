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

#include <cstdlib> // for EXIT_FAILURE
#include <iostream>

#include "Log.hpp"
#include "ARMware.hpp"
#include "Inst/Inst.hpp"

namespace ARMware
{
  void
  DiagnosticErrorWithInst(Inst const &inst, char const * const filename, int const line)
  {
    g_log_file << "File = " << filename << std::endl;
    g_log_file << "Line = " << std::dec << line << std::endl;
    g_log_file << "Inst = " << std::hex << inst.mRawData << std::endl;
    
    exit(EXIT_FAILURE);
  }
  
  std::string const DataWidthTraits<WORD>::DESC_STRING = "WORD";
  std::string const DataWidthTraits<HALF_WORD>::DESC_STRING = "HALF_WORD";
  std::string const DataWidthTraits<BYTE>::DESC_STRING = "BYTE";
}
