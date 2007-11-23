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

#ifndef MachineListScrolledWindow_hpp
#define MachineListScrolledWindow_hpp

// System include
//

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/adjustment.h>

// Local include
//

#include "MachineListViewWithPopup.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class MachineImpl;
  
  typedef class MachineListScrolledWindow MachineListScrolledWindow;
  class MachineListScrolledWindow : public Gtk::ScrolledWindow
  {
  private:
    // Attribute
    
    MachineListViewWithPopup * const mpMachineListViewWithPopup;
    
    int m_width;
    int m_height;
    
    // Friend
    
    friend class MainWindow;
    
    // Signal handler
    
    void OnSizeAllocate(Gtk::Allocation &alloc);
    
  public:
    
    // Life cycle
    
    MachineListScrolledWindow(Gtk::Window &parent);
    
    // Inquiry
    
    inline int
    get_width() const
    { return m_width; }
    
    inline int
    get_height() const
    { return m_height; }
    
    // Operation
    
    inline MachineImpl *
    SelectedMachine()
    {
      return mpMachineListViewWithPopup->SelectedMachine();
    }
    
    inline void
    DisableSelection()
    {
      mpMachineListViewWithPopup->set_sensitive(false);
    }
    
    inline void
    EnableSelection()
    {
      mpMachineListViewWithPopup->set_sensitive(true);
    }
  };
}

#endif
