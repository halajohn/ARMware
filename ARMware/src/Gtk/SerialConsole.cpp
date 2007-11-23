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

// System include
//

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/stock.h>
#include <gtkmm/textview.h>
#include <gtkmm/entry.h>
#include <gtkmm/scrolledwindow.h>

#if WIN32
// for gettext
#include <libintl.h>
#endif

// Local include
//

#include "SerialConsole.hpp"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  // Private == Signal handler ================================================
  
  void
  SerialConsole::on_input_entry_activate()
  {
    m_input_buffer += mp_input_entry->get_text();
    
    // :NOTE: Wei 2005-May-29:
    //
    // When I press Enter in a Gtk::Entry,
    // means that I want to press Enter in the emulation environment, too.
    // Thus, I have to append this Carriage Return character (0xd) to the m_input_buffer.
    m_input_buffer += static_cast<char>(0xd);
    
    mp_input_entry->set_text(Glib::ustring());
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Public == Life cycle =====================================================
  
  SerialConsole::SerialConsole()
    : Gtk::Dialog(gettext("Serial Console")),
      mp_input_entry(Gtk::manage(new Gtk::Entry())),
      mp_input_label(Gtk::manage(new Gtk::Label(gettext("Input here: ")))),
      mp_input_hbox(Gtk::manage(new Gtk::HBox())),
      m_need_append_more(false)
  {
    mp_textbuffer = Gtk::TextBuffer::create();
    
    Gtk::TextView * const p_textview = Gtk::manage(new Gtk::TextView());
    p_textview->set_buffer(mp_textbuffer);
    p_textview->set_editable(false);
    
    Gtk::ScrolledWindow * const p_scrolledwindow = Gtk::manage(new Gtk::ScrolledWindow());
    
    p_scrolledwindow->add(*p_textview);
    p_scrolledwindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    
    get_vbox()->pack_start(*p_scrolledwindow);
    
    mp_input_hbox->pack_start(*mp_input_label, Gtk::PACK_SHRINK);
    mp_input_hbox->pack_start(*mp_input_entry, Gtk::PACK_EXPAND_WIDGET);
    
    get_vbox()->pack_start(*mp_input_hbox, Gtk::PACK_SHRINK);
    
    add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);
    set_default_response(Gtk::RESPONSE_CLOSE);
    
    mp_input_entry->signal_activate().connect(
      sigc::mem_fun(*this, &SerialConsole::on_input_entry_activate));
    
    show_all_children();
  }
}
