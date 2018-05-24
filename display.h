#include <stdint.h>

#define SSD1306_Address               0x3C
#define SSD1306_Command_Mode          0x80
#define SSD1306_Data_Mode             0x40
#define SSD1306_Display_Off_Cmd       0xAE
#define SSD1306_Display_On_Cmd        0xAF
#define SSD1306_Normal_Display_Cmd    0xA6
#define SSD1306_Inverse_Display_Cmd   0xA7
#define SSD1306_Activate_Scroll_Cmd   0x2F
#define SSD1306_Dectivate_Scroll_Cmd  0x2E
#define SSD1306_Set_Brightness_Cmd    0x81

void display_init(void);

void display_set_normal_display(void);

void display_set_inverse_display();

void display_send_command(uint8_t command);

void display_send_data(uint8_t data);

void display_set_page_mode(void);

void display_set_text_xy(uint8_t row, uint8_t col);

void display_clear(void);
    
void display_set_brightness(uint8_t brightness);
    
void display_put_char(uint8_t ch);

void display_put_number(uint8_t n);

void display_print(uint8_t* buffer, uint8_t len);