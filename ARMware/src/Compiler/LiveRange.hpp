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

#ifndef LiveRange_hpp
#define LiveRange_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  typedef struct LiveRange LiveRange;
  struct LiveRange
  {
    uint32_t m_start;
    uint32_t m_end;
    
    // Life cycle
    
    LiveRange(uint32_t const start, uint32_t const end)
      : m_start(start),
        m_end(end)
    { }
    
    // Operator
    
    inline bool
    operator<(LiveRange const &pair) const
    {
      return (m_start < pair.m_start) ? true : false;
    }
  };
}

#endif

#endif
