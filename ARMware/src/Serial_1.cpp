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

// Serial port 1
//

#include <cassert>

#include "Serial_1.hpp"
#include "Core.hpp"
#include "PPC.hpp"
#include "AtmelMicro.hpp"
#include "Gtk/MachineScreen.hpp"

#include "Uart.tcc"

namespace ARMware
{
  ////////////////////////////////// Public ///////////////////////////////////
  //=============================== Life cycle ================================
  
  Serial_1::Serial_1(Core * const core, PPC * const ppc, MachineScreen * const machine_screen)
    : mp_core(core),
      mp_PPC(ppc),
      mp_atmel_micro(new AtmelMicro())
  {
    init_register();
    
    m_UART.register_output_device(reinterpret_cast<void * const>(mp_atmel_micro.get()));
    
    machine_screen->register_atmel_micro(mp_atmel_micro.get());
  }
  
  //================================ Operation ================================
  
  // :NOTE: Wei 2004-Jun-11:
  //
  // According to the iPaq H3600 hardware specification,
  // GPCLK is used to drive an input clock to the audio chip.
  //
  // However, ARMware doesn't support audio output at the present time,
  // thus I don't implement GPCLK functionality currently.
  void
  Serial_1::run()
  {
    // :NOTE: Wei 2004-Jun-11:
    //
    // According to the SA-1110 Developer's Manual,
    // when PPAR_UPR is set, both GPCLK and UART are enabled using different
    // input/output pins.
    // (GPCLK uses serial 1's pin, and UART uses GPIO pin 14 as output and
    //  GPIO pin 15 as input).
    //
    // When PPAR_UPR is unset, serial 1's mode is according to the GPCLKR0_SUS bit.
    // If SUS == 0, serial 1 acts as GPCLK,
    // otherwise, serial 1 acts as UART.
    if ((mp_PPC->get_PPAR() & PPC::PPAR_UPR) || (m_GPCLKR0 & GPCLKR0_SUS))
    {
      m_UART.run();
    }
  }
}
