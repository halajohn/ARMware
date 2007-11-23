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

#ifndef Label_hpp
#define Label_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>

#include <vector>

#include "Compiler.hpp"
#include "../MemoryPool2.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class MIR;
  class LIR;
  class ARMInstInfo;
  
  class LabelARMFirstInst;
  class LabelARMNormalInst;
  class LabelMIRInst;
  class LabelLIRInst;
  class LabelRoutine;
  class LabelBB;
  
  class BasicBlock;
  
  typedef class Label Label;
  class Label : public MemoryPool2<Label, 256>
  {
  public:
    
    // :NOTE: Wei 2004-Nov-3:
    //
    // Label type.
    enum LabelKindEnum
    {
      ARM_FIRST_INST,
      ARM_NORMAL_INST,
      
      MIR_INST,
      
      // :NOTE: Wei 2005-Mar-20:
      //
      // In the last label linking stage, only the following 3 types should be appear.
      RAW_TARGET_OFFSET,
      BASIC_BLOCK,
      ROUTINE
    };
    typedef enum LabelKindEnum LabelKindEnum;
    
    enum PatchTypeEnum
    {
      ABSOLUTE,
      RELATIVE_TO_NEXT
    };
    typedef enum PatchTypeEnum PatchTypeEnum;
    
    struct PatchInfo
    {
      PatchTypeEnum m_type;
      uint32_t m_offset;
      
      inline
      PatchInfo(PatchTypeEnum const type, uint32_t const offset)
        : m_type(type),
          m_offset(offset)
      { }
      
      inline PatchTypeEnum
      type() const
      { return m_type; }
      
      inline uint32_t
      offset() const
      { return m_offset; }
    };
    typedef struct PatchInfo PatchInfo;
    
    friend class MemoryPool2<Label, 256>;
    
  private:
        
    LabelKindEnum m_kind;
    
    union
    {
      struct
      {
        ARMInstInfo *mp_arm_inst;
      } m_ARM_INST;
      
      struct
      {
        MIR *mp_mir;
      } m_MIR_INST;
      
      struct
      {
        uint32_t m_offset;
      } m_RAW_TARGET_OFFSET;
      
      struct
      {
        BasicBlock *mp_bb;
      } m_BB;
      
      struct
      {
        DTFunc_t m_routine;
      } m_ROUTINE;
    } u;
    
    // :NOTE: Wei 2004-Nov-3:
    //
    // Some labels need to be back-patched from ARM_INST to MIR_INST,
    // and these labels will be chianed using the following field.
    Label *mp_next_back_patch_label;
    
    // :NOTE: Wei 2004-Nov-3:
    //
    // Some labels which needs to be back-patched will break a
    // condition block, some are not.
    // Thus we need this field to distinguish that.
    bool m_need_to_check_cond_block;
    
    std::vector<PatchInfo> m_back_patch_info;
    
#if CHECK_CHUNK_CODE
    void check_for_set_mir_inst(MIR * const mir);
    
    bool m_finish_linking;
#endif
    
  public:
    
    // Life cycle
    
    inline void
    init()
    {
      m_need_to_check_cond_block = false;
      
#if CHECK_CHUNK_CODE
      m_finish_linking = false;
#endif
    }
    
    inline
    Label(LabelARMFirstInst *)
      : m_kind(ARM_FIRST_INST)
    {
      init();
    }
    
    inline
    Label(LabelARMNormalInst *)
      : m_kind(ARM_NORMAL_INST)
    {
      init();
    }
    
    inline
    Label(LabelARMFirstInst *, ARMInstInfo * const arm_inst)
      : m_kind(ARM_FIRST_INST)
    {
      assert(arm_inst != 0);
      
      u.m_ARM_INST.mp_arm_inst = arm_inst;
      
      init();
    }
    
    inline
    Label(LabelARMNormalInst *, ARMInstInfo * const arm_inst)
      : m_kind(ARM_NORMAL_INST)
    {
      assert(arm_inst != 0);
      
      u.m_ARM_INST.mp_arm_inst = arm_inst;
      
      init();
    }
    
    inline
    Label(LabelMIRInst *)
      : m_kind(MIR_INST)
    {      
      init();
    }
    
    inline
    Label(LabelMIRInst *, MIR * const mir)
      : m_kind(MIR_INST)
    {
      u.m_MIR_INST.mp_mir = mir;
      
      init();
    }
    
    inline
    Label(LabelRoutine *, DTFunc_t const routine)
      : m_kind(ROUTINE)
    {
      u.m_ROUTINE.m_routine = routine;
      
      init();
    }
    
    inline
    Label(LabelBB *, BasicBlock * const bb)
      : m_kind(BASIC_BLOCK)
    {
      u.m_BB.mp_bb = bb;
      
      init();
    }
    
    inline
    Label(LabelBB *)
      : m_kind(BASIC_BLOCK)
    {
#if CHECK_CHUNK_CODE
      u.m_BB.mp_bb = 0;
#endif
      
      init();
    }
    
    // Operation
    
    inline void
    reset_to_arm_first_inst(ARMInstInfo * const arm_inst)
    {
      assert(arm_inst != 0);
      
      m_kind = ARM_FIRST_INST;
      
      u.m_ARM_INST.mp_arm_inst = arm_inst;
    }
    
    inline void
    reset_to_mir_inst(MIR * const mir)
    {
      assert(mir != 0);
      
      m_kind = MIR_INST;
      
      u.m_MIR_INST.mp_mir = mir;
    }
    
    inline void
    reset_to_raw_target_offset(uint32_t const offset)
    {
      m_kind = RAW_TARGET_OFFSET;
      
      u.m_RAW_TARGET_OFFSET.m_offset = offset;
    }
    
    inline void
    reset_to_bb(BasicBlock * const bb)
    {
      m_kind = BASIC_BLOCK;
      
      u.m_BB.mp_bb = bb;
    }
    
    void back_patch_to_mir_inst();
    void convert_to_basicblock_label();
    
    inline void
    replace_bb_label_bb(BasicBlock * const bb)
    {
      assert(BASIC_BLOCK == m_kind);
      
      assert(u.m_BB.mp_bb != 0);
      assert(bb != 0);
      
      u.m_BB.mp_bb = bb;
    }
    
    inline void
    add_back_patch_info(PatchTypeEnum const type, uint32_t const offset)
    {
      m_back_patch_info.push_back(PatchInfo(type, offset));
    }
    
#if PRINT_CHUNK_CODE
    void dump_info() const;
#endif
    
    // Access
    
    inline void
    set_arm_first_inst(ARMInstInfo * const arm_inst)
    {
      assert(ARM_FIRST_INST == m_kind);
      assert(arm_inst != 0);
      
      u.m_ARM_INST.mp_arm_inst = arm_inst;
    }
    
    inline void
    set_mir_inst(MIR * const mir)
    {
      assert(MIR_INST == m_kind);
      assert(mir != 0);
      
      u.m_MIR_INST.mp_mir = mir;
      
#if CHECK_CHUNK_CODE
      check_for_set_mir_inst(mir);
#endif
    }
    
    inline void
    set_next_need_back_patch_label(Label * const label)
    {
      mp_next_back_patch_label = label;
    }
    
    inline void
    need_to_check_cond_block()
    { m_need_to_check_cond_block = true; }
    
#if CHECK_CHUNK_CODE
    inline void
    set_finish_linking()
    { m_finish_linking = true; }
#endif
    
    // Inquiry
    
    inline LabelKindEnum
    kind() const
    { return m_kind; }
    
    inline Label *
    next_back_patch_label() const
    { return mp_next_back_patch_label; }
    
    inline MIR *
    mir() const
    {
      assert(MIR_INST == m_kind);
      
      return u.m_MIR_INST.mp_mir;
    }
    
    inline BasicBlock *
    bb() const
    {
      assert(BASIC_BLOCK == m_kind);
      
      return u.m_BB.mp_bb;
    }
    
    inline DTFunc_t
    routine() const
    {
      assert(ROUTINE == m_kind);
      
      return u.m_ROUTINE.m_routine;
    }
    
    inline bool
    whether_need_to_check_cond_block() const
    { return m_need_to_check_cond_block; }
    
    inline std::vector<PatchInfo> const &
    back_patch_info() const
    { return m_back_patch_info; }
    
#if CHECK_CHUNK_CODE
    inline bool
    is_finish_linking() const
    { return m_finish_linking; }
#endif
  };
}

#endif

#endif
