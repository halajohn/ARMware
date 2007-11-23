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

#ifndef LIR_hpp
#define LIR_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <string>
#include <vector>
#include <algorithm>

#include "../Compiler.hpp"
#include "../Variable.hpp"
#include "../OperandKind.hpp"

#include "../../Chunk.hpp"
#include "../../MemoryPool.hpp"

namespace ARMware
{
  // Forware declaration
  //
  
  class BasicBlock;
  class Variable;
  
  // Type
  //
  
  class LIRPhi;
  class LIRFrame;
  
  class LIRCall;
  class LIRPush;
  class LIRReturn;
  
  class LIRMov;
  class LIRMov_from_M;
  class LIRMov_from_A;
  
  class LIRLoad_global;
  class LIRStore_global;
  
  class LIRAdd;
  class LIRAdc;
  class LIRSub;
  class LIRSbc;
  class LIRAnd;
  class LIROr;
  class LIREor;
  
  class LIRMul32;
  class LIRImul32;
  
  class LIRNot;
  
  class LIRJmp;
  class LIRJz;
  class LIRJnz;
  class LIRJc;
  class LIRJnc;
  class LIRJs;
  class LIRJns;
  class LIRJo;
  class LIRJno;
  class LIRJge;
  class LIRJl;
  class LIRJg;
  class LIRJle;
  
  class LIRCmp;
  class LIRTst;
  
  class LIRSetS;
  class LIRSetZ;
  class LIRSetC;
  class LIRSetO;
  
  class LIRLSftLeft;
  class LIRLSftRight;
  class LIRASftRight;
  class LIRRor;
  class LIRRrx;
  
  class LIRPushF;
  class LIRPopF;
  
  class LIRLoadSP;
  class LIRStoreSP;
  class LIRAddSP;
  
  class LIRBt;
  class LIRCmc;
  
  typedef class LIR LIR;
  class LIR : public MemoryPool<LIR, 256>
  {
  public:
    
    enum KindEnum
    {
      PHI,
      
      FRAME,
      
      CALL,
      PUSH_R,
      PUSH_I,
      RETURN,
      
      MOV_I_2_R,
      MOV_R_2_R,
      
      MOV_LABEL_2_R,
      
      MOV_FROM_M,
      MOV_FROM_A,
      
      LOAD_GLOBAL,
      STORE_GLOBAL,
      
      // :NOTE: Wei 2004-Dec-2:
      //
      // Because the first operand of x86 can't be constant,
      // thus the LIR doesn't have xxx_IR or xxx_II
      ADD_RR,
      ADD_RI,
      
      ADC_RR,
      ADC_RI,
      
      SUB_RR,
      SUB_RI,
      
      SBC_RR,
      SBC_RI,
      
      AND_RR,
      AND_RI,
      
      OR_RR,
      OR_RI,
      
      EOR_RR,
      EOR_RI,
      
      MUL32,
      IMUL32,
      
      NOT,
      
      JMP_I,
      JMP_R,
      
      JZ, // Z==1
      JNZ, // Z==0
      
      JC, // C==1
      JNC, // C==0
      
      JS, // S==1
      JNS, // S==0
      
      JO, // O==1
      JNO, // O==0
      
      JGE, // S==O
      JL, // S!=O
      
      JG, // Z==0 && S==O
      JLE, // Z==1 || S!=O
      
      CMP_RR,
      CMP_RI,
      
      TST_RR,
      TST_RI,
      
      SET_S,
      SET_Z,
      SET_C,
      SET_O,
      
      LSFT_LEFT_RR,
      LSFT_LEFT_RI,
      
      LSFT_RIGHT_RR,
      LSFT_RIGHT_RI,
      
      ASFT_RIGHT_RR,
      ASFT_RIGHT_RI,
      
      ROR_RR,
      ROR_RI,
      
      RRX_RR,
      RRX_RI,
      
      PUSHF,
      POPF,
      
      LOAD_SP,
      STORE_SP,
      ADD_SP,
      
      BT,
      CMC,
      
      KIND_TOTAL
    };
    typedef enum KindEnum KindEnum;
    
  private:
    
#if PRINT_CHUNK_CODE
    static std::string const KIND_NAME[KIND_TOTAL];
#endif
    
    KindEnum m_kind;
    uint32_t m_idx;
    uint32_t m_native_buffer_offset;
    
