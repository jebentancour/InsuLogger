#include <stdint.h>

/**@brief Funcion de inicializacion del modulo.
 */
void i2c_init(void);


/**@brief Funcion para setear la flag donde indicar el fin de trasnmision de un byte.
 *
 * @param main_tx_flag    Puntero a una flag donde se indicara el fin de trasnmision de un byte.
 */
void i2c_tx_set_flag(volatile uint8_t* main_tx_flag);


/**@brief Funcion para iniciar una transferencia de datos a un esclavo.
 *
 * @param address    La direccion de 7 bits del esclavo.
 */
void i2c_begin_transmission(uint8_t address);


/**@brief Funcion para trasnmitir un byte a un esclavo.
 *
 * @param value    El dato a trasnmitir.
 */
void i2c_write(uint8_t value);


/**@brief Funcion para terminar la trasnsaccion.
 */
void i2c_end_transmission(void);