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

#ifndef Variable_hpp
#define Variable_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>
#include <vector>
#include <list>

#include "Compiler.hpp"
#include "DefinedBBPair.hpp"
#include "LiveInterval.hpp"

#include "../Log.hpp"
#include "../MemoryPool.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class DAGNode;
  class BasicBlock;
  class LIR;
  class Register;
  
  // Variable type
  //
  
  class VarGlobal;
  class VarMemory;
  class VarTemp;
  class VarConst;
  
  typedef class Variable Variable;
  class Variable : public MemoryPool<Variable, 256>
  {
  public:
    
    enum KindEnum
    {
      GLOBAL,
      TEMP,
      MEMORY,
      CONST
    };
    typedef enum KindEnum KindEnum;
    
    enum GlobalVarEnum
    {
      // :NOTE: Wei 2004-Dec-31:
      //
      // The first integer number here must be 0,
      // because Chunk::m_global_var_table[] will use this enum
      // to index itself.
      CR_0 = 0,
      CR_1,
      CR_2,
      CR_3,
      CR_4,
      CR_5,
      CR_6,
      CR_7,
      CR_8,
      CR_9,
      CR_10,
      CR_11,
      CR_12,
      CR_13,
      CR_14,
      CR_15,
      
      CPSR,
      SPSR,
      
      EXCEPTION_TYPE,
      DEVICE_TIMER_INCREMENT,
      
      GLOBAL_VAR_TOTAL,
      
      CR_LR = CR_14,
      CR_PC = CR_15,
    };
    typedef enum GlobalVarEnum GlobalVarEnum;
    
#if PRINT_CHUNK_CODE
    static std::string const GLOBAL_VAR_NAME[GLOBAL_VAR_TOTAL];
#endif
    
    friend class MemoryPool<Variable, 256>;
    
  private:
    
    KindEnum m_kind;
    
    union
    {
      struct
      {
        GlobalVarEnum m_global_var;
      } m_GLOBAL;
      
      struct
      {
        uint32_t m_value;
      } m_CONST;
      
      struct
      {
        // :NOTE: Wei 2005-Mar-01:
        //
        // Used for determine the stack offset.
        uint32_t m_temp_idx;
        
        bool m_act_as_global_var;
      } m_TEMP;
    } u;
    
#if PRINT_CHUNK_CODE
    uint32_t m_sequence_idx;
#endif
    
    uint32_t m_idx;
    
    DAGNode *mp_dag_node;
    DAGNode *mp_curr_dag_node;
    
    VarAttachedTypeEnum m_attached_type;
    
    Variable *mp_next_attached_var;
    
    std::vector<Variable *> m_var_stack;
    
    Variable *mp_prev_link_var;
    Variable *mp_next_link_var;
    
    bool m_live_global;
    
    LiveInterval *mp_live_interval;
    
    Register *mp_preallocated_reg;
    std::vector<Register::KindEnum> m_excluded_reg;
    
    // :NOTE: Wei 2004-Dec-21:
    //
    // Used to indicate which basic block defines this variable.
    std::vector<DefinedBBPair *> m_defined_bb;
    
    std::list<LIR *> m_defined_lir;
    std::vector<BasicBlock *> m_iterated_dominance_frontier;
    
    bool m_is_loaded;
    bool m_has_already_get_ssa;
    
#if CHECK_CHUNK_CODE
    bool m_ssa_already_use;
    bool m_ssa_already_def;
#endif
    
#if PRINT_CHUNK_CODE
    uint32_t m_ssa_number;
    uint32_t m_ssa_number_stack;
#endif
    
    Variable *mp_original_ssa_var;
    Variable *mp_join_var;
    Variable *mp_mem_loc_var;
    
    Variable *mp_scc_value_number;
    
    uint32_t m_access_times;
    
  public:
    
    // Life cycle
    
    inline void
    init()
    {
      // :NOTE: Wei 2004-Nov-8:
      //
      // At first, each variable doesn't have its own DAG node,
      // we have to create one for it.
      mp_dag_node = 0;
      mp_curr_dag_node = 0;
      
      m_live_global = false;
      
      mp_original_ssa_var = this;
      mp_join_var = this;
      mp_mem_loc_var = this;
      
      m_is_loaded = false;
      m_has_already_get_ssa = false;
      
      mp_live_interval = 0;
      mp_preallocated_reg = 0;
      
      mp_scc_value_number = 0;
      
      m_access_times = 0;
      
#if CHECK_CHUNK_CODE
      m_ssa_already_use = false;
      m_ssa_already_def = false;
#endif
      
#if PRINT_CHUNK_CODE
      m_ssa_number = 0;
      m_ssa_number_stack = 0;
#endif
    }
    
    inline
    Variable(VarGlobal *,
             GlobalVarEnum const var)
      : m_kind(GLOBAL)
    {
      init();
      
      u.m_GLOBAL.m_global_var = var;
    }
    
    inline
    Variable(VarMemory *,
             GlobalVarEnum const var)
      : m_kind(MEMORY)
    {
      init();
      
      u.m_GLOBAL.m_global_var = var;
    }
    
    inline
    Variable(VarTemp *, bool const act_as_global_var)
      : m_kind(TEMP)
    {
      init();
      
      u.m_TEMP.m_act_as_global_var = act_as_global_var;
    }
    
    inline
    Variable(VarConst *, uint32_t const value)
      : m_kind(CONST)
    {
      init();
      
      u.m_CONST.m_value = value;
    }
    
    ~Variable();
    
    // Access
    
    inline void
    add_excluded_reg(Register::KindEnum const kind)
    { m_excluded_reg.push_back(kind); }
    
    inline void
    clear_dag_node()
    { mp_dag_node = 0; }
    
    inline void
    set_attached_type(VarAttachedTypeEnum const type)
    { m_attached_type = type; }
    
    inline void
    set_curr_dag_node(DAGNode * const node)
    {
      assert(node != 0);
      
      mp_curr_dag_node = node;
    }
    
    void update_attached_type_and_dag_node(VarAttachedTypeEnum type, DAGNode * const node);
    
    inline void
    set_next_attached_var(Variable * const var)
    {
      assert(var != 0);
      
      mp_next_attached_var = var;
    }
    
#if PRINT_CHUNK_CODE
    inline void
    set_sequence_idx(uint32_t const idx)
    { m_sequence_idx = idx; }
#endif
    
    inline void
    set_idx(uint32_t const idx)
    { m_idx = idx; }
    
    inline void
    set_temp_idx(uint32_t const idx)
    {
      assert(TEMP == m_kind);
      
      u.m_TEMP.m_temp_idx = idx;
    }
    
    std::vector<DefinedBBPair *> &
    defined_bb()
    { return m_defined_bb; }
    
    std::list<LIR *> &
    defined_lir()
    { return m_defined_lir; }
    
    std::vector<BasicBlock *> &
    iterated_dominance_frontier()
    { return m_iterated_dominance_frontier; }
    
    inline std::vector<Variable *> &
    var_stack()
    { return m_var_stack; }
    
    inline void
    set_prev_link_var(Variable * const var)
    {
      mp_prev_link_var = var;
    }
    
    inline void
    set_next_link_var(Variable * const var)
    {
      mp_next_link_var = var;
    }
    
    inline void
    set_live_global()
    { m_live_global = true; }
    
    inline void
    set_original_ssa_var(Variable * const var)
    {
      assert(var != 0);
      
      mp_original_ssa_var = var;
      
#if PRINT_CHUNK_CODE
      m_ssa_number = var->get_next_ssa_number();
#endif
    }
    
    inline void
    set_join_var(Variable * const var)
    {
      assert(var != 0);
      
      mp_join_var = var;
      
      var->increment_access_times(m_access_times);
    }
    
    inline void
    set_mem_loc_var(Variable * const var)
    {
      assert(var != 0);
      
      mp_mem_loc_var = var;
    }
    
    inline void
    set_live_interval(LiveInterval * const live)
    {
      assert(live != 0);
      
      mp_live_interval = live;
    }
    
    inline void
    set_is_loaded()
    {
      assert(false == m_is_loaded);
      
      m_is_loaded = true;
    }
    
    inline void
    set_preallocated_reg(Register * const reg)
    {
      assert(reg != 0);
      assert(0 == mp_preallocated_reg);
      
      mp_preallocated_reg = reg;
    }
    
    inline bool
    set_scc_value_number(Variable * const var)
    {
      assert(var != 0);
      
      Variable const * const old_scc_value_number = mp_scc_value_number;
      
      mp_scc_value_number = var;
      
#if PRINT_CHUNK_CODE
      if (old_scc_value_number != mp_scc_value_number)
      {
        if ((0 == old_scc_value_number) &&
            (this == mp_scc_value_number))
        {
          goto skip;
        }
        
        dump_info<true>();
        
        g_log_file << " reset Value to ";
        
        var->dump_info<true>();
        
        g_log_file << std::endl;
      }
      
    skip:
#endif
      
      return (old_scc_value_number != mp_scc_value_number) ? true : false;
    }
    
    // Inquiry
    
    inline KindEnum
    kind() const
    { return m_kind; }
    
    inline bool
    is_act_as_global_var() const
    {
      assert(TEMP == m_kind);
      
      return u.m_TEMP.m_act_as_global_var;
    }
    
    inline std::vector<Register::KindEnum> const &
    excluded_reg() const
    { return m_excluded_reg; }
    
    inline VarAttachedTypeEnum
    attached_type() const
    { return m_attached_type; }
    
    inline GlobalVarEnum
    global_var() const
    {
      assert(GLOBAL == m_kind);
      
      return u.m_GLOBAL.m_global_var;
    }
    
    inline uint32_t
    idx() const
    { return m_idx; }
    
    inline uint32_t
    temp_idx() const
    {
      assert(TEMP == m_kind);
      
      return u.m_TEMP.m_temp_idx;
    }
    
    inline uint32_t
    const_num() const
    {
      assert(CONST == m_kind);
      
      return u.m_CONST.m_value;
    }
    
    inline DAGNode *
    dag_node() const
    { return mp_dag_node; }
    
    inline DAGNode *
    curr_dag_node() const
    { return mp_curr_dag_node; }
    
    inline Variable *
    next_attached_var() const
    { return mp_next_attached_var; }
    
    inline Variable *
    prev_link_var() const
    { return mp_prev_link_var; }
    
    inline Variable *
    next_link_var() const
    { return mp_next_link_var; }
    
    inline Variable *
    original_ssa_var() const
    { return mp_original_ssa_var; }
    
    inline bool
    is_loaded() const
    { return m_is_loaded; }
    
    inline LiveInterval *
    live_interval() const
    { return mp_live_interval; }
    
    inline Register *
    preallocated_reg() const
    { return mp_preallocated_reg; }
    
    inline std::vector<Variable *> const &
    var_stack() const
    { return m_var_stack; }
    
    inline uint32_t
    access_times() const
    { return m_access_times; }
    
    // Operation
    
    inline void
    add_live_range_real(uint32_t const start, uint32_t const end)
    {
      if (0 == mp_live_interval)
      {
        mp_live_interval = new LiveInterval(start, end, this);
      }
      else
      {
        mp_live_interval->add_live_range(start, end);
      }
    }
    
    inline void
    add_live_range(uint32_t const start, uint32_t const end)
    { join_var()->add_live_range_real(start, end); }
    
    inline uint32_t
    hash_value() const
    {
      switch (m_kind)
      {
      case CONST:
        return u.m_CONST.m_value;
        
      case GLOBAL:
      case TEMP:
        assert(!"I don't expect that GLOBAL or TEMP variable should placed in a hash table.");
        return 0;
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
    }
    
    inline void
    add_defined_bb(BasicBlock * const bb)
    {
      assert(bb != 0);
      
      for (std::vector<DefinedBBPair *>::iterator iter = m_defined_bb.begin();
           iter != m_defined_bb.end();
           ++iter)
      {
        if ((*iter)->defined_bb() == bb)
        {
          (*iter)->increment_defined_count();
          
          return;
        }
      }
      
      m_defined_bb.push_back(new DefinedBBPair(bb));
    }
    
    inline void
    remove_defined_bb(BasicBlock * const bb)
    {
      assert(bb != 0);
      
      for (std::vector<DefinedBBPair *>::iterator iter = m_defined_bb.begin();
           iter != m_defined_bb.end();
           ++iter)
      {
        if ((*iter)->defined_bb() == bb)
        {
          assert((*iter)->defined_count() > 0);
          
          (*iter)->decrement_defined_count();
          
          if (0 == (*iter)->defined_count())
          {
            delete (*iter);
            
            m_defined_bb.erase(iter);
          }
          
          return;
        }
      }
      
      assert(!"Should not reach here.");
    }
    
    inline void
    add_defined_lir(LIR * const lir)
    {
      assert(lir != 0);
      
#if CHECK_CHUNK_CODE
      for (std::list<LIR *>::iterator iter = m_defined_lir.begin();
           iter != m_defined_lir.end();
           ++iter)
      {
        assert((*iter) != lir);
      }
#endif
      
      m_defined_lir.push_back(lir);
    }
    
    inline void
    remove_defined_lir(LIR * const lir)
    {
      assert(lir != 0);
      
      for (std::list<LIR *>::iterator iter = m_defined_lir.begin();
           iter != m_defined_lir.end();
           ++iter)
      {
        if ((*iter) == lir)
        {
          m_defined_lir.erase(iter);
          
          return;
        }
      }
      
      assert(!"Should not reach here.");
    }
    
    // :NOTE: Wei 2005-Mar-08:
    //
    // Const version.
    inline Variable const *
    join_var() const
    {
      if (mp_join_var == this)
      {
        return this;
      }
      else
      {
        return mp_join_var->join_var();
      }
    }
    
    // :NOTE: Wei 2005-Mar-08:
    //
    // Non-const version.
    inline Variable *
    join_var()
    {
      if (mp_join_var == this)
      {
        return this;
      }
      else
      {
        return mp_join_var->join_var();
      }
    }
    
    inline Variable *
    mem_loc_var() const
    { return mp_mem_loc_var; }
    
    inline bool
    is_live_global() const
    { return m_live_global; }
    
    template<DefUseEnum T_kind>
    Variable *get_ssa_var();
    
    void pop_ssa_var();
    
    inline void
    increment_access_times(uint32_t const value = 1)
    { m_access_times = m_access_times + value; }
    
    uint32_t memory_addr() const;
    
    inline uint32_t
    stack_offset() const
    {
      assert(TEMP == m_kind);
      
      return -(u.m_TEMP.m_temp_idx << 2);
    }
    
    inline Variable *
    scc_value_number() const
    { return mp_scc_value_number; }
    
#if PRINT_CHUNK_CODE
    template<bool T_ssa_enabled>
    void dump_info() const;
    
    inline uint32_t
    get_next_ssa_number()
    { return ++m_ssa_number_stack; }
#endif
    
#if CHECK_CHUNK_CODE
    template<DefUseEnum T_type>
    void check_ssa_property(BasicBlock const * const bb);
    
    void check_defined_lir();
    
    void check_not_deleted();
#endif
  };
  
  inline Variable::GlobalVarEnum &
  operator++(Variable::GlobalVarEnum &var) // prefix
  {
    return var = static_cast<Variable::GlobalVarEnum>(var + 1);
  }
}

#endif

#endif
