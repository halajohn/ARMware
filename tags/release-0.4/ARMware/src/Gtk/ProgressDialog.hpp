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

#ifndef ProgressWindow_hpp
#define ProgressWindow_hpp

#include <gtkmm/dialog.h>

namespace Gtk
{
  class ProgressBar;
}

namespace ARMware
{
  typedef class ProgressWindow ProgressWindow;
  class ProgressDialog : public Gtk::Dialog
  {
  private:
    
    bool m_cancel;
    Gtk::ProgressBar * const mp_progress_bar;
    
    // Signal handler
    
    void cancel_button_clicked();
    
  public:
    
    // Life cycle
    
    ProgressDialog(std::string const &title, Gtk::Window &parent);
    
    // Operation
    
    void set_fraction(double const fraction);
    void set_text(Glib::ustring const &text);
    
    bool
    is_cancel()
    {
      return m_cancel;
    }
  };
}

#endif
