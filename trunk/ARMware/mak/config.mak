# ARMware - an ARM emulator
# Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

include $(BUILD_ROOT_DIR)/options.mak

CXX    = /usr/bin/g++
ECHO   = /bin/echo
RM     = /bin/rm
CP     = /bin/cp
SED    = /bin/sed
MKDIR  = /bin/mkdir
STRIP  = /usr/bin/strip
MSGFMT = /usr/bin/msgfmt
TOUCH  = /usr/bin/touch

###############################################################################

BUILD_SRC_DIR    = $(BUILD_ROOT_DIR)/src \
		   $(BUILD_ROOT_DIR)/src/Inst \
                   $(BUILD_ROOT_DIR)/src/Gtk \
		   $(BUILD_ROOT_DIR)/src/Database \
		   $(BUILD_ROOT_DIR)/src/Utils \
		   $(BUILD_ROOT_DIR)/src/Compiler \
		   $(BUILD_ROOT_DIR)/src/Compiler/x86

ifeq ($(CONFIG_DATABASE), GDBM)
BUILD_SRC_DIR += $(BUILD_ROOT_DIR)/src/Database/gdbm
endif

ifeq ($(CONFIG_DATABASE), XML)
BUILD_SRC_DIR += $(BUILD_ROOT_DIR)/src/Database/xml
endif

BUILD_OBJ_DIR    = $(BUILD_ROOT_DIR)/obj
BUILD_BIN_DIR    = $(BUILD_ROOT_DIR)/bin
BUILD_I18N_DIR   = $(BUILD_ROOT_DIR)/i18n
BUILD_PIXMAP_DIR = $(BUILD_ROOT_DIR)/pixmaps

###############################################################################

INSTALL_BIN_DIR    = $(INSTALL_PREFIX)/usr/bin
INSTALL_I18N_DIR   = $(INSTALL_PREFIX)/usr/share/locale
INSTALL_PIXMAP_DIR = $(INSTALL_PREFIX)/usr/share/armware/pixmaps/
INSTALL_DB_NAME    = .armware/db

###############################################################################

PACKAGE_I18N_FILE        = armware.mo
PACKAGE_PIXMAP_FILE      = $(BUILD_PIXMAP_DIR)/power_on.png \
                           $(BUILD_PIXMAP_DIR)/reset.png \
                           $(BUILD_PIXMAP_DIR)/power_off.png \
                           $(BUILD_PIXMAP_DIR)/logo.png
PACKAGE_CUSTOM_EXEC_FILE = $(BUILD_BIN_DIR)/armware
PACKAGE_OPTI_EXEC_FILE   = $(BUILD_BIN_DIR)/armware_opti

###############################################################################

OBJFILE_NAME_FILE = $(BUILD_MAK_DIR)/custom.share.files \
                    $(BUILD_MAK_DIR)/opti.share.files \
                    $(BUILD_MAK_DIR)/custom.inst.files \
                    $(BUILD_MAK_DIR)/opti.inst.files \
                    $(BUILD_MAK_DIR)/custom.gtk.files \
                    $(BUILD_MAK_DIR)/opti.gtk.files \
                    $(BUILD_MAK_DIR)/custom.database.common.files \
                    $(BUILD_MAK_DIR)/opti.database.common.files \
                    $(BUILD_MAK_DIR)/custom.database.dep.files \
                    $(BUILD_MAK_DIR)/opti.database.dep.files \
                    $(BUILD_MAK_DIR)/custom.utils.files \
                    $(BUILD_MAK_DIR)/opti.utils.files \
                    $(BUILD_MAK_DIR)/custom.compiler.files \
                    $(BUILD_MAK_DIR)/opti.compiler.files \
                    $(BUILD_MAK_DIR)/custom.compiler.x86.files \
                    $(BUILD_MAK_DIR)/opti.compiler.x86.files

###############################################################################

# Initialize CFLAGS

# :NOTE: Wei 2004-Oct-14:
#
# -march=pentium4 implies -mcpu=pentium4 -mmmx -msse -msse2

