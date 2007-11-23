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
#if PRINT_CHUNK_CODE
  template<typename mode, UseWhichStatus use_which_status>
  inline void
  InstMSR<mode, use_which_status>::
  print_fields() const
  {
    switch (m_inst.mRawData & FieldTraits<ALL>::INST_BITMASK)
    {
    case 0: // 0000
      break;
      
    case FieldTraits<CONTROL>::INST_BITMASK: // 0001
      g_log_file << "c";
      break;
      
    case FieldTraits<EXTENSION>::INST_BITMASK: // 0010
      g_log_file << "x";
      break;
      
    case (FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 0011
      g_log_file << "cx";
      break;
      
    case FieldTraits<STATUS>::INST_BITMASK: // 0100
      g_log_file << "s";
      break;
      
    case (FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 0101
      g_log_file << "cs";
      break;
      
    case (FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK): // 0110
      g_log_file << "xs";
      break;
      
    case (FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 0111
      g_log_file << "cxs";
      break;
      
    case FieldTraits<FLAGS>::INST_BITMASK: // 1000
      g_log_file << "f";
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1001
      g_log_file << "cf";
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK): // 1010
      g_log_file << "xf";
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1011
      g_log_file << "cxf";
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK): // 1100
      g_log_file << "sf";
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1101
      g_log_file << "csf";
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK): // 1110
      g_log_file << "xsf";
      break;
      
    case (FieldTraits<FLAGS>::INST_BITMASK |
          FieldTraits<STATUS>::INST_BITMASK |
          FieldTraits<EXTENSION>::INST_BITMASK |
          FieldTraits<CONTROL>::INST_BITMASK): // 1111
      g_log_file << "cxsf";
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  namespace
  {
    template<UseWhichStatus use_which_status>
    inline void
    print_code_helper(InstMSR<ImmMSR, use_which_status> const * const msr)
    {
      g_log_file << msr->Imm();
    }
    
    template<UseWhichStatus use_which_status>
    inline void
    print_code_helper(InstMSR<RegMSR, use_which_status> const * const msr)
    {
      g_log_file << CoreRegName(msr->rm_num());
    }
  }
  
  template<typename mode, UseWhichStatus use_which_status>
  inline void
  InstMSR<mode, use_which_status>::
  print_code(Inst const inst)
  {
    m_inst = inst;
    
    g_log_file << "MSR"
              << CondName(m_inst)
              << " "
              << ((USE_CPSR == use_which_status) ? "CPSR_" : "SPSR_");
    
    print_fields();
    
    g_log_file << ", ";
    
    print_code_helper(this);
    
    g_log_file << std::endl;
  }
#endif
}
