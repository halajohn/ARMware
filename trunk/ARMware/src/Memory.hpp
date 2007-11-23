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

// This is the memory used in computers, such like DRAM etc.
//

#ifndef Memory_hpp
#define Memory_hpp

// System include
//

#if WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <sys/mman.h>
#endif

// Project include
//

#include "ARMware.hpp"
#include "MachineType.hpp"
#include "platform_dep.hpp"

#include "Gtk/MachineScreen.hpp"

EXCEPTION_HANDLER_PROTOTYPE

namespace ARMware
{
#if ENABLE_THREADED_CODE
  // Forward declaration
  
  class ChunkChain;
#endif
  
  typedef class Memory Memory;
  class Memory
  {
  public:
    
    // :NOTE: Wei 2004-Jun-30:
    //
    // This is a bug in gcc-3.x.
    // In order to use PANEL_1 & PANEL_2 as the template argument in the template
    // specializations in the bottom of this file, I have to make this PanelEnum
    // public rather than private.
    //
    // In gcc-3.4, this bug is fixed.
    enum PanelEnum
    {
      PANEL_1,
      PANEL_2
    };
    typedef enum PanelEnum PanelEnum;
    
  private:
    
    enum
    {
      ENABLE_DRAWING = 0x1,
      DUAL_PANEL = 0x2
    };
    
    // Attribute
    
    uint32_t const m_size;
    uint8_t * const mp_memory;
    MachineScreen * const mp_machine_screen;
    
    uint32_t m_lcd_status;
    
    uint32_t m_fb_1_start_addr;
    uint32_t m_fb_1_end_addr;
    
    uint32_t m_fb_2_start_addr;
    uint32_t m_fb_2_end_addr;
    
#if ENABLE_THREADED_CODE
    static uint32_t ms_curr_addr;
    static uint8_t *ms_memory;
    static ChunkChain *ms_chunk_chain;
    
    static void segfault_handler();
#endif
    
    // Operation
    
    template<PanelEnum T_panel>
    uint32_t get_frame_buffer_address() const;
    
    template<typename T_store>
    void update_dirty_point_internal(uint32_t const x, uint32_t const y) const;
    
    template<PanelEnum T_panel, typename T_store>
    inline void
    update_dirty_point(uint32_t const address) const
    {
      // :NOTE: Wei 2004-Jun-30:
      //
      // The pixels in the H3600's frame buffer are 2-byte length.
      //
      // diff: count how many pixels between the beginning and the address, including the address.
      uint32_t const diff = (address - get_frame_buffer_address<T_panel>()) >> 1;
      
      uint32_t const x = (diff / MachineScreen::DISPLAY_HEIGHT);
      uint32_t const y = (MachineScreen::DISPLAY_HEIGHT - (diff % MachineScreen::DISPLAY_HEIGHT) - 1);
      
#if TRACE_DRAW_SCREEN
      g_log_file << "SCREEN: update (" << std::dec << x << ", " << y << ")" << std::endl;
#endif
      
      update_dirty_point_internal<T_store>(x, y);
    }
    
  public:
    
    // Life cycle
    
    Memory(uint32_t const size, MachineScreen * const machine_screen);
    
    ~Memory()
    {
      if (mp_memory != 0)
      {
#if ENABLE_THREADED_CODE

#if ENABLE_DRAWING_ASM
        memory_release(mp_memory, page_aligned_size(m_size + (8 * MachineScreen::BYTES_PER_PIXEL)));
#else
        memory_release(mp_memory, page_aligned_size(m_size));
#endif

#else
        delete[] mp_memory;
#endif
      }
    }
    
    // Access
    
    inline uint8_t *
    get_memory_buffer() const
    { return mp_memory; }
    
    inline uint8_t *
    get_memory_native_addr(uint32_t const paddr) const
    { return &(mp_memory[paddr]); }
    
    // Operation
    
#if ENABLE_THREADED_CODE
    inline void
    register_chunk_chain(ChunkChain * const chunk_chain)
    { ms_chunk_chain = chunk_chain; }
#endif
    
    inline void
    set_frame_buffer_addr_1(uint32_t const address)
    {
      // :NOTE: Wei: 2004-Jul-6:
      //
      // Unlink frame buffer 2, there is a palette space in front of the frame buffer 1.
      // Thus I have to skip it.
      m_fb_1_start_addr = address - MachineType::DRAM_BASE + MachineScreen::PALETTE_OFFSET;
      
      uint32_t const size = (MachineScreen::DISPLAY_WIDTH *
                             MachineScreen::DISPLAY_HEIGHT *
                             MachineScreen::BYTES_PER_PIXEL);
      
      if (m_lcd_status & DUAL_PANEL)
      {
        m_fb_1_end_addr = m_fb_1_start_addr + (size >> 1);
      }
      else
      {
        m_fb_1_end_addr = m_fb_1_start_addr + size;
      }
    }
    