# :NOTE: Wei 2004-Oct-14:
#
# I remove '-Winline' for now, because the gcc 3.3 or 3.4 will report too much
# warnings about the inline failures.
#
# It's really annoying!
#
# After searching from Google, this too much warnings seems bugs in the current
# gcc implementations.
# Thus, if there are newer gcc versions, like gcc-3.4.3 or gcc-4.0,
# then I may turn on this '-Winline' flags again.
CUSTOM_CFLAGS = -pipe -march=pentium4 -mfpmath=sse,387 -Wall -W -Wundef -Wwrite-strings -Wredundant-decls -Wpointer-arith -Woverloaded-virtual -Wconversion `pkg-config gtkmm-2.4 --cflags`

ifeq ($(CONFIG_DATABASE), XML)
CUSTOM_CFLAGS += `pkg-config --cflags libxml++-2.6`
endif

DEFINES = -D'INSTALL_PIXMAP_DIR="$(INSTALL_PIXMAP_DIR)"' \
          -D'INSTALL_DB_NAME="$(INSTALL_DB_NAME)"' \
          -D'LINUX=1' \
          -D'WIN32=0'

CONFIG_ARCH_V1 = 0
CONFIG_ARCH_V2 = 0
CONFIG_ARCH_V3 = 0
CONFIG_ARCH_V4 = 0
CONFIG_ARCH_V5 = 0

CONFIG_ARCH_$(CONFIG_ARCH) = 1

DEFINES += -D'CONFIG_MACH_$(CONFIG_MACH)=1' -D'CONFIG_CPU_$(CONFIG_CPU)=1' -D'CONFIG_FLASH_$(CONFIG_FLASH)=1' -D'CONFIG_ARCH_V1=$(CONFIG_ARCH_V1)' -D'CONFIG_ARCH_V2=$(CONFIG_ARCH_V2)' -D'CONFIG_ARCH_V3=$(CONFIG_ARCH_V3)' -D'CONFIG_ARCH_V4=$(CONFIG_ARCH_V4)' -D'CONFIG_ARCH_V5=$(CONFIG_ARCH_V5)'

CONFIG_DATABASE_GDBM = 0
CONFIG_DATABASE_XML = 0

CONFIG_DATABASE_$(CONFIG_DATABASE)=1

DEFINES += -D'CONFIG_DATABASE_GDBM=$(CONFIG_DATABASE_GDBM)' -D'CONFIG_DATABASE_XML=$(CONFIG_DATABASE_XML)'

ifeq ($(CONFIG_T_VARIANT), true)
DEFINES += -D'CONFIG_T_VARIANT=1'
endif

ifeq ($(LOAD_ROM_INTO_MEMORY), true)
DEFINES += -D'LOAD_ROM_INTO_MEMORY=1'
endif

ifeq ($(USE_BALANCE_DOM_ALGO), true)
DEFINES += -D'USE_BALANCE_DOM_ALGO=1'
else
DEFINES += -D'USE_BALANCE_DOM_ALGO=0'
endif

CUSTOM_CFLAGS += $(DEFINES)
OPTI_CFLAGS := $(CUSTOM_CFLAGS)

# The '-malign-double' aligns the addresses of all double variables (8 bytes each) to a multiple of 8.
# This prevents having a double data value split between two cache lines, or even worse, between two
# pages in memory.
# In either case, a cache miss/page fault would occur causing additional latency.
# According to the Intel optimization manual, this has a high impact and is applicable to many types of programs.
#
# From the gcc info page, it says that -O3 will enable '-finline-functions'.
# However, when I use 'g++ -Q -v -O3 -march=pentium4 ddd.cpp' to check the enabled options,
# I don't find '-finline-functions'.
# Thus, I add '-finline-functions' below.
DEFAULT_OPTI_CFLAGS = -O3 -fomit-frame-pointer -ftracer -malign-double -minline-all-stringops -finline-functions -finline-limit=60000 -funit-at-a-time -ftree-loop-linear -ftree-loop-im -ftree-loop-ivcanon -ftree-vectorize

