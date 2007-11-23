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

namespace xmlpp
{
  class Node;
  class Element;
}

namespace ARMware
{
  typedef class DatabaseDep DatabaseDep;
  class DatabaseDep : public Database
  {
  private:
    
    // Attribute
    
    static std::string const KEY_DATABASE_ROOT_NAME;
    static std::string const KEY_MACHINE_NAME;
    static std::string const KEY_ROM_FILE;
    static std::string const KEY_SCREEN_WIDTH;
    static std::string const KEY_SCREEN_HEIGHT;
    
    // Operation
    
    std::string get_field_content(xmlpp::Element * const element);
    void parse_machine_record(xmlpp::Node * const xmlnode, MachineImpl * const machine_impl);
    void store_machine_record(xmlpp::Element * const root, MachineImpl const * const machine);
    
  public:
    
    // Life cycle
    
    DatabaseDep();
    ~DatabaseDep();
  };
}

#endif
