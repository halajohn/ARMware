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

#ifndef __platform_dep_hpp__
#define __platform_dep_hpp__

#include <cstddef>

#include "Type.hpp"

#if WIN32

#define NORETURN
#define GTK_FONT_ENCODING_NAME "UTF-8"

#ifdef BUILDING_DLL
#define MAIN_DLL_EXPORT __declspec(dllimport)
#define DLL_DLL_EXPORT __declspec(dllexport)
#else
#define MAIN_DLL_EXPORT __declspec(dllexport)
#define DLL_DLL_EXPORT __declspec(dllimport)
#endif

#elif LINUX

#define NORETURN __attribute__((__noreturn__))
#define GTK_FONT_ENCODING_NAME "UTF8"

#define MAIN_DLL_EXPORT
#define DLL_DLL_EXPORT

#else
#error "Unknown platform."
#endif

#if WIN32
typedef int mode_t;

#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#define S_IRWXU    (_S_IREAD | _S_IWRITE | _S_IEXEC)

extern int mkdir(
  char const *pathname,
  mode_t mode);
#endif

extern void *library_load(
  char const * const filename);

// 0 means success, otherwise failed.
extern int library_free(
  void * const library);

extern void *library_get_symbol(
  void * const library,
  char const * const name);

#if LINUX

#define EXCEPTION_PROTECTION_BLOCK_BEGIN
#define EXCEPTION_PROTECTION_BLOCK_END
#define EXCEPTION_HANDLER_PROTOTYPE

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
  // register using the "SetUnhandledExceptionFilter" API
  // (However, in the debug build, it still calls it,
  // because the debug build ARMware.exe uses "CRTD").
  //
  // Hence, to be worked on both release and debug build, I
  // will not register an unhandled exception handler,
  // instead, I will use an explicit "__try" and "__except"
  // block to protect the code segment where exceptions may
  // occur.
#define EXCEPTION_PROTECTION_BLOCK_BEGIN \
  __try                                  \
  {

#define EXCEPTION_PROTECTION_BLOCK_END                        \
  }                                                           \
  __except(::segfault_handler(GetExceptionInformation()))     \
  {                                                           \
  }

#define EXCEPTION_HANDLER_PROTOTYPE             \
  extern LONG WINAPI                            \
  segfault_handler(                             \
    struct _EXCEPTION_POINTERS *exc_info);

#else
#error "Platform doesn't support sigaction and we have no implementation."
#endif

extern void install_exception_handler_for_access_violation(
  void (*handler)(void));

extern int memory_protect(
  void *addr,
  size_t len);

extern int memory_unprotect(
  void *addr,
  size_t len);

extern void *memory_allocate(
  size_t const size);

extern void memory_release(
  ARMware::uint8_t * const memory,
  size_t const size);

extern ARMware::uint32_t get_native_page_size();

#endif
