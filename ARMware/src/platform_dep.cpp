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

#include <cassert>
#include "platform_dep.hpp"

#if WIN32

#include <windows.h>
#include "wcl_fmtstr/fmtstr.h"

#elif LINUX

#include <sys/mman.h>
#include <unistd.h>
#include <csignal>
#include <dlfcn.h>

#else
#error "unsupported platform."
#endif

#if WIN32
int
mkdir(
  char const *pathname,
  mode_t mode)
{
  assert(pathname != 0);
  
  wchar_t * const pathname_w = fmtstr_mbstowcs(pathname, NULL);
  assert(pathname_w != NULL);
  
  int const result = CreateDirectory(pathname_w, NULL);
  
  fmtstr_delete(static_cast<void *>(pathname_w));
  
  /* mkdir returns 0 for success, opposite of
   * CreateDirectory().
   */
  return ((result != 0) ? 0 : -1);
}
#elif LINUX
#else
#error "system without mkdir and we have no implementation."
#endif

void *
library_load(
  char const * const filename)
{
#if LINUX
  return ::dlopen(filename, RTLD_LAZY);
#elif WIN32
  wchar_t * const filename_w = fmtstr_mbstowcs(filename, NULL);
  
  HMODULE module = LoadLibrary(filename_w);
  assert(module != NULL);
  
  fmtstr_delete(static_cast<void *>(filename_w));
  
  return module;
#else
#error "Platform doesn't support dlopen and we have no implementation."
#endif
}

// 0 means success, otherwise failed.
int
library_free(
  void * const library)
{
  assert(library != 0);
  
#if LINUX
  return ::dlclose(library);
#elif WIN32
  int const result = FreeLibrary(static_cast<HMODULE>(library));
  
  return ((result != 0) ? 0 : -1);
#else
#error "Platform doesn't support dlclose and we have no implementation."
#endif
}

void *
library_get_symbol(
  void * const library,
  char const * const name)
{
  assert(library != 0);
  
#if LINUX
  return ::dlsym(library, name);
#elif WIN32
  return GetProcAddress(static_cast<HMODULE>(library), name);
#else
#error "Platform doesn't support dlsym and we have no implementation."
#endif
}

static void (*access_violation_handler)(void);

#if LINUX

static void
segfault_handler(int /* signum */)
{
  access_violation_handler();
}

#elif WIN32

MAIN_DLL_EXPORT LONG WINAPI
segfault_handler(
  struct _EXCEPTION_POINTERS *exc_info)
{
  // Check if the exception type is "ACCESS_VIOLATION" and
  // the operation which causes this exception is writing.
  if ((EXCEPTION_ACCESS_VIOLATION == exc_info->ExceptionRecord->ExceptionCode) &&
      (1 == exc_info->ExceptionRecord->ExceptionInformation[0]))
  {
    access_violation_handler();
    
    return EXCEPTION_CONTINUE_EXECUTION;
  }
  else
  {
    return EXCEPTION_CONTINUE_SEARCH;
  }
}

#else
#error "Platform doesn't support sigaction and we have no implementation."
#endif

void
install_exception_handler_for_access_violation(
  void (*handler)(void))
{
  access_violation_handler = handler;
  
#if LINUX
  struct sigaction sa;
  
  sa.sa_handler = segfault_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  
  sigaction(SIGSEGV, &sa, 0);
#elif WIN32
  // The unhandled exception handling on win32 platform
  // after vc++2005 is changed:
  //
  // "The are many situations in which your user-defined
  // Unhandled-Exception-Filter will never be called. This
  // is a major change to the previous versions of the CRT
  // and IMHO not very well documented."
  //
  // The release build uses "CRT", so in the release build,
  // ARMware.exe will not call the "segfault_handler" I
  // register using the following API (However, in the debug
  // build, it still calls it, because the debug build
  // ARMware.exe uses "CRTD").
  //
  // Hence, to be worked on both release and debug build, I
  // will not register an unhandled exception handler,
  // instead, I will use an explicit "__try" and "__except"
  // block to protect the code segment where exceptions may
  // occur.
  //
  // ... SetUnhandledExceptionFilter(segfault_handler);
#else
#error "Platform doesn't support sigaction and we have no implementation."
#endif
}

// 0 means success, otherwise failed.
int
memory_protect(
  void *addr,
  size_t len)
{
  assert(addr != 0);
  assert(len != 0);
  
#if LINUX
  // :NOTE: Wei 2004-Aug-18:
  //
  // Although mprotect() isn't in SUSv3's list of async-signal-safe
  // funtions, (it isn't in POSIX 1003.1-2003's list, either).
  //
  // I still call it (i.e. mprotect()) in my SEGV signal handler.
  // That is because I always call mprotect() with valid arguments
  // (aligned, references to mapped pages, and with an access that is
  // compatible with the access of the underlying object).
  //
  // Thus, I can assume that the call to mprotect() always succeeds.
  //
  // (If a function isn't a async-signal safe function, however,
  //  if it's known that the wanted signal didn't happen inside it,
  //  then it is safe to call it inside the signal handler
  //  corresponding to that signal.) 
  return mprotect(addr, len, PROT_READ);
#elif WIN32
  {
    static DWORD protect_junk;
    
    int const result = VirtualProtect(addr, len, PAGE_EXECUTE_READ, &protect_junk);
    
    return ((result != 0) ? 0 : -1);
  }
#else
#error "Platform doesn't support mprotect and we have no implementation."
#endif
}

// 0 means success, otherwise failed.
int
memory_unprotect(
  void *addr,
  size_t len)
{
  assert(addr != 0);
  assert(len != 0);
  
#if LINUX
  return mprotect(addr, len, PROT_READ | PROT_WRITE);
#elif WIN32
  {
    static DWORD protect_junk;
    
    int const result = VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &protect_junk);
    
    return ((result != 0) ? 0 : -1);
  }
#else
#error "Platform doesn't support mprotect and we have no implementation."
#endif
}

void *
memory_allocate(
  size_t const size)
{
#if LINUX
  // :NOTE: Wei 2004-Aug-11:
  //
  // According to the POSIX.1b, mprotect() can be used only on regions
  // of memory obtained from mmap().
  //
  // Although in the linux platform, large memory allocation will use
  // mmap() to allocate memory. However, to ensure memory allocation
  // indeed uses mmap(), not just only in the linux platform, I use
  // mmap() to allocate memory specifically. 
  //
  // mmap() will allocate page-aligned memory directly from the Linux
  // kernel.
  void *result = mmap(0,
                      size,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS,
                      0,
                      0);
  if (MAP_FAILED == result)
  {
    return 0;
  }
  else
  {
    return result;
  }
#elif WIN32
  return VirtualAlloc(NULL,
                      size,
                      MEM_COMMIT | MEM_RESERVE,
                      PAGE_EXECUTE_READWRITE);
#else
#error "Platform doesn't support mmap and we have no implementation."
#endif
}

void
memory_release(
  ARMware::uint8_t * const memory,
  size_t const size)
{
  assert(memory != 0);

#if LINUX
  munmap(memory, size);
#elif WIN32
  VirtualFree(memory, size, MEM_RELEASE);
#else
#error "Platform doesn't support mmap and we have no implementation."
#endif
}

ARMware::uint32_t
get_native_page_size()
{
#if LINUX
  return static_cast<ARMware::uint32_t>(sysconf(_SC_PAGESIZE));
#elif WIN32
  SYSTEM_INFO SysInfo;
  
  GetSystemInfo(&SysInfo);
  
  return static_cast<ARMware::uint32_t>(SysInfo.dwPageSize);
#else
#error
#endif
}
