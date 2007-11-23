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

// The ARM core
//

#ifndef Core_hpp
#define Core_hpp

// System include
//

#include <cassert>
#include <memory>

// Project include
//

#include "ARMware.hpp"

// Local include
//

#include "Mmu.hpp"
#include "SysCoProc.hpp"
#include "Exception.hpp"
#include "Chunk.hpp"
#include "Global.hpp"

#include "Inst/Inst.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class SysCoProc;
  class Memory;
  class Egpio;
  
  class GPIOControlBlock;
  class RTC;
  class OsTimer;
  class IntController;
  
  class MemoryConf;
  class ResetController;
  class PPC;
  class Serial_1;
  class Serial_2;
  class Serial_3;
  class RomFile;
  class AddressSpace;
  class PowerManager;
  class Mmu;
  class LCDController;
  class MachineImpl;
  
  class MachineScreen;
  class SoftKeyboard;
  class SerialConsole;
  
#if ENABLE_THREADED_CODE
  class Chunk;
  class ChunkChain;
  class ARMInstInfo;
#endif
  
  typedef class Core Core;
  class Core
  {
  private:
        
    // Operation
    
    // :NOTE: Wei 2004-Jul-27:
    //
    // The return value of the following 4 functions means:
    // @true : no exception occurs, run sequentially.
    // @false: an exception occurs, need to handle it.
    ExecResultEnum exec();
    
    inline void
    ModifyFCSEVaddr(uint32_t &vaddr) const
    {
      // :SA-1110 Developer's Manual: p.61: Wei 2004-Apr-15:
      //
      // The SA-1110 supports the remapping of virtual addresses
      // through a process ID (PID) register. The 6-bit PID value
      // is OR ed with bits 30..25 of the virtual address when bits
      // 31..25 of the virtual address are zero. This effectively
      // remaps the address to one of 64 slots in the lower 2 Gbyte
      // address space.
      if (0 == (vaddr & (0xFE << 24)))
      {
        vaddr |= mpSysCoProc->get_reg_value(CPR_13);
      }
    }
    
  public:
    
    enum ExceptionType
    {
#if CHECK_EXCEPTION_RAISE
      EXCEPTION_NONE,
#endif
      EXCEPTION_SWI,      // Software interrupt
      EXCEPTION_UND,      // Undefined
      EXCEPTION_PREFETCH, // Prefetch abort
      EXCEPTION_DATA      // Data abort
    };
    typedef enum ExceptionType ExceptionType;
    
  private:
    
    // Attribute
    
    std::auto_ptr<SysCoProc>         mpSysCoProc;
    std::auto_ptr<Egpio>             mpEgpio;
    std::auto_ptr<Memory>            mp_memory;
    
    std::auto_ptr<GPIOControlBlock>  mp_GPIO_control_block;
    std::auto_ptr<OsTimer>           mpOsTimer;
    std::auto_ptr<RTC>               mpRTC;
    std::auto_ptr<LCDController>     mp_LCD_controller;
    
    std::auto_ptr<PPC>               mp_PPC;
    
    std::auto_ptr<Serial_1>          mp_serial_1;
    std::auto_ptr<Serial_2>          mp_serial_2;
    std::auto_ptr<Serial_3>          mp_serial_3;
    
    std::auto_ptr<IntController>     mpIntController;
    
    std::auto_ptr<MemoryConf>        mp_memory_conf;
    std::auto_ptr<PowerManager>      mpPowerManager;
    std::auto_ptr<ResetController>   mpResetController;
    std::auto_ptr<RomFile>           mpRomFile;
    std::auto_ptr<AddressSpace>      mpAddressSpace;
    std::auto_ptr<Mmu>               mpMmu;
    
    // :NOTE: Wei 2004-Jan-11:
    //
    // Used for serial port 3, and controlled by EGPIO pin 7.
    bool      m_RS232_is_enabled;
    
    CoreMode  m_curr_mode;
    bool      m_idle_mode;
    
    ExceptionType m_exception_type;
    
#if USE_SHIFTER_CARRY
    uint32_t  mShifterCarry;
#endif
    uint32_t  mCoreFreq;
    
#if CHECK_PC_MODIFY
    uint32_t  m_orig_pc;
    bool      m_pc_incremented;
#endif
    
    uint32_t m_curr_reg_bank[16]; // 16 elements
    
    uint32_t m_USR_reg_bank[16];  // only 7 elements are used
    uint32_t m_FIQ_reg_bank[16];  // only 7 elements are used
    uint32_t m_IRQ_reg_bank[16];  // only 2 elements are used
    uint32_t m_SVC_reg_bank[16];  // only 2 elements are used
    uint32_t m_ABT_reg_bank[16];  // only 2 elements are used
    uint32_t m_UND_reg_bank[16];  // only 2 elements are used
    
    uint32_t m_CPSR;
    
    uint32_t m_curr_SPSR;
    
    uint32_t m_FIQ_SPSR;
    uint32_t m_SVC_SPSR;
    uint32_t m_ABT_SPSR;
    uint32_t m_IRQ_SPSR;
    uint32_t m_UND_SPSR;
    
#if ENABLE_THREADED_CODE
    bool m_chunk_start;
    uint32_t m_chunk_length;
    bool m_chunk_disappear;
    
    uint32_t m_device_timer_increment;
    
    Chunk *mp_curr_chunk;
    Chunk *mp_delay_clean_chunk;
    
    std::auto_ptr<ChunkChain> mp_chunk_chain;
    
    enum ChunksLengthEnum
    {
      CL_0 = 0,
      CL_10, CL_20, CL_30, CL_40,
      CL_50, CL_60, CL_70, CL_80, CL_90,
      CL_100, CL_110, CL_120, CL_130, CL_140,
      CL_150, CL_160, CL_170, CL_180, CL_190,
      CL_OTHER,
      CL_TOTAL
    };
    typedef enum ChunksLengthEnum ChunksLengthEnum;
    
    uint32_t m_chunks_length[CL_TOTAL];
    
    // :NOTE: Wei 2004-Aug-21:
    //
    // This value have to be > Chunk::INITIAL_HIT_COUNT.
    static uint32_t const GENERATE_THREADED_CODE_THRESHOLD = 2;
#endif
    
#if ENABLE_DYNAMIC_TRANSLATOR
    static uint32_t const GENERATE_DT_BUFFER_THRESHOLD =
#if DEBUG_MODE
    3
#else
    3
#endif
    ;
    
    enum ExecInst
    {
      EXEC_INST_ALWAYS, // always execute the instruction
      EXEC_INST_PASS,   // don't execute the instruction
      EXEC_INST_COND    // execute the instruction depends on the condition test result.
    };
    typedef enum ExecInst ExecInst;
#endif
    
    // :NOTE: Wei 2004-Jun-09:
    //
    // The bps (bits per second) for serial port 3 is 115200
    // (i.e. 14400 bytes per second)
    //
    // However, 115200 seems too slow for ARMware,
    // Thus I increment this frequency.
    static uint32_t const UART_FREQ = (206000000 / 14400) / 100;
    uint32_t m_UART_clocks;
    
    static uint32_t const _3_6864_MHZ_FREQ = (206000000 / 3686400);
    uint32_t m_3_6864_mhz_clocks;
    
    // :NOTE: Wei 2004-Jul-8:
    //
    // I don't know which value of this GET_EVENT_FREQ should be.
    // Maybe I should think about it and do some tests.
    static uint32_t const GET_EVENT_FREQ = 2000;
    uint32_t m_get_event_clocks;
    
    MachineScreen * const mp_machine_screen;
    
#if ENABLE_INST_RECALL
    struct InstRecallEntry
    {
      uint32_t mIpVaddr;
      uint32_t mIpPaddr;
      uint32_t mInst;
      
      InstNameTag mInstNameTag;
      InstTypeTag mInstTypeTag;
      
      bool mUpdateStatusRegister;
      bool mExec;
      
      CoreRegNum mRdNum;
      CoreRegNum mRnNum;
      CoreRegNum mRmNum;
      CoreRegNum mRsNum;
      
      uint32_t mRd;
      uint32_t mRn;
      uint32_t mRm;
      uint32_t mRs;
      
      uint32_t mSftOperand;
      
      CoProcNum mCoProcNum;
      
      CoreRegNum mCurrentRegNum;
      
      bool mEnable[16];
      uint32_t mVaddr[16];
      uint32_t mPaddr[16];
      uint32_t mValue[16];
    };
    typedef struct InstRecallEntry InstRecallEntry;
    
    static uint32_t const INST_RECALL_BUFFER_SIZE = 32768;
    mutable InstRecallEntry mInstRecallBuffer[INST_RECALL_BUFFER_SIZE];
    uint32_t mInstRecallEntryIndex;
    
    inline InstRecallEntry &
    NewInstRecallEntry()
    {
      if ((INST_RECALL_BUFFER_SIZE - 1) == mInstRecallEntryIndex)
      {
        mInstRecallEntryIndex = 0;
        return mInstRecallBuffer[0];
      }
      else
      {
        return mInstRecallBuffer[++mInstRecallEntryIndex];
      }
    }
    
    void DumpInstRecallEntry(uint32_t const index) const;
#endif // #if ENABLE_INST_RECALL
    
#if ENABLE_THREADED_CODE
    void gen_threaded_code();
    ExecResultEnum exec_threaded_code();
    
    template<UsageEnum T_usage>
#endif
    ExecResultEnum decode(Inst const inst);
    
#if ENABLE_DYNAMIC_TRANSLATOR
    // :NOTE: Wei 2005-Mar-19:
    //
    // I think the size of the native codes a chunk can generated will not
    // greater than 0xFFFFF(==1048575).
    static uint32_t const MAX_VALUE_TO_RESET_THE_NATIVE_CODE_BUFFER_SIZE_CALCULATING = 0xFFF00000;
    
    uint32_t m_all_native_code_buffer_size;
    uint32_t m_all_compiled_arm_code_size;
    uint32_t m_average_native_code_buffer_size;
    
    void gen_dt_buffer();
    ExecResultEnum exec_dt_buffer();
#endif
    
#if PRINT_CHUNK_CODE
    bool m_exec_in_chunk;
#endif
    
  public:
    
    // Symbolic constant
    
    enum StatusFlags
    {
      NBIT_MASK = 0x80000000,
      ZBIT_MASK = 0x40000000,
      CBIT_MASK = 0x20000000,
      VBIT_MASK = 0x10000000,
      IBIT_MASK = 0x00000080,
      FBIT_MASK = 0x00000040,
      TBIT_MASK = 0x00000020,
      MODE_MASK = 0x0000001F
    };
    typedef enum StatusFlags StatusFlags;
    
    // Life cycle
    
    Core(MachineImpl const &machineImpl,
         MachineScreen * const machine_screen,
         SoftKeyboard * const soft_keyboard,
         SerialConsole &serial_console);
    
    ~Core();
    
    // Operation
    
    // STM
    
    inline ExecResultEnum exec_STM_1_DA(Inst const inst)   { return g_STM_1_DA.Process(inst); }
    inline ExecResultEnum exec_STM_1_DA_w(Inst const inst) { return g_STM_1_DA_w.Process(inst); }
    inline ExecResultEnum exec_STM_2_DA(Inst const inst)   { return g_STM_2_DA.Process(inst); }
  
    inline ExecResultEnum exec_STM_1_DB(Inst const inst)   { return g_STM_1_DB.Process(inst); }
    inline ExecResultEnum exec_STM_1_DB_w(Inst const inst) { return g_STM_1_DB_w.Process(inst); }
    inline ExecResultEnum exec_STM_2_DB(Inst const inst)   { return g_STM_2_DB.Process(inst); }
  
    inline ExecResultEnum exec_STM_1_IA(Inst const inst)   { return g_STM_1_IA.Process(inst); }
    inline ExecResultEnum exec_STM_1_IA_w(Inst const inst) { return g_STM_1_IA_w.Process(inst); }
    inline ExecResultEnum exec_STM_2_IA(Inst const inst)   { return g_STM_2_IA.Process(inst); }
  
    inline ExecResultEnum exec_STM_1_IB(Inst const inst)   { return g_STM_1_IB.Process(inst); }
    inline ExecResultEnum exec_STM_1_IB_w(Inst const inst) { return g_STM_1_IB_w.Process(inst); }
    inline ExecResultEnum exec_STM_2_IB(Inst const inst)   { return g_STM_2_IB.Process(inst); }
      
    // LDM
  
    inline ExecResultEnum exec_LDM_1_DA(Inst const inst)    { return g_LDM_1_DA.Process(inst); }
    inline ExecResultEnum exec_LDM_1_DA_w(Inst const inst)  { return g_LDM_1_DA_w.Process(inst); }
    inline ExecResultEnum exec_LDM_23_DA(Inst const inst)   { return g_LDM_23_DA.Process(inst); }
    inline ExecResultEnum exec_LDM_23_DA_w(Inst const inst) { return g_LDM_23_DA_w.Process(inst); }
  
    inline ExecResultEnum exec_LDM_1_DB(Inst const inst)    { return g_LDM_1_DB.Process(inst); }
    inline ExecResultEnum exec_LDM_1_DB_w(Inst const inst)  { return g_LDM_1_DB_w.Process(inst); }
    inline ExecResultEnum exec_LDM_23_DB(Inst const inst)   { return g_LDM_23_DB.Process(inst); }
    inline ExecResultEnum exec_LDM_23_DB_w(Inst const inst) { return g_LDM_23_DB_w.Process(inst); }
    
    inline ExecResultEnum exec_LDM_1_IA(Inst const inst)    { return g_LDM_1_IA.Process(inst); }
    inline ExecResultEnum exec_LDM_1_IA_w(Inst const inst)  { return g_LDM_1_IA_w.Process(inst); }
    inline ExecResultEnum exec_LDM_23_IA(Inst const inst)   { return g_LDM_23_IA.Process(inst); }
    inline ExecResultEnum exec_LDM_23_IA_w(Inst const inst) { return g_LDM_23_IA_w.Process(inst); }
  
    inline ExecResultEnum exec_LDM_1_IB(Inst const inst)    { return g_LDM_1_IB.Process(inst); }
    inline ExecResultEnum exec_LDM_1_IB_w(Inst const inst)  { return g_LDM_1_IB_w.Process(inst); }
    inline ExecResultEnum exec_LDM_23_IB(Inst const inst)   { return g_LDM_23_IB.Process(inst); }
    inline ExecResultEnum exec_LDM_23_IB_w(Inst const inst) { return g_LDM_23_IB_w.Process(inst); }
      
    // SWP
  
    inline ExecResultEnum exec_SWP(Inst const inst)  { return g_SWP.Process(inst); }
    inline ExecResultEnum exec_SWPB(Inst const inst) { return g_SWPB.Process(inst); }
    
    // STR
  
    inline ExecResultEnum exec_STR_imm_post_down(Inst const inst)   { return g_STR_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_STR_imm_post_up(Inst const inst)     { return g_STR_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_STR_imm_offset_down(Inst const inst) { return g_STR_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_STR_imm_offset_up(Inst const inst)   { return g_STR_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_STR_imm_pre_down(Inst const inst)    { return g_STR_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_STR_imm_pre_up(Inst const inst)      { return g_STR_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_STR_reg_post_down(Inst const inst)   { return g_STR_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_STR_reg_post_up(Inst const inst)     { return g_STR_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_STR_reg_offset_down(Inst const inst) { return g_STR_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_STR_reg_offset_up(Inst const inst)   { return g_STR_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_STR_reg_pre_down(Inst const inst)    { return g_STR_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_STR_reg_pre_up(Inst const inst)      { return g_STR_reg_pre_up.Process(inst); }
      
    // STRT
  
    inline ExecResultEnum exec_STRT_imm_post_down(Inst const inst) { return g_STRT_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_STRT_imm_post_up(Inst const inst)   { return g_STRT_imm_post_up.Process(inst); }
  
    inline ExecResultEnum exec_STRT_reg_post_down(Inst const inst) { return g_STRT_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_STRT_reg_post_up(Inst const inst)   { return g_STRT_reg_post_up.Process(inst); }
    
    // STRB
  
    inline ExecResultEnum exec_STRB_imm_post_down(Inst const inst)   { return g_STRB_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_STRB_imm_post_up(Inst const inst)     { return g_STRB_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_STRB_imm_offset_down(Inst const inst) { return g_STRB_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_STRB_imm_offset_up(Inst const inst)   { return g_STRB_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_STRB_imm_pre_down(Inst const inst)    { return g_STRB_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_STRB_imm_pre_up(Inst const inst)      { return g_STRB_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_STRB_reg_post_down(Inst const inst)   { return g_STRB_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_STRB_reg_post_up(Inst const inst)     { return g_STRB_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_STRB_reg_offset_down(Inst const inst) { return g_STRB_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_STRB_reg_offset_up(Inst const inst)   { return g_STRB_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_STRB_reg_pre_down(Inst const inst)    { return g_STRB_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_STRB_reg_pre_up(Inst const inst)      { return g_STRB_reg_pre_up.Process(inst); }
    
    // STRBT
  
    inline ExecResultEnum exec_STRBT_imm_post_down(Inst const inst) { return g_STRBT_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_STRBT_imm_post_up(Inst const inst)   { return g_STRBT_imm_post_up.Process(inst); }
  
    inline ExecResultEnum exec_STRBT_reg_post_down(Inst const inst) { return g_STRBT_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_STRBT_reg_post_up(Inst const inst)   { return g_STRBT_reg_post_up.Process(inst); }
    
    // LDR
  
    inline ExecResultEnum exec_LDR_imm_post_down(Inst const inst)   { return g_LDR_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_LDR_imm_post_up(Inst const inst)     { return g_LDR_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_LDR_imm_offset_down(Inst const inst) { return g_LDR_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDR_imm_offset_up(Inst const inst)   { return g_LDR_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDR_imm_pre_down(Inst const inst)    { return g_LDR_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDR_imm_pre_up(Inst const inst)      { return g_LDR_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_LDR_reg_post_down(Inst const inst)   { return g_LDR_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_LDR_reg_post_up(Inst const inst)     { return g_LDR_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_LDR_reg_offset_down(Inst const inst) { return g_LDR_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDR_reg_offset_up(Inst const inst)   { return g_LDR_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDR_reg_pre_down(Inst const inst)    { return g_LDR_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDR_reg_pre_up(Inst const inst)      { return g_LDR_reg_pre_up.Process(inst); }
  
    // LDRT
  
    inline ExecResultEnum exec_LDRT_imm_post_down(Inst const inst) { return g_LDRT_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRT_imm_post_up(Inst const inst)   { return g_LDRT_imm_post_up.Process(inst); }
  
    inline ExecResultEnum exec_LDRT_reg_post_down(Inst const inst) { return g_LDRT_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRT_reg_post_up(Inst const inst)   { return g_LDRT_reg_post_up.Process(inst); }
    
    // LDRB
  
    inline ExecResultEnum exec_LDRB_imm_post_down(Inst const inst)   { return g_LDRB_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRB_imm_post_up(Inst const inst)     { return g_LDRB_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRB_imm_offset_down(Inst const inst) { return g_LDRB_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRB_imm_offset_up(Inst const inst)   { return g_LDRB_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRB_imm_pre_down(Inst const inst)    { return g_LDRB_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRB_imm_pre_up(Inst const inst)      { return g_LDRB_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_LDRB_reg_post_down(Inst const inst)   { return g_LDRB_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRB_reg_post_up(Inst const inst)     { return g_LDRB_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRB_reg_offset_down(Inst const inst) { return g_LDRB_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRB_reg_offset_up(Inst const inst)   { return g_LDRB_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRB_reg_pre_down(Inst const inst)    { return g_LDRB_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRB_reg_pre_up(Inst const inst)      { return g_LDRB_reg_pre_up.Process(inst); }
      
    // LDRBT
  
    inline ExecResultEnum exec_LDRBT_imm_post_down(Inst const inst) { return g_LDRBT_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRBT_imm_post_up(Inst const inst)   { return g_LDRBT_imm_post_up.Process(inst); }
  
    inline ExecResultEnum exec_LDRBT_reg_post_down(Inst const inst) { return g_LDRBT_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRBT_reg_post_up(Inst const inst)   { return g_LDRBT_reg_post_up.Process(inst); }
      
    // STRH
  
    inline ExecResultEnum exec_STRH_imm_post_down(Inst const inst)   { return g_STRH_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_STRH_imm_post_up(Inst const inst)     { return g_STRH_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_STRH_imm_offset_down(Inst const inst) { return g_STRH_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_STRH_imm_offset_up(Inst const inst)   { return g_STRH_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_STRH_imm_pre_down(Inst const inst)    { return g_STRH_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_STRH_imm_pre_up(Inst const inst)      { return g_STRH_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_STRH_reg_post_down(Inst const inst)   { return g_STRH_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_STRH_reg_post_up(Inst const inst)     { return g_STRH_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_STRH_reg_offset_down(Inst const inst) { return g_STRH_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_STRH_reg_offset_up(Inst const inst)   { return g_STRH_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_STRH_reg_pre_down(Inst const inst)    { return g_STRH_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_STRH_reg_pre_up(Inst const inst)      { return g_STRH_reg_pre_up.Process(inst); }
      
    // LDRH
  
    inline ExecResultEnum exec_LDRH_imm_post_down(Inst const inst)   { return g_LDRH_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRH_imm_post_up(Inst const inst)     { return g_LDRH_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRH_imm_offset_down(Inst const inst) { return g_LDRH_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRH_imm_offset_up(Inst const inst)   { return g_LDRH_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRH_imm_pre_down(Inst const inst)    { return g_LDRH_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRH_imm_pre_up(Inst const inst)      { return g_LDRH_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_LDRH_reg_post_down(Inst const inst)   { return g_LDRH_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRH_reg_post_up(Inst const inst)     { return g_LDRH_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRH_reg_offset_down(Inst const inst) { return g_LDRH_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRH_reg_offset_up(Inst const inst)   { return g_LDRH_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRH_reg_pre_down(Inst const inst)    { return g_LDRH_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRH_reg_pre_up(Inst const inst)      { return g_LDRH_reg_pre_up.Process(inst); }
      
    // LDRSB
  
    inline ExecResultEnum exec_LDRSB_imm_post_down(Inst const inst)   { return g_LDRSB_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRSB_imm_post_up(Inst const inst)     { return g_LDRSB_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRSB_imm_offset_down(Inst const inst) { return g_LDRSB_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRSB_imm_offset_up(Inst const inst)   { return g_LDRSB_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRSB_imm_pre_down(Inst const inst)    { return g_LDRSB_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRSB_imm_pre_up(Inst const inst)      { return g_LDRSB_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_LDRSB_reg_post_down(Inst const inst)   { return g_LDRSB_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRSB_reg_post_up(Inst const inst)     { return g_LDRSB_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRSB_reg_offset_down(Inst const inst) { return g_LDRSB_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRSB_reg_offset_up(Inst const inst)   { return g_LDRSB_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRSB_reg_pre_down(Inst const inst)    { return g_LDRSB_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRSB_reg_pre_up(Inst const inst)      { return g_LDRSB_reg_pre_up.Process(inst); }
      
    // LDRSH
  
    inline ExecResultEnum exec_LDRSH_imm_post_down(Inst const inst)   { return g_LDRSH_imm_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRSH_imm_post_up(Inst const inst)     { return g_LDRSH_imm_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRSH_imm_offset_down(Inst const inst) { return g_LDRSH_imm_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRSH_imm_offset_up(Inst const inst)   { return g_LDRSH_imm_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRSH_imm_pre_down(Inst const inst)    { return g_LDRSH_imm_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRSH_imm_pre_up(Inst const inst)      { return g_LDRSH_imm_pre_up.Process(inst); }
  
    inline ExecResultEnum exec_LDRSH_reg_post_down(Inst const inst)   { return g_LDRSH_reg_post_down.Process(inst); }
    inline ExecResultEnum exec_LDRSH_reg_post_up(Inst const inst)     { return g_LDRSH_reg_post_up.Process(inst); }
    inline ExecResultEnum exec_LDRSH_reg_offset_down(Inst const inst) { return g_LDRSH_reg_offset_down.Process(inst); }
    inline ExecResultEnum exec_LDRSH_reg_offset_up(Inst const inst)   { return g_LDRSH_reg_offset_up.Process(inst); }
    inline ExecResultEnum exec_LDRSH_reg_pre_down(Inst const inst)    { return g_LDRSH_reg_pre_down.Process(inst); }
    inline ExecResultEnum exec_LDRSH_reg_pre_up(Inst const inst)      { return g_LDRSH_reg_pre_up.Process(inst); }
      
    // DPI
  
    inline ExecResultEnum exec_AND_regimm(Inst const inst)  { return g_AND_regimm.Process(inst); }
    inline ExecResultEnum exec_AND_regreg(Inst const inst)  { return g_AND_regreg.Process(inst); }
    inline ExecResultEnum exec_AND_imm(Inst const inst)     { return g_AND_imm.Process(inst); }
    inline ExecResultEnum exec_ANDS_regimm(Inst const inst) { return g_ANDS_regimm.Process(inst); }
    inline ExecResultEnum exec_ANDS_regreg(Inst const inst) { return g_ANDS_regreg.Process(inst); }
    inline ExecResultEnum exec_ANDS_imm(Inst const inst)    { return g_ANDS_imm.Process(inst); }
  
    inline ExecResultEnum exec_EOR_regimm(Inst const inst)  { return g_EOR_regimm.Process(inst); }
    inline ExecResultEnum exec_EOR_regreg(Inst const inst)  { return g_EOR_regreg.Process(inst); }
    inline ExecResultEnum exec_EOR_imm(Inst const inst)     { return g_EOR_imm.Process(inst); }
    inline ExecResultEnum exec_EORS_regimm(Inst const inst) { return g_EORS_regimm.Process(inst); }
    inline ExecResultEnum exec_EORS_regreg(Inst const inst) { return g_EORS_regreg.Process(inst); }
    inline ExecResultEnum exec_EORS_imm(Inst const inst)    { return g_EORS_imm.Process(inst); }
  
    inline ExecResultEnum exec_SUB_regimm(Inst const inst)  { return g_SUB_regimm.Process(inst); }
    inline ExecResultEnum exec_SUB_regreg(Inst const inst)  { return g_SUB_regreg.Process(inst); }
    inline ExecResultEnum exec_SUB_imm(Inst const inst)     { return g_SUB_imm.Process(inst); }
    inline ExecResultEnum exec_SUBS_regimm(Inst const inst) { return g_SUBS_regimm.Process(inst); }
    inline ExecResultEnum exec_SUBS_regreg(Inst const inst) { return g_SUBS_regreg.Process(inst); }
    inline ExecResultEnum exec_SUBS_imm(Inst const inst)    { return g_SUBS_imm.Process(inst); }
  
    inline ExecResultEnum exec_RSB_regimm(Inst const inst)  { return g_RSB_regimm.Process(inst); }
    inline ExecResultEnum exec_RSB_regreg(Inst const inst)  { return g_RSB_regreg.Process(inst); }
    inline ExecResultEnum exec_RSB_imm(Inst const inst)     { return g_RSB_imm.Process(inst); }
    inline ExecResultEnum exec_RSBS_regimm(Inst const inst) { return g_RSBS_regimm.Process(inst); }
    inline ExecResultEnum exec_RSBS_regreg(Inst const inst) { return g_RSBS_regreg.Process(inst); }
    inline ExecResultEnum exec_RSBS_imm(Inst const inst)    { return g_RSBS_imm.Process(inst); }
  
    inline ExecResultEnum exec_ADD_regimm(Inst const inst)  { return g_ADD_regimm.Process(inst); }
    inline ExecResultEnum exec_ADD_regreg(Inst const inst)  { return g_ADD_regreg.Process(inst); }
    inline ExecResultEnum exec_ADD_imm(Inst const inst)     { return g_ADD_imm.Process(inst); }
    inline ExecResultEnum exec_ADDS_regimm(Inst const inst) { return g_ADDS_regimm.Process(inst); }
    inline ExecResultEnum exec_ADDS_regreg(Inst const inst) { return g_ADDS_regreg.Process(inst); }
    inline ExecResultEnum exec_ADDS_imm(Inst const inst)    { return g_ADDS_imm.Process(inst); }
  
    inline ExecResultEnum exec_ADC_regimm(Inst const inst)  { return g_ADC_regimm.Process(inst); }
    inline ExecResultEnum exec_ADC_regreg(Inst const inst)  { return g_ADC_regreg.Process(inst); }
    inline ExecResultEnum exec_ADC_imm(Inst const inst)     { return g_ADC_imm.Process(inst); }
    inline ExecResultEnum exec_ADCS_regimm(Inst const inst) { return g_ADCS_regimm.Process(inst); }
    inline ExecResultEnum exec_ADCS_regreg(Inst const inst) { return g_ADCS_regreg.Process(inst); }
    inline ExecResultEnum exec_ADCS_imm(Inst const inst)    { return g_ADCS_imm.Process(inst); }
  
    inline ExecResultEnum exec_SBC_regimm(Inst const inst)  { return g_SBC_regimm.Process(inst); }
    inline ExecResultEnum exec_SBC_regreg(Inst const inst)  { return g_SBC_regreg.Process(inst); }
    inline ExecResultEnum exec_SBC_imm(Inst const inst)     { return g_SBC_imm.Process(inst); }
    inline ExecResultEnum exec_SBCS_regimm(Inst const inst) { return g_SBCS_regimm.Process(inst); }
    inline ExecResultEnum exec_SBCS_regreg(Inst const inst) { return g_SBCS_regreg.Process(inst); }
    inline ExecResultEnum exec_SBCS_imm(Inst const inst)    { return g_SBCS_imm.Process(inst); }
  
    inline ExecResultEnum exec_RSC_regimm(Inst const inst)  { return g_RSC_regimm.Process(inst); }
    inline ExecResultEnum exec_RSC_regreg(Inst const inst)  { return g_RSC_regreg.Process(inst); }
    inline ExecResultEnum exec_RSC_imm(Inst const inst)     { return g_RSC_imm.Process(inst); }
    inline ExecResultEnum exec_RSCS_regimm(Inst const inst) { return g_RSCS_regimm.Process(inst); }
    inline ExecResultEnum exec_RSCS_regreg(Inst const inst) { return g_RSCS_regreg.Process(inst); }
    inline ExecResultEnum exec_RSCS_imm(Inst const inst)    { return g_RSCS_imm.Process(inst); }
  
    inline ExecResultEnum exec_ORR_regimm(Inst const inst)  { return g_ORR_regimm.Process(inst); }
    inline ExecResultEnum exec_ORR_regreg(Inst const inst)  { return g_ORR_regreg.Process(inst); }
    inline ExecResultEnum exec_ORR_imm(Inst const inst)     { return g_ORR_imm.Process(inst); }
    inline ExecResultEnum exec_ORRS_regimm(Inst const inst) { return g_ORRS_regimm.Process(inst); }
    inline ExecResultEnum exec_ORRS_regreg(Inst const inst) { return g_ORRS_regreg.Process(inst); }
    inline ExecResultEnum exec_ORRS_imm(Inst const inst)    { return g_ORRS_imm.Process(inst); }
  
    inline ExecResultEnum exec_MOV_regimm(Inst const inst)  { return g_MOV_regimm.Process(inst); }
    inline ExecResultEnum exec_MOV_regreg(Inst const inst)  { return g_MOV_regreg.Process(inst); }
    inline ExecResultEnum exec_MOV_imm(Inst const inst)     { return g_MOV_imm.Process(inst); }
    inline ExecResultEnum exec_MOVS_regimm(Inst const inst) { return g_MOVS_regimm.Process(inst); }
    inline ExecResultEnum exec_MOVS_regreg(Inst const inst) { return g_MOVS_regreg.Process(inst); }
    inline ExecResultEnum exec_MOVS_imm(Inst const inst)    { return g_MOVS_imm.Process(inst); }
  
    inline ExecResultEnum exec_BIC_regimm(Inst const inst)  { return g_BIC_regimm.Process(inst); }
    inline ExecResultEnum exec_BIC_regreg(Inst const inst)  { return g_BIC_regreg.Process(inst); }
    inline ExecResultEnum exec_BIC_imm(Inst const inst)     { return g_BIC_imm.Process(inst); }
    inline ExecResultEnum exec_BICS_regimm(Inst const inst) { return g_BICS_regimm.Process(inst); }
    inline ExecResultEnum exec_BICS_regreg(Inst const inst) { return g_BICS_regreg.Process(inst); }
    inline ExecResultEnum exec_BICS_imm(Inst const inst)    { return g_BICS_imm.Process(inst); }
  
    inline ExecResultEnum exec_MVN_regimm(Inst const inst)  { return g_MVN_regimm.Process(inst); }
    inline ExecResultEnum exec_MVN_regreg(Inst const inst)  { return g_MVN_regreg.Process(inst); }
    inline ExecResultEnum exec_MVN_imm(Inst const inst)     { return g_MVN_imm.Process(inst); }
    inline ExecResultEnum exec_MVNS_regimm(Inst const inst) { return g_MVNS_regimm.Process(inst); }
    inline ExecResultEnum exec_MVNS_regreg(Inst const inst) { return g_MVNS_regreg.Process(inst); }
    inline ExecResultEnum exec_MVNS_imm(Inst const inst)    { return g_MVNS_imm.Process(inst); }
  
    inline ExecResultEnum exec_TST_regimm(Inst const inst)  { return g_TST_regimm.Process(inst); }
    inline ExecResultEnum exec_TST_regreg(Inst const inst)  { return g_TST_regreg.Process(inst); }
    inline ExecResultEnum exec_TST_imm(Inst const inst)     { return g_TST_imm.Process(inst); }
  
    inline ExecResultEnum exec_TEQ_regimm(Inst const inst)  { return g_TEQ_regimm.Process(inst); }
    inline ExecResultEnum exec_TEQ_regreg(Inst const inst)  { return g_TEQ_regreg.Process(inst); }
    inline ExecResultEnum exec_TEQ_imm(Inst const inst)     { return g_TEQ_imm.Process(inst); }
  
    inline ExecResultEnum exec_CMP_regimm(Inst const inst)  { return g_CMP_regimm.Process(inst); }
    inline ExecResultEnum exec_CMP_regreg(Inst const inst)  { return g_CMP_regreg.Process(inst); }
    inline ExecResultEnum exec_CMP_imm(Inst const inst)     { return g_CMP_imm.Process(inst); }
  
    inline ExecResultEnum exec_CMN_regimm(Inst const inst)  { return g_CMN_regimm.Process(inst); }
    inline ExecResultEnum exec_CMN_regreg(Inst const inst)  { return g_CMN_regreg.Process(inst); }
    inline ExecResultEnum exec_CMN_imm(Inst const inst)     { return g_CMN_imm.Process(inst); }
      
    // Branch
  
    inline ExecResultEnum exec_BLX_1(Inst const inst) { return g_BLX_1.Process(inst); }
    inline ExecResultEnum exec_BLX_2(Inst const inst) { return g_BLX_2.Process(inst); }
    inline ExecResultEnum exec_BX(Inst const inst)    { return g_BX.Process(inst); }
    inline ExecResultEnum exec_B(Inst const inst)     { return g_B.Process(inst); }
    inline ExecResultEnum exec_BL(Inst const inst)    { return g_BL.Process(inst); }
  
    // Multi
  
    inline ExecResultEnum exec_MUL(Inst const inst)    { return g_MUL.Process(inst); }
    inline ExecResultEnum exec_MULS(Inst const inst)   { return g_MULS.Process(inst); }
    inline ExecResultEnum exec_MLA(Inst const inst)    { return g_MLA.Process(inst); }
    inline ExecResultEnum exec_MLAS(Inst const inst)   { return g_MLAS.Process(inst); }
    inline ExecResultEnum exec_UMULL(Inst const inst)  { return g_UMULL.Process(inst); }
    inline ExecResultEnum exec_UMULLS(Inst const inst) { return g_UMULLS.Process(inst); }
    inline ExecResultEnum exec_UMLAL(Inst const inst)  { return g_UMLAL.Process(inst); }
    inline ExecResultEnum exec_UMLALS(Inst const inst) { return g_UMLALS.Process(inst); }
    inline ExecResultEnum exec_SMULL(Inst const inst)  { return g_SMULL.Process(inst); }
    inline ExecResultEnum exec_SMULLS(Inst const inst) { return g_SMULLS.Process(inst); }
    inline ExecResultEnum exec_SMLAL(Inst const inst)  { return g_SMLAL.Process(inst); }
    inline ExecResultEnum exec_SMLALS(Inst const inst) { return g_SMLALS.Process(inst); }
    
    // MRS
    
    inline ExecResultEnum exec_MRS_cpsr(Inst const inst) { return g_MRS_cpsr.Process(inst); }
    inline ExecResultEnum exec_MRS_spsr(Inst const inst) { return g_MRS_spsr.Process(inst); }
    
    // MSR
    
    inline ExecResultEnum exec_MSR_reg_cpsr(Inst const inst) { return g_MSR_reg_cpsr.Process(inst); }
    inline ExecResultEnum exec_MSR_reg_spsr(Inst const inst) { return g_MSR_reg_spsr.Process(inst); }
    inline ExecResultEnum exec_MSR_imm_cpsr(Inst const inst) { return g_MSR_imm_cpsr.Process(inst); }
    inline ExecResultEnum exec_MSR_imm_spsr(Inst const inst) { return g_MSR_imm_spsr.Process(inst); }
    
    // Undefined instruction
    
    inline ExecResultEnum
    exec_UND(Inst const /* inst */)
    {
      m_exception_type = EXCEPTION_UND;
      return ER_EXCEPTION;
    }
    
    // CDP
    
    inline ExecResultEnum
    exec_CDP(Inst const inst)
    {
      switch (CDPCpNum(inst))
      {
      case CP_0:
      case CP_1:
      case CP_2:
      case CP_3:
      case CP_4:
      case CP_5:
      case CP_6:
      case CP_7:
      case CP_8:
      case CP_9:
      case CP_10:
      case CP_11:
      case CP_12:
      case CP_13:
      case CP_14:
        // :ARM-DDI-0100E: p.A4-20: Wei 2004-May-12:
        //
        // If no coprocessors indicate that they can execute the instruction,
        // an Undefined Instruction exception is generated.
        return exec_UND(inst);
        
      case CP_15:
        // :ARM-DDI-0100E: p.B2-3: Wei 2004-May-12:
        //
        // The only defined System Control coprocessor instructions are:
        //  - MCR instructions to write an ARM register to a CP15 register
        //  - MRC instructions to read the value of a CP15 register into an ARM register.
        //
        // All CP15 CDP, LDC and STC instructions are UNDEFINED.
        return exec_UND(inst);
        
      default:
        assert(!"Should not reach here.");
        return ER_NORMAL;
      }
    }
    
    // CDT
    
    inline ExecResultEnum
    exec_CDT(Inst const inst)
    {
      switch (CDTCpNum(inst))
      {
      case CP_0:
      case CP_1:
      case CP_2:
      case CP_3:
      case CP_4:
      case CP_5:
      case CP_6:
      case CP_7:
      case CP_8:
      case CP_9:
      case CP_10:
      case CP_11:
      case CP_12:
      case CP_13:
      case CP_14:
        // :ARM-DDI-0100E: p.A4-28: Wei 2004-May-12:
        //
        // If no coprocessors indicate that they can execute the instruction,
        // an Undefined Instruction exception is generated.
        return exec_UND(inst);
        
      case CP_15:
        // :ARM-DDI-0100E: p.B2-3: Wei 2004-May-12:
        //
        // The only defined System Control coprocessor instructions are:
        //  - MCR instructions to write an ARM register to a CP15 register
        //  - MRC instructions to read the value of a CP15 register into an ARM register.
        //
        // All CP15 CDP, LDC and STC instructions are UNDEFINED.
        return exec_UND(inst);
        
      default:
        assert(!"Should not reach here.");
        return ER_NORMAL;
      }
    }
    
    // CRT
    
    template<IsLoad isLoad>
    ExecResultEnum exec_CRT(Inst const inst);
    
    // SWI
    
    inline ExecResultEnum
    exec_SWI(Inst const /* inst */)
    {
      m_exception_type = EXCEPTION_SWI;
      return ER_EXCEPTION;
    }
    
#if ENABLE_INST_RECALL    
    inline InstRecallEntry &
    CurrentInstRecallEntry() const
    {
      return mInstRecallBuffer[mInstRecallEntryIndex];
    }
    
    template<IsUpdateStatus is_update_status>
    void RecordUpdateStatus();
    
    void DumpInstRecallBuffer() const;
#endif
    
#if ENABLE_THREADED_CODE
    inline void
    reset_chunk_after_tc_or_dt()
    {
      // :NOTE: Wei 2004-Sep-21:
      //
      // This resetting to 0 is necessary, because Core::get_data<INST>
      // will check this value to decide whether it needs to get a new chunk
      // or not.
      //
      // See Core.tcc for Core::get_data<INST>.
      mp_curr_chunk = 0;
    }
    
    inline void
    reset_chunk()
    {
      // :NOTE: Wei 2004-Sep-21:
      //
      // This resetting to false is necessary, because it is possible
      // that a chunk is started right before a page boundary.
      //
      // If this situation happened, and I do not reset 'm_chunk_start'
      // to 'false', then in Core::exec(), I will want to execute the
      // threaded codes belong to some chunk because of 'm_chunk_start==true'.
      //
      // See Core.cpp for Core::exec().
      m_chunk_start = false;
      
      // :NOTE: Wei 2004-Sep-21:
      //
      // This resetting to 0 is necessary, because Core::get_data<INST>
      // will check this value to decide whether it needs to get a new chunk
      // or not.
      //
      // See Core.tcc for Core::get_data<INST>.
      mp_curr_chunk = 0;
    }
    
    inline void
    set_delay_clean_chunk(Chunk * const chunk)
    {
#if CHECK_CHUNK_CODE
      assert(0 == mp_delay_clean_chunk);
#endif
      
      assert(chunk != 0);
      
      mp_delay_clean_chunk = chunk;
    }
    
#if PRINT_CHUNKS_LENGTH
    static void int_handler(int signum);
    
    inline uint32_t *
    chunks_length()
    {
      return m_chunks_length;
    }
#endif
    
    // :NOTE: Wei 2004-Aug-18:
    //
    // Overloading.
    inline void
    finalize_chunk_no_check(uint32_t const length)
    {
#if PRINT_CHUNKS_LENGTH
      switch (length)
      {
      case 0 ... 9: ++m_chunks_length[CL_0]; break;
      case 10 ... 19: ++m_chunks_length[CL_10]; break;
      case 20 ... 29: ++m_chunks_length[CL_20]; break;
      case 30 ... 39: ++m_chunks_length[CL_30]; break;
      case 40 ... 49: ++m_chunks_length[CL_40]; break;
      case 50 ... 59: ++m_chunks_length[CL_50]; break;
      case 60 ... 69: ++m_chunks_length[CL_60]; break;
      case 70 ... 79: ++m_chunks_length[CL_70]; break;
      case 80 ... 89: ++m_chunks_length[CL_80]; break;
      case 90 ... 99: ++m_chunks_length[CL_90]; break;
      case 100 ... 109: ++m_chunks_length[CL_100]; break;
      case 110 ... 119: ++m_chunks_length[CL_110]; break;
      case 120 ... 129: ++m_chunks_length[CL_120]; break;
      case 130 ... 139: ++m_chunks_length[CL_130]; break;
      case 140 ... 149: ++m_chunks_length[CL_140]; break;
      case 150 ... 159: ++m_chunks_length[CL_150]; break;
      case 160 ... 169: ++m_chunks_length[CL_160]; break;
      case 170 ... 179: ++m_chunks_length[CL_170]; break;
      case 180 ... 189: ++m_chunks_length[CL_180]; break;
      case 190 ... 199: ++m_chunks_length[CL_190]; break;
      default: ++m_chunks_length[CL_OTHER]; break;
      }
#endif
      
      mp_curr_chunk->set_length(length);
      
      reset_chunk();
    }
    
    inline void
    finalize_chunk_no_check()
    {
      finalize_chunk_no_check(m_chunk_length);
    }
    
    inline void
    finalize_chunk()
    {
      if (mp_curr_chunk != 0)
      {
        finalize_chunk_no_check();
      }
      else
      {
        reset_chunk();
      }
    }
    
    inline Chunk *
    curr_chunk() const
    { return mp_curr_chunk; }
    
    inline void 
    set_next_tc_inst_idx(uint32_t const inst_idx)
    {
      mp_curr_chunk->set_curr_arm_inst(inst_idx);
    }
#endif // #if ENABLE_THREADED_CODE
    
#if ENABLE_DYNAMIC_TRANSLATOR
    void *get_global_var_mem_addr(Variable::GlobalVarEnum const kind);
    
    inline void
    calculate_average_native_code_buffer_size(uint32_t const native_size, uint32_t const arm_size)
    {
      if (m_all_native_code_buffer_size > MAX_VALUE_TO_RESET_THE_NATIVE_CODE_BUFFER_SIZE_CALCULATING)
      {
        // :NOTE: Wei 2005-Mar-19:
        //
        // Becuase the size of all native codes will grow faster,
        // thus its value may be overflow.
        // If this happens, then the calculated average value will not be correct,
        // thus I reset the 2 fields to 0 if the size of all native codes exceeds a predefined value.
        m_all_native_code_buffer_size = native_size;
        m_all_compiled_arm_code_size = arm_size;
      }
      else
      {
        m_all_native_code_buffer_size += native_size;
        m_all_compiled_arm_code_size += arm_size;
      }
      
      m_average_native_code_buffer_size = (m_all_native_code_buffer_size / m_all_compiled_arm_code_size);
    }
    
    inline uint32_t
    average_native_code_buffer_size() const
    { return m_average_native_code_buffer_size; }
#endif
    
    void run();
    void reset();
    
    inline void
    set_data_abort()
    { m_exception_type = EXCEPTION_DATA; }
    
    void raise_UND_exception();
    void raise_SWI_exception();
    void raise_IRQ_exception();
    void raise_FIQ_exception();
    void raise_prefetch_abort();
    void raise_data_abort();
    
    template<DPIOpcodeEnum T_opcode, IsUpdateStatus is_update_status>
    uint32_t FASTCALL PreformImmDPISftOper(uint32_t const value, uint32_t const amount);
    
    template<DPIOpcodeEnum T_opcode, IsUpdateStatus is_update_status>
    uint32_t FASTCALL PreformRegImmDPISftOper(uint32_t const value, SftType const type, uint32_t const amount);
    
    template<DPIOpcodeEnum T_opcode, IsUpdateStatus is_update_status>
    uint32_t FASTCALL PreformRegRegDPISftOper(uint32_t const value, SftType const type, uint32_t const amount);
    
    void restore_cpsr();
    
    template<bool T_is_in_idle_mode, bool T_update_core>
    ExecResultEnum check_periphery();
    
    inline PassCondResultEnum FASTCALL
    pass_cond_eq(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (m_CPSR & ZBIT_MASK)
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_ne(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (0 == (m_CPSR & ZBIT_MASK))
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
      
    inline PassCondResultEnum FASTCALL
    pass_cond_cs(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (m_CPSR & CBIT_MASK)
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_cc(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (0 == (m_CPSR & CBIT_MASK))
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_mi(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (m_CPSR & NBIT_MASK)
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_pl(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (0 == (m_CPSR & NBIT_MASK))
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_vs(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (m_CPSR & VBIT_MASK)
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_vc(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      if (0 == (m_CPSR & VBIT_MASK))
      {
        return PASS_TRUE;
      }
      else
      {
        return PASS_FALSE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_hi(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Original:
      //
      // if ((m_CPSR & CBIT_MASK) && (0 == (m_CPSR & ZBIT_MASK)))
      // {
      //   return PASS_TRUE;
      // }
    
      switch (m_CPSR & (CBIT_MASK | ZBIT_MASK))
      {
      case CBIT_MASK:
        return PASS_TRUE;
      
      case 0:
      case ZBIT_MASK:
      case (CBIT_MASK | ZBIT_MASK):
        return PASS_FALSE;
      
      default:
        assert(!"Should not reach here.");
        return PASS_TRUE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_ls(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Original:
      //
      // if ((0 == (m_CPSR & CBIT_MASK)) || (m_CPSR & ZBIT_MASK))
      // {
      //   return PASS_TRUE;
      // }
    
      switch (m_CPSR & (CBIT_MASK | ZBIT_MASK))
      {
      case 0:
      case ZBIT_MASK:
      case (CBIT_MASK | ZBIT_MASK):
        return PASS_TRUE;
      
      case CBIT_MASK:
        return PASS_FALSE;
      
      default:
        assert(!"Should not reach here.");
        return PASS_TRUE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_ge(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Original:
      //
      // if (((m_CPSR & NBIT_MASK) && (m_CPSR & VBIT_MASK)) ||
      //     ((0 == (m_CPSR & NBIT_MASK)) && (0 == (m_CPSR & VBIT_MASK))))
      // {
      //   return PASS_TRUE;
      // }
    
      switch (m_CPSR & (NBIT_MASK | VBIT_MASK))
      {
      case 0:
      case (NBIT_MASK | VBIT_MASK):
        return PASS_TRUE;
      
      case NBIT_MASK:
      case VBIT_MASK:
        return PASS_FALSE;
      
      default:
        assert(!"Should not reach here.");
        return PASS_TRUE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_lt(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Original:
      //
      // if (((m_CPSR & NBIT_MASK) && (0 == (m_CPSR & VBIT_MASK))) ||
      //     ((0 == (m_CPSR & NBIT_MASK)) && (m_CPSR & VBIT_MASK)))
      // {
      //   return PASS_TRUE;
      // }
    
      switch (m_CPSR & (NBIT_MASK | VBIT_MASK))
      {
      case NBIT_MASK:
      case VBIT_MASK:
        return PASS_TRUE;
      
      case 0:
      case (NBIT_MASK | VBIT_MASK):
        return PASS_FALSE;
      
      default:
        assert(!"Should not reach here.");
        return PASS_TRUE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_gt(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Original:
      //
      // if ((0 == (m_CPSR & ZBIT_MASK)) &&
      //     (((m_CPSR & NBIT_MASK) && (m_CPSR & VBIT_MASK)) ||
      //      ((0 == (m_CPSR & NBIT_MASK)) && (0 == (m_CPSR & VBIT_MASK)))))
      // {
      //   return PASS_TRUE;
      // }
    
      switch (m_CPSR & (NBIT_MASK | ZBIT_MASK | VBIT_MASK))
      {
      case 0:
      case (NBIT_MASK | VBIT_MASK):
        return PASS_TRUE;
      
      case NBIT_MASK:
      case VBIT_MASK:
      case ZBIT_MASK:
      case (ZBIT_MASK | VBIT_MASK):
      case (NBIT_MASK | ZBIT_MASK):
      case (NBIT_MASK | ZBIT_MASK | VBIT_MASK):
        return PASS_FALSE;
      
      default:
        assert(!"Should not reach here.");
        return PASS_TRUE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_le(
#if CONFIG_ARCH_V5
      Inst const inst
#endif 
      ) const
    {
      // :NOTE: Wei 2004-Jul-8:
      //
      // Original:
      //
      // if ((m_CPSR & ZBIT_MASK) ||
      //     (((m_CPSR & NBIT_MASK) && (0 == (m_CPSR & VBIT_MASK))) ||
      //      ((0 == (m_CPSR & NBIT_MASK)) && (m_CPSR & VBIT_MASK))))
      // {
      //   return PASS_TRUE;
      // }
    
      switch (m_CPSR & (NBIT_MASK | ZBIT_MASK | VBIT_MASK))
      {
      case ZBIT_MASK:
      case (ZBIT_MASK | VBIT_MASK):
      case (NBIT_MASK | ZBIT_MASK):
      case (NBIT_MASK | ZBIT_MASK | VBIT_MASK):
      case NBIT_MASK:
      case VBIT_MASK:
        return PASS_TRUE;
      
      case 0:
      case (NBIT_MASK | VBIT_MASK):
        return PASS_FALSE;
      
      default:
        assert(!"Should not reach here.");
        return PASS_TRUE;
      }
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_al(
#if CONFIG_ARCH_V5
      Inst const inst
#endif
      ) const
    {
      return PASS_TRUE;
    }
    
    inline PassCondResultEnum FASTCALL
    pass_cond_nv(
#if CONFIG_ARCH_V5
      Inst const inst
#endif
      ) const
    {
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2
      // :ARM-DDI-0100E: p.A3-5: Wei 2004-Feb-12:
      //
      // Prior to ARM architecture version 3,
      // a condition field of 0b1111 meant that the instruction was never executed.
      // The mnemonic extension for this condition was NV.
      return PASS_FALSE;
#elif CONFIG_ARCH_V3 || CONFIG_ARCH_V4
      // :ARM-DDI-0100E: p.A3-5: Wei 2004-Feb-12:
      //
      // In ARM architecture V3 & V4,
      // any instruction with a condition field of 0b1111 is UNPREDICTABLE.
      assert(!"Should not reach here.");
      return PASS_TRUE;
#elif CONFIG_ARCH_V5
      switch (inst.mRawData & 0x0F000000)
      {
      case 0x0A000000:
      case 0x0B000000:
        // :NOTE: Wei 2004-Jan-20:
        //
        // BLX(1):
        return PASS_TRUE;
      
      default:
        // :ARM-DDI-0100E: p.A3-2: Wei 2004-Feb-12:
        //
        // According to Figure 3-1:
        //
        // in ARM architecture V5, those instructions which are not BLX(1) is
        // Undefined instructions.
        return PASS_EXCEPTION;
      }
#endif
    }
    
    // :NOTE: Wei 2004-Feb-06:
    //
    // Because the only place uses pass_cond() is in Core.cpp,
    // thus I put the definition of pass_cond() into Core.cpp
    // to simplify Core.hpp
    PassCondResultEnum FASTCALL pass_cond(Inst const inst);
    
    void change_reg_bank(CoreMode const newMode);
    
    inline bool
    is_in_idle_mode() const
    { return m_idle_mode; }
    
    inline void
    enter_idle_mode()
    { m_idle_mode = true; }
    
    inline void
    exit_idle_mode()
    { m_idle_mode = false; }
    
    template<AccessTypeEnum accessTypeEnum, RWType rw_type>
    uint32_t FASTCALL virtual_to_physical(uint32_t &vaddr,
                                          CoreMode const currentMode,
                                          bool &exception_occur) const;
    
    template<DataWidthEnum dataWidthEnum, AccessTypeEnum accessTypeEnum>
    typename DataWidthTraits<dataWidthEnum>::UnsignedType FASTCALL get_data(uint32_t vaddr,
                                                                            CoreMode const currentMode,
                                                                            bool &exception_occur)
#if !ENABLE_THREADED_CODE
      // :NOTE: Wei 2004-Aug-13:
      //
      // Because Core::get_data() threaded code version will modify 'm_chunk_start' & 'mp_curr_chunk',
      // thus I have to declare Core::get_data() as a non-const member function.
      // Otherwise, I can declare Core::get_data() as a const member function.
      const
#endif
      ;
    
#if CHECK_CHUNK_CODE
    uint32_t get_curr_inst() const;
#endif
    
    template<DataWidthEnum dataWidthEnum>
    void FASTCALL put_data(uint32_t vaddr,
                           typename DataWidthTraits<dataWidthEnum>::UnsignedType const value,
                           CoreMode const currentMode,
                           bool &exception_occur);
    
    template<DataWidthEnum dataWidthEnum>
    uint32_t CheckAlign(uint32_t const address, bool &exception_occur) const;
    
#if DUMP_CPU_INFO
    template<bool T_dump_USR>
    void dump_cpu_info() const;
#endif
    
    // Access method
    
    inline uint32_t &
    rCurReg(CoreRegNum const index)
    { return m_curr_reg_bank[index]; }
    
    inline uint32_t &
    rUsrReg(CoreRegNum const index)
    {
      switch (m_curr_mode)
      {
      case MODE_USR:
      case MODE_SYS:
        return m_curr_reg_bank[index];
        
      case MODE_IRQ:
      case MODE_SVC:
      case MODE_ABT:
      case MODE_UND:
        switch (index)
        {
        case CR_0:
        case CR_1:
        case CR_2:
        case CR_3:
        case CR_4:
        case CR_5:
        case CR_6:
        case CR_7:
        case CR_8:
        case CR_9:
        case CR_10:
        case CR_11:
        case CR_12:
        case CR_PC:
          return m_curr_reg_bank[index];
          
        case CR_13:
        case CR_14:
          return m_USR_reg_bank[index];
          
        default:
          assert(!"Should not reach here.");
          return m_USR_reg_bank[index];
        }
        break;
        
      case MODE_FIQ:
        switch (index)
        {
        case CR_0:
        case CR_1:
        case CR_2:
        case CR_3:
        case CR_4:
        case CR_5:
        case CR_6:
        case CR_7:
        case CR_PC:
          return m_curr_reg_bank[index];
          
        case CR_8:
        case CR_9:
        case CR_10:
        case CR_11:
        case CR_12:
        case CR_13:
        case CR_14:
          return m_USR_reg_bank[index];
          
        default:
          assert(!"Should not reach here.");
          return m_USR_reg_bank[index];
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        return m_USR_reg_bank[index];
      }
      
      assert(!"Should not reach here.");
      return m_curr_reg_bank[index];
    }
    
    inline uint32_t &
    rCurSpsr()
    {
      assert((MODE_SVC == m_curr_mode) ||
             (MODE_ABT == m_curr_mode) ||
             (MODE_UND == m_curr_mode) ||
             (MODE_IRQ == m_curr_mode) ||
             (MODE_FIQ == m_curr_mode));
      
      return m_curr_SPSR;
    }
    
    inline uint32_t &
    rCpsr()
    { return m_CPSR; }
    
    inline ExceptionType &
    r_exception_type()
    { return m_exception_type; }
    
    inline uint32_t &
    r_device_timer_increment()
    { return m_device_timer_increment; }
    
    // Set
    
    inline void
    set_CPSR_N_bit() // Set N
    { m_CPSR |= NBIT_MASK; }
    
    inline void
    set_CPSR_Z_bit() // Set Z
    { m_CPSR |= ZBIT_MASK; }
    
    inline void
    set_CPSR_C_bit() // Set C
    { m_CPSR |= CBIT_MASK; }
    
    inline void
    set_CPSR_V_bit() // Set V
    { m_CPSR |= VBIT_MASK; }
    
    inline void
    set_CPSR_T_bit() // Set T
    { m_CPSR |= TBIT_MASK; }
    
    inline void
    set_CPSR_NC_bit() // Set N, C
    { m_CPSR |= (NBIT_MASK | CBIT_MASK); }
    
    inline void
    set_CPSR_NV_bit() // Set N, V
    { m_CPSR |= (NBIT_MASK | VBIT_MASK); }
    
    inline void
    set_CPSR_ZC_bit() // Set Z, C
    { m_CPSR |= (ZBIT_MASK | CBIT_MASK); }
    
    inline void
    set_CPSR_ZV_bit() // Set Z, V
    { m_CPSR |= (ZBIT_MASK | VBIT_MASK); }
    
    inline void
    set_CPSR_CV_bit() // Set C, V
    { m_CPSR |= (CBIT_MASK | VBIT_MASK); }
    
    inline void
    set_CPSR_NCV_bit() // Set N, C, V
    { m_CPSR |= (NBIT_MASK | CBIT_MASK | VBIT_MASK); }
    
    inline void
    set_CPSR_ZCV_bit() // Set Z, C, V
    { m_CPSR |= (ZBIT_MASK | CBIT_MASK | VBIT_MASK); }
    
    // Clear
    
    inline void
    clear_CPSR_N_bit()
    { m_CPSR &= ~NBIT_MASK; }
    
    inline void
    clear_CPSR_Z_bit()
    { m_CPSR &= ~ZBIT_MASK; }
    
    inline void
    clear_CPSR_C_bit()
    { m_CPSR &= ~CBIT_MASK; }
    
    inline void
    clear_CPSR_V_bit()
    { m_CPSR &= ~VBIT_MASK; }
    
    inline void
    clear_CPSR_T_bit()
    { m_CPSR &= ~TBIT_MASK; }
    
    inline void
    clear_CPSR_NZ_bit()
    { m_CPSR &= (0x0FFFFFFF | VBIT_MASK | CBIT_MASK); }
    
    inline void
    clear_CPSR_NZC_bit()
    { m_CPSR &= (0x0FFFFFFF | VBIT_MASK); }
    
    inline void
    clear_CPSR_NZCV_bit()
    { m_CPSR &= 0x0FFFFFFF; }
    
    // rs232
    
    inline void
    enable_RS232()
    { m_RS232_is_enabled = true; }
    
    inline void
    disable_RS232()
    { m_RS232_is_enabled = false; }
    
    // PC relative
    
    inline void
    ModifyPC(uint32_t const newValue)
    {
      m_curr_reg_bank[CR_PC] = newValue;
      
#if CHECK_UNPREDICTABLE
      assert(m_curr_reg_bank[CR_PC] <= 0xFFFFFFFFU);
#endif
    }
    
    inline void
    IncrePC()
    {
      m_curr_reg_bank[CR_PC] += 4;
      
#if CHECK_PC_MODIFY
      m_pc_incremented = true;
#endif
      
#if CHECK_UNPREDICTABLE
      assert(m_curr_reg_bank[CR_PC] <= 0xFFFFFFFFU);
#endif
    }
    
    // Inquiry method
    
#if PRINT_CHUNK_CODE
    inline bool
    is_exec_in_chunk() const
    { return m_exec_in_chunk; }
#endif
    
    inline bool
    IsIrqEnabled() const
    { return (m_CPSR & IBIT_MASK) ? false : true; }
    
    inline bool
    IsFiqEnabled() const
    { return (m_CPSR & FBIT_MASK) ? false : true; }
    
    inline uint32_t
    Freq() const
    { return mCoreFreq; }
    
    inline uint32_t
    CurReg(CoreRegNum const index) const
    { return m_curr_reg_bank[index]; }
    
    inline uint32_t
    UsrReg(CoreRegNum const index) const
    {
      switch (m_curr_mode)
      {
      case MODE_USR:
      case MODE_SYS:
        return m_curr_reg_bank[index];
        
      case MODE_IRQ:
      case MODE_SVC:
      case MODE_ABT:
      case MODE_UND:
        switch (index)
        {
        case CR_0:
        case CR_1:
        case CR_2:
        case CR_3:
        case CR_4:
        case CR_5:
        case CR_6:
        case CR_7:
        case CR_8:
        case CR_9:
        case CR_10:
        case CR_11:
        case CR_12:
        case CR_PC:
          return m_curr_reg_bank[index];
          
        case CR_13:
        case CR_14:
          return m_USR_reg_bank[index];
          
        default:
          assert(!"Should not reach here.");
          return 0;
        }
        break;
        
      case MODE_FIQ:
        switch (index)
        {
        case CR_0:
        case CR_1:
        case CR_2:
        case CR_3:
        case CR_4:
        case CR_5:
        case CR_6:
        case CR_7:
        case CR_PC:
          return m_curr_reg_bank[index];
          
        case CR_8:
        case CR_9:
        case CR_10:
        case CR_11:
        case CR_12:
        case CR_13:
        case CR_14:
          return m_USR_reg_bank[index];
          
        default:
          assert(!"Should not reach here.");
          return 0;
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
      
      assert(!"Should not reach here.");
      return m_curr_reg_bank[index];
    }
    
#if USE_SHIFTER_CARRY
    inline uint32_t
    ShifterCarry() const
    { return mShifterCarry; }
#endif
    
    inline bool
    IsCpsrNbitSet() const
    { return ((m_CPSR & NBIT_MASK) != 0) ? true : false; }
    
    inline bool
    IsCpsrZbitSet() const
    { return ((m_CPSR & ZBIT_MASK) != 0) ? true : false; }
    
    inline bool
    IsCpsrCbitSet() const
    { return ((m_CPSR & CBIT_MASK) != 0) ? true : false; }
    
    inline bool
    IsCpsrVbitSet() const
    { return ((m_CPSR & VBIT_MASK) != 0) ? true : false; }
    
    inline bool
    IsCpsrTbitSet() const
    { return ((m_CPSR & TBIT_MASK) != 0) ? true : false; }
    
    inline CoreMode
    CurrentMode() const
    { return m_curr_mode; }
    
    inline uint32_t
    ReadPC() const
    { return m_curr_reg_bank[CR_PC] + 8; }
    
    inline uint32_t
    read_raw_pc() const
    { return m_curr_reg_bank[CR_PC]; }
    
    inline uint32_t
    Cpsr() const
    { return m_CPSR; }
    
    inline uint32_t
    CurSpsr() const
    { return m_curr_SPSR; }
    
    // MMU interface
    
    inline void
    flush_inst_tlb() const
    { mpMmu->flush_inst_tlb(); }
    
    inline void
    flush_data_tlb() const
    { mpMmu->flush_data_tlb(); }
    
    inline void
    flush_data_tlb_single_entry(uint32_t const address) const
    { mpMmu->flush_data_tlb_single_entry(address); }
    
    // Fault Status & Fault address
    
    inline void
    update_FSR_FAR(FaultStatus const status, uint32_t const domain_num, uint32_t const vaddr) const
    {
      mpSysCoProc->update_FSR_FAR(status, domain_num, vaddr);
    }
  };
  
  template<>
  uint32_t Core::CheckAlign<WORD>(uint32_t const address, bool &exception_occur) const;
  
  template<>
  uint32_t Core::CheckAlign<HALF_WORD>(uint32_t const address, bool &exception_occur) const;
  
  template<>
  uint32_t Core::CheckAlign<BYTE>(uint32_t const address, bool &exception_occur) const;
  
#if ENABLE_INST_RECALL
  template<>
  inline void
  Core::RecordUpdateStatus<UPDATE_STATUS>()
  {
    CurrentInstRecallEntry().mUpdateStatusRegister = true;
  }
  
  template<>
  inline void
  Core::RecordUpdateStatus<NO_UPDATE_STATUS>()
  {
    CurrentInstRecallEntry().mUpdateStatusRegister = false;
  }
#endif
    
  // Global object pointer
  extern Core * gp_core;
}

#endif
