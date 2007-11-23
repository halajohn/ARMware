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

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/stock.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/alignment.h>

// Local include
//

#include "ErrorDialog.hpp"
#include "Layout.hpp"

#if WIN32
// for gettext
#include <libintl.h>
#endif

namespace ARMware
{
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  ErrorDialog::ErrorDialog(Gtk::Window &parent)
    : mp_message_vbox(Gtk::manage(new Gtk::VBox(false, Layout::SPACING)))
  {
    set_title(gettext("Error"));
    set_resizable(false);
    set_transient_for(parent);
    set_has_separator(false);
    
    Gtk::HBox * const p_hbox = Gtk::manage(new Gtk::HBox(false, Layout::SPACING));
    
    p_hbox->set_border_width(Layout::BORDER_WIDTH);
    
    get_vbox()->pack_start(*p_hbox, Gtk::PACK_SHRINK);
    
    Gtk::Image * const p_image = Gtk::manage(new Gtk::Image(Gtk::Stock::DIALOG_ERROR, Gtk::ICON_SIZE_DIALOG));
    
    p_hbox->pack_start(*p_image, Gtk::PACK_SHRINK);
    
    Gtk::Alignment * const p_alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 0.0, 0.0));
    p_alignment->add(*mp_message_vbox);
    
    p_hbox->pack_start(*p_alignment, Gtk::PACK_EXPAND_WIDGET);
    
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    set_default_response(Gtk::RESPONSE_OK);
    
    show_all_children();
  }
  
  //============================== Operation ==================================
  
  void
  ErrorDialog::add_message(Glib::ustring const &message)
  {
    Gtk::Alignment * const alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 0.0, 0.0));
    Gtk::Label * const label = Gtk::manage(new Gtk::Label(message));
    
    alignment->add(*label);
    
    alignment->show();
    label->show();
    
    mp_message_vbox->pack_start(*alignment, Gtk::PACK_SHRINK);
  }
}
