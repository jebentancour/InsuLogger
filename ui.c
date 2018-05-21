#include "ui.h"

#include <stdint.h>

#include "nrf_log.h"
#include "rtc.h"
#include "gpio.h"

/* Estructura interna para representar el estado */
typedef enum {
    off,
    hello,
    show_time,
    menu_a,
    menu_b,
    sync,
    input_glicemia_cent,
    input_glicemia_dec,
    input_glimemia_un,
    input_type_a,
    input_type_b,
    input_dosis,
    bye    
} internal_state_t;

#define HELLO_TICKS     4       /* 1 s          */
#define SHOW_TICKS      40      /* 10 s         */
#define BYE_TICKS       2       /* 0.5 s        */

static  internal_state_t        m_state;
static  uint8_t                 m_timer;

/**@brief Funcion de inicializacion del modulo.
 *
 * @warning Se debe inicializar el modulo gpio antes de llamar a esta funcion
 */
void ui_init(void)
{
    gpio_led_off();
    m_state = off;
}

/**@brief Funcion procesar los eventos.
 *
 * @param event    Evento del tipo event_t.
 */
void ui_process_event(event_t event)
{
    switch(m_state)
    {
        case off:
            // La pantalla esta apagada
            if(event == pressed_ok){
                NRF_LOG_INFO("UI: Hello!\r\n");
                gpio_led_on();
                m_timer = HELLO_TICKS;
                m_state = hello;
            }
            break;
        case hello:
            // La pantalla muestra: InsuLogger
            if(event == time_update){
                m_timer -= 1;
                NRF_LOG_INFO("UI: Hello time %d\r\n", m_timer);
                if(m_timer == 0)
                {
                    m_timer = SHOW_TICKS;
                    m_state = show_time;
                }
            }
            break;
        case show_time:
            // La pnatalla muestra el tiempo transcurrido desde la ultima inyeccion
            if(event == time_update){
                m_timer -= 1;
                NRF_LOG_INFO("UI: Show time %d\r\n", rtc_get());
                if(m_timer == 0)
                {
                    m_timer = BYE_TICKS;
                    m_state = bye;
                }
            }
            if(event == pressed_ok){
                NRF_LOG_INFO("UI: Menu\r\n");
                m_state = menu_a;
            }
            break;
        case menu_a:
            // La pantalle muestra el menu con opcion Nueva dosis seleccionada
            break;
        case menu_b:
            // La pantalle muestra el menu con opcion Sinconizar seleccionada
            break;
        case sync:
            break;
        case input_glicemia_cent:
            break;
        case input_glicemia_dec:
            break;
        case input_glimemia_un:
            break;
        case input_type_a:
            break;
        case input_type_b:
            break;
        case input_dosis:
            break;
        case bye:
            // La pantalla muestra un mensaje de despedida
            if(event == time_update){
                m_timer -= 1;
                NRF_LOG_INFO("UI: Bye time %d\r\n", m_timer);
                if(m_timer == 0)
                {
                    NRF_LOG_INFO("UI: Off\r\n");
                    gpio_led_off();
                    m_state = off;
                }
            }
            break;
        default:
            // Nunca deberia llegar a este lugar
            NRF_LOG_INFO("UI: invalid state\r\n");
            gpio_led_off();
            m_state = off;
            break;
    }
}

/**@brief Funcion para apagar ui.
 */
void ui_off(void)
{
    if(m_state != off){
        NRF_LOG_INFO("UI: External off\r\n");
        gpio_led_off();
        m_state = off;
    }
}