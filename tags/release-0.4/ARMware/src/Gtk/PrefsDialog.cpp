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

#include <gdk/gdkkeysyms.h>

#include <gtkmm/checkbutton.h>
#include <gtkmm/notebook.h>
#include <gtkmm/stock.h>
#include <gtkmm/frame.h>

#if WIN32
// for gettext
#include <libintl.h>
#endif

// Local include
//

#include "PrefsDialog.hpp"
#include "Layout.hpp"

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //========================== Signal handler =================================
  
  bool
  PrefsDialog::on_key_press_event(GdkEventKey *event)
  {
    if (GDK_Escape == event->keyval)
    {
      response(Gtk::RESPONSE_CANCEL);
      return true;
    }
    else
    {
      return Gtk::Dialog::on_key_press_event(event);
    }
  }
  
  void
  PrefsDialog::on_response(int response_id)
  {
    switch (response_id)
    {
    case Gtk::RESPONSE_OK:
      break;
      
    case Gtk::RESPONSE_APPLY:
      signal_response().emission_stop();
      break;
      
    case Gtk::RESPONSE_CANCEL:
      break;
    }
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  PrefsDialog::PrefsDialog()
    : mp_serial_console_check(Gtk::manage(new Gtk::CheckButton(gettext("Open serial console")))),
      mp_machine_list_check(Gtk::manage(new Gtk::CheckButton(gettext("Show machine list")))),
      mp_softkey_check(Gtk::manage(new Gtk::CheckButton(gettext("Show soft keyboard"))))
  {
    set_title(gettext("Preference"));
    
    Gtk::Notebook * const p_notebook = Gtk::manage(new Gtk::Notebook());
    
    p_notebook->set_border_width(Layout::BORDER_WIDTH);
    get_vbox()->pack_start(*p_notebook, Gtk::PACK_SHRINK);
    
    Gtk::VBox * const p_vbox_1 = Gtk::manage(new Gtk::VBox());
    
    p_vbox_1->set_border_width(Layout::BORDER_WIDTH);
    
    p_notebook->append_page(*p_vbox_1, gettext("Display"));
    
    Gtk::Frame * const p_frame = Gtk::manage(new Gtk::Frame(gettext("When ARMware starts...")));
    
    p_vbox_1->pack_start(*p_frame);
    
    Gtk::VBox * const p_vbox_2 = Gtk::manage(new Gtk::VBox());
    
    p_frame->add(*p_vbox_2);
    
    p_vbox_2->pack_start(*mp_serial_console_check);
    p_vbox_2->pack_start(*mp_machine_list_check);
    p_vbox_2->pack_start(*mp_softkey_check);
    
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_APPLY);
    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    
    set_default_response(Gtk::RESPONSE_CANCEL);
    
    // :NOTE: Wei 2004-Mar-19:
    //
    // I have to use connect_notify() to plug a 'response' signal handler _before_ the default one.
    signal_response().connect_notify(sigc::mem_fun(*this, &PrefsDialog::on_response));
    
    show_all_children();
  }
}
