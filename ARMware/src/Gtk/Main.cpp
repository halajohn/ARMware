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

#include <cstdlib> // for EXIT_SUCCESS

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/main.h>
#include <gtkmm/window.h>

#include "MainWindow.hpp"

#if WIN32
// for gettext
#include <libintl.h>
#endif

int
main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");
  bind_textdomain_codeset("armware", "UTF-8");
  textdomain("armware");
  
  Gtk::Main kit(argc, argv);
  
  ARMware::MainWindow window;
  
  Gtk::Main::run(window);
  
  return EXIT_SUCCESS;
}
