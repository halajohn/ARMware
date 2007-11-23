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

#if CONFIG_DATABASE_GDBM

#include <sstream>
#include <cassert>
#include <gdbm.h>

#include "DatabaseDep.hpp"

// :NOTE: Wei 2003-Nov-13:
//
// Because the gdbm_...() functions have a argument of GDBM_FILE type,
// and the object pointed by this pointer have to be at _file scope_
// rather than in namespace scope, thus I have to place this p_gdbm_database
// at file scope rather than at namespace scope.
//
// And the default linkage of a file scope variable is internal.
GDBM_FILE p_gdbm_database = 0;

namespace ARMware
{
  //////////////////////////////// Private ////////////////////////////////////
  //============================== Attribute ==================================
  
  std::string const DatabaseDep::KEY_MACHINE_COUNT = "machine_count";
  std::string const DatabaseDep::KEY_MACHINE_NAME_LIST = "machine_list";
  std::string const DatabaseDep::KEY_ROM_FILE = "rom_file";
  std::string const DatabaseDep::KEY_SCREEN_WIDTH = "screen_width";
  std::string const DatabaseDep::KEY_SCREEN_HEIGHT = "screen_height";
  
  //============================== Operation ==================================
  
  template<typename T>
  T
  DatabaseDep::get_attribute(std::string const &key)
  {
    T result;
    std::string attribute_string = m_machine_name + "." + key;
    
    datum key_data;
    
    key_data.dptr = const_cast<char *>(attribute_string.c_str());
    key_data.dsize = attribute_string.length() + 1;
    
    datum return_data;
    
    return_data = gdbm_fetch(p_gdbm_database, key_data);
    assert(return_data.dptr != 0);
    
    std::stringstream a;
    
    a << return_data.dptr;
    a >> result;
    
    free(return_data.dptr);
    
    return result;
  }
  
  template<typename T>
  void
  DatabaseDep::put_atrribute(T const &result, std::string const &key)
  {
    std::string attribute_string = m_machine_name + "." + key;
    
    datum key_data;
    
    key_data.dptr = const_cast<char *>(attribute_string.c_str());
    key_data.dsize = attribute_string.length() + 1;
    
    datum store_data;
    std::string store_string;
    std::stringstream a;
    
    a << result;
    a >> store_string;
    
    store_data.dptr = const_cast<char *>(store_string.c_str());
    store_data.dsize = store_string.length() + 1;
    
    gdbm_store(p_gdbm_database, key_data, store_data, GDBM_INSERT);
  }
  
  //////////////////////////////// Public /////////////////////////////////////
  //============================ Life cycle ===================================
  
  DatabaseDep::DatabaseDep()
  {
    p_gdbm_database = gdbm_open(const_cast<char *>(m_db_filename.c_str()), 0, GDBM_READER, 00664, 0);
    
    if (p_gdbm_database != 0)
    {
      int machine_count;
      
      datum key_data;
      
      key_data.dptr = const_cast<char *>(KEY_MACHINE_COUNT.c_str());
      key_data.dsize = KEY_MACHINE_COUNT.length() + 1;
      
      datum return_data = gdbm_fetch(p_gdbm_database, key_data);
      
      std::stringstream a;
      
      if (return_data.dptr != 0)
      {
        a << return_data.dptr;
        a >> machine_count;
        
        free(return_data.dptr);
      }
      else
      {
        machine_count = 0;
      }
      
      std::string index_string;
      std::string result_name;
      
      MachineImpl new_machine_impl;
      
      for (int i = 0; i < machine_count; ++i)
      {
        a.clear();
        a.str("");
        
        index_string.erase();
        result_name.erase();
        
        a << i;
        a >> index_string;
        
        result_name = KEY_MACHINE_NAME_LIST + index_string;
        
        key_data.dptr = const_cast<char *>(result_name.c_str());
        key_data.dsize = result_name.length() + 1;
        
        return_data = gdbm_fetch(p_gdbm_database, key_data);
        assert(return_data.dptr != 0);
        
        new_machine_impl.set_machine_name(return_data.dptr);
        load_attributes(new_machine_impl);
        
        m_list.push_back(new_machine_impl);
        
        free(return_data.dptr);
      }
      
      gdbm_close(p_gdbm_database);
    }
  }
  
  DatabaseDep::~DatabaseDep()
  {
    p_gdbm_database = gdbm_open(const_cast<char *>(m_db_filename.c_str()), 0, GDBM_NEWDB, 00664, 0);
    assert(p_gdbm_database != 0);
    
    datum key_data;
    
    key_data.dptr = const_cast<char *>(KEY_MACHINE_COUNT.c_str());
    key_data.dsize = KEY_MACHINE_COUNT.length() + 1;
    
    datum store_data;
    std::string store_string;
    std::stringstream a;
    
    a << m_list.size();
    a >> store_string;
    
    store_data.dptr = const_cast<char *>(store_string.c_str());
    store_data.dsize = store_string.length() + 1;
    
    gdbm_store(p_gdbm_database, key_data, store_data, GDBM_INSERT);
    
    std::deque<MachineImpl>::size_type size = m_list.size();
    std::string index_string;
    std::string result_name;
    
    for (std::deque<MachineImpl>::size_type i = 0; i != size; ++i)
    {
      a.clear();
      a.str("");
      
      index_string.erase();
      result_name.erase();
      
      a << i;
      a >> index_string;
      
      result_name = KEY_MACHINE_NAME_LIST + index_string;
      
      key_data.dptr = const_cast<char *>(result_name.c_str());
      key_data.dsize = result_name.length() + 1;
      
      store_data.dptr = const_cast<char *>((m_list[i].get_machine_name()).c_str());
      store_data.dsize = (m_list[i].get_machine_name()).length() + 1;
      
      gdbm_store(p_gdbm_database, key_data, store_data, GDBM_INSERT);
      
      store_attributes(m_list[i]);
    }
    
    gdbm_close(p_gdbm_database);
  }
  
  //============================== Operation ==================================
  
  void
  DatabaseDep::load_attributes(MachineImpl &machine)
  {
    m_machine_name = machine.get_machine_name();
    
    machine.set_rom_file(get_attribute<std::string>(KEY_ROM_FILE));
    machine.set_screen_width(get_attribute<uint32_t>(KEY_SCREEN_WIDTH));
    machine.set_screen_height(get_attribute<uint32_t>(KEY_SCREEN_HEIGHT));
  }
  
  void
  DatabaseDep::store_attributes(MachineImpl const &machine)
  {
    m_machine_name = machine.get_machine_name();
    
    put_atrribute(machine.get_rom_file(), KEY_ROM_FILE);
    put_atrribute(machine.get_screen_width(), KEY_SCREEN_WIDTH);
    put_atrribute(machine.get_screen_height(), KEY_SCREEN_HEIGHT);
  }
}

#endif
