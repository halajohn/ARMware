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

#ifndef BitPosition_hpp
#define BitPosition_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include "../Type.hpp"

#include "Variable.hpp"

namespace ARMware
{
  inline void
  calculate_bit_position(uint32_t const idx,
                         uint32_t &vector_idx,
                         uint32_t &bit_position)
  {
    vector_idx = (idx >> 5);
    bit_position = (1 << (idx - (vector_idx << 5)));
  }
}

#endif

#endif
