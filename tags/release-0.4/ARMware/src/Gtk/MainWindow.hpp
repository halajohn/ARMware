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

#ifndef MainWindow_hpp
#define MainWindow_hpp

// System include
//

#include <gtkmm/alignment.h>
#include <gtkmm/window.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/box.h>
#include <gtkmm/arrow.h>

// Local include
//

#include "MachineListScrolledWindow.hpp"
#include "MachineScreen.hpp"
#include "SoftKeyboard.hpp"

namespace Gtk
{
  class Image;
}

namespace ARMware
{
  class AboutDialog;
  class SerialConsole;
  
  typedef class MainWindow MainWindow;
  class MainWindow : public Gtk::Window
  {
  private:
    
    // Atribute
    
    Gtk::VBox mVBoxRight;
    
    Gtk::MenuBar mMenuBar;
    
    Gtk::Menu mMenuFile;
    Gtk::Menu mMenuOperation;
    Gtk::Menu mMenuTool;
    Gtk::Menu mMenuHelp;
    
    Gtk::MenuItem *mpMenuItemOperationPowerOn;
    Gtk::MenuItem *mpMenuItemOperationReset;
    Gtk::MenuItem *mpMenuItemOperationPowerOff;
    
    Gtk::CheckMenuItem *mpMenuItemToolShowSerialConsole;
    Gtk::CheckMenuItem *mpMenuItemToolShowMachineList;
    Gtk::CheckMenuItem *mpMenuItemToolShowSoftKeyboard;
    
    Gtk::Alignment mLogoAlignment;
    
    Gtk::Statusbar mStatusBar;
    
    guint mStatusBarContextId;
    
    Gtk::Toolbar mToolBar;
    
    Gtk::ToolButton *mpPowerOnButton;
    Gtk::ToolButton *mpPowerOffButton;
    Gtk::ToolButton *mpResetButton;
    
    Gtk::Button  mMachineListShowHideBar;
    Gtk::Button  m_soft_keyboard_show_hide_bar;
    
    Gtk::Arrow m_soft_keyboard_show_hide_bar_arrow;
    Gtk::Arrow mMachineListShowHideBarArrow;
    
    SoftKeyboard m_soft_keyboard;
    MachineScreen m_machine_screen;
    
    AboutDialog   *mp_about_dialog;
    SerialConsole *mp_serial_console;
    
    bool mShowSerialConsoleByDefault;
    bool mShowSoftKeyboardByDefault;
    bool mShowMachineListByDefault;
    bool m_finish_start;
    
    int m_width;
    int m_height;
    
    MachineListScrolledWindow mMachineListScrolledWindow;
    MachineImpl     *mpCurrMachine;
    MachineImpl      mCurrMachine;
    
    // Signal handler
    
    void OnMenuFileExit();
    
    void OnMenuOperationPowerOn();
    void OnMenuOperationPowerOff();
    void OnMenuOperationReset();
    
    void OnMenuHelpAbout();
    
    void OnMenuItemFileQuitSelect();
    void OnMenuItemFileQuitDeselect();
    
    void OnMenuItemOperationPowerOnSelect(GdkEventCrossing *event);
    void OnMenuItemOperationPowerOnDeselect(GdkEventCrossing *event);
    void OnMenuItemOperationResetSelect(GdkEventCrossing *event);
    void OnMenuItemOperationResetDeselect(GdkEventCrossing *event);
    void OnMenuItemOperationPowerOffSelect(GdkEventCrossing *event);
    void OnMenuItemOperationPowerOffDeselect(GdkEventCrossing *event);
    
    void OnMenuItemToolShowSerialConsole();
    void OnMenuItemToolSerialConsoleSelect();
    void OnMenuItemToolSerialConsoleDeselect();
    void OnMenuItemToolShowMachineList();
    void OnMenuItemToolShowMachineListSelect();
    void OnMenuItemToolShowMachineListDeselect();
    void OnMenuItemToolShowSoftKeyboard();
    void OnMenuItemToolShowSoftKeyboardSelect();
    void OnMenuItemToolShowSoftKeyboardDeselect();
    void OnMenuItemToolPreference();
    void OnMenuItemToolPreferenceSelect();
    void OnMenuItemToolPreferenceDeselect();
    
    void OnMenuItemHelpAboutSelect();
    void OnMenuItemHelpAboutDeselect();
    
    void OnAboutDialogSignalResponse(int response_id);
    void OnSerialConsoleSignalResponse(int response_id);
    
    void OnMachineListShowHideBarClicked();
    bool OnMachineListShowHideBarEnterNotifyEvent(GdkEventCrossing *event);
    bool OnMachineListShowHideBarLeaveNotifyEvent(GdkEventCrossing *event);
    
    void OnSoftKeyboardShowHideBarClicked();
    bool OnSoftKeyboardShowHideBarEnterNotifyEvent(GdkEventCrossing *event);
    bool OnSoftKeyboardShowHideBarLeaveNotifyEvent(GdkEventCrossing *event);
    
    void OnSizeAllocate(Gtk::Allocation &alloc);
    
  public:
    
    // Life cycle
    
    MainWindow();
    ~MainWindow();
    
    // Operation
    
    MachineImpl *&
    rpCurrMachine()
    {
      return mpCurrMachine;
    }
  };
}

#endif
