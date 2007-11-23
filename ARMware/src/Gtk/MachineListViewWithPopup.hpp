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

#ifndef MachineListViewWithPopup_hpp
#define MachineListViewWithPopup_hpp

// System include
//

#include <gtkmm/menu.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

// Local include
//

#include "../Database/Database.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class MachineImpl;
  
  typedef class MachineListViewWithPopup MachineListViewWithPopup;
  class MachineListViewWithPopup : public Gtk::TreeView
  {
  private:
    
    class MachineListModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
      
      MachineListModelColumns()
      {
        add(m_col_name);
      }
      
      Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };
    typedef class MachineListModelColumns MachineListModelColumns;
    
    // Friend
    
    friend class NewMachineDialog;
    friend class MainWindow;
    
    // Attribute
    
    Gtk::Window &mParentWindow;
    Glib::RefPtr<Gtk::ListStore> mpMachineListListStore;
    
    MachineListModelColumns mColumns;
    
    Gtk::Menu mMenuPopup;
    Gtk::MenuItem *mpMenuPopupNew;
    Gtk::MenuItem *mpMenuPopupEdit;
    Gtk::MenuItem *mpMenuPopupDelete;
    
    // Signal handler
    
    bool on_button_press_event(GdkEventButton *event);
    
    void OnMenuPopupNew();
    void OnMenuPopupEdit();
    void OnMenuPopupDelete();
    
    // Operation
    
    inline int CurrentSelection();
    
  public:
    
    // Life cycle
    
    MachineListViewWithPopup(Gtk::Window &parent);
    
    // Operation
    
    MachineImpl *SelectedMachine();
  };
}

#endif
