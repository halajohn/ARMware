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

#if ENABLE_THREADED_CODE
#include "../ChunkChain.hpp"
#endif

#include "../Utils/Number.hpp"
#include "InstSWP.hpp"

#include "../AddressSpace.tcc"
#include "../Mmu.tcc"
#include "../Core.tcc"
#include "InstSWP.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  // SWP
  
  template struct InstSWP<WORD, USAGE_NORMAL>;
  template struct InstSWP<BYTE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // SWP - threaded code version
  
  template struct InstSWP<WORD, USAGE_THREADED_CODE>;
  template struct InstSWP<BYTE, USAGE_THREADED_CODE>;
#endif
}
