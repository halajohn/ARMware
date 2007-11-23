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

#include <cassert>

#if TRACE_UART
#include <iostream>
#endif

#include "UartTraits.hpp"
#include "AtmelMicro.hpp"
#include "Gtk/SerialConsole.hpp"

namespace ARMware
{
  ////////////////////////////////// Private //////////////////////////////////
  //================================ Operation ================================
  
  template<typename T_serial_port>
  inline void
  Uart<T_serial_port>::tx_data()
  {
    assert(!"Should not reach here.");
  }
  
  template<typename T_serial_port>
  inline void
  Uart<T_serial_port>::rx_data()
  {
    assert(!"Should not reach here.");
  }
  
  // Serial 1
  
  template<>
  inline void
  Uart<Serial_1>::tx_data()
  {
    reinterpret_cast<AtmelMicro *>(mp_output_device)->get_data_from_serial_port(m_tx_fifo);
  }
  
  template<>
  inline void
  Uart<Serial_1>::rx_data()
  {
    reinterpret_cast<AtmelMicro *>(mp_output_device)->put_data_to_serial_port(m_rx_fifo);
  }
  
  // Serial 3
  
  template<>
  inline void
  Uart<Serial_3>::tx_data()
  {
    // :NOTE: Wei 2004-Jun-10:
    //
    // Becuase of using fifo[0], fifo has to reside in a consecutive memory blocks.
    // This is why I use std::vector rather than std::deque for m_tx_fifo.
    reinterpret_cast<SerialConsole *>(mp_output_device)->put_char_to_serial_console(m_tx_fifo);
  }
  
  template<>
  inline void
  Uart<Serial_3>::rx_data()
  {
    // :TODO: Wei 2004-Jun-29:
    //
    // Receives data from serial console
    reinterpret_cast<SerialConsole *>(mp_output_device)->get_char_from_serial_console();
  }
  
  ////////////////////////////////// Public ///////////////////////////////////
  //============================== Life cycle =================================
  
  template<typename T_serial_port>
  Uart<T_serial_port>::Uart()
  {
    init_register();
  }
  
  //================================ Operation ================================
  
  template<typename T_serial_port>
  inline void
  Uart<T_serial_port>::run()
  {
    if ((m_UTCR3 & UTCR3_TXE) && (m_tx_fifo.size() != 0))
    {
      tx_data();
      
      m_tx_fifo.clear();
      
      m_UTSR0 |= UTSR0_TFS; // Enable Transmit FIFO service request
      m_UTSR1 |= UTSR1_TNF; // Transmit FIFO not full
    }
    
    if (m_UTCR3 & UTCR3_RXE)
    {
      // :NOTE: Wei 2004-Jul-1:
      //
      // Try to get more data.
      rx_data();
      
      switch (m_rx_fifo.size())
      {
      case  0:
        m_UTSR1 &= ~UTSR1_RNE;
        break;
        
      case  1:
      case  2:
      case  3:
      case  4:
      case  5:
      case  6:
      case  7:
        // :SA-1110 Developer's Manual: p.338: Wei 2003-Jun-29:
        //
        // If receiver is enabled, receive FIFO not empty, 3 frame times elapsed without receiving data, request interrupt.
        //
        // :NOTE: Wei 2004-Jun-29:
        //
        // However, I don't see the reasone that I should implement a time-critical operation like this (3 frame times),
        // thus, whenever the rx fifo isn't empty, ARMware will set the RID bit in UTSR0 up.
        m_UTSR0 |= UTSR0_RID;
        
        m_UTSR1 |= UTSR1_RNE;
        break;
        
      case  8:
      case  9:
      case 10:
      case 11:
      case 12:
      default:
        // :SA-1110 Developer's Manual: p.338: Wei 2003-Jun-29:
        //
        // Receive FIFO is one- to two-thirds full (contains 5, 6, 7, or 8 entries of data) or more,
        // and receiver operation is enabled, DMA service request signalled,
        // and interrupt request signalled if not masked (if RIE=1).
        //
        // :NOTE: Wei 2004-Jun-29:
        //
        // And I choice 8 elements as the delimiter.
        m_UTSR0 |= (UTSR0_RFS | UTSR0_RID);
        
        m_UTSR1 |= UTSR1_RNE;
        break;
      }
      
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                 << ">: receive: m_rx_fifo.size() = " 
                 << m_rx_fifo.size()
                 << ", UTSR0: " << std::hex
                 << m_UTSR0
                 << ", UTSR1: "
                 << m_UTSR1
                 << std::endl;
#endif
    }
  }
  