ifeq ($(DEBUG_MODE), true)
CUSTOM_CFLAGS += -g3 -gdwarf-2 -D'DEBUG_MODE=1'
else
CUSTOM_CFLAGS += $(DEFAULT_OPTI_CFLAGS) -D'DEBUG_MODE=0'
endif
OPTI_CFLAGS += $(DEFAULT_OPTI_CFLAGS) -D'DEBUG_MODE=0'

ifeq ($(PROFILE_MODE), true)
CUSTOM_CFLAGS += -pg
endif

ifeq ($(ENABLE_ASSERT), false)
CUSTOM_CFLAGS += -DNDEBUG
endif
OPTI_CFLAGS += -DNDEBUG

ifeq ($(ENABLE_COND_CODES_ASM), true)
CUSTOM_CFLAGS += -D'ENABLE_COND_CODES_ASM=1'
else
CUSTOM_CFLAGS += -D'ENABLE_COND_CODES_ASM=0'
endif
OPTI_CFLAGS += -D'ENABLE_COND_CODES_ASM=1'

ifeq ($(USE_SHIFTER_CARRY), true)
CUSTOM_CFLAGS += -D'USE_SHIFTER_CARRY=1'
else
CUSTOM_CFLAGS += -D'USE_SHIFTER_CARRY=0'
endif
OPTI_CFLAGS += -D'USE_SHIFTER_CARRY=0'

# :NOTE: Wei 2004-Jul-29:
#
# If I use debug mode, than gcc-3.3 will report an error about:
#
# error: can't find a register in class `GENERAL_REGS' while reloading `asm'
#
# for my 'MMX & SSE2 drawing optimization code'.
#
# After searching using Google, someone says that it is a bug in the GCC.
# I don't know which 2 CFLAGS cause this problem.
# Maybe I have to include -fomit-frame-pointer when using MMX & SSE2 codes.
# However, -fomit-frame-pointer will make the debug impossible.
# Thus, I disable the MMX & SSE2 optimization codes when I enable the debug code.

# :NOTE: Wei 2004-Aug-19:
#
# Because of the differences between the codes of ENABLE_DRAWING_ASM & !ENABLE_DRAWING_ASM,
# the screen of this 2 modes are slight different.
#
# ENABLE_DRAWING_ASM is more beautiful, and !ENABLE_DRAWING_ASM is uglier.

ifeq ($(DEBUG_MODE), true)
CUSTOM_CFLAGS += -D'ENABLE_DRAWING_ASM=0'
else

ifeq ($(ENABLE_DRAWING_ASM), true)
CUSTOM_CFLAGS += -D'ENABLE_DRAWING_ASM=1'
else
CUSTOM_CFLAGS += -D'ENABLE_DRAWING_ASM=0'
endif

endif
OPTI_CFLAGS += -D'ENABLE_DRAWING_ASM=1'

ifeq ($(ENABLE_COMPILER_MAGIC), true)

# When I turn on these compiler magics, gdb will not be able to do the debugging.
# Thus I only turn on these compiler magics when DEBUG_MODE==false
ifeq ($(DEBUG_MODE), false)
CUSTOM_CFLAGS += -D'FASTCALL=__attribute__((regparm (3)))'
CUSTOM_CFLAGS += -D'FUNCTION_CONST=__attribute__((const))'
else
CUSTOM_CFLAGS += -D'FASTCALL='
CUSTOM_CFLAGS += -D'FUNCTION_CONST='
endif

else
CUSTOM_CFLAGS += -D'FASTCALL='
CUSTOM_CFLAGS += -D'FUNCTION_CONST='
endif
OPTI_CFLAGS += -D'FASTCALL=__attribute__((regparm (3)))'
OPTI_CFLAGS += -D'FUNCTION_CONST=__attribute__((const))'

ifeq ($(CHECK_ASM_CORRECT), true)
CUSTOM_CFLAGS += -D'CHECK_ASM_CORRECT=1'
else
CUSTOM_CFLAGS += -D'CHECK_ASM_CORRECT=0'
endif
OPTI_CFLAGS += -D'CHECK_ASM_CORRECT=0'