    std::vector<uint32_t> m_def;
    std::vector<uint32_t> m_use;
    std::vector<uint32_t> m_in;
    std::vector<uint32_t> m_out;
    
    std::vector<OperandKind *> m_operand;
    std::vector<OperandKind *> m_dest;
    
    std::vector<OperandKind *> m_memory_use_list;
    std::vector<OperandKind *> m_memory_def_list;
    
    uint32_t m_memory_operation_idx;
    Tuple *mp_equivalent_operation;
    
    LIR *mp_prev_lir;
    
    BasicBlock *mp_defined_bb;
    
    bool m_cond_flags_involved;
    
    friend class MemoryPool<LIR, 256>;
    
  public:
    
    // Life cycle
    
    inline
    LIR(LIRPhi *,
        Variable * const var,
        uint32_t const operand_count)
      : m_kind(PHI)
    {
      m_dest.push_back(new OperandKind(var));
      
      for (uint32_t i = 0; i < operand_count; ++i)
      {
        m_operand.push_back(new OperandKind(static_cast<Variable *>(0)));
      }
      
      var->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRFrame *)
      : m_kind(FRAME)
    {
      for (Variable *curr_var = gp_chunk->global_var_head();
           curr_var != 0;
           curr_var = curr_var->next_link_var())
      {
        OperandKind * const memory_operand = new OperandKind(gp_chunk->find_memory_var(curr_var->global_var()));
        
#if CHECK_CHUNK_CODE
        memory_operand->variable()->add_defined_lir(this);
#endif
        
        m_memory_def_list.push_back(memory_operand);
      }
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRCall *, Label * const label)
      : m_kind(CALL)
    {
      m_operand.push_back(new OperandKind(label));
      
      for (Variable *curr_var = gp_chunk->global_var_head();
           curr_var != 0;
           curr_var = curr_var->next_link_var())
      {
        m_memory_use_list.push_back(new OperandKind(gp_chunk->find_memory_var(curr_var->global_var())));
        
        OperandKind * const memory_operand = new OperandKind(gp_chunk->find_memory_var(curr_var->global_var()));
        
#if CHECK_CHUNK_CODE
        memory_operand->variable()->add_defined_lir(this);
#endif
        
        m_memory_def_list.push_back(memory_operand);
      }
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRMov *,
        Variable * const src,
        Variable * const dest)
      : m_kind(MOV_R_2_R)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRMov *,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(MOV_I_2_R)
    {
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRMov_from_M *,
        Variable * const src,
        Variable * const dest)
      : m_kind(MOV_FROM_M)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRLoad_global *,
        Variable * const dest)
      : m_kind(LOAD_GLOBAL)
    {
      assert(dest != 0);
      
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_memory_use_list.push_back(new OperandKind(gp_chunk->find_memory_var(dest->global_var())));
      
      m_cond_flags_involved = false;
      mp_equivalent_operation = 0;
    }
    
    inline
    LIR(LIRStore_global *,
        Variable * const src)
      : m_kind(STORE_GLOBAL)
    {
      assert(src != 0);
      
      m_operand.push_back(new OperandKind(src));
      
      m_memory_use_list.push_back(new OperandKind(gp_chunk->find_memory_var(src->global_var())));
      
      OperandKind * const memory_operand = new OperandKind(gp_chunk->find_memory_var(src->global_var()));
      
#if CHECK_CHUNK_CODE
      memory_operand->variable()->add_defined_lir(this);
#endif
      
      m_memory_def_list.push_back(memory_operand);
      
      m_cond_flags_involved = false;
      mp_equivalent_operation = 0;
    }
    
    inline
    LIR(LIRMov *,
        Label * const label,
        Variable * const dest)
      : m_kind(MOV_LABEL_2_R)
    {
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(label));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
        
