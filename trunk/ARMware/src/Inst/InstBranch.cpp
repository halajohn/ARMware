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
#include "../Serial_3.hpp"
#include "../GPIOControlBlock.hpp"
#include "../RTC.hpp"
#include "../IntController.hpp"

#include "../Utils/Number.hpp"

#include "../Gtk/MachineScreen.hpp"

#include "InstBranch.hpp"

#if ENABLE_THREADED_CODE
#include "../MachineType.hpp"
#endif

#include "../Core.tcc"
#include "../Uart.tcc"

#include "InstBranch.tcc"

namespace ARMware
{
  // Explicit instantiation
  
  template struct InstBranch<ImmBranch,    LINK,    EXCHANGE, USAGE_NORMAL>;
  template struct InstBranch<RegBranch, NO_LINK,    EXCHANGE, USAGE_NORMAL>;
  template struct InstBranch<RegBranch,    LINK,    EXCHANGE, USAGE_NORMAL>;
  template struct InstBranch<ImmBranch, NO_LINK, NO_EXCHANGE, USAGE_NORMAL>;
  template struct InstBranch<ImmBranch,    LINK, NO_EXCHANGE, USAGE_NORMAL>;
  
#if ENABLE_THREADED_CODE
  template struct InstBranch<ImmBranch,    LINK,    EXCHANGE, USAGE_THREADED_CODE>;
  template struct InstBranch<RegBranch, NO_LINK,    EXCHANGE, USAGE_THREADED_CODE>;
  template struct InstBranch<RegBranch,    LINK,    EXCHANGE, USAGE_THREADED_CODE>;
  template struct InstBranch<ImmBranch, NO_LINK, NO_EXCHANGE, USAGE_THREADED_CODE>;
  template struct InstBranch<ImmBranch,    LINK, NO_EXCHANGE, USAGE_THREADED_CODE>;
#endif
}
