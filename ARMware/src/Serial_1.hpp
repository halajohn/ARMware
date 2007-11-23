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

#ifndef Serial_1_hpp
#define Serial_1_hpp

// System include
//

#include <memory>

// Project include
//

#include "ARMware.hpp"

// Local include
//

#include "Uart.hpp"

namespace ARMware
{
  class Core;
  class PPC;
  class AtmelMicro;
  class MachineScreen;
  
  typedef class Serial_1 Serial_1;
  class Serial_1
  {
  private:
    
    // Bit definition
    
    enum
    {
      GPCLKR0_SUS = (1 << 0), // GPCLK/UART select.
      GPCLKR0_SCE = (1 << 4), // Sample clock enable.
      GPCLKR0_SCD = (1 << 5)  // Sample clock direction.
    };
    
    enum
    {
      GPCLKR1_TXE = (1 << 1) // Transmit Enable
    };
    
    Core * const mp_core;
    PPC * const mp_PPC;
    
    Uart<Serial_1> m_UART;
    std::auto_ptr<AtmelMicro> const mp_atmel_micro;
    
    uint8_t m_GPCLKR0; // GPCLK Control Register 0
    uint8_t m_GPCLKR1; // GPCLK Control Register 1
    uint8_t m_GPCLKR2; // GPCLK Control Register 2
    uint8_t m_GPCLKR3; // GPCLK Control Register 3
    
    // Operation
    
    void
    init_register()
    {
      m_GPCLKR0 = 0;
      m_GPCLKR1 = 0;
      m_GPCLKR2 = 0;
      m_GPCLKR3 = 0;
    }
    
  public:
    
    enum
    {
      GPCLKR0 = 0x80020060,
      GPCLKR1 = 0x80020064,
      GPCLKR2 = 0x8002006C,
      GPCLKR3 = 0x80020070
    };
    
    // Life cycle
    
    Serial_1(Core * const core, PPC * const ppc, MachineScreen * const machine_screen);
    
    // Access
    
    Uart<Serial_1> const &
    get_UART() const
    { return m_UART; };
    
    // Operation
    
    inline void
    reset()
    {
      m_GPCLKR0 &= ~GPCLKR0_SUS;
      m_GPCLKR1 &= ~GPCLKR1_TXE;
      m_GPCLKR2 &= 0xF;
      
      m_UART.reset();
    }
    
    void run();
    
    // :SA-1110 Developer's Manual: Wei 2003-Dec-14:
    //
    // All registers in the Peripheral Control Module are accessed via
    // the CPU must be performed using word reads and writes.
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address);
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
}

#endif
