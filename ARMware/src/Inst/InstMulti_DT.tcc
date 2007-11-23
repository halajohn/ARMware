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
  template<IsLong is_long,
           IsSigned is_signed,
           IsAccumulate is_accumulate,
           IsUpdateStatus is_update_status>
  inline void
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  gen_dt_code(Inst const inst, bool const is_last)
  {
    mInst = inst;
    
    switch (is_long)
    {
    case NORMAL:
      switch (is_signed)
      {
      case UNSIGNED:
        // :NOTE: Wei 2004-Sep-20:
        //
        // There is NO 'normal-unsigned' variants in Multi instructions.
        assert(!"Should not reach here.");
        break;
        
      case SIGNED:
        switch (is_accumulate)
        {
        case NO_ACCUMULATE:
          // :ARM-DDI-0100E: p.A4-66: Wei 2004-Oct-30:
          //
          // Rd = (Rm * Rs)[31:0]
          //
          // :ARM-DDI-0100E: p.A4-67: Wei 2004-Oct-30:
          //
          // Specifying R15 for register <Rd>, <Rm>, or <Rs> has UNPREDICTABLE results.
          //
          // :ARM-DDI-0100E: p.A4-67: Wei 2005-Mar-14:
          //
          // Because the MUL instruction produces only the lower 32 bits of the 64-bit product,
          // MUL gives the same answer for multiplication of both signed and unsigned numbers.
          //
          // :NOTE: Wei 2005-Mar-14:
          //
          // Thus I use the x86 MUL instruction to accomplish the ARM MUL instruction rather than
          // the x86 IMUL instruction.
          {
            MIR * const mir =
              gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMul32 *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RsNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
            
            switch (is_update_status)
            {
            case NO_UPDATE_STATUS:
              break;
              
            case UPDATE_STATUS:
              // :ARM-DDI-0100E: p.A4-66: Wei 2004-Oct-30:
              //
              // if S == 1 then
              //   N Flag = Rd[31]
              //   Z Flag = if Rd == 0 then 1 else 0
              //   C Flag = unaffected /* See "C flag" note */
              //   V Flag = unaffected
              mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
              break;
              
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          break;
          
        case ACCUMULATE:
          {
            // :ARM-DDI-0100E: p.A4-54: Wei 2004-Oct-30:
            //
            // Rd = (Rm * Rs + Rn)[31:0]
            //
            // :ARM-DDI-0100E: p.A4-55: Wei 2004-Oct-30:
            //
            // Specifying R15 for register <Rd>, <Rm>, <Rs>, or <Rn> has UNPREDICTABLE results.
            Variable * const tmp_var = gp_chunk->get_new_temp();
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(
              new MIR(static_cast<MIRMul32 *>(0),
                      gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                      gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RsNum())),
                      tmp_var));
            
            MIR * const mir =
              gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRAdd *>(0),
                        tmp_var,
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RnNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
            
            switch (is_update_status)
            {
            case NO_UPDATE_STATUS:
              break;
              
            case UPDATE_STATUS:
              // :ARM-DDI-0100E: p.A4-54: Wei 2004-Oct-30:
              //
              // if S == 1 then
              //   N Flag = Rd[31]
              //   Z Flag = if Rd == 0 then 1 else 0
              //   C Flag = unaffected /* See "C flag" note */
              //   V Flag = unaffected
              mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
              break;
              
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      default: assert(!"Should not reach here."); break;
      }
      break;
      
    case LONG:
      switch (is_signed)
      {
      case UNSIGNED:
        switch (is_accumulate)
        {
        case NO_ACCUMULATE:
          {
            // :ARM-DDI-0100E: p.A4-112: Wei 2004-Oct-31:
            //
            // RdHi = (Rm * Rs)[63:32] /* Unsigned multiplication */
            // RdLo = (Rm * Rs)[31:0]
            //
            // :ARM-DDI-0100E: p.A4-112: Wei 2004-Oct-31:
            //
            // Specifying R15 for register <RdHi>, <RdLo>, <Rm>, or <Rs> has UNPREDICTABLE results.
            MIR * const mir =
              gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRUMul64 *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RsNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdHiNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdLoNum()))));
            
            switch (is_update_status)
            {
            case NO_UPDATE_STATUS:
              break;
              
            case UPDATE_STATUS:
              // :ARM-DDI-0100E: p.A4-112: Wei 2004-Oct-31:
              //
              // if S == 1 then
              //   N Flag = RdHi[31]
              //   Z Flag = if (RdHi == 0) and (RdLo == 0) then 1 else 0
              //   C Flag = unaffected /* See "C and V flags" note */
              //   V Flag = unaffected /* See "C and V flags" note */
              mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
              break;
              
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          break;
          
        case ACCUMULATE:
          {
            // :ARM-DDI-0100E: p.A4-110: Wei 2004-Oct-31:
            //
            // RdLo = (Rm * Rs)[31:0] + RdLo /* Unsigned multiplication */
            // RdHi = (Rm * Rs)[63:32] + RdHi + CarryFrom((Rm * Rs)[31:0] + RdLo)
            //
            // :ARM-DDI-0100E: p.A4-110: Wei 2004-Oct-31:
            //
            // Specifying R15 for register <RdHi>, <RdLo>, <Rm>, or <Rs> has UNPREDICTABLE results.
            Variable * const hi_tmp = gp_chunk->get_new_temp();
            Variable * const lo_tmp = gp_chunk->get_new_temp();
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRUMul64 *>(0),
                                         gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                                         gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RsNum())),
                                         hi_tmp,
                                         lo_tmp));
            
            MIR * const mir =
              gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRAdd64 *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdHiNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdLoNum())),
                        hi_tmp,
                        lo_tmp,
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdHiNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdLoNum()))));
            
            switch (is_update_status)
            {
            case NO_UPDATE_STATUS:
              break;
              
            case UPDATE_STATUS:
              // :ARM-DDI-0100E: p.A4-110: Wei 2004-Oct-31:
              //
              // if S == 1 then
              //   N Flag = RdHi[31]
              //   Z Flag = if (RdHi == 0) and (RdLo == 0) then 1 else 0
              //   C Flag = unaffected /* See "C and V flags" note */
              //   V Flag = unaffected /* See "C and V flags" note */
              mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
              break;
              
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      case SIGNED:
        switch (is_accumulate)
        {
        case NO_ACCUMULATE:
          {
            // :ARM-DDI-0100E: p.A4-81: Wei 2004-Oct-31:
            //
            // RdHi = (Rm * Rs)[63:32] /* Signed multiplication */
            // RdLo = (Rm * Rs)[31:0]
            //
            // :ARM-DDI-0100E: p.A4-81: Wei 2004-Oct-31:
            //
            // Specifying R15 for register <RdHi>, <RdLo>, <Rm>, or <Rs> has UNPREDICTABLE results.
            MIR * const mir =
              gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRSMul64 *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RsNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdHiNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdLoNum()))));
            
            switch (is_update_status)
            {
            case NO_UPDATE_STATUS:
              break;
              
            case UPDATE_STATUS:
              // :ARM-DDI-0100E: p.A4-81: Wei 2004-Oct-31:
              //
              // if S == 1 then
              //   N Flag = RdHi[31]
              //   Z Flag = if (RdHi == 0) and (RdLo == 0) then 1 else 0
              //   C Flag = unaffected /* See "C and V flags" note */
              //   V Flag = unaffected /* See "C and V flags" note */
              mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
              break;
              
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          break;
          
        case ACCUMULATE:
          {
            // :ARM-DDI-0100E: p.A4-79: Wei 2004-Oct-31:
            //
            // RdLo = (Rm * Rs)[31:0] + RdLo /* Signed multiplication */
            // RdHi = (Rm * Rs)[63:32] + RdHi + CarryFrom((Rm * Rs)[31:0] + RdLo)
            //
            // :ARM-DDI-0100E: p.A4-79: Wei 2004-Oct-31:
            //
            // Specifying R15 for register <RdHi>, <RdLo>, <Rm>, or <Rs> has UNPREDICTABLE results.
            Variable * const hi_tmp = gp_chunk->get_new_temp();
            Variable * const lo_tmp = gp_chunk->get_new_temp();
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(
              new MIR(static_cast<MIRSMul64 *>(0),
                      gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                      gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RsNum())),
                      hi_tmp,
                      lo_tmp));
            
            MIR * const mir =
              gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRAdd64 *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdHiNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdLoNum())),
                        hi_tmp,
                        lo_tmp,
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdHiNum())),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdLoNum()))));
            
            switch (is_update_status)
            {
            case NO_UPDATE_STATUS:
              break;
              
            case UPDATE_STATUS:
              // :ARM-DDI-0100E: p.A4-79: Wei 2004-Oct-31:
              //
              // if S == 1 then
              //   N Flag = RdHi[31]
              //   Z Flag = if (RdHi == 0) and (RdLo == 0) then 1 else 0
              //   C Flag = unaffected /* See "C and V flags" note */
              //   V Flag = unaffected /* See "C and V flags" note */
              mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
              break;
              
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      default: assert(!"Should not reach here."); break;
      }
      break;
      
    default: assert(!"Should not reach here."); break;
    }
    
    gp_chunk->incre_PC_DT();
    
    if (true == is_last)
    {
      // :NOTE: Wei 2004-Oct-05:
      //
      // We have finish the translation to MIR.
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                   gp_chunk->find_const_var(ER_NORMAL)));
    }
  }
#endif
}