    inline
    LIR(LIRMov_from_A *,
        Variable * const var)
      : m_kind(MOV_FROM_A)
    {
      assert(var != 0);
      
      m_dest.push_back(new OperandKind(var));
      
      var->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRAdd *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(ADD_RR)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRAdd *,
        Variable * const src,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(ADD_RI)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRAdc *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(ADC_RR)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRAdc *,
        Variable * const src,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(ADC_RI)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRSub *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(SUB_RR)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRSub *,
        Variable * const src,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(SUB_RI)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRSbc *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(SBC_RR)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRSbc *,
        Variable * const src,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(SBC_RI)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRAnd *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(AND_RR)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRAnd *,
        Variable * const src,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(AND_RI)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIROr *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(OR_RR)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIROr *,
        Variable * const src,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(OR_RI)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIREor *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(EOR_RR)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIREor *,
        Variable * const src,
        uint32_t const imm,
        Variable * const dest)
      : m_kind(EOR_RI)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(imm));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRMul32 *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest_hi,
        Variable * const dest_lo)
      : m_kind(MUL32)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest_hi != 0);
      assert(dest_lo != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest_hi));
      m_dest.push_back(new OperandKind(dest_lo));
      
      dest_hi->add_defined_lir(this);
      dest_lo->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRImul32 *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest_hi,
        Variable * const dest_lo)
      : m_kind(IMUL32)
    {
      assert(src1 != 0);
      assert(src2 != 0);
      assert(dest_hi != 0);
      assert(dest_lo != 0);
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest_hi));
      m_dest.push_back(new OperandKind(dest_lo));
      
      dest_hi->add_defined_lir(this);
      dest_lo->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRNot *,
        Variable * const src,
        Variable * const dest)
      : m_kind(NOT)
    {
      assert(src != 0);
      assert(dest != 0);
      
      m_operand.push_back(new OperandKind(src));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRPush *,
        Variable * const var)
      : m_kind(PUSH_R)
    {
      assert(var != 0);
      
      m_operand.push_back(new OperandKind(var));
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRPush *,
        uint32_t const imm)
      : m_kind(PUSH_I)
    {
      m_operand.push_back(new OperandKind(imm));
      
      m_cond_flags_involved = false;
    }
    
    LIR(LIRReturn *)
      : m_kind(RETURN)
    {
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRJmp *,
        Label * const label)
      : m_kind(JMP_I)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRJmp *,
        Variable * const var)
      : m_kind(JMP_R)
    {
      m_operand.push_back(new OperandKind(var));
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRJz *, Label * const label)
      : m_kind(JZ)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJnz *, Label * const label)
      : m_kind(JNZ)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJc *, Label * const label)
      : m_kind(JC)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJnc *, Label * const label)
      : m_kind(JNC)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJs *, Label * const label)
      : m_kind(JS)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJns *, Label * const label)
      : m_kind(JNS)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJo *, Label * const label)
      : m_kind(JO)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJno *, Label * const label)
      : m_kind(JNO)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJge *, Label * const label)
      : m_kind(JGE)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJl *, Label * const label)
      : m_kind(JL)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJg *, Label * const label)
      : m_kind(JG)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRJle *, Label * const label)
      : m_kind(JLE)
    {
      m_operand.push_back(new OperandKind(label));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRCmp *,
        Variable * const var1,
        Variable * const var2)
      : m_kind(CMP_RR)
    {
      assert(var1 != 0);
      assert(var2 != 0);
      
      m_operand.push_back(new OperandKind(var1));
      m_operand.push_back(new OperandKind(var2));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRCmp *,
        Variable * const var,
        uint32_t const imm)
      : m_kind(CMP_RI)
    {
      assert(var != 0);
      
      m_operand.push_back(new OperandKind(var));
      m_operand.push_back(new OperandKind(imm));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRTst *,
        Variable * const var1,
        Variable * const var2)
      : m_kind(TST_RR)
    {
      assert(var1 != 0);
      assert(var2 != 0);
      
      m_operand.push_back(new OperandKind(var1));
      m_operand.push_back(new OperandKind(var2));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRTst *,
        Variable * const var,
        uint32_t const imm)
      : m_kind(TST_RI)
    {
      assert(var != 0);
      
      m_operand.push_back(new OperandKind(var));
      m_operand.push_back(new OperandKind(imm));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRSetS *,
        Variable * const dest)
      : m_kind(SET_S)
    {
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRSetZ *, Variable * const dest)
      : m_kind(SET_Z)
    {
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRSetC *, Variable * const dest)
      : m_kind(SET_C)
    {
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRSetO *, Variable * const dest)
      : m_kind(SET_O)
    {
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRLSftLeft *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(LSFT_LEFT_RR)
    {
      assert(Register::ECX == src2->preallocated_reg()->kind());
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRLSftLeft *,
        Variable * const src,
        uint32_t const amount,
        Variable * const dest)
      : m_kind(LSFT_LEFT_RI)
    {
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(amount));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRLSftRight *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(LSFT_RIGHT_RR)
    {
      assert(Register::ECX == src2->preallocated_reg()->kind());
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRLSftRight *,
        Variable * const src,
        uint32_t const amount,
        Variable * const dest)
      : m_kind(LSFT_RIGHT_RI)
    {
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(amount));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRASftRight *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(ASFT_RIGHT_RR)
    {
      assert(Register::ECX == src2->preallocated_reg()->kind());
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRASftRight *,
        Variable * const src,
        uint32_t const amount,
        Variable * const dest)
      : m_kind(ASFT_RIGHT_RI)
    {
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(amount));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRRor *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(ROR_RR)
    {
      assert(Register::ECX == src2->preallocated_reg()->kind());
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRRor *,
        Variable * const src,
        uint32_t const amount,
        Variable * const dest)
      : m_kind(ROR_RI)
    {
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(amount));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRRrx *,
        Variable * const src1,
        Variable * const src2,
        Variable * const dest)
      : m_kind(RRX_RR)
    {
      assert(Register::ECX == src2->preallocated_reg()->kind());
      
      m_operand.push_back(new OperandKind(src1));
      m_operand.push_back(new OperandKind(src2));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRRrx *,
        Variable * const src,
        uint32_t const amount,
        Variable * const dest)
      : m_kind(RRX_RI)
    {
      m_operand.push_back(new OperandKind(src));
      m_operand.push_back(new OperandKind(amount));
      m_dest.push_back(new OperandKind(dest));
      
      dest->add_defined_lir(this);
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRPushF *)
      : m_kind(PUSHF)
    {
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRPopF *)
      : m_kind(POPF)
    {
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRLoadSP *,
        Variable * const var)
      : m_kind(LOAD_SP)
    {
      m_dest.push_back(new OperandKind(var));
      
      var->add_defined_lir(this);
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRAddSP *,
        uint32_t const count)
      : m_kind(ADD_SP)
    {
      m_operand.push_back(new OperandKind(count));
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRStoreSP *,
        Variable * const var)
      : m_kind(STORE_SP)
    {
      m_operand.push_back(new OperandKind(var));
      
      m_cond_flags_involved = false;
    }
    
    inline
    LIR(LIRBt *,
        Variable * const var,
        uint32_t const offset)
      : m_kind(BT)
    {
      assert(offset <= 31);
      
      m_operand.push_back(new OperandKind(var));
      m_operand.push_back(new OperandKind(offset));
      
      m_cond_flags_involved = true;
    }
    
    inline
    LIR(LIRCmc *)
      : m_kind(CMC)
    {
      m_cond_flags_involved = true;
    }
    
    ~LIR();
    
    // Operation
    