ifeq ($(CHECK_UNPREDICTABLE), true)
CUSTOM_CFLAGS += -D'CHECK_UNPREDICTABLE=1'
else
CUSTOM_CFLAGS += -D'CHECK_UNPREDICTABLE=0'
endif
OPTI_CFLAGS += -D'CHECK_UNPREDICTABLE=0'

ifeq ($(CHECK_GTK_ERROR), true)
CUSTOM_CFLAGS += -D'CHECK_GTK_ERROR=1'
else
CUSTOM_CFLAGS += -D'CHECK_GTK_ERROR=0'
endif
OPTI_CFLAGS += -D'CHECK_GTK_ERROR=0'

ifeq ($(CHECK_PC_MODIFY), true)
CUSTOM_CFLAGS += -D'CHECK_PC_MODIFY=1'
else
CUSTOM_CFLAGS += -D'CHECK_PC_MODIFY=0'
endif
OPTI_CFLAGS += -D'CHECK_PC_MODIFY=0'

ifeq ($(CHECK_AMTEL_CHKSUM), true)
CUSTOM_CFLAGS += -D'CHECK_AMTEL_CHKSUM=1'
else
CUSTOM_CFLAGS += -D'CHECK_AMTEL_CHKSUM=0'
endif
OPTI_CFLAGS += -D'CHECK_AMTEL_CHKSUM=0'

ifeq ($(CHECK_EXCEPTION_RAISE), true)
CUSTOM_CFLAGS += -D'CHECK_EXCEPTION_RAISE=1'
else
CUSTOM_CFLAGS += -D'CHECK_EXCEPTION_RAISE=0'
endif
OPTI_CFLAGS += -D'CHECK_EXCEPTION_RAISE=0'

ifeq ($(ENABLE_INST_RECALL), true)
CUSTOM_CFLAGS += -D'ENABLE_INST_RECALL=1'
else
CUSTOM_CFLAGS += -D'ENABLE_INST_RECALL=0'
endif
OPTI_CFLAGS += -D'ENABLE_INST_RECALL=0'

# threaded code
ifeq ($(ENABLE_THREADED_CODE), true)
CUSTOM_CFLAGS += -D'ENABLE_THREADED_CODE=1'

ifeq ($(CHECK_MPROTECT), true)
CUSTOM_CFLAGS += -D'CHECK_MPROTECT=1'
else
CUSTOM_CFLAGS += -D'CHECK_MPROTECT=0'
endif

ifeq ($(CHECK_THREADED_CODE_PC), true)
CUSTOM_CFLAGS += -D'CHECK_THREADED_CODE_PC=1'
else
CUSTOM_CFLAGS += -D'CHECK_THREADED_CODE_PC=0'
endif

ifeq ($(CHECK_THREADED_CODE_LENGTH), true)
CUSTOM_CFLAGS += -D'CHECK_THREADED_CODE_LENGTH=1'
else
CUSTOM_CFLAGS += -D'CHECK_THREADED_CODE_LENGTH=0'
endif

else
CUSTOM_CFLAGS += -D'ENABLE_THREADED_CODE=0'
CUSTOM_CFLAGS += -D'CHECK_MPROTECT=0'
CUSTOM_CFLAGS += -D'CHECK_THREADED_CODE_PC=0'
CUSTOM_CFLAGS += -D'CHECK_THREADED_CODE_LENGTH=0'
endif
OPTI_CFLAGS += -D'ENABLE_THREADED_CODE=1'
OPTI_CFLAGS += -D'CHECK_MPROTECT=0'
OPTI_CFLAGS += -D'CHECK_THREADED_CODE_PC=0'
OPTI_CFLAGS += -D'CHECK_THREADED_CODE_LENGTH=0'
# end threaded code

