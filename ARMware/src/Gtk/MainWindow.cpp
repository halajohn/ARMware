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

#include <gtkmm/accelkey.h>
#include <gtkmm/stock.h>
#include <gtkmm/image.h>

#include <sigc++/retype_return.h>

// Local include
//

#include "MainWindow.hpp"
#include "AboutDialog.hpp"
#include "SerialConsole.hpp"
#include "PrefsDialog.hpp"
#include "ErrorDialog.hpp"
#include "Layout.hpp"

#include "../Database/Database.hpp"
#include "../StartMachine.hpp"

#if WIN32
// for gettext
#include <libintl.h>
#endif

namespace ARMware
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================ Signal handler ===============================
  
  void
  MainWindow::OnAboutDialogSignalResponse(int /* response_id */)
  {
    mp_about_dialog->hide();
  }
  
  void
  MainWindow::OnSerialConsoleSignalResponse(int /* response_id */)
  {
    mpMenuItemToolShowSerialConsole->set_active(false);
  }
  
  // File -> Exit
  
  void
  MainWindow::OnMenuFileExit()
  {
    hide();
  }
  
  void
  MainWindow::OnMenuItemFileQuitSelect()
  { mStatusBar.push(gettext("Quit ARMware"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemFileQuitDeselect()
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Operation -> Power On
  
  void
  MainWindow::OnMenuOperationPowerOn()
  {
    mpCurrMachine = mMachineListScrolledWindow.SelectedMachine();
    
    if (mpCurrMachine != 0)
    {
      mCurrMachine = *mpCurrMachine;
      
      Database::put_database();
      
      mpCurrMachine = 0;
      
      mpPowerOnButton->set_sensitive(false);
      mpResetButton->set_sensitive(true);
      mpPowerOffButton->set_sensitive(true);
      
      mpMenuItemOperationPowerOn->set_sensitive(false);
      mpMenuItemOperationReset->set_sensitive(true);
      mpMenuItemOperationPowerOff->set_sensitive(true);
      
      m_machine_screen.change_screen_size(mCurrMachine.get_screen_width(),
                                          mCurrMachine.get_screen_height());
      
      mMachineListScrolledWindow.DisableSelection();
      
      mLogoAlignment.reference();
      
      mVBoxRight.remove(mLogoAlignment);
      mVBoxRight.pack_start(m_machine_screen, Gtk::PACK_EXPAND_WIDGET);
      
      m_machine_screen.unreference();
      
      start_machine(mCurrMachine, &m_machine_screen,
                    &m_soft_keyboard, *mp_serial_console);
    }
    else
    {
      ErrorDialog dialog(*this);
      
      dialog.add_message(gettext("You have to Select a machine first"));
      
      dialog.run();
    }
  }
  
  void
  MainWindow::OnMenuItemOperationPowerOnSelect(GdkEventCrossing * /* event */)
  { mStatusBar.push(gettext("Power on the selected machine"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemOperationPowerOnDeselect(GdkEventCrossing * /* event */)
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Operation -> Reset
  
  void
  MainWindow::OnMenuOperationReset()
  {
  }
  
  void
  MainWindow::OnMenuItemOperationResetSelect(GdkEventCrossing * /* event */)
  { mStatusBar.push(gettext("Reset the selected machine"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemOperationResetDeselect(GdkEventCrossing * /* event */)
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Operation -> Power Off
  
  void
  MainWindow::OnMenuOperationPowerOff()
  {
    mpPowerOnButton->set_sensitive(true);
    mpResetButton->set_sensitive(false);
    mpPowerOffButton->set_sensitive(false);
    
    mpMenuItemOperationPowerOn->set_sensitive(true);
    mpMenuItemOperationReset->set_sensitive(false);
    mpMenuItemOperationPowerOff->set_sensitive(false);
    
    mMachineListScrolledWindow.EnableSelection();
    
    m_machine_screen.reference();
    
    mVBoxRight.remove(m_machine_screen);
    mVBoxRight.pack_start(mLogoAlignment, Gtk::PACK_EXPAND_WIDGET);
    
    mLogoAlignment.unreference();
  }
  
  void
  MainWindow::OnMenuItemOperationPowerOffSelect(GdkEventCrossing * /* event */)
  { mStatusBar.push(gettext("Power off the selected machine"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemOperationPowerOffDeselect(GdkEventCrossing * /* event */)
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Tool -> Serial Console
  
  void
  MainWindow::OnMenuItemToolShowSerialConsole()
  {
    if (true == mp_serial_console->is_visible())
    {
      mp_serial_console->hide();
    }
    else
    {
      mp_serial_console->show();
    }
  }
  
  void
  MainWindow::OnMenuItemToolSerialConsoleSelect()
  {
    mStatusBar.push(gettext("Open a window to show the output from the serial console of the selected machine"),
                    mStatusBarContextId);
  }
  
  void
  MainWindow::OnMenuItemToolSerialConsoleDeselect()
  {
    mStatusBar.pop(mStatusBarContextId);
  }
  
  // Tool -> Show Machine List
  
  void
  MainWindow::OnMenuItemToolShowMachineList()
  {
    if (false == mMachineListScrolledWindow.is_visible())
    {
      mMachineListScrolledWindow.show();
      static_cast<Gtk::Arrow *>(mMachineListShowHideBar.get_child())->set(Gtk::ARROW_LEFT, Gtk::SHADOW_OUT);
      
      if (true == m_finish_start)
      {
        resize(m_width + mMachineListScrolledWindow.get_width(), m_height);
      }
    }
    else
    {
      mMachineListScrolledWindow.hide();
      static_cast<Gtk::Arrow *>(mMachineListShowHideBar.get_child())->set(Gtk::ARROW_RIGHT, Gtk::SHADOW_OUT);
      
      if (true == m_finish_start)
      {
        resize(m_width - mMachineListScrolledWindow.get_width(), m_height);
      }
    }
  }
  
  void
  MainWindow::OnMenuItemToolShowMachineListSelect()
  { mStatusBar.push(gettext("Show machine list"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemToolShowMachineListDeselect()
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Tool -> Show Soft Keyboard
  
  void
  MainWindow::OnMenuItemToolShowSoftKeyboard()
  {
    if (0 == mMachineListScrolledWindow.SelectedMachine())
    {
      return;
    }
    
    if (false == m_soft_keyboard.is_visible())
    {
      m_soft_keyboard.show();
      static_cast<Gtk::Arrow *>(m_soft_keyboard_show_hide_bar.get_child())->set(Gtk::ARROW_UP, Gtk::SHADOW_OUT);
      
      if (true == m_finish_start)
      {
        resize(m_width, m_height + m_soft_keyboard.get_height());
      }
    }
    else
    {
      m_soft_keyboard.hide();
      static_cast<Gtk::Arrow *>(m_soft_keyboard_show_hide_bar.get_child())->set(Gtk::ARROW_DOWN, Gtk::SHADOW_OUT);
      
      if (true == m_finish_start)
      {
        resize(m_width, m_height - m_soft_keyboard.get_height());
      }
    }
  }
  
  void
  MainWindow::OnMenuItemToolShowSoftKeyboardSelect()
  { mStatusBar.push(gettext("Show soft keyboard"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemToolShowSoftKeyboardDeselect()
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Tool -> Preference
  
  void
  MainWindow::OnMenuItemToolPreference()
  {
    PrefsDialog prefs_dialog;
    int result = prefs_dialog.run();
    
    switch (result)
    {
    case Gtk::RESPONSE_OK:
      break;
      
    case Gtk::RESPONSE_CANCEL:
      break;
      
    case Gtk::RESPONSE_APPLY:
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  void
  MainWindow::OnMenuItemToolPreferenceSelect()
  { mStatusBar.push(gettext("Open preference dialog"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemToolPreferenceDeselect()
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Help -> About
  
  void
  MainWindow::OnMenuHelpAbout()
  {
    if (0 == mp_about_dialog)
    {
      mp_about_dialog = new AboutDialog();
      
      mp_about_dialog->signal_response().connect(sigc::mem_fun(*this, &MainWindow::OnAboutDialogSignalResponse));
    }
    
    mp_about_dialog->show();
  }
  
  void
  MainWindow::OnMenuItemHelpAboutSelect()
  { mStatusBar.push(gettext("Information about ARMware"), mStatusBarContextId); }
  
  void
  MainWindow::OnMenuItemHelpAboutDeselect()
  { mStatusBar.pop(mStatusBarContextId); }
  
  // Machine List Show Hide Bar
  
  void
  MainWindow::OnMachineListShowHideBarClicked()
  {
    if (true == mMachineListScrolledWindow.is_visible())
    {
      mpMenuItemToolShowMachineList->set_active(false);
    }
    else
    {
      mpMenuItemToolShowMachineList->set_active(true);
    }
  }
  
  bool
  MainWindow::OnMachineListShowHideBarEnterNotifyEvent(GdkEventCrossing * /* event */)
  {
    if (true == mMachineListScrolledWindow.is_visible())
    {
      mStatusBar.push(gettext("Hide machine list"), mStatusBarContextId);
    }
    else
    {
      mStatusBar.push(gettext("Show machine list"), mStatusBarContextId);
    }
    
    return false;
  }
  
  bool
  MainWindow::OnMachineListShowHideBarLeaveNotifyEvent(GdkEventCrossing * /* event */)
  {
    mStatusBar.pop(mStatusBarContextId);
    return false;
  }
  
  // Soft Keyboard Show Hide Bar
  
  void
  MainWindow::OnSoftKeyboardShowHideBarClicked()
  {
    if (true == m_soft_keyboard.is_visible())
    {
      mpMenuItemToolShowSoftKeyboard->set_active(false);
    }
    else
    {
      mpMenuItemToolShowSoftKeyboard->set_active(true);
    }
  }
  
  bool
  MainWindow::OnSoftKeyboardShowHideBarEnterNotifyEvent(GdkEventCrossing * /* event */)
  {
    if (true == m_soft_keyboard.is_visible())
    {
      mStatusBar.push(gettext("Hide soft keyboard"), mStatusBarContextId);
    }
    else
    {
      mStatusBar.push(gettext("Show soft keyboard"), mStatusBarContextId);
    }
    
    return false;
  }
  
  bool
  MainWindow::OnSoftKeyboardShowHideBarLeaveNotifyEvent(GdkEventCrossing * /* event */)
  {
    mStatusBar.pop(mStatusBarContextId);
    
    return false;
  }
  
  void
  MainWindow::OnSizeAllocate(Gtk::Allocation &alloc)
  {
    m_width = alloc.get_width();
    m_height = alloc.get_height();
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================= Life cycle ==================================
  
  MainWindow::MainWindow()
    : m_soft_keyboard_show_hide_bar_arrow(Gtk::ARROW_DOWN, Gtk::SHADOW_OUT),
      mMachineListShowHideBarArrow(Gtk::ARROW_LEFT, Gtk::SHADOW_OUT),
      mp_about_dialog(0),
      mp_serial_console(new SerialConsole()),
      mShowSerialConsoleByDefault(false),
      mShowSoftKeyboardByDefault(false),
      mShowMachineListByDefault(true),
      m_finish_start(false),
      m_width(0),
      m_height(0),
      mMachineListScrolledWindow(*this),
      mpCurrMachine(0)
  {
    set_title(gettext("ARMware"));
    
    mp_serial_console->signal_response().connect(sigc::mem_fun(*this, &MainWindow::OnSerialConsoleSignalResponse));
    
    mStatusBarContextId = mStatusBar.get_context_id("ARMware StatusBar Context Id");
    
    // File
    
    mMenuBar.items().push_back(Gtk::Menu_Helpers::MenuElem(gettext("_File"), mMenuFile));
    
    // Operation
    
    mMenuBar.items().push_back(Gtk::Menu_Helpers::MenuElem(gettext("_Operation"), mMenuOperation));
    
    // Tool
    
    mMenuBar.items().push_back(Gtk::Menu_Helpers::MenuElem(gettext("_Tool"), mMenuTool));
    
    // Help
    
    mMenuBar.items().push_back(Gtk::Menu_Helpers::MenuElem(gettext("_Help"), mMenuHelp));
    
    // File -> ...
    
    {
      Gtk::Menu::MenuList &menu_list = mMenuFile.items();
      
      menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("Quit"), Gtk::AccelKey("<control>q"),
                                                      sigc::mem_fun(*this, &MainWindow::OnMenuFileExit)));
      
      mMenuFile.items().back().signal_select().connect(sigc::mem_fun(*this, &MainWindow::OnMenuItemFileQuitSelect));
      mMenuFile.items().back().signal_deselect().connect(sigc::mem_fun(*this, &MainWindow::OnMenuItemFileQuitDeselect));
    }
    
    // Operation -> ...
    
    {
      Gtk::Menu::MenuList &menu_list = mMenuOperation.items();
      
      menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("Power On"),
                                                      Gtk::AccelKey("<control>n"),
                                                      sigc::mem_fun(*this, &MainWindow::OnMenuOperationPowerOn)));
      
      mpMenuItemOperationPowerOn = &(mMenuOperation.items().back());
      
      mpMenuItemOperationPowerOn->set_sensitive(true);
      mpMenuItemOperationPowerOn->signal_select().connect(
        sigc::bind<GdkEventCrossing *>(
          sigc::mem_fun(*this, &MainWindow::OnMenuItemOperationPowerOnSelect), 0));
      mpMenuItemOperationPowerOn->signal_deselect().connect(
        sigc::bind<GdkEventCrossing *>(
          sigc::mem_fun(*this, &MainWindow::OnMenuItemOperationPowerOnDeselect), 0));
      
      menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("Reset"),
                                                      Gtk::AccelKey("<control>r"),
                                                      sigc::mem_fun(*this, &MainWindow::OnMenuOperationReset)));
      
      mpMenuItemOperationReset = &(mMenuOperation.items().back());
      
      mpMenuItemOperationReset->set_sensitive(false);
      mpMenuItemOperationReset->signal_select().connect(
        sigc::bind<GdkEventCrossing *>(
          sigc::mem_fun(*this, &MainWindow::OnMenuItemOperationResetSelect), 0));
      mpMenuItemOperationReset->signal_deselect().connect(
        sigc::bind<GdkEventCrossing *>(
          sigc::mem_fun(*this, &MainWindow::OnMenuItemOperationResetDeselect), 0));
      
      menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("Power Off"),
                                                      Gtk::AccelKey("<control>f"),
                                                      sigc::mem_fun(*this, &MainWindow::OnMenuOperationPowerOff)));
      
      mpMenuItemOperationPowerOff = &(mMenuOperation.items().back());
      
      mpMenuItemOperationPowerOff->set_sensitive(false);
      mpMenuItemOperationPowerOff->signal_select().connect(
        sigc::bind<GdkEventCrossing *>(
          sigc::mem_fun(*this, &MainWindow::OnMenuItemOperationPowerOffSelect), 0));
      mpMenuItemOperationPowerOff->signal_deselect().connect(
        sigc::bind<GdkEventCrossing *>(
          sigc::mem_fun(*this, &MainWindow::OnMenuItemOperationPowerOffDeselect), 0));
    }
    
    // Tool -> ...
    
    {
      Gtk::Menu::MenuList &menu_list = mMenuTool.items();
      
      menu_list.push_back(Gtk::Menu_Helpers::CheckMenuElem(gettext("Open Serial Console Output Window"),
                                                           Gtk::AccelKey("<control>s"),
                                                           sigc::mem_fun(*this, &MainWindow::OnMenuItemToolShowSerialConsole)));
      
      mpMenuItemToolShowSerialConsole = static_cast<Gtk::CheckMenuItem *>(&(mMenuTool.items().back()));
      
      mMenuTool.items().back().signal_select().connect(sigc::mem_fun(*this,
                                                                  &MainWindow::OnMenuItemToolSerialConsoleSelect));
      mMenuTool.items().back().signal_deselect().connect(sigc::mem_fun(*this,
                                                                    &MainWindow::OnMenuItemToolSerialConsoleDeselect));
      
      menu_list.push_back(Gtk::Menu_Helpers::SeparatorElem());
      
      menu_list.push_back(Gtk::Menu_Helpers::CheckMenuElem(gettext("Show Machine List"), Gtk::AccelKey("<control>m"),
                                                           sigc::mem_fun(*this, &MainWindow::OnMenuItemToolShowMachineList)));
      
      mpMenuItemToolShowMachineList = static_cast<Gtk::CheckMenuItem *>(&(mMenuTool.items().back()));
      
      mpMenuItemToolShowMachineList->signal_select().
        connect(sigc::mem_fun(*this, &MainWindow::OnMenuItemToolShowMachineListSelect));
      mpMenuItemToolShowMachineList->signal_deselect().
        connect(sigc::mem_fun(*this, &MainWindow::OnMenuItemToolShowMachineListDeselect));
      
      menu_list.push_back(Gtk::Menu_Helpers::CheckMenuElem(gettext("Show Soft Keyboard"), Gtk::AccelKey("<control>k"),
                                                           sigc::mem_fun(*this, &MainWindow::OnMenuItemToolShowSoftKeyboard)));
      
      mpMenuItemToolShowSoftKeyboard = static_cast<Gtk::CheckMenuItem *>(&(mMenuTool.items().back()));
      
      mMenuTool.items().back().signal_select().connect(sigc::mem_fun(*this,
                                                                  &MainWindow::OnMenuItemToolShowSoftKeyboardSelect));
      mMenuTool.items().back().signal_deselect().connect(sigc::mem_fun(*this,
                                                                    &MainWindow::OnMenuItemToolShowSoftKeyboardDeselect));
      
      menu_list.push_back(Gtk::Menu_Helpers::SeparatorElem());
      
      menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("Preference"), Gtk::AccelKey("<control>r"),
                                                      sigc::mem_fun(*this, &MainWindow::OnMenuItemToolPreference)));
      
      mMenuTool.items().back().signal_select().connect(sigc::mem_fun(*this,
                                                                  &MainWindow::OnMenuItemToolPreferenceSelect));
      mMenuTool.items().back().signal_deselect().connect(sigc::mem_fun(*this,
                                                                    &MainWindow::OnMenuItemToolPreferenceDeselect));
    }
    
    // Help -> ...
    
    {
      Gtk::Menu::MenuList &menu_list = mMenuHelp.items();
      
      menu_list.push_back(Gtk::Menu_Helpers::MenuElem(gettext("About"), Gtk::AccelKey("<control>a"),
                                                      sigc::mem_fun(*this, &MainWindow::OnMenuHelpAbout)));
      
      mMenuHelp.items().back().signal_select().connect(sigc::mem_fun(*this, &MainWindow::OnMenuItemHelpAboutSelect));
      mMenuHelp.items().back().signal_deselect().connect(sigc::mem_fun(*this, &MainWindow::OnMenuItemHelpAboutDeselect));
    }
    
    // Tool Bar
    
    char const *power_on_pixmap_path;
    char const *power_off_pixmap_path;
    char const *reset_pixmap_path;
    char const *logo_pixmap_path;
    
#if WIN32
    char * const base_path = getenv("ARMWARE_BASEPATH");
    assert(base_path != 0);
    
    std::string power_on_pixmap_path_string = base_path;
    power_on_pixmap_path_string += "\\pixmaps\\power_on.png";
    
    std::string power_off_pixmap_path_string = base_path;
    power_off_pixmap_path_string += "\\pixmaps\\power_off.png";
    
    std::string reset_pixmap_path_string = base_path;
    reset_pixmap_path_string += "\\pixmaps\\reset.png";
    
    std::string logo_pixmap_path_string = base_path;
    logo_pixmap_path_string += "\\pixmaps\\logo.png";
    
    power_on_pixmap_path = power_on_pixmap_path_string.c_str();
    power_off_pixmap_path = power_off_pixmap_path_string.c_str();
    reset_pixmap_path = reset_pixmap_path_string.c_str();
    logo_pixmap_path = logo_pixmap_path_string.c_str();
#elif LINUX
    power_on_pixmap_path = INSTALL_PIXMAP_DIR"power_on.png";
    power_off_pixmap_path = INSTALL_PIXMAP_DIR"power_off.png";
    reset_pixmap_path = INSTALL_PIXMAP_DIR"reset.png";
    logo_pixmap_path = INSTALL_PIXMAP_DIR"logo.png";
#endif
    
    Gtk::Image * const p_power_on_image = Gtk::manage(new Gtk::Image(power_on_pixmap_path));
    Gtk::Image * const p_power_off_image = Gtk::manage(new Gtk::Image(power_off_pixmap_path));
    Gtk::Image * const p_reset_image = Gtk::manage(new Gtk::Image(reset_pixmap_path));
    
    mpPowerOnButton = Gtk::manage(new Gtk::ToolButton(*p_power_on_image, gettext("Power On")));
    mpResetButton = Gtk::manage(new Gtk::ToolButton(*p_reset_image, gettext("Reset")));
    mpPowerOffButton = Gtk::manage(new Gtk::ToolButton(*p_power_off_image, gettext("Power Off")));
    
    mToolBar.append(*mpPowerOnButton, sigc::mem_fun(*this, &MainWindow::OnMenuOperationPowerOn));
    mToolBar.append(*mpResetButton, sigc::mem_fun(*this, &MainWindow::OnMenuOperationReset));
    mToolBar.append(*mpPowerOffButton, sigc::mem_fun(*this, &MainWindow::OnMenuOperationPowerOff));
    
    mpResetButton->set_sensitive(false);
    mpPowerOffButton->set_sensitive(false);
    
    mMachineListShowHideBar.add(mMachineListShowHideBarArrow);
    
    mMachineListShowHideBar.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::OnMachineListShowHideBarClicked));
    mMachineListShowHideBar.signal_enter_notify_event().connect_notify(
      SigC::hide_return(sigc::mem_fun(*this, &MainWindow::OnMachineListShowHideBarEnterNotifyEvent)));
    mMachineListShowHideBar.signal_leave_notify_event().connect_notify(
      SigC::hide_return(sigc::mem_fun(*this, &MainWindow::OnMachineListShowHideBarLeaveNotifyEvent)));
    
    m_soft_keyboard_show_hide_bar.add(m_soft_keyboard_show_hide_bar_arrow);
    
    m_soft_keyboard_show_hide_bar.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::OnSoftKeyboardShowHideBarClicked));
    m_soft_keyboard_show_hide_bar.signal_enter_notify_event().connect_notify(
      SigC::hide_return(sigc::mem_fun(*this, &MainWindow::OnSoftKeyboardShowHideBarEnterNotifyEvent)));
    m_soft_keyboard_show_hide_bar.signal_leave_notify_event().connect_notify(
      SigC::hide_return(sigc::mem_fun(*this, &MainWindow::OnSoftKeyboardShowHideBarLeaveNotifyEvent)));
    
    signal_size_allocate().connect(sigc::mem_fun(*this, &MainWindow::OnSizeAllocate));
    
    Gtk::Image * const p_logo_image = Gtk::manage(new Gtk::Image(logo_pixmap_path));
    p_logo_image->set_alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER);
    
    Gtk::Label * const p_logo_label_1 = Gtk::manage(new Gtk::Label(gettext("ARMware")));
    Gtk::Label * const p_logo_label_2 = Gtk::manage(new Gtk::Label(gettext("A virtual machine for embedded system")));
    Gtk::Label * const p_logo_label_3 = Gtk::manage(new Gtk::Label(gettext("Version")));
    
    p_logo_label_1->set_justify(Gtk::JUSTIFY_CENTER);
    p_logo_label_2->set_justify(Gtk::JUSTIFY_CENTER);
    p_logo_label_3->set_justify(Gtk::JUSTIFY_CENTER);
    
    Gtk::VBox * const p_logo_label_vbox = Gtk::manage(new Gtk::VBox());
    
    p_logo_label_vbox->pack_start(*p_logo_label_1, Gtk::PACK_SHRINK);
    p_logo_label_vbox->pack_start(*p_logo_label_2, Gtk::PACK_SHRINK);
    p_logo_label_vbox->pack_start(*p_logo_label_3, Gtk::PACK_SHRINK);
    
    Gtk::VBox * const p_vbox_logo = Gtk::manage(new Gtk::VBox());
    
    p_vbox_logo->set_border_width(Layout::BORDER_WIDTH);
    p_vbox_logo->set_spacing(Layout::SPACING);
    p_vbox_logo->pack_start(*p_logo_image, Gtk::PACK_EXPAND_WIDGET);
    p_vbox_logo->pack_start(*p_logo_label_vbox, Gtk::PACK_SHRINK);
    
    mLogoAlignment.set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, 0.0, 0.0);
    mLogoAlignment.add(*p_vbox_logo);
    
    mVBoxRight.pack_start(mLogoAlignment, Gtk::PACK_EXPAND_WIDGET);
    mVBoxRight.pack_end(m_soft_keyboard_show_hide_bar, Gtk::PACK_SHRINK);
    mVBoxRight.pack_end(m_soft_keyboard, Gtk::PACK_SHRINK);
    
    Gtk::HBox * const p_hbox_center = Gtk::manage(new Gtk::HBox());
    
    p_hbox_center->pack_start(mVBoxRight, Gtk::PACK_EXPAND_WIDGET);
    p_hbox_center->pack_start(mMachineListScrolledWindow, Gtk::PACK_SHRINK);
    p_hbox_center->pack_start(mMachineListShowHideBar, Gtk::PACK_SHRINK);
    
    Gtk::VBox * const p_vbox_topmost = Gtk::manage(new Gtk::VBox());
    
    p_vbox_topmost->pack_start(mMenuBar, Gtk::PACK_SHRINK);
    p_vbox_topmost->pack_start(mToolBar, Gtk::PACK_SHRINK);
    p_vbox_topmost->pack_start(*p_hbox_center, Gtk::PACK_EXPAND_WIDGET);
    p_vbox_topmost->pack_start(mStatusBar, Gtk::PACK_SHRINK);
    
    add(*p_vbox_topmost);
    
    show_all_children();
    m_machine_screen.show();
    
    m_soft_keyboard.hide();
    mMachineListScrolledWindow.hide();
    
    if (true == mShowSerialConsoleByDefault)
    {
      mpMenuItemToolShowSerialConsole->set_active(true);
    }
    
    if (true == mShowMachineListByDefault)
    {
      mpMenuItemToolShowMachineList->set_active(true);
    }
    
    if (true == mShowSoftKeyboardByDefault)
    {
      mpMenuItemToolShowSoftKeyboard->set_active(true);
    }
    
    // :NOTE: Wei 2004-Apr-13:
    //
    // The initial reference count of an widget which has already packed into an container is 2.
    // (I don't know why, if I have time, I will trace into gtk & glib to see why)
    //
    // Thus I have to increment m_machine_screen's reference count to 2 to
    // emulate which has packed into mVBoxRight, just like mLogoAlignment.
    m_machine_screen.reference();
    
    // :NOTE: Wei 2004-Mar-31:
    //
    // get the smallest possible size of MachineScreenScrolledWindow.
    Gtk::Requisition req1;
    mMenuBar.size_request(req1);
    
    Gtk::Requisition req2;
    mMachineListShowHideBar.size_request(req2);
    
    m_machine_screen.set_size_request(req1.width - req2.width, 300);
    mLogoAlignment.set_size_request(req1.width - req2.width, 300);
    
    m_finish_start = true;
    
    // :NOTE: Wei 2004-Jun-29:
    //
    // re-drawing the screen 10 times per second.
    Glib::signal_timeout().connect(sigc::mem_fun(&m_machine_screen,
                                                 &MachineScreen::draw_screen), 1000 / 10);
  }
  
  MainWindow::~MainWindow()
  {
    Database::put_database();
    
    if (mp_about_dialog != 0)
    {
      delete mp_about_dialog;
    }
    
    assert(mp_serial_console != 0);
    
    delete mp_serial_console;
  }
}
