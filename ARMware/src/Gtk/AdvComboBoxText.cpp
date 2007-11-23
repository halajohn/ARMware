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

#include "AdvComboBoxText.hpp"
#include "../Type.hpp"

namespace ARMware
{
  //////////////////////////////////// Public /////////////////////////////////
  //================================== Operation ==============================
  
  void
  AdvComboBoxText::set_active(Glib::ustring const &str)
  {
    Glib::RefPtr<Gtk::TreeModel> p_model = get_model();
    Gtk::TreeNodeChildren const &children = p_model->children();
    int i = 0;
    
    for (Gtk::TreeIter iter = children.begin(); iter != children.end(); ++iter)
    {
      if ((*iter)[m_text_columns.m_column] == str)
      {
        set_active(i);
        break;
      }
      
      ++i;
    }
  }
  
  Glib::ustring
  AdvComboBoxText::get_active_text() const
  {
    Glib::ustring result;
    
    uint32_t active_row = get_active_row_number();
    Glib::RefPtr<const Gtk::TreeModel> p_model = get_model();
    Gtk::TreeModel::Children const &children = p_model->children();
    
    if (active_row < children.size())
    {
      Gtk::TreeModel::Row row = children[active_row];
      
      result = row[m_text_columns.m_column];
    }
    
    return result;
  }
}
