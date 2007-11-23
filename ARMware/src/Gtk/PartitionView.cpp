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

#include <fstream>
#include <sstream>

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/main.h>
#include <gtkmm/messagedialog.h>

#if WIN32
// for gettext
#include <libintl.h>
#endif

#include "PartitionView.hpp"
#include "ProgressDialog.hpp"
#include "compose/ucompose.hpp"

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //======================= Cell renderer data func ===========================
  
  void
  PartitionView::convert_int_to_string(Gtk::CellRenderer *cell, Gtk::TreeModel::iterator const &iter)
  {
    Gtk::TreeModel::Row row = *iter;
    
    std::stringstream s;
    std::string str;
    
    s << static_cast<uint32_t>(row[m_model_columns.m_col_size]);
    s >> str;
    
    static_cast<Gtk::CellRendererText *>(cell)->property_text() = str;
  }
  
  ///////////////////////////////// Protected /////////////////////////////////
  //============================== Signal handler =============================
  
  bool
  PartitionView::on_button_press_event(GdkEventButton * const event)
  {
    bool return_value = TreeView::on_button_press_event(event);
    
    if ((GDK_BUTTON_PRESS == event->type) && (1 == event->button))
    {
      Gtk::TreeModel::Path path;
      Gtk::TreeViewColumn *column;
      int cell_x, cell_y;
      
      if (false == get_path_at_pos(static_cast<int>(event->x),
                                   static_cast<int>(event->y),
                                   path, column, cell_x, cell_y))
      {
        get_selection()->unselect_all();
        
        m_signal_row_unselected.emit();
      }
      else
      {
        Gtk::TreeModel::iterator iter = get_selection()->get_selected();
        Gtk::TreeModel::Row row = *iter;
        
        m_signal_row_selected.emit(row[m_model_columns.m_col_addr],
                                   row[m_model_columns.m_col_size],
                                   row[m_model_columns.m_col_unit_enum],
                                   row[m_model_columns.m_col_file]);
      }
    }
    
    return return_value;
  }
  
  void
  PartitionView::on_row_deleted(Gtk::TreeModel::Path const &)
  {
    m_signal_row_deleted.emit();
  }
  
  //================================ Operation ================================
  
  void
  PartitionView::set_input_field(Gtk::TreeModel::Row &row,
                                 Glib::ustring const &addr,
                                 double const size,
                                 UnitEnum const &unit,
                                 Glib::ustring const &file)
  {
    row[m_model_columns.m_col_addr] = addr;
    row[m_model_columns.m_col_size] = size;
    
    switch (unit)
    {
    case UNIT_GBYTE:
      row[m_model_columns.m_col_unit] = "GByte";
      break;
      
    case UNIT_MBYTE:
      row[m_model_columns.m_col_unit] = "MByte";
      break;
      
    case UNIT_KBYTE:
      row[m_model_columns.m_col_unit] = "KByte";
      break;
      
    case UNIT_BYTE:
      row[m_model_columns.m_col_unit] = "Byte";
      break;
    }
    
    row[m_model_columns.m_col_unit_enum] = unit;
    row[m_model_columns.m_col_file] = file;
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  PartitionView::PartitionView()
  {
    mp_partition_model = Gtk::ListStore::create(m_model_columns);
    set_model(mp_partition_model);
    
    append_column(gettext("Starting Address"), m_model_columns.m_col_addr);
    
    {
      Gtk::TreeView::Column * const p_column = Gtk::manage(new Gtk::TreeView::Column(gettext("Partition Size")));
      Gtk::CellRendererText * const p_renderer = Gtk::manage(new Gtk::CellRendererText());
      
      p_column->pack_start(*p_renderer);
      
      p_column->set_cell_data_func(*p_renderer, sigc::mem_fun(*this, &PartitionView::convert_int_to_string));
      
      p_column->pack_start(m_model_columns.m_col_unit);
      
      append_column(*p_column);
    }
    
    append_column(gettext("Partition Image File"), m_model_columns.m_col_file);
    
    mp_partition_model->signal_row_deleted().connect(
      sigc::mem_fun(*this, &PartitionView::on_row_deleted));
  }
  
  //================================ Operation ================================
  
  void
  PartitionView::append_row(Glib::ustring const &addr, double const size, UnitEnum const &unit, Glib::ustring const &file)
  {
    Gtk::TreeModel::iterator iter = mp_partition_model->append();
    Gtk::TreeModel::Row row = *iter;
    
    set_input_field(row, addr, size, unit, file);
  }
  
  void
  PartitionView::modify_row(Glib::ustring const &addr, double const size, UnitEnum const &unit, Glib::ustring const &file)
  {
    Gtk::TreeModel::iterator iter = get_selection()->get_selected();
    Gtk::TreeModel::Row row = *iter;
    
    set_input_field(row, addr, size, unit, file);
  }
  
  void
  PartitionView::delete_selected_row()
  {
    Gtk::TreeModel::iterator iter = get_selection()->get_selected();
    
    mp_partition_model->erase(iter);
  }
  
  Gtk::TreePath
  PartitionView::check_size(uint32_t const rom_size) const
  {
    uint32_t end_addr;
    std::stringstream s;
    
    for (Gtk::TreeModel::iterator iter = get_model()->children().begin();
         iter != get_model()->children().end();
         ++iter)
    {
      Gtk::TreeRow row = *iter;
      
      s << static_cast<Glib::ustring>(row[m_model_columns.m_col_addr]).c_str();
      s >> std::hex >> end_addr;
      
      switch (row[m_model_columns.m_col_unit_enum])
      {
      case UNIT_GBYTE:
        end_addr += (static_cast<uint32_t>(row[m_model_columns.m_col_size]) * 1024 * 1024 * 1024);
        break;
        
      case UNIT_MBYTE:
        end_addr += (static_cast<uint32_t>(row[m_model_columns.m_col_size]) * 1024 * 1024);
        break;
        
      case UNIT_KBYTE:
        end_addr += (static_cast<uint32_t>(row[m_model_columns.m_col_size]) * 1024);
        break;
        
      case UNIT_BYTE:
        end_addr += static_cast<uint32_t>(row[m_model_columns.m_col_size]);
        break;
      }
      
      s.clear();
      s.str("");
      
      if (end_addr > rom_size)
      {
        return get_model()->get_path(iter);
      }
    }
    
    return Gtk::TreePath();
  }
  
  void
  PartitionView::create_rom(Glib::ustring const &rom_file)
  {
    std::ofstream ofs(rom_file.c_str(), std::ios::binary | std::ios::trunc);
    int8_t ch;
    uint32_t input_file_size;
    uint32_t fraction_base;
    uint32_t read_count;
    uint32_t addr;
    std::stringstream s;
    
    ProgressDialog progress_dialog(gettext("Burning Flash ROM"), *(dynamic_cast<Gtk::Window *>(get_toplevel())));
    progress_dialog.show();
    
    for (Gtk::TreeModel::iterator iter = get_model()->children().begin();
         iter != get_model()->children().end();
         ++iter)
    {
      Gtk::TreeRow row = *iter;
      
      std::ifstream ifs(static_cast<Glib::ustring>(row[m_model_columns.m_col_file]).c_str(), std::ios::binary);
      
      ifs.seekg(0, std::ios::end);
      input_file_size = std::streamoff(ifs.tellg());
      ifs.seekg(0, std::ios::beg);
      
      fraction_base = input_file_size / 100;
      
      s << static_cast<Glib::ustring>(row[m_model_columns.m_col_addr]).c_str();
      s >> std::hex >> addr;
      
      ofs.seekp(static_cast<std::fstream::pos_type>(addr));
      
      s.clear();
      s.str("");
      
      ch = ifs.get();
      
      read_count = 0;
      
      progress_dialog.set_text(static_cast<Glib::ustring>(row[m_model_columns.m_col_file]));
      progress_dialog.set_fraction(0.0);
      
      while (ifs.eof() != true)
      {
        Gtk::Main::iteration(false);
        
        ofs << ch;
        ch = ifs.get();
        
        ++read_count;
        
        if (0 == (read_count % fraction_base))
        {
          // :NOTE: Wei 2004-Jun-02:
          //
          // The following statement can be optimized below.
          //
          //dialog.set_fraction(0.01 * (read_count / fraction_base));
          progress_dialog.set_fraction(static_cast<double>(read_count) / static_cast<double>(input_file_size));
        }
        
        if (true == progress_dialog.is_cancel())
        {
          progress_dialog.hide();
          ifs.close();
          ofs.close();
          return;
        }
      }
      
      ifs.close();
    }
    
    ofs.close();
    
    Gtk::MessageDialog done_dialog(progress_dialog,
                                   String::ucompose(gettext("Burning %1 done."), rom_file),
                                   false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
    
    done_dialog.run();
  }
  
  Glib::ustring
  PartitionView::get_image_name(Gtk::TreePath const &path)
  {
    Gtk::TreeModel::Row row = *(get_model()->get_iter(path));
    
    return static_cast<Glib::ustring>(row[m_model_columns.m_col_file]);
  }
}