    inline void
    set_frame_buffer_addr_2(uint32_t const address)
    {
      m_fb_2_start_addr = address - MachineType::DRAM_BASE;
      
      m_fb_2_end_addr = m_fb_2_start_addr + ((MachineScreen::DISPLAY_WIDTH *
                                              MachineScreen::DISPLAY_HEIGHT *
                                              MachineScreen::BYTES_PER_PIXEL) >> 1);
    }
    
    inline void
    enable_drawing()
    { m_lcd_status |= ENABLE_DRAWING; }
    
    inline void
    disable_drawing()
    { m_lcd_status &= ~ENABLE_DRAWING; }
    
    inline void
    enable_dual_panel()
    { m_lcd_status |= DUAL_PANEL; }
    
    inline void
    disable_dual_panel()
    { m_lcd_status &= ~DUAL_PANEL; }
    
    template<typename T_return>
    inline T_return FASTCALL
    get_data(uint32_t const address) const
    {
      assert(address <= m_size);
      
      return *reinterpret_cast<T_return *>(&(mp_memory[address]));
    }
    
    template<typename T_store>
    inline void FASTCALL
    put_data(uint32_t const address, T_store const value)
    {
      assert(address <= m_size);
      
#if ENABLE_THREADED_CODE
      ms_curr_addr = address;
#endif
      
      EXCEPTION_PROTECTION_BLOCK_BEGIN
        *reinterpret_cast<T_store *>(&(mp_memory[address])) = value;
      EXCEPTION_PROTECTION_BLOCK_END
      
      switch (m_lcd_status)
      {
      case 0:
      case DUAL_PANEL:
        // disable drawing
        break;
        
      case ENABLE_DRAWING:
        // single panel
        
        // :NOTE: Wei 2004-Jul-10:
        //
        // I was planned to use MMX parallel compare instruction (PCMPGTD) to replace the following
        // twice 'if' comparsions.
        // However, when I do some simple test to compare the time needed by the 2 different methods:
        //
        // 1) use original twice 'if' comparsions.
        // 2) use MMX PCMPGTD to compare parallelly.
        //
        // I found that the method 1 (i.e. original one) is better than the MMX version !!!
        //
        // The reasone why I found are:
        //
        // 1) Even I use -O3, gcc-3.3 doesn't generate the best code.
        // 2) Even after tunning the generated assembly codes by me, the result is still worse than the original one.
        //
        // The resone why I found are:
        //
        // 1) I have to build a MMX register using argument 'address' to compare with the
        //    'm_fb_1_start_addr' & 'm_fb_1_end_addr' pairs.
        //
        // 2) After PCMPGTD, I have to move values from MMX register to 2 general registers to prepare the switch
        //    statement comparsion.
        //
        // 3) And the performance effects of the result assembly codes of the 'switch' statement is almost the same
        //    with the original twice 'if' comparsions.
        //
        // Thus I still use the original twice 'if' comparsions.
        if ((address < m_fb_1_start_addr) || (address >= m_fb_1_end_addr))
        {
          break;
        }
        else
        {
#if TRACE_DRAW_SCREEN
          g_log_file << "SCREEN: update: " << std::hex << address << ", "
                     << type_name<T_store>() << ", "
                     << value << std::endl;
#endif
          
          update_dirty_point<PANEL_1, T_store>(address);
        }
        break;
        
      case (ENABLE_DRAWING | DUAL_PANEL):
        if ((address < m_fb_1_start_addr) || (address >= m_fb_1_end_addr))
        {
          if ((address < m_fb_2_start_addr) || (address >= m_fb_2_end_addr))
          {
            break;
          }
          else
          {
#if TRACE_DRAW_SCREEN
            g_log_file << "SCREEN: update<2>: " << std::hex << address << std::endl;
#endif
            
            update_dirty_point<PANEL_2, T_store>(address);
          }
        }
        else
        {
#if TRACE_DRAW_SCREEN
          g_log_file << "SCREEN: update<1>: " << std::hex << address << std::endl;
#endif
          
          update_dirty_point<PANEL_1, T_store>(address);
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
  };
  
  template<>
  inline uint32_t
  Memory::get_frame_buffer_address<Memory::PANEL_1>() const
  { return m_fb_1_start_addr; }
  
  template<>
  inline uint32_t
  Memory::get_frame_buffer_address<Memory::PANEL_2>() const
  { return m_fb_2_start_addr; }
  
  template<>
  inline void
  Memory::update_dirty_point_internal<uint8_t>(uint32_t const /* x */, uint32_t const /* y */) const
  {
    // :NOTE: Wei 2004-Jul-8:
    //
    // Because the pixel size of H3600 is 2-byte, thus I don't think ARMware will
    // update frame buffer using 1-byte accessing.
    assert(!"Should not reach here.");
  }
  
  template<>
  inline void
  Memory::update_dirty_point_internal<uint16_t>(uint32_t const x, uint32_t const y) const
  {
    mp_machine_screen->update_dirty_point(x, y);
  }
  
  template<>
  inline void
  Memory::update_dirty_point_internal<uint32_t>(uint32_t const x, uint32_t const y) const
  {
    mp_machine_screen->update_dirty_point(x, y);
    mp_machine_screen->update_dirty_point(x, y - 1);
  }
}

#endif
