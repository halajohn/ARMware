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

#include <iostream>

#include "AddressSpace.hpp"
#include "IntController.hpp"
#include "GPIOControlBlock.hpp"
#include "Serial_1.hpp"
#include "Serial_2.hpp"
#include "Serial_3.hpp"
#include "RTC.hpp"
#include "PPC.hpp"
#include "Egpio.hpp"
#include "Memory.hpp"
#include "RomFile.hpp"
#include "ResetController.hpp"
#include "PowerManager.hpp"
#include "MemoryConf.hpp"
#include "LCDController.hpp"
#include "OsTimer.hpp"
#include "MachineType.hpp"

#include "Egpio.tcc"
#include "LCDController.tcc"
#include "RomFile.tcc"
#include "Uart.tcc"
#include "Serial_1.tcc"
#include "Serial_2.tcc"

namespace ARMware
{
  template<typename T_return>
  T_return
  AddressSpace::get_data(uint32_t const address, bool &exception_occur) const
  {
    switch (address & 0xFF000000)
    {
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 0 (128 MB)
    case 0x00000000:
      // :H3600: Wei 2003-Dec-07:
      //
      // onboard flash ROM (16 MB)
      return mpRomFile->get_data<T_return>(address);
      
    case 0x01000000:
    case 0x02000000:
    case 0x03000000:
    case 0x04000000:
    case 0x05000000:
    case 0x06000000:
    case 0x07000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 1 (128 MB)
    case 0x08000000:
    case 0x09000000:
    case 0x0A000000:
    case 0x0B000000:
    case 0x0C000000:
    case 0x0D000000:
    case 0x0E000000:
    case 0x0F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 2 (128 MB)
    case 0x10000000:
    case 0x11000000:
    case 0x12000000:
    case 0x13000000:
    case 0x14000000:
    case 0x15000000:
    case 0x16000000:
    case 0x17000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 3 (128 MB)
    case 0x18000000:
    case 0x19000000:
    case 0x1A000000:
    case 0x1B000000:
    case 0x1C000000:
    case 0x1D000000:
    case 0x1E000000:
    case 0x1F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // PCMCIA Socket 0 (256 MB)
    case 0x20000000:
    case 0x21000000:
    case 0x22000000:
    case 0x23000000:
    case 0x24000000:
    case 0x25000000:
    case 0x26000000:
    case 0x27000000:
    case 0x28000000:
    case 0x29000000:
    case 0x2A000000:
    case 0x2B000000:
    case 0x2C000000:
    case 0x2D000000:
    case 0x2E000000:
    case 0x2F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // PCMCIA Socket 1 (256 MB)
    case 0x30000000:
    case 0x31000000:
    case 0x32000000:
    case 0x33000000:
    case 0x34000000:
    case 0x35000000:
    case 0x36000000:
    case 0x37000000:
    case 0x38000000:
    case 0x39000000:
    case 0x3A000000:
    case 0x3B000000:
    case 0x3C000000:
    case 0x3D000000:
    case 0x3E000000:
    case 0x3F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 4 (128 MB)
    case 0x40000000:
    case 0x41000000:
    case 0x42000000:
    case 0x43000000:
    case 0x44000000:
    case 0x45000000:
    case 0x46000000:
    case 0x47000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 5 (128 MB)
    case 0x48000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Reads from nonexistent memory are not detected in hardware.
      // In case no memory is selected on a read,
      // the value last driven on the data bus is returned.
      
      // :NOTE: Wei 2003-Dec-07:
      //
      // However, due to simplicity, I return 0 in this situation.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << "Get data from unknown address: " << std::hex << address << std::endl;
#endif
      return 0;
      
    case 0x49000000:
#if CONFIG_MACH_H3600
      assert(Egpio::EGPIO_ADDRESS == address);
#endif
      return mpEgpio->get_data<T_return>(address);
      
    case 0x4A000000:
    case 0x4B000000:
    case 0x4C000000:
    case 0x4D000000:
    case 0x4E000000:
    case 0x4F000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Reads from nonexistent memory are not detected in hardware.
      // In case no memory is selected on a read,
      // the value last driven on the data bus is returned.
      
      // :NOTE: Wei 2003-Dec-07:
      //
      // However, due to simplicity, I return 0 in this situation.
      return 0;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Reserved (768 MB)
    case 0x50000000:
    case 0x51000000:
    case 0x52000000:
    case 0x53000000:
    case 0x54000000:
    case 0x55000000:
    case 0x56000000:
    case 0x57000000:
    case 0x58000000:
    case 0x59000000:
    case 0x5A000000:
    case 0x5B000000:
    case 0x5C000000:
    case 0x5D000000:
    case 0x5E000000:
    case 0x5F000000:
    case 0x60000000:
    case 0x61000000:
    case 0x62000000:
    case 0x63000000:
    case 0x64000000:
    case 0x65000000:
    case 0x66000000:
    case 0x67000000:
    case 0x68000000:
    case 0x69000000:
    case 0x6A000000:
    case 0x6B000000:
    case 0x6C000000:
    case 0x6D000000:
    case 0x6E000000:
    case 0x6F000000:
    case 0x70000000:
    case 0x71000000:
    case 0x72000000:
    case 0x73000000:
    case 0x74000000:
    case 0x75000000:
    case 0x76000000:
    case 0x77000000:
    case 0x78000000:
    case 0x79000000:
    case 0x7A000000:
    case 0x7B000000:
    case 0x7C000000:
    case 0x7D000000:
    case 0x7E000000:
    case 0x7F000000:
      // :SA-1110 Developer's Manual: p.124: Wei 2003-Dec-07
      //
      // Reads from reserved address locations will result in a data abort exception.
      exception_occur = true;
      return 0;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Peripheral Control Module Registers (256 MB)
    case 0x80000000:
      switch (address & 0xFFFF0000)
      {
      case 0x80010000:
      case 0x80020000:
        return mpSerial_1->get_data<T_return>(address);
        
      case 0x80030000:
        return mpSerial_2->get_data<T_return>(address);
        
      case 0x80050000:
        return mpSerial_3->get_data<T_return>(address);
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x81000000:
    case 0x82000000:
    case 0x83000000:
    case 0x84000000:
    case 0x85000000:
    case 0x86000000:
    case 0x87000000:
    case 0x88000000:
    case 0x89000000:
    case 0x8A000000:
    case 0x8B000000:
    case 0x8C000000:
    case 0x8D000000:
    case 0x8E000000:
    case 0x8F000000:
      assert(!"Should not reach here.");
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // System Control Module Registers (256 MB)
    case 0x90000000:
      switch (address & 0xFFFF0000)
      {
      case 0x90000000:
        return mpOsTimer->get_data<T_return>(address);
        
      case 0x90010000:
        return mpRTC->get_data<T_return>(address);
        
      case 0x90020000:
        return mpPowerManager->get_data<T_return>(address);
        
      case 0x90030000:
        return mpResetController->get_data<T_return>(address);
        
      case 0x90040000:
        return mp_GPIO_control_block->get_data<T_return>(address);
        
      case 0x90050000:
        return mp_int_controller->get_data<T_return>(address);
        
      case 0x90060000:
        return mpPPC->get_data<T_return>(address);
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x91000000:
    case 0x92000000:
    case 0x93000000:
    case 0x94000000:
    case 0x95000000:
    case 0x96000000:
    case 0x97000000:
    case 0x98000000:
    case 0x99000000:
    case 0x9A000000:
    case 0x9B000000:
    case 0x9C000000:
    case 0x9D000000:
    case 0x9E000000:
    case 0x9F000000:
      assert(!"Should not reach here.");
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Memory and Expansion Registers (256 MB)
    case 0xA0000000:
      switch (address & 0xFFFF0000)
      {
      case 0xA0000000:
        return mp_memory_conf->get_data<T_return>(address);
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0xA1000000:
    case 0xA2000000:
    case 0xA3000000:
    case 0xA4000000:
    case 0xA5000000:
    case 0xA6000000:
    case 0xA7000000:
    case 0xA8000000:
    case 0xA9000000:
    case 0xAA000000:
    case 0xAB000000:
    case 0xAC000000:
    case 0xAD000000:
    case 0xAE000000:
    case 0xAF000000:
      assert(!"Should not reach here.");
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // LCD and DMA Registers (256 MB)
    case 0xB0000000:
      switch (address & 0x100000)
      {
      case 0:
        // :NOTE: Wei 2004-Jan-13:
        //
        // DMA Controller
        assert(!"Should not reach here.");
        break;
        
      case 0x100000:
        // :NOTE: Wei 2004-Jan-13:
        //
        // LCD Controller
        return mpLCDController->get_data<T_return>(address);
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0xB1000000:
    case 0xB2000000:
    case 0xB3000000:
    case 0xB4000000:
    case 0xB5000000:
    case 0xB6000000:
    case 0xB7000000:
    case 0xB8000000:
    case 0xB9000000:
    case 0xBA000000:
    case 0xBB000000:
    case 0xBC000000:
    case 0xBD000000:
    case 0xBE000000:
    case 0xBF000000:
      assert(!"Should not reach here.");
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 0 (128 MB)
    case 0xC0000000:
    case 0xC1000000:
    case 0xC2000000:
    case 0xC3000000:
    case 0xC4000000:
    case 0xC5000000:
    case 0xC6000000:
    case 0xC7000000:
      // :NOTE: Wei 2004-Apr-24:
      //
      // The reason why I wrote the following code:
      // Take an example, say H3600, it has a 32MB dram entity, and a 128MB dram bank0.
      // Althought StrongARM may issue an physical address between 32MB ~ 128MB
      // into the physical address bus, the outside dram module may or may not response
      // a data abort signal to StrongARM.
      //
      // Some outside dram module just ignore several most significent digits for simplicitly,
      // so that when StrongARM issue an physical address between 32MB ~ 128MB,
      // the outside dram module will make it access to the 0 ~ 32MB memory space.
      return mp_memory->get_data<T_return>((address & (MachineType::DRAM_BASE +
                                                       MachineType::DRAM_SIZE)) -
                                           MachineType::DRAM_BASE);
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 1 (128 MB)
    case 0xC8000000:
    case 0xC9000000:
    case 0xCA000000:
    case 0xCB000000:
    case 0xCC000000:
    case 0xCD000000:
    case 0xCE000000:
    case 0xCF000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 2 (128 MB)
    case 0xD0000000:
    case 0xD1000000:
    case 0xD2000000:
    case 0xD3000000:
    case 0xD4000000:
    case 0xD5000000:
    case 0xD6000000:
    case 0xD7000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 3 (128 MB)
    case 0xD8000000:
    case 0xD9000000:
    case 0xDA000000:
    case 0xDB000000:
    case 0xDC000000:
    case 0xDD000000:
    case 0xDE000000:
    case 0xDF000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Reads from nonexistent memory are not detected in hardware.
      // In case no memory is selected on a read,
      // the value last driven on the data bus is returned.
      
      // :NOTE: Wei 2003-Dec-07:
      //
      // However, due to simplicity, I return 0 in this situation.
      return 0;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Zero Bank (128 MB)
    case 0xE0000000:
    case 0xE1000000:
    case 0xE2000000:
    case 0xE3000000:
    case 0xE4000000:
    case 0xE5000000:
    case 0xE6000000:
    case 0xE7000000:
      return 0;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Reserved (384 MB)
    case 0xE8000000:
    case 0xE9000000:
    case 0xEA000000:
    case 0xEB000000:
    case 0xEC000000:
    case 0xED000000:
    case 0xEE000000:
    case 0xEF000000:
    case 0xF0000000:
    case 0xF1000000:
    case 0xF2000000:
    case 0xF3000000:
    case 0xF4000000:
    case 0xF5000000:
    case 0xF6000000:
    case 0xF7000000:
    case 0xF8000000:
    case 0xF9000000:
    case 0xFA000000:
    case 0xFB000000:
    case 0xFC000000:
    case 0xFD000000:
    case 0xFE000000:
    case 0xFF000000:
      // :SA-1110 Developer's Manual: p.124: Wei 2003-Dec-07
      //
      // Reads from reserved address locations will result in a data abort exception.
      exception_occur = true;
      return 0;
    }
    
    assert(!"Should not reach here.");
    return 0;
  }
  
  template<typename T_store>
  void
  AddressSpace::put_data(uint32_t const address, T_store value)
  {
    switch (address & 0xFF000000)
    {
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 0 (128 MB)
    case 0x00000000:
      // :H3600: Wei 2003-Dec-07:
      //
      // onboard flash ROM (16 MB)
      mpRomFile->put_data(address, value);
      break;
      
    case 0x01000000:
    case 0x02000000:
    case 0x03000000:
    case 0x04000000:
    case 0x05000000:
    case 0x06000000:
    case 0x07000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 1 (128 MB)
    case 0x08000000:
    case 0x09000000:
    case 0x0A000000:
    case 0x0B000000:
    case 0x0C000000:
    case 0x0D000000:
    case 0x0E000000:
    case 0x0F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 2 (128 MB)
    case 0x10000000:
    case 0x11000000:
    case 0x12000000:
    case 0x13000000:
    case 0x14000000:
    case 0x15000000:
    case 0x16000000:
    case 0x17000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 3 (128 MB)
    case 0x18000000:
    case 0x19000000:
    case 0x1A000000:
    case 0x1B000000:
    case 0x1C000000:
    case 0x1D000000:
    case 0x1E000000:
    case 0x1F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // PCMCIA Socket 0 (256 MB)
    case 0x20000000:
    case 0x21000000:
    case 0x22000000:
    case 0x23000000:
    case 0x24000000:
    case 0x25000000:
    case 0x26000000:
    case 0x27000000:
    case 0x28000000:
    case 0x29000000:
    case 0x2A000000:
    case 0x2B000000:
    case 0x2C000000:
    case 0x2D000000:
    case 0x2E000000:
    case 0x2F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // PCMCIA Socket 1 (256 MB)
    case 0x30000000:
    case 0x31000000:
    case 0x32000000:
    case 0x33000000:
    case 0x34000000:
    case 0x35000000:
    case 0x36000000:
    case 0x37000000:
    case 0x38000000:
    case 0x39000000:
    case 0x3A000000:
    case 0x3B000000:
    case 0x3C000000:
    case 0x3D000000:
    case 0x3E000000:
    case 0x3F000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 4 (128 MB)
    case 0x40000000:
    case 0x41000000:
    case 0x42000000:
    case 0x43000000:
    case 0x44000000:
    case 0x45000000:
    case 0x46000000:
    case 0x47000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Static Bank 5 (128 MB)
    case 0x48000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Writes to nonexistent memory are not detected in hardware.
      
      // :NOTE: Wei 2004-Jan-12:
      //
      // familiar bootldr will check 0x4000076C
      // (the DPDR register's address of HP Jornada),
      // so that I have to let it pass, and don't use assert() here.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << "Put data to unknown address: " << std::hex << address << std::endl;
#endif
      break;
      
    case 0x49000000:
#if CONFIG_MACH_H3600
      assert(Egpio::EGPIO_ADDRESS == address);
#endif
      
      mpEgpio->put_data(address, value);
      break;
      
    case 0x4A000000:
    case 0x4B000000:
    case 0x4C000000:
    case 0x4D000000:
    case 0x4E000000:
    case 0x4F000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Writes to nonexistent memory are not detected in hardware.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
#endif
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Reserved (768 MB)
    case 0x50000000:
    case 0x51000000:
    case 0x52000000:
    case 0x53000000:
    case 0x54000000:
    case 0x55000000:
    case 0x56000000:
    case 0x57000000:
    case 0x58000000:
    case 0x59000000:
    case 0x5A000000:
    case 0x5B000000:
    case 0x5C000000:
    case 0x5D000000:
    case 0x5E000000:
    case 0x5F000000:
    case 0x60000000:
    case 0x61000000:
    case 0x62000000:
    case 0x63000000:
    case 0x64000000:
    case 0x65000000:
    case 0x66000000:
    case 0x67000000:
    case 0x68000000:
    case 0x69000000:
    case 0x6A000000:
    case 0x6B000000:
    case 0x6C000000:
    case 0x6D000000:
    case 0x6E000000:
    case 0x6F000000:
    case 0x70000000:
    case 0x71000000:
    case 0x72000000:
    case 0x73000000:
    case 0x74000000:
    case 0x75000000:
    case 0x76000000:
    case 0x77000000:
    case 0x78000000:
    case 0x79000000:
    case 0x7A000000:
    case 0x7B000000:
    case 0x7C000000:
    case 0x7D000000:
    case 0x7E000000:
    case 0x7F000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07"
      //
      // Writes to reserved address space will have no effect.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
#endif
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Peripheral Control Module Registers (256 MB)
    case 0x80000000:
      switch (address & 0xFFFF0000)
      {
      case 0x80010000:
      case 0x80020000:
        mpSerial_1->put_data(address, value);
        break;
        
      case 0x80030000:
        mpSerial_2->put_data(address, value);
        break;
        
      case 0x80050000:
        mpSerial_3->put_data(address, value);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x81000000:
    case 0x82000000:
    case 0x83000000:
    case 0x84000000:
    case 0x85000000:
    case 0x86000000:
    case 0x87000000:
    case 0x88000000:
    case 0x89000000:
    case 0x8A000000:
    case 0x8B000000:
    case 0x8C000000:
    case 0x8D000000:
    case 0x8E000000:
    case 0x8F000000:
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
      assert(!"Should not reach here.");
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // System Control Module Registers (256 MB)
    case 0x90000000:
      switch (address & 0xFFFF0000)
      {
      case 0x90000000:
        mpOsTimer->put_data(address, value);
        break;
        
      case 0x90010000:
        mpRTC->put_data(address, value);
        break;
        
      case 0x90020000:
        mpPowerManager->put_data(address, value);
        break;
        
      case 0x90030000:
        mpResetController->put_data(address, value);
        break;
        
      case 0x90040000:
        mp_GPIO_control_block->put_data(address, value);
        break;
        
      case 0x90050000:
        mp_int_controller->put_data(address, value);
        break;
        
      case 0x90060000:
        mpPPC->put_data(address, value);
        break;
        
      default:
        g_log_file << std::hex << "address = " << address << std::endl;
        g_log_file << std::hex << "value = " << value << std::endl;
        
#if ENABLE_INST_RECALL
        mp_core->DumpInstRecallBuffer();
#endif
        
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x91000000:
    case 0x92000000:
    case 0x93000000:
    case 0x94000000:
    case 0x95000000:
    case 0x96000000:
    case 0x97000000:
    case 0x98000000:
    case 0x99000000:
    case 0x9A000000:
    case 0x9B000000:
    case 0x9C000000:
    case 0x9D000000:
    case 0x9E000000:
    case 0x9F000000:
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
      assert(!"Should not reach here.");
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Memory and Expansion Registers (256 MB)
    case 0xA0000000:
      switch (address & 0xFFFF0000)
      {
      case 0xA0000000:
        mp_memory_conf->put_data(address, value);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0xA1000000:
    case 0xA2000000:
    case 0xA3000000:
    case 0xA4000000:
    case 0xA5000000:
    case 0xA6000000:
    case 0xA7000000:
    case 0xA8000000:
    case 0xA9000000:
    case 0xAA000000:
    case 0xAB000000:
    case 0xAC000000:
    case 0xAD000000:
    case 0xAE000000:
    case 0xAF000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Writes to nonexistent memory are not detected in hardware.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
#endif
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // LCD and DMA Registers (256 MB)
    case 0xB0000000:
      switch (address & 0x100000)
      {
      case 0:
        // :NOTE: Wei 2004-Jan-13:
        //
        // DMA Controller
        assert(!"Should not reach here.");
        break;
        
      case 0x100000:
        // :NOTE: Wei 2004-Jan-13:
        //
        // LCD Controller
        mpLCDController->put_data(address, value);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0xB1000000:
    case 0xB2000000:
    case 0xB3000000:
    case 0xB4000000:
    case 0xB5000000:
    case 0xB6000000:
    case 0xB7000000:
    case 0xB8000000:
    case 0xB9000000:
    case 0xBA000000:
    case 0xBB000000:
    case 0xBC000000:
    case 0xBD000000:
    case 0xBE000000:
    case 0xBF000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Writes to nonexistent memory are not detected in hardware.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
#endif
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 0 (128 MB)
    case 0xC0000000:
    case 0xC1000000:
    case 0xC2000000:
    case 0xC3000000:
    case 0xC4000000:
    case 0xC5000000:
    case 0xC6000000:
    case 0xC7000000:
      // :NOTE: Wei 2004-Apr-24:
      //
      // The reason why I wrote the following code:
      // Take an example, say H3600, it has a 32MB dram entity, and a 128MB dram bank0.
      // Althought StrongARM may issue an physical address between 32MB ~ 128MB
      // into the physical address bus, the outside dram module may or may not response
      // a data abort signal to StrongARM.
      //
      // Some outside dram module just ignore several most significent digits for simplicitly,
      // so that when StrongARM issue an physical address between 32MB ~ 128MB,
      // the outside dram module will make it access to the 0 ~ 32MB memory space.
      mp_memory->put_data((address & (MachineType::DRAM_BASE + MachineType::DRAM_SIZE)) -
                          MachineType::DRAM_BASE, value);
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 1 (128 MB)
    case 0xC8000000:
    case 0xC9000000:
    case 0xCA000000:
    case 0xCB000000:
    case 0xCC000000:
    case 0xCD000000:
    case 0xCE000000:
    case 0xCF000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 2 (128 MB)
    case 0xD0000000:
    case 0xD1000000:
    case 0xD2000000:
    case 0xD3000000:
    case 0xD4000000:
    case 0xD5000000:
    case 0xD6000000:
    case 0xD7000000:
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // DRAM Bank 3 (128 MB)
    case 0xD8000000:
    case 0xD9000000:
    case 0xDA000000:
    case 0xDB000000:
    case 0xDC000000:
    case 0xDD000000:
    case 0xDE000000:
    case 0xDF000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Writes to nonexistent memory are not detected in hardware.
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Zero Bank (128 MB)
    case 0xE0000000:
    case 0xE1000000:
    case 0xE2000000:
    case 0xE3000000:
    case 0xE4000000:
    case 0xE5000000:
    case 0xE6000000:
    case 0xE7000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Writes to this space have no effect.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
#endif
      break;
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07:
      //
      // Reserved (384 MB)
    case 0xE8000000:
    case 0xE9000000:
    case 0xEA000000:
    case 0xEB000000:
    case 0xEC000000:
    case 0xED000000:
    case 0xEE000000:
    case 0xEF000000:
    case 0xF0000000:
    case 0xF1000000:
    case 0xF2000000:
    case 0xF3000000:
    case 0xF4000000:
    case 0xF5000000:
    case 0xF6000000:
    case 0xF7000000:
    case 0xF8000000:
    case 0xF9000000:
    case 0xFA000000:
    case 0xFB000000:
    case 0xFC000000:
    case 0xFD000000:
    case 0xFE000000:
    case 0xFF000000:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-07"
      //
      // Writes to reserved address space will have no effect.
#if TRACE_UNKNOWN_MEMORY_ADDR
      g_log_file << std::hex << "address = " << address << std::endl;
      g_log_file << std::hex << "value = " << value << std::endl;
#endif
      break;
    }
  }
}
