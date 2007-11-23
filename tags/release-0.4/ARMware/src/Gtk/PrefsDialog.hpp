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

#ifndef PrefsDialog_hpp
#define PrefsDialog_hpp

// System include
//

#include <gtkmm/dialog.h>

namespace Gtk
{
  class CheckButton;
}

namespace ARMware
{
  typedef class PrefsDialog PrefsDialog;
  class PrefsDialog : public Gtk::Dialog
  {
  private:
    
    // Attribute
    
    Gtk::CheckButton * const mp_serial_console_check;
    Gtk::CheckButton * const mp_machine_list_check;
    Gtk::CheckButton * const mp_softkey_check;
    
    // Signal handler
    
    void on_response(int response_id);
    bool on_key_press_event(GdkEventKey *event);
    
  public:
    
    // Life cycle
    
    PrefsDialog();
  };
}

#endif
