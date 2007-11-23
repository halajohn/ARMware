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

#include "InstMSR.hpp"

#if ENABLE_THREADED_CODE
#include "../MachineType.hpp"
#endif

#include "../Core.tcc"
#include "InstMSR.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  // MSR
  
  template struct InstMSR<RegMSR,         USE_CPSR>;
  template struct InstMSR<RegMSR, USE_CURRENT_SPSR>;
  template struct InstMSR<ImmMSR,         USE_CPSR>;
  template struct InstMSR<ImmMSR, USE_CURRENT_SPSR>;
}
