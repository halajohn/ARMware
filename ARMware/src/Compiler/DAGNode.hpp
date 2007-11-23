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

#ifndef DAGNode_hpp
#define DAGNode_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>
#include <deque>
#include <list>

#include "ChildInfo.hpp"
#include "Compiler.hpp"
#include "Variable.hpp"

#include "../MemoryPool.hpp"

namespace ARMware
{
  class BasicBlock;
  class Label;
  
  typedef class DAGNode DAGNode;
  class DAGNode : public MemoryPool<DAGNode, 512>
  {
  public:
    
    struct DAGNodePair
    {
      DAGNode *mp_prev_dag_node;
      
      uint32_t m_prev_dag_node_slot_idx;
      
      DAGNode *mp_next_dag_node;
      
      uint32_t m_next_dag_node_slot_idx;
      
      inline
      DAGNodePair(DAGNode * const prev_node,
                  uint32_t const prev_slot_idx,
                  DAGNode * const next_node,
                  uint32_t const next_slot_idx)
        : mp_prev_dag_node(prev_node),
          m_prev_dag_node_slot_idx(prev_slot_idx),
          mp_next_dag_node(next_node),
          m_next_dag_node_slot_idx(next_slot_idx)
      { }
    };
    typedef struct DAGNodePair DAGNodePair;
    
  private:
    
    // Attribute
    
#if PRINT_CHUNK_CODE || CHECK_CHUNK_CODE
    static std::string const KIND_NAME[DAG_KIND_TOTAL];
    
    static uint32_t s_sequence_idx;
    
    uint32_t m_sequence_idx;
#endif
    
    DAGKindEnum m_kind;
    
    union
    {
      struct
      {
        Label *mp_label;
      } m_LABEL;
      
      struct
      {
        uint32_t m_pop_count;
      } m_RETRIEVE;
    } u;
    
    uint32_t m_in_flags;
    uint32_t m_out_flags;
    
    std::deque<ChildInfo> m_children;
    
    std::list<DAGNode *> m_hi_ancestor;
    std::list<DAGNode *> m_lo_ancestor;
    
    std::list<Variable *> m_hi_attached_var;
    std::list<Variable *> m_lo_attached_var;
    
    bool m_init_hi_attached_var_iter;
    bool m_init_lo_attached_var_iter;
    
    std::list<Variable *>::iterator m_hi_attached_var_iter;
    std::list<Variable *>::iterator m_lo_attached_var_iter;
    
    std::vector<DAGNodePair> m_prev_next_dag_node_pair;
    
    bool m_init_prev_next_dag_node_pair_iter;
    
    std::vector<DAGNodePair>::iterator m_prev_next_dag_node_pair_iter;
    
    uint32_t m_global_attached_var_ref_cnt;
    uint32_t m_global_attached_var_def_cnt;
    
    bool m_usable;
    
  public:
    
    // Operation
    
    template<VarAttachedTypeEnum T_attach>
    inline std::list<Variable *> const &
    attached_var() const
    {
      switch (T_attach)
      {
      case VAR_ATTACH_HI:
        return m_hi_attached_var;
        
      case VAR_ATTACH_LO:
      case VAR_ATTACH_NORMAL:
        return m_lo_attached_var;
        
      default:
        assert(!"Should not reach here.");
        return m_lo_attached_var;
      }
    }
    
    template<VarAttachedTypeEnum T_attach>
    inline std::list<Variable *> &
    attached_var()
    {
      switch (T_attach)
      {
      case VAR_ATTACH_HI:
        return m_hi_attached_var;
        
      case VAR_ATTACH_LO:
      case VAR_ATTACH_NORMAL:
        return m_lo_attached_var;
        
      default:
        assert(!"Should not reach here.");
        return m_lo_attached_var;
      }
    }
    
    template<VarAttachedTypeEnum T_attach>
    inline std::list<Variable *>::iterator &
    attached_var_iter()
    {
      switch (T_attach)
      {
      case VAR_ATTACH_HI:
        if (false == m_init_hi_attached_var_iter)
        {
          m_hi_attached_var_iter = m_hi_attached_var.begin();
          
          m_init_hi_attached_var_iter = true;
        }
        
        return m_hi_attached_var_iter;
        
      case VAR_ATTACH_LO:
      case VAR_ATTACH_NORMAL:
        if (false == m_init_lo_attached_var_iter)
        {
          m_lo_attached_var_iter = m_lo_attached_var.begin();
          
          m_init_lo_attached_var_iter = true;
        }
        
        return m_lo_attached_var_iter;
        
      default:
        assert(!"Should not reach here.");
        return m_lo_attached_var_iter;
      }
    }
        
