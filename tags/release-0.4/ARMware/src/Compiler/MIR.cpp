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

#if PRINT_CHUNK_CODE
#include <iostream>
#endif

#include "MIR.hpp"

#include "../Log.hpp"
#include "../Chunk.hpp"

namespace ARMware
{
#if ENABLE_DYNAMIC_TRANSLATOR
  //////////////////////////////// Public /////////////////////////////////////
  //============================= Life cycle ==================================
  
  void
  MIR::set_in_out_flags(uint32_t const in_flags, uint32_t const out_flags)
  {
    assert(0 == (in_flags & ~(SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG)));
    assert(0 == (out_flags & ~(SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG)));
    
    m_in_flags = in_flags;
    m_out_flags = out_flags;
    
    if (out_flags != 0)
    {
      // :NOTE: Wei 2004-Nov-3:
      //
      // Because changing ARM flags might be closing a condition block,
      // thus we have to check it now.
      gp_chunk->check_end_a_cond_block(out_flags);
    }
  }
  
  void
  MIR::emit_native_code(std::vector<uint8_t> &native_code_buffer)
  {
    assert(MIR_DATA32 == m_kind);
    
    switch (u.m_DATA32.mp_label->kind())
    {
    case Label::MIR_INST:
      // :NOTE: Wei 2005-Mar-21:
      //
      // This must be an unused jump table entry.
      assert(0 == u.m_DATA32.mp_label->mir());
      
      native_code_buffer.push_back(0);
      native_code_buffer.push_back(0);
      native_code_buffer.push_back(0);
      native_code_buffer.push_back(0);
      break;
      
    case Label::BASIC_BLOCK:
      {
        uint32_t const offset = native_code_buffer.size();
        
        native_code_buffer.push_back(0);
        native_code_buffer.push_back(0);
        native_code_buffer.push_back(0);
        native_code_buffer.push_back(0);
        
        u.m_DATA32.mp_label->add_back_patch_info(Label::ABSOLUTE, offset);
      }
    break;
    
    case Label::ARM_NORMAL_INST:
    case Label::ARM_FIRST_INST:
    case Label::ROUTINE:
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
#if PRINT_CHUNK_CODE
  std::string const MIR::KIND_NAME[] =
  {
    "CALL_0",
    "CALL_1",
    "RETURN",
    "RETRIEVE",
    
    "ADD",
    "ADD64",
    "ADC",
    "MOV",
    "AND",
    "OR",
    "EOR",
    "SUB",
    "SBC",
    "TST",
    "CMP",
    "CMP_EQ",
    "NOT",
    
    "MUL32",
    "UMUL64",
    "SMUL64",
    
    "LSFT_LEFT",
    "LSFT_RIGHT",
    "ASFT_RIGHT",
    "RRX",
    "ROR",
    
    "GOTO",
    
    "GOTO_EQ",
    "GOTO_NE",
    "GOTO_CS",
    "GOTO_CC",
    "GOTO_MI",
    "GOTO_PL",
    "GOTO_VS",
    "GOTO_VC",
    "GOTO_HI",
    "GOTO_LS",
    "GOTO_GE",
    "GOTO_LT",
    "GOTO_GT",
    "GOTO_LE",
    
    "JUMP",
    
    "PRODUCE_CBIT",
    "SET_CBIT",
    
    "DATA32",
    
    "LOAD_LABEL",
    "LOAD"
  };
  
  void
  MIR::dump_info() const
  {
    // :NOTE: Wei 2004-Nov-6:
    //
    // Check to see whether this MIR is the leader of a basic block.
    if (0 == mp_prev_mir)
    {
      // :NOTE: Wei 2004-Nov-6:
      //
      // The first MIR always is a leader.
      g_log_file << "> ";
    }
    else
    {
      g_log_file << ((mp_prev_mir->bb() != mp_bb) ? "> " : "  ");
    }
    
    g_log_file << "["
               << ((m_in_flags & SIGN_FLAG) ? "N" : "-")
               << ((m_in_flags & ZERO_FLAG) ? "Z" : "-")
               << ((m_in_flags & CARRY_FLAG) ? "C" : "-")
               << ((m_in_flags & OVERFLOW_FLAG) ? "V" : "-")
               << "] ["
               << ((m_out_flags & SIGN_FLAG) ? "N" : "-")
               << ((m_out_flags & ZERO_FLAG) ? "Z" : "-")
               << ((m_out_flags & CARRY_FLAG) ? "C" : "-")
               << ((m_out_flags & OVERFLOW_FLAG) ? "V" : "-")
               << "] ";
    
    g_log_file << KIND_NAME[m_kind];
    
    switch (m_kind)
    {
    case MIR_CALL_0:
      assert(0 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << std::endl;
      break;
      
    case MIR_CALL_1:
      assert(1 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_RETURN:
      assert(1 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_RETRIEVE:
      assert(0 == m_operand_var.size());
      assert(m_dest_var.size() <= 1);
      
      if (m_dest_var.size() != 0)
      {
        g_log_file << " ";
        m_dest_var.front()->dump_info<false>();
      }
      
      g_log_file << std::endl;
      break;
      
    case MIR_ADD:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_ADD64:
      assert(4 == m_operand_var.size());
      assert(2 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var[0]->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var[1]->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var[2]->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var[3]->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.back()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_ADC:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_MOV:
      assert(1 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_AND:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_OR:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_EOR:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_SUB:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_SBC:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_TST:
      assert(2 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_CMP:
      assert(2 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_CMP_EQ:
      assert(2 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      u.m_CMP_EQ.mp_label->dump_info();
      
      g_log_file << std::endl;
      break;
      
    case MIR_NOT:
      assert(1 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_MUL32:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_UMUL64:
      assert(2 == m_operand_var.size());
      assert(2 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.back()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_SMUL64:
      assert(2 == m_operand_var.size());
      assert(2 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.back()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_LSFT_LEFT:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_LSFT_RIGHT:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_ASFT_RIGHT:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_RRX:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_ROR:
      assert(2 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
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
      assert(0 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      u.m_GOTO.mp_label->dump_info();
      
      g_log_file << std::endl;
      break;
      
    case MIR_JUMP:
      assert(1 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_PRODUCE_CBIT:
      assert(2 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_operand_var.back()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_CLEAR_CBIT:
      assert(0 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << std::endl;
      break;
      
    case MIR_DATA32:
      assert(0 == m_operand_var.size());
      assert(0 == m_dest_var.size());
      
      g_log_file << " ";
      u.m_DATA32.mp_label->dump_info();
      
      g_log_file << std::endl;
      break;
      
    case MIR_LOAD_LABEL:
      assert(0 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      u.m_LOAD_LABEL.mp_label->dump_info();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    case MIR_LOAD:
      assert(1 == m_operand_var.size());
      assert(1 == m_dest_var.size());
      
      g_log_file << " ";
      m_operand_var.front()->dump_info<false>();
      
      g_log_file << " ";
      m_dest_var.front()->dump_info<false>();
      
      g_log_file << std::endl;
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
#endif
#endif
}

#endif
