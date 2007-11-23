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

#ifndef LCDController_hpp
#define LCDController_hpp

#include <cassert>

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Core;
  class Memory;
  class MachineScreen;
  
  typedef class LCDController LCDController;
  class LCDController
  {
  private:
    
    enum
    {
      LCCR0_LEN = (1 << 0), // LCD controller enable
      LCCR0_CMS = (1 << 1), // Color/monochrome select.
      LCCR0_SDS = (1 << 2), // Single-/dual-panel display select.
      LCCR0_LDM = (1 << 3), // LCD disable done mask
      LCCR0_BAM = (1 << 4), // Base address update interrupt mask
      LCCR0_ERM = (1 << 5), // Error mask.
      LCCR0_PAS = (1 << 7), // Passive/active display select
      LCCR0_BLE = (1 << 8), // Big/little endian select.
      LCCR0_DPD = (1 << 9), // Double-pixel data pin mode.
      LCCR0_VCS = ((1 << 10) | (1 << 11)), // Vertical slant line correction.
      LCCR0_PDD = ((1 << 12) | (1 << 13) |
                   (1 << 14) | (1 << 15) |
                   (1 << 16) | (1 << 17) |
                   (1 << 18) | (1 << 19)) // Palette DMA request delay.
    };
    
    enum
    {
      LCSR_LDD = (1 << 0), // LCD disable done status.
      LCSR_BAU = (1 << 1), // Base address update flag (read-only, maskable interrupt)
      LCSR_BER = (1 << 2), // Bus error status.
      LCSR_ABC = (1 << 3), // AC bias count status.
      
      // :NOTE: Wei 2004-Jun-06:
      //
      // I think in ARMware, we shouldn't raise the following bits.
      // As a result, no interrupts should be raised because of the following bits.
      
      LCSR_IOL = (1 <<  4), // Input FIFO overrun lower panel status.
      LCSR_IUL = (1 <<  5), // Input FIFO underrun lower panel status.
      LCSR_IOU = (1 <<  6), // Input FIFO overrun upper panel status.
      LCSR_IUU = (1 <<  7), // Input FIFO underrun upper panel status.
      LCSR_OOL = (1 <<  8), // Output FIFO overrun lower panel status.
      LCSR_OUL = (1 <<  9), // Output FIFO underrun lower panel status.
      LCSR_OOU = (1 << 10), // Output FIFO overrun upper panel status.
      LCSR_OUU = (1 << 11), // Output FIFO underrun upper panel status.
      
      // :NOTE: Wei 2004-Jun-30:
      //
      // Althought page 249 in the Intel SA-1110 Developer's Manual says that BAU flag is read-only,
      // however, in the same page (p.249), it says that BAU flag can be cleared when it is written
      // to a 1.
      // After searching google, I find a post related this:
      //
      // 寄件者：bratfi_t@my-deja.com (bratfi_t@my-deja.com)
      // 主旨：Re: BAU flag on SA1100
      // 網上論壇：comp.sys.arm
      // 日期：1999/08/31 
      //
      // ...
      // According to the manual, the BAU flag is read-only, but it can be cleared like a status bit!
      // ...
      //
      // Thus I include BAU in the following constant.
      LCSR_READ_WRITE_BITMASK = (LCSR_LDD |
                                 LCSR_BAU |
                                 LCSR_BER |
                                 LCSR_ABC |
                                 LCSR_IOL |
                                 LCSR_IUL |
                                 LCSR_IOU |
                                 LCSR_IUU |
                                 LCSR_OOL |
                                 // LCSR_OUL |
                                 LCSR_OOU |
                                 LCSR_OUU)
    };
    
    // Operation
    
    uint32_t CalculateFrameBufferEndAddress();
    
    // Attribute
    
    Core * const mp_core;
    Memory * const mp_memory;
    MachineScreen * const mp_machine_screen;
    
    // Register location
    
    enum
    {
      LCCR0 = 0xB0100000,
      LCSR  = 0xB0100004,
      DBAR1 = 0xB0100010,
      DCAR1 = 0xB0100014,
      DBAR2 = 0xB0100018,
      DCAR2 = 0xB010001C,
      LCCR1 = 0xB0100020,
      LCCR2 = 0xB0100024,
      LCCR3 = 0xB0100028
    };
    
    // Register
    
    uint32_t m_LCCR0; // LCD controller control register 0
    uint32_t m_LCSR;  // LCD controller status register 1
    uint32_t m_DBAR1; // DMA channel 1 base address register
    uint32_t m_DCAR1; // DMA channel 1 current address register
    uint32_t m_DBAR2; // DMA channel 2 base address register
    uint32_t m_DCAR2; // DMA channel 2 current address register
    uint32_t m_LCCR1; // LCD controller control register 1
    uint32_t m_LCCR2; // LCD controller control register 2
    uint32_t m_LCCR3; // LCD controller control register 3
    
    // Operation
    
    void
    init_register()
    {
      m_LCCR0 = 0;
      m_LCSR = 0;
      m_DBAR1 = 0;
      m_DCAR1 = 0;
      m_DBAR2 = 0;
      m_DCAR2 = 0;
      m_LCCR1 = 0;
      m_LCCR2 = 0;
      m_LCCR3 = 0;
    }
    
    // Friend
    
    friend class IntController;
    
  public:
    
    // Life cycle
    
    LCDController(Core * const core,
                  Memory * const memory,
                  MachineScreen * const machine_screen);
    
    // Operation
    
    void reset();
    
    inline void
    finish_one_frame()
    {
      assert(LCCR0_LEN == (m_LCCR0 & LCCR0_LEN));
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-08:
      //
      // Value in DBAR1(or 2) is transferred to DCAR1(or 2) when LCD is first enabled (LEN = 0->1)
      // and when the current address pointer value equals the end-of-frame buffer.
      
      // :NOTE: Wei 2004-Jan-15:
      //
      // Althought I don't update the m_DCAR1(or 2) when (m_DBAR1 == m_DCAR1) here,
      // however, they are actually transfered in the real SA-1110,
      // thus the following LCSR_BAU setting operation are always performed.
      
      // :SA-1110 Developer's Manual: p.249: Wei 2004-Jan-15:
      //
      // The base address update status (BAU) is a read/write status bit
      // that is set after the contents of the DMA base address register 1
      // are transferred to the DMA current Address register 1
      m_LCSR |= LCSR_BAU;
    }
    
    inline uint32_t
    get_interrupt_status() const
    {
      return m_LCSR;
    }
    
    inline uint32_t
    get_ctrl_reg_0() const
    {
      return m_LCCR0;
    }
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
}

#endif