ifeq ($(ENABLE_DYNAMIC_TRANSLATOR), true)
CUSTOM_CFLAGS += -D'ENABLE_DYNAMIC_TRANSLATOR=1'
else
CUSTOM_CFLAGS += -D'ENABLE_DYNAMIC_TRANSLATOR=0'
endif
OPTI_CFLAGS += -D'ENABLE_DYNAMIC_TRANSLATOR=1'

ifeq ($(ASSUME_NOT_USE_TINY_PAGE), true)
CUSTOM_CFLAGS += -D'ASSUME_NOT_USE_TINY_PAGE=1'
else
CUSTOM_CFLAGS += -D'ASSUME_NOT_USE_TINY_PAGE=0'
endif
OPTI_CFLAGS += -D'ASSUME_NOT_USE_TINY_PAGE=1'

ifeq ($(TRACE_MMU), true)
CUSTOM_CFLAGS += -D'TRACE_MMU=1'
else
CUSTOM_CFLAGS += -D'TRACE_MMU=0'
endif
OPTI_CFLAGS += -D'TRACE_MMU=0'

ifeq ($(TRACE_TLB), true)
CUSTOM_CFLAGS += -D'TRACE_TLB=1'
else
CUSTOM_CFLAGS += -D'TRACE_TLB=0'
endif
OPTI_CFLAGS += -D'TRACE_TLB=0'

ifeq ($(TRACE_UNALIGN_ACCESS), true)
CUSTOM_CFLAGS += -D'TRACE_UNALIGN_ACCESS=1'
else
CUSTOM_CFLAGS += -D'TRACE_UNALIGN_ACCESS=0'
endif
OPTI_CFLAGS += -D'TRACE_UNALIGN_ACCESS=0'

ifeq ($(COMPARE_TO_ALTERNATIVE_DPI), true)
CUSTOM_CFLAGS += -D'COMPARE_TO_ALTERNATIVE_DPI=1'
else
CUSTOM_CFLAGS += -D'COMPARE_TO_ALTERNATIVE_DPI=0'
endif
OPTI_CFLAGS += -D'COMPARE_TO_ALTERNATIVE_DPI=0'

ifeq ($(CHECK_CALL_ONLY_ONE), true)
CUSTOM_CFLAGS += -D'CHECK_CALL_ONLY_ONE=1'
else
CUSTOM_CFLAGS += -D'CHECK_CALL_ONLY_ONE=0'
endif
OPTI_CFLAGS += -D'CHECK_CALL_ONLY_ONE=0'

ifeq ($(CHECK_LSM_END_ADDR), true)
CUSTOM_CFLAGS += -D'CHECK_LSM_END_ADDR=1'
else
CUSTOM_CFLAGS += -D'CHECK_LSM_END_ADDR=0'
endif
OPTI_CFLAGS += -D'CHECK_LSM_END_ADDR=0'

ifeq ($(TRACE_FLASH), true)
CUSTOM_CFLAGS += -D'TRACE_FLASH=1'
else
CUSTOM_CFLAGS += -D'TRACE_FLASH=0'
endif
OPTI_CFLAGS += -D'TRACE_FLASH=0'

ifeq ($(TRACE_GPIO), true)
CUSTOM_CFLAGS += -D'TRACE_GPIO=1'
else
CUSTOM_CFLAGS += -D'TRACE_GPIO=0'
endif
OPTI_CFLAGS += -D'TRACE_GPIO=0'

ifeq ($(TRACE_OS_TIMER), true)
CUSTOM_CFLAGS += -D'TRACE_OS_TIMER=1'
else
CUSTOM_CFLAGS += -D'TRACE_OS_TIMER=0'
endif
OPTI_CFLAGS += -D'TRACE_OS_TIMER=0'

ifeq ($(TRACE_RTC), true)
CUSTOM_CFLAGS += -D'TRACE_RTC=1'
else
CUSTOM_CFLAGS += -D'TRACE_RTC=0'
endif
OPTI_CFLAGS += -D'TRACE_RTC=0'

