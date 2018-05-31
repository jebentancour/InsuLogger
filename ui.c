#include "ui.h"

#include <stdint.h>

#include "nrf_log.h"
#include "rtc.h"
#include "gpio.h"
#include "ble_uart.h"
#include "i2c.h"
#include "display.h"
#include "logger.h"

/* Estructura interna para representar el estado */
typedef enum {
    off,
    hello,
    show_time,
    menu_a,
    menu_b,
    sync,
    input_glucemia_cent,
    input_glucemia_dec,
    input_glucemia_un,
    input_type_a,
    input_type_b,
    input_dosis,
    bye    
} internal_state_t;

#define HELLO_TICKS     4       /* 1 s          */
#define SHOW_TICKS      40      /* 10 s         */
#define BYE_TICKS       4       /* 1 s          */

static uint8_t m_sinc_i;

static internal_state_t        m_state;
static uint8_t                 m_timer;

static uint8_t                m_glucemia_un;
static uint8_t                m_glucemia_dec;
static uint8_t                m_glucemia_cent;
static uint8_t                m_type;           /* 0 -> A, 1 -> B */
static uint8_t                m_dosis;

static ble_uart_status_t ui_ble_uart_status;

/**@brief Funcion de inicializacion del modulo.
 *
 * @warning Se debe inicializar el modulo gpio antes de llamar a esta funcion
 */
void ui_init(void)
{
    m_state = off;
    m_glucemia_un = 0;
    m_glucemia_dec = 0;
    m_glucemia_cent = 0;
    m_type = 0;
    m_dosis = 0;
    gpio_led_off();	
    display_init();
    display_clear();
}

/**@brief Funcion interna para manejar el contenido del display.
 */
