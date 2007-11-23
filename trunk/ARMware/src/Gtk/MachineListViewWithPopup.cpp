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

#include <cassert>

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/messagedialog.h>
#include <gtkmm/treeselection.h>

// Local include
//

#include "MainWindow.hpp"
#include "MachineListViewWithPopup.hpp"
#include "NewMachineDialog.hpp"

#include "compose/ucompose.hpp"

#include "../Database/Database.hpp"

#if WIN32
// for gettext
#include <libintl.h>
#endif

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================== Operation ==================================
  
  int
  MachineListViewWithPopup::CurrentSelection()
  {
    Gtk::TreeModel::iterator iter = get_selection()->get_selected();
    
    if (false == iter)
    {
      // -1 means no selection now.
      return -1;
    }
    
    Gtk::TreeModel::Path path = get_model()->get_path(iter);
    
    assert(1 == path.get_depth()); // The path's depth of ListStore model is always 1.
    
    return path[0];
  }
  
  //============================= Signal handler ==============================
  
  bool
  MachineListViewWithPopup::on_button_press_event(GdkEventButton *event)
  {
    bool return_value = TreeView::on_button_press_event(event);
    
    if ((GDK_BUTTON_PRESS == event->type) && (3 == event->button))
    {
      Gtk::TreeModel::Path path;
      Gtk::TreeViewColumn *column;
      int cell_x, cell_y;
      
      // :NOTE: Wei 2004-Mar-16:
      //
      // Check to see whether I click a row or not.
      if (false == get_path_at_pos(static_cast<int>(event->x), static_cast<int>(event->y),
                                   path, column, cell_x, cell_y))
      {
        mpMenuPopupEdit->set_sensitive(false);
        mpMenuPopupDelete->set_sensitive(false);
      }
      else
      {
        mpMenuPopupEdit->set_sensitive(true);
        mpMenuPopupDelete->set_sensitive(true);
      }
      
      mMenuPopup.popup(event->button, event->time);
    }
    
    return return_value;
  }
  
  void
  MachineListViewWithPopup::OnMenuPopupNew()
  {
    NewMachineDialog dialog(gettext("Create a new machine"),
                            mParentWindow,
                            mpMachineListListStore,
                            mColumns,
                            true,
                            Gtk::TreeModel::iterator());
    
    dialog.run();
  }
  
  void
  MachineListViewWithPopup::OnMenuPopupEdit()
  {
    // :NOTE: Wei 2004-Mar-16:
    //
    // Because of the check in MachineListViewWithPopup::on_button_press_event(),
    // if I enter this signal handler, I must click on some row now.
    //
    // Thus I don't need to check the validation of the iterator.
    Gtk::TreeModel::iterator iter = get_selection()->get_selected();
    
    NewMachineDialog dialog(gettext("Edit an existed machine"),
                            mParentWindow,
                            mpMachineListListStore,
                            mColumns,
                            false,
                            iter);
    
    dialog.run();
  }
  
  void
  MachineListViewWithPopup::OnMenuPopupDelete()
  {
    // :NOTE: Wei 2004-Mar-16:
    //
    // Because of the check in MachineListViewWithPopup::on_button_press_event(),
    // if I enter this signal handler, I must click on some row now.
    //
    // Thus I don't need to check the validation of the iterator.
    Gtk::TreeModel::iterator iter = get_selection()->get_selected();
    Gtk::TreeModel::Row row = *iter;
    
    Gtk::MessageDialog dialog(mParentWindow,
                              String::ucompose(gettext("Do you want to delete %1"), row.get_value(mColumns.m_col_name)), false,
                              Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
    
    int const result = dialog.run();
    
    switch (result)
    {
    case Gtk::RESPONSE_OK:
      {
        Gtk::TreeModel::Path path = get_model()->get_path(iter);
        
        assert(1 == path.get_depth()); // The path's depth of ListStore model is always 1.
        
        int index = path[0];
        
        mpMachineListListStore->erase(iter);
        
        Database::get_database()->delete_machine(index);
      }
      break;
      
    case Gtk::RESPONSE_CANCEL:
      break;
      
    default:
      break;
    }
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //===================== Life cycle =============================
  
  MachineListViewWithPopup::MachineListViewWithPopup(Gtk::Window &parent)
    : mParentWindow(parent)
  {
    mpMachineListListStore = Gtk::ListStore::create(mColumns);
    set_model(mpMachineListListStore);
    
    uint32_t machine_count = Database::get_database()->machine_count();
    
    if (machine_count != 0)
    {
      for (uint32_t i = 0; i < machine_count; ++i)
      {
        Gtk::TreeModel::Row row = *(mpMachineListListStore->append());
        row[mColumns.m_col_name] = (*Database::get_database())[i].get_machine_name();
      }
    }
    
    append_column(gettext("Machine Name"), mColumns.m_col_name);
    
    Gtk::Menu::MenuList &menu_list = mMenuPopup.items();
    
    menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("_New"),
                                                    sigc::mem_fun(*this, &MachineListViewWithPopup::OnMenuPopupNew)));
    mpMenuPopupNew = &menu_list.back();
    
    menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("_Edit"),
                                                    sigc::mem_fun(*this, &MachineListViewWithPopup::OnMenuPopupEdit)));
    mpMenuPopupEdit = &menu_list.back();
    
    menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("_Delete"),
                                                    sigc::mem_fun(*this, &MachineListViewWithPopup::OnMenuPopupDelete)));
    mpMenuPopupDelete = &menu_list.back();
  }
  
  //============================== Operation ==================================
  
  MachineImpl *
  MachineListViewWithPopup::SelectedMachine()
  {
    return Database::get_database()->pick_machine(CurrentSelection());
  }
}
