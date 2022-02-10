/**
 * @file st7789v.h
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

#ifndef __ST7789V_H
#define __ST7789V_H

#include <inttypes.h>
#include <SPI.h>

#define NUMARGS(...)  (sizeof((int[]){__VA_ARGS__}) / sizeof(int))

#define write_reg(handle, ...)                                            \
    ((handle)->st7789v_ops.write_register(par, NUMARGS(__VA_ARGS__), __VA_ARGS__))
#define delay_us(x) delayMicroseconds(x)

#ifndef ST7789V_USE_HARDWARE_SPI
    #define ST7789V_USE_HARDWARE_SPI 0
#endif

#ifndef ST7789V_USE_SOFTWARE_SPI
    #define ST7789V_USE_SOFTWARE_SPI 1
#endif

union WDATA {
    uint16_t w;
    struct {
        uint8_t lsb;
        uint8_t msb;
    } bytes;
};

typedef struct {
    int ( *write )( void *buf, size_t len );
    int ( *read )( void *buf, size_t len );
    void ( *write_register )( void *buf, size_t len );
    /*  int (*write_vmem)(struct fbtft_par *par, size_t offset, size_t len);
    
        void (*set_addr_win)(struct fbtft_par *par,
                     int xs, int ys, int xe, int ye);
        void (*reset)(struct fbtft_par *par);
        void (*mkdirty)(struct fb_info *info, int from, int to);
        void (*update_display)(struct fbtft_par *par,
                       unsigned int start_line, unsigned int end_line);
        int (*init_display)(struct fbtft_par *par);
        int (*blank)(struct fbtft_par *par, bool on);
    
        unsigned long (*request_gpios_match)(struct fbtft_par *par,
                             const struct fbtft_gpio *gpio);
        int (*request_gpios)(struct fbtft_par *par);
        int (*verify_gpios)(struct fbtft_par *par);
    
        void (*register_backlight)(struct fbtft_par *par);
        void (*unregister_backlight)(struct fbtft_par *par);
    
        int (*set_var)(struct fbtft_par *par);
        int (*set_gamma)(struct fbtft_par *par, u32 *curves);
    */
} st7789v_ops_t;

typedef struct
{
    uint8_t scl;
    uint8_t sda;
    uint8_t cs;
    uint8_t res;
    uint8_t dc;
    uint8_t spi_mode;
    uint8_t spi_bit_order;
    uint32_t spi_speed;
    
    uint16_t width;
    uint16_t height;
    
    uint32_t *framebuffer;
    
    st7789v_ops_t st7789v_ops;
} st7789v_handle_t;

struct st7789v_cmd_param {
    u8 cmd; // command
    u8 len; // parameter length
    u8 *data;   // parameter data
    
    u16 delay_ms;   // delay ms
    u16 delay_us;   // delay us
};

class ST7789V: SPIClass
{
private:

public:
    static st7789v_handle_t m_st7789v_handle;
    
    ST7789V( int scl, int sda, int cs, int dc, int rst );
    ST7789V( int cs, int dc, int rst );
    
    void init( u16 width, u16 height );
    
    /**
     * @brief Set the cs object
     *
     * @param val
     */
    inline static void set_cs( int val )
    {
        if( val )
        {
            digitalWrite( m_st7789v_handle.cs, HIGH );
        }
        else
        {
            digitalWrite( m_st7789v_handle.cs, LOW );
        }
    }
    
    inline static void set_dc( int val )
    {
        if( val )
        {
            digitalWrite( m_st7789v_handle.dc, HIGH );
        }
        else
        {
            digitalWrite( m_st7789v_handle.dc, LOW );
        }
    }
    
    inline static void set_rst( int val )
    {
        if( val ) {
            digitalWrite( m_st7789v_handle.res, HIGH );
        }
        else {
            digitalWrite( m_st7789v_handle.res, LOW );
        }
    }
    
    inline static void set_scl( int val )
    {
        if( val ) {
            digitalWrite( m_st7789v_handle.scl, HIGH );
        }
        else {
            digitalWrite( m_st7789v_handle.scl, LOW );
        }
    }
    
    inline static void set_sda( int val )
    {
        if( val ) {
            digitalWrite( m_st7789v_handle.sda, HIGH );
        }
        else {
            digitalWrite( m_st7789v_handle.sda, LOW );
        }
    }
    
    inline static bool read_sda()
    {
        return digitalRead( m_st7789v_handle.sda );
    }
    
