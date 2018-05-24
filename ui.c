#include "ui.h"

#include <stdint.h>

#include "nrf_log.h"
#include "rtc.h"
#include "gpio.h"
#include "ble_uart.h"
#include "i2c.h"
#include "display.h"

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
#define BYE_TICKS       4       /* 0.5 s        */

static uint8_t m_sinc_i;

static internal_state_t        m_state;
static uint8_t                 m_timer;

static ble_uart_status_t ui_ble_uart_status;

/**@brief Funcion de inicializacion del modulo.
 *
 * @warning Se debe inicializar el modulo gpio antes de llamar a esta funcion
 */
void ui_init(void)
{
    m_state = off;
    gpio_led_off();
    i2c_init();	
    display_init();
    display_clear();
}

static void ui_process_display(void)
{    
    switch(m_state)
    {
        case off:
            // La pantalla esta apagada
            display_clear();
            break;
        case hello:
            // La pantalla muestra: InsuLogger
            //display_clear();
            display_set_text_xy(5,4);
            display_print("Insu",4);
            display_set_text_xy(6,6);
            display_print("Logger",6);
            break;
        case show_time:
            // La pantalla muestra el tiempo transcurrido desde la ultima inyeccion
            //display_clear();
            display_set_text_xy(4,4);
            display_print("A:      ",8);
            display_set_text_xy(5,4);
            display_print("   HH:MM",8);
            display_set_text_xy(6,4);
            display_print("B:      ",8);
            display_set_text_xy(7,4);
            display_print("   HH:MM",8);
            break;
        case menu_a:
            // La pantalle muestra el menu con opcion Nueva dosis seleccionada
            //display_clear();
            display_set_text_xy(4,4);
            display_print("Menu:",5);
            display_set_text_xy(5,4);
            display_print(">Regist.",8);
            display_set_text_xy(6,4);
            display_print(" Sinc.",6);            
            break;
        case menu_b:
            // La pantalle muestra el menu con opcion Sinconizar seleccionada
            //display_clear();
            display_set_text_xy(4,4);
            display_print("Menu:",5);
            display_set_text_xy(5,4);
            display_print(" Regist.",8);
            display_set_text_xy(6,4);
            display_print(">Sinc.",6); 
            break;
        case sync:
            //display_clear();
            display_set_text_xy(4,4);
            switch(m_sinc_i)
            {
                case 0: 
                    display_print("Sinc.  ",7);
                    break;
                case 1: 
                    display_print("Sinc .  ",7);
                    break;
                case 2:
                    display_print("Sinc  .",7);
                    break;
                default:
                    m_sinc_i = 0;
            }
            m_sinc_i++;
            if(m_sinc_i >= 3)
            {
                m_sinc_i = 0;
            }
            break;
        case input_glicemia_cent:
            //display_clear();
            break;
        case input_glicemia_dec:
            //display_clear();
            break;
        case input_glimemia_un:
            //display_clear();
            break;
        case input_type_a:
            //display_clear();
            break;
        case input_type_b:
            //display_clear();
            break;
        case input_dosis:
            //display_clear();
            break;
        case bye:
            // La pantalla muestra un mensaje de despedida
            //display_clear();
            display_set_text_xy(5,4);
            display_print("Hasta",5);
            display_set_text_xy(6,6);
            display_print("luego!",6);
            break;
        default:
            // Nunca deberia llegar a este lugar
            display_clear();
            break;
    }    
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
                m_timer = HELLO_TICKS;
                m_state = hello;
                ui_process_display();
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
                    ui_process_display();
                }
            }
            break;
        case show_time:
            // La pantalla muestra el tiempo transcurrido desde la ultima inyeccion
            if(event == time_update){
                m_timer -= 1;
                NRF_LOG_INFO("UI: Show time %d\r\n", rtc_get());
                if(m_timer == 0)
                {
                    m_timer = BYE_TICKS;
                    m_state = bye;
                    display_clear();
                    ui_process_display();
                }
            }
            if(event == pressed_ok){
                NRF_LOG_INFO("UI: Menu -> Nuevo\r\n");
                m_state = menu_a;
                display_clear();
                ui_process_display();
            }
            break;
        case menu_a:
            // La pantalle muestra el menu con opcion Nueva dosis seleccionada
            if(event == pressed_up){
                NRF_LOG_INFO("UI: Menu -> Nuevo\r\n");
                m_state = menu_a;
                ui_process_display();
            }
            if(event == pressed_down){
                NRF_LOG_INFO("UI: Menu -> Sincronizar\r\n");
                m_state = menu_b;
                ui_process_display();
            }
            if(event == pressed_ok){
                NRF_LOG_INFO("UI: Glicemia\r\n");
                m_state = input_glicemia_cent;
                display_clear();
                ui_process_display();
            }
            break;
        case menu_b:
            // La pantalle muestra el menu con opcion Sinconizar seleccionada
            if(event == pressed_up){
                NRF_LOG_INFO("UI: Menu -> Nuevo\r\n");
                m_state = menu_a;
                ui_process_display();
            }
            if(event == pressed_down){
                NRF_LOG_INFO("UI: Menu -> Sincronizar\r\n");
                m_state = menu_b;
                ui_process_display();
            }
            if(event == pressed_ok){
                NRF_LOG_INFO("UI: Sincronizando...\r\n");
                m_sinc_i = 0;
                ble_uart_advertising_start();
                m_state = sync;
                display_clear();
                ui_process_display();
            }
            break;
        case sync:
            if(event == time_update){
                ui_process_display();
                ui_ble_uart_status = ble_uart_get_status();
                if(ui_ble_uart_status.advertising)
                {
                    gpio_led_toggle();
                }
                if(ui_ble_uart_status.connected)
                {
                    gpio_led_on();
                }
                if(!ui_ble_uart_status.advertising && !ui_ble_uart_status.connected){
                    m_timer = BYE_TICKS;
                    m_state = bye;
                    display_clear();
                    ui_process_display();
                }
            }
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
                    ui_process_display();
                }
            }
            break;
        default:
            // Nunca deberia llegar a este lugar
            NRF_LOG_INFO("UI: invalid state\r\n");
            gpio_led_off();
            m_state = off;
            ui_process_display();
            break;
    }
}

/**@brief Funcion para apagar ui.
 */
void ui_off(void)
{
    if(m_state != off){
        NRF_LOG_INFO("UI: External off\r\n");
        display_clear();
        gpio_led_off();
        m_state = off;
    }
}