#if CHECK_CHUNK_CODE
    void check_label_linked() const;
    void check_bb_link_relationship() const;
#endif
    
    inline void 
    set_PHI_operand(uint32_t const idx)
    {
      assert(PHI == m_kind);
      assert(m_operand.size() > idx);
      assert(1 == m_dest.size());
      
      Variable * const var = m_dest.front()->variable()->original_ssa_var()->get_ssa_var<USE>();
      
      assert(var != 0);
      
      m_operand[idx]->set_variable(var);
    }
    
    inline void 
    replace_PHI_operand(uint32_t const idx, Variable * const var)
    {
      assert(PHI == m_kind);
      assert(m_operand.size() > idx);
      assert(1 == m_dest.size());
      assert(var != 0);
      
      m_operand[idx]->set_variable(var);
    }
    
    template<bool T_need_load_store>
    void before_append_lir(BasicBlock * const bb);
    
    template<bool T_need_load_store>
    void after_append_lir(BasicBlock * const bb);
    
    template<ConsiderMemoryVarEnum T_consider>
    void determine_def_use(BasicBlock * const bb);
    
    void determine_AVAIL_AVLOC(BasicBlock * const bb);
    
    void pop_ssa_var();
    
    template<DefUseEnum T_type, VariableTypeEnum T_var_type>
    void set_ssa_variable_internal(OperandKind * const operand);
    
    void set_ssa_variable();
    
    void computing_def_use(std::vector<Variable *> &def_var, std::vector<Variable *> &use_var);
    
    bool is_jmp_type() const;
    ValueNumberingTypeEnum value_numbering_type() const;
    
    void join_live_interval();
    void emit_native_code(std::vector<uint8_t> &native_code_buffer);
    
    inline void
    set_defined_bb(BasicBlock * const bb)
    { mp_defined_bb = bb; }
    
