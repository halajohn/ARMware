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

// This is the memory used in computers, such like DRAM etc.
//

#include <csignal>

#include "Memory.hpp"
#include "ChunkChain.hpp"
#include "platform_dep.hpp"

namespace ARMware
{
  //////////////////////////////// Private ////////////////////////////////////
  
#if ENABLE_THREADED_CODE
  uint32_t Memory::ms_curr_addr;
  uint8_t *Memory::ms_memory;
  ChunkChain *Memory::ms_chunk_chain;
  
  void
  Memory::segfault_handler()
  {
    ms_chunk_chain->flush_chunk(ms_curr_addr);
    
    // :NOTE: Wei 2004-Aug-11:
    //
    // Make the desired memory region writable.
#if CHECK_MPROTECT
    if (
#endif
      // :NOTE: Wei 2004-Aug-18:
      //
      // Although mprotect() isn't in SUSv3's list of async-signal-safe funtions,
      // (it isn't in POSIX 1003.1-2003's list, either).
      //
      // I still call it (i.e. mprotect()) in my SEGV signal handler.
      // That is because I always call mprotect() with valid arguments (aligned,
      // references to mapped pages, and with an access that is compatible with
      // the access of the underlying object).
      //
      // Thus, I can assume that the call to mprotect() always succeeds.
      //
      // (If a function isn't a async-signal safe function, however,
      //  if it's known that the wanted signal didn't happen inside it, then it is safe
      //  to call it inside the signal handler corresponding to that signal.)
      memory_unprotect(page_aligned_addr(&(ms_memory[ms_curr_addr])),
                       NATIVE_PAGE_SIZE)
#if CHECK_MPROTECT
      != 0)
    {
      perror("Fail to mprotect()");
      assert(!"Should not reach here.");
    }
#else
    ;
#endif
  }
#endif
  
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Life cycle =================================
  
  Memory::Memory(uint32_t const size, MachineScreen * const machine_screen)
    : m_size(size),
#if ENABLE_DRAWING_ASM
      
      // :NOTE: Wei 2004-Jul-12:
      //
      // The reason why I need to enlarge the size of the memory when using ENABLE_DRAWING_ASM
      // is because I will get 8 pixels data at most at once.
      //
      // Thus I have to increment the memory size to avoid the possible segmentation fault.
      //
      // Ex: when the frame buffer is at the end of the memory.
#if ENABLE_THREADED_CODE
      // :NOTE: Wei 2004-Aug-11:
      //
      // According to the POSIX.1b, mprotect() can be used only on regions of memory obtained from mmap().
      //
      // Although in the linux platform, large memory allocation will use mmap() to allocate memory.
      // However, to ensure memory allocation indeed uses mmap(), not just only in the linux platform,
      // I use mmap() to allocate memory specifically.
      //
      // mmap() will allocate page-aligned memory directly from the Linux kernel.
      mp_memory(static_cast<uint8_t *>(memory_allocate(page_aligned_size(size + (8 * MachineScreen::BYTES_PER_PIXEL))))),
#else
      mp_memory(new uint8_t[size + (8 * MachineScreen::BYTES_PER_PIXEL)]),
#endif
      
#else // ENABLE_DRAWING_ASM
      
#if ENABLE_THREADED_CODE
      mp_memory(static_cast<uint8_t *>(memory_allocate(page_aligned_size(size)))),
#else
      mp_memory(new uint8_t[size]),
#endif
      
#endif // ENABLE_DRAWING_ASM
      mp_machine_screen(machine_screen),
      m_lcd_status(0)
  {
#if ENABLE_THREADED_CODE || ENABLE_DYNAMIC_TRANSLATOR
    install_exception_handler_for_access_violation(segfault_handler);
    
    ms_memory = mp_memory;
#endif
  }
}