    /**
     * @brief
     *
     * @return u8
     */
    inline static u8 readbyte()
    {
        uint8_t tmp = 0x00;
        st7789v_handle_t *handle = &m_st7789v_handle;
        
        pinMode( handle->sda, INPUT );
        
        for( u8 i = 0; i < 8; i++ )
        {
            set_scl( HIGH );
            
            tmp <<= 1;
            
            if( read_sda() ) {
                tmp++;
            }
            
            set_scl( LOW );
        }
        
        pinMode( handle->sda, OUTPUT );
        
        return tmp;
    }
    
    inline static void writebyte( u8 data )
    {
        st7789v_handle_t *handle = &m_st7789v_handle;
        
        for( u8 i = 0; i < 8; i++ )
        {
            if( data & 0x80 ) {
                set_sda( HIGH );
            }
            else {
                set_sda( LOW );
            }
            
            set_scl( HIGH );
            data <<= 1;
            set_scl( LOW );
        }
        
    }
    
    inline static void write_cmd( u8 cmd )
    {
        set_cs( LOW );
        set_dc( LOW );
        
#if ST7789V_USE_SOFTWARE_SPI
        writebyte( cmd );
#else
        SPI.transfer( cmd );
#endif
        set_cs( HIGH );
    }
    
    inline static void write_data( u8 data )
    {
        set_cs( LOW );
        set_dc( HIGH );
        
#if ST7789V_USE_SOFTWARE_SPI
        writebyte( data );
#else
        SPI.transfer( data );
#endif
        set_cs( HIGH );
    }
    
    inline static void write_wdata( u16 dat )
    {
        set_cs( LOW );
        set_dc( HIGH );
#if ST7789V_USE_SOFTWARE_SPI
        writebyte( dat >> 8 );
        writebyte( dat );
#else
        SPI.transfer( dat >> 8 );
        SPI.transfer( dat );
#endif
        set_cs( HIGH );  
    }
    
    inline static void send_command( u8 cmd, const u8 *buf, u8 lens )
    {
        /*
        set_cs( LOW );
        set_dc( LOW );
        writebyte( cmd );
        
        set_dc( HIGH );
        
        Serial.print( "cmd: " );
        Serial.print( cmd, HEX );
        Serial.print( " " );
        
        for( u8 i = 0; i < lens; i++ )
        {
            Serial.print( " " );
            Serial.print( *buf, HEX );
            writebyte( *buf++ );
        }
        
        Serial.println();
        
        set_cs( HIGH );
        */

               Serial.print( "cmd: " );
        Serial.print( cmd, HEX );
        Serial.print( " " );
        write_cmd( cmd );
        
        for( u8 i = 0; i < lens; i++ )
        {
            Serial.print( " " );
            Serial.print( *buf, HEX );
            write_data( *buf++ );
        }
        Serial.println();
    }
    
    inline static void read_command8( u8 cmd, u8 index )
    {
        u8 result;
        set_cs( LOW );
        set_dc( LOW );
        writebyte( cmd );
        
        set_dc( HIGH );
        
        do
        {
            result = readbyte();
            Serial.print( "read_command8: " );
            Serial.print( result );
        }
        while( index-- );
        
        Serial.println();
        set_cs( HIGH );
    }
    
    /**
     * @brief
     *
     * @param cmd
     * @param buf
     * @param lens
     */
    inline static void read_command_lens( u8 cmd, u8 *buf, u8 lens )
    {
        u8 result;
        set_cs( LOW );
        set_dc( LOW );
        writebyte( cmd );
        
        set_dc( HIGH );
        
        for( u8 i = 0; i < lens; i++ )
        {
            result = readbyte();
            buf[i] = result;
            // Serial.println(result, HEX);
        }
        
        set_cs( HIGH );
    }
    
    inline static u8 st7788v_rw_byte( u8 data )
    {
        u8 tmp;
        st7789v_handle_t *handle = &m_st7789v_handle;
        
        digitalWrite( handle->dc, 0 );
        digitalWrite( handle->cs, 0 );
        
        writebyte( data );
        tmp = readbyte();
        
        digitalWrite( handle->cs, 1 );
        digitalWrite( handle->dc, 1 );
        
        return tmp;
    }
    
