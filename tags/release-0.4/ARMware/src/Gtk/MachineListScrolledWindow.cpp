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

// Local include
//

#include "MachineListScrolledWindow.hpp"

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================= Signal handler ==============================
  
  void
  MachineListScrolledWindow::OnSizeAllocate(Gtk::Allocation &alloc)
  {
    m_width = alloc.get_width();
    m_height = alloc.get_height();
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //===================== Life cycle =============================
  
  MachineListScrolledWindow::MachineListScrolledWindow(Gtk::Window &parent)
    : mpMachineListViewWithPopup(Gtk::manage(new MachineListViewWithPopup(parent))),
      m_width(0),
      m_height(0)
  {
    set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    add(*mpMachineListViewWithPopup);
    
    signal_size_allocate().connect(sigc::mem_fun(*this, &MachineListScrolledWindow::OnSizeAllocate));
  }
}
