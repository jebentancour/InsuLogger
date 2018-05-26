#include <stdint.h>

#include "display.h"
#include "font8x8.h"
#include "i2c.h"

static const uint8_t* m_font;      // Current font.
static uint8_t m_font_offset = 2;  // Font bytes for meta data.
static uint8_t m_font_width;       // Font witdth.

static volatile uint8_t i2c_tx_flag;

static void display_set_font(const uint8_t* font)
{
    m_font = font;
    m_font_width = m_font[0];
}

void display_send_command(uint8_t command)
{
    i2c_begin_transmission(SSD1306_Address);      // Begin I2C communication
    i2c_tx_flag = 0;                              // Reset flag
    i2c_write(SSD1306_Command_Mode);              // Set OLED command mode
    while(!i2c_tx_flag){};                        // Wait...
    i2c_tx_flag = 0;                              // Reset flag
    i2c_write(command);                           // Send command
    while(!i2c_tx_flag){};                        // Wait...
    i2c_end_transmission();                       // End I2C communication
}

void display_init(void)
{
    i2c_tx_set_flag(&i2c_tx_flag);         // Initialize I2C
    i2c_tx_flag = 0;
    i2c_init();
    
    display_send_command(0xAE);            // Display off
    display_send_command(0xA6);            // Set Normal Display (default)
    display_send_command(0xAE);            // DISPLAYOFF
    display_send_command(0xD5);            // SETDISPLAYCLOCKDIV
    display_send_command(0x80);            // The suggested ratio 0x80
    display_send_command(0xA8);            // SSD1306_SETMULTIPLEX
    display_send_command(0x3F);               
    display_send_command(0xD3);            // SETDISPLAYOFFSET
    display_send_command(0x0);             // No offset
    display_send_command(0x40|0x0);        // SETSTARTLINE
    display_send_command(0x8D);            // CHARGEPUMP
    display_send_command(0x14);               
    display_send_command(0x20);            // MEMORYMODE
    display_send_command(0x00);            // 0x0 act like ks0108
    display_send_command(0xA1);            // SEGREMAP   Mirror screen horizontally (A0)
    display_send_command(0xC8);            // COMSCANDEC Rotate screen vertically (C0)
    display_send_command(0xDA);            // 0xDA
    display_send_command(0x12);            // COMSCANDEC
    display_send_command(0x81);            // SETCONTRAST
    display_send_command(0xCF);               
    display_send_command(0xd9);            // SETPRECHARGE 
    display_send_command(0xF1);               
    display_send_command(0xDB);            // SETVCOMDETECT                
    display_send_command(0x40);               
    display_send_command(0xA4);            // DISPLAYALLON_RESUME        
    display_send_command(0xA6);            // NORMALDISPLAY             
    display_clear();                          
    display_send_command(0x2E);            // Stop scroll
    display_send_command(0x20);            // Set Memory Addressing Mode
    display_send_command(0x00);            // Set Memory Addressing Mode ab Horizontal addressing mode
    display_set_font(font8x8);
}

void display_set_text_xy(uint8_t row, uint8_t col)
{
    display_send_command(0xB0 + row);                                   // Set page address
    display_send_command(0x00 + (m_font_width * col & 0x0F));           // Set column lower addr
    display_send_command(0x10 + ((m_font_width * col >> 4) & 0x0F));    // Set column higher addr
}

void display_clear(void)
{
  uint8_t i;
  uint8_t j;
  display_send_command(SSD1306_Display_Off_Cmd);     // Display off
  for(j=0;j<8;j++)
  {    
    display_set_text_xy(j,0);    
    {
      for(i=0;i<16;i++)  // Clear all columns
      {
        display_put_char(' ');    
      }
    }
  }
  display_send_command(SSD1306_Display_On_Cmd);     // Display on
  display_set_text_xy(0,0);    
}

void display_send_data(uint8_t data)
{
  i2c_begin_transmission(SSD1306_Address);      // begin I2C communication
  i2c_tx_flag = 0;                              // reset flag
  i2c_write(SSD1306_Data_Mode);                 // Set OLED data mode
  while(!i2c_tx_flag){};                        // wait...
  i2c_tx_flag = 0;                              // reset flag
  i2c_write(data);                              // Send command
  while(!i2c_tx_flag){};                        // wait...
  i2c_end_transmission();                       // End I2C communication
}

void display_put_char(uint8_t ch)
{
    if (!m_font) return;
    // Ignore non-printable ASCII characters. This can be modified for
    // multilingual font.  
    if(ch < 32 || ch > 127) 
    {
        ch = ' ';
    }
    uint8_t i;    
    for(i=0;i<m_font_width;i++)
    {
       // Font array starts at 0, ASCII starts at 32
       display_send_data(m_font[(ch-32)*m_font_width+m_font_offset+i]); 
    }
}

void display_put_number(uint8_t n)
{
    display_put_char('0' + (n % 10));
}

void display_print(uint8_t* buffer, uint8_t len)
{
    uint8_t i;
    for(i=0;i<len;i++)
    {
        display_put_char(buffer[i]);
    }
}