    inline static void st7789v_write_then_readlens( u8 data, u8 *buf, u8 lens )
    {
        st7789v_handle_t *handle = &m_st7789v_handle;
        
        digitalWrite( handle->cs, LOW );
        digitalWrite( handle->dc, LOW );
        
        writebyte( data );
        
        digitalWrite( handle->dc, HIGH );
        
        for( int i = 0; i < lens; i++ )
        {
            buf[i] = readbyte();
            Serial.println( buf[i], HEX );
        }
        
        digitalWrite( handle->cs, HIGH );
        
    }
    
    // BASIC API ***************************************************
    inline static void set_col_addr( u16 x0, u16 x1 )
    {
        // u8 col_start_end[] = {( u8 )( x0 >> 8 ), ( u8 )( x0 ), ( u8 )( x1 >> 8 ), ( u8 )( x1 )};
        WDATA wdata_x0, wdata_x1;
        wdata_x0.w = x0;
        wdata_x1.w = x1;
        
        u8 col_start_end[] = {wdata_x0.bytes.msb,
                              wdata_x0.bytes.lsb,
                              wdata_x1.bytes.msb,
                              wdata_x1.bytes.lsb
                             };
        // column start and end
        send_command( 0x2A, col_start_end,
                      sizeof( col_start_end ) / sizeof( col_start_end[0] ) );
    }
    
    inline static void set_row_addr( u16 y0, u16 y1 )
    {
        WDATA wdata_y0, wdata_y1;
        wdata_y0.w = y0;
        wdata_y1.w = y1;
        //u8 row_start_end[] = {( u8 )( y0 >> 8 ), ( u8 )( y0 ), ( u8 )( y1 >> 8 ), ( u8 )( y1 )};
        u8 row_start_end[] = {wdata_y0.bytes.msb,
                              wdata_y0.bytes.lsb,
                              wdata_y1.bytes.msb,
                              wdata_y1.bytes.lsb
                             };
        // row start and end
        send_command( 0x2B, row_start_end,
                      sizeof( row_start_end ) / sizeof( row_start_end[0] ) );
    }
    
    inline static void set_addr( u16 x1, u16 y1, u16 x2, u16 y2 )
    {
        // set_col_addr( x1, x2 );
        // set_row_addr( y1, y2 );
        // write_cmd( 0x2C );
        write_cmd( 0x2A );
        write_wdata( x1 );
        write_wdata( x2 );
        write_cmd( 0x2B );
        write_wdata( y1 );
        write_wdata( y2 );
        write_cmd( 0x2C );
    }
    
    inline static void set_display_power( bool on )
    {
        if( on ) {
            send_command( 0x29, NULL, 0 );
        }
        else {
            send_command( 0x28, NULL, 0 );
        }
    }
    
    // DRAW API ***************************************************
    inline static void clear_screen_directly( u16 color )
    {
        // set_addr( 0, 0, 240, 320 );
        set_addr(0,0,240, 135);
        u16 i, j;
        
        for( i = 0; i < 320; i++ )
        {
            for( j = 0; j < 240; j++ )
            {
                write_wdata( color );
            }
        }
        
        // for( int k = 0; k < 200; k++ ) {
        //     write_data( color >> 8 );
        //     write_data( color );
        // }
    }

    inline static void set_rotation( u8 rotation )
    {
        u8 r = rotation % 4;
        u8 param_rotation;

        switch (r)
        {
        case 0:
            param_rotation = (3 << 6);
            break;
        
        case  1:
            param_rotation = (2 << 6);
            break;

        default:
            break;
        }
        
        
        //send_command( 0x36, param_rotation, sizeof( param ) / sizeof( param[0] ) );
    }
    
    inline static void put_pixel( u16 x, u16 y,
                                  u16 color )
    {
        /*
        if( x >= handle->width || y >= handle->height ) {
            return;
        }
        
        u16 *p = ( u16 * )handle->framebuffer + x + y * handle->width;
        *p = color;
        */
        set_addr( x, y, x + 1, y + 1 );
        write_wdata( color );
    }
    
protected:
    inline static void init_display( const void *cmdList, size_t cmdLen )
    {
        struct st7789v_cmd_param *disp_cmd = ( struct st7789v_cmd_param * )cmdList;
        
        // write command list
        for( u8 i = 0; i < cmdLen; i++ )
        {
            send_command( disp_cmd->cmd, disp_cmd->data, disp_cmd->len );
            delay_us( disp_cmd->delay_ms * 1000 + disp_cmd->delay_us );
            disp_cmd++;
        }
        
        // others init
        set_display_power( true );
        // clear_screen_directly( 0xaaaa );
    }
    
};

// extern ST7789V st7789v;
#endif