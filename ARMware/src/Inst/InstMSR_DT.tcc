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
    template<UseWhichStatus use_which_status>
    inline void
    determine_dt_func_helper(InstMSR<ImmMSR, use_which_status> * const msr)
    {
      switch (use_which_status)
      {
      case USE_CPSR:
        msr->dt_func = reinterpret_cast<DTFunc_t>(exec_MSR_imm_cpsr_TC);
        break;
        
      case USE_CURRENT_SPSR:
        msr->dt_func = reinterpret_cast<DTFunc_t>(exec_MSR_imm_spsr_TC);
        break;
        
      default: assert(!"Should not reach here."); break;
      }
    }
    
    template<UseWhichStatus use_which_status>
    inline void
    determine_dt_func_helper(InstMSR<RegMSR, use_which_status> * const msr)
    {
      switch (use_which_status)
      {
      case USE_CPSR:
        msr->dt_func = reinterpret_cast<DTFunc_t>(exec_MSR_reg_cpsr_TC);
        break;
        
      case USE_CURRENT_SPSR:
        msr->dt_func = reinterpret_cast<DTFunc_t>(exec_MSR_reg_spsr_TC);
        break;
        
      default: assert(!"Should not reach here."); break;
      }
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline void
  InstMSR<mode, use_which_status>::
  gen_dt_code(Inst const inst, bool const is_last)
  {
    m_inst = inst;
    
    determine_dt_func_helper(this);
    
    Label * const routine_label =
      gp_chunk->add_new_label(new Label(static_cast<LabelRoutine *>(0), dt_func));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(
      new MIR(static_cast<MIRCall_1 *>(0),
              routine_label,
              gp_chunk->find_const_var(m_inst.mRawData)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRRetrieve *>(0), 1, 0));
    
    if (true == is_last)
    {
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                   gp_chunk->find_const_var(ER_NORMAL)));
    }
  }
#endif
}
