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

#ifndef AboutDialog_hpp
#define AboutDialog_hpp

// System include
//

#include <gtkmm/dialog.h>
#include <gtkmm/liststore.h>

namespace ARMware
{
  typedef class AboutDialog AboutDialog;
  class AboutDialog : public Gtk::Dialog
  {
  private:
    class DeveloperModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
      
      // Life cycle
      
      DeveloperModelColumns()
      {
        add(mColComponent);
        add(mColName);
        add(mColEmailAddr);
      }
      
      // Attribute
      
      Gtk::TreeModelColumn<Glib::ustring> mColComponent;
      Gtk::TreeModelColumn<Glib::ustring> mColName;
      Gtk::TreeModelColumn<Glib::ustring> mColEmailAddr;
    };
    typedef class DeveloperModelColumns DeveloperModelColumns;
    
    class TranslatorModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
      
      // Life cycle
      
      TranslatorModelColumns()
      {
        add(mColLanguage);
        add(mColCountry);
        add(mColName);
        add(mColEmailAddr);
      }
      
      // Attribute
      
      Gtk::TreeModelColumn<Glib::ustring> mColLanguage;
      Gtk::TreeModelColumn<Glib::ustring> mColCountry;
      Gtk::TreeModelColumn<Glib::ustring> mColName;
      Gtk::TreeModelColumn<Glib::ustring> mColEmailAddr;
    };
    typedef class TranslatorModelColumns TranslatorModelColumns;
    
    // Signal handler
    
    // :NOTE: Wei 2004-Mar-15:
    //
    // Override the defualt key_press_event signal handler.
    bool on_key_press_event(GdkEventKey *event);
    
  public:
    
    // Life cycle
    
    AboutDialog();
  };
}

#endif
