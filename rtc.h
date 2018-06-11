/**
 * @defgroup RTC
 * @{
 *
 * @file rtc.h
 * 
 * @version 1.0
 * @author  Rodrigo De Soto, Maite Gil, José Bentancour.
 * @date 12 Julio 2018
 * 
 * @brief Módulo que implementa un reloj de tiempo real (Real Time Clock).
 *
 * @paragraph 
 * 
 * Es utilizado para llevar la cuenta del tiempo entre los eventos.
 * El SoC cuenta con dos Real Time Counters (RTC0 y RTC1) uno de ellos es utilizado por SOFTDEVICE (RTC0) y 
 * el otro por TIMER LIBRARY (RTC1). 
 * Por esto el módulo RTC utiliza la librería TIMER LIBRARY que permite crear diferentes timers en software 
 * partiendo de un mismo recurso de hardware.
 */
 
#include <stdint.h>


/**@brief Función de inicialización del módulo.
 *
 * @warning Se debe inicializar el módulo ble_uart antes de llamar a esta función.
 */
void rtc_init(void);


/**@brief Función para setear la flag donde indicar que hubo un incremento en el rtc.
 *
 * @param main_tick_flag    Puntero a una flag donde se indicara que que hubo un incremento en el rtc.
 */
void rtc_tick_set_flag(volatile uint8_t* main_tick_flag);


/**@brief Función para llevar a cero la cuenta del rtc.
 */
void rtc_reset(void);


/**@brief Función para obtener la cuenta del rtc.
 *
 * @return rtc        La cuenta del rtc en ms.
 */
uint32_t rtc_get(void);


/**@brief Función para sobreescribir la cuenta del rtc.
 *
 * @param rtc_counter_new    Nuevo valor para el rtc en ms.
 */
void rtc_set(uint32_t rtc_counter_new);
