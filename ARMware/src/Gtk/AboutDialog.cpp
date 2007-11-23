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

#include <gdk/gdkkeysyms.h>

#include <gtkmm/button.h>
#include <gtkmm/stock.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>

#include "AboutDialog.hpp"
#include "Layout.hpp"

#if WIN32
// for gettext
#include <libintl.h>
#endif

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //=========================== Signal handler ================================
  
  bool
  AboutDialog::on_key_press_event(GdkEventKey *event)
  {
    if (GDK_Escape == event->keyval)
    {
      response(Gtk::RESPONSE_OK);
      return true;
    }
    else
    {
      return Gtk::Dialog::on_key_press_event(event);
    }
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  AboutDialog::AboutDialog()
    : Gtk::Dialog(gettext("About ARMware"), false, false)
  {
    set_default_size(400, 200);
    
    // Developer
    
    DeveloperModelColumns developer_model_columns;
    Glib::RefPtr<Gtk::TreeStore> p_developer_tree_store = Gtk::TreeStore::create(developer_model_columns);
    
    Gtk::TreeModel::Row row = *(p_developer_tree_store->append());
    
    row[developer_model_columns.mColComponent] = gettext("Main Program");
    
    Gtk::TreeModel::Row row_child = *(p_developer_tree_store->append(row.children()));
    
    row_child[developer_model_columns.mColName] = gettext("Wei Hu");
    row_child[developer_model_columns.mColEmailAddr] = gettext("wei.hu.tw@gmail.com");
    
    Gtk::TreeView * const p_developer_view = Gtk::manage(new Gtk::TreeView());
    
    p_developer_view->set_model(p_developer_tree_store);
    p_developer_view->get_selection()->set_mode(Gtk::SELECTION_NONE);
    
    p_developer_view->append_column(gettext("Component"), developer_model_columns.mColComponent);
    p_developer_view->append_column(gettext("Developer"), developer_model_columns.mColName);
    p_developer_view->append_column(gettext("E-mail"), developer_model_columns.mColEmailAddr);
    
    Gtk::ScrolledWindow * const p_developer_window = Gtk::manage(new Gtk::ScrolledWindow());
    
    p_developer_window->add(*p_developer_view);
    p_developer_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    
    // Translator
    
    TranslatorModelColumns translator_model_columns;
    Glib::RefPtr<Gtk::TreeStore> p_translator_tree_store = Gtk::TreeStore::create(translator_model_columns);
    
    row = *(p_translator_tree_store->append());
    
    row[translator_model_columns.mColLanguage] = gettext("Traditional Chinese");
    
    row_child = *(p_translator_tree_store->append(row.children()));
    
    row_child[translator_model_columns.mColCountry] = gettext("Taiwan");
    
    Gtk::TreeModel::Row row_child_child = *(p_translator_tree_store->append(row_child.children()));
    
    row_child_child[translator_model_columns.mColName] = gettext("Wei Hu");
    row_child_child[translator_model_columns.mColEmailAddr] = gettext("wei.hu.tw@gmail.com");
    
    Gtk::TreeView * const p_translator_view = Gtk::manage(new Gtk::TreeView());
    
    p_translator_view->set_model(p_translator_tree_store);
    p_translator_view->get_selection()->set_mode(Gtk::SELECTION_NONE);
    
    p_translator_view->append_column(gettext("Language"), translator_model_columns.mColLanguage);
    p_translator_view->append_column(gettext("Country"), translator_model_columns.mColCountry);
    p_translator_view->append_column(gettext("Translator"), translator_model_columns.mColName);
    p_translator_view->append_column(gettext("E-mail"), translator_model_columns.mColEmailAddr);
    
    Gtk::ScrolledWindow * const p_translator_window = Gtk::manage(new Gtk::ScrolledWindow());
    
    p_translator_window->add(*p_translator_view);
    p_translator_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    
    Gtk::Notebook * const p_notebook = Gtk::manage(new Gtk::Notebook());
    
    p_notebook->set_border_width(Layout::BORDER_WIDTH);
    get_vbox()->pack_start(*p_notebook, Gtk::PACK_EXPAND_WIDGET);
    
    p_notebook->append_page(*p_developer_window, gettext("Programmer"));
    p_notebook->append_page(*p_translator_window, gettext("Translator"));
    
    // Button OK
    
    add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
    set_default_response(Gtk::RESPONSE_OK);
    
    show_all_children();
  }
}
