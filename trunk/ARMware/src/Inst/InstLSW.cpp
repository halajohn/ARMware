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
#include "InstLSW.hpp"

#include "../AddressSpace.tcc"
#include "../Mmu.tcc"
#include "../Core.tcc"
#include "InstLSW.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  // STR
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // STR - threaded code version
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
#endif
  
  // STRT
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // STRT - threaded code version
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE, STORE, USAGE_THREADED_CODE>;
#endif
  
  // STRB
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // STRB - threaded code version
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE, STORE, USAGE_THREADED_CODE>;
#endif
  
  // STRBT
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // STRBT - threaded code version
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE, STORE, USAGE_THREADED_CODE>;
#endif
  
  // LDR
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDR - threaded code version
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, WORD, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
#endif
  
  // LDRT
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDRT - threaded code version
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, WORD,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
#endif
  
  // LDRB
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDRB - threaded code version
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,   OFFSET,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX, DOWN, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW,  PRE_IDX,   UP, BYTE, CURRENT_MODE,  LOAD, USAGE_THREADED_CODE>;
#endif
  
  // LDRBT
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDRBT
  
  template struct InstLSW<ImmLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<ImmLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSW<RegLSW, POST_IDX, DOWN, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSW<RegLSW, POST_IDX,   UP, BYTE,    USER_MODE,  LOAD, USAGE_THREADED_CODE>;
#endif
}
