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

#ifndef MIR_hpp
#define MIR_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>

#include "Compiler.hpp"

#include "../MemoryPool.hpp"
#include "../Chunk.hpp"

#if PRINT_CHUNK_CODE
#include "../Log.hpp"
#endif

namespace ARMware
{
  // Forward declaration
  //
  
  class BasicBlock;
  class ConditionBlock;
  class Label;
  class Variable;
  
  typedef class MIR MIR;
  class MIR : public MemoryPool<MIR, 256>
  {
  private:
    
#if PRINT_CHUNK_CODE
    static std::string const KIND_NAME[MIR_KIND_TOTAL];
    
    uint32_t m_idx;
#endif
    
    friend class MemoryPool<MIR, 256>;
    
    MIRKindEnum m_kind;
    
    // :NOTE: Wei 2004-Nov-3:
    //
    // This MIR instruction belongs to which basic block.
    BasicBlock *mp_bb;
    
    // :NOTE: Wei 2004-Nov-3:
    //
    // If this MIR belongs to any condition block,
    // then this 'mp_cond_block' field will point to that condition block,
    // otherwise, this 'mp_cond_block' field will be 0.
    ConditionBlock *mp_cond_block;
    
    ARMInstInfo *mp_belong_to_which_arm_inst;
    
    MIR *mp_prev_mir;
    
    uint32_t m_in_flags;
    uint32_t m_out_flags;
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Used only in MIRJump.
    std::vector<Label *> m_jump_target_table;
    
    union
    {
      struct
      {
        Label *mp_routine_label;
      } m_CALL_0;
      
      struct
      {
        Label *mp_routine_label;
      } m_CALL_1;
      
      struct
      {
        uint32_t m_pop_count;
      } m_RETRIEVE;
      
      struct
      {
        Label    *mp_label;
      } m_CMP_EQ;
      
      struct
      {
        Label *mp_label;
      } m_GOTO;
            
      struct
      {
        Label *mp_jump_table_label;
      } m_JUMP;
      
      struct
      {
        Label *mp_label;
      } m_DATA32;
      
      struct
      {
        Label *mp_label;
      } m_LOAD_LABEL;
    } u;
    
    std::vector<Variable *> m_operand_var;
    std::vector<Variable *> m_dest_var;
    
  public:
    
    // Operation
    
    inline void
    init()
    {
      // :NOTE: Wei 2004-Nov-3:
      //
      // If this MIR belongs to any condition block,
      // then this 'mp_cond_block' field will point to that condition block,
      // otherwise, this 'mp_cond_block' field will be 0.
      mp_cond_block = 0;
      
      mp_belong_to_which_arm_inst = 0;
      
      m_in_flags = 0;
      m_out_flags = 0;
    }
    
    // Life cycle
    
    inline
    MIR(MIRCall_0 *, Label * const routine_label)
      : m_kind(MIRCall_0::KIND)
    {
      u.m_CALL_0.mp_routine_label = routine_label;
      
      init();
    }
    
    inline
    MIR(MIRCall_1 *,
        Label * const routine_label,
        Variable * const operand_var)
      : m_kind(MIRCall_1::KIND)
    {
      u.m_CALL_1.mp_routine_label = routine_label;
      
      m_operand_var.push_back(operand_var);
      
      init();
    }
    
    inline
    MIR(MIRReturn *,
        Variable * const operand_var)
      : m_kind(MIRReturn::KIND)
    {
      m_operand_var.push_back(operand_var);
      
      init();
    }
    
    inline
    MIR(MIRRetrieve *, uint32_t const pop_count, Variable * const var)
      : m_kind(MIRRetrieve::KIND)
    {
      if (var != 0)
      {
        m_dest_var.push_back(var);
      }
      
      u.m_RETRIEVE.m_pop_count = pop_count;
      
      init();
    }
    
