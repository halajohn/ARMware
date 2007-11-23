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

namespace ARMware
{
#if PRINT_CHUNK_CODE
  template<bool T_ssa_enabled, bool T_print_memory_operation_idx>
  void
  LIR::dump_info() const
  {
    g_log_file << KIND_NAME[m_kind];
    
    switch (m_kind)
    {
    case PHI:
      // :NOTE: Wei 2004-Dec-24:
      //
      // Because I will dump the LIR info before the insertion of the Phi nodes,
      // thus I shouldn't face a condition of dumpping the info of Phi nodes.
      assert(true == T_ssa_enabled);
      
      g_log_file << " ";
      
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      g_log_file << ": ";
      
      for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
           iter != m_operand.end();
           ++iter)
      {
        (*iter)->variable()->dump_info<T_ssa_enabled>();
        g_log_file << " ";
      }
      
      g_log_file << std::endl;
      break;
      
    case FRAME:
      if (true == T_print_memory_operation_idx)
      {
        g_log_file << " ("
                   << m_memory_operation_idx
                   << ")";
      }
      
      g_log_file << " [";
      
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
           iter != m_memory_def_list.end();
           ++iter)
      {
        (*iter)->variable()->dump_info<T_ssa_enabled>();
        g_log_file << " ";
      }
      
      g_log_file << "]"
                 << std::endl;
      break;
      
    case CALL:
      if (true == T_print_memory_operation_idx)
      {
        g_log_file << " ("
                   << m_memory_operation_idx
                   << ")";
      }
      
      g_log_file << " [";
      
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_use_list.begin();
           iter != m_memory_use_list.end();
           ++iter)
      {
        (*iter)->variable()->dump_info<T_ssa_enabled>();
        g_log_file << " ";
      }
      
      g_log_file << "] [";
      
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
           iter != m_memory_def_list.end();
           ++iter)
      {
        (*iter)->variable()->dump_info<T_ssa_enabled>();
        g_log_file << " ";
      }
      
      g_log_file << "]"
                 << std::endl;
      break;
      
    case PUSH_R:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case PUSH_I:
      g_log_file << " "
                 << m_operand.front()->imm()
                 << std::endl;
      break;
      
    case RETURN:
      g_log_file << " ";
      
      g_log_file << std::endl;
      break;
      
    case MOV_R_2_R:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case MOV_I_2_R:
      g_log_file << " 0x"
                 << std::hex
                 << m_operand.front()->imm();
      
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case MOV_FROM_M:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case MOV_FROM_A:
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case MOV_LABEL_2_R:
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case LOAD_GLOBAL:
      if (true == T_print_memory_operation_idx)
      {
        g_log_file << " ("
                   << m_memory_operation_idx
                   << ")";
      }
      
      g_log_file << " ";
      
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " [";
      
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_use_list.begin();
           iter != m_memory_use_list.end();
           ++iter)
      {
        (*iter)->variable()->dump_info<T_ssa_enabled>();
        g_log_file << " ";
      }
      
      g_log_file << "]"
                 << std::endl;
      break;
      
    case STORE_GLOBAL:
      if (true == T_print_memory_operation_idx)
      {
        g_log_file << " ("
                   << m_memory_operation_idx
                   << ")";
      }
      
      g_log_file << " ";
      
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " [";
      
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_use_list.begin();
           iter != m_memory_use_list.end();
           ++iter)
      {
        (*iter)->variable()->dump_info<T_ssa_enabled>();
        g_log_file << " ";
      }
      
      g_log_file << "] [";
      
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
           iter != m_memory_def_list.end();
           ++iter)
      {
        (*iter)->variable()->dump_info<T_ssa_enabled>();
        g_log_file << " ";
      }
      
      g_log_file << "]"
                 << std::endl;
      break;
      
    case ADD_RR:
    case ADC_RR:
    case SUB_RR:
    case SBC_RR:
    case AND_RR:
    case OR_RR:
    case EOR_RR:
    case LSFT_LEFT_RR:
    case LSFT_RIGHT_RR:
    case ASFT_RIGHT_RR:
    case ROR_RR:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_operand.back()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case ADD_RI:
    case ADC_RI:
    case SUB_RI:
    case SBC_RI:
    case AND_RI:
    case OR_RI:
    case EOR_RI:
    case LSFT_LEFT_RI:
    case LSFT_RIGHT_RI:
    case ASFT_RIGHT_RI:
    case ROR_RI:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " 0x"
                 << std::hex
                 << m_operand.back()->imm();
      
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case MUL32:
    case IMUL32:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_operand.back()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_dest.back()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case CMP_RR:
    case TST_RR:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_operand.back()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case CMP_RI:
    case TST_RI:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " 0x"
                 << std::hex
                 << m_operand.back()->imm()
                 << std::endl;
      break;
      
    case SET_S:
    case SET_Z:
    case SET_C:
    case SET_O:
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case LOAD_SP:
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case STORE_SP:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case ADD_SP:
      g_log_file << " "
                 << m_operand.front()->imm()
                 << std::endl;
      break;
      
    case BT:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " "
                 << m_operand.back()->imm();
      
      g_log_file << std::endl;
      break;
      
    case JMP_R:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case NOT:
      g_log_file << " ";
      m_operand.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << " ";
      m_dest.front()->variable()->dump_info<T_ssa_enabled>();
      
      g_log_file << std::endl;
      break;
      
    case JMP_I:
    case JZ:
    case JNZ:
    case JC:
    case JNC:
    case JS:
    case JNS:
    case JO:
    case JNO:
    case JGE:
    case JL:
    case JG:
    case JLE:
      switch (m_operand.front()->label()->kind())
      {
      case Label::BASIC_BLOCK:
        g_log_file << " (BB)"
                   << m_operand.front()->label()->bb()->sequence_idx();
        break;
        
      default:
        break;
      }
      
      g_log_file << std::endl;
      break;
      
    case PUSHF:
    case POPF:
    case CMC:
      g_log_file << std::endl;
      break;
      
    default:
      std::cerr << m_kind
                << std::endl;
      
      assert(!"Should not reach here.");
      break;
    }
  }
