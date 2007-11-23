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

#ifndef Pattern_DAGAdd64_hpp
#define Pattern_DAGAdd64_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  namespace Pattern_DAGAdd64
  {
    inline void
    emit_lir_code(BasicBlock * const curr_bb, DAGNode * const node)
    {
      assert(DAG_ADD64 == node->kind());
      
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
        
        Variable *var[4];
        
        uint32_t i = 0;
        
        for (std::deque<ChildInfo>::const_iterator iter = children.begin();
             iter != children.end();
             ++iter, ++i)
        {
          var[i] = (*iter).peek_first_usable_attached_var();
        }
        
        switch (node->out_flags())
        {
        case 0:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRAdd *>(0),
                                            var[0],
                                            var[2],
                                            node->get_curr_emission_var<VAR_ATTACH_HI>()));
          
          curr_bb->append_lir<true>(new LIR(static_cast<LIRAdc *>(0),
                                            var[1],
                                            var[3],
                                            node->get_curr_emission_var<VAR_ATTACH_LO>()));
          break;
          
        case SIGN_FLAG:
          curr_bb->append_lir<true>(new LIR(static_cast<LIRAdd *>(0),
                                            var[0],
                                            var[2],
                                            node->get_curr_emission_var<VAR_ATTACH_HI>()));
          
          curr_bb->append_lir<true>(new LIR(static_cast<LIRAdc *>(0),
                                            var[1],
                                            var[3],
                                            node->get_curr_emission_var<VAR_ATTACH_LO>()));
          
          store_native_flags(curr_bb, SIGN_FLAG);
          break;
          
        case ZERO_FLAG:
          {
            curr_bb->append_lir<true>(
              new LIR(static_cast<LIRAdd *>(0),
                      var[0],
                      var[2],
                      node->get_curr_emission_var<VAR_ATTACH_HI>())
              )->set_cond_flags_involved();
            
            store_native_flags(curr_bb, ZERO_FLAG);
            load_native_flags(curr_bb, ZERO_FLAG);
            
            Label * const label = gp_chunk->add_new_label(new Label(static_cast<LabelBB *>(0), 0));
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRJnz *>(0), label));
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRAdc *>(0),
                                              var[1],
                                              var[3],
                                              node->get_curr_emission_var<VAR_ATTACH_LO>()));
            
            store_native_flags(curr_bb, ZERO_FLAG);
            
            Label * const label_2 = gp_chunk->add_new_label(new Label(static_cast<LabelBB *>(0), 0));
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRJmp *>(0), label_2));
            
            curr_bb->back_patch_bb_label_use_next_lir(label);
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRAdc *>(0),
                                              var[1],
                                              var[3],
                                              node->get_curr_emission_var<VAR_ATTACH_LO>()));
            
            curr_bb->back_patch_bb_label_use_next_lir(label_2);
          }
          break;
          
        case SIGN_FLAG | ZERO_FLAG:
          {
            curr_bb->append_lir<true>(
              new LIR(static_cast<LIRAdd *>(0),
                      var[0],
                      var[2],
                      node->get_curr_emission_var<VAR_ATTACH_HI>())
              )->set_cond_flags_involved();
            
            store_native_flags(curr_bb, ZERO_FLAG);
            load_native_flags(curr_bb, ZERO_FLAG);
            
            Label * const label = gp_chunk->add_new_label(new Label(static_cast<LabelBB *>(0), 0));
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRJnz *>(0), label));
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRAdc *>(0),
                                              var[1],
                                              var[3],
                                              node->get_curr_emission_var<VAR_ATTACH_LO>()));
            
            store_native_flags(curr_bb, SIGN_FLAG | ZERO_FLAG);
            
            Label * const label_2 = gp_chunk->add_new_label(new Label(static_cast<LabelBB *>(0), 0));
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRJmp *>(0), label_2));
            
            curr_bb->back_patch_bb_label_use_next_lir(label);
            
            curr_bb->append_lir<true>(new LIR(static_cast<LIRAdc *>(0),
                                              var[1],
                                              var[3],
                                              node->get_curr_emission_var<VAR_ATTACH_LO>()));
            
            store_native_flags(curr_bb, SIGN_FLAG);
            
            curr_bb->back_patch_bb_label_use_next_lir(label_2);
          }
          break;
          
        default:
          assert(!"Should not reach here.");
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