    inline
    MIR(MIRAdd *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRAdd::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRAdd64 *,
        Variable * const operand_var1_hi,
        Variable * const operand_var1_lo,
        Variable * const operand_var2_hi,
        Variable * const operand_var2_lo,
        Variable * const dest_var_hi,
        Variable * const dest_var_lo)
      : m_kind(MIRAdd64::KIND)
    {
      m_operand_var.push_back(operand_var1_hi);
      m_operand_var.push_back(operand_var1_lo);
      m_operand_var.push_back(operand_var2_hi);
      m_operand_var.push_back(operand_var2_lo);
      
      m_dest_var.push_back(dest_var_hi);
      m_dest_var.push_back(dest_var_lo);
      
      init();
    }
    
    inline
    MIR(MIRAdc *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRAdc::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRMov *,
        Variable * const operand_var,
        Variable * const dest_var)
      : m_kind(MIRMov::KIND)
    {
      assert((operand_var != dest_var) ||
             !"Move one variable to the same variable, I should consider not to create this foolish MIR instruction.");
      
      m_operand_var.push_back(operand_var);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRAnd *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRAnd::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIROr *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIROr::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIREor *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIREor::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRSub *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRSub::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRSbc *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRSbc::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRTst *,
        Variable * const operand_var1,
        Variable * const operand_var2)
      : m_kind(MIRTst::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      init();
    }
    
    inline
    MIR(MIRCmp *,
        Variable * const operand_var1,
        Variable * const operand_var2)
      : m_kind(MIRCmp::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      init();
    }
    
    inline
    MIR(MIRCmpEQ *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Label * const label)
      : m_kind(MIRCmpEQ::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      u.m_CMP_EQ.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRNot *,
        Variable * const operand_var,
        Variable * const dest_var)
      : m_kind(MIRNot::KIND)
    {
      m_operand_var.push_back(operand_var);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRMul32 *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRMul32::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRUMul64 *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var_hi,
        Variable * const dest_var_lo)
      : m_kind(MIRUMul64::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var_hi);
      m_dest_var.push_back(dest_var_lo);
      
      init();
    }
    
    inline
    MIR(MIRSMul64 *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var_hi,
        Variable * const dest_var_lo)
      : m_kind(MIRSMul64::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var_hi);
      m_dest_var.push_back(dest_var_lo);
      
