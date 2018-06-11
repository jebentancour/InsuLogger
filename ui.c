/**
 * @defgroup UI
 * @{
 * 
 * @file ui.c
 * 
 * @version 1.0
 * @author  Rodrigo De Soto, Maite Gil, José Bentancour.
 * @date 12 Julio 2018
 * 
 * @brief Módulo encargado de la interacción con el usuario.
 *
 * @paragraph 
 * 
 * Muestra los diferentes menús en el display y toma como la entrada los botones presionados por el usuario.
 */

#include "ui.h"

#include <stdint.h>

#include "rtc.h"
#include "gpio.h"
#include "ble_uart.h"
#include "i2c.h"
#include "display.h"
#include "logger.h"

/** Estructura interna para representar el estado */
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
    show_data,
    input_ok,
    input_cancel,
    bye    
} internal_state_t;

#define HELLO_TICKS     4       /** 1 s          */
#define SHOW_TIME_TICKS 20      /** 5 s          */
#define SHOW_DATA_TICKS 12      /** 3 s          */
#define BYE_TICKS       4       /** 1 s          */

static uint8_t                m_sinc_i;         /** Variable para animación en pantalla */

static internal_state_t       m_state;          /** Estructura que representa el estado de ui */
static uint8_t                m_timer;          /** Contador para estados con timeout */

static uint8_t                m_glucemia_un;
static uint8_t                m_glucemia_dec;
static uint8_t                m_glucemia_cent;
static uint8_t                m_type;           /** 0 -> A, 1 -> B */
static uint8_t                m_dosis;

static ble_uart_status_t ui_ble_uart_status;


/**@brief Función de inicialización del modulo.
 *
 * @warning Se debe inicializar el modulo gpio antes de llamar a esta función.
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


/**@brief Función interna para manejar el contenido del display.
 */
static void ui_process_display(void)
{    
    display_send_command(SSD1306_Display_On_Cmd);
    
    /** Variables auxiliares */
    uint8_t cent;
    uint8_t dec;
    uint8_t un;
    uint32_t ms;
    uint32_t s;
    uint32_t m;
    uint32_t h;
    
    switch(m_state)
    {
        case off:
            // Pantalla vacía
            display_send_command(SSD1306_Display_Off_Cmd);
            break;
        case hello:
            //
            // Insu
            //    Logger
            //
            display_set_text_xy(5,4);
            display_print("Insu ",5);
            display_set_text_xy(6,6);
            display_print("Logger",6);
            break;
        case show_time:
            // A:
            // HH:MM:SS
            // B:
            // HH:MM:SS
            display_set_text_xy(4,4);
            display_print("A:",2);
            ms = logger_get_last_a();
            s = (ms / 1000) % 60;
            m = (ms / 60000) % 60;
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
            // Menu:
            // >Regist.
            //  Sinc.
            //
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
            // Menu:
            //  Regist.
            // >Sinc.
            //
            display_set_text_xy(5,4);
            display_print(" Regist.",8);
            display_set_text_xy(6,4);
            display_print(">Sinc.",6); 
            break;
        case sync:
            // Sinc...
            //
            //
            //
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
            // Glucem.:
            //
            // 000mg/dL
            //   ^
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
            // Glucem.:
            //
            // 000mg/dL
            //  ^
            display_set_text_xy(6,4);
            display_put_number(m_glucemia_cent);
            display_put_number(m_glucemia_dec);
            display_put_number(m_glucemia_un);
            display_set_text_xy(7,4);
            display_print(" ^ ",3); 
            break;
        case input_glucemia_cent:
            // Glucem.:
            //
            // 000mg/dL
            // ^
            display_set_text_xy(6,4);
            display_put_number(m_glucemia_cent);
            display_put_number(m_glucemia_dec);
            display_put_number(m_glucemia_un);
            display_set_text_xy(7,4);
            display_print("^  ",3);     
            break;
        case input_type_a:
            // Tipo:
            // >A
            //  B
            //
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
            // Tipo:
            //  A
            // >B
            //
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
            // Dosis:
            // 
            //     000U
            //
            display_set_text_xy(4,4);
            display_print("Dosis:",6);
            display_set_text_xy(5,4);
            display_print("  ",2); 
            display_set_text_xy(6,4);
            display_print("    ",4);
            cent = m_dosis / 100;
            if(cent > 0)
            {
                display_put_number(cent);
            }
            else
            {
                display_put_char(' ');
            }
            dec = (m_dosis % 100) / 10;
            if((dec > 0)||(cent >0))
            {
                display_put_number(dec);
            }
            else
            {
                display_put_char(' ');
            }
            un = m_dosis % 10;
            display_put_number(un);
            display_put_char('U');
            break;
        case show_data:
            // Datos:
            // XXXmg/dL
            // Tipo X
            // XXX U
            display_set_text_xy(4,4);
            display_print("Datos:",6);
            display_set_text_xy(5,4);
            display_put_number(m_glucemia_cent);
            display_put_number(m_glucemia_dec);
            display_put_number(m_glucemia_un);
            display_print("mg/dL",5);
            display_set_text_xy(6,4);
            display_print("Tipo ",5);
            if(m_type)
            {
                display_print("B   ",4);
            }
            else
            {
                display_print("A   ",4);
            }
            display_set_text_xy(7,4);
            cent = m_dosis / 100;
            if(cent > 0)
            {
                display_put_number(cent);
            }
            dec = (m_dosis % 100) / 10;
            if((dec > 0)||(cent >0))
            {
                display_put_number(dec);
            }
            else
            un = m_dosis % 10;
            display_put_number(un);
            display_put_char(' ');
            display_put_char('U');
            break;
        case input_ok:
            // Guardar?
            // >Si
            //  No
            //
            display_set_text_xy(4,4);
            display_print("Guardar?",8);
            display_set_text_xy(5,4);
            display_print(">Si     ",8);
            display_set_text_xy(6,4);
            display_print(" No     ",8);
            display_set_text_xy(7,4);
            display_print("        ",8);
            break;
        case input_cancel:
            // Guardar?
            //  Si
            // >No
            //
            display_set_text_xy(4,4);
            display_print("Guardar?",8);
            display_set_text_xy(5,4);
            display_print(" Si",3);
            display_set_text_xy(6,4);
            display_print(">No",3);
            break;
        case bye:
            //
            // Hasta
            //   luego!
            //
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
            // Nunca debería llegar a este lugar
            display_clear();
            break;
    }    
}