  private:
    
    // Operation
    
    template<VarAttachedTypeEnum T_attach>
    void cleanup_attached_var_internal();
    
    template<VarAttachedTypeEnum T_attach>
    inline void
    clear_dag_node_of_attached_var_internal()
    {
      for (std::list<Variable *>::iterator iter = attached_var<T_attach>().begin();
           iter != attached_var<T_attach>().end();
           ++iter)
      {
        if ((*iter)->dag_node() == this)
        {
          (*iter)->clear_dag_node();
        }
      }
    }
    
#if PRINT_CHUNK_CODE
    template<VarAttachedTypeEnum T_attach>
    void dump_attached_var_internal() const;
#endif
    
    friend class MemoryPool<DAGNode, 512>;
    
  public:
    
    // Life cycle
    
    inline void
    init()
    {
      m_in_flags = 0;
      m_out_flags = 0;
      
      m_init_hi_attached_var_iter = false;
      m_init_lo_attached_var_iter = false;
      m_init_prev_next_dag_node_pair_iter = false;
      
      m_global_attached_var_ref_cnt = 0;
      m_global_attached_var_def_cnt = 0;
      
      m_usable = true;
      
#if PRINT_CHUNK_CODE || CHECK_CHUNK_CODE
      m_sequence_idx = s_sequence_idx++;
#endif
    }
    
