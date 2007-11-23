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

#include <sstream>
#include <string>

#include <cassert>

#include "MachineImpl.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================ Life cycle ===================================
  
  MachineImpl::MachineImpl()
    : m_machine_name(""),
      m_rom_file(""),
      m_screen_width(0),
      m_screen_height(0)
  {
  }
  
  MachineImpl::MachineImpl(std::string const &machineName)
    : m_machine_name(machineName),
      m_rom_file(""),
      m_screen_width(0),
      m_screen_height(0)
  {
  }
  
  //============================== Operator ===================================
  
  MachineImpl &
  MachineImpl::operator=(MachineImpl const &src)
  {
    m_machine_name = src.get_machine_name();
    m_rom_file = src.get_rom_file();
    m_screen_width = src.get_screen_width();
    m_screen_height = src.get_screen_height();
    
    return *this;
  }
}
