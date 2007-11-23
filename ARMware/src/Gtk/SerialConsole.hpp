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

#ifndef SerialConsole_hpp
#define SerialConsole_hpp

// System include
//

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/textbuffer.h>
#include <gtkmm/dialog.h>

// Project include
//

#include "../ARMware.hpp"

namespace Gtk
{
  class Entry;
}

namespace ARMware
{
  typedef class SerialConsole SerialConsole;
  class SerialConsole : public Gtk::Dialog
  {
  private:
    
    // Attribute
    
    Glib::RefPtr<Gtk::TextBuffer> mp_textbuffer;
    Gtk::Entry * const mp_input_entry;
    Gtk::Label * const mp_input_label;
    Gtk::HBox * const mp_input_hbox;
    
    Glib::ustring m_input_buffer;
    Glib::ustring m_output_buffer;
    bool m_need_append_more;
    
    std::deque<uint8_t> *mp_rx_fifo;
    
    void on_input_entry_activate();
    
  public:
    
    // Life cycle
    
    SerialConsole();
    
    // Operation
    
    inline void
    register_rx_fifo(std::deque<uint8_t> *rx_fifo)
    {
      assert(rx_fifo != 0);
      
      mp_rx_fifo = rx_fifo;
    }
    
    inline void
    put_char_to_serial_console(std::vector<uint8_t> &tx_fifo)
    {
      // :NOTE: Wei 2004-Jun-10:
      //
      // Because Glib::ustring can only eat UTF-8 string,
      // and UTF-8 contains pure ASCII codes (i.e. 0~127).
      //
      // That is to say, extended ASCII codes are not included in the UTF-8 encoding.
      // Thus I have to check argument str to convert each non-pure-ASCII
      // characters to space characters (0x20).
      // Otherwise, when Glib::ustring outputs data into Gtk::TextBuffer,
      // a glibmm internal error will terminate ARMware ! (Oops)
      for (std::vector<uint8_t>::iterator iter = tx_fifo.begin();
           iter != tx_fifo.end();
           ++iter)
      {
        if ((*iter) & 0x80)
        {
          (*iter) = 0x20;
        }
      }
      
      if (0xd == tx_fifo.back())
      {
        m_output_buffer.append(reinterpret_cast<char const *>(&(tx_fifo[0])), tx_fifo.size());
        
        m_need_append_more = true;
      }
      else
      {
        if (true == m_need_append_more)
        {
          m_output_buffer.append(reinterpret_cast<char const *>(&(tx_fifo[0])), tx_fifo.size());
          
          m_need_append_more = false;
        }
        else
        {
          if (false == m_output_buffer.empty())
          {
            mp_textbuffer->insert(mp_textbuffer->end(), m_output_buffer);
            m_output_buffer.clear();
          }
          
          mp_textbuffer->insert(mp_textbuffer->end(), Glib::ustring(reinterpret_cast<char const *>(&(tx_fifo[0])), tx_fifo.size()));
        }
      }
    }
    
    inline void
    get_char_from_serial_console()
    {
      for (Glib::ustring::size_type i = 0;
           i != m_input_buffer.size();
           ++i)
      {
        mp_rx_fifo->push_back(m_input_buffer[i]);
      }
      
      m_input_buffer.clear();
    }
  };
}

#endif
