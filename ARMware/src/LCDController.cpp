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

#include "LCDController.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Life cycle =================================
  
  LCDController::LCDController(Core * const core,
                               Memory * const memory,
                               MachineScreen * const machine_screen)
    : mp_core(core),
      mp_memory(memory),
      mp_machine_screen(machine_screen)
  {
    init_register();
  }
  
  //============================== Operation ==================================
  
  void
  LCDController::reset()
  {
    m_LCCR0 = 0;
    m_LCCR1 = 0;
    m_LCCR2 = 0;
    m_LCCR3 = 0;
    
    m_LCSR = 0;
  }
}
