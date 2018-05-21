#include <stdint.h>


/**@brief Funcion de inicializacion del modulo.
 */
void gpio_init(void);


/**@brief Funcion para setear la flag donde indicar que el boton OK fue presionado.
 *
 * @param main_boton_ok_flag    Puntero a una flag donde se indicara que el boton OK fue presionado.
 */
void gpio_boton_ok_set_flag(volatile uint8_t* main_boton_ok_flag);


/**@brief Funcion para setear la flag donde indicar que el boton DOWN fue presionado.
 *
 * @param main_boton_down_flag    Puntero a una flag donde se indicara que el boton DOWN fue presionado.
 */
void gpio_boton_down_set_flag(volatile uint8_t* main_boton_down_flag);

    
/**@brief Funcion para setear la flag donde indicar que el boton UP fue presionado.
 *
 * @param main_boton_up_flag    Puntero a una flag donde se indicara que el boton UP fue presionado.
 */
void gpio_boton_up_set_flag(volatile uint8_t* main_boton_up_flag);


/**@brief Funcion para encender el LED.
 */
void gpio_led_on();


/**@brief Funcion para apagar el LED.
 */
void gpio_led_off();


/**@brief Funcion para conmutar el estado del LED.
 */
void gpio_led_toggle();