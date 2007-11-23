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

#include "../Core.hpp"
#include "../ChunkChain.hpp"
#include "../Utils/Number.hpp"
#include "InstDPI.hpp"

#include "../AddressSpace.tcc"
#include "../Core.tcc"
#include "InstDPI.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  // DPI
  
  template struct InstDPI<AND, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<AND, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<AND,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<AND, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<AND, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<AND,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<EOR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<EOR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<EOR,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<EOR, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<EOR, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<EOR,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<SUB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SUB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SUB,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SUB, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SUB, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SUB,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<RSB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSB,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSB, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSB, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSB,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<ADD, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADD, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADD,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADD, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADD, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADD,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<ADC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ADC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<SBC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SBC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SBC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SBC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SBC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<SBC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<RSC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<RSC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<TST, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<TST, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<TST,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<TEQ, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<TEQ, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<TEQ,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<CMP, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<CMP, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<CMP,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<CMN, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<CMN, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<CMN,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<ORR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ORR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ORR,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ORR, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ORR, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<ORR,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<MOV, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MOV, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MOV,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MOV, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MOV, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MOV,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<BIC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<BIC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<BIC,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<BIC, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<BIC, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<BIC,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
  template struct InstDPI<MVN, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MVN, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MVN,       ImmDPI, NO_UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MVN, RegImmSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MVN, RegRegSftDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  template struct InstDPI<MVN,       ImmDPI,    UPDATE_STATUS, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  template struct InstDPI<AND, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<AND, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<AND,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<AND, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<AND, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<AND,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<EOR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<EOR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<EOR,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<EOR, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<EOR, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<EOR,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<SUB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SUB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SUB,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SUB, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SUB, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SUB,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<RSB, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSB, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSB,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSB, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSB, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSB,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<ADD, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADD, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADD,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADD, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADD, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADD,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<ADC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ADC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<SBC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SBC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SBC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SBC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SBC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<SBC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<RSC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<RSC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<TST, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<TST, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<TST,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<TEQ, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<TEQ, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<TEQ,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<CMP, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<CMP, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<CMP,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<CMN, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<CMN, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<CMN,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<ORR, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ORR, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ORR,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ORR, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ORR, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<ORR,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<MOV, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MOV, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MOV,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MOV, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MOV, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MOV,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<BIC, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<BIC, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<BIC,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<BIC, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<BIC, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<BIC,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  
  template struct InstDPI<MVN, RegImmSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MVN, RegRegSftDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MVN,       ImmDPI, NO_UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MVN, RegImmSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MVN, RegRegSftDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
  template struct InstDPI<MVN,       ImmDPI,    UPDATE_STATUS, USAGE_THREADED_CODE>;
#endif
}