static void ui_process_display(void)
{    
    switch(m_state)
    {
        case off:
            display_clear();
            break;
        case hello:
            display_set_text_xy(5,4);
            display_print("Insu",4);
            display_set_text_xy(6,6);
            display_print("Logger",6);
            break;
        case show_time:
            display_set_text_xy(4,4);
            display_print("A:",2);
            
            uint32_t ms;
            ms = logger_get_last_a();
            uint32_t s;
            s = (ms / 1000) % 60;
            uint32_t m;
            m = (ms / 60000) % 60;
            uint32_t h;
            h = ms / 3600000;
            
            display_set_text_xy(5,4);
            display_put_number((h/10)%10);
            display_put_number(h%10);
            display_put_char(':');
            display_put_number((m/10)%10);
            display_put_number(m%10);
            display_put_char(':');
            display_put_number((s/10)%10);
            display_put_number(s%10);           
            
            display_set_text_xy(6,4);
            display_print("B:      ",8);
            
            ms = logger_get_last_b();
            s = (ms / 1000) % 60;
            m = (ms / 60000) % 60;
            h = ms / 3600000;
            
            display_set_text_xy(7,4);
            display_put_number((h/10)%10);
            display_put_number(h%10);
            display_put_char(':');
            display_put_number((m/10)%10);
            display_put_number(m%10);
            display_put_char(':');
            display_put_number((s/10)%10);
            display_put_number(s%10);
            break;
        case menu_a:
            display_set_text_xy(4,4);
            display_print("Menu:",5);
            display_set_text_xy(5,4);
            display_print(">Regist.",8);
            display_set_text_xy(6,4);
            display_print(" Sinc.",6);
            display_set_text_xy(7,4);
            display_print("        ",8);
            break;
        case menu_b:
            display_set_text_xy(5,4);
            display_print(" Regist.",8);
            display_set_text_xy(6,4);
            display_print(">Sinc.",6); 
            break;
        case sync:
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
        case input_glucemia_un:
            display_set_text_xy(4,4);
            display_print("Glucem.:",8);
            display_set_text_xy(5,4);
            display_print("        ",8);
            display_set_text_xy(6,4);
            display_put_number(m_glucemia_cent);
            display_put_number(m_glucemia_dec);
            display_put_number(m_glucemia_un);
            display_print("mg/dL",5);
            display_set_text_xy(7,4);
            display_print("  ^",3); 
            break;
        case input_glucemia_dec:
            display_set_text_xy(6,4);
            display_put_number(m_glucemia_cent);
            display_put_number(m_glucemia_dec);
            display_put_number(m_glucemia_un);
            display_set_text_xy(7,4);
            display_print(" ^ ",3); 
            break;
        case input_glucemia_cent:         
            display_set_text_xy(6,4);
            display_put_number(m_glucemia_cent);
            display_put_number(m_glucemia_dec);
            display_put_number(m_glucemia_un);
            display_set_text_xy(7,4);
            display_print("^  ",3);     
            break;
        case input_type_a:
            display_set_text_xy(4,4);
            display_print("Tipo:   ",8);
            display_set_text_xy(5,4);
            display_print(">A      ",8);
            display_set_text_xy(6,4);
            display_print(" B      ",8);
            display_set_text_xy(7,4);
            display_print("        ",8); 
            break;
        case input_type_b:
            display_set_text_xy(4,4);
            display_print("Tipo:   ",8);
            display_set_text_xy(5,4);
            display_print(" A      ",8);
            display_set_text_xy(6,4);
            display_print(">B      ",8);
            display_set_text_xy(7,4);
            display_print("        ",8); 
            break;
        case input_dosis:
            display_set_text_xy(4,4);
            display_print("Dosis:",6);
            display_set_text_xy(5,4);
            display_print("  ",2); 
            display_set_text_xy(6,4);
            display_print("    ",4);
            uint8_t cent;
            cent = m_dosis / 100;
            if(cent > 0)
            {
                display_put_number(cent);
            }
            else
            {
                display_put_char(' ');
            }
            uint8_t dec;
            dec = (m_dosis % 100) / 10;
            if((dec > 0)||(cent >0))
            {
                display_put_number(dec);
            }
            else
            {
                display_put_char(' ');
            }
            uint8_t un;
            un = m_dosis % 10;
            display_put_number(un);
            display_put_char('U');
            break;
        case bye:
            display_set_text_xy(4,4);
            display_print("        ",8);
            display_set_text_xy(5,4);
            display_print("Hasta   ",8);
            display_set_text_xy(6,4);
            display_print("  luego!",8);
            display_set_text_xy(7,4);
            display_print("        ",8);
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
                m_state = hello;
                ui_process_display();
                m_timer = HELLO_TICKS;
            }
            break;
        case hello:
            // La pantalla muestra: InsuLogger
            if(event == time_update){
                m_timer -= 1;
                if(m_timer == 0)
                {
                    m_state = show_time;
                    ui_process_display();
                    m_timer = SHOW_TICKS;
                }
            }
            break;
        case show_time:
            // La pantalla muestra el tiempo transcurrido desde la ultima inyeccion
            if(event == time_update){
                ui_process_display();
                if(m_timer == 0)
                {
                    m_state = bye;
                    ui_process_display();
                    m_timer = BYE_TICKS;
                }
            }
            if(event == pressed_ok){
                m_state = menu_a;
                ui_process_display();
            }
            break;
        case menu_a:
            // La pantalle muestra el menu con opcion Nueva dosis seleccionada
            if(event == pressed_up){
                m_state = menu_a;
                ui_process_display();
            }
            if(event == pressed_down){
                m_state = menu_b;
                ui_process_display();
            }
            if(event == pressed_ok){
                m_state = input_glucemia_un;
                ui_process_display();
            }
            break;
        case menu_b:
            // La pantalle muestra el menu con opcion Sinconizar seleccionada
            if(event == pressed_up){
                m_state = menu_a;
                ui_process_display();
            }
            if(event == pressed_down){
                m_state = menu_b;
                ui_process_display();
            }
            if(event == pressed_ok){
                m_sinc_i = 0;
                ble_uart_advertising_start();
                m_state = sync;
                display_clear();
                ui_process_display();
            }
            break;
        case sync:
            // // La pantalle muestra Sinc...
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
                    m_state = bye;
                    ui_process_display();
                    m_timer = BYE_TICKS;
                }
            }
            break;
        case input_glucemia_un:
            // La pantalle muestra Glucemia con unidades seleccionada
            if(event == pressed_up){
                if(m_glucemia_un < 9)
                {
                  m_glucemia_un++;
                }
                ui_process_display();
            }
            if(event == pressed_down){
                if(m_glucemia_un > 0)
                {
                  m_glucemia_un--;
                }
                ui_process_display();
            }
            if(event == pressed_ok){
                m_state = input_glucemia_dec;
                ui_process_display();
            }
            break;
        case input_glucemia_dec:
            // La pantalle muestra Glucemia con decenas seleccionada
            if(event == pressed_up){
                if(m_glucemia_dec < 9)
                {
                  m_glucemia_dec++;
                }
                ui_process_display();
            }
            if(event == pressed_down){
                if(m_glucemia_dec > 0)
                {
                  m_glucemia_dec--;
                }
                ui_process_display();
            }
            if(event == pressed_ok){
                m_state = input_glucemia_cent;
                ui_process_display();
            }
            break;
        case input_glucemia_cent:
            // La pantalle muestra Glucemia con centenas seleccionada
            if(event == pressed_up){
                if(m_glucemia_cent < 9)
                {
                  m_glucemia_cent++;
                }
                ui_process_display();
            }
            if(event == pressed_down){
                if(m_glucemia_cent > 0)
                {
                  m_glucemia_cent--;
                }
                ui_process_display();
            }
            if(event == pressed_ok){
                if(m_type > 0)
                {
                    m_state = input_type_b;
                }
                else
                {
                    m_state = input_type_a;
                }
                ui_process_display();
            }
            break;
        case input_type_a:
            // La pantalle muestra Tipo con opcion A seleccionada
            if(event == pressed_down){
                m_state = input_type_b;
                ui_process_display();
            }
            if(event == pressed_ok){
                m_type = 0;
                m_state = input_dosis;
                ui_process_display();
            }
            break;
        case input_type_b:
            // La pantalle muestra Tipo con opcion A seleccionada
            if(event == pressed_up){
                m_state = input_type_a;
                ui_process_display();
            }
            if(event == pressed_ok){
                m_type = 1;
                m_state = input_dosis;
                ui_process_display();
            }
            break;
        case input_dosis:
            // La pantalle muestra la cantidad de Dosis
            if(event == pressed_up){
                if(m_dosis < 100)
                {
                  m_dosis++;
                }
                ui_process_display();
            }
            if(event == pressed_down){
                if(m_dosis > 0)
                {
                  m_dosis--;
                }
                ui_process_display();
            }
            if(event == pressed_ok){
                logger_new_register(m_glucemia_un+10*m_glucemia_dec+100*m_glucemia_cent, m_type, m_dosis, rtc_get());
                m_state = bye;
                ui_process_display();
                m_timer = BYE_TICKS;
            }
            break;
        case bye:
            // La pantalla muestra un mensaje de despedida
            if(event == time_update){
                m_timer -= 1;
                if(m_timer == 0)
                {
                    gpio_led_off();
                    m_state = off;
                    ui_process_display();
                }
            }
            break;
        default:
            // Nunca deberia llegar a este lugar
            NRF_LOG_ERROR("UI invalid state\r\n");
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
        NRF_LOG_INFO("ui_off\r\n");
        display_clear();
        gpio_led_off();
        m_state = off;
    }
}