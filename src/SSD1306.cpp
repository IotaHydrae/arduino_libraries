#include "SSD1306.h"

#if SSD1306_BS_MODE_I2C
    #include "Wire.h"
#else
    
#endif

oled_handle_t SSD1306::m_oled_handle = {
    .interface = OLED_INTERFACE_I2C,
    .width = 128,
    .height = 64,
    .depth = OLED_COLOR_DEPTH_1,
    0,
};
uint8_t SSD1306::m_oled_buffer[1024] = {0};

// Constructors ////////////////////////////////////////////////////////////////
SSD1306::SSD1306( oled_size_t width, oled_size_t height,
                  oled_color_depth_t depth,
                  oled_pin_t scl, oled_pin_t sda )
{
    m_oled_handle.interface = OLED_INTERFACE_I2C;
    m_oled_handle.width     = width;
    m_oled_handle.height    = height;
    m_oled_handle.depth     = depth;
    
    m_oled_handle.pin_scl   = scl;
    m_oled_handle.pin_sda   = sda;
    
    m_oled_handle.status    = OLED_STATUS_UNINITIALIZED;
    m_oled_handle.lock      = OLED_LOCKED;
}

SSD1306::SSD1306( oled_size_t width, oled_size_t height,
                  oled_color_depth_t depth,
                  oled_pin_t sclk, oled_pin_t mosi,
                  oled_pin_t miso, oled_pin_t nss )
{
    // m_oled_handle.interface = OLED_INTERFACE_I2C;
    // m_oled_handle.width     = width;
    // m_oled_handle.height    = height;
    // m_oled_handle.depth     = depth;
    
    // m_oled_handle.pin_sclk  = sclk;
    // m_oled_handle.pin_mosi  = mosi;
    // m_oled_handle.pin_miso  = miso;
    // m_oled_handle.pin_nss   = nss;
    
    // m_oled_handle.status    = OLED_STATUS_UNINITIALIZED;
    // m_oled_handle.lock      = OLED_LOCKED;
    
    // init( &m_oled_handle );
}

// Public Methods //////////////////////////////////////////////////////////////

void SSD1306::init( oled_handle_t *handle )
{
    /* setup twi interface */
    Wire.begin();
    Wire.beginTransmission(0x3C);

    return;
    
    /* check if controller still uninitialized */
    if( handle->status == OLED_STATUS_UNINITIALIZED )
    {
    
        /* Send ssd1306 init table */
        write_cmd( 0x01 );
        /* Shared command */
        
        /* Exclusive command */
        
        /* initialize done */
        handle->status = OLED_STATUS_RAEDY;
    }
    
init_failed:
    handle->status = OLED_STATUS_UNINITIALIZED;
    return;
    
}

void SSD1306::test()
{
    Wire.begin();
    Wire.beginTransmission(0x3C);

}
/*
    i2c_start();
    i2c_sendbyte(0x78); //slave address
    i2c_sndack();
    i2c_sendbyte(COMMAND);
    i2c_sndack();
    i2c_sendbyte(val);
    i2c_sndack();
    i2c_stop();
*/
// Private Methods //////////////////////////////////////////////////////////////
void SSD1306::write_cmd( oled_dc_t val )
{
#if SSD1306_BS_MODE_I2C
    /* I2C logic here*/
    Wire.beginTransmission( SSD1306_DEVICE_ADDR );
    // Wire.write( SSD1306_COMMAND );
    // Wire.write( val );
    // Wire.endTransmission();
#else
    /* SPI logic here*/
    
#endif
}

void SSD1306::write_dat( oled_dc_t val )
{
#if SSD1306_BS_MODE_I2C
    /* I2C logic here*/
    Wire.beginTransmission( SSD1306_DEVICE_ADDR );
    Wire.write( SSD1306_DATA );
    Wire.write( val );
    Wire.endTransmission();
#else
    /* SPI logic here*/
#endif
}

SSD1306 ssd1306 = SSD1306( 128, 64, OLED_COLOR_DEPTH_1, 19, 18 );