#endif
  
  template<bool T_need_load_store>
  void
  LIR::before_append_lir(BasicBlock * const bb)
  {
    assert(m_kind != PHI);
    
    // :NOTE: Wei 2005-May-01:
    //
    // Determine the defined basic block of the dest variable.
    for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
         iter != m_dest.end();
         ++iter)
    {
      if (OperandKind::VARIABLE == (*iter)->kind())
      {
        (*iter)->variable()->add_defined_bb(bb);
      }
    }
    
    // :NOTE: Wei 2005-May-20:
    //
    // Extends to support SCCVN.
    for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
         iter != m_memory_def_list.end();
         ++iter)
    {
      assert(OperandKind::VARIABLE == (*iter)->kind());
      
      (*iter)->variable()->add_defined_bb(bb);
    }
    
    if (true == T_need_load_store)
    {
      // :NOTE: Wei 2005-May-01:
      //
      // Load global variables.
      for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
           iter != m_operand.end();
           ++iter)
      {
        switch ((*iter)->kind())
        {
        case OperandKind::VARIABLE:
          {
            Variable * const var = (*iter)->variable();
            
            if (Variable::GLOBAL == var->kind())
            {
              bb->template append_lir<false>(new LIR(static_cast<LIRLoad_global *>(0), var));
            }
          }
          break;
          
        default:
          break;
        }
      }
    }
  }
  
  template<bool T_need_load_store>
  void
  LIR::after_append_lir(BasicBlock * const bb)
  {
    if (true == T_need_load_store)
    {
      // :NOTE: Wei 2005-May-01:
      //
      // Store global variables.
      for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
           iter != m_dest.end();
           ++iter)
      {
        switch ((*iter)->kind())
        {
        case OperandKind::VARIABLE:
          {
            Variable * const var = (*iter)->variable();
            
            if (Variable::GLOBAL == var->kind())
            {
              bb->template append_lir<false>(new LIR(static_cast<LIRStore_global *>(0), var));
            }
          }
          break;
          
        default:
          break;
        }
      }
    }
  }
  
  template<ConsiderMemoryVarEnum T_consider>
  void
  LIR::determine_def_use(BasicBlock * const bb)
  {
    // :NOTE: Wei 2005-Jan-06:
    //
    // The <DEF> have to be in front of <USE>,
    // because <DEF> will be erase the <USE> of the following LIR.
    //
    // Ex: USE[x=z; x=x+1; y=1;] = {z} (x not in USE)
    //     DEF[x=z; x=x+1; y=1;] = {x, y}
    for (std::vector<OperandKind *>::const_iterator iter = m_dest.begin();
         iter != m_dest.end();
         ++iter)
    {
      if (OperandKind::VARIABLE == (*iter)->kind())
      {
        bb->set_def_use<DEF>((*iter)->variable());
      }
    }
    
    if (m_kind != PHI)
    {
      // :NOTE: Wei 2005-May-20:
      //
      // I read a new paper, it says:
      //
      // For some reason, add MOV instructions in each predecesor basic block
      // of a basic block which defines PHI instructions.
      //
      // I don't know why before, but maybe I know it now.
      // Because this way I can simply the analysis of the live variables.
      // I can not add the operands of a PHI instruction in the USE set of its basic block,
      // if I add, all operands of a PHI instruction will be appear in every upper paths from
      // the basic block, and this is wrong, because one PHI operand should only be appear in one
      // upper path. Thus I shouldn't include the PHI operands into the USE set here.
      // Because the newly added MOV instruction in the predecesor block implicitly does this thing.
      for (std::vector<OperandKind *>::const_iterator iter = m_operand.begin();
           iter != m_operand.end();
           ++iter)
      {
        if (OperandKind::VARIABLE == (*iter)->kind())
        {
          bb->set_def_use<USE>((*iter)->variable());
        }
      }
    }
    
    if (CONSIDER_MEMORY_VAR == T_consider)
    {
      // :NOTE: Wei 2005-May-20:
      //
      // Extends the live variable analysis to support SCCVN.
      // Still, DEF should be in front of USE.
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_def_list.begin();
           iter != m_memory_def_list.end();
           ++iter)
      {
        assert(OperandKind::VARIABLE == (*iter)->kind());
        
        bb->set_def_use<DEF>((*iter)->variable());
      }
      
      for (std::vector<OperandKind *>::const_iterator iter = m_memory_use_list.begin();
           iter != m_memory_use_list.end();
           ++iter)
      {
        assert(OperandKind::VARIABLE == (*iter)->kind());
        
        bb->set_def_use<USE>((*iter)->variable());
      }
    }
  }
}

#endif
