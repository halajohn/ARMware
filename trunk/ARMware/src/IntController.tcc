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

#include "IntController.hpp"
#include "UartTraits.hpp"

namespace ARMware
{
  //////////////////////////////// Private ////////////////////////////////////
  //============================= Operation ===================================
  
  template<typename T_serial_port>
  void
  IntController::check_UART_interrupt(Uart<T_serial_port> const &uart)
  {
    uint32_t const UART_status = uart.get_interrupt_status();
    
    if (0 == UART_status)
    {
      m_ICPR &= ~UartTraits<T_serial_port>::INT_CTRL_BIT_MASK;
    }
    else
    {
      uint32_t const ctrl_3 = uart.get_ctrl_reg_3();
      
      switch (ctrl_3 & (Uart<T_serial_port>::UTCR3_TIE |
                        Uart<T_serial_port>::UTCR3_RIE))
      {
      case 0:
        break;
        
      case Uart<T_serial_port>::UTCR3_TIE:
        if ((UART_status & Uart<T_serial_port>::UTSR0_TFS) != 0)
        {
          m_ICPR |= UartTraits<T_serial_port>::INT_CTRL_BIT_MASK;
        }
        else
        {
          m_ICPR &= ~UartTraits<T_serial_port>::INT_CTRL_BIT_MASK;
        }
        break;
        
      case Uart<T_serial_port>::UTCR3_RIE:
        if ((UART_status & (Uart<T_serial_port>::UTSR0_RFS |
                            Uart<T_serial_port>::UTSR0_RID)) != 0)
        {
          m_ICPR |= UartTraits<T_serial_port>::INT_CTRL_BIT_MASK;
        }
        else
        {
          m_ICPR &= ~UartTraits<T_serial_port>::INT_CTRL_BIT_MASK;
        }
        break;
        
      case (Uart<T_serial_port>::UTCR3_TIE | Uart<T_serial_port>::UTCR3_RIE):
        if ((UART_status & (Uart<T_serial_port>::UTSR0_TFS |
                            Uart<T_serial_port>::UTSR0_RFS |
                            Uart<T_serial_port>::UTSR0_RID)) != 0)
        {
          m_ICPR |= UartTraits<T_serial_port>::INT_CTRL_BIT_MASK;
        }
        else
        {
          m_ICPR &= ~UartTraits<T_serial_port>::INT_CTRL_BIT_MASK;
        }
        break;
      }
    }
  }
}
