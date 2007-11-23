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

#if ENABLE_DYNAMIC_TRANSLATOR

#include "Label.hpp"
#include "MIR.hpp"

#if PRINT_CHUNK_CODE
#include "../Log.hpp"
#endif

#include "../ARMInstInfo.hpp"

namespace ARMware
{
#if CHECK_CHUNK_CODE
  void
  Label::check_for_set_mir_inst(MIR * const mir)
  {
    assert(mir != 0);
    
    if (mir->prev_mir() != 0)
    {
      // :NOTE: Wei 2005-Mar-09:
      //
      // When I need to set a MIR instruction to a label,
      // this means that the MIR instruction is the beginning of a basic block.
      // Thus I can check this fact now.
      assert(mir->bb() != mir->prev_mir()->bb());
    }
  }
#endif
  
  void
  Label::back_patch_to_mir_inst()
  {
    assert((ARM_FIRST_INST == m_kind) || (ARM_NORMAL_INST == m_kind));
    
#if CHECK_CHUNK_CODE
    assert((u.m_ARM_INST.mp_arm_inst->first_mir() != 0) &&
           (u.m_ARM_INST.mp_arm_inst->first_normal_mir() != 0));
#endif
    
    switch (m_kind)
    {
    case ARM_FIRST_INST:
      u.m_MIR_INST.mp_mir = u.m_ARM_INST.mp_arm_inst->first_mir();
      break;
      
    case ARM_NORMAL_INST:
      u.m_MIR_INST.mp_mir = u.m_ARM_INST.mp_arm_inst->first_normal_mir();
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    m_kind = MIR_INST;
  }
  
  void
  Label::convert_to_basicblock_label()
  {
    assert(MIR_INST == m_kind);
    
    assert(u.m_MIR_INST.mp_mir != 0);
    
#if CHECK_CHUNK_CODE
    if (u.m_MIR_INST.mp_mir->prev_mir() != 0)
    {
      assert(u.m_MIR_INST.mp_mir->bb() != u.m_MIR_INST.mp_mir->prev_mir()->bb());
    }
#endif
    
    m_kind = BASIC_BLOCK;
    
    assert(u.m_MIR_INST.mp_mir->bb() != 0);
    
    u.m_BB.mp_bb = u.m_MIR_INST.mp_mir->bb();
  }
  
#if PRINT_CHUNK_CODE
  void
  Label::dump_info() const
  {
    // :NOTE: Wei 2004-Nov-1:
    //
    // Before we dump the label info, we should back-patching
    // all the Labels with 'ARM_INST' type to 'MIR_INST' type.
    assert(MIR_INST == m_kind);
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // The corresponding MIR of a label in MIRData32 instruction
    // could be 0.
    if (u.m_MIR_INST.mp_mir != 0)
    {
      g_log_file << std::dec
                << u.m_MIR_INST.mp_mir->idx();
    }
    else
    {
      g_log_file << "0";
    }
  }
#endif
}

#endif
