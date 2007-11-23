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
#include "../Utils/Number.hpp"

#if ENABLE_THREADED_CODE
#include "../ChunkChain.hpp"
#endif

#include "InstLSM.hpp"

#include "../AddressSpace.tcc"
#include "../Mmu.tcc"
#include "../Core.tcc"
#include "InstLSM.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  // STM
  
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_NORMAL>;
  template struct InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE  
  // STM - threaded code version
  
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK, STORE, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK, STORE, USAGE_THREADED_CODE>;
#endif
  
  // LDM
  
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<DOWN,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<DOWN, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<  UP,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_NORMAL>;
  template struct InstLSM<  UP, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDM - threaded code version
  
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<DOWN, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP,  AFTER, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP,  AFTER,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP,  AFTER,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP, BEFORE, CURRENT_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP, BEFORE,    USER_BANK, NO_WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSM<  UP, BEFORE,    USER_BANK,    WRITE_BACK,  LOAD, USAGE_THREADED_CODE>;
#endif
}