#if CHECK_CHUNK_CODE
    void check_ssa_property(BasicBlock const * const bb);
    std::vector<OperandKind *> &find_list(Variable const * const var);
    void check_var_is_dest(Variable const * const var);
    void check_live_interval_preallocated_reg();
#endif
    
#if PRINT_CHUNK_CODE
    template<bool T_ssa_enabled, bool T_print_memory_operation_idx>
    void dump_info() const;
#endif
    
    // Access
    
    inline void
    set_kind(KindEnum const kind)
    { m_kind = kind; }
    
    inline void
    set_memory_operation_idx(uint32_t const idx)
    { m_memory_operation_idx = idx; }
    
    inline void
    set_equivalent_operation(Tuple * const tuple)
    {
      assert(tuple != 0);
      
      mp_equivalent_operation = tuple;
    }
    
    inline void
    set_cond_flags_involved()
    {
      assert(false == m_cond_flags_involved);
      
      m_cond_flags_involved = true;
    }
    
    inline void
    set_next_lir(LIR * const lir)
    { mp_next_used_node = lir; }
    
    inline void
    set_prev_lir(LIR * const lir)
    { mp_prev_lir = lir; }
    
    inline void
    set_idx(uint32_t const idx)
    { m_idx = idx; }
    
    inline std::vector<OperandKind *> &
    operand()
    { return m_operand; }
    
    inline void
    set_native_buffer_offset(uint32_t const offset)
    { m_native_buffer_offset = offset; }
    
    template<VariableTypeEnum T_type>
    inline void
    set_ssa_def_variable(Variable * const orig,
                         Variable * const new_var)
    {
      assert(orig->kind() == new_var->kind());
      
      switch (T_type)
      {
      case NON_MEMORY_VAR:
        if (orig != new_var)
        {
          orig->remove_defined_lir(this);
          
#if CHECK_CHUNK_CODE
          orig->remove_defined_bb(mp_defined_bb);
#endif
          
          new_var->add_defined_lir(this);
          
#if CHECK_CHUNK_CODE
          new_var->add_defined_bb(mp_defined_bb);
#endif
        }
        break;
        
      case MEMORY_VAR:
#if CHECK_CHUNK_CODE
        if (orig != new_var)
        {
          orig->remove_defined_lir(this);
          
          orig->remove_defined_bb(mp_defined_bb);
          
          new_var->add_defined_lir(this);
          
          new_var->add_defined_bb(mp_defined_bb);
        }
#endif
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    // Inquiry
    
    inline uint32_t
    memory_operation_idx() const
    { return m_memory_operation_idx; }
    
    inline Tuple *
    equivalent_operation() const
    { return mp_equivalent_operation; }
    
    inline bool
    is_cond_flags_involved() const
    { return m_cond_flags_involved; }
    
    inline LIR *
    next_lir() const
    { return mp_next_used_node; }
    
    inline LIR *
    prev_lir() const
    { return mp_prev_lir; }
    
    inline uint32_t
    idx() const
    { return m_idx; }
    
    inline KindEnum
    kind() const
    { return m_kind; }
    
    inline Variable *
    phi_def_var() const
    {
      assert(1 == m_dest.size());
      
      return m_dest.front()->variable();
    }
    
    inline std::vector<OperandKind *> const &
    operand() const
    { return m_operand; }
    
    inline std::vector<OperandKind *> const &
    dest() const
    { return m_dest; }
    
    inline std::vector<OperandKind *> const &
    memory_use_list() const
    { return m_memory_use_list; }
    
    inline std::vector<OperandKind *> const &
    memory_def_list() const
    { return m_memory_def_list; }
    
    inline BasicBlock *
    defined_bb() const
    { return mp_defined_bb; }
    
    inline uint32_t
    native_buffer_offset() const
    { return m_native_buffer_offset; }
  };
}

#endif

#endif
