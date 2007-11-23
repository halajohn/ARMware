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

#include "InstLSH.hpp"

#include "../AddressSpace.tcc"
#include "../Core.tcc"
#include "../Mmu.tcc"
#include "InstLSH.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  // STRH
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // STRH - threaded code version
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD, STORE, USAGE_THREADED_CODE>;
#endif
  
  // LDRH
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDRH - threaded code version
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP, UNSIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
#endif
  
  // LDRSB
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDRSB - threaded code version
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED,      BYTE,  LOAD, USAGE_THREADED_CODE>;
#endif
  
  // LDRSH
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  // LDRSH - threaded code version
  
  template struct InstLSH<RegLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<RegLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  
  template struct InstLSH<ImmLSH, POST_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH, POST_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,   OFFSET,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX, DOWN,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
  template struct InstLSH<ImmLSH,  PRE_IDX,   UP,   SIGNED, HALF_WORD,  LOAD, USAGE_THREADED_CODE>;
#endif
}
