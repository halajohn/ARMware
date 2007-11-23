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

// Memory Management Unit
//

#include <cassert>

#include "Mmu.hpp"
#include "SysCoProc.hpp"
#include "AddressSpace.hpp"
#include "Exception.hpp"

#include "AddressSpace.tcc"

namespace ARMware
{
  ////////////////////////////////// Public ///////////////////////////////////
  //============================== Life cycle =================================
  
  Mmu::Mmu(Core * const core, SysCoProc * const sysCoProc, AddressSpace * const addressSpace)
    : mp_core(core),
      mpSysCoProc(sysCoProc),
      mpAddressSpace(addressSpace)
  { }
  
  ////////////////////////////////// Private //////////////////////////////////
  //================================ Operation ================================
  
  Mmu::Domain
  Mmu::check_domain(uint32_t const index) const
  {
    switch (index)
    {
    case  0: return static_cast<Domain>(mpSysCoProc->get_DAC_reg() & 0x3);
    case  1: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC) >> 2);
    case  2: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0x30) >> 4);
    case  3: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC0) >> 6);
    case  4: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0x300) >> 8);
    case  5: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC00) >> 10);
    case  6: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0x3000) >> 12);
    case  7: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC000) >> 14);
    case  8: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0x30000) >> 16);
    case  9: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC0000) >> 18);
    case 10: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0x300000) >> 20);
    case 11: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC00000) >> 22);
    case 12: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0x3000000) >> 24);
    case 13: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC000000) >> 26);
    case 14: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0x30000000) >> 28);
    case 15: return static_cast<Domain>((mpSysCoProc->get_DAC_reg() & 0xC0000000) >> 30);
    }
    
    assert(!"Should not reach here.");
    
    return NO_ACCESS;
  }  
}
