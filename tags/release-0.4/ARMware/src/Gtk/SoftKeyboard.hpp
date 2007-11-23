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

#ifndef SoftKeyboard_hpp
#define SoftKeyboard_hpp

// System include
//

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/box.h>

namespace Gtk
{
  class Button;
}

namespace ARMware
{
  // Forward declaration
  //
  
  class GPIOControlBlock;
  
  typedef class SoftKeyboard SoftKeyboard;
  class SoftKeyboard : public Gtk::VBox
  {
  private:
    
    // Attribute
    
    Gtk::Button * const mp_action;
    
    int m_width;
    int m_height;
    
    GPIOControlBlock *mp_GPIO_control_block;
    
    // Signal handler
    
    void OnSizeAllocate(Gtk::Allocation &alloc);
    
    void on_action_button_pressed();
    void on_action_button_released();
    
  public:
    
    // Life cycle
    
    SoftKeyboard();
    
    // Operation
    
    void
    register_GPIO_control_block(GPIOControlBlock * const GPIO_control_block)
    { mp_GPIO_control_block = GPIO_control_block; }
    
    // Inquiry
    
    int
    get_width() const
    { return m_width; }
    
    int
    get_height() const
    { return m_height; }
  };
}

#endif
