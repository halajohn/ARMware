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

#ifndef AtmelMicro_hpp
#define AtmelMicro_hpp

#include <deque>

#include "Type.hpp"

#include "Gtk/MachineScreen.hpp"

namespace ARMware
{
  typedef class AtmelMicro AtmelMicro;
  class AtmelMicro
  {
  private:
    
    // Introduction
    // ------------
    // This module (h3650_ts.c) is a driver designed for the iPAQ (H3650) hand held
    // device which is powered by the StrongArm (sa1110) microprocessor.
    // 
    // The iPAQ design uses an Atmel microcontroller to service a number of peripheral
    // devices such as the touch screen, leds, buttons and others - a full list is
    // show below.
    // 
    // The Atmel acts as a proxy controller for the StrongArm.
    // 
    // Commands are sent from the StrongArm using a simple serial packet format
    // shown in figure 1. Events and responses also use this format.
    // 
    // This communication is carried out via serial port1 of the sa1110.
    // 
    // Packet Format
    // -------------
    // Commands, responses and events are transmitted using the following packet
    // format. 
    // 
    //   byte	  1	  2		  3 		 len + 4
    // 	+-------+---------------+---------------+--=------------+
    // 	|SOF	|id	|len	| len bytes	| Chksum	|
    // 	+-------+---------------+---------------+--=------------+
    //   bit	0     7  8    11 12   15 16
    // 
    // 	+-------+---------------+-------+
    // 	|SOF	|id	|0	|Chksum	| - Note Chksum does not include SOF
    // 	+-------+---------------+-------+
    //   bit	0     7  8    11 12   15 16
    // 
    // 	FIGURE1
    // 	------
    // 
    //   SOF	(1 byte)
    // 	Start of Frame character (0x02)
    //   id	(4bits)
    // 	Each command/respomse and event has an unique ID (see table below)
    //   len	(4bits)
    // 	Each command/response or event may have a payload of 'len' bytes.
    // 	FIGURE 2 shows the touch screen payload format. Other formats TBD.
    //   data	'len' bytes
    // 	This is the payload.
    //   chk	(1byte)
    // 	This a modulo 256 checksum. It does not incluse the SOF.
    //
    // The raw touch screen data read from the Atmel is formatted as follows:
    // 	+-------+-------+-------+-------+
    // 	| Xmsb	| Xlsb	| Ymsb	| Ylsb	|
    // 	+-------+-------+-------+-------+
    // 	byte 0	  1	  2	  3
    // 
    // 	Xmsb - most significant byte of Xcoordinate
    // 	Xlsb - least significant byte of Xcoordinate
    // 	FIGURE 2
    
    enum State
    {
      STATE_SOF,
      STATE_ID_LEN,
      STATE_DATA,
      STATE_CHK
    };
    typedef enum State State;
    
    // :NOTE: Wei 2004-Jun-29:
    //
    // Becuase message id resides in the upper 4-bit of the 2nd byte,
    // Thus I define the following constants 0x?0 rather than 0x0?
    enum MsgEnum
    {
      MSG_VERSION       = 0x00, // Get or Set the version number.
      MSG_KEYBD_ID      = 0x20, // Button events?.
      MSG_TOUCHS_ID     = 0x30, // Touch Screen events.
      MSG_EEPROM_READ   = 0x40, // Read from EEPROM.
      MSG_EEPROM_WRITED = 0x50, // Write to EEPROM
      MSG_LED           = 0x80, // Switch on/off led.
      MSG_BATTERY       = 0x90, // Get battery status.
      MSG_IIC_READ      = 0xb0, // IIC read - get data via the IIC protocol.
      MSG_IIC_WRITE     = 0xc0, // IIC write - send data via the IIC protocol.
      MSG_BACKLIGHT     = 0xd0  // Backligh control command.
    };
    typedef enum MsgEnum MsgEnum;
    
    // :NOTE: Wei 2004-Jul-1:
    //
    // Commands can be divided into two categories -  GET and SET.
    // A GET command will return data to the sa1110 from an entity identified by it¡¦s ID number
    // and a SET command will write data to the entity identified by it¡¦s ID number.
    //
    // For example the front light bas a SET command.
    // Data will be sent to the front light (ID=FLITE_ID) to control it¡¦s brightness or
    // switch it on or off.
    // No data will be returned in the front light set command response packet.
    enum MsgResponseLength
    {
      MSG_VERSION_GET_RESPONSE_LEN = 4,
      
      MSG_BACKLIGHT_SET_RESPONSE_LEN = 0,
      MSG_BACKLIGHT_GET_RESPONSE_LEN = 1
    };
    typedef enum MsgResponseLength MsgResponseLength;
    
    enum MsgEventLength
    {
      MSG_TOUCH_PANEL_PEN_DOWN_EVENT_LEN = 4,
      MSG_TOUCH_PANEL_PEN_UP_EVENT_LEN = 0
    };
    typedef enum MsgEventLength MsgEventLength;
    
    enum BackLightMode
    {
      FLITE_AUTO_MODE = 1,
      FLITE_MANUAL_MODE = 2, // Linux kernel 2.4.19 doesn't use this value, however, bootldr 2.21.12 uses it.
      FLITE_GET_LIGHT_SENSOR = 3 // Returns light reading in "brightness" field
    };
    typedef enum BackLightMode BackLightMode;
    
