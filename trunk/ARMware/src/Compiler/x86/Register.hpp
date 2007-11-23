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

#ifndef Register_hpp
#define Register_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>
#include <vector>

#include "../../MemoryPool2.hpp"

namespace ARMware
{
  typedef class Register Register;
  class Register : public MemoryPool2<Register, 256>
  {
  public:
    
    enum KindEnum
    {
      EAX = (1 << 0), // 000001
      EBX = (1 << 1), // 000010
      ECX = (1 << 2), // 000100
      EDX = (1 << 3), // 001000
      ESI = (1 << 4), // 010000
      EDI = (1 << 5), // 100000
      
      ALL_REG_BITSET = (EAX | EBX | ECX | EDX | ESI | EDI)
    };
    typedef enum KindEnum KindEnum;
    
    enum RegNativeRep
    {
      EAX_NATIVE_REP = 0,
      ECX_NATIVE_REP = 1,
      EDX_NATIVE_REP = 2,
      EBX_NATIVE_REP = 3,
      ESP_NATIVE_REP = 4,
      EBP_NATIVE_REP = 5,
      ESI_NATIVE_REP = 6,
      EDI_NATIVE_REP = 7
    };
    typedef enum RegNativeRep RegNativeRep;
    
    static uint32_t const REG_NUM = 6;
    
    struct RegisterWeight
    {
      Register::KindEnum m_kind;
      uint32_t m_weight;
      
      // Life cycle
      
      inline
      RegisterWeight(Register::KindEnum const kind, uint32_t const weight)
        : m_kind(kind),
          m_weight(weight)
      { }
      
      // Operator
      
      inline bool
      operator<(RegisterWeight const &weight) const
      {
        return (m_weight < weight.m_weight) ? true : false;
      }
      
#if PRINT_CHUNK_CODE
      void dump_info() const;
#endif
    };
    typedef struct RegisterWeight RegisterWeight;
    
    static uint32_t const UNLIMITED_VALUE = static_cast<uint32_t>(1 << 31);
    
  private:
    
    KindEnum const m_kind;
    bool const m_fixed;
    
    static std::vector<RegisterWeight> REG_WEIGHT;
    
    friend class MemoryPool2<Register, 256>;
        
  public:
    
    // Life cycle
    
    inline
    Register(KindEnum const kind, bool const fixed = false)
      : m_kind(kind),
        m_fixed(fixed)
    { }
    
    // Inquiry
    
    inline KindEnum
    kind() const
    { return m_kind; }
    
    inline bool
    is_fixed() const
    { return m_fixed; }
    
    // Operation
    
    inline RegNativeRep
    native_represent() const
    {
      switch (m_kind)
      {
      case EAX: return EAX_NATIVE_REP;
      case ECX: return ECX_NATIVE_REP;
      case EDX: return EDX_NATIVE_REP;
      case EBX: return EBX_NATIVE_REP;
      case ESI: return ESI_NATIVE_REP;
      case EDI: return EDI_NATIVE_REP;
        
      default:
        assert(!"Should not reach here.");
        return EAX_NATIVE_REP;
      }
    }
    
    // Static operation
    
    static inline void
    register_init()
    {
      assert(0 == REG_WEIGHT.size());
      
      for (uint32_t i = 0; i < REG_NUM; ++i)
      {
        REG_WEIGHT.push_back(RegisterWeight(static_cast<KindEnum>(1 << i), 0));
      }
    }
    
    static inline void
    register_init(std::vector<RegisterWeight> &weight)
    {
      weight.clear();
      
      assert(REG_WEIGHT.size() != 0);
      
      weight = REG_WEIGHT;
    }
    
    static inline uint32_t
    bitset_size()
    { return ((REG_NUM >> 5) + 1); }
    
    static inline void
    set_init_available_set(std::vector<uint32_t> &bitset)
    {
      assert(1 == bitset.size());
      
      bitset.front() = static_cast<uint32_t>(ALL_REG_BITSET);
    }
    
    static inline bool
    is_empty(std::vector<uint32_t> &bitset)
    {
      assert(1 == bitset.size());
      
      if (0 == bitset.front())
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    
    static inline Register *
    create_reg(KindEnum const kind)
    {
      return new Register(kind);
    }
    
    static inline Register *
    create_free_reg(std::vector<uint32_t> &bitset)
    {
      assert(1 == bitset.size());
      assert(false == is_empty(bitset));
      
      uint32_t reg = bitset.front();
      
      for (uint32_t i = 0; i < REG_NUM; ++i)
      {
        if ((reg & 0x1) != 0)
        {
          return new Register(static_cast<KindEnum>(1 << i));
        }
        
        reg >>= 1;
      }
      
      assert(!"Should not reach here.");
      
      return 0;
    }
    
    static inline void
    set_available(std::vector<uint32_t> &bitset, KindEnum const kind)
    {
      assert(1 == bitset.size());
      
      bitset.front() |= kind;
    }
    
    static inline void
    set_available(std::vector<uint32_t> &bitset, Register * const reg)
    {
      assert(reg != 0);
      
      set_available(bitset, reg->kind());
    }
    
    static inline void
    clear_available(std::vector<uint32_t> &bitset, KindEnum const kind)
    {
      assert(1 == bitset.size());
      
      bitset.front() &= ~kind;
    }
    
    static inline void
    clear_available(std::vector<uint32_t> &bitset, Register * const reg)
    {
      assert(reg != 0);
      
      clear_available(bitset, reg->kind());
    }
    
    static inline bool
    check_available(std::vector<uint32_t> &bitset, KindEnum const kind)
    {
      assert(1 == bitset.size());
      
      return (bitset.front() & kind) ? true : false;
    }
    
    static inline bool
    check_available(std::vector<uint32_t> &bitset, Register * const reg)
    {
      assert(reg != 0);
      
      return check_available(bitset, reg->kind());
    }
        
    static inline uint32_t
    array_index(KindEnum const kind)
    {
      switch (kind)
      {
      case EAX: return 0;
      case EBX: return 1;
      case ECX: return 2;
      case EDX: return 3;
      case ESI: return 4;
      case EDI: return 5;
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
    }
    
    static inline KindEnum
    get_one_scratch_reg(uint32_t const exclude_bits = 0,
                        KindEnum const expected = static_cast<KindEnum>(0))
    {
      uint32_t all_reg_bitset = ALL_REG_BITSET;
      
      all_reg_bitset &= ~exclude_bits;
      
      if ((all_reg_bitset & expected) != 0)
      {
        return expected;
      }
      else
      {
        for (uint32_t i = 0; i < REG_NUM; ++i)
        {
          if ((all_reg_bitset & 0x1) != 0)
          {
            return static_cast<KindEnum>(1 << i);
          }
          
          all_reg_bitset >>= 1;
        }
        
        assert(!"Should not reach here.");
        
        return static_cast<KindEnum>(0);
      }
    }
    
    static inline RegNativeRep
    convert_reg_kind_to_its_native_rep(KindEnum const kind)
    {
      switch (kind)
      {
      case EAX: return EAX_NATIVE_REP;
      case EBX: return EBX_NATIVE_REP;
      case ECX: return ECX_NATIVE_REP;
      case EDX: return EDX_NATIVE_REP;
      case ESI: return ESI_NATIVE_REP;
      case EDI: return EDI_NATIVE_REP;
        
      default:
        assert(!"Should not reach here.");
        return static_cast<RegNativeRep>(0);
      }
    }
    
#if PRINT_CHUNK_CODE
    void dump_info() const;
#endif
  };
}

#endif

#endif
