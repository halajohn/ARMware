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

// Serial port 3
//

#ifndef Serial_3_hpp
#define Serial_3_hpp

// Project include
//

#include "ARMware.hpp"

// Local include
//

#include "Uart.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class SerialConsole;
  
  typedef class Serial_3 Serial_3;
  class Serial_3
  {
  private:
    
    Uart<Serial_3> m_UART;
    
  public:
    
    // Life cycle
    
    Serial_3(SerialConsole &serial_console);
    
    // Access
    
    Uart<Serial_3> const &
    get_UART() const
    { return m_UART; };
    
    // Operation
    
    inline void
    reset()
    { m_UART.reset(); }
    
    inline void
    run()
    { m_UART.run(); }
    
    template<typename T_return>
    inline T_return FASTCALL
    get_data(uint32_t const address)
    { return m_UART.get_data<T_return>(address); }
    
    template<typename T_store>
    inline void FASTCALL 
    put_data(uint32_t const address, T_store const value)
    { m_UART.put_data(address, value); }
  };
}

#endif
