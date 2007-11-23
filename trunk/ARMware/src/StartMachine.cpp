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

#if PRINT_CHUNKS_LENGTH
#include <csignal>
#endif

#include <memory>

#include "StartMachine.hpp"
#include "Core.hpp"

#include "Compiler/x86/Register.hpp"

namespace ARMware
{
  uint32_t
  start_machine(MachineImpl const &machineImpl,
                MachineScreen * const machine_screen,
                SoftKeyboard * const soft_keyboard,
                SerialConsole &serialConsole)
  {
#if ENABLE_THREADED_CODE || ENABLE_DYNAMIC_TRANSLATOR
    // Check the value of NATIVE_PAGE_SIZE
    assert(NATIVE_PAGE_SIZE == get_native_page_size());
#endif
    
    std::auto_ptr<Core> p_core(new Core(machineImpl,
                                        machine_screen,
                                        soft_keyboard,
                                        serialConsole));
    
    assert(p_core.get() != 0);
    
    gp_core = p_core.get();
    
#if PRINT_CHUNKS_LENGTH
    struct sigaction sa;
    
    sa.sa_handler = Core::int_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, 0);
#endif
    
#if ENABLE_DYNAMIC_TRANSLATOR
    Register::register_init();
#endif
    
    p_core->run();
    
    return 0;
  }
}
