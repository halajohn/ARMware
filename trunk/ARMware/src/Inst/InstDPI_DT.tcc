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
  /////////////////////////////////////////////////////////////////////////////
  // generate dt code helper
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline Variable *
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  gen_shifter_operand(Label ** const after_switch_label)
  {
    switch (mode::ADDR_MODE)
    {
    case IMM:
      {
        // :ARM-DDI-0100E: p.A5-6: Wei 2004-Oct-26:
        //
        // shifter_operand = immed_8 Rotate_Right (rotate_imm * 2)
        uint32_t const shifter_operand = RotateRight(Imm(), RotImm() << 1);
        
        if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
        {
          if ((UPDATE_STATUS == is_update_status) &&
              (RdNum() != CR_PC))
          {
            // :ARM-DDI-0100E: p.A5-6: Wei 2004-Oct-26:
            //
            // if rotate_imm == 0 then
            //   shifter_carry_out = C flag
            if (0 == RotImm())
            {
              // :NOTE: Wei 2004-Oct-28:
              //
              // Because the shifter carry is used to reserve the value of the C flag,
              // thus I can instead modify the C flag directly.
              // However, the operation 'shifter_carry_out = C flag' implies
              // 'C flag = C flag', thus I don't need to do anything here.
            }
            else
            {
              // :ARM-DDI-0100E: p.A5-6: Wei 2004-Oct-26:
              //
              // else /* rotate_imm != 0 */
              //   shifter_carry_out = shifter_operand[31]
              
              // :NOTE: Wei 2004-Oct-30:
              //
              // Rotate right 0 bit implies rotate right 32 bits in the ARM platform,
              // and the shifter carry out should be the lastest bit shifted out.
              // However, not all platforms have shifting more than 32 bits,
              // for example, IA32 doesn't have this behavior, thus I have to create
              // a new MIR instruction which should produce the specified bit as the
              // C flag on the target platform.
              MIR * const mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRProduceCBit *>(0),
                        gp_chunk->find_const_var(shifter_operand),
                        gp_chunk->find_const_var(31)));
              
              mir->set_in_out_flags(0, CARRY_FLAG);
            }
          }
        }
        
        return gp_chunk->find_const_var(shifter_operand);
      }
      
    case IMM_SFT:
      {
        Variable * const shifter_operand = gp_chunk->get_new_temp();
        Variable *var_rm;
        
        if (CR_PC == RmNum())
        {
          var_rm = gp_chunk->get_new_temp();
          
          gp_chunk->append_mir<MIR_FOR_NORMAL>(
            new MIR(static_cast<MIRAdd *>(0),
                    gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(CR_PC)),
                    gp_chunk->find_const_var(8),
                    var_rm));
        }
        else
        {
          var_rm = gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum()));
        }
        
        switch (WhichSftType())
        {
        case LSL:
          // :ARM-DDI-0100E: p.A5-9: Wei 2004-Oct-27:
          //
          // if shift_imm == 0 then /* Register Operand */
          if (0 == SftImm())
          {
            // :ARM-DDI-0100E: p.A5-9: Wei 2004-Oct-27:
            //
            // shifter_operand = Rm
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMov *>(0),
                                                         var_rm,
                                                         shifter_operand));
            
            // :ARM-DDI-0100E: p.A5-9: Wei 2004-Oct-27:
            //
            // shifter_carry_out = C Flag
          }
          // :ARM-DDI-0100E: p.A5-9: Wei 2004-Oct-27:
          //
          // else /* shift_imm > 0 */
          else
          {
            // :ARM-DDI-0100E: p.A5-9: Wei 2004-Oct-27:
            //
            // shifter_operand = Rm Logical_Shift_Left shift_imm
            MIR * const shift_mir = new MIR(static_cast<MIRLSftLeft *>(0),
                                            var_rm,
                                            gp_chunk->find_const_var(SftImm()),
                                            shifter_operand);
            
            // :ARM-DDI-0100E: p.A5-9: Wei 2004-Oct-27:
            //
            // shifter_carry_out = Rm[32 - shift_imm]
            //
            // :NOTE: Wei 2004-Oct-28:
            //
            // That is to say, for each shift operation, the most signification bit
            // of the destination operand is shifted into the C flag.
            if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
            {
              if ((UPDATE_STATUS == is_update_status) &&
                  (RdNum() != CR_PC))
              {
                shift_mir->set_in_out_flags(0, CARRY_FLAG);
              }
            }
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(shift_mir);
          }
          break;
          
        case LSR:
          // :ARM-DDI-0100E: p.A5-11: Wei 2004-Oct-27:
          //
          // if shift_imm == 0 then 
          if (0 == SftImm())
          {
            // :ARM-DDI-0100E: p.A5-11: Wei 2004-Oct-27:
            //
            // shifter_operand = 0
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMov *>(0),
                                         gp_chunk->find_const_var(0),
                                         shifter_operand));
            
            if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
            {
              // :ARM-DDI-0100E: p.A5-11: Wei 2004-Oct-27:
              //
              // shifter_carry_out = Rm[31]
              if ((UPDATE_STATUS == is_update_status) &&
                  (RdNum() != CR_PC))
              {
                MIR * const mir = new MIR(static_cast<MIRProduceCBit *>(0),
                                          var_rm,
                                          gp_chunk->find_const_var(31));
                
                mir->set_in_out_flags(0, CARRY_FLAG);
                
                gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
              }
            }
          }
          // :ARM-DDI-0100E: p.A5-11: Wei 2004-Oct-27:
          //
          // else /* shift_imm > 0 */
          else
          {
            // :ARM-DDI-0100E: p.A5-11: Wei 2004-Oct-27:
            //
            // shifter_operand = Rm Logical_Shift_Right shift_imm
            MIR * const mir = new MIR(static_cast<MIRLSftRight *>(0),
                                      var_rm,
                                      gp_chunk->find_const_var(SftImm()),
                                      shifter_operand);
            
            if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
            {
              // :ARM-DDI-0100E: p.A5-11: Wei 2004-Oct-27:
              //
              // shifter_carry_out = Rm[shift_imm - 1]
              if ((UPDATE_STATUS == is_update_status) &&
                  (RdNum() != CR_PC))
              {
                mir->set_in_out_flags(0, CARRY_FLAG);
              }
            }
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
          }
          break;
          
        case ASR:
          // :ARM-DDI-0100E: p.A5-13: Wei 2004-Oct-27:
          //
          // if shift_imm == 0 then
          if (0 == SftImm())
          {
            // :ARM-DDI-0100E: p.A5-13: Wei 2004-Oct-27:
            //
            // if Rm[31] == 0 then
            //    shifter_operand = 0
            //    shifter_carry_out = Rm[31]
            // else /* Rm[31] == 1 */
            //    shifter_operand = 0xFFFFFFFF
            //    shifter_carry_out = Rm[31]
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRASftRight *>(0),
                                                         var_rm,
                                                         gp_chunk->find_const_var(31),
                                                         shifter_operand));
            
            if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
            {
              if ((UPDATE_STATUS == is_update_status) &&
                  (RdNum() != CR_PC))
              {
                MIR * const mir = new MIR(static_cast<MIRProduceCBit *>(0),
                                          var_rm,
                                          gp_chunk->find_const_var(31));
                
                mir->set_in_out_flags(0, CARRY_FLAG);
                
                gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
              }
            }
          }
          // :ARM-DDI-0100E: p.A5-13: Wei 2004-Oct-27:
          //
          // else /* shift_imm > 0 */
          else
          {
            // :ARM-DDI-0100E: p.A5-13: Wei 2004-Oct-27:
            //
            // shifter_operand = Rm Arithmetic_Shift_Right <shift_imm>
            MIR * const mir = new MIR(static_cast<MIRASftRight *>(0),
                                      var_rm,
                                      gp_chunk->find_const_var(SftImm()),
                                      shifter_operand);
            
            if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
            {
              if ((UPDATE_STATUS == is_update_status) &&
                  (RdNum() != CR_PC))
              {
                // :ARM-DDI-0100E: p.A5-13: Wei 2004-Oct-27:
                //
                // shifter_carry_out = Rm[shift_imm - 1]
                mir->set_in_out_flags(0, CARRY_FLAG);
              }
            }
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
          }
          break;
        
        case ROR:
          // :ARM-DDI-0100E: p.A5-15: Wei 2004-Oct-27:
          //
          // if shift_imm == 0 then See 'Data-processing operands - Rotate right with extend' on
          // ARM-DDI-0100E, page A5-17
          if (0 == SftImm())
          {
            // :ARM-DDI-0100E: p.A5-17: Wei 2004-Oct-27:
            //
            // shifter_operand = (C Flag Logical_Shift_Left 31) OR (Rm Logical_Shift_Right 1)
            MIR * const mir = new MIR(static_cast<MIRRrx *>(0),
                                      var_rm,
                                      gp_chunk->find_const_var(1),
                                      shifter_operand);
            
            if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
            {
              if ((UPDATE_STATUS == is_update_status) &&
                  (RdNum() != CR_PC))
              {
                // :ARM-DDI-0100E: p.A5-17: Wei 2004-Oct-27:
                //
                // shifter_carry_out = Rm[0]
                mir->set_in_out_flags(CARRY_FLAG, CARRY_FLAG);
              }
            }
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
          }
          // :ARM-DDI-0100E: p.A5-15: Wei 2004-Oct-27:
          //
          // else /* shift_imm > 0 */
          else
          {
            // :ARM-DDI-0100E: p.A5-15: Wei 2004-Oct-27:
            //
            // shifter_operand = Rm Rotate_Right shift_imm
            MIR * const mir = new MIR(static_cast<MIRRor *>(0),
                                      var_rm,
                                      gp_chunk->find_const_var(SftImm()),
                                      shifter_operand);
            
            if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
            {
              if ((UPDATE_STATUS == is_update_status) &&
                  (RdNum() != CR_PC))
              {
                // :ARM-DDI-0100E: p.A5-15: Wei 2004-Oct-27:
                //
                // shifter_carry_out = Rm[shift_imm - 1]
                mir->set_in_out_flags(0, CARRY_FLAG);
              }
            }
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
          }
          break;
          
        case RRX:
        default:
          assert(!"Should not reach here.");
          break;
        }
      
        return shifter_operand;
      }
      break;
      
    case REG_SFT:
      {
        Variable * const shifter_operand = gp_chunk->get_new_temp(true);
        Variable * const var_rs_tmp = gp_chunk->get_new_temp(true);
        
        // :ARM-DDI-0100E: p.A5-10: Wei 2003-Nov-1:
        //
        // ... shift by the value in the least significant byte of register Rs.
        gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIRAnd *>(0),
                  gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RsNum())),
                  gp_chunk->find_const_var(0xFF),
                  var_rs_tmp));
        
        Label * const table_base_label =
          gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
        
        MIR * const jmp_mir = gp_chunk->insert_switch_statement_DT(var_rs_tmp, table_base_label);
        
        Label *label_0;
        Label *label_1_31;
        Label *label_32;
        Label *label_32_64_96_128_160_192_224;
        Label *label_other;
        
        Label * const after_switch_label_local =
          gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
        
        *after_switch_label = after_switch_label_local;
        
        // :NOTE: Wei 2004-Oct-30:
        //
        /// Jump table.
        switch (WhichSftType())
        {
        case LSL:
        case LSR:
          {
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 0:
            label_0 = gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
            
            table_base_label->set_mir_inst(
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0), label_0)));
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 1 ... 31:
            label_1_31 = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            for (uint32_t i = 1; i <= 31; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_1_31));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 32:
            label_32 = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32));
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 33 ... 255:
            label_other = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            for (uint32_t i = 33; i <= 255; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // Avoid the compiler's 'might be used uninitialized in this function' warning.
            label_32_64_96_128_160_192_224 = 0;
            
            jmp_mir->add_jump_target_label(label_0);
            jmp_mir->add_jump_target_label(label_1_31);
            jmp_mir->add_jump_target_label(label_32);
            jmp_mir->add_jump_target_label(label_other);
          }
          break;
          
        case ASR:
          {
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 0:
            label_0 = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            table_base_label->set_mir_inst(
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0), label_0)));
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 1 ... 31:
            label_1_31 = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            for (uint32_t i = 1; i <= 31; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_1_31));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 32 ... 255:
            label_other = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            for (uint32_t i = 32; i <= 255; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // Avoid the compiler's 'might be used uninitialized in this function' warning.
            label_32 = 0;
            label_32_64_96_128_160_192_224 = 0;
            
            jmp_mir->add_jump_target_label(label_0);
            jmp_mir->add_jump_target_label(label_1_31);
            jmp_mir->add_jump_target_label(label_other);
          }
          break;
          
        case ROR:
          {
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 0:
            label_0 = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            table_base_label->set_mir_inst(
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0), label_0)));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 1 ... 31:
            label_other = gp_chunk->add_new_label(
              new Label(static_cast<LabelMIRInst *>(0)));
            
            for (uint32_t i = 1; i <= 31 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 32:
            label_32_64_96_128_160_192_224 =
              gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
            
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32_64_96_128_160_192_224));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 33 ... 63:
            for (uint32_t i = 33; i <= 63 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 64:
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32_64_96_128_160_192_224));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 65 ... 95:
            for (uint32_t i = 65; i <= 95 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 96:
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32_64_96_128_160_192_224));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 97 ... 127:
            for (uint32_t i = 97; i <= 127 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 128:
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32_64_96_128_160_192_224));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 129 ... 159:
            for (uint32_t i = 129; i <= 159 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 160:
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32_64_96_128_160_192_224));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 161 ... 191:
            for (uint32_t i = 161; i <= 191 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 192:
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32_64_96_128_160_192_224));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 193 ... 223:
            for (uint32_t i = 193; i <= 223 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Oct-30:
            //
            // case 224:
            gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                         label_32_64_96_128_160_192_224));
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // case 225 ... 255:
            for (uint32_t i = 225; i <= 255 ; ++i)
            {
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                           label_other));
            }
            
            // :NOTE: Wei 2004-Nov-6:
            //
            // Avoid the compiler's 'might be used uninitialized in this function' warning.
            label_1_31 = 0;
            label_32 = 0;
            
            jmp_mir->add_jump_target_label(label_0);
            jmp_mir->add_jump_target_label(label_32_64_96_128_160_192_224);
            jmp_mir->add_jump_target_label(label_other);
          }
          break;
          
        default:
          assert(!"Should not reach here.");
          label_0 = 0;
          label_1_31 = 0;
          label_32 = 0;
          label_32_64_96_128_160_192_224 = 0;
          label_other = 0;
          break;
        }
        
        // :NOTE: Wei 2004-Nov-5:
        //
        // End of the basic block for the jump table.
        gp_chunk->next_mir_is_leader();
        
        switch (WhichSftType())
        {
        case LSL:
          // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-28:
          //
          // Specifying R15 as register Rd, Rm, Rn, or Rs has UNPREDICTABLE results.
          {
            // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
            //
            // Rs[7:0] == 0
            //   shifter_operand = Rm
            {
              label_0->set_mir_inst(gp_chunk->append_mir<MIR_FOR_NORMAL>(
                                      new MIR(static_cast<MIRMov *>(0),
                                              gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                                              shifter_operand)));
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if (UPDATE_STATUS == is_update_status)
                {
                  // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
                  //
                  // shifter_carry_out = C Flag
                }
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
            
            // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
            //
            // Rs[7:0] == 1 ... 31
            //   shifter_operand = Rm Logical_Shift_Left Rs[7:0]
            {
              MIR * const case_1_31_mir =
                gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRLSftLeft *>(0),
                                             gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                                             var_rs_tmp,
                                             shifter_operand));
              
              label_1_31->set_mir_inst(case_1_31_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
                  //
                  // shifter_carry_out = Rm[32 - Rs[7:0]]
                  case_1_31_mir->set_in_out_flags(0, CARRY_FLAG);
                }
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
            
            // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
            //
            // Rs[7:0] == 32
            //   shifter_operand = 0
            {
              MIR * const case_32_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_const_var(0),
                        shifter_operand));
              
              label_32->set_mir_inst(case_32_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
                  //
                  // shifter_carry_out = Rm[0]
                  MIR * const mir =
                    new MIR(static_cast<MIRProduceCBit *>(0),
                            gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                            gp_chunk->find_const_var(0));
                  
                  mir->set_in_out_flags(0, CARRY_FLAG);
                  
                  gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
                }
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
          
            // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
            //
            // else /* Rs[7:0] > 32 */
            //   shifter_operand = 0
            {
              MIR * const case_other_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_const_var(0),
                        shifter_operand));
              
              label_other->set_mir_inst(case_other_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  // :ARM-DDI-0100E: p.A5-10: Wei 2004-Oct-29:
                  //
                  // shifter_carry_out = 0
                  MIR * const mir = new MIR(static_cast<MIRClearCBit *>(0));
                  
                  mir->set_in_out_flags(0, CARRY_FLAG);
                  
                  gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
                }
              }
              
              // :NOTE: Wei 2004-Nov-6:
              //
              // Becasue this is the last one in this jump table,
              // thus I can avoid the MIRGoto here, and let the control flow
              // falling throw.
              gp_chunk->next_mir_is_leader();
            }
          }
          break;
          
        case LSR:
          {          
            // :ARM-DDI-0100E: p.A5-12: Wei 2004-Oct-30:
            //
            // Rs[7:0] == 0
            //   shifter_operand = Rm
            {
              MIR * const case_0_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        shifter_operand));
              
              label_0->set_mir_inst(case_0_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if (UPDATE_STATUS == is_update_status)
                {
                  // :ARM-DDI-0100E: p.A5-12: Wei 2004-Oct-30:
                  //
                  // shifter_carry_out = C Flag
                }
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
            
            // :ARM-DDI-0100E: p.A5-12: Wei 2004-Oct-30:
            //
            // Rs[7:0] == 1 ... 31
            //   shifter_operand = Rm Logical_Shift_Right Rs[7:0]
            {
              MIR * const case_1_31_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRLSftRight *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        var_rs_tmp,
                        shifter_operand));
              
              label_1_31->set_mir_inst(case_1_31_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  // :ARM-DDI-0100E: p.A5-12: Wei 2004-Oct-30:
                  //
                  // shifter_carry_out = Rm[Rs[7:0] - 1]
                  case_1_31_mir->set_in_out_flags(0, CARRY_FLAG);
                }
              }
            
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
          
            // :ARM-DDI-0100E: p.A5-12: Wei 2004-Oct-30:
            //
            // Rs[7:0] == 32
            //   shifter_operand = 0
            {
              MIR * const case_32_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_const_var(0),
                        shifter_operand));
              
              label_32->set_mir_inst(case_32_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  // :ARM-DDI-0100E: p.A5-12: Wei 2004-Oct-30:
                  //
                  // shifter_carry_out = Rm[31]
                  MIR * const mir =
                    new MIR(static_cast<MIRProduceCBit *>(0),
                            gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                            gp_chunk->find_const_var(31));
                  
                  mir->set_in_out_flags(0, CARRY_FLAG);
                  
                  gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
                }
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
          
            // :NOTE: Wei 2004-Oct-26:
            //
            // else /* Rs[7:0] > 32 */
            //   shifter_operand = 0
            {
              MIR * const case_other_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_const_var(0),
                        shifter_operand));
              
              label_other->set_mir_inst(case_other_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  // :ARM-DDI-0100E: p.A5-12: Wei 2004-Oct-29:
                  //
                  // shifter_carry_out = 0
                  MIR * const mir = new MIR(static_cast<MIRClearCBit *>(0));
                  
                  mir->set_in_out_flags(0, CARRY_FLAG);
                  
                  gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
                }
              }
              
              // :NOTE: Wei 2004-Nov-6:
              //
              // Becasue this is the last one in this jump table,
              // thus I can avoid the MIRGoto here, and let the control flow
              // falling throw.
              gp_chunk->next_mir_is_leader();
            }
          }
          break;
        
        case ASR:
          {
            // :ARM-DDI-0100E: p.A5-14: Wei 2004-Oct-30:
            //
            // Rs[7:0] == 0
            //   shifter_operand = Rm
            {
              MIR * const case_0_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        shifter_operand));
              
              label_0->set_mir_inst(case_0_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                // :ARM-DDI-0100E: p.A5-14: Wei 2004-Oct-30:
                //
                // shifter_carry_out = C Flag
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
            
            // :ARM-DDI-0100E: p.A5-14: Wei 2004-Oct-30:
            //
            // Rs[7:0] == 1 ... 31
            //   shifter_operand = Rm Arithmetic_Shift_Right Rs[7:0]
            {
              MIR * const case_1_31_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRASftRight *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        var_rs_tmp,
                        shifter_operand));
              
              label_1_31->set_mir_inst(case_1_31_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                // :ARM-DDI-0100E: p.A5-14: Wei 2004-Oct-30:
                //
                // shifter_carry_out = Rm[Rs[7:0] - 1]
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  case_1_31_mir->set_in_out_flags(0, CARRY_FLAG);
                }
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
            
            // :ARM-DDI-0100E: p.A5-14: Wei 2004-Oct-30:
            //
            // else /* Rs[7:0] >= 32 */
            //   if Rm[31] == 0 then
            //      shifter_operand = 0
            //      shifter_carry_out = Rm[31]
            //   else /* Rm[31] == 1 */
            //      shifter_operand = 0xFFFFFFFF
            //      shifter_carry_out = Rm[31]
            {
              MIR * const case_32_255_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRASftRight *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        gp_chunk->find_const_var(31),
                        shifter_operand));
              
              label_other->set_mir_inst(case_32_255_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                // :ARM-DDI-0100E: p.A5-14: Wei 2004-Oct-30:
                //
                // shifter_carry_out = Rm[31]
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  MIR * const mir =
                    new MIR(static_cast<MIRProduceCBit *>(0),
                            gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                            gp_chunk->find_const_var(31));
                  
                  mir->set_in_out_flags(0, CARRY_FLAG);
                  
                  gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
                }
              }
              
              // :NOTE: Wei 2004-Nov-6:
              //
              // Becasue this is the last one in this jump table,
              // thus I can avoid the MIRGoto here, and let the control flow
              // falling throw.
              gp_chunk->next_mir_is_leader();
            }
          }
          break;
          
        case ROR:
          {
            // :ARM-DDI-0100E: p.A5-16: Wei 2004-Oct-30:
            //
            // Rs[7:0] == 0
            //   shifter_operand = Rm
            {
              MIR * const case_0_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        shifter_operand));
              
              label_0->set_mir_inst(case_0_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                // :ARM-DDI-0100E: p.A5-16: Wei 2004-Oct-30:
                //
                // shifter_carry_out = C Flag
              }
            
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
            
            // :ARM-DDI-0100E: p.A5-16: Wei 2004-Oct-30:
            //
            // Rs[7:0] == 32, 64, 96, 128, 160, 192, 224
            //   shifter_operand = Rm
            {
              MIR * const case_32_64_96_128_160_192_224_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRMov *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        shifter_operand));
              
              label_32_64_96_128_160_192_224->set_mir_inst(
                case_32_64_96_128_160_192_224_mir);
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                // :ARM-DDI-0100E: p.A5-16: Wei 2004-Oct-30:
                //
                // shifter_carry_out = Rm[31]
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  MIR * const mir =
                    new MIR(static_cast<MIRProduceCBit *>(0),
                            gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                            gp_chunk->find_const_var(31));
                  
                  mir->set_in_out_flags(0, CARRY_FLAG);
                  
                  gp_chunk->append_mir<MIR_FOR_NORMAL>(mir);
                }
              }
              
              gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRGoto *>(0),
                                                           after_switch_label_local));
            }
            
            // :ARM-DDI-0100E: p.A5-16: Wei 2004-Oct-30:
            //
            // else /* Rs[4:0] > 0 */
            //   shifter_operand = Rm Rotate_Right Rs[4:0]
            {
              Variable * const var_rs_tmp2 = gp_chunk->get_new_temp();
              
              label_other->set_mir_inst(gp_chunk->append_mir<MIR_FOR_NORMAL>(
                                          new MIR(static_cast<MIRAnd *>(0),
                                                  var_rs_tmp,
                                                  gp_chunk->find_const_var(0x1F),
                                                  var_rs_tmp2)));
              
              MIR * const case_other_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
                new MIR(static_cast<MIRRor *>(0),
                        gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RmNum())),
                        var_rs_tmp2,
                        shifter_operand));
              
              if (true == opcode::SHIFTER_SHELL_PRODUCE_C_BIT)
              {
                // :ARM-DDI-0100E: p.A5-16: Wei 2004-Oct-30:
                //
                // shifter_carry_out = Rm[Rs[4:0] - 1]
                if ((UPDATE_STATUS == is_update_status) &&
                    (RdNum() != CR_PC))
                {
                  case_other_mir->set_in_out_flags(0, CARRY_FLAG);
                }
              }
              
              // :NOTE: Wei 2004-Nov-6:
              //
              // Becasue this is the last one in this jump table,
              // thus I can avoid the MIRGoto here, and let the control flow
              // falling throw.
              gp_chunk->next_mir_is_leader();
            }
          }
          break;
          
        case RRX:
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        return shifter_operand;
      }
      break;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
    
    assert(!"Should not reach here.");
    return 0;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // generate dt code
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline MIR *
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  produce_main_mir(Variable * const rn_var,
                   Variable * const sft_operand)
  {
    MIR *first_main_mir;
    
    switch (opcode::OPCODE)
    {
    case DPI_OP_AND:
      // :ARM-DDI-0100E: p.A4-9: Wei 2004-Oct-26:
      //
      // Rd = Rn AND shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRAnd *>(0),
                rn_var,
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
      }
      break;
      
    case DPI_OP_EOR:
      // :ARM-DDI-0100E: p.A4-27: Wei 2004-Oct-26:
      //
      // Rd = Rn EOR shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIREor *>(0),
                rn_var,
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
      }
      break;
      
    case DPI_OP_SUB:
      // :ARM-DDI-0100E: p.A4-99: Wei 2004-Oct-26:
      //
      // Rd = Rn - shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRSub *>(0),
                rn_var,
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
      }
      break;
      
    case DPI_OP_RSB:
      // :ARM-DDI-0100E: p.A4-73: Wei 2004-Oct-26:
      //
      // Rd = shifter_operand - Rn
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRSub *>(0),
                sft_operand,
                rn_var,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
      }
      break;
      
    case DPI_OP_ADD:
      // :ARM-DDI-0100E: p.A4-7: Wei 2004-Oct-26:
      //
      // Rd = Rn + shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRAdd *>(0),
                rn_var,
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
      }
      break;
      
    case DPI_OP_ADC:
      // :ARM-DDI-0100E: p.A4-5: Wei 2004-Oct-26:
      //
      // Rd = Rn + shifter_operand + C Flag
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRAdc *>(0),
                rn_var,
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(CARRY_FLAG, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
      }
      else
      {
        first_main_mir->set_in_flags(CARRY_FLAG);
      }
      break;
      
    case DPI_OP_SBC:
      // :ARM-DDI-0100E: p.A4-77: Wei 2004-Oct-26:
      //
      // Rd = Rn - shifter_operand - NOT(C Flag)
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRSbc *>(0),
                rn_var,
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(CARRY_FLAG, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
      }
      else
      {
        first_main_mir->set_in_flags(CARRY_FLAG);
      }
      break;
      
    case DPI_OP_RSC:
      // :ARM-DDI-0100E: p.A4-75: Wei 2004-Oct-26:
      //
      // Rd = shifter_operand - Rn - NOT(C Flag)
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRSbc *>(0),
                sft_operand,
                rn_var,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(CARRY_FLAG, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
      }
      else
      {
        first_main_mir->set_in_flags(CARRY_FLAG);
      }
      break;
      
    case DPI_OP_ORR:
      // :ARM-DDI-0100E: p.A4-71: Wei 2004-Oct-26:
      //
      // Rd = Rn OR shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIROr *>(0),
                rn_var,
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
      }
      break;
      
    case DPI_OP_BIC:
      // :ARM-DDI-0100E: p.A4-13: Wei 2004-Oct-26:
      //
      // Rd = Rn AND NOT shifter_operand
      {
        Variable * const tmp1 = gp_chunk->get_new_temp();
        
        first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIRNot *>(0),
                  sft_operand,
                  tmp1));
        
        MIR * const mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIRAnd *>(0),
                  rn_var,
                  tmp1,
                  gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
        
        if ((UPDATE_STATUS == is_update_status) &&
            (RdNum() != CR_PC))
        {
          mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
        }
      }
      break;
      
    case DPI_OP_MOV:
      // :ARM-DDI-0100E: p.A4-57: Wei 2004-Oct-26:
      //
      // Rd = shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRMov *>(0),
                sft_operand,
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
      }
      break;
      
    case DPI_OP_MVN:
      // :ARM-DDI-0100E: p.A4-69: Wei 2004-Oct-26:
      //
      // Rd = NOT shifter_operand
      {
        Variable * const tmp1 = gp_chunk->get_new_temp();
        
        first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIRNot *>(0),
                  sft_operand,
                  tmp1));
        
        MIR * const mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIRMov *>(0),
                  tmp1,
                  gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RdNum()))));
        
        if ((UPDATE_STATUS == is_update_status) &&
            (RdNum() != CR_PC))
        {
          mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
        }
      }
      break;
      
    case DPI_OP_TST:
      // :ARM-DDI-0100E: p.A4-107: Wei 2004-Oct-26:
      //
      // alu_out = Rn AND shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRTst *>(0),
                rn_var,
                sft_operand));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
      }
      break;
      
    case DPI_OP_TEQ:
      // :ARM-DDI-0100E: p.A4-106: Wei 2004-Oct-26:
      //
      // alu_out = Rn EOR shifter_operand
      {
        Variable * const tmp1 = gp_chunk->get_new_temp();
        
        first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIREor *>(0),
                  rn_var,
                  sft_operand,
                  tmp1));
        
        if ((UPDATE_STATUS == is_update_status) &&
            (RdNum() != CR_PC))
        {
          first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG);
        }
      }
      break;
      
    case DPI_OP_CMP:
      // :ARM-DDI-0100E: p.A4-25: Wei 2004-Oct-26:
      //
      // alu_out = Rn - shifter_operand
      first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRCmp *>(0),
                rn_var,
                sft_operand));
      
      if ((UPDATE_STATUS == is_update_status) &&
          (RdNum() != CR_PC))
      {
        first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
      }
      break;
      
    case DPI_OP_CMN:
      // :ARM-DDI-0100E: p.A4-23: Wei 2004-Oct-26:
      //
      // alu_out = Rn + shifter_operand
      {
        Variable * const tmp1 = gp_chunk->get_new_temp();
          
        first_main_mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIRAdd *>(0),
                  rn_var,
                  sft_operand,
                  tmp1));
        
        if ((UPDATE_STATUS == is_update_status) &&
            (RdNum() != CR_PC))
        {
          first_main_mir->set_in_out_flags(0, SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG);
        }
      }
      break;
      
    default:
      assert(!"Should not reach here.");
      first_main_mir = 0;
      break;
    }
    
    return first_main_mir;
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  gen_dt_code(Inst const inst, bool const is_last)
  {
    m_inst = inst;
    
    Label *after_switch_label = 0;
    
    Variable * const shifter_operand = gen_shifter_operand(&after_switch_label);
    
    Variable *var_rn;
    
    switch (opcode::OPCODE)
    {
    case DPI_OP_MOV:
    case DPI_OP_MVN:
      // :NOTE: Wei 2004-Dec-29:
      //
      // These 2 operations don't need Rn.
      var_rn = 0;
      break;
      
    default:
      if (CR_PC == RnNum())
      {
        var_rn = gp_chunk->get_new_temp();
        
        MIR * const mir = gp_chunk->append_mir<MIR_FOR_NORMAL>(
          new MIR(static_cast<MIRAdd *>(0),
                  gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(CR_PC)),
                  gp_chunk->find_const_var(8),
                  var_rn));
        
        if (after_switch_label != 0)
        {
          after_switch_label->set_mir_inst(mir);
          
          after_switch_label = 0;
        }
      }
      else
      {
        var_rn = gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(RnNum()));
      }
      break;
    }
    
    MIR * const main_mir = produce_main_mir(var_rn, shifter_operand);
    
    if (after_switch_label != 0)
    {
      after_switch_label->set_mir_inst(main_mir);
    }
    
    switch (is_update_status)
    {
    case UPDATE_STATUS:
      if (CR_PC == RdNum())
      {
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRMov *>(0),
                                                     gp_chunk->find_global_var(Variable::SPSR),
                                                     gp_chunk->find_global_var(Variable::CPSR)));
        
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                     gp_chunk->find_const_var(ER_NORMAL)));
      }
      else
      {
        // :NOTE: Wei 2004-Nov-4:
        //
        // If RdNum() != CR_PC, then the condition flags calculation should be done
        // already.
        gp_chunk->incre_PC_DT();
        
        if (true == is_last)
        {
          gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                       gp_chunk->find_const_var(ER_NORMAL)));
        }
      }
      break;
      
    case NO_UPDATE_STATUS:
      if (CR_PC == RdNum())
      {
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                     gp_chunk->find_const_var(ER_NORMAL)));
      }
      else
      {
        gp_chunk->incre_PC_DT();
        
        if (true == is_last)
        {
          gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                       gp_chunk->find_const_var(ER_NORMAL)));
        }
      }
      break;
      
    default:
      assert(!"Sould not reach here.");
      break;
    }
  }
#endif
}
