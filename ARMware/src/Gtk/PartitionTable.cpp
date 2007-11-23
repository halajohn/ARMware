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

#include <sstream>
#include <fstream>

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/button.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <gtkmm/alignment.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/fileselection.h>
#include <gtkmm/messagedialog.h>

#if WIN32
// for gettext
#include <libintl.h>
#endif

#include "PartitionTable.hpp"
#include "ErrorDialog.hpp"
#include "Layout.hpp"
#include "AdvComboBoxText.hpp"

#include "../Utils/File.hpp"

#include "compose/ucompose.hpp"

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================== Operation ==================================
  
  bool
  PartitionTable::check_valid_hex_string(std::string const &str) const
  {
    uint32_t value;
    std::stringstream s;
    
    s << str;
    s >> std::hex >> value;
    
    if (!s)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  
  void
  PartitionTable::clear_input_field()
  {
    mp_addr_entry->set_text("");
    mp_image_size_spinbutton->set_value(0.0);
    mp_image_size_unit_combobox->unset_active();
    mp_image_button->set_label("");
  }
  
  bool
  PartitionTable::check_input_field_valid()
  {
    ErrorDialog dialog(*(dynamic_cast<Gtk::Window *>(get_toplevel())));
    
    uint32_t error_count = 1;
    bool error = false;
    
    if (false == check_valid_hex_string(mp_addr_entry->get_text()))
    {
      dialog.add_message(String::ucompose(gettext("%1: Starting address isn't a valid hexadecimal number."),
                                          error_count++));
      
      error = true;
    }
    
    if (0 == mp_image_size_spinbutton->get_value())
    {
      dialog.add_message(String::ucompose(gettext("%1: Partition size shouldn't be 0."), error_count++));
      
      error = true;
    }
    
    if (-1 == mp_image_size_unit_combobox->get_active_row_number())
    {
      dialog.add_message(String::ucompose(gettext("%1: You haven't specify the unit yet."), error_count++));
      
      error = true;
    }
    
    if (true == mp_image_button->get_label().empty())
    {
      dialog.add_message(String::ucompose(gettext("%1: You haven't specify a image file yet."), error_count++));
      
      error = true;
    }
    
    if (false == error)
    {
      std::ifstream ifs(mp_image_button->get_label().c_str());
      
      ifs.seekg(0, std::ios::end);
      uint32_t const input_file_size = std::streamoff(ifs.tellg());
      ifs.close();
      
      uint32_t partition_size = 0;
      
      switch (mp_image_size_unit_combobox->get_active_row_number())
      {
      case UNIT_GBYTE:
        partition_size = static_cast<uint32_t>(mp_image_size_spinbutton->get_value()) * 1024 * 1024 * 1024;
        break;
        
      case UNIT_MBYTE:
        partition_size = static_cast<uint32_t>(mp_image_size_spinbutton->get_value()) * 1024 * 1024;
        break;
        
      case UNIT_KBYTE:
        partition_size = static_cast<uint32_t>(mp_image_size_spinbutton->get_value()) * 1024;
        break;
        
      case UNIT_BYTE:
        partition_size = static_cast<uint32_t>(mp_image_size_spinbutton->get_value());
        break;
      }
      
      if (partition_size < input_file_size)
      {
        dialog.add_message(String::ucompose(gettext("%1: The image file size is larger than the partition size."), error_count++));
        
        error = true;
      }
    }
    
    if (true == error)
    {
      dialog.run();
      
      return false;
    }
    else
    {
      return true;
    }
  }
  
  //============================ Signal handler ===============================
  
  void
  PartitionTable::on_row_deleted()
  {
    on_row_unselected();
  }
  
  void
  PartitionTable::on_row_unselected()
  {
    mp_add_button->set_sensitive(true);
    mp_delete_button->set_sensitive(false);
    mp_modify_button->set_sensitive(false);
    
    clear_input_field();
  }
  
  void
  PartitionTable::on_row_selected(Glib::ustring const &addr, double const size, UnitEnum const &unit, Glib::ustring const &file)
  {
    mp_add_button->set_sensitive(false);
    mp_delete_button->set_sensitive(true);
    mp_modify_button->set_sensitive(true);
    
    mp_addr_entry->set_text(addr);
    mp_image_size_spinbutton->set_value(size);
    mp_image_size_unit_combobox->set_active(unit);
    mp_image_button->set_label(file);
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  PartitionTable::PartitionTable(Gtk::Button * const p_rom_button,
                                 Gtk::SpinButton * const p_rom_size_spinbutton,
                                 AdvComboBoxText * const p_rom_size_unit_combobox)
    : mp_addr_entry(Gtk::manage(new Gtk::Entry())),
      mp_image_button(Gtk::manage(new Gtk::Button())),
      mp_image_size_spinbutton(Gtk::manage(new Gtk::SpinButton(*(Gtk::manage(new Gtk::Adjustment(0.0, 0.0, 1000000.0)))))),
      mp_image_size_unit_combobox(Gtk::manage(new AdvComboBoxText())),
      mp_create_button(Gtk::manage(new Gtk::Button(gettext("Burn")))),
      mp_add_button(Gtk::manage(new Gtk::Button(gettext("Add")))),
      mp_delete_button(Gtk::manage(new Gtk::Button(gettext("Delete")))),
      mp_modify_button(Gtk::manage(new Gtk::Button(gettext("Modify")))),
      mp_rom_button(p_rom_button),
      mp_rom_size_spinbutton(p_rom_size_spinbutton),
      mp_rom_size_unit_combobox(p_rom_size_unit_combobox)
  {
    set_spacing(Layout::SPACING);
    
    mp_addr_entry->set_alignment(Gtk::ALIGN_RIGHT);
    
    Gtk::ScrolledWindow * const p_scrolled_window = Gtk::manage(new Gtk::ScrolledWindow());
    p_scrolled_window->set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    
    mp_partition_view = Gtk::manage(new PartitionView());
    
    p_scrolled_window->add(*mp_partition_view);
    p_scrolled_window->set_vadjustment(mp_partition_view->get_vadjustment());
    
    pack_start(*p_scrolled_window, Gtk::PACK_EXPAND_WIDGET);
    
    Gtk::Table * const p_table = Gtk::manage(new Gtk::Table(3, 2));
    
    p_table->set_col_spacings(Layout::SPACING);
    
    pack_start(*p_table, Gtk::PACK_SHRINK);
    
    p_table->attach(*(Gtk::manage(new Gtk::Label(gettext("Starting Address")))), 0, 1, 0, 1,
                    Gtk::FILL | Gtk::SHRINK, Gtk::FILL | Gtk::SHRINK);
    p_table->attach(*(Gtk::manage(new Gtk::Label(gettext("Partition Size")))), 1, 2, 0, 1,
                    Gtk::FILL | Gtk::SHRINK, Gtk::FILL | Gtk::SHRINK);
    p_table->attach(*(Gtk::manage(new Gtk::Label(gettext("Partition Image File")))), 2, 3, 0, 1);
    
    p_table->attach(*mp_addr_entry, 0, 1, 1, 2,
                    Gtk::FILL | Gtk::SHRINK, Gtk::FILL | Gtk::SHRINK);
    
    mp_image_size_spinbutton->set_wrap(false);
    mp_image_size_spinbutton->set_numeric(true);
    mp_image_size_spinbutton->set_digits(0);
    
    Gtk::HBox * const p_size_hbox = Gtk::manage(new Gtk::HBox());
    
    p_size_hbox->set_spacing(Layout::SPACING);
    p_size_hbox->pack_start(*mp_image_size_spinbutton, Gtk::PACK_EXPAND_WIDGET);
    
    mp_image_size_unit_combobox->append_text(gettext("GByte"));
    mp_image_size_unit_combobox->append_text(gettext("MByte"));
    mp_image_size_unit_combobox->append_text(gettext("KByte"));
    mp_image_size_unit_combobox->append_text(gettext("Byte"));
    
    p_size_hbox->pack_start(*mp_image_size_unit_combobox, Gtk::PACK_SHRINK);
    
    p_table->attach(*p_size_hbox, 1, 2, 1, 2,
                    Gtk::FILL | Gtk::SHRINK, Gtk::FILL | Gtk::SHRINK);
    p_table->attach(*mp_image_button, 2, 3, 1, 2);
    
    Gtk::HBox * const p_hbox = Gtk::manage(new Gtk::HBox());
    
    p_hbox->set_spacing(Layout::SPACING);
    
    p_hbox->pack_start(*mp_create_button, Gtk::PACK_SHRINK);
    
    p_hbox->pack_end(*mp_delete_button, Gtk::PACK_SHRINK);
    p_hbox->pack_end(*mp_modify_button, Gtk::PACK_SHRINK);
    p_hbox->pack_end(*mp_add_button, Gtk::PACK_SHRINK);
    
    pack_start(*p_hbox, Gtk::PACK_SHRINK);
    
    show_all_children();
    
    mp_create_button->signal_clicked().connect(
      sigc::mem_fun(*this, &PartitionTable::on_create_button_clicked));
    
    mp_add_button->signal_clicked().connect(
      sigc::mem_fun(*this, &PartitionTable::on_add_button_clicked));
    
    mp_delete_button->signal_clicked().connect(
      sigc::mem_fun(*this, &PartitionTable::on_delete_button_clicked));
    
    mp_modify_button->signal_clicked().connect(
      sigc::mem_fun(*this, &PartitionTable::on_modify_button_clicked));
    
    mp_image_button->signal_clicked().connect(
      sigc::mem_fun(*this, &PartitionTable::on_image_button_clicked));
    
    mp_partition_view->signal_row_deleted().connect(
      sigc::mem_fun(*this, &PartitionTable::on_row_deleted));
    
    mp_partition_view->signal_row_selected().connect(
      sigc::mem_fun(*this, &PartitionTable::on_row_selected));
    
    mp_partition_view->signal_row_unselected().connect(
      sigc::mem_fun(*this, &PartitionTable::on_row_unselected));
    
    mp_add_button->set_sensitive(true);
    mp_delete_button->set_sensitive(false);
    mp_modify_button->set_sensitive(false);
  }
  
  //============================ Signal handler ===============================
  
  void
  PartitionTable::on_image_button_clicked()
  {
    Gtk::FileSelection dialog(gettext("Select an image file for a partition"));
    
    if (false == m_last_selected_file.empty())
    {
      dialog.set_filename(m_last_selected_file);
    }
    
    int const result = dialog.run();
    
    switch (result)
    {
    case Gtk::RESPONSE_OK:
      mp_image_button->set_label(dialog.get_filename());
      
      m_last_selected_file = dialog.get_filename();
      break;
      
    default:
      break;
    }
  }
  
  void
  PartitionTable::on_create_button_clicked()
  {
    ErrorDialog dialog(*(dynamic_cast<Gtk::Window *>(get_toplevel())));
    uint32_t error_count = 1;
    bool error = false;
    
    if (true == mp_partition_view->get_model()->children().empty())
    {
      dialog.add_message(String::ucompose(gettext("%1: You haven't specify any image files yet."), error_count++));
      error = true;
    }
    
    if (true == mp_rom_button->get_label().empty())
    {
      dialog.add_message(String::ucompose(gettext("%1: You haven't specify a flash ROM file yet."), error_count++));
      
      error = true;
    }
    else
    {
      switch (check_file_error(mp_rom_button->get_label().c_str()))
      {
      case ERROR_IS_DIR:
        dialog.add_message(String::ucompose(gettext("%1: You specify a directory (%2) as the flash ROM file."),
                                            error_count++, mp_rom_button->get_label()));
        error = true;
        break;
        
      default:
        break;
      }
    }
    
    if (0 == mp_rom_size_spinbutton->get_value())
    {
      dialog.add_message(String::ucompose(gettext("%1: Flash ROM file size shouldn't be 0."), error_count++));
      
      error = true;
    }
    
    if (-1 == mp_rom_size_unit_combobox->get_active_row_number())
    {
      dialog.add_message(String::ucompose(gettext("%1: You haven't specify the unit used for flash ROM file yet."), error_count++));
      
      error = true;
    }
    
    uint32_t rom_size = static_cast<uint32_t>(mp_rom_size_spinbutton->get_value());
    
    switch (mp_rom_size_unit_combobox->get_active_row_number())
    {
    case UNIT_GBYTE:
      rom_size *= (1024 * 1024 * 1024);
      break;
      
    case UNIT_MBYTE:
      rom_size *= (1024 * 1024);
      break;
      
    case UNIT_KBYTE:
      rom_size *= 1024;
      break;
      
    case UNIT_BYTE:
      break;
    }
    
    Gtk::TreePath const path = mp_partition_view->check_size(rom_size);
    
    if (path.empty() != true)
    {
      dialog.add_message(String::ucompose(gettext("%1: The image file (%2) size is larger than that of the flash ROM file."),
                                          error_count++, mp_partition_view->get_image_name(path)));
      
      error = true;
    }
    
    if (true == error)
    {
      dialog.run();
      return;
    }
    else
    {
      if (ERROR_FILE_NONEXIST == check_file_error(mp_rom_button->get_label().c_str()))
      {
        Gtk::MessageDialog dialog(*(dynamic_cast<Gtk::Window *>(get_toplevel())),
                                  String::ucompose(gettext("%1 doesn't exist.\nDo you want me to create it for you?"),
                                                   mp_rom_button->get_label()),
                                  false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
        
        int const result = dialog.run();
        
        switch (result)
        {
        case Gtk::RESPONSE_OK:
          break;
          
        default:
          return;
        }
      }
      
      mp_partition_view->create_rom(mp_rom_button->get_label());
    }
  }
  
  void
  PartitionTable::on_modify_button_clicked()
  {
    if (false == check_input_field_valid())
    {
      return;
    }
    
    mp_partition_view->modify_row(mp_addr_entry->get_text(),
                                  mp_image_size_spinbutton->get_value(),
                                  static_cast<UnitEnum>(mp_image_size_unit_combobox->get_active_row_number()),
                                  mp_image_button->get_label());
  }
  
  void
  PartitionTable::on_add_button_clicked()
  {
    if (false == check_input_field_valid())
    {
      return;
    }
    
    mp_partition_view->append_row(mp_addr_entry->get_text(),
                                  mp_image_size_spinbutton->get_value(),
                                  static_cast<UnitEnum>(mp_image_size_unit_combobox->get_active_row_number()),
                                  mp_image_button->get_label());
    
    clear_input_field();
  }
  
  void
  PartitionTable::on_delete_button_clicked()
  {
    mp_partition_view->delete_selected_row();
  }
}