ifeq ($(TRACE_INTERRUPT), true)
CUSTOM_CFLAGS += -D'TRACE_INTERRUPT=1'
else
CUSTOM_CFLAGS += -D'TRACE_INTERRUPT=0'
endif
OPTI_CFLAGS += -D'TRACE_INTERRUPT=0'

ifeq ($(TRACE_LCD), true)
CUSTOM_CFLAGS += -D'TRACE_LCD=1'
else
CUSTOM_CFLAGS += -D'TRACE_LCD=0'
endif
OPTI_CFLAGS += -D'TRACE_LCD=0'

ifeq ($(TRACE_PPC), true)
CUSTOM_CFLAGS += -D'TRACE_PPC=1'
else
CUSTOM_CFLAGS += -D'TRACE_PPC=0'
endif
OPTI_CFLAGS += -D'TRACE_PPC=0'

ifeq ($(TRACE_UART), true)
CUSTOM_CFLAGS += -D'TRACE_UART=1'
else
CUSTOM_CFLAGS += -D'TRACE_UART=0'
endif
OPTI_CFLAGS += -D'TRACE_UART=0'

ifeq ($(TRACE_EGPIO), true)
CUSTOM_CFLAGS += -D'TRACE_EGPIO=1'
else
CUSTOM_CFLAGS += -D'TRACE_EGPIO=0'
endif
OPTI_CFLAGS += -D'TRACE_EGPIO=0'

ifeq ($(TRACE_MEMORY_CONF), true)
CUSTOM_CFLAGS += -D'TRACE_MEMORY_CONF=1'
else
CUSTOM_CFLAGS += -D'TRACE_MEMORY_CONF=0'
endif
OPTI_CFLAGS += -D'TRACE_MEMORY_CONF=0'

ifeq ($(TRACE_POWER_MGR), true)
CUSTOM_CFLAGS += -D'TRACE_POWER_MGR=1'
else
CUSTOM_CFLAGS += -D'TRACE_POWER_MGR=0'
endif
OPTI_CFLAGS += -D'TRACE_POWER_MGR=0'

ifeq ($(TRACE_RESET_CTRL), true)
CUSTOM_CFLAGS += -D'TRACE_RESET_CTRL=1'
else
CUSTOM_CFLAGS += -D'TRACE_RESET_CTRL=0'
endif
OPTI_CFLAGS += -D'TRACE_RESET_CTRL=0'

ifeq ($(TRACE_SERIAL_1), true)
CUSTOM_CFLAGS += -D'TRACE_SERIAL_1=1'
else
CUSTOM_CFLAGS += -D'TRACE_SERIAL_1=0'
endif
OPTI_CFLAGS += -D'TRACE_SERIAL_1=0'

ifeq ($(TRACE_SERIAL_2), true)
CUSTOM_CFLAGS += -D'TRACE_SERIAL_2=1'
else
CUSTOM_CFLAGS += -D'TRACE_SERIAL_2=0'
endif
OPTI_CFLAGS += -D'TRACE_SERIAL_2=0'

ifeq ($(TRACE_SYS_CO), true)
CUSTOM_CFLAGS += -D'TRACE_SYS_CO=1'
else
CUSTOM_CFLAGS += -D'TRACE_SYS_CO=0'
endif
OPTI_CFLAGS += -D'TRACE_SYS_CO=0'

ifeq ($(TRACE_CORE_MODE), true)
CUSTOM_CFLAGS += -D'TRACE_CORE_MODE=1'
else
CUSTOM_CFLAGS += -D'TRACE_CORE_MODE=0'
endif
OPTI_CFLAGS += -D'TRACE_CORE_MODE=0'

ifeq ($(TRACE_DRAW_SCREEN), true)
CUSTOM_CFLAGS += -D'TRACE_DRAW_SCREEN=1'
else
CUSTOM_CFLAGS += -D'TRACE_DRAW_SCREEN=0'
endif
OPTI_CFLAGS += -D'TRACE_DRAW_SCREEN=0'

