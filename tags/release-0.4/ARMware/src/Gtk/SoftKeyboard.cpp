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

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/button.h>

#if WIN32
// for gettext
#include <libintl.h>
#endif

#include "SoftKeyboard.hpp"
#include "../GPIOControlBlock.hpp"

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================= Signal handler ==============================
  
  void
  SoftKeyboard::OnSizeAllocate(Gtk::Allocation &alloc)
  {
    m_width = alloc.get_width();
    m_height = alloc.get_height();
  }
  
  void
  SoftKeyboard::on_action_button_pressed()
  { mp_GPIO_control_block->press_action_button(); }
  
  void
  SoftKeyboard::on_action_button_released()
  { mp_GPIO_control_block->release_action_button(); }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //===================== Life cycle =============================
  
  SoftKeyboard::SoftKeyboard()
    : mp_action(Gtk::manage(new Gtk::Button(gettext("Action")))),
      m_width(0),
      mp_GPIO_control_block(0)
  {
    pack_start(*mp_action, Gtk::PACK_EXPAND_WIDGET);
    
    signal_size_allocate().connect(sigc::mem_fun(*this, &SoftKeyboard::OnSizeAllocate));
    mp_action->signal_pressed().connect(sigc::mem_fun(*this, &SoftKeyboard::on_action_button_pressed));
    mp_action->signal_released().connect(sigc::mem_fun(*this, &SoftKeyboard::on_action_button_released));
    
    show_all_children();
    
    // :NOTE: Wei 2004-Mar-31:
    //
    // get initial width
    Gtk::Requisition req;
    size_request(req);
    m_height = req.height;
  }
}
