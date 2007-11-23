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

#include "Inst.hpp"

namespace ARMware
{
#if PRINT_CHUNK_CODE
  std::string const AND::NAME = "AND";
  std::string const EOR::NAME = "EOR";
  std::string const SUB::NAME = "SUB";
  std::string const RSB::NAME = "RSB";
  std::string const ADD::NAME = "ADD";
  std::string const ADC::NAME = "ADC";
  std::string const SBC::NAME = "SBC";
  std::string const RSC::NAME = "RSC";
  std::string const ORR::NAME = "ORR";
  std::string const BIC::NAME = "BIC";
  
  std::string const MOV::NAME = "MOV";
  std::string const MVN::NAME = "MVN";
  
  std::string const TST::NAME = "TST";
  std::string const TEQ::NAME = "TEQ";
  std::string const CMP::NAME = "CMP";
  std::string const CMN::NAME = "CMN";
#endif
}
