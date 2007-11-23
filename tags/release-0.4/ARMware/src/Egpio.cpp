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

#include "Egpio.hpp"
#include "Core.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Life cycle =================================
  
  // :NOTE: Wei 2003-Dec-29:
  //
  // a segmentation fault is better than a raw pointer.
  Egpio::Egpio(Core * const core)
    : mp_core(core)
  {
    init_register();
  }
  
  //============================== Operation ==================================
  
  void
  Egpio::reset()
  {
    m_egpio = 0;
  }
}
