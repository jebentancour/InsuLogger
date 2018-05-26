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

/**@brief Funcion de inicializacion del modulo.
 *
 * @warning Se debe inicializar el modulo i2c antes de llamar a esta funcion.
 */
void display_init(void);

/**@brief Funcion para enviar un comando al display.
 *
 * @param command    Comando a enviar.
 */
void display_send_command(uint8_t command);

/**@brief Funcion para enviar un dato al display.
 *
 * @param data    Dato a enviar.
 */
void display_send_data(uint8_t data);

/**@brief Funcion para setear el comienzo del caracter en el display.
 *
 * |X\Y| 4| 5| 6| 7| 8| 9|10|11|
 * |4  |  |  |  |  |  |  |  |  |
 * |5  |  |  |  |  |  |  |  |  |
 * |6  |  |  |  |  |  |  |  |  |
 * |7  |  |  |  |  |  |  |  |  |
 *
 * @param row    Fila  (entre 4 y 7).
 * @param col    Columna  (entre 4 y 11).
 */
void display_set_text_xy(uint8_t row, uint8_t col);

/**@brief Funcion para borrar el contenido del display.
 */
void display_clear(void);

/**@brief Funcion para desplegar un caracter en el display.
 *
 * La posicion horizontal es incrementada automaticamente.
 *
 * @param ch    Caracter a mostrar.
 */
void display_put_char(uint8_t ch);

/**@brief Funcion para desplegar un numero en el display.
 *
 * La posicion horizontal es incrementada automaticamente.
 *
 * @param n   Numero a mostrar (de 0 a 9).
 */
void display_put_number(uint8_t n);


/**@brief Funcion para desplegar un buffer en el display.
 *
 * La posicion horizontal es incrementada automaticamente.
 *
 * @param buffer   Puntero al inicio del buffer.
 * @param len      Largo del buffer.
 */
void display_print(uint8_t* buffer, uint8_t len);