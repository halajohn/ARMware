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

namespace ARMware
{
  template<typename T_return>
  inline T_return
  RomFile::read_from_rom(uint32_t const address) const
  {
    assert(address < MachineType::FLASH_SIZE);
    
#if LOAD_ROM_INTO_MEMORY
    return *(reinterpret_cast<T_return *>(&(mp_rom_data[address])));
#else
    T_return ret_value;
    
    m_file_stream.seekg(static_cast<std::fstream::pos_type>(address));
    
    if (false == m_file_stream.good())
    {
      if (true == m_file_stream.eof())
      {
        m_file_stream.clear();
        return 0;
      }
      else
      {
        assert(!"Should not reach here.");
      }
    }
    
    m_file_stream.read(reinterpret_cast<char *>(&ret_value), sizeof(T_return));
    
    if (false == m_file_stream.good())
    {
      if (true == m_file_stream.eof())
      {
        m_file_stream.clear();
        return 0;
      }
      else
      {
        assert(!"Should not reach here.");
      }
    }
    
    return ret_value;
#endif
  }
  
  template<typename T_store>
  inline void
  RomFile::write_to_rom(uint32_t const address, T_store const value)
  {
    assert(address < MachineType::FLASH_SIZE);
    
#if LOAD_ROM_INTO_MEMORY
    *(reinterpret_cast<T_store *>(&(mp_rom_data[address]))) = value;
#else
    m_file_stream.seekg(static_cast<std::fstream::pos_type>(address));
    
    if (false == m_file_stream.good())
    {
      if (true == m_file_stream.eof())
      {
        m_file_stream.clear();
        return;
      }
      else
      {
        assert(!"Should not reach here.");
      }
    }
    
    m_file_stream.write(reinterpret_cast<char const *>(&value), sizeof(T_store));
    
    if (false == m_file_stream.good())
    {
      if (true == m_file_stream.eof())
      {
        m_file_stream.clear();
        return;
      }
      else
      {
        assert(!"Should not reach here.");
      }
    }
#endif
  }
  
  template<typename T_return>
  inline T_return
  RomFile::get_data(uint32_t const address) const
  {
    assert(MachineType::FlashInfo::READ_ARRAY_MODE == mMode);
    
    return read_from_rom<T_return>(address);
  }
  
  template<typename T_store>
  inline void
  RomFile::put_data(uint32_t const /* address */, T_store const /* value */)
  {
#if ENABLE_INST_RECALL
    mp_core->DumpInstRecallBuffer();
#endif
    
    assert(!"Should not reach here.");
  }
}
