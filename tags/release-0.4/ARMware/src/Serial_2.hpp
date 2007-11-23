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

// Serial port 2
//

#ifndef Serial_2_hpp
#define Serial_2_hpp

// Project include
//

#include "ARMware.hpp"

// Local include
//

#include "Uart.hpp"

namespace ARMware
{
  typedef class Serial_2 Serial_2;
  class Serial_2
  {
  private:
    
    enum
    {
      HSCR0 = 0x80040080,
      HSCR1 = 0x80040084,
      HSDR  = 0x8004008C,
      HSSR0 = 0x80040074,
      HSSR1 = 0x80040078,
      
      UTCR4 = 0x80030010
    };
    
    Uart<Serial_2> m_UART;
    
    uint8_t m_UTCR4; // UART control register 4
    
    uint8_t m_HSCR0; // HSSP Control Register 0
    uint8_t m_HSCR1; // HSSP Control Register 1
    uint8_t m_HSDR;  // HSSP Data Register
    uint8_t m_HSSR0; // HSSP Status Register 0
    uint8_t m_HSSR1; // HSSP Status Register 1
    
    // Operation
    
    void
    init_register()
    {
      m_HSCR0 = 0;
      m_HSCR1 = 0;
      m_HSDR = 0;
      m_HSSR0 = 0;
      m_HSSR1 = 0;
      
      m_UTCR4 = 0;
    }
    
  public:
    
    // Life cycle
    
    Serial_2();
    
    // Access
    
    Uart<Serial_2> const &
    get_UART() const
    { return m_UART; };
    
    // Operation
    
    inline void
    reset()
    {
      m_HSCR0 &= ~((1 << 4) | (1 << 3));
      m_HSDR = 0;
      m_HSSR0 &= ~((1 << 7) | (1 << 6) | (1 << 4) | (1 << 3));
      m_HSSR1 = (1 << 3);
      
      m_UTCR4 = 0;
      
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
