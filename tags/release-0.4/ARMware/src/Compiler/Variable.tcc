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

namespace ARMware
{
#if ENABLE_DYNAMIC_TRANSLATOR
  template<DefUseEnum T_kind>
  Variable *
  Variable::get_ssa_var()
  {
    // :NOTE: Wei 2005-Jan-06:
    //
    // I should always call this function for a normal variable, not a ssa variable.
    // And a normal variable should have its origianl_ssa_var() the same with itself.
    assert(mp_original_ssa_var == this);
    
    if (DEF == T_kind)
    {
      if (false == m_has_already_get_ssa)
      {
        // :NOTE: Wei 2005-Jan-05:
        //
        // This is the first SSA variable,
        // thus add itself to its variable stack.
        m_var_stack.push_back(this);
        
        m_has_already_get_ssa = true;
        
        return this;
      }
      else
      {
        Variable *new_var;
        
        switch (m_kind)
        {
        case Variable::GLOBAL:
          new_var = gp_chunk->add_new_var<Variable::GLOBAL>(new Variable(static_cast<VarGlobal *>(0), u.m_GLOBAL.m_global_var));
          break;
          
        case Variable::MEMORY:
          new_var = gp_chunk->add_new_var<Variable::MEMORY>(new Variable(static_cast<VarMemory *>(0), u.m_GLOBAL.m_global_var));
          break;
          
        case Variable::TEMP:
          new_var = gp_chunk->get_new_temp();
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          new_var = 0;
          break;
        }
        
        // :NOTE: Wei 2005-Jan-06:
        //
        // Replace the original ssa variable of the new_var from new_var to var.
        new_var->set_original_ssa_var(this);
        
        m_var_stack.push_back(new_var);
        
        return new_var;
      }
    }
    else
    {
      assert(m_var_stack.size() != 0);
      
      return m_var_stack.back();
    }
  }
  
#if CHECK_CHUNK_CODE
  template<DefUseEnum T_type>
  inline void
  Variable::check_ssa_property(BasicBlock const * const bb)
  {
    assert(bb != 0);
    
    switch (T_type)
    {
    case DEF:
      // :NOTE: Wei 2005-Jan-19:
      //
      // SSA properties:
      //
      // 3) A preorder DFS over the CFG visits variable definitions before uses.
      assert(false == m_ssa_already_use);
      
      // :NOTE: Wei 2005-Jan-19:
      //
      // SSA properties:
      //
      // 1) Each variable has 1 definition at most.
      assert(false == m_ssa_already_def);
      
      m_ssa_already_def = true;
      break;
      
    case USE:
      m_ssa_already_use = true;
      
      if (true == m_ssa_already_def)
      {
        assert(1 == m_defined_bb.size());
        assert(1 == m_defined_bb.front()->defined_count());
        assert(1 == m_defined_lir.size());
        assert(m_defined_lir.front()->defined_bb() == m_defined_bb.front()->defined_bb());
        
        // :NOTE: Wei 2005-Jan-19:
        //
        // SSA properties:
        //
        // 2) A definition of a variable dominates all uses.
        //    Thus, The mp_defined_bb should dominate bb.
        assert(true == m_defined_bb.front()->defined_bb()->is_dominated_by_me(bb));
      }
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
#endif
  
#if PRINT_CHUNK_CODE
  template<bool T_ssa_enabled>
  void
  Variable::dump_info() const
  {
    switch (m_kind)
    {
    case Variable::MEMORY:
      g_log_file << "MEMORY:";
      
      goto print;
      
    case Variable::GLOBAL:
      g_log_file << "GLOBAL:";
      
    print:
      
      switch (u.m_GLOBAL.m_global_var)
      {
      case CR_0: g_log_file << "CR_0"; break;
      case CR_1: g_log_file << "CR_1"; break;
      case CR_2: g_log_file << "CR_2"; break;
      case CR_3: g_log_file << "CR_3"; break;
      case CR_4: g_log_file << "CR_4"; break;
      case CR_5: g_log_file << "CR_5"; break;
      case CR_6: g_log_file << "CR_6"; break;
      case CR_7: g_log_file << "CR_7"; break;
      case CR_8: g_log_file << "CR_8"; break;
      case CR_9: g_log_file << "CR_9"; break;
      case CR_10: g_log_file << "CR_10"; break;
      case CR_11: g_log_file << "CR_11"; break;
      case CR_12: g_log_file << "CR_12"; break;
      case CR_13: g_log_file << "CR_13"; break;
      case CR_14: g_log_file << "CR_14"; break;
      case CR_15: g_log_file << "CR_15"; break;
      case CPSR: g_log_file << "CPSR"; break;
      case SPSR: g_log_file << "SPSR"; break;
      case EXCEPTION_TYPE: g_log_file << "EXCEPT_TYPE"; break;
      case DEVICE_TIMER_INCREMENT: g_log_file << "DEV_TIMER_INC"; break;
        
      case GLOBAL_VAR_TOTAL:
        assert(!"I should not use GLOBAL_VAR_TOTAL as a real global variable.");
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case Variable::TEMP:
      g_log_file << "TEMP:"
                 << std::dec
                 << m_sequence_idx;
      break;
      
    case Variable::CONST:
      g_log_file << "CONST:0x"
                 << std::hex
                 << u.m_CONST.m_value;
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    if (true == T_ssa_enabled)
    {
      g_log_file << "-"
                 << std::dec
                 << m_ssa_number;
    }
    
    Register *reg;
    
    if (join_var()->live_interval() != 0)
    {
      reg = join_var()->live_interval()->preallocated_reg();
    }
    else
    {
      reg = mp_preallocated_reg;
    }
    
    if (reg != 0)
    {
      g_log_file << "(";
      reg->dump_info();
      g_log_file << ")";
    }
  }
#endif
#endif // ENABLE_DYNAMIC_TRANSLATOR
}
