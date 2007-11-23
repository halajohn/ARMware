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

INSTALL_PREFIX = $(DESTDIR)

###############################################################################

# supports cpu:
#  - SA1110

CONFIG_CPU = SA1110

# supports arch:
#  - V1,
#  - V2,
#  - V3,
#  - V4,
#  - V5

CONFIG_ARCH = V4

# T variant:
#  - true,
#  - false

CONFIG_T_VARIANT = true

# supports machine:
#  - H3600

CONFIG_MACH = H3600

# support flash chip:
#  - INTEL_STRATA_J3

CONFIG_FLASH = INTEL_STRATA_J3

# database format:
#  - GDBM,
#  - XML

CONFIG_DATABASE = XML

###############################################################################
# custom build options

DEBUG_MODE = true
PRODUCE_OPTI_VERSION = true
PROFILE_MODE = false

ENABLE_INST_RECALL = false
ENABLE_ASSERT = true
ENABLE_COND_CODES_ASM = true
USE_SHIFTER_CARRY = false

# :NOTE: Wei 2004-Aug-18:
# Comments have to be placed in a new line rather than place in the end of a non-comment line.
#
# Ex:
#
# < Right >
#
# # work as expected when DEBUG_MODE==false
# ENABLE_DRAWING_ASM = true
#
# < Wrong >
#
# ENABLE_DRAWING_ASM = true # work as expected when DEBUG_MODE==false
#
# Because in the second example, 'make' will treat 'true # work as expected when DEBUG_MODE==false'
# as the whole parameter to 'ENABLE_DRAWING_ASM' rather than a simple 'true'.

# work as expected when DEBUG_MODE==false
ENABLE_DRAWING_ASM = true

# work as expected when DEBUG_MODE==false
ENABLE_COMPILER_MAGIC = true

ENABLE_THREADED_CODE = true
ENABLE_DYNAMIC_TRANSLATOR = true

LOAD_ROM_INTO_MEMORY = true

USE_BALANCE_DOM_ALGO = true

CHECK_UNPREDICTABLE = false
CHECK_GTK_ERROR = true
CHECK_CALL_ONLY_ONE = false
CHECK_LSM_END_ADDR = false
CHECK_PC_MODIFY = false
CHECK_AMTEL_CHKSUM = true
CHECK_ASM_CORRECT = false
CHECK_EXCEPTION_RAISE = false

# work as expected when ENABLE_THREADED_CODE==true
CHECK_MPROTECT = false

# work as expected when ENABLE_THREADED_CODE==true
CHECK_THREADED_CODE_PC = false

# work as expected when ENABLE_THREADED_CODE==true
CHECK_THREADED_CODE_LENGTH = false

ASSUME_NOT_USE_TINY_PAGE = true

TRACE_CORE_MODE = false
TRACE_MMU = false
TRACE_TLB = false
TRACE_UNALIGN_ACCESS = false
TRACE_FLASH = false
TRACE_EGPIO = false
TRACE_GPIO = false
TRACE_OS_TIMER = false
TRACE_RTC = false
TRACE_INTERRUPT = false
TRACE_LCD = false
TRACE_PPC = false
TRACE_UART = false
TRACE_MEMORY_CONF = false
TRACE_POWER_MGR = false
TRACE_RESET_CTRL = false
TRACE_SERIAL_1 = false
TRACE_SERIAL_2 = false
TRACE_SYS_CO = false
TRACE_DRAW_SCREEN = false
TRACE_UNKNOWN_MEMORY_ADDR = false
TRACE_IDLE_MODE = false

PRINT_CHUNK_CODE = false
PRINT_CHUNK_CODE_MORE_DETAIL = false
DUMP_CPU_INFO = false
DUMP_CPU_INFO_MORE_DETAIL = false
CHECK_CHUNK_CODE = false
PRINT_CHUNKS_LENGTH = false

COMPARE_TO_ALTERNATIVE_DPI = false
