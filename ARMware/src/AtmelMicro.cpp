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

#include <cassert>
#include <iostream>

#include "Log.hpp"
#include "AtmelMicro.hpp"
#include "Serial_1.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================= Operation ===================================
  
  void
  AtmelMicro::get_data_from_serial_port(std::vector<uint8_t> const &fifo)
  {
    for (std::vector<uint8_t>::const_iterator iter = fifo.begin();
         iter != fifo.end();
         ++iter)
    {
      switch (m_get_state)
      {
      case STATE_SOF:
        assert(SOF == (*iter));
        
        m_get_state = STATE_ID_LEN;
        break;
        
      case STATE_ID_LEN:
#if CHECK_AMTEL_CHKSUM
        m_chk = (*iter);
#endif
        
        m_id = ((*iter) & 0xF0);
        m_len = ((*iter) & 0x0F);
        
        if (0 == m_len)
        {
          m_get_state = STATE_CHK;
        }
        else
        {
          m_get_state = STATE_DATA;
        }
        break;
        
      case STATE_DATA:
#if CHECK_AMTEL_CHKSUM
        m_chk += (*iter);
#endif
        
        m_data[m_read] = (*iter);
        
        ++m_read;
        
        if (m_read == m_len)
        {
          m_get_state = STATE_CHK;
          m_read = 0;
        }
        break;
        
      case STATE_CHK:
#if CHECK_AMTEL_CHKSUM
        assert(m_chk == (*iter));
#endif
        
        switch (m_id)
        {
        case MSG_VERSION:
          // :NOTE: Wei 2004-Jun-29:
          //
          // Ok... I really don't know how should I response the version request.
          // After digging the linux kernel source code:
          // 2.4.19-rmk6-pxa1-hh30/arch/arm/mach-sa1100::h3600_micro_version_ack()
          // I decide return a 4-byte data of {0, 0, 0, 0}
          m_response.push_back(SOF);
          m_response.push_back(MSG_VERSION | MSG_VERSION_GET_RESPONSE_LEN);
          m_response.push_back(0);
          m_response.push_back(0);
          m_response.push_back(0);
          m_response.push_back(0);
          m_response.push_back(MSG_VERSION | MSG_VERSION_GET_RESPONSE_LEN);
          break;
          
        case MSG_BACKLIGHT:
          switch (m_data[0])
          {
          case FLITE_AUTO_MODE:
          case FLITE_MANUAL_MODE:
            // Set command
            
            // :NOTE: Wei 2004-Jun-30:
            //
            // m_data[1] is whether backlight is on or not.
            //
            // m_data[1] == 0: power off
            // m_data[1] == 1: power on
            m_brightness = m_data[2];
            
            m_response.push_back(SOF);
            m_response.push_back(MSG_BACKLIGHT | MSG_BACKLIGHT_SET_RESPONSE_LEN);
            m_response.push_back(MSG_BACKLIGHT | MSG_BACKLIGHT_SET_RESPONSE_LEN); // ack
            break;
            
          case FLITE_GET_LIGHT_SENSOR:
            // Get command
            
            m_response.push_back(SOF);
            m_response.push_back(MSG_BACKLIGHT | MSG_BACKLIGHT_GET_RESPONSE_LEN);
            m_response.push_back(m_brightness);
            
            // :NOTE: Wei 2004-Jul-1:
            //
            // This a modulo 256 checksum. It does not incluse the SOF.
            // (modulo 256 == 'AND' 255)
            m_response.push_back(((MSG_BACKLIGHT | MSG_BACKLIGHT_GET_RESPONSE_LEN) + m_brightness) & 0xFF); // ack
            break;
            
          default:
            g_log_file << m_data[1] << std::endl;
            assert(!"Should not reach here.");
            break;
          }
          break;
          
        default:
          g_log_file << m_id << std::endl;
          assert(!"Should not reach here.");
          break;
        }
        
        m_get_state = STATE_SOF;
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
  }
  
  void
  AtmelMicro::put_data_to_serial_port(std::deque<uint8_t> &rx_fifo)
  {
    if (0 == m_response.size())
    {
      return;
    }
    else
    {
      while (m_response.size() != 0)
      {
        rx_fifo.push_back(m_response.front());
        
        m_response.pop_front();
        
        if (Uart<Serial_1>::RX_FIFO_SIZE == rx_fifo.size())
        {
          // the Uart RX fifo is full.
          break;
        }
      }
      
      return;
    }
  }
}
