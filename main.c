#include <stdint.h>

#include "softdevice_handler.h"
#include "nrf_delay.h"

#include "ble_uart.h"
#include "gpio.h"
#include "rtc.h"
#include "ui.h"
#include "shell.h"
#include "logger.h"

#define IDLE_TICKS      40

/* Variable para el timeout. */
uint8_t idle_timer;

/* Variables para ble_uart. */
ble_uart_status_t ble_uart_status;
uint8_t ble_uart_rx_msg[MAX_LEN];
uint8_t ble_uart_tx_msg[MAX_LEN];

/* Flags */
volatile uint8_t ble_uart_rx_flag;
volatile uint8_t ble_uart_tx_flag;
volatile uint8_t rtc_tick_flag;
volatile uint8_t gpio_ok_flag;
volatile uint8_t gpio_up_flag;
volatile uint8_t gpio_down_flag;
volatile uint8_t logger_send_flag;

/* Variables para shell. */
ShellFunPtr quefuncion;
unsigned int argc;
char* argv[64];
int retval;


/**@brief Main.
 */
int main(void)
{  
    /* Inicializacion de modulos y variables. */
    ble_uart_rx_set_flag(&ble_uart_rx_flag);
    ble_uart_tx_flag = 1;
    ble_uart_tx_set_flag(&ble_uart_tx_flag);
    ble_uart_init();
    
    rtc_tick_flag = 0;
    rtc_tick_set_flag(&rtc_tick_flag);
    rtc_init();
    
    logger_set_flag(&logger_send_flag);
    logger_init();
    
    gpio_boton_ok_set_flag(&gpio_ok_flag);
    gpio_boton_up_set_flag(&gpio_up_flag);
    gpio_boton_down_set_flag(&gpio_down_flag);
    gpio_init();
    
    ui_init();

    idle_timer = 0;
    
    /* Low power */
    sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
    
    /* Loop principal. */
    for (;;)
    {      
        /* Se resiono el boton OK. */
        if (gpio_ok_flag)
        {
            idle_timer = 0;
            gpio_ok_flag = 0;
            nrf_delay_ms(5);
            if(!gpio_ok_flag)
            {
                ui_process_event(pressed_ok); /* Se envia el evento a ui. */
            }
        }
        
        /* Se resiono el boton UP. */
        if (gpio_up_flag)
        {
            idle_timer = 0;
            gpio_up_flag = 0;
            nrf_delay_ms(5);
            if(!gpio_up_flag)
            {
                ui_process_event(pressed_up); /* Se envia el evento a ui. */
            }
        }
        
        /* Se resiono el boton DOWN. */
        if (gpio_down_flag)
        {
            idle_timer = 0;
            gpio_down_flag = 0;
            nrf_delay_ms(5);
            if(!gpio_down_flag)
            {
                ui_process_event(pressed_down); /* Se envia el evento a ui. */
            }
        }
         
        /* Se incremento el rtc, este evento es periodico. */
        if(rtc_tick_flag)
        {
            rtc_tick_flag = 0;
            
            ble_uart_status = ble_uart_get_status();
            if (ble_uart_status.advertising || ble_uart_status.connected){
                idle_timer = 0;
            }
            
            ui_process_event(time_update); /* Se envia el evento a ui. */
            
            idle_timer += 1;
            if(idle_timer >= IDLE_TICKS)
            {
                ui_off();
                idle_timer = IDLE_TICKS;
            }                
        }
        
        /* Se recibio un nuevo mensaje por ble_uart. */
        if (ble_uart_rx_flag) 
        {
            // Se recibe un valor nuevo por ble_uart.
            uint16_t length = ble_uart_get_msg(ble_uart_rx_msg);
            ble_uart_rx_flag = 0;
            
            // Se agrega delimitador NULL al final del buffer para convertirlo en string.
            ble_uart_rx_msg[length - 1]='\0';
            
            // Se revisa si el comando concuerda con alguno de los definidos en shell.
            switch (sisem_shell((char*)ble_uart_rx_msg,&quefuncion, &argc, argv))
            {
                case OK:						
                    // Ejecutar la funcion en el caso que se encontro.
                    // Se ejecuta la funcion correspondiente pasando como parametros (unsigned int argc, char** argv).
                    retval=(quefuncion)(argc, argv);
                    break;
                    
                case EXIT:						
                    // Terminar comunicacion bluetooth.
                    ble_uart_disconnect();
                    ble_uart_advertising_stop();
                    break;

                case NOTFOUND:
                    // Avisar que la funcion no existe.
                    break;

                default:
                    // Nunca deberiamos llegar aca.
                    break;
            }
        }
        
        /* Indica que logger tiene un valor pendiente para enviar. */
        if (logger_send_flag)
        {           
            if (ble_uart_tx_flag) // Revisar si ble_uart esta listo para enviar un nuevo mensaje.
            {                
                // Se llama a logger_send para que ponga en ble_uart_tx_msg el mensaje que se quiere enviar y en tx_length el largo del mensaje.
                uint8_t tx_length;
                tx_length = logger_send(ble_uart_tx_msg);
                
                // Se le pasa a ble_uart_data_send el puntero con el mensaje.
                ble_uart_data_send(ble_uart_tx_msg, tx_length);
                // Una vez logre enviar todo entrará denuevo a este condicional hasta que logger_send considere que no hay más datos para enviar.
            }
        }
        
        if(!gpio_ok_flag && !gpio_up_flag && !gpio_down_flag && !rtc_tick_flag && !ble_uart_rx_flag && !logger_send_flag && (idle_timer == IDLE_TICKS))
        {
            sd_app_evt_wait();          // Se va a System ON Low Power Mode, cuando se despierta sigue la ejecucion normal.
            //sd_power_system_off();      // Se va a System OFF, no vuelve, cuando se despierta es como si hubieramos resetado.
        }
    }
}
