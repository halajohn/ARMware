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

#include "Core.hpp"
#include "Log.hpp"

namespace ARMware
{
  template<typename T_return>
  T_return
  Egpio::get_data(uint32_t const /* address */) const
  {
    // :NOTE: Wei 2004-Jan-11:
    //
    // According to the iPaq H3600 Hardware specification,
    // EGPIO is a write-only register.
    //
    // Althought it doesn't demonstrate what's value should it return when read,
    // however, when I examine the source code of familiar bootldr,
    // I think return 0 would be a good choise.
    return 0;
  }
  
  template<typename T_store>
  void
  Egpio::put_data(uint32_t const /* address */, T_store const value)
  {
#if TRACE_EGPIO
    g_log_file << "EGPIO: write: " << std::hex << value << ", orig: " << m_egpio << std::endl;
#endif
    
    // :NOTE: Wei 2004-Jan-11:
    //
    // Exclusive OR
    uint16_t diff = m_egpio ^ static_cast<uint16_t>(value);
    
    switch (diff)
    {
    case 0:
      // :NOTE: Wei 2004-Jan-11:
      //
      // No change, thus I just return.
      return;
      
    case EGPIO_VPEN:
      // :NOTE: Wei 2004-Jun-05:
      //
      // This signal is useful when we want to program or erase the flash ROM.
      break;
      
    case EGPIO_CARD_RESET:
    case EGPIO_OPT_RESET:
    case EGPIO_CODEC_RESET:
    case EGPIO_OPT_NVRAM_ON:
    case EGPIO_OPT_ON:
    case EGPIO_LCD_ON:
      g_log_file << std::hex << "diff value = " << diff << std::endl;
      assert(!"Should not reach here.");
      break;
      
    case EGPIO_RS232_ON:
      if (0 == (static_cast<uint16_t>(value) & EGPIO_RS232_ON))
      {
#if TRACE_EGPIO
        g_log_file << "EGPIO: disable rs232." << std::endl;
#endif
        
        // :NOTE: Wei 2004-Jan-14:
        //
        // If we want to disable the RS232.
        mp_core->disable_RS232();
      }
      else
      {
#if TRACE_EGPIO
        g_log_file << "EGPIO: enable rs232." << std::endl;
#endif
        
        mp_core->enable_RS232();
      }
      break;
      
    case EGPIO_QMUTE:
      // Mutes the onboard audio codec
      break;
      
    case EGPIO_LCD_PCI:
    case EGPIO_IR_ON:
    case EGPIO_AUD_ON:
    case EGPIO_AUD_PWD_ON:
    case EGPIO_IR_FSEL:
    case EGPIO_LCD_5V_ON:
    case EGPIO_LVDD_ON:
      g_log_file << std::hex << "diff value = " << diff << std::endl;
      assert(!"Should not reach here.");
      break;
      
    case (EGPIO_LCD_ON | EGPIO_LCD_PCI | EGPIO_LCD_5V_ON | EGPIO_LVDD_ON):
#if TRACE_EGPIO
      if (0 == (value & (EGPIO_LCD_ON | EGPIO_LCD_PCI | EGPIO_LCD_5V_ON | EGPIO_LVDD_ON)))
      {
        g_log_file << "EGPIO: disable LCD." << std::endl;
      }
      else
      {
        g_log_file << "EGPIO: enable LCD." << std::endl;
      }
#endif
      
      // :NOTE: Wei 2004-Jan-16:
      //
      // These 4 bits are relative to the power of the LCD,
      // the upper software layer may turn on these 4 bits in one instruction
      // (Ex: the bootldr used by familiar project does).
      //
      // Thus I treat these 4-bit values as a special case,
      // i.e. use a single switch-case instead of several if-then-else statements.
      break;
      
    default:
      g_log_file << std::hex << "diff value = " << diff << std::endl;
      assert(!"Should not reach here.");
      break;
    }
    
    m_egpio = static_cast<uint16_t>(value);
  }
}
