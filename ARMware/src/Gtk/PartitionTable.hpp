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

#ifndef PartitionTable_hpp
#define PartitionTable_hpp

#include <gtkmm/box.h>

#include "../Type.hpp"
#include "PartitionView.hpp"

namespace Gtk
{
  class Entry;
  class Button;
  class SpinButton;
  class ComboBoxText;
}

namespace ARMware
{
  // Forward declaration
  //
  
  class AdvComboBoxText;
  
  typedef class PartitionTable PartitionTable;
  class PartitionTable : public Gtk::VBox
  {
  private:
    
    // Attribute
    
    Glib::ustring m_last_selected_file;
    
    PartitionView * mp_partition_view;
    
    Gtk::Entry * const mp_addr_entry;
    
    Gtk::Button     * const mp_image_button;
    Gtk::SpinButton * const mp_image_size_spinbutton;
    AdvComboBoxText * const mp_image_size_unit_combobox;
    
    Gtk::Button * const mp_create_button;
    Gtk::Button * const mp_add_button;
    Gtk::Button * const mp_delete_button;
    Gtk::Button * const mp_modify_button;
    
    Gtk::Button     * const mp_rom_button;
    Gtk::SpinButton * const mp_rom_size_spinbutton;
    AdvComboBoxText * const mp_rom_size_unit_combobox;
    
    // Operation
    
    bool check_input_field_valid();
    bool check_valid_hex_string(std::string const &str) const;
    void clear_input_field();
    
    // Signal handler
    
    void on_add_button_clicked();
    void on_modify_button_clicked();
    void on_delete_button_clicked();
    void on_image_button_clicked();
    void on_create_button_clicked();
    
    void on_row_deleted();
    void on_row_selected(Glib::ustring const &addr, double const size, UnitEnum const &unit, Glib::ustring const &file);
    void on_row_unselected();
    
  public:
    
    PartitionTable(Gtk::Button * const p_rom_button,
                   Gtk::SpinButton * const p_rom_size_spinbutton,
                   AdvComboBoxText * const p_rom_size_unit_combobox);
  };
}

#endif