  template<typename T_serial_port>
  void
  Uart<T_serial_port>::reset()
  {
    // bit 7 sets 0 on reset.
    m_UTCR0 &= ~(1 << 7);
    
    // bit 4, 5, 6, 7 set 0 on reset.
    m_UTCR1 &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4));
    
    // bit 0, 1, 6, 7 set 0 on reset.
    m_UTCR3 &= ~((1 << 7) | (1 << 6) | (1 << 1) | (1 << 0));
    
    // bit 0, 1, 5, 6, 7 set 0 on reset.
    m_UTSR0 &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 1) | (1 << 0));
    
    // bit 0, 1, 5, 6, 7 set 0 on reset.
    m_UTSR1 &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 1) | (1 << 0));
  }
  
  template<typename T_serial_port>
  template<typename T_return>
  T_return
  Uart<T_serial_port>::get_data(uint32_t const address)
  {
    switch (address)
    {
    case UartTraits<T_serial_port>::UTCR0:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: get UTCR0: " << std::hex
                << static_cast<T_return>(m_UTCR0) << std::endl;
#endif
      
      return static_cast<T_return>(m_UTCR0);
      
    case UartTraits<T_serial_port>::UTCR1:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: get UTCR1: " << std::hex
                << static_cast<T_return>(m_UTCR1) << std::endl;
#endif
      
      return static_cast<T_return>(m_UTCR1);
      
    case UartTraits<T_serial_port>::UTCR2:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: get UTCR2: " << std::hex
                << static_cast<T_return>(m_UTCR2) << std::endl;
#endif
      
      return static_cast<T_return>(m_UTCR2);
      
    case UartTraits<T_serial_port>::UTCR3:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: get UTCR3: " << std::hex
                << static_cast<T_return>(m_UTCR3) << std::endl;
#endif
      
      return static_cast<T_return>(m_UTCR3);
      
    case UartTraits<T_serial_port>::UTDR:
      {
        if (true == m_rx_fifo.empty())
        {
          assert(0 == (m_UTSR1 & UTSR1_RNE));
          
#if TRACE_UART
          g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                     << ">: "
                     << "WARNING: Get data from UART when its RX fifo is empty." << std::endl;
#endif
          
          return static_cast<T_return>(0);
        }
        else
        {
          assert(UTSR1_RNE == (m_UTSR1 & UTSR1_RNE));
          
#if TRACE_UART
          g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                     << ">: get UTDR: " << std::hex
                     << static_cast<T_return>(m_rx_fifo.front()) << std::endl;
#endif
          
          uint8_t const ch = m_rx_fifo.front();
          
          m_rx_fifo.pop_front();
          
          switch (m_rx_fifo.size())
          {
          case 0:
            m_UTSR0 &= ~(UTSR0_RID | UTSR0_RFS);
            
            m_UTSR1 &= ~UTSR1_RNE;
            break;
            
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
            m_UTSR0 &= ~UTSR0_RFS;
            break;
            
          case 8:
          case 9:
          case 10:
          case 11:
          case 12:
          default:
            break;
          }
          
#if TRACE_UART
          g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                     << ">: rx fifo size = "
                     << m_rx_fifo.size()
                     << ", UTSR0: "
                     << m_UTSR0 << std::endl;
#endif
          
          return static_cast<T_return>(ch);
        }
      }
      
    case UartTraits<T_serial_port>::UTSR0:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: get UTSR0: " << std::hex
                << static_cast<T_return>(m_UTSR0) << std::endl;
#endif
      
      return static_cast<T_return>(m_UTSR0);
      
    case UartTraits<T_serial_port>::UTSR1:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: get UTSR1: " << std::hex
                << static_cast<T_return>(m_UTSR1) << std::endl;
#endif
      
      // :NOTE: Wei 2004-Jan-03:
      //
      // Bit 6, 7 are reserved.
      //
      // However, this is a read-only register, thus I don't have to do the mask operation.
      return static_cast<T_return>(m_UTSR1);
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<typename T_serial_port>
  template<typename T_store>
  void
  Uart<T_serial_port>::put_data(uint32_t const address, T_store const value)
  {
    switch (address)
    {
    case UartTraits<T_serial_port>::UTCR0:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: put UTCR0: " << std::hex << value << std::endl;
#endif
      
      // :NOTE: Wei 2004-Jan-03:
      //
      // Bit 7 is reserved.
      m_UTCR0 = static_cast<uint8_t>(value & ~(1 << 7));
      break;
      
    case UartTraits<T_serial_port>::UTCR1:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: put UTCR1: " << std::hex << value << std::endl;
#endif
      
      // :NOTE: Wei 2004-Jan-03:
      //
      // Bit 4, 5, 6, 7 are reserved.
      m_UTCR1 = static_cast<uint8_t>(value & ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4)));
      break;
      
    case UartTraits<T_serial_port>::UTCR2:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                << ">: put UTCR2: " << std::hex << value << std::endl;
#endif
      
      m_UTCR2 = static_cast<uint8_t>(value);
      break;
      
    case UartTraits<T_serial_port>::UTCR3:
      {
#if TRACE_UART
        g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                  << ">: put UTCR3: " << std::hex << value << std::endl;
#endif
        
        uint8_t const diff = (m_UTCR3 ^ value);
        
        if ((diff & UTCR3_TXE) && (0 == (value & UTCR3_TXE)))
        {
          if (m_tx_fifo.size() != 0)
          {
            // :SA-1110 Developer's Manual: p.332: Wei 2003-Jun-29:
            //
            // If the TXE bit is cleared to zero, all entries within the transmit FIFO are reset.
            m_tx_fifo.clear();
            
            m_UTSR0 |= UTSR0_TFS; // Enable Transmit FIFO service request
            m_UTSR1 |= UTSR1_TNF; // Transmit FIFO not full
          }
        }
        
        if ((diff & UTCR3_RXE) && (0 == (value & UTCR3_RXE)))
        {
          // :SA-1110 Developer's Manual: p.332: Wei 2003-Jun-29:
          //
          // If the RXE bit is cleared to zero, all entries within the receive FIFO are reset
          if (false == m_rx_fifo.empty())
          {
            m_rx_fifo.clear();
            
            m_UTSR0 &= ~(UTSR0_RID | UTSR0_RFS);
            m_UTSR1 &= ~UTSR1_RNE;
          }
        }
        
        // :NOTE: Wei 2004-Jan-03:
        //
        // Bit 6, 7 are reserved.
        m_UTCR3 = static_cast<uint8_t>(value & ~((1 << 7) | (1 << 6)));
      }
      break;
      
    case UartTraits<T_serial_port>::UTDR:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                 << ">: put UTDR: " << static_cast<char>(value)
                 << " (" << std::hex << static_cast<uint32_t>(value) << ")" << std::endl;
#endif
      
      switch (m_tx_fifo.size())
      {
      case 0:
      case 1:
      case 2:
      case 3:
        break;
        
      case 4:
        m_UTSR0 &= ~UTSR0_TFS; // disable 'Transmit FIFO service request'
        break;
        
      case 5:
      case 6:
        break;        
        
      case 7:
        // :NOTE: Wei 2005-May-29:
        //
        // Because the ARMware has unlimited UART buffer,
        // thus I will never rise down the TNF (Transmit FIFO Not Full) bit.
        //
        // m_UTSR1 &= ~(Uart<T_serial_port>::UTSR1_TNF);
        break;
        
      case 8:
      default:
        break;
      }
      
      m_tx_fifo.push_back(static_cast<uint8_t>(value & 0xFF));
      break;
      
    case UartTraits<T_serial_port>::UTSR0:
#if TRACE_UART
      g_log_file << "UART<" << UartTraits<T_serial_port>::PORT_NUMBER
                 << ">: put UTSR0: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: p.336: Wei 2003-Jun-07:
      //
      // Writing a one to a sticky status bit clears it; writing a zero has no effect.
      // Read-only flags are set and cleared by hardware; writes have no effect.
      
      // :NOTE: Wei 2004-Jan-03:
      //
      // Bit 6, 7 are reserved.
      // bits 0, 1, 5 are read-only.
      m_UTSR0 &= ~(value & UTSR0_READ_WRITE_BITS);
      break;
      
    case UartTraits<T_serial_port>::UTSR1:
      // :NOTE: Wei 2004-Jan-01:
      //
      // read-only register.
      assert(!"Should not reach here.");
      return;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}
