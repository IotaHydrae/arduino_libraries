/**
 * @file st7789v.cpp
 * @author Zheng Hua (writeforever@foxmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-09
 *
 * MIT License
 *
 * Copyright 2022 Zheng Hua(writeforever@foxmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "st7789v.h"

st7789v_handle_t ST7789V::m_st7789v_handle = {
    .scl = 0,
    .sda = 0,
    .cs  = 0,
    .res = 0,
    .dc  = 0,
    .spi_mode = 0,
    .spi_bit_order = 0,
    .spi_speed = 0
};


enum st7789v_command {
    NOP       = 0x00,   // No operation
    SWRESET   = 0x01,   // Software reset
    RDDID     = 0x04,   // Read display identification information
    RDDST     = 0x09,   // Read display status
    RDDPM     = 0x0A,   // Read display power mode
    RDDMADCTL = 0x0B,   // Read display MADCTL
    RDDCOLMOD = 0x0C,   // Read display pixel format
    RDDIM     = 0x0D,   // Read display image mode
    RDDSM     = 0x0E,   // Read display signal mode
    RDDSDR    = 0x0F,   // Read display self-diagnostic result
    SLPIN     = 0x10,   // Enter sleep mode
    SLPOUT    = 0x11,   // Sleep out
    PTLON     = 0x12,   // Partial mode on
    NORON     = 0x13,   // Normal display mode on
    INVOFF    = 0x20,   // Display inversion off
    INVON     = 0x21,   // Display inversion on
    GAMSET    = 0x26,   // Gamma set
    DISPOFF   = 0x28,   // Display off
    DISPON    = 0x29,   // Display on
    CASET     = 0x2A,   // Column address set
    RASET     = 0x2B,   // Row address set
    RAMWR     = 0x2C,   // Memory write
    RAMRD     = 0x2E,   // Memory read
    PTLAR     = 0x30,   // Partial start/end address set
    VSCRDEF   = 0x33,   // Vertical scrolling definition
    TEOFF     = 0x34,   // Tearing effect line off
    TEON      = 0x35,   // Tearing effect line on
    MADCTL    = 0x36,   // Memory data access control
    VSCRSADD  = 0x37,   // Vertical scrolling start address
    IDMOFF    = 0x38,   // Idle mode off
    IDMON     = 0x39,   // Idle mode on
    COLMOD    = 0x3A,   // Interface pixel format
    RAMWRC    = 0x3C,   // Memory write continue
    RAMRDC    = 0x3E,   // Memory read continue
    TESCAN    = 0x44,   // Set tear scanline
    RDTESCAN  = 0x45,   // Read tear scanline
    WRDISBV   = 0x51,   // Write display brightness
    RDDISBV   = 0x52,   // Read display brightness
    WRCTRLD   = 0x53,   // Write control display
    RDCTRLD   = 0x54,   // Read control display
    WRCACE    = 0x55,   // Write content adaptive brightness control
    RDCACB    = 0x56,   // Read content adaptive brightness control
    WRCABCMB  = 0x5E,   // Write CABC minimum brightness
    RDCABCMB  = 0x5F,   // Read CABC minimum brightness
    RDABCSDR  = 0x68,   // Read auto brightness control self-diagnostic result
    RDID1     = 0xDA,   // Read ID1
    RDID2     = 0xDB,   // Read ID2
    RDID3     = 0xDC,   // Read ID3
};

struct st7789v_cmd_param st7789v_init_cmds[] = {
    // command  len data  ms  us
    SWRESET, 0, NULL, 120, 0,   // software reset
    SLPOUT,  0, NULL, 10, 0,     // sleep out
    
    COLMOD,  1, ( u8[] ){0x55}, 10, 0, // interface pixel format, 16-bit/pixel for RGB 565 format
    CASET,   4, ( u8[] ){0x00, 0x00, 0x00, 0xEF}, 0, 0, // column address set - from 0 to 239
    RASET,   4, ( u8[] ){0x00, 0x00, 0x00, 0x86}, 0, 0, // row address set - from 0 to 134
    MADCTL,  1, ( u8[] ){0x00}, 0, 0, // memory data access control
    
    NORON,   0, NULL, 10, 0,     // normal display mode on, means partial mode off
    
    //WRDISBV, 1, ( u8[] ){0xFF}, 0, 0, // write display brightness
    //INVOFF,  0, NULL, 10, 0,     // display inversion off
    //DISPON,  0, NULL, 10, 0,    // display on, recover from display off mode, output from the frame memory is enabled.
};


ST7789V::ST7789V( int scl, int sda, int cs, int dc, int rst )
{
    st7789v_handle_t handle;
    
    handle.scl = scl;
    handle.sda = sda;
    handle.cs  = cs;
    handle.dc  = dc;
    handle.res = rst;
    handle.spi_speed = 14000000;
    handle.spi_mode  = SPI_MODE0;
    handle.spi_bit_order = MSBFIRST;
    
    m_st7789v_handle = handle;
}


ST7789V::ST7789V( int cs, int dc, int rst )
{
    st7789v_handle_t handle;
    
    handle.scl = 0;
    handle.sda = 0;
    handle.cs  = cs;
    handle.dc  = dc;
    handle.res = rst;
    handle.spi_speed = 14000000;
    handle.spi_mode  = SPI_MODE0;
    handle.spi_bit_order = MSBFIRST;
    
    m_st7789v_handle = handle;
}

inline static  void st7789_set_init_pinMode()
{
//#if ST7789V_USE_SOFTWARE_SPI
    pinMode( ST7789V::m_st7789v_handle.scl, OUTPUT );
    pinMode( ST7789V::m_st7789v_handle.sda, OUTPUT );
//#endif
    pinMode( ST7789V::m_st7789v_handle.cs, OUTPUT );
    pinMode( ST7789V::m_st7789v_handle.dc, OUTPUT );
    pinMode( ST7789V::m_st7789v_handle.res, OUTPUT );
}

inline static void st7789_set_init_pinState()
{
//#if ST7789V_USE_SOFTWARE_SPI
    digitalWrite( ST7789V::m_st7789v_handle.scl, LOW );
    digitalWrite( ST7789V::m_st7789v_handle.sda, LOW );
//#endif

    digitalWrite( ST7789V::m_st7789v_handle.cs, HIGH );
    digitalWrite( ST7789V::m_st7789v_handle.dc, LOW );
    digitalWrite( ST7789V::m_st7789v_handle.res, LOW );
}

inline static void st7789_hardware_reset()
{
    digitalWrite( ST7789V::m_st7789v_handle.res, LOW );
    delay( 10 );
    digitalWrite( ST7789V::m_st7789v_handle.res, HIGH );
    delay( 10 );
}


void ST7789V::init( u16 width, u16 height )
{
    st7789_set_init_pinMode();
    st7789_set_init_pinState();
    st7789_hardware_reset();

#if ST7789V_USE_HARDWARE_SPI
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
#endif
    
    init_display( st7789v_init_cmds,
                  sizeof( st7789v_init_cmds ) / sizeof( st7789v_init_cmds[0] ) );
}


