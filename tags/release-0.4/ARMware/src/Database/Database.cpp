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

#include <sys/stat.h>
#include <cassert>
#include <fstream>

#include "Database.hpp"
#include "../Exception.hpp"
#include "../Utils/File.hpp"
#include "../platform_dep.hpp"

#if CONFIG_DATABASE_GDBM
#include "gdbm/DatabaseDep.hpp"
#endif
#if CONFIG_DATABASE_XML
#include "xml/DatabaseDep.hpp"
#endif

namespace ARMware
{
  Database *Database::msp_database = 0;
  
  Database *
  Database::get_database()
  {
    if (0 == msp_database)
    {
      msp_database = new DatabaseDep();
    }
    
    return msp_database;
  }
  
  void
  Database::put_database()
  {
    if (msp_database != 0)
    {
      delete msp_database;
      msp_database = 0; // MainWindow's Destructor depends on this behavior.
    }
  }
  
  //////////////////////////////// Private ////////////////////////////////////
  //============================= Operation ===================================
  
  void
  Database::create_dirs_with_parent(std::string const &dir) const
  {
    if (ERROR_FILE_NONEXIST == check_file_error(dir.c_str()))
    {
      std::string upper_dir = dir.substr(0, dir.find_last_of('/'));
      
      create_dirs_with_parent(upper_dir);
      
      mkdir(dir.c_str(), S_IRWXU);
    }
  }
  
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Life cycle =================================
  
  Database::Database()
  {
    std::string home_dir(getenv("HOME"));
    
    if (home_dir[home_dir.length() - 1] != '/')
    {
      home_dir += '/';
    }
    
    m_db_filename = home_dir + INSTALL_DB_NAME;
    
    create_dirs_with_parent(m_db_filename.substr(0, m_db_filename.find_last_of('/')));
    
    switch (check_file_error(m_db_filename.c_str()))
    {
    case NO_ERROR:
      break;
      
    case ERROR_FILE_NONEXIST:
      {
        std::fstream db_stream(m_db_filename.c_str(), std::ios::out); // create if nonexist
        db_stream.close();
      }
      break;
      
    default:
      assert(!"Should not reach here."); // I don't handle other conditions yet.
      break;
    }
  }
  
  Database::~Database()
  {
  }
  
  //============================== Operation ==================================
  
  MachineImpl &
  Database::new_machine(std::string const machine_name)
  {
    std::deque<MachineImpl>::size_type size = m_list.size();
    
    for (std::deque<MachineImpl>::size_type i = 0; i != size; ++i)
    {
      if (m_list[i].get_machine_name() == machine_name)
      {
        throw SameMachineNameException();
      }
    }
    
    MachineImpl new_machine_impl(machine_name);
    m_list.push_back(new_machine_impl);
    
    return m_list[m_list.size() - 1];
  }
  
  void
  Database::delete_machine(uint32_t const index)
  {
    std::deque<MachineImpl>::iterator itr = m_list.erase(m_list.begin() + index);
  }
  
  MachineImpl *
  Database::pick_machine(int32_t const index)
  {
    if (-1 == index)
    {
      // -1 means I don't select any machine now.
      return 0;
    }
    else
    {
      return &(m_list[index]);
    }
  }
}
