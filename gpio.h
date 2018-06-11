/**
 * @defgroup GPIO
 * @{
 *
 * @paragraph 
 * 
 * Este módulo pertenece a la capa de abstracción de hardware y proporciona interfaces
 * para generar los eventos con los botones y manipular un led.
 *
 * @file gpio.h
 * 
 * @version 1.0
 * @author  Rodrigo De Soto, Maite Gil, José Bentancour.
 * @date 12 Julio 2018
 * 
 * @brief Módulo que proporciona funciones para manejar pines de entrada y salida.
 */
 
#include <stdint.h>


/**@brief Función de inicialización del módulo.
 */
void gpio_init(void);


/**@brief Función para setear la flag donde indicar que el botón OK fue presionado.
 *
 * @param main_botón_ok_flag    Puntero a una flag donde se indicara que el botón OK fue presionado.
 */
void gpio_boton_ok_set_flag(volatile uint8_t* main_boton_ok_flag);


/**@brief Función para setear la flag donde indicar que el botón DOWN fue presionado.
 *
 * @param main_botón_down_flag    Puntero a una flag donde se indicara que el botón DOWN fue presionado.
 */
void gpio_boton_down_set_flag(volatile uint8_t* main_boton_down_flag);

    
/**@brief Función para setear la flag donde indicar que el botón UP fue presionado.
 *
 * @param main_botón_up_flag    Puntero a una flag donde se indicara que el botón UP fue presionado.
 */
void gpio_boton_up_set_flag(volatile uint8_t* main_boton_up_flag);


/**@brief Función para encender el LED.
 */
void gpio_led_on();


/**@brief Función para apagar el LED.
 */
void gpio_led_off();


/**@brief Función para conmutar el estado del LED.
 */
void gpio_led_toggle();