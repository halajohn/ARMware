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

#ifndef ColorTableTraits_hpp
#define ColorTableTraits_hpp

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
#if CONFIG_MACH_H3600
  // :NOTE: Wei 2004-Jan-15:
  //
  // Althought the LCD controller of H3600 is programmed in 16-bit TFT mode,
  // but the TFT LCD uses the most highest 4 bit for each color(RGB).
  // That makes a total of 4096 colors.
  //
  // This 4-bit field is like an index into a color table.
  // Thus I convert each sub-pixel's value to the real color value through the
  // following color rable.
  
  template<int ShiftAmount>
  class ColorTableTraits
  {
  private:
    
    static uint32_t const mColorTable[16];
    
  public:
    
    // Inquiry method
    
    inline uint32_t
    operator[](int const index) const
    { return mColorTable[index]; }
  };
  
  template<int ShiftAmount>
  uint32_t const ColorTableTraits<ShiftAmount>::mColorTable[16] =
  {
    /* Level  0 */ 0x0,
    /* Level  1 */ ((256 / 16) * 1) << ShiftAmount,
    /* Level  2 */ ((256 / 16) * 2) << ShiftAmount,
    /* Level  3 */ ((256 / 16) * 3) << ShiftAmount,
    /* Level  4 */ ((256 / 16) * 4) << ShiftAmount,
    /* Level  5 */ ((256 / 16) * 5) << ShiftAmount,
    /* Level  6 */ ((256 / 16) * 6) << ShiftAmount,
    /* Level  7 */ ((256 / 16) * 7) << ShiftAmount,
    /* Level  8 */ ((256 / 16) * 8) << ShiftAmount,
    /* Level  9 */ ((256 / 16) * 9) << ShiftAmount,
    /* Level 10 */ ((256 / 16) * 10) << ShiftAmount,
    /* Level 11 */ ((256 / 16) * 11) << ShiftAmount,
    /* Level 12 */ ((256 / 16) * 12) << ShiftAmount,
    /* Level 13 */ ((256 / 16) * 13) << ShiftAmount,
    /* Level 14 */ ((256 / 16) * 14) << ShiftAmount,
    /* Level 15 */ (0xFF << ShiftAmount)
  };
  
  // :NOTE: Wei 2004-Apr-13:
  //
  // The reason why I create these 3 table is just because of preformance.
  // I don't what to preform the shift left operation at run time.
  extern ColorTableTraits<0> mRedTable;
  extern ColorTableTraits<8> mGreenTable;
  extern ColorTableTraits<16> mBlueTable;
#endif
}

#endif