ifeq ($(TRACE_UNKNOWN_MEMORY_ADDR), true)
CUSTOM_CFLAGS += -D'TRACE_UNKNOWN_MEMORY_ADDR=1'
else
CUSTOM_CFLAGS += -D'TRACE_UNKNOWN_MEMORY_ADDR=0'
endif
OPTI_CFLAGS += -D'TRACE_UNKNOWN_MEMORY_ADDR=0'

ifeq ($(TRACE_IDLE_MODE), true)
CUSTOM_CFLAGS += -D'TRACE_IDLE_MODE=1'
else
CUSTOM_CFLAGS += -D'TRACE_IDLE_MODE=0'
endif
OPTI_CFLAGS += -D'TRACE_IDLE_MODE=0'

ifeq ($(PRINT_CHUNK_CODE), true)
CUSTOM_CFLAGS += -D'PRINT_CHUNK_CODE=1'
else
CUSTOM_CFLAGS += -D'PRINT_CHUNK_CODE=0'
endif
OPTI_CFLAGS += -D'PRINT_CHUNK_CODE=0'

ifeq ($(PRINT_CHUNK_CODE_MORE_DETAIL), true)
CUSTOM_CFLAGS += -D'PRINT_CHUNK_CODE_MORE_DETAIL=1'
else
CUSTOM_CFLAGS += -D'PRINT_CHUNK_CODE_MORE_DETAIL=0'
endif
OPTI_CFLAGS += -D'PRINT_CHUNK_CODE_MORE_DETAIL=0'

ifeq ($(DUMP_CPU_INFO), true)
CUSTOM_CFLAGS += -D'DUMP_CPU_INFO=1'
else
CUSTOM_CFLAGS += -D'DUMP_CPU_INFO=0'
endif
OPTI_CFLAGS += -D'DUMP_CPU_INFO=0'

ifeq ($(DUMP_CPU_INFO_MORE_DETAIL), true)
CUSTOM_CFLAGS += -D'DUMP_CPU_INFO_MORE_DETAIL=1'
else
CUSTOM_CFLAGS += -D'DUMP_CPU_INFO_MORE_DETAIL=0'
endif
OPTI_CFLAGS += -D'DUMP_CPU_INFO_MORE_DETAIL=0'

ifeq ($(CHECK_CHUNK_CODE), true)
CUSTOM_CFLAGS += -D'CHECK_CHUNK_CODE=1'
else
CUSTOM_CFLAGS += -D'CHECK_CHUNK_CODE=0'
endif
OPTI_CFLAGS += -D'CHECK_CHUNK_CODE=0'

ifeq ($(PRINT_CHUNKS_LENGTH), true)
CUSTOM_CFLAGS += -D'PRINT_CHUNKS_LENGTH=1'
else
CUSTOM_CFLAGS += -D'PRINT_CHUNKS_LENGTH=0'
endif
OPTI_CFLAGS += -D'PRINT_CHUNKS_LENGTH=0'

# Initialize LDFLAGS

LDFLAGS = `pkg-config gtkmm-2.4 --libs`

ifeq ($(CONFIG_DATABASE), GDBM)
LDFLAGS += -lgdbm
endif

ifeq ($(CONFIG_DATABASE), XML)
LDFLAGS += `pkg-config --libs libxml++-2.6`
endif

###############################################################################
# Include Share Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.share.files,$(wildcard $(BUILD_MAK_DIR)/custom.share.files))
include $(BUILD_MAK_DIR)/custom.share.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.share.files,$(wildcard $(BUILD_MAK_DIR)/opti.share.files))
include $(BUILD_MAK_DIR)/opti.share.files
endif

###############################################################################
# Include Inst Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.inst.files,$(wildcard $(BUILD_MAK_DIR)/custom.inst.files))
include $(BUILD_MAK_DIR)/custom.inst.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.inst.files,$(wildcard $(BUILD_MAK_DIR)/opti.inst.files))
include $(BUILD_MAK_DIR)/opti.inst.files
endif

###############################################################################
# Include Database common Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.database.common.files,$(wildcard $(BUILD_MAK_DIR)/custom.database.common.files))
include $(BUILD_MAK_DIR)/custom.database.common.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.database.common.files,$(wildcard $(BUILD_MAK_DIR)/opti.database.common.files))
include $(BUILD_MAK_DIR)/opti.database.common.files
endif

