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

#ifndef Egpio_hpp
#define Egpio_hpp

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
  
  typedef class Egpio Egpio;
  class Egpio
  {
  private:
    
    // Pin definition
    
    enum
    {
      EGPIO_VPEN         = (1 << 0),  // Enables programming and erasing of Flash
      EGPIO_CARD_RESET   = (1 << 1),  // CF/PCMCIA card reset signal
      EGPIO_OPT_RESET    = (1 << 2),  // Expansion pack reset signal for functions other than CF/PCMCIA
      EGPIO_CODEC_RESET  = (1 << 3),  // onboard codec reset signal
      EGPIO_OPT_NVRAM_ON = (1 << 4),  // Enables power supply to the NVRAM of the expansion pack
      EGPIO_OPT_ON       = (1 << 5),  // Enables full power to the expansion pack
      EGPIO_LCD_ON       = (1 << 6),  // Enables LCD 3.3V power supply
      EGPIO_RS232_ON     = (1 << 7),  // Enables RS232 transceiver
      EGPIO_LCD_PCI      = (1 << 8),  // Enables power to LCD control IC
      EGPIO_IR_ON        = (1 << 9),  // Enables power to IR module
      EGPIO_AUD_ON       = (1 << 10), // Enables power to audio output amplifier
      EGPIO_AUD_PWD_ON   = (1 << 11), // Enables power to all audio circuitry besides the audio output amplifier
      EGPIO_QMUTE        = (1 << 12), // Mutes the onboard audio codec
      EGPIO_IR_FSEL      = (1 << 13), // FIR mode selection: H=FIR, L=SIR
      EGPIO_LCD_5V_ON    = (1 << 14), // Enables 5V to the LCD module
      EGPIO_LVDD_ON      = (1 << 15)  // Enables 9V and -6.5V to the LCD module
    };
    
    // Attribute
    
    uint16_t m_egpio;
    
    Core * const mp_core;
    
    // Operation
    
    void
    init_register()
    {
      m_egpio = 0;
    }
    
  public:
    
    // Memory address
    
#if CONFIG_MACH_H3600
    enum
    {
      EGPIO_ADDRESS = 0x49000000
    };
#endif
    
    // Life cycle
    
    Egpio(Core * const core);
    
    // Flash VPen Operation
    
    inline void
    EnableFlashVPen()
    { m_egpio |= EGPIO_VPEN; }
    
    inline void
    DisableFlashVPen()
    { m_egpio &= ~EGPIO_VPEN; }
    
    inline bool
    FlashVPenIsEnabled() const
    { return (EGPIO_VPEN == (m_egpio & EGPIO_VPEN)) ? true : false; }
    
    // Operation
    
    void reset();
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
}

#endif
