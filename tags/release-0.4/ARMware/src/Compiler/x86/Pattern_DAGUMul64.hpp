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

#ifndef Pattern_DAGUMul64_hpp
#define Pattern_DAGUMul64_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGUMul64
  {
    inline void
    emit_lir_code_helper_internal(BasicBlock * const curr_bb,
                                  DAGNode * const node,
                                  Variable * const tmp1,
                                  Variable * const var2,
                                  /* HI */ Variable * const hi_dest,
                                  /* LO */ Variable * const lo_dest)
    {
      Variable * const hi_dest_tmp = gp_chunk->get_new_temp();
      Variable * const lo_dest_tmp = gp_chunk->get_new_temp();
      
      tmp1->set_preallocated_reg(new Register(Register::EAX, true));
      
      curr_bb->append_lir<true>(new LIR(static_cast<LIRMul32 *>(0),
                                        var2,
                                        tmp1,
                                        hi_dest_tmp,
                                        lo_dest_tmp));
      
      hi_dest_tmp->set_preallocated_reg(new Register(Register::EDX, true));
      lo_dest_tmp->set_preallocated_reg(new Register(Register::EAX, true));
      
      curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0), hi_dest_tmp, hi_dest));
      curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0), lo_dest_tmp, lo_dest));
      
      switch (node->out_flags())
      {
      case SIGN_FLAG:
        curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0), hi_dest, hi_dest));
        
        store_native_flags(curr_bb, SIGN_FLAG);
        break;
        
      case ZERO_FLAG:
        {
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0), hi_dest, hi_dest));
          
          store_native_flags(curr_bb, ZERO_FLAG);
          load_native_flags(curr_bb, ZERO_FLAG);
          
          Label * const label = gp_chunk->add_new_label(new Label(static_cast<LabelBB *>(0)));
          
          curr_bb->append_lir<true>(new LIR(static_cast<LIRJnz *>(0), label));
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0), lo_dest, lo_dest));
          
          store_native_flags(curr_bb, ZERO_FLAG);
          
          curr_bb->back_patch_bb_label_use_next_lir(label);
        }
        break;
        
      case SIGN_FLAG | ZERO_FLAG:
        {
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0), hi_dest, hi_dest));
          
          store_native_flags(curr_bb, SIGN_FLAG | ZERO_FLAG);
          load_native_flags(curr_bb, ZERO_FLAG);
          
          Label * const label = gp_chunk->add_new_label(new Label(static_cast<LabelBB *>(0)));
          
          curr_bb->append_lir<true>(new LIR(static_cast<LIRJnz *>(0), label));
          curr_bb->append_lir<true>(new LIR(static_cast<LIRTst *>(0), lo_dest, lo_dest));
          
          store_native_flags(curr_bb, ZERO_FLAG);
          
          curr_bb->back_patch_bb_label_use_next_lir(label);
        }
        break;
        
      case 0:
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    inline void
    emit_lir_code_helper(BasicBlock * const curr_bb,
                         DAGNode * const node,
                         uint32_t const value,
                         Variable * const var2,
                         /* HI */ Variable * const hi_dest,
                         /* LO */ Variable * const lo_dest)
    {
      Variable * const tmp1 = gp_chunk->get_new_temp();
      
      curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                        value,
                                        tmp1));
      
      return emit_lir_code_helper_internal(curr_bb, node, tmp1, var2, hi_dest, lo_dest);
    }
    
    inline void
    emit_lir_code_helper(BasicBlock * const curr_bb,
                         DAGNode * const node,
                         Variable * const var1,
                         Variable * const var2,
                         /* HI */ Variable * const hi_dest,
                         /* LO */ Variable * const lo_dest)
    {
      Variable * const tmp1 = gp_chunk->get_new_temp();
      
      curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                        var1,
                                        tmp1));
      
      return emit_lir_code_helper_internal(curr_bb, node, tmp1, var2, hi_dest, lo_dest);
    }
    
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_UMUL64 == node->kind());
      
      assert(0 == node->in_flags());
      assert(0 == (node->out_flags() & (CARRY_FLAG | OVERFLOW_FLAG)));
      
      assert(node->attached_var_amount() != 0);
      
      Variable *src_hi_var = 0;
      Variable *src_lo_var = 0;
      
      if (0 == node->out_flags())
      {
        // :NOTE: Wei 2005-May-08:
        //
        // Try to get previous still valid attached variable.
        src_hi_var = node->get_first_usable_attached_var<VAR_ATTACH_HI>();
        src_lo_var = node->get_first_usable_attached_var<VAR_ATTACH_LO>();
      }
      else
      {
        assert(node->attached_var<VAR_ATTACH_HI>().begin() == node->attached_var_iter<VAR_ATTACH_HI>());
        assert(node->attached_var<VAR_ATTACH_LO>().begin() == node->attached_var_iter<VAR_ATTACH_LO>());
      }
      
      if ((src_hi_var != 0) && (src_lo_var != 0))
      {
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          src_hi_var,
                                          node->get_curr_emission_var<VAR_ATTACH_HI>()));
        
        curr_bb->append_lir<true>(new LIR(static_cast<LIRMov *>(0),
                                          src_lo_var,
                                          node->get_curr_emission_var<VAR_ATTACH_LO>()));
      }
      else
      {
        std::deque<ChildInfo> const &children = node->children();
        
        ChildInfo const &child_0 = children.front();
        ChildInfo const &child_1 = children.back();
        
        switch (child_0.node()->kind())
        {
        case DAG_VAR_CONST:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            assert(!"Both operands of the MUL instruction in x86 can't be constant values.");
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            emit_lir_code_helper(curr_bb,
                                 node,
                                 child_0.peek_first_usable_attached_var()->const_num(),
                                 child_1.peek_first_usable_attached_var(),
                                 node->get_curr_emission_var<VAR_ATTACH_HI>(),
                                 node->get_curr_emission_var<VAR_ATTACH_LO>());
            break;
            
          default:
            assert(!"Both operands of the MUL instruction in x86 can't be constant values.");
            break;
          }
          break;
          
        case DAG_VAR_TEMP:
        case DAG_VAR_GLOBAL:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            emit_lir_code_helper(curr_bb,
                                 node,
                                 child_1.peek_first_usable_attached_var()->const_num(),
                                 child_0.peek_first_usable_attached_var(),
                                 node->get_curr_emission_var<VAR_ATTACH_HI>(),
                                 node->get_curr_emission_var<VAR_ATTACH_LO>());
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            emit_lir_code_helper(curr_bb,
                                 node,
                                 child_0.peek_first_usable_attached_var(),
                                 child_1.peek_first_usable_attached_var(),
                                 node->get_curr_emission_var<VAR_ATTACH_HI>(),
                                 node->get_curr_emission_var<VAR_ATTACH_LO>());
            break;
            
          default:
            emit_lir_code_helper(curr_bb,
                                 node,
                                 child_0.peek_first_usable_attached_var(),
                                 child_1.peek_first_usable_attached_var(),
                                 node->get_curr_emission_var<VAR_ATTACH_HI>(),
                                 node->get_curr_emission_var<VAR_ATTACH_LO>());
            break;
          }
          break;
          
        default:
          switch (child_1.node()->kind())
          {
          case DAG_VAR_CONST:
            assert(!"Both operands of the MUL instruction in x86 can't be constant values.");
            break;
            
          case DAG_VAR_TEMP:
          case DAG_VAR_GLOBAL:
            emit_lir_code_helper(curr_bb,
                                 node,
                                 child_1.peek_first_usable_attached_var(),
                                 child_0.peek_first_usable_attached_var(),
                                 node->get_curr_emission_var<VAR_ATTACH_HI>(),
                                 node->get_curr_emission_var<VAR_ATTACH_LO>());
            break;
            
          default:
            emit_lir_code_helper(curr_bb,
                                 node,
                                 child_0.peek_first_usable_attached_var(),
                                 child_1.peek_first_usable_attached_var(),
                                 node->get_curr_emission_var<VAR_ATTACH_HI>(),
                                 node->get_curr_emission_var<VAR_ATTACH_LO>());
            break;
          }
          break;
        }
      }
      
      node->get_curr_emission_var<VAR_ATTACH_HI>()->set_curr_dag_node(node);
      node->get_curr_emission_var<VAR_ATTACH_LO>()->set_curr_dag_node(node);
      
      node->jump_to_next_emission_var<VAR_ATTACH_HI>();
      node->jump_to_next_emission_var<VAR_ATTACH_LO>();
    }
  }
}

#endif

#endif
