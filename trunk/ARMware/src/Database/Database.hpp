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

#ifndef Database_hpp
#define Database_hpp

#include <deque>
#include <string>

#include "../Type.hpp"
#include "MachineImpl.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class DatabaseDep;
  
  typedef class Database Database;
  class Database
  {
  private:
    
    void create_dirs_with_parent(std::string const &dir) const;
    static Database * msp_database;
    
  protected:
    
    std::string m_db_filename;
    std::deque<MachineImpl> m_list;
    
  public:
    
    // Life cycle
    
    Database();
    virtual ~Database() = 0;
    
    // Operation
    
    static Database *get_database();
    static void put_database();
    
    MachineImpl &new_machine(std::string const machineName);
    MachineImpl *pick_machine(int32_t const index);
    
    void delete_machine(uint32_t const index);
    
    inline MachineImpl &
    operator[](int const index)
    {
      return m_list[index];
    }
    
    inline MachineImpl
    operator[](int const index) const
    {
      return m_list[index];
    }
    
    uint32_t
    machine_count() const
    {
      return m_list.size();
    }
    
    // Inquiry method
    
    std::string
    machine_name(uint32_t const index) const
    {
      return const_cast<MachineImpl &>(m_list[index]).get_machine_name();
    }
  };
}

#endif
