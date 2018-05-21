#include <stdint.h>


/**@brief Funcion de inicializacion del modulo.
 *
 * @warning Se debe inicializar el modulo ble_uart antes de llamar a esta funcion
 */
void rtc_init(void);


/**@brief Funcion para setear la flag donde indicar que hubo un incremento en el rtc.
 *
 * @param main_tick_flag    Puntero a una flag donde se indicara que que hubo un incremento en el rtc.
 */
void rtc_tick_set_flag(volatile uint8_t* main_tick_flag);


/**@brief Funcion para llevar a cero la cuenta del rtc.
 */
void rtc_reset(void);


/**@brief Funcion para obtener la cuenta del rtc.
 *
 * @return rtc        La cuenta del rtc en ms.
 */
uint32_t rtc_get(void);


/**@brief Funcion para sobreescribir la cuenta del rtc.
 *
 * @param rtc_counter_new    Nuevo valor para el rtc en ms.
 */
void rtc_set(uint32_t rtc_counter_new);