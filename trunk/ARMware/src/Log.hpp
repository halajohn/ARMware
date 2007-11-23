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

#ifndef Log_hpp
#define Log_hpp

#include <cassert>
#include <streambuf>
#include <fstream>
#include <sstream>

#include "Type.hpp"

namespace ARMware
{
  extern std::ostream g_log_file;
  
  typedef class SplittedLogStreamBuf SplittedLogStreamBuf;
  class SplittedLogStreamBuf : public std::streambuf
  {
  private:
    
    static uint32_t const BUFFER_SIZE = 1024;
    char m_buffer[BUFFER_SIZE];
    
    uint32_t m_times;
    static uint32_t const MAX_OUT_TIMES = 1000000;
    
    static char const *BASENAME;
    
    std::stringstream m_converter;
    
    uint32_t m_out_file_number;
    std::ofstream m_out_file;
    
  public:
    
    inline
    SplittedLogStreamBuf()
      : m_times(MAX_OUT_TIMES),
        m_out_file_number(0)
    {
      setp(m_buffer, m_buffer + BUFFER_SIZE);
    }
    
    inline int
    sync()
    {
      if (MAX_OUT_TIMES == m_times)
      {
        m_converter.clear();
        m_converter.str("");
        
        m_converter << m_out_file_number;
        
        if (true == m_out_file.is_open())
        {
          m_out_file.close();
        }
        
        m_out_file.open((std::string(BASENAME) + m_converter.str()).c_str());
        
        ++m_out_file_number;
        m_times = 0;
      }
      
      assert(true == m_out_file.is_open());
      
      ++m_times;
      
      m_out_file.write(m_buffer, pptr() - m_buffer);
      m_out_file.flush();
      
      setp(m_buffer, m_buffer + BUFFER_SIZE);
      
      // :NOTE: Wei 2004-Nov-18:
      //
      // In case of success zero is returned, otherwise the function shall return -1.
      return 0;
    }
    
    inline int
    overflow(int c)
    {
      if (c != EOF)
      {
        assert(pptr() == epptr());
        
        sync();
        
        return sputc(c);
      }
      else
      {
        return 0;
      }
    }
  };
}

#endif
