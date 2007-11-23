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
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    gen_dt_code_helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> * const branch)
    {
      switch (is_link)
      {
      case LINK:
        switch (exchange_to_thumb)
        {
        case EXCHANGE:
          // BLX_1
          assert(!"Only exist on version 5 and above.");
          break;
          
        case NO_EXCHANGE:
          // BL
          //
          // :ARM-DDI-0100E: P.A4-10: Wei 2004-Nov-5:
          //
          // LR = address of the instruction after the branch instruction
          // PC = PC + (SignExtend(signed_immed_24) << 2)
          {
            Variable * const tmp1 = gp_chunk->get_new_temp();
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                                         gp_chunk->find_global_var(Variable::CR_PC),
                                         gp_chunk->find_const_var(4),
                                         tmp1));
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMov *>(0),
                                         tmp1,
                                         gp_chunk->find_global_var(Variable::CR_LR)));
            
            Variable * const tmp2 = gp_chunk->get_new_temp();
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                                         gp_chunk->find_global_var(Variable::CR_PC),
                                         gp_chunk->find_const_var(8),
                                         tmp2));
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                                         tmp2,
                                         gp_chunk->find_const_var(branch->Imm()),
                                         gp_chunk->find_global_var(Variable::CR_PC)));
          }
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
        
      case NO_LINK:
        switch (exchange_to_thumb)
        {
        case EXCHANGE:
          // :NOTE: Wei 2004-Sep-20:
          //
          // There is no 'imm-no_link-exchange' variants in Branch instructions.
          assert(!"Should not reach here.");
          break;
          
        case NO_EXCHANGE:
          // B
          //
          // :ARM-DDI-0100E: P.A4-10: Wei 2004-Nov-5:
          //
          // PC = PC + (SignExtend(signed_immed_24) << 2)
          {
            Variable * const tmp1 = gp_chunk->get_new_temp();
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                                                         gp_chunk->find_global_var(Variable::CR_PC),
                                                         gp_chunk->find_const_var(8),
                                                         tmp1));
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRAdd *>(0),
                                                         tmp1,
                                                         gp_chunk->find_const_var(branch->Imm()),
                                                         gp_chunk->find_global_var(Variable::CR_PC)));
          }
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    gen_dt_code_helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> * const /* branch */)
    {
      switch (is_link)
      {
      case LINK:
        switch (exchange_to_thumb)
        {
        case EXCHANGE:
          // BLX_2
          assert(!"Only exist on version 5 and above.");
          break;
          
        case NO_EXCHANGE:
          // :NOTE: Wei 2004-Sep-20:
          //
          // There is no 'reg-link-no_exchange' variants in Branch instructions.
          assert(!"Should not reach here.");
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
        
      case NO_LINK:
        switch (exchange_to_thumb)
        {
        case EXCHANGE:
          // BX
          assert(!"Only exist on version 5 and above, plus T variants of version 4.");
          break;
          
        case NO_EXCHANGE:
          // :NOTE: Wei 2004-Sep-20:
          //
          // There is no 'reg-no_link-no_exchange' variants in Branch instructions.
          assert(!"Should not reach here.");
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    check_jump_into_chunk_helper(InstBranch<ImmBranch,
                                 is_link,
                                 exchange_to_thumb,
                                 T_usage> * const branch,
                                 bool const /* is_last */)
    {
      int32_t inst_idx = gp_chunk->inst_idx() + branch->dest_offset();
      
      if ((inst_idx >= 0) && (inst_idx < static_cast<int32_t>(gp_chunk->length())))
      {
        // :NOTE: Wei 2004-Oct-03:
        //
        // Jump into the same chunk, need to check periphery.
        Label * const routine_label =
          gp_chunk->add_new_label(new Label(static_cast<LabelRoutine *>(0),
                                            reinterpret_cast<DTFunc_t>(check_periphery_DT)));
        
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRCall_0 *>(0), routine_label));
        
        Variable * const tmp1 = gp_chunk->get_new_temp();
        
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRRetrieve *>(0), 0, tmp1));
        
        Label * const near_label = gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
        
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRCmpEQ *>(0),
                                                     tmp1,
                                                     gp_chunk->find_const_var(ER_NORMAL_CONT_EXEC_IN_CHUNK),
                                                     near_label));
        
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0), tmp1));
        
        near_label->set_mir_inst(
          gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMov *>(0),
                                                       gp_chunk->find_const_var(0),
                                                       gp_chunk->find_global_var(Variable::DEVICE_TIMER_INCREMENT))));
        
        // :NOTE: Wei 2004-Nov-3:
        //
        // Jump to the target in the current chunk.
        Label * const label =
          gp_chunk->add_new_label(new Label(static_cast<LabelARMFirstInst *>(0),
                                            gp_chunk->peek_arm_inst_info(inst_idx)));
        
        label->need_to_check_cond_block();
        
        // :NOTE: Wei 2004-Oct-31:
        //
        // This label needs to be back-patched later.
        gp_chunk->add_pending_back_patch_label(label);
        
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0), label));
      }
      else
      {
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                     gp_chunk->find_const_var(ER_NORMAL_MODIFY_PC)));
      }
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    check_jump_into_chunk_helper(InstBranch<RegBranch,
                                 is_link,
                                 exchange_to_thumb,
                                 T_usage> * const /* branch */,
                                 bool const /* is_last */)
    {
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                           gp_chunk->find_const_var(ER_NORMAL_MODIFY_PC)));
    }
  }
  
  template<typename mode,
           IsLink is_link,
           ExchangeToThumb exchange_to_thumb,
           UsageEnum T_usage>
  inline void
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  gen_dt_code(Inst const inst, bool const is_last)
  {
    mInst = inst;
    
    gen_dt_code_helper(this);
    
    check_jump_into_chunk_helper(this, is_last);
  }
#endif
}