      init();
    }
    
    inline
    MIR(MIRLSftLeft *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRLSftLeft::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRLSftRight *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRLSftRight::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRASftRight *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRASftRight::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRRrx *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRRrx::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRRor *,
        Variable * const operand_var1,
        Variable * const operand_var2,
        Variable * const dest_var)
      : m_kind(MIRRor::KIND)
    {
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRGoto *, Label * const label)
      : m_kind(MIRGoto::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
        
    inline
    MIR(MIRGotoEQ *, Label * const label)
      : m_kind(MIRGotoEQ::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
        
    inline
    MIR(MIRGotoNE *, Label * const label)
      : m_kind(MIRGotoNE::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoCS *, Label * const label)
      : m_kind(MIRGotoCS::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoCC *, Label * const label)
      : m_kind(MIRGotoCC::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoMI *, Label * const label)
      : m_kind(MIRGotoMI::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoPL *, Label * const label)
      : m_kind(MIRGotoPL::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoVS *, Label * const label)
      : m_kind(MIRGotoVS::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoVC *, Label * const label)
      : m_kind(MIRGotoVC::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoHI *, Label * const label)
      : m_kind(MIRGotoHI::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoLS *, Label * const label)
      : m_kind(MIRGotoLS::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoGE *, Label * const label)
      : m_kind(MIRGotoGE::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoLT *, Label * const label)
      : m_kind(MIRGotoLT::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoGT *, Label * const label)
      : m_kind(MIRGotoGT::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRGotoLE *, Label * const label)
      : m_kind(MIRGotoLE::KIND)
    {
      u.m_GOTO.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRJump *,
        Variable * const operand_var,
        Label * const jump_table_label)
      : m_kind(MIRJump::KIND)
    {
      m_operand_var.push_back(operand_var);
      
      u.m_JUMP.mp_jump_table_label = jump_table_label;
      
      init();
    }
    
    inline
    MIR(MIRProduceCBit *,
        Variable * const operand_var1,
        Variable * const operand_var2)
      : m_kind(MIRProduceCBit::KIND)
    {
      assert(Variable::CONST == operand_var2->kind());
      assert(operand_var2->const_num() <= 31);
      
      m_operand_var.push_back(operand_var1);
      m_operand_var.push_back(operand_var2);
      
      init();
    }
    
    inline
    MIR(MIRClearCBit *)
      : m_kind(MIRClearCBit::KIND)
    {
      init();
    }
    
    inline
    MIR(MIRData32 *, Label * const label)
      : m_kind(MIRData32::KIND)
    {
      u.m_DATA32.mp_label = label;
      
      init();
    }
    
    inline
    MIR(MIRLoadLabel *,
        Label * const label,
        Variable * const dest_var)
      : m_kind(MIRLoadLabel::KIND)
    {
      u.m_LOAD_LABEL.mp_label = label;
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    inline
    MIR(MIRLoad *,
        Variable * const operand_var,
        Variable * const dest_var)
      : m_kind(MIRLoad::KIND)
    {
      m_operand_var.push_back(operand_var);
      
      m_dest_var.push_back(dest_var);
      
      init();
    }
    
    // Operation
    
    inline void
    add_jump_target_label(Label * const label)
    {
      assert(MIRJump::KIND == m_kind);
      
      m_jump_target_table.push_back(label);
    }
    
    void emit_native_code(std::vector<uint8_t> &native_code_buffer);
    
#if PRINT_CHUNK_CODE
    void dump_info() const;
#endif
    
    // Access
    
    inline void
    set_belong_to_which_arm_inst(ARMInstInfo * const arm)
    {
      assert(0 == mp_belong_to_which_arm_inst);
      
      mp_belong_to_which_arm_inst = arm;
    }
    
    inline void
    set_prev_mir(MIR * const mir)
    { mp_prev_mir = mir; }
    
    inline void
    set_next_mir(MIR * const mir)
    { mp_next_used_node = mir; }
    
    inline void
    set_bb(BasicBlock * const bb)
    {
      mp_bb = bb;
    }
    
    inline void
    set_cond_block(ConditionBlock * const cond_block)
    {
      assert(0 == mp_cond_block);
      
      mp_cond_block = cond_block;
    }
    
    inline void
    set_in_flags(uint32_t const in_flags)
    {
      assert(0 == (in_flags & ~(SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG)));
      
      m_in_flags = in_flags;
    }
    
    void set_in_out_flags(uint32_t const in_flags, uint32_t const out_flags);
    
#if PRINT_CHUNK_CODE
    inline void
    set_idx(uint32_t const idx)
    { m_idx = idx; }
#endif
    
    inline void
    clear_n_bit()
    {
#if PRINT_CHUNK_CODE
      g_log_file << "<opt> clear N bit for "
                 << m_idx
                 << std::endl;
#endif
      
      m_out_flags &= ~SIGN_FLAG;
    }
    
    inline void
    clear_z_bit()
    {
#if PRINT_CHUNK_CODE
      g_log_file << "<opt> clear Z bit for "
                 << m_idx
                 << std::endl;
#endif
      
      m_out_flags &= ~ZERO_FLAG;
    }
    
    inline void
    clear_c_bit()
    {
#if PRINT_CHUNK_CODE
      g_log_file << "<opt> clear C bit for "
                 << m_idx
                 << std::endl;
#endif
      
      m_out_flags &= ~CARRY_FLAG;
    }
    
    inline void
    clear_v_bit()
    {
#if PRINT_CHUNK_CODE
      g_log_file << "<opt> clear V bit for "
                 << m_idx
                 << std::endl;
#endif
      
      m_out_flags &= ~OVERFLOW_FLAG;
    }
    
    inline std::vector<Variable *> &
    operand_variable()
    { return m_operand_var; }
    
    inline void
    set_operand_variable(uint32_t const idx, Variable * const var)
    {
      assert(idx < m_operand_var.size());
      
      m_operand_var[idx] = var;
    }
    
    // Inquiry
    
    inline MIRKindEnum
    kind() const
    { return m_kind; }
    
    inline ARMInstInfo *
    belong_to_which_arm_inst() const
    {
      assert(mp_belong_to_which_arm_inst != 0);
      
      return mp_belong_to_which_arm_inst;
    }
    
    inline uint32_t
    pop_count() const
    {
      assert(MIR_RETRIEVE == m_kind);
      
      return u.m_RETRIEVE.m_pop_count;
    }
    
    inline std::vector<Variable *> const &
    operand_variable() const
    { return m_operand_var; }
    
    template<VarAttachedTypeEnum T_attach>
    inline Variable *
    dest_variable() const
    {
      switch (T_attach)
      {
      case VAR_ATTACH_HI:
        assert(2 == m_dest_var.size());
        return m_dest_var.front();
        
      case VAR_ATTACH_LO:
        assert(2 == m_dest_var.size());
        return m_dest_var.back();
        
      case VAR_ATTACH_NORMAL:
        if (1 == m_dest_var.size())
        {
          return m_dest_var.front();
        }
        else
        {
          assert(0 == m_dest_var.size());
          
          return 0;
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
      
      assert(!"Should not reach here.");
      return 0;
    }
    
    template<MIRKindEnum T_kind>
    inline Label *
    label() const
    {
      assert((MIR_GOTO == T_kind) ? true : (T_kind == m_kind));
      
      switch (T_kind)
      {
      case MIR_CALL_0:
        return u.m_CALL_0.mp_routine_label;
        
      case MIR_CALL_1:
        return u.m_CALL_1.mp_routine_label;
        
      case MIR_CMP_EQ:
        return u.m_CMP_EQ.mp_label;
        
      case MIR_GOTO:
      case MIR_GOTO_EQ:
      case MIR_GOTO_NE:
      case MIR_GOTO_CS:
      case MIR_GOTO_CC:
      case MIR_GOTO_MI:
      case MIR_GOTO_PL:
      case MIR_GOTO_VS:
      case MIR_GOTO_VC:
      case MIR_GOTO_HI:
      case MIR_GOTO_LS:
      case MIR_GOTO_GE:
      case MIR_GOTO_LT:
      case MIR_GOTO_GT:
      case MIR_GOTO_LE:
        return u.m_GOTO.mp_label;
        
      case MIR_JUMP:
        return u.m_JUMP.mp_jump_table_label;
        
      case MIR_DATA32:
        return u.m_DATA32.mp_label;
        
      case MIR_LOAD_LABEL:
        return u.m_LOAD_LABEL.mp_label;
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
    }
    
    inline MIR *
    prev_mir() const
    { return mp_prev_mir; }
    
    inline MIR *
    next_mir() const
    { return mp_next_used_node; }
    
    inline uint32_t
    in_flags() const
    { return m_in_flags; }
    
    inline uint32_t
    out_flags() const
    { return m_out_flags; }
    
    inline std::vector<Label *> const *
    jump_target_table() const
    { return &m_jump_target_table; }
    
    inline BasicBlock *
    bb() const 
    { return mp_bb; }
    
    inline ConditionBlock *
    cond_block() const
    { return mp_cond_block; }
    
#if PRINT_CHUNK_CODE
    inline uint32_t
    idx() const
    { return m_idx; }
#endif
  };
}

#endif // #if ENABLE_DYNAMIC_TRANSLATOR

#endif
