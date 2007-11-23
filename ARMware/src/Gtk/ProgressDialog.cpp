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

#include <gtkmm/progressbar.h>
#include <gtkmm/stock.h>

#include "ProgressDialog.hpp"
#include "Layout.hpp"

namespace ARMware
{
  //////////////////////////////// Private ////////////////////////////////////
  //============================= Signal handler ==============================
  
  void
  ProgressDialog::cancel_button_clicked()
  {
    m_cancel = true;
  }
  
  /////////////////////////// Public //////////////////////////////////////////
  //========================= Life cycle ======================================
  
  ProgressDialog::ProgressDialog(std::string const &title, Gtk::Window &parent)
    : Gtk::Dialog(title, parent, true),
      m_cancel(false),
      mp_progress_bar(Gtk::manage(new Gtk::ProgressBar()))
  {
    set_resizable(false);
    set_has_separator(false);
    
    get_vbox()->set_border_width(Layout::BORDER_WIDTH);
    get_vbox()->pack_start(*mp_progress_bar, Gtk::PACK_SHRINK);
    
    Gtk::Button * const p_cancel_button = Gtk::manage(new Gtk::Button(Gtk::Stock::CANCEL));
    add_action_widget(*p_cancel_button, Gtk::RESPONSE_CANCEL);
    
    p_cancel_button->signal_clicked().connect(
      sigc::mem_fun(*this, &ProgressDialog::cancel_button_clicked));
    
    show_all_children();
  }
  
  void
  ProgressDialog::set_fraction(double const fraction)
  {
    mp_progress_bar->set_fraction(fraction);
  }
  
  void
  ProgressDialog::set_text(Glib::ustring const &text)
  {
    mp_progress_bar->set_text(text);
  }
}
