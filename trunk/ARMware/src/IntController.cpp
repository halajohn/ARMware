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
#include "OsTimer.hpp"
#include "GPIOControlBlock.hpp"
#include "RTC.hpp"
#include "Core.hpp"
#include "Serial_1.hpp"
#include "Serial_2.hpp"
#include "Serial_3.hpp"
#include "LCDController.hpp"

#include "IntController.tcc"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Life cycle =================================
  
  IntController::IntController(Core * const core,
                               GPIOControlBlock * const gpio,
                               OsTimer * const os_timer,
                               RTC * const rtc,
                               LCDController * const lcd_controller,
                               Serial_1 * const serial_1,
                               Serial_2 * const serial_2,
                               Serial_3 * const serial_3)
    : mp_core(core),
      mp_gpio(gpio),
      mp_os_timer(os_timer),
      mp_rtc(rtc),
      mp_lcd_controller(lcd_controller),
      mp_serial_1(serial_1),
      mp_serial_2(serial_2),
      mp_serial_3(serial_3)
  {
    init_register();
  }
  
  //============================== Operation ==================================
  
  void
  IntController::reset()
  {
    m_ICIP = 0;
    
    // :NOTE: Wei 2004-Apr-25:
    //
    // Although the SA-1110 Developer's Manual doesn't specify whether ICFP will be
    // initialized to 0 when reset. However, SA-1110 Developer's Manual does specify
    // ICIP will be initialized to 0 when reset.
    // Thus I _assume_ this is a bug in the SA-1110 Developer's Manual, and will do
    // the initialization by my self.
    
    // :NOTE: Wei 2004-Jul-20;
    //
    // It is really a bug.
    // According to the Intel-StrongARM SA-1110 Microprocessor Specification Update
    // (December 2001), p.37:
    //
    // Changed the reset values for the Interrupt Controller IRQ and FIQ Pending
    // Registers (ICIP and ICFP) from undefined to 0.
    m_ICFP = 0;
    
    m_ICCR = 0;
    
    // :NOTE: Wei 2004-Jun-06:
    //
    // Althought there is no words in SA-1110 Developer's manual says that
    // ICPR will be set to 0 when reset.
    // However, I think it should set to 0 when reset to indicate that no
    // pending interrupt exist.
    m_ICPR = 0;
  }
  
  void
  IntController::run()
  {
    // check GPIO interrupt
    
    {
      uint32_t const gpio_status = mp_gpio->get_interrupt_status();
      
      if (gpio_status != 0)
      {
        m_ICPR &= ~GPIO_0_10_EDGE_BIT_MASK;
        m_ICPR |= (gpio_status & GPIO_0_10_EDGE_BIT_MASK);
        
        if ((gpio_status & ~GPIO_0_10_EDGE_BIT_MASK) != 0)
        {
          m_ICPR |= GPIO_11_27_EDGE_BIT_MASK;
        }
        else
        {
          m_ICPR &= ~GPIO_11_27_EDGE_BIT_MASK;
        }
      }
    }
    
    // check OS timer
    
    {
      uint32_t const os_timer_status = mp_os_timer->get_interrupt_status();
      
      switch (os_timer_status)
      {
      case 0: // 0000
        {
          m_ICPR &= ~(OS_TIMER_0_BIT_MASK |
                      OS_TIMER_1_BIT_MASK |
                      OS_TIMER_2_BIT_MASK |
                      OS_TIMER_3_BIT_MASK);
        }
        break;
        
      case OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK: // 0001
        {
          m_ICPR |= OS_TIMER_0_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_1_BIT_MASK |
                      OS_TIMER_2_BIT_MASK |
                      OS_TIMER_3_BIT_MASK);
        }
        break;
        
      case OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK: // 0010
        {
          m_ICPR |= OS_TIMER_1_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_0_BIT_MASK |
                      OS_TIMER_2_BIT_MASK |
                      OS_TIMER_3_BIT_MASK);
        }
        break;
        
      case (OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK): // 0011
        {
          m_ICPR |= OS_TIMER_0_BIT_MASK;
          m_ICPR |= OS_TIMER_1_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_2_BIT_MASK |
                      OS_TIMER_3_BIT_MASK);
        }
      break;
      
      case OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK: // 0100
        {
          m_ICPR |= OS_TIMER_2_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_0_BIT_MASK |
                      OS_TIMER_1_BIT_MASK |
                      OS_TIMER_3_BIT_MASK);
        }
        break;
        
      case (OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK): // 0101
        {
          m_ICPR |= OS_TIMER_0_BIT_MASK;
          m_ICPR |= OS_TIMER_2_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_1_BIT_MASK |
                      OS_TIMER_3_BIT_MASK);
        }
      break;
      
      case (OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK): // 0110
        {
          m_ICPR |= OS_TIMER_1_BIT_MASK;
          m_ICPR |= OS_TIMER_2_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_0_BIT_MASK |
                      OS_TIMER_3_BIT_MASK);
        }
      break;
        
      case (OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK): // 0111
        {
          m_ICPR |= OS_TIMER_0_BIT_MASK;
          m_ICPR |= OS_TIMER_1_BIT_MASK;
          m_ICPR |= OS_TIMER_2_BIT_MASK;
          
          m_ICPR &= ~OS_TIMER_3_BIT_MASK;
        }
      break;
        
      case OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK: // 1000
        {
          m_ICPR |= OS_TIMER_3_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_0_BIT_MASK |
                      OS_TIMER_1_BIT_MASK |
                      OS_TIMER_2_BIT_MASK);
        }
        break;
        
      case (OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK): // 1001
        {
          m_ICPR |= OS_TIMER_0_BIT_MASK;
          m_ICPR |= OS_TIMER_3_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_1_BIT_MASK |
                      OS_TIMER_2_BIT_MASK);
        }
      break;
      
      case (OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK): // 1010
        {
          m_ICPR |= OS_TIMER_1_BIT_MASK;
          m_ICPR |= OS_TIMER_3_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_0_BIT_MASK |
                      OS_TIMER_2_BIT_MASK);
        }
      break;
      
      case (OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK): // 1011
        {
          m_ICPR |= OS_TIMER_0_BIT_MASK;
          m_ICPR |= OS_TIMER_1_BIT_MASK;
          m_ICPR |= OS_TIMER_3_BIT_MASK;
          
          m_ICPR &= ~OS_TIMER_2_BIT_MASK;
        }
      break;
        
      case (OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK): // 1100
        {
          m_ICPR |= OS_TIMER_2_BIT_MASK;
          m_ICPR |= OS_TIMER_3_BIT_MASK;
          
          m_ICPR &= ~(OS_TIMER_0_BIT_MASK |
                      OS_TIMER_1_BIT_MASK);
        }
      break;
      
      case (OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK): // 1101
        {
          m_ICPR |= OS_TIMER_0_BIT_MASK;
          m_ICPR |= OS_TIMER_2_BIT_MASK;
          m_ICPR |= OS_TIMER_3_BIT_MASK;
          
          m_ICPR &= ~OS_TIMER_1_BIT_MASK;
        }
      break;
      
      case (OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK): // 1110
        {
          m_ICPR |= OS_TIMER_1_BIT_MASK;
          m_ICPR |= OS_TIMER_2_BIT_MASK;
          m_ICPR |= OS_TIMER_3_BIT_MASK;
          
          m_ICPR &= ~OS_TIMER_0_BIT_MASK;
        }
      break;
      
      case (OsTimer::OSMRTraits<OsTimer::OSMR0>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR1>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR2>::BITMASK |
            OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK): // 1111
        {
          m_ICPR |= (OS_TIMER_0_BIT_MASK |
                     OS_TIMER_1_BIT_MASK |
                     OS_TIMER_2_BIT_MASK |
                     OS_TIMER_3_BIT_MASK);
        }
      break;
      
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    // check RTC
    
    {
      uint32_t const rtc_status = mp_rtc->get_interrupt_status();
      
      switch (rtc_status)
      {
      case 0:
        m_ICPR &= ~(RTC_ALARM_BIT_MASK | RTC_HZ_BIT_MASK);
        break;
        
      case RTC::STATUS_AL_BIT:
        m_ICPR |= RTC_ALARM_BIT_MASK;
        m_ICPR &= ~RTC_HZ_BIT_MASK;
        break;
        
      case RTC::STATUS_HZ_BIT:
        m_ICPR |= RTC_HZ_BIT_MASK;
        m_ICPR &= ~RTC_ALARM_BIT_MASK;
        break;
        
      case (RTC::STATUS_AL_BIT | RTC::STATUS_HZ_BIT):
        m_ICPR |= (RTC_ALARM_BIT_MASK | RTC_HZ_BIT_MASK);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    // check LCD controller
    
    {
      uint32_t const lcd_status = mp_lcd_controller->get_interrupt_status();
      
      if (0 == lcd_status)
      {
        m_ICPR &= ~LCD_SERVICE_BIT_MASK;
      }
      else
      {
        if ((lcd_status & ~(LCDController::LCSR_BAU | LCDController::LCSR_LDD)) != 0)
        {
          m_ICPR |= LCD_SERVICE_BIT_MASK;
        }
        else
        {
          switch (lcd_status & (LCDController::LCSR_BAU | LCDController::LCSR_LDD))
          {
          case 0:
            // If we go here, means (lcd_status == 0), however, (lcd_status == 0) should
            // go to the upper 'if' block, thus if we go here, it is a bug.
            assert(!"Should not reach here.");
            break;
            
          case LCDController::LCSR_BAU:
            {
              uint32_t const ctrl_0 = mp_lcd_controller->get_ctrl_reg_0();
              
              if (0 == (ctrl_0 & LCDController::LCCR0_BAM))
              {
                // Enable BAU
                m_ICPR |= LCD_SERVICE_BIT_MASK;
              }
              else
              {
                m_ICPR &= ~LCD_SERVICE_BIT_MASK;
              }
            }
            break;
            
          case LCDController::LCSR_LDD:
            {
              uint32_t const ctrl_0 = mp_lcd_controller->get_ctrl_reg_0();
              
              if (0 == (ctrl_0 & LCDController::LCCR0_LDM))
              {
                // Enable LDD
                m_ICPR |= LCD_SERVICE_BIT_MASK;
              }
              else
              {
                m_ICPR &= ~LCD_SERVICE_BIT_MASK;
              }
            }
            break;
            
          case (LCDController::LCSR_BAU | LCDController::LCSR_LDD):
            {
              uint32_t const ctrl_0 = mp_lcd_controller->get_ctrl_reg_0();
              
              switch (ctrl_0 & (LCDController::LCCR0_BAM | LCDController::LCCR0_LDM))
              {
              case 0:
              case LCDController::LCCR0_BAM:
              case LCDController::LCCR0_LDM:
                // Enable BAU
                m_ICPR |= LCD_SERVICE_BIT_MASK;
                break;
                
              case (LCDController::LCCR0_BAM | LCDController::LCCR0_LDM):
                m_ICPR &= ~LCD_SERVICE_BIT_MASK;
                break;
                
              default:
                assert(!"Should not reach here.");
                break;
              }
            }
            break;
          }
        }
      }
    }
    
    // Check UART in the serial port 1, 2 & 3
    check_UART_interrupt(mp_serial_1->get_UART());
    check_UART_interrupt(mp_serial_2->get_UART());
    check_UART_interrupt(mp_serial_3->get_UART());
    
    // Determine IRQ & FIQ
    
    if (m_ICPR != 0)
    {
#if TRACE_INTERRUPT
      //g_log_file << "signal pending = " << std::hex << m_ICPR << std::endl;
#endif
      
      uint32_t const temp = m_ICPR & m_ICMR;
      
      m_ICIP = temp & (~m_ICLR);
      m_ICFP = temp & m_ICLR;
      
      if (true == mp_core->is_in_idle_mode())
      {
        // We have at least one interrupt.
        if (0 == (m_ICCR & ICCR_DIM_BITMASK))
        {
          // :SA-1110 Developer's Manual: p.89: Wei 2004-May-09:
          //
          // All enabled interrupts will bring the SA-1110 out of idle mode.
          mp_core->exit_idle_mode();
        }
        else
        {
          if (temp != 0)
          {
            // :SA-1110 Developer's Manual: p.89: Wei 2004-May-09:
            //
            // Only enabled and unmasked (as defined in the ICMR) will bring
            // the SA-1110 out of idle mode.
            mp_core->exit_idle_mode();
          }
        }
      }
    }
    else
    {
      // Optimize the condition of no pending interrupt.
      
      // m_ICPR == 0: We have no pending interrupt.
      m_ICIP = 0;
      m_ICFP = 0;
    }
  }
  
  template<>
  uint32_t
  IntController::get_data(uint32_t const address) const
  {
    switch (address)
    {
    case ICIP: return m_ICIP;
    case ICMR: return m_ICMR;
    case ICLR: return m_ICLR;
    case ICCR: return m_ICCR;
    case ICFP: return m_ICFP;
    case ICPR: return m_ICPR;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<>
  void
  IntController::put_data(uint32_t const address, uint32_t const value)
  {
    switch (address)
    {
    case ICIP:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-09:
      //
      // ICIP is a read-only register
      assert(!"Should not reach here.");
      break;
      
    case ICMR:
#if TRACE_INTERRUPT
      g_log_file << "INTR: signal mask = " << std::hex << value << std::endl;
#endif
      
      m_ICMR = value;
      break;
      
    case ICLR:
#if TRACE_INTERRUPT
      g_log_file << "INTR: signal level = " << std::hex << value << std::endl;
#endif
      
      m_ICLR = value;
      break;
      
    case ICCR:
      // :SA-1110 Developer's Manual: p.89: Wei 2004-May-09:
      //
      // bits[31:1] are reserved.
      m_ICCR = (value & 0x1);
      break;
      
    case ICFP:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-09:
      //
      // ICFP is a read-only register
      assert(!"Should not reach here.");
      break;
      
    case ICPR:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-09:
      //
      // ICPR is a read-only register
      assert(!"Should not reach here.");
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}
