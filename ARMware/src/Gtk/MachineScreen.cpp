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

// System include
//

#include <cassert>

#if TRACE_DRAW_SCREEN
#include <iostream>
#endif

#if WIN32
#pragma warning(disable: 4250)
#endif

#include <gtkmm/main.h>

// Local include
//

#include "../Memory.hpp"
#include "../ColorTableTraits.hpp"
#include "../LCDController.hpp"
#include "../AtmelMicro.hpp"

#include "MachineScreen.hpp"

namespace ARMware
{
  ///////////////////////////////// Private ////////////////////////////////////
  //============================== Operation ===================================
  
  void
  MachineScreen::init_pixmap_and_pixbuf(uint32_t const width, uint32_t const height)
  {
    // :NOTE: Wei 2004-Apr-13:
    //
    // I create a pixbuf of 32-bit per pixel.
    //
    // The reason why I create a pixbuf with an alpha channel is just because
    // that each pixel is 4-byte long.
    //
    // This size will meet the alignment requirement in most case, and should be
    // faster.
    
    // :NOTE: Wei 2004-Jul-12:
    //
    // The reason why the height of the pixbuf is 'height + 8' when using ENABLE_DRAWING_ASM
    // is because I will get 8 pixels data at most at once.
    //
    // Thus I have to increment the height to avoid the segmentation fault.
    mp_pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,
                                    true, // has_alpha
                                    8, // bits_per_sample
                                    width,
#if ENABLE_DRAWING_ASM
                                    height + 8
#else
                                    height
#endif
                                    );
    
    // Clear the pixbuf's buffer to black
    //
    // :NOTE: Wei 2004-Apr-13:
    //
    // Because mp_pixbuf has an alpha channel,
    // thus I have to set the value of the alpha channel to 0xFF to show the
    // RGB value I set.
    mp_pixbuf->fill(0x000000FF);
    
    mp_pixmap = Gdk::Pixmap::create(m_drawing_area.get_window(),
                                    width,
                                    height,
                                    -1
                                    );
    
    mp_pixmap->draw_pixbuf(mp_empty_gc,
                           mp_pixbuf,
                           0, 0, // src
                           0, 0, // dest
                           static_cast<int>(width),
                           static_cast<int>(height),
                           Gdk::RGB_DITHER_NONE,
                           0, 0 // dither offset
                           );
    
    // I don't want the drawing area to have a backing pixmap,
    // I will redraw part of it in the expose event signal handler.
    m_drawing_area.get_window()->set_back_pixmap(Glib::RefPtr<Gdk::Pixmap>(),
                                                 false // parent_relative
                                                 );
    
