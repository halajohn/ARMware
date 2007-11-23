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

#ifndef AdvComboBoxText_hpp
#define AdvComboBoxText_hpp

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/comboboxtext.h>

namespace ARMware
{
  typedef class AdvComboBoxText AdvComboBoxText;
  class AdvComboBoxText : public Gtk::ComboBoxText
  {
  public:
    
    // Operation
    
    using Gtk::ComboBoxText::set_active;
    
    void set_active(Glib::ustring const &str);
    Glib::ustring get_active_text() const;
  };
}

#endif
