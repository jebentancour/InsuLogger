#include <stdint.h>

/**

*/
void gpio_inti();


/**

*/
void gpio_boton_ok_set_flag(volatile uint8_t* main_boton_ok_flag);

/**

*/
void gpio_boton_down_set_flag(volatile uint8_t* main_boton_down_flag);

/**

*/
void gpio_boton_up_set_flag(volatile uint8_t* main_boton_up_flag);

/**

*/
void gpio_led_on();

/**

*/
void gpio_led_off();