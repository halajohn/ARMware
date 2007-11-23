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

#include <gtkmm/notebook.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/table.h>
#include <gtkmm/stock.h>
#include <gtkmm/frame.h>
#include <gtkmm/fileselection.h>
#include <gtkmm/button.h>

#if WIN32
// for gettext
#include <libintl.h>
#endif

// Local include
//

#include "NewMachineDialog.hpp"
#include "ErrorDialog.hpp"
#include "AdvComboBoxText.hpp"
#include "PartitionTable.hpp"
#include "Layout.hpp"

#include "../Utils/File.hpp"

#include "compose/ucompose.hpp"

#include "../Exception.hpp"
#include "../Database/MachineImpl.hpp"

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================= Signal handler ==============================
  
  void
  NewMachineDialog::on_rom_file_button_clicked()
  {
    Gtk::FileSelection dialog(gettext("Please choose a file"));
    dialog.set_transient_for(*this);
    
    if (false == mpRomFileButton->get_label().empty())
    {
      dialog.set_filename(mpRomFileButton->get_label());
    }
    
    int const result = dialog.run();
    
    switch (result)
    {
    case Gtk::RESPONSE_OK:
      mpRomFileButton->set_label(dialog.get_filename());
      break;
      
    case Gtk::RESPONSE_CANCEL:
      break;
      
    default:
      break;
    }
  }
  
  void
  NewMachineDialog::check_total_correct()
  {
    ErrorDialog dialog(*this);
    
    uint32_t error_count = 1; // I want to count from 1
    bool stopped = false;
    
    if (true == mpMachineNameEntry->get_text().empty())
    {
      dialog.add_message(String::ucompose(gettext("%1: You haven't specify a machine name yet."), error_count++));
      stopped = true;
    }
    
    if (true == mpRomFileButton->get_label().empty())
    {
      dialog.add_message(String::ucompose(gettext("%1: You haven't specify a flash ROM file yet."), error_count++));
      stopped = true;
    }
    else
    {
      switch (check_file_error(mpRomFileButton->get_label().c_str()))
      {
      case NO_ERROR:
        break;
        
      case ERROR_IS_DIR:
        dialog.add_message(String::ucompose(gettext("%1: You specify a directory (%2) as the flash ROM file."),
                                            error_count++, mpRomFileButton->get_label()));
        stopped = true;
        break;
        
      case ERROR_FILE_NONEXIST:
        dialog.add_message(String::ucompose(gettext("%1: Flash ROM file '%2' not found."),
                                            error_count++, mpRomFileButton->get_label()));
        stopped = true;
        break;
      }
    }
    
    if (true == stopped)
    {
      dialog.run();
      
      signal_response().emission_stop();
    }
    else
    {
      MachineImpl *machineImpl;
      
      if (true == mPreformNewMachine)
      {
        try
        {
          machineImpl = &(Database::get_database()->new_machine(mpMachineNameEntry->get_text()));
          
          Gtk::TreeModel::Row row = *(mpMachineListListStore->append());
          row[mColumns.m_col_name] = mpMachineNameEntry->get_text();
        }
        catch (SameMachineNameException &/* e */)
        {
          dialog.add_message(String::ucompose(gettext("You specify an existed machine name: %1"),
                                              mpMachineNameEntry->get_text()));
          dialog.run();
          
          signal_response().emission_stop();
          return;
        }
      }
      else
      {
        machineImpl = &((*Database::get_database())[mClickOnIndex]);
        
        machineImpl->set_machine_name(mpMachineNameEntry->get_text());
        
        Gtk::TreeModel::Row row = *mrClickIter;
        row[mColumns.m_col_name] = mpMachineNameEntry->get_text();
      }
      
      // :NOTE: Wei 2004-Apr-10:
      //
      // Modify the underlying MachineImpl
      machineImpl->set_rom_file(mpRomFileButton->get_label());
      
      machineImpl->set_screen_width(static_cast<uint32_t>(mpScreenWidthSpinButton->get_value()));
      machineImpl->set_screen_height(static_cast<uint32_t>(mpScreenHeightSpinButton->get_value()));
    }
  }
  
  void
  NewMachineDialog::on_my_response(int response_id)
  {
    switch (response_id)
    {
    case Gtk::RESPONSE_OK:
      check_total_correct();
      break;
      
    case Gtk::RESPONSE_APPLY:
      check_total_correct();
      signal_response().emission_stop();
      break;
      
    case Gtk::RESPONSE_CANCEL:
      break;
    }
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================= Life cycle ==================================
  
  NewMachineDialog::NewMachineDialog(Glib::ustring const &title,
                                     Gtk::Window &parent,
                                     Glib::RefPtr<Gtk::ListStore> machineListListStore,
                                     MachineListViewWithPopup::MachineListModelColumns &columns,
                                     bool preformNewMachine,
                                     Gtk::TreeModel::iterator const &iter)
    : Gtk::Dialog(title, parent),
      mpMachineListListStore(machineListListStore),
      mColumns(columns),
      mPreformNewMachine(preformNewMachine),
      mrClickIter(iter),
      mpMachineNameEntry(Gtk::manage(new Gtk::Entry())),
      mpRomFileButton(Gtk::manage(new Gtk::Button(""))),
      mpRomSizeSpinButton(Gtk::manage(new Gtk::SpinButton(*(Gtk::manage(new Gtk::Adjustment(0.0, 0.0, 1000000)))))),
      mpRomUnitComboBox(Gtk::manage(new AdvComboBoxText())),
      mp_partition_table(Gtk::manage(new PartitionTable(mpRomFileButton, mpRomSizeSpinButton, mpRomUnitComboBox))),
      mpScreenWidthSpinButton(Gtk::manage(new Gtk::SpinButton(*(Gtk::manage(new Gtk::Adjustment(1.0, 1.0, 2000.0)))))),
      mpScreenHeightSpinButton(Gtk::manage(new Gtk::SpinButton(*(Gtk::manage(new Gtk::Adjustment(1.0, 1.0, 2000.0))))))
  {
    set_resizable(false);
    
    Gtk::Label * const p_machine_name_label = Gtk::manage(new Gtk::Label(gettext("Machine Name: ")));
    
    Gtk::HBox * const p_machine_name_hbox = Gtk::manage(new Gtk::HBox());
    p_machine_name_hbox->set_spacing(Layout::SPACING);
    p_machine_name_hbox->pack_start(*p_machine_name_label, Gtk::PACK_SHRINK);
    p_machine_name_hbox->pack_start(*mpMachineNameEntry, Gtk::PACK_EXPAND_WIDGET);
    
    Gtk::VBox * const p_vbox_1 = Gtk::manage(new Gtk::VBox());
    p_vbox_1->set_border_width(Layout::BORDER_WIDTH);
    p_vbox_1->pack_start(*p_machine_name_hbox, Gtk::PACK_SHRINK);
    
    Gtk::Label * const p_rom_file_label = Gtk::manage(new Gtk::Label(gettext("Flash ROM File: ")));
    Gtk::Label * const p_rom_size_label = Gtk::manage(new Gtk::Label(gettext("Flash ROM Size: ")));
    
    mpRomSizeSpinButton->set_wrap(false);
    mpRomSizeSpinButton->set_numeric(true);
    
    mpRomUnitComboBox->append_text(gettext("GByte"));
    mpRomUnitComboBox->append_text(gettext("MByte"));
    mpRomUnitComboBox->append_text(gettext("KByte"));
    mpRomUnitComboBox->append_text(gettext("Byte"));
    
    Gtk::HBox * const p_rom_file_hbox = Gtk::manage(new Gtk::HBox());
    p_rom_file_hbox->set_spacing(Layout::SPACING);
    p_rom_file_hbox->pack_start(*p_rom_file_label, Gtk::PACK_SHRINK);
    p_rom_file_hbox->pack_start(*mpRomFileButton, Gtk::PACK_EXPAND_WIDGET);
    
    Gtk::HBox * const p_rom_size_hbox = Gtk::manage(new Gtk::HBox());
    p_rom_size_hbox->set_spacing(Layout::SPACING);
    p_rom_size_hbox->pack_start(*p_rom_size_label, Gtk::PACK_SHRINK);
    p_rom_size_hbox->pack_start(*mpRomSizeSpinButton, Gtk::PACK_SHRINK);
    p_rom_size_hbox->pack_start(*mpRomUnitComboBox, Gtk::PACK_SHRINK);
    
    mp_partition_table->set_border_width(Layout::BORDER_WIDTH);
    
    Gtk::VBox * const p_partition_frame_vbox = Gtk::manage(new Gtk::VBox());
    p_partition_frame_vbox->set_border_width(Layout::BORDER_WIDTH);
    p_partition_frame_vbox->pack_start(*p_rom_size_hbox, Gtk::PACK_SHRINK);
    p_partition_frame_vbox->pack_start(*mp_partition_table, Gtk::PACK_EXPAND_WIDGET);
    
    Gtk::Frame * const p_partition_frame = Gtk::manage(new Gtk::Frame(gettext("Burning New Flash ROM")));
    p_partition_frame->add(*p_partition_frame_vbox);
    
    Gtk::VBox * const p_vbox_2 = Gtk::manage(new Gtk::VBox());
    p_vbox_2->set_border_width(Layout::BORDER_WIDTH);
    p_vbox_2->set_spacing(Layout::SPACING * 2);
    p_vbox_2->pack_start(*p_rom_file_hbox, Gtk::PACK_SHRINK);
    p_vbox_2->pack_start(*p_partition_frame, Gtk::PACK_EXPAND_WIDGET);
    
    Gtk::Label * const p_screen_width_label = Gtk::manage(new Gtk::Label(gettext("Width: ")));
    Gtk::Label * const p_screen_height_label = Gtk::manage(new Gtk::Label(gettext("Height: ")));
    
    Gtk::Table * const p_screen_table = Gtk::manage(new Gtk::Table(2, 2, false));
    p_screen_table->set_spacings(Layout::SPACING);
    
    p_screen_table->attach(*p_screen_width_label, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL | Gtk::EXPAND, 0, 0);
    p_screen_table->attach(*p_screen_height_label, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL | Gtk::EXPAND, 0, 0);
    p_screen_table->attach(*mpScreenWidthSpinButton, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL | Gtk::EXPAND, 0, 0);
    p_screen_table->attach(*mpScreenHeightSpinButton, 1, 2, 1, 2, Gtk::FILL, Gtk::FILL | Gtk::EXPAND, 0, 0);
    
    mpScreenWidthSpinButton->set_wrap(false);
    mpScreenWidthSpinButton->set_numeric(true);
    
    mpScreenHeightSpinButton->set_wrap(false);
    mpScreenHeightSpinButton->set_numeric(true);
    
    Gtk::VBox * const p_vbox_3 = Gtk::manage(new Gtk::VBox());
    p_vbox_3->set_border_width(Layout::BORDER_WIDTH);
    p_vbox_3->pack_start(*p_screen_table, Gtk::PACK_SHRINK);
    
    Gtk::Notebook * const p_notebook = Gtk::manage(new Gtk::Notebook());
    
    p_notebook->append_page(*p_vbox_1, gettext("Basic Information"));
    p_notebook->append_page(*p_vbox_2, gettext("Flash ROM"));
    p_notebook->append_page(*p_vbox_3, gettext("Screen Size"));
    
    get_vbox()->set_border_width(Layout::BORDER_WIDTH);
    get_vbox()->pack_start(*p_notebook, Gtk::PACK_EXPAND_WIDGET);
    
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    
    if (false == mPreformNewMachine)
    {
      // Edit attributes of an existed machine.
      Gtk::Button * const p_apply_button = Gtk::manage(new Gtk::Button(Gtk::Stock::APPLY));
      
      add_action_widget(*p_apply_button, Gtk::RESPONSE_APPLY);
      
      // Read attributes
      
      Gtk::TreeModel::Path path = mpMachineListListStore->get_path(iter);
      
      assert(1 == path.get_depth()); // The path's depth of ListStore model is always 1.
      
      mClickOnIndex = path[0];
      
      MachineImpl &machineImpl = (*Database::get_database())[mClickOnIndex];
      
      mpMachineNameEntry->set_text(machineImpl.get_machine_name());
      
      mpRomFileButton->set_label(machineImpl.get_rom_file());
      
      mpScreenWidthSpinButton->set_value(machineImpl.get_screen_width());
      mpScreenHeightSpinButton->set_value(machineImpl.get_screen_height());
    }
    
    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    
    // :NOTE: Wei 2004-Mar-19:
    //
    // I have to use connect_notify() to plug a 'response' signal handler _before_ the default one.
    signal_response().connect_notify(sigc::mem_fun(*this, &NewMachineDialog::on_my_response));
    
    mpRomFileButton->signal_clicked().connect(sigc::mem_fun(*this, &NewMachineDialog::on_rom_file_button_clicked));
    
    set_default_response(Gtk::RESPONSE_CANCEL);
    
    show_all_children();
  }
  
  NewMachineDialog::~NewMachineDialog()
  { }
}
