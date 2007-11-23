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

#ifndef Pattern_DAGSbc_hpp
#define Pattern_DAGSbc_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGSbc
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_SBC == node->kind());
      
      assert(CARRY_FLAG == node->in_flags());
      
      assert(1 == node->attached_var_amount());
      
      assert(node->attached_var<VAR_ATTACH_NORMAL>().begin() ==
             node->attached_var_iter<VAR_ATTACH_NORMAL>());
      
      std::deque<ChildInfo> const &children = node->children();
      
      load_native_flags(curr_bb, CARRY_FLAG);
      
      // :NOTE: Wei 2005-May-07:
      //
      // Because the SBC and RSC of ARM are
      //
      // 'Rd = Rn - shifter_operand - NOT(C Flag)'
      //
      // or
      //
      // 'Rd = shifter_operand - Rn - NOT(C Flag)'
      curr_bb->append_lir<true>(new LIR(static_cast<LIRCmc *>(0)));
      
      ChildInfo const &child_0 = children.front();
      ChildInfo const &child_1 = children.back();
      
      switch (child_0.node()->kind())
      {
      case DAG_VAR_CONST:
        switch (child_1.node()->kind())
        {
        case DAG_VAR_CONST:
          assert(!"The 1st operand of the SBC instruction in x86 can't be a constant value.");
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          {
            Variable * const tmp = gp_chunk->get_new_temp();
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                              child_0.peek_first_usable_attached_var()->const_num(),
                                              tmp));
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRSbc *>(0),
                                              tmp,
                                              child_1.peek_first_usable_attached_var(),
                                              node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          }
          break;
          
        default:
          assert(!"The 1st operand of the SBC instruction in x86 can't be a constant value.");
          break;
        }
        break;
        
      case DAG_VAR_TEMP:
      case DAG_VAR_GLOBAL:
        switch (child_1.node()->kind())
        {
        case DAG_VAR_CONST:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRSbc *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var()->const_num(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRSbc *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
          
        default:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRSbc *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
        }
        break;
        
      default:
        switch (child_1.node()->kind())
        {
        case DAG_VAR_CONST:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRSbc *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var()->const_num(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRSbc *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
          
        default:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRSbc *>(0),
                                            child_0.peek_first_usable_attached_var(),
                                            child_1.peek_first_usable_attached_var(),
                                            node->get_curr_emission_var<VAR_ATTACH_NORMAL>()));
          break;
        }
        break;
      }
      
      if (node->out_flags() & CARRY_FLAG)
      {
        curr_bb->append_lir<true>(new LIR(static_cast<LIRCmc *>(0)));
      }
      
      store_native_flags(curr_bb, node->out_flags());
      
      node->get_curr_emission_var<VAR_ATTACH_NORMAL>()->set_curr_dag_node(node);
      
      node->jump_to_next_emission_var<VAR_ATTACH_NORMAL>();
    }
  }
}

#endif

#endif
