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

#ifndef MachineScreen_hpp
#define MachineScreen_hpp

// System include
//

#if TRACE_DRAW_SCREEN
#include <iostream>
#endif

#include <cassert>

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/table.h>

// LOCAL INCLUDE
//

#include "../ARMware.hpp"
#include "../MachineType.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Memory;
  class LCDController;
  class AtmelMicro;
  
  typedef class MachineScreen MachineScreen;
  class MachineScreen : public Gtk::ScrolledWindow
  {
  public:
    
    enum PanelEnum
    {
      PANEL_1,
      PANEL_2
    };
    typedef enum PanelEnum PanelEnum;
    
  private:
    
    enum DualPanelEnum
    {
      SINGLE,
      DUAL
    };
    typedef enum DualPanelEnum DualPanelEnum;
    
    enum
    {
      ENABLE_DRAWING = 0x1,
      DUAL_PANEL = 0x2
    };
    
    // Attribute
    
    Gtk::DrawingArea m_drawing_area;
    Gtk::Table       m_table;
    
    Glib::RefPtr<Gdk::Pixbuf> mp_pixbuf;
    Glib::RefPtr<Gdk::Pixmap> mp_pixmap;
    Glib::RefPtr<Gdk::GC> mp_empty_gc;
    
    uint32_t m_lcd_status;
    
    uint32_t m_frame_buffer_addr_1;
    uint32_t m_frame_buffer_addr_2;
    
    uint32_t m_dirty_left;
    uint32_t m_dirty_top;
    
    // :NOTE: Wei 2004-Jun-30:
    //
    // m_dirty_right & m_dirty_bottom are the points right after the dirty rectangle edge.
    uint32_t m_dirty_right;
    uint32_t m_dirty_bottom;
    
    // :NOTE: Wei 2004-Jul-11:
    //
    // The reason why I embedded the address of the memory buffer here rather than the address
    // of the AddressSpace object is because I found that linux will only use memory as its frame buffer.
    //
    // Embedded memory buffer here will increase the performance of the drawing operation
    // because of skipping the switch statement in the AddressSpace class.
    //
    // Another reason I embedded memory buffer address here is because the SSE2 assembly codes
    // will use it directly.
    uint8_t *mp_memory;
    
    LCDController *mp_LCD_controller;
    AtmelMicro *mp_atmel;
    
#if ENABLE_DRAWING_ASM
    struct
    {
      uint64_t m_low __attribute__ ((aligned (16)));
      uint64_t m_high __attribute__ ((packed));
    } m_red_bitmask;
    
    struct
    {
      uint64_t m_low __attribute__ ((aligned (16)));
      uint64_t m_high __attribute__ ((packed));
    } m_green_bitmask;
    
    struct
    {
      uint64_t m_low __attribute__ ((aligned (16)));
      uint64_t m_high __attribute__ ((packed));
    } m_blue_bitmask;
    
    struct
    {
      uint64_t m_low __attribute__((aligned (16)));
      uint64_t m_high __attribute__((packed));
    } m_alpha_value;
#endif
    
#if CONFIG_MACH_H3600
    static uint32_t const DISPLAY_DEPTH = 16;
    
    // :NOTE: Wei 2004-Jan-15:
    //
    // The LCD controller of H3600 is programmed in 16-bit TFT mode.
    //
    // rrrr rggg gggb bbbb
    //
    // Thus...
    //
    // the red color offset is 12
    // the green color offset is 7
    // the blue color offset is 1
    static uint32_t const RED_OFFSET = 12;
    static uint32_t const GREEN_OFFSET = 7;
    static uint32_t const BLUE_OFFSET = 1;
    
    // :NOTE: Wei 2004-Jan-15:
    //
    // Althought the LCD controller of H3600 is programmed in 16-bit TFT mode,
    // but the TFT LCD uses the most highest 4 bit for each color(RGB).
    // That makes a total of 4096 colors.
    //
    // Thus the max values of red, green, blue are 15 (=0xF)
    static uint32_t const RED_MAX_VALUE = 0xF;
    static uint32_t const GREEN_MAX_VALUE = 0xF;
    static uint32_t const BLUE_MAX_VALUE = 0xF;
#endif
    
    // Operation
    
    void init_pixmap_and_pixbuf(uint32_t const width, uint32_t const height);
    
    template<PanelEnum T_panel, DualPanelEnum T_dual>
    void draw_panel();
    
    template<PanelEnum T_panel>
    uint32_t get_panel_start_address() const;
    
    template<PanelEnum T_panel, DualPanelEnum T_dual>
    guint8 *get_pixbuf_start_address() const;
    
    template<PanelEnum T_panel, DualPanelEnum T_dual>
    uint32_t get_drawing_left() const;
    
    template<PanelEnum T_panel, DualPanelEnum T_dual>
    uint32_t get_drawing_right() const;
    
    void drawing_1_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
    
#if ENABLE_DRAWING_ASM
    void drawing_2_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
    void drawing_3_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
    void drawing_4_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
    void drawing_5_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
    void drawing_6_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
    void drawing_7_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
    void drawing_8_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const;
#endif
    
    inline void
    clear_dirty_rect()
    {
      m_dirty_left = DISPLAY_WIDTH;
      m_dirty_right = 0;
      
      m_dirty_top = DISPLAY_HEIGHT;
      m_dirty_bottom = 0;
    }
    
    inline bool
    has_dirty_rect() const
    {
      assert(m_dirty_left != m_dirty_right);
      
      return ((m_dirty_left < m_dirty_right)
              ? true
              : false);
    }
    
    inline void
    update_whole_screen()
    {
      m_dirty_left = 0;
      m_dirty_right = DISPLAY_WIDTH;
      
      m_dirty_top = 0;
      m_dirty_bottom = DISPLAY_HEIGHT;
    }
    
    template<PanelEnum T_panel, DualPanelEnum T_dual>
    inline bool
    dirty_rect_in_this_panel() const
    {
      switch (T_dual)
      {
      case SINGLE:
        return true;
        
      case DUAL:
        switch (T_panel)
        {
        case PANEL_1:
          if (m_dirty_left < (DISPLAY_WIDTH >> 1))
          {
            return true;
          }
          else
          {
            return false;
          }
          
        case PANEL_2:
          if (m_dirty_right > (DISPLAY_WIDTH >> 1))
          {
            return true;
          }
          else
          {
            return false;
          }
          
        default:
          assert(!"Should not reach here.");
          return true;
        }
        
      default:
        assert(!"Should not reach here.");
        return true;
      }
    }
    
    template<PanelEnum T_panel, DualPanelEnum T_dual>
    inline uint32_t
    get_x_diff() const
    {
      switch (T_dual)
      {
      case SINGLE:
        return m_dirty_left;
        
      case DUAL:
        switch (T_panel)
        {
        case PANEL_1:
          return m_dirty_left;
          
        case PANEL_2:
          return m_dirty_left - (DISPLAY_WIDTH >> 1);
          
        default:
          assert(!"Should not reach here.");
          return 0;
        }
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
    }
    
    // Signal handler
    
    void on_drawing_area_size_allocate(Gtk::Allocation &alloc);
    void on_drawing_area_expose_event(GdkEventExpose *event);
    
    void on_drawing_area_button_press_event(GdkEventButton *event);
    void on_drawing_area_button_release_event(GdkEventButton *event);
    void on_drawing_area_motion_notify_event(GdkEventMotion *event);
    
  public:
    
#if CONFIG_MACH_H3600
    static uint32_t const DISPLAY_WIDTH = 240;
    static uint32_t const DISPLAY_HEIGHT = 320;
    static uint32_t const BYTES_PER_PIXEL = (DISPLAY_DEPTH / 8);
    static uint32_t const PALETTE_OFFSET = 32; // 32 bytes of "dummy" palette
#endif
    
    enum PenDownTimes
    {
      PEN_DOWN_FIRST,
      PEN_DOWN_SUCCESSIVE
    };
    typedef enum PenDownTimes PenDownTimes;
    
    // Life cycle
    
    MachineScreen();
    ~MachineScreen();
    
    // Operation
    
    void change_screen_size(uint32_t const width, uint32_t const height);
    
    void get_and_store_next_armware_event();
    bool draw_screen();
    
    inline void
    update_dirty_point(uint32_t const x, uint32_t const y)
    {
      assert(x < DISPLAY_WIDTH);
      assert(y < DISPLAY_HEIGHT);
      
      if (x < m_dirty_left)
      {
        m_dirty_left = x;
      }
      
      if (y < m_dirty_top)
      {
        m_dirty_top = y;
      }
      
      if (x >= m_dirty_right)
      {
        m_dirty_right = (x + 1);
      }
      
      if (y >= m_dirty_bottom)
      {
        m_dirty_bottom = (y + 1);
      }
      
#if TRACE_DRAW_SCREEN
      g_log_file << "SCREEN: 2: " << std::dec
                << m_dirty_left << ", "
                << m_dirty_right << ", "
                << m_dirty_top << ", "
                << m_dirty_bottom << std::endl;
#endif
    }
    
    template<PanelEnum T_panel>
    inline void
    update_panel()
    {
      if (PANEL_1 == T_panel)
      {
        m_dirty_left = 0;
        
        switch (m_lcd_status & DUAL_PANEL)
        {
        case 0:
          // singal panel
          m_dirty_right = DISPLAY_WIDTH;
          break;
          
        case DUAL_PANEL:
          m_dirty_right = (DISPLAY_WIDTH >> 1);
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      else
      {
        m_dirty_left = (DISPLAY_WIDTH >> 1);
        m_dirty_right = DISPLAY_WIDTH;
      }
      
      m_dirty_top = 0;
      m_dirty_bottom = DISPLAY_HEIGHT;
    }
    
    inline void
    enable_drawing()
    {
      m_lcd_status |= ENABLE_DRAWING;
      
      update_whole_screen();
    }
    
    inline void
    disable_drawing()
    {
      m_lcd_status &= ~ENABLE_DRAWING;
      
      // Clear the pixbuf's buffer to black
      //
      // :NOTE: Wei 2004-Apr-13:
      //
      // Because mp_pixbuf has an alpha channel,
      // thus I have to set the value of the alpha channel to 0xFF to show the
      // RGB value I set.
      mp_pixbuf->fill(0x000000FF);
      
      mp_pixmap->draw_pixbuf(mp_empty_gc,
                             mp_pixbuf,
                             0, 0, // src
                             0, 0, // dest
                             static_cast<int>(DISPLAY_WIDTH),
                             static_cast<int>(DISPLAY_HEIGHT),
                             Gdk::RGB_DITHER_NONE,
                             0, 0 // dither offset
                             );
      
      // Force X Window Server to send an expose event to the m_drawing_area.
      m_drawing_area.get_window()->clear_area_e(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    }
    
    inline void
    enable_dual_panel()
    { m_lcd_status |= DUAL_PANEL; }
    
    inline void
    disable_dual_panel()
    { m_lcd_status &= ~DUAL_PANEL; }
    
    inline void
    register_memory(uint8_t * const memory)
    { mp_memory = memory; }
    
    inline void
    register_LCD_controller(LCDController * const LCD_controller)
    { mp_LCD_controller = LCD_controller; }
    
    void register_atmel_micro(AtmelMicro * const atmel);
    
    inline void
    set_frame_buffer_addr_1(uint32_t const address)
    {
      assert((address >= MachineType::DRAM_BASE) &&
             (address <= (MachineType::DRAM_BASE + MachineType::DRAM_SIZE)));
      
      // :SA-1110 Developer's Manual: p.247: Wei 2004-Jun-29:
      //
      // The palette buffer, which resides within the first 16 or 256 entries of the frame buffer,
      // is utilized only by DMA channel 1.
      m_frame_buffer_addr_1 = ((address - MachineType::DRAM_BASE) + PALETTE_OFFSET);
    }
    
    inline void
    set_frame_buffer_addr_2(uint32_t const address)
    {
      assert((address >= MachineType::DRAM_BASE) &&
             (address <= (MachineType::DRAM_BASE + MachineType::DRAM_SIZE)));
      
      m_frame_buffer_addr_2 = (address - MachineType::DRAM_BASE);
    }
  };
}

#endif