/**@brief Función procesar los eventos.
 *
 * @param event    Evento del tipo event_t.
 */
void ui_process_event(event_t event)
{
    switch(m_state)
    {
        case off:
            // La pantalla esta apagada.
            display_send_command(SSD1306_Display_Off_Cmd);
            if(event == pressed_ok){
                m_state = hello;
                ui_process_display();
                m_timer = HELLO_TICKS;
            }
            break;
        case hello:
            // La pantalla muestra InsuLogger.
            if(event == time_update){
                m_timer -= 1;
                if(m_timer == 0)
                {
                    m_state = show_time;
                    ui_process_display();
                    m_timer = SHOW_TIME_TICKS;
                }
            }
            break;
        case show_time:
            // La pantalla muestra el tiempo transcurrido desde la ultima inyección.
            if(event == time_update){
                ui_process_display();
                m_timer -= 1;
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
            // La pantalla muestra el menú con opción Registrar seleccionada.
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
            // La pantalla muestra el menú con opción Sincronizar seleccionada.
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
            // La pantalla muestra Sinc...
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
            // La pantalla muestra Glucemia con unidades seleccionada.
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
            // La pantalla muestra Glucemia con decenas seleccionada.
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
            // La pantalla muestra Glucemia con centenas seleccionada.
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
            // La pantalla muestra Tipo con opción A seleccionada.
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
            // La pantalla muestra Tipo con opción A seleccionada.
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
            // La pantalla muestra la cantidad de Dosis.
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
                m_state = show_data;
                ui_process_display();
                m_timer = SHOW_DATA_TICKS;
            }
            break;
        case show_data:
            // La pantalla muestra los datos ingresados.
            if(event == time_update){
                m_timer -= 1;
                if(m_timer == 0)
                {
                    m_state = input_ok;
                    ui_process_display();
                }
            }
            break;
        case input_ok:
            // Guardar? con la opción Si seleccionada.
            if(event == pressed_down){
                m_state = input_cancel;
                ui_process_display();
            }
            if(event == pressed_ok){
                logger_new_register(m_glucemia_un+10*m_glucemia_dec+100*m_glucemia_cent, m_type, m_dosis, rtc_get());
                m_state = bye;
                ui_process_display();
                m_timer = BYE_TICKS;
            }
            break;
        case input_cancel:
            // Guardar? con la opción No seleccionada.
            if(event == pressed_up){
                m_state = input_ok;
                ui_process_display();
            }
            if(event == pressed_ok){
                m_state = bye;
                ui_process_display();
                m_timer = BYE_TICKS;
            }
            break;
        case bye:
            // La pantalla muestra un mensaje de despedida.
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
            // Nunca debería llegar a este lugar
            gpio_led_off();
            m_state = off;
            ui_process_display();
            break;
    }
}


/**@brief Función para apagar ui.
 */
void ui_off(void)
{
    if(m_state != off){
        display_clear();
        display_send_command(SSD1306_Display_Off_Cmd);
        gpio_led_off();
        m_state = off;
    }
}
