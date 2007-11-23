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

$(BUILD_OBJ_DIR)/%.o: %.cpp
	@$(ECHO) " --- Compiling $@ (custom mode)"
	@$(CXX) $(CUSTOM_CFLAGS) -c $< -o $@

$(BUILD_OBJ_DIR)/%.o.opti: %.cpp
	@$(ECHO) " --- Compiling $@ (optimize mode)"
	@$(CXX) $(OPTI_CFLAGS) -c $< -o $@

.depend.custom: *.cpp
	@$(ECHO) " --- Make dependencies (custom mode)"
	@$(CXX) $(CUSTOM_CFLAGS) -M -E *.cpp > .depend_orig
	@$(SED) 's/\(.*\)\.o/$$(BUILD_OBJ_DIR)\/\1\.o/g' .depend_orig > .depend.custom
	@$(RM) -f .depend_orig

.depend.opti: *.cpp
	@$(ECHO) " --- Make dependencies (optimize mode)"
	@$(CXX) $(OPTI_CFLAGS) -M -E *.cpp > .depend_orig
	@$(SED) 's/\(.*\)\.o/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' .depend_orig > .depend.opti
	@$(RM) -f .depend_orig

###############################################################################
# Determine Share Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.share.files: $(BUILD_MAK_DIR)/build.share.files
	@$(ECHO) " --- Determine share object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_SHARE_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_SHARE_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.share.files > $(BUILD_MAK_DIR)/custom.share.files

$(BUILD_MAK_DIR)/opti.share.files: $(BUILD_MAK_DIR)/build.share.files
	@$(ECHO) " --- Determine share object files (optimized mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_SHARE_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_SHARE_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.share.files > $(BUILD_MAK_DIR)/opti.share.files

###############################################################################
# Determine Inst Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.inst.files: $(BUILD_MAK_DIR)/build.inst.files
	@$(ECHO) " --- Determine inst object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_INST_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_INST_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.inst.files > $(BUILD_MAK_DIR)/custom.inst.files

$(BUILD_MAK_DIR)/opti.inst.files: $(BUILD_MAK_DIR)/build.inst.files
	@$(ECHO) " --- Determine inst object files (optimized mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_INST_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_INST_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.inst.files > $(BUILD_MAK_DIR)/opti.inst.files

###############################################################################
# Determine Database common Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.database.common.files: $(BUILD_MAK_DIR)/build.database.common.files
	@$(ECHO) " --- Determine database.common object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_DATABASE_COMMON_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_DATABASE_COMMON_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.database.common.files > $(BUILD_MAK_DIR)/custom.database.common.files

$(BUILD_MAK_DIR)/opti.database.common.files: $(BUILD_MAK_DIR)/build.database.common.files
	@$(ECHO) " --- Determine database.common object files (optimize mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_DATABASE_COMMON_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_DATABASE_COMMON_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.database.common.files > $(BUILD_MAK_DIR)/opti.database.common.files

###############################################################################
# Determine Database depend Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.database.dep.files: $(BUILD_MAK_DIR)/build.database.dep.files
	@$(ECHO) " --- Determine database.dep object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_DATABASE_DEP_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_DATABASE_DEP_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.database.dep.files > $(BUILD_MAK_DIR)/custom.database.dep.files

$(BUILD_MAK_DIR)/opti.database.dep.files: $(BUILD_MAK_DIR)/build.database.dep.files
	@$(ECHO) " --- Determine database.dep object files (optimize mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_DATABASE_DEP_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_DATABASE_DEP_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.database.dep.files > $(BUILD_MAK_DIR)/opti.database.dep.files

###############################################################################
# Determine Utils Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.utils.files: $(BUILD_MAK_DIR)/build.utils.files
	@$(ECHO) " --- Determine utils object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_UTILS_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_UTILS_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.utils.files > $(BUILD_MAK_DIR)/custom.utils.files

$(BUILD_MAK_DIR)/opti.utils.files: $(BUILD_MAK_DIR)/build.utils.files
	@$(ECHO) " --- Determine utils object files (optimize mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_UTILS_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_UTILS_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.utils.files > $(BUILD_MAK_DIR)/opti.utils.files

###############################################################################
# Determine Gtk Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.gtk.files: $(BUILD_MAK_DIR)/build.gtk.files
	@$(ECHO) " --- Determine Gtk object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_GTK_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_GTK_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.gtk.files > $(BUILD_MAK_DIR)/custom.gtk.files

$(BUILD_MAK_DIR)/opti.gtk.files: $(BUILD_MAK_DIR)/build.gtk.files
	@$(ECHO) " --- Determine Gtk object files (optimize mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_GTK_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_GTK_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.gtk.files > $(BUILD_MAK_DIR)/opti.gtk.files

###############################################################################
# Determine Compiler Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.compiler.files: $(BUILD_MAK_DIR)/build.compiler.files
	@$(ECHO) " --- Determine compiler object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_COMPILER_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_COMPILER_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.compiler.files > $(BUILD_MAK_DIR)/custom.compiler.files

$(BUILD_MAK_DIR)/opti.compiler.files: $(BUILD_MAK_DIR)/build.compiler.files
	@$(ECHO) " --- Determine compiler object files (optimized mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_COMPILER_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_COMPILER_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.compiler.files > $(BUILD_MAK_DIR)/opti.compiler.files

###############################################################################
# Determine Compiler X86 Objects File Names
###############################################################################

$(BUILD_MAK_DIR)/custom.compiler.x86.files: $(BUILD_MAK_DIR)/build.compiler.x86.files
	@$(ECHO) " --- Determine compiler object files (custom mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o/g' -e '1,1s/\(.*\)/CUSTOM_COMPILER_X86_OBJFILES=\1/' -e '2,$$s/\(.*\)/CUSTOM_COMPILER_X86_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.compiler.x86.files > $(BUILD_MAK_DIR)/custom.compiler.x86.files

$(BUILD_MAK_DIR)/opti.compiler.x86.files: $(BUILD_MAK_DIR)/build.compiler.x86.files
	@$(ECHO) " --- Determine compiler object files (optimized mode)"
	@$(SED) -e 's/\([A-Za-z0-9_]*\)/$$(BUILD_OBJ_DIR)\/\1\.o.opti/g' -e '1,1s/\(.*\)/OPTI_COMPILER_X86_OBJFILES=\1/' -e '2,$$s/\(.*\)/OPTI_COMPILER_X86_OBJFILES+=\1/' $(BUILD_MAK_DIR)/build.compiler.x86.files > $(BUILD_MAK_DIR)/opti.compiler.x86.files
