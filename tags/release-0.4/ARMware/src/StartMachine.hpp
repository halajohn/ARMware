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

#ifndef StartMachine_hpp
#define StartMachine_hpp

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class MachineImpl;
  class MachineScreen;
  class SoftKeyboard;
  class SerialConsole;
  
  uint32_t start_machine(MachineImpl const &machineImpl,
                         MachineScreen * const machine_screen,
                         SoftKeyboard * const soft_keyboard,
                         SerialConsole &serialConsole);
}

#endif