    static uint32_t const SOF = 0x02;
    
    // Incoming data
    
    State m_get_state;
    
    uint8_t m_id;
    uint8_t m_len;
    uint8_t m_data[15];
    
#if CHECK_AMTEL_CHKSUM
    uint8_t m_chk;
#endif
    
    uint8_t m_read;
    
    // Response data
    
    std::deque<uint8_t> m_response;
    
    // H3600's state
    
    uint8_t m_brightness; // the brightness of the backlight of the LCD.
    
    // Operation
    
    inline void
    push_touch_panel_pen_down_event(uint32_t const x, uint32_t const y)
    {
      m_response.push_back(SOF);
      m_response.push_back(MSG_TOUCHS_ID | MSG_TOUCH_PANEL_PEN_DOWN_EVENT_LEN);
      
      uint8_t const mx = static_cast<uint8_t>((x >> 8) & 0xFF);
      uint8_t const lx = static_cast<uint8_t>(x & 0xFF);
      uint8_t const my = static_cast<uint8_t>((y >> 8) & 0xFF);
      uint8_t const ly = static_cast<uint8_t>(y & 0xFF);
      
      m_response.push_back(mx); // most significant of x
      m_response.push_back(lx); // least significant of x
      m_response.push_back(my); // most significant of y
      m_response.push_back(ly); // least significant of y
      
      // :NOTE: Wei 2004-Jul-7:
      //
      // This a modulo 256 checksum. It does not incluse the SOF.
      m_response.push_back(((MSG_TOUCHS_ID | MSG_TOUCH_PANEL_PEN_DOWN_EVENT_LEN) + mx + lx + my + ly) & (256 - 1));
    }
    
    inline uint32_t
    y2x(uint32_t const y) const
    { return MachineScreen::DISPLAY_HEIGHT - y - 1; }
    
    inline uint32_t
    x2y(uint32_t const x) const
    { return x; }
    
  public:
    
    // Life cycle
    
    AtmelMicro()
      : m_get_state(STATE_SOF),
        m_read(0),
        m_brightness(0)
    { }
    
    // Operation
    
    void get_data_from_serial_port(std::vector<uint8_t> const &fifo);
    void put_data_to_serial_port(std::deque<uint8_t> &rx_fifo);
    
    inline void
    receive_touch_panel_pen_down_event(GdkEventButton const * const event)
    {
      assert(GDK_BUTTON_PRESS == event->type);
      
      assert((event->x > 0) && (event->x < MachineScreen::DISPLAY_WIDTH));
      assert((event->y > 0) && (event->y < MachineScreen::DISPLAY_HEIGHT));
      
      uint32_t const x = y2x(static_cast<uint32_t>(event->y));
      uint32_t const y = x2y(static_cast<uint32_t>(event->x));
      
      push_touch_panel_pen_down_event(x, y);
      
      // :NOTE: Wei 2004-Jul-7:
      //
      // In the real world, when pens down, the iPaq touch panel will issue several pen down events.
      //
      // idea condition:
      //
      //        |------------|
      // -------|            |-----------------
      //
      // real condition:
      //
      //        |--|  |-|  |-|
      // -------|  |--| |--| |-----------------
      //
      // And the axis information of the first several pen down events can't be trust.
      // (because of some circuit characteristic)
      // Thus the linux kernel will discard the first several pen down events,
      // and read the axis information of the successive pen down events.
      //
      // Ex: In 2.4.19-rmk6-pxa1-hh30, linux kernel discards the first 2 pen down events.
      //     One in arch/arm/mach-sa1100/h3600_micro.c : h3600_micro_touchpanel_message()
      //     Another in drivers/char/h3600_ts.c : h3600_ts_touchpanel_event()
      //
      //     However, because ARMware doesn't have circuit characteristic problem,
      //     Thus, I modify the linux kernel to let it just ignore one.
      //
      //     i.e. Modify 'discard_initial_touch' variable in h3600_ts.c from 1 to 0
      //
      // Hence, when pens down, I push 2 pen down events into the m_response queue
      // to let the linux kernel to discard the 1st one and read the 2nd one.
      push_touch_panel_pen_down_event(x, y);
    }
    
    inline void
    receive_touch_panel_pen_move_event(GdkEventMotion const * const event)
    {
      assert((event->x > 0) && (event->x < MachineScreen::DISPLAY_WIDTH));
      assert((event->y > 0) && (event->y < MachineScreen::DISPLAY_HEIGHT));
      
      push_touch_panel_pen_down_event(y2x(static_cast<uint32_t>(event->y)),
                                      x2y(static_cast<uint32_t>(event->x)));
    }
    
    inline void
    receive_touch_panel_pen_up_event(GdkEventButton const * const
#ifndef NDEBUG
                                     event
#endif
                                     )
    {
      assert(GDK_BUTTON_RELEASE == event->type);
      
      m_response.push_back(SOF);
      m_response.push_back(MSG_TOUCHS_ID | MSG_TOUCH_PANEL_PEN_UP_EVENT_LEN);
      m_response.push_back(MSG_TOUCHS_ID | MSG_TOUCH_PANEL_PEN_UP_EVENT_LEN);
    }
  };
}

#endif
