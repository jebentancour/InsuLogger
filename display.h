/**
 * @defgroup DISPLAY
 * @{
 *
 * @paragraph
 * 
 * Proporciona funciones para la inicializar el hardware y controlar los píxeles.
 * El circuito integrado utilizado para manejar el display es el SSD1306.
 * Este módulo fue creado utilizando como base la librería para Arduino: 
 * https://github.com/kodera2t/nRF51822_sample/tree/master/ACROBOTIC_SSD1306
 *
 * @file display.h
 * 
 * @version 1.0
 * @author  Rodrigo De Soto, Maite Gil, José Bentancour.
 * @date 12 Julio 2018
 *
 * @brief Módulo que permite el manejo del display utilizado.
 */

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


/**@brief Función de Inicialización del módulo.
 *
 * @warning Se debe inicializar el módulo i2c antes de llamar a esta función.
 */
void display_init(void);


/**@brief Función para enviar un comando al display.
 *
 * @param command    Comando a enviar.
 */
void display_send_command(uint8_t command);


/**@brief Función para enviar un dato al display.
 *
 * @param data    Dato a enviar.
 */
void display_send_data(uint8_t data);


/**@brief Función para setear el comienzo del caracter en el display.
 *
 * |X\Y| 4| 5| 6| 7| 8| 9|10|11|
 *
 * |4  |  |  |  |  |  |  |  |  |
 *
 * |5  |  |  |  |  |  |  |  |  | 
 *
 * |6  |  |  |  |  |  |  |  |  |
 *
 * |7  |  |  |  |  |  |  |  |  |
 *
 * @param row    Fila  (entre 4 y 7).
 * @param col    Columna  (entre 4 y 11).
 */
void display_set_text_xy(uint8_t row, uint8_t col);


/**@brief Función para borrar el contenido del display.
 */
void display_clear(void);


/**@brief Función para desplegar un caracter en el display.
 *
 * La posición horizontal es incrementada automáticamente.
 *
 * @param ch    Caracter a mostrar.
 */
void display_put_char(uint8_t ch);


/**@brief Función para desplegar un numero en el display.
 *
 * La posición horizontal es incrementada automáticamente.
 *
 * @param n   Numero a mostrar (de 0 a 9).
 */
void display_put_number(uint8_t n);


/**@brief Función para desplegar un buffer en el display.
 *
 * La posición horizontal es incrementada automáticamente.
 *
 * @param buffer   Puntero al inicio del buffer.
 * @param len      Largo del buffer.
 */
void display_print(uint8_t* buffer, uint8_t len);