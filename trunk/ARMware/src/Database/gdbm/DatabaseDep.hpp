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

#ifndef DatabaseDep_hpp
#define DatabaseDep_hpp

// System include
//

#include <string>

// Local include
//

#include "../Database.hpp"

namespace ARMware
{
  typedef class DatabaseDep DatabaseDep;
  class DatabaseDep : public Database
  {
  private:
    
    // Attribute
    
    static std::string const KEY_MACHINE_COUNT;
    static std::string const KEY_MACHINE_NAME_LIST;
    static std::string const KEY_ROM_FILE;
    static std::string const KEY_SCREEN_WIDTH;
    static std::string const KEY_SCREEN_HEIGHT;
    
    std::string m_machine_name;
    
    // Operation
    
    template<typename T>
    T get_attribute(std::string const &key);
    
    template<typename T>
    void put_atrribute(T const &result, std::string const &key);
    
    void load_attributes(MachineImpl &machine);
    void store_attributes(MachineImpl const &machine);
    
  public:
    
    // Life cycle
    
    DatabaseDep();
    ~DatabaseDep();
  };
}

#endif
