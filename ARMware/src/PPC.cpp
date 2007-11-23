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

#include <cassert>
#include "PPC.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Operation ==================================
  
  void
  PPC::reset()
  {
    m_PPDR = 0xFFC00000;
    m_PPSR |= 0xFFC00000;
    m_PPAR = static_cast<uint32_t>(~((1 << 12) | (1 << 18)));
    m_PSDR = 0x3FFFFF;
    m_PPFR = 0xFFFFFFFF;
    
    m_HSCR2 = ((1 << 19) | (1 << 18));
  }
}
