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

#ifndef MachineImpl_hpp
#define MachineImpl_hpp

// System include
//

#include <string>

// Project include
//

#include "../ARMware.hpp"

namespace ARMware
{
  typedef class MachineImpl MachineImpl;
  class MachineImpl
  {
  private:
    
    // Attribute
    
    std::string m_machine_name;
    std::string m_rom_file;
    uint32_t m_screen_width;
    uint32_t m_screen_height;
    
  public:
    
    // Life cycle
    
    MachineImpl();
    explicit MachineImpl(std::string const &machineName);
    
    // Operator
    
    MachineImpl &operator=(MachineImpl const &src);
    
    // Inquiry method
    
    inline std::string const
    get_machine_name() const
    {
      return m_machine_name;
    }
    
    inline std::string const
    get_rom_file() const
    {
      return m_rom_file;
    }
    
    inline uint32_t
    get_screen_width() const
    {
      return m_screen_width;
    }
    
    inline uint32_t
    get_screen_height() const
    {
      return m_screen_height;
    }
    
    // Access method
    
    inline void
    set_machine_name(std::string const &machine_name)
    {
      m_machine_name = machine_name;
    }
    
    inline void
    set_rom_file(std::string const &filename)
    {
      m_rom_file = filename;
    }
    
    inline void
    set_screen_width(uint32_t const &width)
    {
      m_screen_width = width;
    }
    
    inline void
    set_screen_height(uint32_t const &height)
    {
      m_screen_height = height;
    }
  };
}

#endif