    inline
    DAGNode(DAGVar *, Variable * const var)
    {
      assert(var != 0);
      
#if CHECK_CHUNK_CODE
      var->check_not_deleted();
#endif
      
      init();
      
      // :NOTE: Wei 2005-Apr-22:
      //
      // Because a DAGVar node will never be a needed node when there were no
      // other nodes use it. Thus I don't need to increase the
      // m_global_attached_def_ref_cnt here.
      //
      // I only increase or decrease m_global_attached_var_def_cnt at
      // add_attached_var() & remove_attached_var().
      switch (var->kind())
      {
      case Variable::TEMP:
        m_kind = DAG_VAR_TEMP;
        break;
        
      case Variable::GLOBAL:
        m_kind = DAG_VAR_GLOBAL;
        break;
        
      case Variable::CONST:
        m_kind = DAG_VAR_CONST;
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      m_lo_attached_var.push_back(var);
    }
    
    inline
    DAGNode(DAGCall *, Label * const routine_label)
      : m_kind(DAGCall::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = routine_label;
    }
    
    inline
    DAGNode(DAGReturn *)
      : m_kind(DAGReturn::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGRetrieve *,
            uint32_t const pop_count)
      : m_kind(DAGRetrieve::KIND)
    {
      init();
      
      u.m_RETRIEVE.m_pop_count = pop_count;
    }
    
    inline
    DAGNode(DAGAdd *)
      : m_kind(DAGAdd::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGAdd64 *)
      : m_kind(DAGAdd64::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGAdc *)
      : m_kind(DAGAdc::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGMov *)
      : m_kind(DAGMov::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGAnd *)
      : m_kind(DAGAnd::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGOr *)
      : m_kind(DAGOr::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGEor *)
      : m_kind(DAGEor::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGSub *)
      : m_kind(DAGSub::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGSbc *)
      : m_kind(DAGSbc::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGTst *)
      : m_kind(DAGTst::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGCmp *)
      : m_kind(DAGCmp::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGCmpEQ *, Label * const bb_label)
      : m_kind(DAGCmpEQ::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGNot *)
      : m_kind(DAGNot::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGMul32 *)
      : m_kind(DAGMul32::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGUMul64 *)
      : m_kind(DAGUMul64::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGSMul64 *)
      : m_kind(DAGSMul64::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGLSftLeft *)
      : m_kind(DAGLSftLeft::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGLSftRight *)
      : m_kind(DAGLSftRight::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGASftRight *)
      : m_kind(DAGASftRight::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGRrx *)
      : m_kind(DAGRrx::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGRor *)
      : m_kind(DAGRor::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGGoto *, Label * const bb_label)
      : m_kind(DAGGoto::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoEQ *, Label * const bb_label)
      : m_kind(DAGGotoEQ::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoNE *, Label * const bb_label)
      : m_kind(DAGGotoNE::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoCS *, Label * const bb_label)
      : m_kind(DAGGotoCS::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoCC *, Label * const bb_label)
      : m_kind(DAGGotoCC::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoMI *, Label * const bb_label)
      : m_kind(DAGGotoMI::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoPL *, Label * const bb_label)
      : m_kind(DAGGotoPL::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoVS *, Label * const bb_label)
      : m_kind(DAGGotoVS::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoVC *, Label * const bb_label)
      : m_kind(DAGGotoVC::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoHI *, Label * const bb_label)
      : m_kind(DAGGotoHI::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoLS *, Label * const bb_label)
      : m_kind(DAGGotoLS::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoGE *, Label * const bb_label)
      : m_kind(DAGGotoGE::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoLT *, Label * const bb_label)
      : m_kind(DAGGotoLT::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoGT *, Label * const bb_label)
      : m_kind(DAGGotoGT::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGGotoLE *, Label * const bb_label)
      : m_kind(DAGGotoLE::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGJump *)
      : m_kind(DAGJump::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGProduceCBit *)
      : m_kind(DAGProduceCBit::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGSetCBit *)
      : m_kind(DAGSetCBit::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGClearCBit *)
      : m_kind(DAGClearCBit::KIND)
    {
      init();
    }
    
    inline
    DAGNode(DAGLoadLabel *, Label * const bb_label)
      : m_kind(DAGLoadLabel::KIND)
    {
      init();
      
      u.m_LABEL.mp_label = bb_label;
    }
    
    inline
    DAGNode(DAGLoad *)
      : m_kind(DAGLoad::KIND)
    {
      init();
    }
    
    inline
    ~DAGNode()
    {
      // :NOTE: Wei 2005-Apr-15:
      //
      // Because this dag node will be deleted,
      // then the variable points to it should not still point to it.
      switch (m_kind)
      {
      case DAG_UMUL64:
      case DAG_SMUL64:
      case DAG_ADD64:
        clear_dag_node_of_attached_var_internal<VAR_ATTACH_HI>();
        
      default:
        clear_dag_node_of_attached_var_internal<VAR_ATTACH_LO>();
        break;
      }
    }
    
    // Inquiry
    
#if PRINT_CHUNK_CODE || CHECK_CHUNK_CODE
    inline uint32_t
    sequence_idx() const
    { return m_sequence_idx; }
#endif
    
    inline DAGKindEnum
    kind() const
    { return m_kind; }
    
    inline uint32_t
    pop_count() const
    {
      assert(DAG_RETRIEVE == m_kind);
      
      return u.m_RETRIEVE.m_pop_count;
    }
    
    inline Label *
    label() const
    {
      assert((DAG_GOTO == m_kind) ||
             (DAG_GOTO_EQ == m_kind) ||
             (DAG_GOTO_NE == m_kind) ||
             (DAG_GOTO_CS == m_kind) ||
             (DAG_GOTO_CC == m_kind) ||
             (DAG_GOTO_MI == m_kind) ||
             (DAG_GOTO_PL == m_kind) ||
             (DAG_GOTO_VS == m_kind) ||
             (DAG_GOTO_VC == m_kind) ||
             (DAG_GOTO_HI == m_kind) ||
             (DAG_GOTO_LS == m_kind) ||
             (DAG_GOTO_GE == m_kind) ||
             (DAG_GOTO_LT == m_kind) ||
             (DAG_GOTO_GT == m_kind) ||
             (DAG_GOTO_LE == m_kind) ||
             (DAG_CMP_EQ == m_kind) ||
             (DAG_LOAD_LABEL == m_kind) ||
             (DAG_CALL == m_kind));
      
      return u.m_LABEL.mp_label;
    }
    
    inline std::deque<ChildInfo> const &
    children() const
    { return m_children; }
    
    inline uint32_t
    in_flags() const
    { return m_in_flags; }
    
    inline uint32_t
    out_flags() const
    { return m_out_flags; }
    
    inline std::vector<DAGNodePair> const &
    prev_next_dag_node_pair() const
    { return m_prev_next_dag_node_pair; }
    
    inline uint32_t
    attached_var_amount() const
    { return m_hi_attached_var.size() + m_lo_attached_var.size(); }
    
    inline uint32_t
    global_attached_var_ref_cnt() const
    { return m_global_attached_var_ref_cnt; }
    
    inline uint32_t
    global_attached_var_def_cnt() const
    { return m_global_attached_var_def_cnt; }
    
    inline bool
    is_usable() const
    { return m_usable; }
    
    // Access
    
#if PRINT_CHUNK_CODE || CHECK_CHUNK_CODE
    static void
    reset_static_sequence_idx()
    { s_sequence_idx = 0; }
    
    inline void
    set_sequence_idx(uint32_t const idx)
    { m_sequence_idx = idx; }
#endif
    
    inline void
    clear_usable()
    { m_usable = false; }
    
    inline void
    set_kind(DAGKindEnum const kind)
    { m_kind = kind; }
    
    inline void
    set_const_var(Variable * const var)
    {
      assert(var != 0);
      
#if CHECK_CHUNK_CODE
      var->check_not_deleted();
#endif
      
      assert(DAG_VAR_CONST == m_kind);
      assert(Variable::CONST == var->kind());
      assert(1 == m_lo_attached_var.size());
      
      assert(0 == m_lo_attached_var.front()->dag_node());
      assert(0 == var->dag_node());
      
      m_lo_attached_var.front() = var;
    }
    
    inline void
    add_const_var(Variable * const var)
    {
      assert(var != 0);
      
#if CHECK_CHUNK_CODE
      var->check_not_deleted();
#endif
      
      assert(DAG_VAR_CONST == m_kind);
      assert(Variable::CONST == var->kind());
      assert(0 == m_lo_attached_var.size());
      
      assert(0 == var->dag_node());
      
      m_lo_attached_var.push_front(var);
    }
    
    inline std::vector<DAGNodePair> &
    prev_next_dag_node_pair()
    { return m_prev_next_dag_node_pair; }
    
    template<VarAttachedTypeEnum T_attach>
    inline std::list<DAGNode *> &
    ancestor()
    {
      switch (T_attach)
      {
      case VAR_ATTACH_HI:
        return m_hi_ancestor;
        
      case VAR_ATTACH_LO:
      case VAR_ATTACH_NORMAL:
        return m_lo_ancestor;
        
      default:
        assert(!"Should not reach here.");
        return m_lo_ancestor;
      }
    }
    
    inline std::deque<ChildInfo> &
    children()
    { return m_children; }
    
    // Operation
    
    template<VarAttachedTypeEnum T_attach>
    inline Variable *
    get_curr_emission_var()
    {
      return *(attached_var_iter<T_attach>());
    }
    
    template<VarAttachedTypeEnum T_attach>
    inline void
    jump_to_next_emission_var()
    {
      ++(attached_var_iter<T_attach>());
    }
    
    inline DAGNode *
    get_next_emission_node()
    {
      if (false == m_init_prev_next_dag_node_pair_iter)
      {
        m_prev_next_dag_node_pair_iter = m_prev_next_dag_node_pair.begin();
        
        m_init_prev_next_dag_node_pair_iter = true;
      }
      
      assert(m_prev_next_dag_node_pair_iter != m_prev_next_dag_node_pair.end());
      
      DAGNode * const node = (*m_prev_next_dag_node_pair_iter).mp_next_dag_node;
      
      ++m_prev_next_dag_node_pair_iter;
      
      return node;
    }
    
    inline void
    add_global_attached_var_ref_cnt(uint32_t const addon)
    { m_global_attached_var_ref_cnt += addon; }
    
    inline void
    inc_global_attached_var_ref_cnt()
    { ++m_global_attached_var_ref_cnt; }
    
    inline void
    minus_global_attached_var_ref_cnt(uint32_t const value)
    {
      assert(m_global_attached_var_ref_cnt >= value);
      
      m_global_attached_var_ref_cnt -= value;
    }
    
    inline bool
    is_needed() const
    { return ((m_global_attached_var_ref_cnt | m_global_attached_var_def_cnt) != 0) ? true : false; }
    
    template<VarAttachedTypeEnum T_attach>
    inline Variable *
    peek_first_usable_attached_var()
    {
      Variable * const var = get_first_usable_attached_var<T_attach>();
      
      assert(var != 0);
      
      return var;
    }
    
    template<VarAttachedTypeEnum T_attach>
    Variable *get_first_usable_attached_var();
    
    template<VarAttachedTypeEnum T_attach>
    inline void
    add_attached_var(Variable * const var)
    {
      assert(var != 0);
      
#if CHECK_CHUNK_CODE
      var->check_not_deleted();
#endif
      
      // :NOTE: Wei 2005-Apr-20:
      //
      // Because the last definition of this variable is attached to this dag node,
      // thus I will increment the ref count to prevent this dag node from deleting.
      switch (var->kind())
      {
      case Variable::CONST:
        break;
        
      case Variable::TEMP:
        if (var->is_act_as_global_var())
        {
          ++m_global_attached_var_def_cnt;
        }
        break;
        
      case Variable::GLOBAL:
        ++m_global_attached_var_def_cnt;
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      attached_var<T_attach>().push_back(var);
    }
    
    inline void
    remove_attached_var(Variable * const var)
    {
#if CHECK_CHUNK_CODE
      ensure_var_attached(var);
#endif
      
      switch (m_kind)
      {
      case DAG_VAR_TEMP:
      case DAG_VAR_GLOBAL:
      case DAG_VAR_CONST:
        break;
        
      default:
        switch (var->kind())
        {
        case Variable::CONST:
          break;
          
        case Variable::TEMP:
          if (var->is_act_as_global_var())
          {
            assert(m_global_attached_var_def_cnt > 0);
            
            --m_global_attached_var_def_cnt;
          }
          break;
          
        case Variable::GLOBAL:
          assert(m_global_attached_var_def_cnt > 0);
          
          --m_global_attached_var_def_cnt;
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
      }
    }
    
    inline void
    cleanup_attached_var()
    {
      switch (m_kind)
      {
      case DAG_UMUL64:
      case DAG_SMUL64:
      case DAG_ADD64:
        cleanup_attached_var_internal<VAR_ATTACH_HI>();
        
      default:
        cleanup_attached_var_internal<VAR_ATTACH_LO>();
        break;
      }
    }
        
    inline void
    set_in_out_flags(uint32_t const in_flags, uint32_t const out_flags)
    {
      assert(0 == (in_flags & ~(SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG)));
      assert(0 == (out_flags & ~(SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG)));
      
      m_in_flags = in_flags;
      m_out_flags = out_flags;
    }
    
    inline void
    set_in_flags(uint32_t const in_flags)
    {
      assert(0 == (in_flags & ~(SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG)));
      
      m_in_flags = in_flags;
    }
    
    inline void
    set_out_flags(uint32_t const out_flags)
    {
      assert(0 == (out_flags & ~(SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG)));
      
      m_out_flags = out_flags;
    }
    
    template<VarAttachedTypeEnum T_attach>
    inline void
    add_ancestor(DAGNode * const node)
    {
      assert(node != 0);
      
      switch (T_attach)
      {
      case VAR_ATTACH_HI:
        assert((DAG_UMUL64 == m_kind) ||
               (DAG_SMUL64 == m_kind) ||
               (DAG_ADD64 == m_kind));
        
        m_hi_ancestor.push_back(node);
        break;
        
      case VAR_ATTACH_LO:
        assert((DAG_UMUL64 == m_kind) ||
               (DAG_SMUL64 == m_kind) ||
               (DAG_ADD64 == m_kind));
        
      case VAR_ATTACH_NORMAL:
        m_lo_ancestor.push_back(node);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    template<VarAttachedTypeEnum T_attach>
    inline void
    remove_ancestor(DAGNode * const node)
    {
      assert(node != 0);
      
      switch (T_attach)
      {
      case VAR_ATTACH_HI:
        assert((DAG_UMUL64 == m_kind) ||
               (DAG_SMUL64 == m_kind) ||
               (DAG_ADD64 == m_kind));
        
        for (std::list<DAGNode *>::iterator iter = m_hi_ancestor.begin();
             iter != m_hi_ancestor.end();
             ++iter)
        {
          if ((*iter) == node)
          {
            m_hi_ancestor.erase(iter);
            
            return;
          }
        }
        
        assert(!"The argument node should be an ancestor of this node.");
        break;
        
      case VAR_ATTACH_LO:
        assert((DAG_UMUL64 == m_kind) ||
               (DAG_SMUL64 == m_kind) ||
               (DAG_ADD64 == m_kind));
        
      case VAR_ATTACH_NORMAL:
        for (std::list<DAGNode *>::iterator iter = m_lo_ancestor.begin();
             iter != m_lo_ancestor.end();
             ++iter)
        {
          if ((*iter) == node)
          {
            m_lo_ancestor.erase(iter);
            
            return;
          }
        }
        
        assert(!"The argument node should be an ancestor of this node.");
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    inline uint32_t
    ancestor_number() const
    { return m_hi_ancestor.size() + m_lo_ancestor.size(); }
    
    inline void
    clear_all_ancestor()
    {
      switch (m_kind)
      {
      case DAG_UMUL64:
      case DAG_SMUL64:
      case DAG_ADD64:
        m_hi_ancestor.clear();
        
      default:
        m_lo_ancestor.clear();
        break;
      }
    }
    
    void replace_child(DAGNode * const orig, ChildInfo const &after);
    
#if CHECK_CHUNK_CODE  
    template<VarAttachedTypeEnum T_attach>
    bool ensure_var_attached_internal(Variable * const var) const;
    
    void ensure_var_attached(Variable * const var) const;
#endif
    
#if PRINT_CHUNK_CODE
    void dump_attached_var() const;
    void dump_info() const;
#endif
  };
}

#endif

#endif
