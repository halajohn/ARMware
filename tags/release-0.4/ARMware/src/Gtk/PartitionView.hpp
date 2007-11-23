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

#ifndef PartitionView_hpp
#define PartitionView_hpp

#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

#include "../Type.hpp"
#include "Unit.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class PartitionTable;
  
  typedef class PartitionView PartitionView;
  class PartitionView : public Gtk::TreeView
  {
  private:
    
    struct ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
      Gtk::TreeModelColumn<Glib::ustring> m_col_addr;
      Gtk::TreeModelColumn<double> m_col_size;
      Gtk::TreeModelColumn<Glib::ustring> m_col_unit;
      Gtk::TreeModelColumn<UnitEnum> m_col_unit_enum;
      Gtk::TreeModelColumn<Glib::ustring> m_col_file;
      
      ModelColumns()
      {
        add(m_col_addr);
        add(m_col_size);
        add(m_col_unit);
        add(m_col_unit_enum);
        add(m_col_file);
      }
    };
    
    ModelColumns m_model_columns;
    Glib::RefPtr<Gtk::ListStore> mp_partition_model;
    
    // Cell renderer data func
    
    void convert_int_to_string(Gtk::CellRenderer *cell, Gtk::TreeModel::iterator const &iter);
    
    // Signal
    
    sigc::signal0<void> m_signal_row_deleted;
    sigc::signal0<void> m_signal_row_unselected;
    sigc::signal4<void, Glib::ustring const &, double const, UnitEnum const &, Glib::ustring const &> m_signal_row_selected;
    
    // Operation
    
    void set_input_field(Gtk::TreeModel::Row &row,
                         Glib::ustring const &addr,
                         double const size,
                         UnitEnum const &unit,
                         Glib::ustring const &file);
    
  protected:
    
    // Signal handler
    
    virtual bool on_button_press_event(GdkEventButton *event);
    virtual void on_row_deleted(Gtk::TreeModel::Path const &);
    
  public:
    
    // Life cycle
    
    PartitionView();
    
    // Operation
    
    void append_row(Glib::ustring const &addr, double const size, UnitEnum const &unit, Glib::ustring const &file);
    void modify_row(Glib::ustring const &addr, double const size, UnitEnum const &unit, Glib::ustring const &file);
    void create_rom(Glib::ustring const &rom_file);
    void delete_selected_row();
    Gtk::TreePath check_size(uint32_t const rom_size) const;
    Glib::ustring get_image_name(Gtk::TreePath const &path);
    
    // Signal
    
    sigc::signal0<void> &
    signal_row_deleted()
    {
      return m_signal_row_deleted;
    }
    
    sigc::signal0<void> &
    signal_row_unselected()
    {
      return m_signal_row_unselected;
    }
    
    sigc::signal4<void, Glib::ustring const &, double const, UnitEnum const &, Glib::ustring const &> &
    signal_row_selected()
    {
      return m_signal_row_selected;
    }
  };
}

#endif
