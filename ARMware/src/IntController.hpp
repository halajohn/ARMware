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

#ifndef IntController_hpp
#define IntController_hpp

// System include
//

#include <cassert>

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Core;
  class GPIOControlBlock;
  class OsTimer;
  class RTC;
  class LCDController;
  
  class Serial_1;
  class Serial_2;
  class Serial_3;
  
  template<typename T_serial_port> class Uart;
  
  typedef class IntController IntController;
  class IntController
  {
  public:
    
    // Interrupt Bit Mask
    
    enum IntCtrlBitMaskEnum
    {
      GPIO_0_EDGE_BIT_MASK     = (1 <<  0),
      GPIO_1_EDGE_BIT_MASK     = (1 <<  1),
      GPIO_2_EDGE_BIT_MASK     = (1 <<  2),
      GPIO_3_EDGE_BIT_MASK     = (1 <<  3),
      GPIO_4_EDGE_BIT_MASK     = (1 <<  4),
      GPIO_5_EDGE_BIT_MASK     = (1 <<  5),
      GPIO_6_EDGE_BIT_MASK     = (1 <<  6),
      GPIO_7_EDGE_BIT_MASK     = (1 <<  7),
      GPIO_8_EDGE_BIT_MASK     = (1 <<  8),
      GPIO_9_EDGE_BIT_MASK     = (1 <<  9),
      GPIO_10_EDGE_BIT_MASK    = (1 << 10),
      
      GPIO_0_10_EDGE_BIT_MASK  = (GPIO_0_EDGE_BIT_MASK |
                                  GPIO_1_EDGE_BIT_MASK |
                                  GPIO_2_EDGE_BIT_MASK |
                                  GPIO_3_EDGE_BIT_MASK |
                                  GPIO_4_EDGE_BIT_MASK |
                                  GPIO_5_EDGE_BIT_MASK |
                                  GPIO_6_EDGE_BIT_MASK |
                                  GPIO_7_EDGE_BIT_MASK |
                                  GPIO_8_EDGE_BIT_MASK |
                                  GPIO_9_EDGE_BIT_MASK |
                                  GPIO_10_EDGE_BIT_MASK),
      
      GPIO_11_27_EDGE_BIT_MASK = (1 << 11),
      
      LCD_SERVICE_BIT_MASK     = (1 << 12),
      
      SERIAL_1_BIT_MASK        = (1 << 15),
      SERIAL_2_BIT_MASK        = (1 << 16),
      SERIAL_3_BIT_MASK        = (1 << 17),
      
      OS_TIMER_0_BIT_MASK      = (1 << 26),
      OS_TIMER_1_BIT_MASK      = (1 << 27),
      OS_TIMER_2_BIT_MASK      = (1 << 28),
      OS_TIMER_3_BIT_MASK      = (1 << 29),
      
      RTC_HZ_BIT_MASK          = (1 << 30),
      RTC_ALARM_BIT_MASK       = (1 << 31)
    };
    typedef enum IntCtrlBitMaskEnum IntCtrlBitMaskEnum;
    
  private:
    
    // Attribute
    
    Core * const mp_core;
    GPIOControlBlock * const mp_gpio;
    OsTimer * const mp_os_timer;
    RTC * const mp_rtc;
    LCDController * const mp_lcd_controller;
    
    Serial_1 * const mp_serial_1;
    Serial_2 * const mp_serial_2;
    Serial_3 * const mp_serial_3;
    
    // Register location
    
    enum
    {
      ICIP = 0x90050000,
      ICMR = 0x90050004,
      ICLR = 0x90050008,
      ICCR = 0x9005000C,
      ICFP = 0x90050010,
      ICPR = 0x90050020
    };
    
    // Register
    
    uint32_t m_ICIP;
    uint32_t m_ICMR;
    uint32_t m_ICLR;
    uint32_t m_ICCR;
    uint32_t m_ICFP;
    uint32_t m_ICPR;
        
    enum
    {
      ICCR_DIM_BITMASK = 0x1
    };
    
    // Operation
    
    void
    init_register()
    {
      m_ICIP = 0;
      m_ICMR = 0;
      m_ICLR = 0;
      m_ICCR = 0;
      m_ICFP = 0;
      m_ICPR = 0;
    }
    
    template<typename T_serial_port>
    void check_UART_interrupt(Uart<T_serial_port> const &uart);
    
  public:
    
    // Life cycle
    
    IntController(Core * const core,
                  GPIOControlBlock * const gpio,
                  OsTimer * const os_timer,
                  RTC * const rtc,
                  LCDController * const lcd_controller,
                  Serial_1 * const serial_1,
                  Serial_2 * const serial_2,
                  Serial_3 * const serial_3);
    
    // Operation
    
    void run();
    void reset();
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
    
    inline bool
    have_pending_irq() const
    {
      return (0 == m_ICIP) ? false : true;
    }
    
    inline bool
    have_pending_fiq() const
    {
      return (0 == m_ICFP) ? false : true;
    }
  };
  
  template<typename T_return>
  T_return
  IntController::get_data(uint32_t const /* address */) const
  {
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  template<>
  uint32_t FASTCALL IntController::get_data(uint32_t const address) const;
  
  template<typename T_store>
  void
  IntController::put_data(uint32_t const /* address */, T_store const /* value */)
  { assert(!"Should not reach here."); }
  
  template<>
  void FASTCALL IntController::put_data(uint32_t const address, uint32_t const value);
}

#endif