    m_drawing_area.get_window()->draw_drawable(m_drawing_area.get_style()->get_bg_gc(Gtk::STATE_NORMAL),
                                               mp_pixmap,
                                               0, 0, // src x, y (i.e. mp_pixmap)
                                               0, 0  // dest x, y (i.e. m_drawing_area.get_window())
                                               );
  }
  
  template<MachineScreen::PanelEnum T_panel>
  inline uint32_t
  MachineScreen::get_panel_start_address() const
  {
    switch (T_panel)
    {
    case PANEL_1:
      return m_frame_buffer_addr_1;
      
    case PANEL_2:
      return m_frame_buffer_addr_2;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  } 
  
  template<MachineScreen::PanelEnum T_panel, MachineScreen::DualPanelEnum T_dual>
  inline guint8 *
  MachineScreen::get_pixbuf_start_address() const
  {
    switch (T_dual)
    {
    case SINGLE:
      assert(PANEL_1 == T_panel);
      return mp_pixbuf->get_pixels();
      
    case DUAL:
      switch (T_panel)
      {
      case PANEL_1:
        return mp_pixbuf->get_pixels();
        
      case PANEL_2:
        // :NOTE: Wei 2004-Jun-29:
        //
        // The pixel width of Gdk pixbuf is 4-byte.
        // (Alpha, red, green, blue)
        return mp_pixbuf->get_pixels() + ((DISPLAY_WIDTH >> 1) << 2);
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<MachineScreen::PanelEnum T_panel, MachineScreen::DualPanelEnum T_dual>
  inline uint32_t
  MachineScreen::get_drawing_left() const
  {
    switch (T_dual)
    {
    case SINGLE:
      assert(PANEL_1 == T_panel);
      return m_dirty_left;
      
    case DUAL:
      switch (T_panel)
      {
      case PANEL_1:
        return ((m_dirty_left < (DISPLAY_WIDTH >> 1))
                ? m_dirty_left
                : (DISPLAY_WIDTH >> 1));
        
      case PANEL_2:
        return ((m_dirty_left < (DISPLAY_WIDTH >> 1))
                ? (DISPLAY_WIDTH >> 1)
                : m_dirty_left);
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<MachineScreen::PanelEnum T_panel, MachineScreen::DualPanelEnum T_dual>
  inline uint32_t
  MachineScreen::get_drawing_right() const
  {
    switch (T_dual)
    {
    case SINGLE:
      assert(PANEL_1 == T_panel);
      return m_dirty_right;
      
    case DUAL:
      switch (T_panel)
      {
      case PANEL_1:
        return ((m_dirty_right < (DISPLAY_WIDTH >> 1))
                ? m_dirty_right
                : (DISPLAY_WIDTH >> 1));
        
      case PANEL_2:
        return ((m_dirty_right < (DISPLAY_WIDTH >> 1))
                ? (DISPLAY_WIDTH >> 1)
                : m_dirty_right);
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
#if ENABLE_DRAWING_ASM
  
  // :NOTE: Wei 2004-Jul-12:
  //
  // The 'psrlq' for MMX & 'psrldq' for SSE are different.
  //
  // 'psrlq' uses bits as element to shift, however,
  // 'psrldq' uses bytes as element to shift.
#define MMX_STORE_PIXEL(mmx_r) "subl %6, %%edi \n\t" \
                               "psrlq $32, %%" #mmx_r "\n\t" \
                               "maskmovq %%mm2, %%" #mmx_r "\n\t"
  
#define SSE2_STORE_PIXEL(sse_r) "subl %6, %%edi \n\t" \
                                "psrldq $4, %%" #sse_r "\n\t" \
                                "maskmovdqu %%xmm2, %%" #sse_r "\n\t"
  
  // :NOTE: Wei 2004-Jul-12:
  //
  // If just drawing 1 or 2 pixels, then we can eliminate some operations, see below.
#define MMX_DRAW_1_2_PIXELS_PROLOGUE \
      asm volatile ( \
      "movq %0, %%mm0 \n\t" /* read 4 pixels */ \
      "movq %%mm0, %%mm1 \n\t" \
      "movq %%mm0, %%mm2 \n\t" \
      \
      "pand %1, %%mm0 \n\t" /* mm0: red */ \
      "pand %2, %%mm1 \n\t" /* mm1: green */ \
      "pand %3, %%mm2 \n\t" /* mm2: blue */ \
      \
      "psrlw $8, %%mm0\n\t" /* red shift right 12, multiply 16 */ \
      "psrlw $3, %%mm1\n\t" /* green shift right 7, multiply 16 */ \
      "psllw $3, %%mm2\n\t" /* blue shift right 1, multiply 16 */ \
      \
      "packuswb %%mm3, %%mm0 \n\t" /* mm0: low 32-bit red */ \
      "packuswb %%mm3, %%mm1 \n\t" /* mm1: low 32-bit green */ \
      "packuswb %%mm3, %%mm2 \n\t" /* mm2: low 32-bit blue */ \
      \
      "punpcklbw %%mm1, %%mm0 \n\t" /* mm0: 64-bit {green, red} */ \
      /* "movq %%mm0, %%mm1 \n\t" --> mm1: 64-bit {green, red} */ \
      "punpcklbw %4, %%mm2 \n\t" /* mm2: 64-bit {alpha, blue} */ \
      \
      "punpcklwd %%mm2, %%mm0 \n\t" /* mm0: 64-bit {alpha, blue, green, red} */ \
      /* "punpckhwd %%mm2, %%mm1 \n\t" --> mm1: 64-bit {alpha, blue, green, red} */ \
      \
      "movl %5, %%edi \n\t" /* edi: dest address */ \
      \
      "movl $0xFFFFFFFF, %%edx \n\t" \
      "movd %%edx, %%mm2 \n\t" /* mm2: 0x00000000FFFFFFFF */ \
      "maskmovq %%mm2, %%mm0 \n\t"
                     
#define MMX_DRAW_PIXELS_PROLOGUE \
      asm volatile ( \
      "movq %0, %%mm0 \n\t" /* read 4 pixels */ \
      "movq %%mm0, %%mm1 \n\t" \
      "movq %%mm0, %%mm2 \n\t" \
      \
      "pand %1, %%mm0 \n\t" /* mm0: red */ \
      "pand %2, %%mm1 \n\t" /* mm1: green */ \
      "pand %3, %%mm2 \n\t" /* mm2: blue */ \
      \
      "psrlw $8, %%mm0\n\t" /* red shift right 12, multiply 16 */ \
      "psrlw $3, %%mm1\n\t" /* green shift right 7, multiply 16 */ \
      "psllw $3, %%mm2\n\t" /* blue shift right 1, multiply 16 */ \
      \
      "packuswb %%mm3, %%mm0 \n\t" /* mm0: low 32-bit red */ \
      "packuswb %%mm3, %%mm1 \n\t" /* mm1: low 32-bit green */ \
      "packuswb %%mm3, %%mm2 \n\t" /* mm2: low 32-bit blue */ \
      \
      "punpcklbw %%mm1, %%mm0 \n\t" /* mm0: 64-bit {green, red} */ \
      "movq %%mm0, %%mm1 \n\t" /* mm1: 64-bit {green, red} */ \
      "punpcklbw %4, %%mm2 \n\t" /* mm2: 64-bit {alpha, blue} */ \
      \
      "punpcklwd %%mm2, %%mm0 \n\t" /* mm0: 64-bit {alpha, blue, green, red} */ \
      "punpckhwd %%mm2, %%mm1 \n\t" /* mm1: 64-bit {alpha, blue, green, red} */ \
      \
      "movl %5, %%edi \n\t" /* edi: dest address */ \
      \
      "movl $0xFFFFFFFF, %%edx \n\t" \
      "movd %%edx, %%mm2 \n\t" /* mm2: 0x00000000FFFFFFFF */ \
      "maskmovq %%mm2, %%mm0 \n\t"
                     
#define SSE2_DRAW_PIXELS_PROLOGUE \
      asm volatile ( \
      "movdqu %0, %%xmm0 \n\t" /* read 8 pixels */ \
      "movdqa %%xmm0, %%xmm1 \n\t" \
      "movdqa %%xmm0, %%xmm2 \n\t" \
      \
      "pand %1, %%xmm0 \n\t" /* xmm0: red */ \
      "pand %2, %%xmm1 \n\t" /* xmm1: green */ \
      "pand %3, %%xmm2 \n\t" /* xmm2: blue */ \
      \
      "psrlw $8, %%xmm0\n\t" /* red shift right 12, multiply 16 */ \
      "psrlw $3, %%xmm1\n\t" /* green shift right 7, multiply 16 */ \
      "psllw $3, %%xmm2\n\t" /* blue shift right 1, multiply 16 */ \
      \
      "packuswb %%xmm3, %%xmm0 \n\t" /* xmm0: low 64-bit red */ \
      "packuswb %%xmm3, %%xmm1 \n\t" /* xmm1: low 64-bit green */ \
      "packuswb %%xmm3, %%xmm2 \n\t" /* xmm2: low 64-bit blue */ \
      \
      "punpcklbw %%xmm1, %%xmm0 \n\t" /* xmm0: 128-bit {green, red} */ \
      "movdqa %%xmm0, %%xmm1 \n\t" /* xmm1: 128-bit {green, red} */ \
      "punpcklbw %4, %%xmm2 \n\t" /* xmm2: 128-bit {alpha, blue} */ \
      \
      "punpcklwd %%xmm2, %%xmm0 \n\t" /* xmm0: 128-bit {alpha, blue, green, red} */ \
      "punpckhwd %%xmm2, %%xmm1 \n\t" /* xmm1: 128-bit {alpha, blue, green, red} */ \
      \
      "movl %5, %%edi \n\t" /* edi: dest address */ \
      \
      "movl $0xFFFFFFFF, %%edx \n\t" \
      "movd %%edx, %%xmm2 \n\t" /* xmm2: 0x000000000000000000000000FFFFFFFF */ \
      "maskmovdqu %%xmm2, %%xmm0 \n\t"
      
#define MMX_DRAW_PIXELS_EPILOGUE \
      "subl %6, %%edi \n\t" \
      "movl %%edi, %5" \
      : \
      : "m" (mp_memory[curr_addr]), \
      "m" (m_red_bitmask.m_low), \
      "m" (m_green_bitmask.m_low), \
      "m" (m_blue_bitmask.m_low), \
      "m" (m_alpha_value.m_low), \
      "m" (dest), \
      "r" (row_length) \
      : "edx", "edi", "memory", "mm0", "mm1", "mm2" \
      )
      
#define SSE2_DRAW_PIXELS_EPILOGUE \
      "subl %6, %%edi \n\t" \
      "movl %%edi, %5" \
      : \
      : "m" (mp_memory[curr_addr]), \
      "m" (m_red_bitmask), \
      "m" (m_green_bitmask), \
      "m" (m_blue_bitmask), \
      "m" (m_alpha_value), \
      "m" (dest), \
      "r" (row_length) \
      : "edx", "edi", "memory", "xmm0", "xmm1", "xmm2" \
      )
      
  // :NOTE: Wei 2004-Jul-12:
  //
  // The reason why I have the following 1, 2, 3, 4, 5, 6, 7, 8 pixels drawing routines
  // is because I want to minimize the store operations in the end of each drawing routines.
  //
  // Ex: If I just need to drawing 6 pixels, however, I use drawing_8_pixels() to drawing,
  //     then I drawing 2 unnecessary pixels.
  //     And because of the load/store to memory is slow, thus I want to minimize its uses.
#define MMX_DRAW_1_PIXELS \
      MMX_DRAW_1_2_PIXELS_PROLOGUE \
      MMX_DRAW_PIXELS_EPILOGUE
  
#define MMX_DRAW_2_PIXELS \
      MMX_DRAW_1_2_PIXELS_PROLOGUE \
      \
      MMX_STORE_PIXEL(mm0) \
      \
      MMX_DRAW_PIXELS_EPILOGUE
  
#define MMX_DRAW_3_PIXELS \
      MMX_DRAW_PIXELS_PROLOGUE \
      \
      MMX_STORE_PIXEL(mm0) \
      \
      "subl %6, %%edi \n\t" \
      "maskmovq %%mm2, %%mm1 \n\t" \
      \
      MMX_DRAW_PIXELS_EPILOGUE
  
#define MMX_DRAW_4_PIXELS \
      MMX_DRAW_PIXELS_PROLOGUE \
      \
      MMX_STORE_PIXEL(mm0) \
      \
      "subl %6, %%edi \n\t" \
      "maskmovq %%mm2, %%mm1 \n\t" \
      \
      MMX_STORE_PIXEL(mm1) \
      \
      MMX_DRAW_PIXELS_EPILOGUE
  
#define SSE2_DRAW_5_PIXELS \
      SSE2_DRAW_PIXELS_PROLOGUE \
      \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      \
      "subl %6, %%edi \n\t" \
      "maskmovdqu %%xmm2, %%xmm1 \n\t" \
      \
      SSE2_DRAW_PIXELS_EPILOGUE
  
#define SSE2_DRAW_6_PIXELS \
      SSE2_DRAW_PIXELS_PROLOGUE \
      \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      \
      "subl %6, %%edi \n\t" \
      "maskmovdqu %%xmm2, %%xmm1 \n\t" \
      \
      SSE2_STORE_PIXEL(xmm1) \
      \
      SSE2_DRAW_PIXELS_EPILOGUE
  
#define SSE2_DRAW_7_PIXELS \
      SSE2_DRAW_PIXELS_PROLOGUE \
      \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      \
      "subl %6, %%edi \n\t" \
      "maskmovdqu %%xmm2, %%xmm1 \n\t" \
      \
      SSE2_STORE_PIXEL(xmm1) \
      SSE2_STORE_PIXEL(xmm1) \
      \
      SSE2_DRAW_PIXELS_EPILOGUE
  
#define SSE2_DRAW_8_PIXELS \
      SSE2_DRAW_PIXELS_PROLOGUE \
      \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      SSE2_STORE_PIXEL(xmm0) \
      \
      "subl %6, %%edi \n\t" \
      "maskmovdqu %%xmm2, %%xmm1 \n\t" \
      \
      SSE2_STORE_PIXEL(xmm1) \
      SSE2_STORE_PIXEL(xmm1) \
      SSE2_STORE_PIXEL(xmm1) \
      \
      SSE2_DRAW_PIXELS_EPILOGUE
  
  inline void
  MachineScreen::drawing_1_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    MMX_DRAW_1_PIXELS;
    
    curr_addr += (1 * BYTES_PER_PIXEL);
  }
  
  inline void
  MachineScreen::drawing_2_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    MMX_DRAW_2_PIXELS;
    
    curr_addr += (2 * BYTES_PER_PIXEL);
  }
  
  inline void
  MachineScreen::drawing_3_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    MMX_DRAW_3_PIXELS;
    
    curr_addr += (3 * BYTES_PER_PIXEL);
  }
  
  inline void
  MachineScreen::drawing_4_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    MMX_DRAW_4_PIXELS;
    
    curr_addr += (4 * BYTES_PER_PIXEL);
  }
  
  inline void
  MachineScreen::drawing_5_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    SSE2_DRAW_5_PIXELS;
    
    curr_addr += (5 * BYTES_PER_PIXEL);
  }
  
  inline void
  MachineScreen::drawing_6_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    SSE2_DRAW_6_PIXELS;
    
    curr_addr += (6 * BYTES_PER_PIXEL);
  }
  
  inline void
  MachineScreen::drawing_7_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    SSE2_DRAW_7_PIXELS;
    
    curr_addr += (7 * BYTES_PER_PIXEL);
  }
  
  inline void
  MachineScreen::drawing_8_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    SSE2_DRAW_8_PIXELS;
    
    curr_addr += (8 * BYTES_PER_PIXEL);
  }
  
#else // ENABLE_DRAWING_ASM
  
  inline void
  MachineScreen::drawing_1_pixel(uint32_t &curr_addr, uint8_t *&dest, uint32_t const row_length) const
  {
    uint16_t const pixel = *reinterpret_cast<uint16_t *>(&(mp_memory[curr_addr]));
    
    curr_addr += BYTES_PER_PIXEL;
    
    *reinterpret_cast<uint32_t *>(dest) = (0xFF000000 |
                                           mRedTable[(pixel >> RED_OFFSET) /* & RED_MAX_VALUE */] |
                                           mGreenTable[(pixel >> GREEN_OFFSET) & GREEN_MAX_VALUE] |
                                           mBlueTable[(pixel >> BLUE_OFFSET) & BLUE_MAX_VALUE]
                                           );
    
    dest -= row_length;
  }
  
#endif // ENABLE_DRAWING_ASM
  
  template<MachineScreen::PanelEnum T_panel, MachineScreen::DualPanelEnum T_dual>
  inline void
  MachineScreen::draw_panel()
  {
#if TRACE_DRAW_SCREEN
    g_log_file << "SCREEN: drawing: ";
#endif
    
    assert(mp_memory != 0);
    
    if (false == dirty_rect_in_this_panel<T_panel, T_dual>())
    {
      return;
    }
    
    uint32_t const machine_bpl = DISPLAY_HEIGHT * BYTES_PER_PIXEL; // bytes per line
    
    uint32_t const start_addr = (get_panel_start_address<T_panel>() +
                                 (DISPLAY_HEIGHT * BYTES_PER_PIXEL * get_x_diff<T_panel, T_dual>()) +
                                 (((DISPLAY_HEIGHT - m_dirty_bottom)) * BYTES_PER_PIXEL));
    
    uint32_t prev_addr = start_addr;
    
    guint8 * const pixbuf_startaddress = get_pixbuf_start_address<T_panel, T_dual>();
    uint32_t const row_length = mp_pixbuf->get_rowstride();
    
    uint8_t *dest_base = pixbuf_startaddress + ((m_dirty_bottom - 1) * row_length) + (m_dirty_left << 2);
    
    uint32_t curr_addr;
    uint8_t *dest;
    
#if ENABLE_DRAWING_ASM
    uint32_t const height = (m_dirty_bottom - m_dirty_top);
    
    uint32_t const p8_count = (height >> 3);
    uint32_t const p1_count = (height & (8 - 1));
#endif
    
    for (uint32_t x = get_drawing_left<T_panel, T_dual>();
         x < get_drawing_right<T_panel, T_dual>();
         ++x)
    {
      curr_addr = prev_addr;
      dest = dest_base;
      
      prev_addr = curr_addr;
      
#if ENABLE_DRAWING_ASM
      for (uint32_t c8 = 0; c8 < p8_count; ++c8)
      {
        drawing_8_pixel(curr_addr, dest, row_length);
      }
      
      switch (p1_count)
      {
      case 0: break;
      case 1: drawing_1_pixel(curr_addr, dest, row_length); break;
      case 2: drawing_2_pixel(curr_addr, dest, row_length); break;
      case 3: drawing_3_pixel(curr_addr, dest, row_length); break;
      case 4: drawing_4_pixel(curr_addr, dest, row_length); break;
      case 5: drawing_5_pixel(curr_addr, dest, row_length); break;
      case 6: drawing_6_pixel(curr_addr, dest, row_length); break;
      case 7: drawing_7_pixel(curr_addr, dest, row_length); break;
      default: assert(!"Should not reach here."); break;
      }
#else
      for (uint32_t y = m_dirty_top; y < m_dirty_bottom; ++y)
      {
        drawing_1_pixel(curr_addr, dest, row_length);
      }
#endif
      
      dest_base += 4;
      prev_addr += machine_bpl;
    }
    
#if ENABLE_DRAWING_ASM
    asm volatile ("emms");
#endif
    
#if TRACE_DRAW_SCREEN
    g_log_file << std::endl;
#endif
    
    mp_pixmap->draw_pixbuf(mp_empty_gc,
                           mp_pixbuf,
                           m_dirty_left, m_dirty_top, // src
                           m_dirty_left, m_dirty_top, // dest
                           static_cast<int>(m_dirty_right - m_dirty_left),
                           static_cast<int>(m_dirty_bottom - m_dirty_top),
                           Gdk::RGB_DITHER_NONE,
                           0, 0 // dither offset
                           );
    
    // Force X Window Server to send an expose event to the m_drawing_area.
    m_drawing_area.get_window()->clear_area_e(m_dirty_left,
                                              m_dirty_top,
                                              m_dirty_right - m_dirty_left,
                                              m_dirty_bottom - m_dirty_top);
  }
  
  //============================ Signal handler ================================
  
  void
  MachineScreen::on_drawing_area_size_allocate(Gtk::Allocation &alloc)
  {
    if (false == mp_pixmap)
    {
      mp_pixmap.clear();
    }
    
    if (false == mp_pixbuf)
    {
      mp_pixbuf.clear();
    }
    
    init_pixmap_and_pixbuf(alloc.get_width(), alloc.get_height());
  }
  
  void
  MachineScreen::on_drawing_area_expose_event(GdkEventExpose *event)
  {
#if TRACE_DRAW_SCREEN
    g_log_file << "SCREEN: expose (" << std::dec
              << event->area.x << ", "
              << event->area.y << ", "
              << event->area.width << ", "
              << event->area.height << ")" << std::endl;
#endif
    
    m_drawing_area.get_window()->draw_drawable(m_drawing_area.get_style()->get_bg_gc(Gtk::STATE_NORMAL),
                                               mp_pixmap,
                                               event->area.x,
                                               event->area.y,
                                               event->area.x,
                                               event->area.y,
                                               event->area.width,
                                               event->area.height);
  }
  
  void
  MachineScreen::on_drawing_area_button_press_event(GdkEventButton *event)
  {
    assert(mp_atmel != 0);
    
    // :NOTE: Wei 2004-Jul-06:
    //
    // Ignore Gdk::DOUBLE_BUTTON_PRESS & Gdk::TRIPLE_BUTTON_PRESS
    if (GDK_BUTTON_PRESS == event->type)
    {
      mp_atmel->receive_touch_panel_pen_down_event(event);
    }
  }
  
  void
  MachineScreen::on_drawing_area_button_release_event(GdkEventButton *event)
  {
    assert(mp_atmel != 0);
    
    mp_atmel->receive_touch_panel_pen_up_event(event);
  }
  
  void
  MachineScreen::on_drawing_area_motion_notify_event(GdkEventMotion *event)
  {
    assert(mp_atmel != 0);
    
    assert(Gdk::BUTTON1_MASK == (event->state & Gdk::BUTTON1_MASK));
    
    mp_atmel->receive_touch_panel_pen_move_event(event);
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Life cycle =================================
  
  MachineScreen::MachineScreen()
    : m_table(1, 1, true),
      m_lcd_status(0),
      mp_memory(0),
      mp_LCD_controller(0),
      mp_atmel(0)
  {
    set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    
    add(m_table);
    m_table.attach(m_drawing_area, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 0, 0);
    
    m_drawing_area.set_double_buffered(false);
    
    m_drawing_area.signal_size_allocate().
      connect(sigc::mem_fun(*this, &MachineScreen::on_drawing_area_size_allocate));
    
    m_drawing_area.signal_expose_event().
      connect_notify(sigc::mem_fun(*this, &MachineScreen::on_drawing_area_expose_event));
    
    show_all_children();
    
    clear_dirty_rect();
    
#if ENABLE_DRAWING_ASM
    m_red_bitmask.m_low = 0xF800F800F800F800LL;
    m_red_bitmask.m_high = m_red_bitmask.m_low;
    
    m_green_bitmask.m_low = 0x07E007E007E007E0LL;
    m_green_bitmask.m_high = m_green_bitmask.m_low;
    
    m_blue_bitmask.m_low = 0x001F001F001F001FLL;
    m_blue_bitmask.m_high = m_blue_bitmask.m_low;
    
    m_alpha_value.m_low = 0xFFFFFFFFFFFFFFFFLL;
    m_alpha_value.m_high = 0;
#endif
  }
  
  MachineScreen::~MachineScreen()
  {
    if (false == mp_pixmap)
    {
      mp_pixmap.clear();
    }
    
    if (false == mp_pixbuf)
    {
      mp_pixbuf.clear();
    }
  }
  
  //============================= Operation ===================================
  
  void
  MachineScreen::register_atmel_micro(AtmelMicro * const atmel)
  {
    mp_atmel = atmel;
    
    m_drawing_area.add_events(Gdk::BUTTON_PRESS_MASK);
    m_drawing_area.add_events(Gdk::BUTTON_RELEASE_MASK);
    m_drawing_area.add_events(Gdk::BUTTON1_MOTION_MASK);
    
    m_drawing_area.signal_button_press_event().
      connect_notify(sigc::mem_fun(*this, &MachineScreen::on_drawing_area_button_press_event));
    
    m_drawing_area.signal_button_release_event().
      connect_notify(sigc::mem_fun(*this, &MachineScreen::on_drawing_area_button_release_event));
    
    m_drawing_area.signal_motion_notify_event().
      connect_notify(sigc::mem_fun(*this, &MachineScreen::on_drawing_area_motion_notify_event));
  }
  
  void
  MachineScreen::get_and_store_next_armware_event()
  {
    Gtk::Main::iteration(false);
  }
  
  void
  MachineScreen::change_screen_size(uint32_t const width, uint32_t const height)
  {
    m_drawing_area.set_size_request(width, height);
  }
  
  bool
  MachineScreen::draw_screen()
  {
    switch (m_lcd_status)
    {
    case 0:
    case DUAL_PANEL:
      // disable drawing
      break;
      
    case ENABLE_DRAWING:
      // single panel
      if (true == has_dirty_rect())
      {
        draw_panel<PANEL_1, SINGLE>();
        
        clear_dirty_rect();
      }
      
      mp_LCD_controller->finish_one_frame();
      break;
      
    case (ENABLE_DRAWING | DUAL_PANEL):
      if (true == has_dirty_rect())
      {
        draw_panel<PANEL_1, DUAL>();
        draw_panel<PANEL_2, DUAL>();
        
        clear_dirty_rect();
      }
      
      mp_LCD_controller->finish_one_frame();
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    // :NOTE: Wei 2004-Jun-29:
    //
    // Return true so the function will be called again;
    // returning false removes this timeout function.
    return true;
  }
}
