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
#include "../AddressSpace.hpp"
#include "../ChunkChain.hpp"
#include "../OsTimer.hpp"
#include "../Serial_1.hpp"
#include "../Serial_2.hpp"
#include "../Serial_3.hpp"
#include "../GPIOControlBlock.hpp"
#include "../RTC.hpp"
#include "../IntController.hpp"

#include "../Utils/Number.hpp"
#include "../Gtk/MachineScreen.hpp"

#include "InstMulti.hpp"

#if ENABLE_THREADED_CODE
#include "../MachineType.hpp"
#endif

#include "../Core.tcc"
#include "../Mmu.tcc"
#include "InstMulti.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  template struct InstMulti<NORMAL,   SIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS>;
  template struct InstMulti<NORMAL,   SIGNED, NO_ACCUMULATE,    UPDATE_STATUS>;
  template struct InstMulti<NORMAL,   SIGNED,    ACCUMULATE, NO_UPDATE_STATUS>;
  template struct InstMulti<NORMAL,   SIGNED,    ACCUMULATE,    UPDATE_STATUS>;
  template struct InstMulti<  LONG, UNSIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS>;
  template struct InstMulti<  LONG, UNSIGNED, NO_ACCUMULATE,    UPDATE_STATUS>;
  template struct InstMulti<  LONG, UNSIGNED,    ACCUMULATE, NO_UPDATE_STATUS>;
  template struct InstMulti<  LONG, UNSIGNED,    ACCUMULATE,    UPDATE_STATUS>;
  template struct InstMulti<  LONG,   SIGNED, NO_ACCUMULATE, NO_UPDATE_STATUS>;
  template struct InstMulti<  LONG,   SIGNED, NO_ACCUMULATE,    UPDATE_STATUS>;
  template struct InstMulti<  LONG,   SIGNED,    ACCUMULATE, NO_UPDATE_STATUS>;
  template struct InstMulti<  LONG,   SIGNED,    ACCUMULATE,    UPDATE_STATUS>;
}