###############################################################################
# Include Database dep Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.database.dep.files,$(wildcard $(BUILD_MAK_DIR)/custom.database.dep.files))
include $(BUILD_MAK_DIR)/custom.database.dep.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.database.dep.files,$(wildcard $(BUILD_MAK_DIR)/opti.database.dep.files))
include $(BUILD_MAK_DIR)/opti.database.dep.files
endif

###############################################################################
# Include Utils Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.utils.files,$(wildcard $(BUILD_MAK_DIR)/custom.utils.files))
include $(BUILD_MAK_DIR)/custom.utils.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.utils.files,$(wildcard $(BUILD_MAK_DIR)/opti.utils.files))
include $(BUILD_MAK_DIR)/opti.utils.files
endif

###############################################################################
# Include GTK Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.gtk.files,$(wildcard $(BUILD_MAK_DIR)/custom.gtk.files))
include $(BUILD_MAK_DIR)/custom.gtk.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.gtk.files,$(wildcard $(BUILD_MAK_DIR)/opti.gtk.files))
include $(BUILD_MAK_DIR)/opti.gtk.files
endif

###############################################################################
# Include Compiler Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.compiler.files,$(wildcard $(BUILD_MAK_DIR)/custom.compiler.files))
include $(BUILD_MAK_DIR)/custom.compiler.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.compiler.files,$(wildcard $(BUILD_MAK_DIR)/opti.compiler.files))
include $(BUILD_MAK_DIR)/opti.compiler.files
endif

###############################################################################
# Include Compiler x86 Object File Names
###############################################################################

ifeq ($(BUILD_MAK_DIR)/custom.compiler.x86.files,$(wildcard $(BUILD_MAK_DIR)/custom.compiler.x86.files))
include $(BUILD_MAK_DIR)/custom.compiler.x86.files
endif

ifeq ($(BUILD_MAK_DIR)/opti.compiler.x86.files,$(wildcard $(BUILD_MAK_DIR)/opti.compiler.x86.files))
include $(BUILD_MAK_DIR)/opti.compiler.x86.files
endif

###############################################################################
# OBJFILES
###############################################################################

INST_OBJFILES = $(CUSTOM_INST_OBJFILES)
GTK_OBJFILES = $(CUSTOM_GTK_OBJFILES)
COMPILER_OBJFILES = $(CUSTOM_COMPILER_OBJFILES)
COMPILER_X86_OBJFILES = $(CUSTOM_COMPILER_X86_OBJFILES)
DATABASE_COMMON_OBJFILES = $(CUSTOM_DATABASE_COMMON_OBJFILES)
DATABASE_DEP_OBJFILES = $(CUSTOM_DATABASE_DEP_OBJFILES)
UTILS_OBJFILES = $(CUSTOM_UTILS_OBJFILES)
SHARE_OBJFILES = $(CUSTOM_SHARE_OBJFILES)
PACKAGE_EXEC_FILE = $(PACKAGE_CUSTOM_EXEC_FILE)

ifeq ($(PRODUCE_OPTI_VERSION), true)
INST_OBJFILES += $(OPTI_INST_OBJFILES)
GTK_OBJFILES += $(OPTI_GTK_OBJFILES)
COMPILER_OBJFILES += $(OPTI_COMPILER_OBJFILES)
COMPILER_X86_OBJFILES += $(OPTI_COMPILER_X86_OBJFILES)
DATABASE_COMMON_OBJFILES += $(OPTI_DATABASE_COMMON_OBJFILES)
DATABASE_DEP_OBJFILES += $(OPTI_DATABASE_DEP_OBJFILES)
UTILS_OBJFILES += $(OPTI_UTILS_OBJFILES)
SHARE_OBJFILES += $(OPTI_SHARE_OBJFILES)
PACKAGE_EXEC_FILE += $(PACKAGE_OPTI_EXEC_FILE)
endif
