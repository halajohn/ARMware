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

#ifndef NewMachineDialog_hpp
#define NewMachineDialog_hpp

// System include
//

#include <gtkmm/dialog.h>

// Local include
//

#include "MachineListViewWithPopup.hpp"

namespace Gtk
{
  class SpinButton;
  class Entry;
}

namespace ARMware
{
  // Forward declaration
  //
  
  class AdvComboBoxText;
  class PartitionTable;
  
  typedef class NewMachineDialog NewMachineDialog;
  class NewMachineDialog : public Gtk::Dialog
  {
  private:
    
    // Attribute
    
    Glib::RefPtr<Gtk::ListStore> mpMachineListListStore;
    MachineListViewWithPopup::MachineListModelColumns &mColumns;
    bool mPreformNewMachine;
    Gtk::TreeModel::iterator const &mrClickIter;
    int mClickOnIndex;
    
    Gtk::Entry * const mpMachineNameEntry;
    
    Gtk::Button * const mpRomFileButton;
    Gtk::SpinButton * const mpRomSizeSpinButton;
    AdvComboBoxText * const mpRomUnitComboBox;
    
    PartitionTable * const mp_partition_table;
    
    Gtk::SpinButton * const mpScreenWidthSpinButton;
    Gtk::SpinButton * const mpScreenHeightSpinButton;
    
    // Signal handler
    
    void on_rom_file_button_clicked();
    void check_total_correct();
    void on_my_response(int response_id);
    
  public:
    
    // Life cycle
    
    NewMachineDialog(Glib::ustring const &title,
                     Gtk::Window &parent,
                     Glib::RefPtr<Gtk::ListStore> machineListListStore,
                     MachineListViewWithPopup::MachineListModelColumns &columns,
                     bool preformNewMachine,
                     Gtk::TreeModel::iterator const &iter);
    
    ~NewMachineDialog();
  };
}

#endif
