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

#ifndef Uart_hpp
#define Uart_hpp

// System include
//

#include <deque>

// Project include
//

#include "ARMware.hpp"

// Local include
//

namespace ARMware
{
  template<typename T_serial_port>
  class Uart
  {
  public:
    
    // Symbolic constant
    
    enum
    {
      UTCR3_RXE = (1 << 0), // Receiver enable.
      UTCR3_TXE = (1 << 1), // Transmitter enable.
      UTCR3_BRK = (1 << 2), // Break.
      UTCR3_RIE = (1 << 3), // Receive FIFO interrupt enable.
      UTCR3_TIE = (1 << 4), // Transmit FIFO interrupt enable.
      UTCR3_LBM = (1 << 5)  // Loopback mode.
    };
    
    enum
    {
      UTSR0_TFS = (1 << 0), // Transmit FIFO service request
      UTSR0_RFS = (1 << 1), // Receive FIFO service request
      UTSR0_RID = (1 << 2), // Receiver idle.
      UTSR0_RBB = (1 << 3), // Receiver begin of break.
      UTSR0_REB = (1 << 4), // Receiver end of break.
      UTSR0_EIF = (1 << 5), // Error in FIFO (read-only).
      
      UTSR0_READ_WRITE_BITS = (UTSR0_RID | UTSR0_RBB | UTSR0_REB),
    };
    
    enum
    {
      UTSR1_TBY = (1 << 0), // Transmitter busy flag
      UTSR1_RNE = (1 << 1), // Receive FIFO not empty
      UTSR1_TNF = (1 << 2), // Transmit FIFO not full
      UTSR1_PRE = (1 << 3), // Parity error
      UTSR1_FRE = (1 << 4), // Framing error
      UTSR1_ROR = (1 << 5)  // Receive FIFO overrun
    };
    
    static uint32_t const RX_FIFO_SIZE = 12;
    static uint32_t const TX_FIFO_SIZE = 8;
    
  private:
    
    // Attribute
    
    uint8_t m_UTCR0; // UART control register 0
    uint8_t m_UTCR1; // UART control register 1
    uint8_t m_UTCR2; // UART control register 2
    uint8_t m_UTCR3; // UART control register 3
    uint8_t m_UTDR;  // UART data register
    uint8_t m_UTSR0; // UART status register 0
    uint8_t m_UTSR1; // UART status register 1
    
    // :NOTE: Wei 2005-May-29:
    //
    // SerialConsole needs m_tx_fifo be in a continus memory space,
    // thus I use std::vector here.
    std::vector<uint8_t> m_tx_fifo;
    std::deque<uint8_t> m_rx_fifo;
    
    // :NOTE: Wei 2004-Mar-22:
    //
    // For now, this mp_output_device is only used in Uart<Serial_3> to output to SerialConsole.
    void *mp_output_device;
    
    // Operation
    
    void
    init_register()
    {
      m_UTCR0 = 0;
      m_UTCR1 = 0;
      m_UTCR2 = 0;
      m_UTCR3 = 0;
      m_UTDR = 0;
      
      m_UTSR0 = UTSR0_TFS; // Transmit FIFO service request
      m_UTSR1 = UTSR1_TNF; // Transmit FIFO not full
    }
    
    void tx_data();
    void rx_data();
    
  public:
    
    // Life cycle
    
    Uart();
    
    // Operation
    
    void reset();
    
    inline void
    register_output_device(void * const output_device)
    { mp_output_device = output_device; }
    
    void run();
    
    inline uint32_t
    get_interrupt_status() const
    { return m_UTSR0; }
    
    inline uint32_t
    get_ctrl_reg_3() const
    {
      return m_UTCR3;
    }
    
    // :SA-1110 Developer's Manual: Wei 2003-Dec-14:
    //
    // All registers in the Peripheral Control Module are accessed via
    // the CPU must be performed using word reads and writes.
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address);
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
    
    // Access
    
    inline std::deque<uint8_t> &
    rx_fifo()
    { return m_rx_fifo; }
  };
}

#endif
