#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_delay.h"

#include "ble_uart.h"
#include "gpio.h"
#include "rtc.h"
#include "ui.h"

#define IDLE_TICKS 120                  /* Ticks de timeout para ui (30 s)*/

ble_uart_status_t ble_uart_status;
uint8_t ble_uart_rx_msg[MAX_LEN];

volatile uint8_t ble_uart_rx_flag;
volatile uint8_t ble_uart_tx_flag;
volatile uint8_t rtc_tick_flag;
volatile uint8_t gpio_ok_flag;
volatile uint8_t gpio_up_flag;
volatile uint8_t gpio_down_flag;

uint8_t idle_timer;

/**@brief Application main function.
 */
int main(void)
{
    /* Initialize. */
    ble_uart_rx_set_flag(&ble_uart_rx_flag);
    ble_uart_tx_flag = 1;
    ble_uart_tx_set_flag(&ble_uart_tx_flag);
    ble_uart_init();
    
    rtc_tick_flag = 0;
    rtc_tick_set_flag(&rtc_tick_flag);
    rtc_init();
    
    gpio_boton_ok_set_flag(&gpio_ok_flag);
    gpio_boton_up_set_flag(&gpio_up_flag);
    gpio_boton_down_set_flag(&gpio_down_flag);
    gpio_init();
    gpio_led_off();
    
    ui_init();

    idle_timer = 0;
    
    NRF_LOG_INIT(NULL);
    NRF_LOG_INFO("main init\r\n");
    
    /* Enter main loop. */
    for (;;)
    {
        NRF_LOG_FLUSH();
        
        if (gpio_ok_flag)
        {
            idle_timer = 0;
            gpio_ok_flag = 0;
            nrf_delay_ms(5);
            if(!gpio_ok_flag)
            {
                ui_process_event(pressed_ok);
            }
        }
        
        if (gpio_up_flag)
        {
            idle_timer = 0;
            gpio_up_flag = 0;
            nrf_delay_ms(5);
            if(!gpio_up_flag)
            {
                ui_process_event(pressed_up);
            }
        }
        
        if (gpio_down_flag)
        {
            idle_timer = 0;
            gpio_down_flag = 0;
            nrf_delay_ms(5);
            if(!gpio_down_flag)
            {
                ui_process_event(pressed_down);
            }
        }
        
        
        if(rtc_tick_flag)
        {
            rtc_tick_flag = 0;
            
            ble_uart_status = ble_uart_get_status();
            if (ble_uart_status.advertising || ble_uart_status.connected){
                idle_timer = 0;
            }
            
            ui_process_event(time_update);
            
            idle_timer += 1;
            if(idle_timer >= IDLE_TICKS)
            {
                ui_off();
                idle_timer = IDLE_TICKS;
            }                
        }
        
        if (ble_uart_rx_flag) {
            uint16_t length = ble_uart_get_msg(ble_uart_rx_msg);
            ble_uart_rx_flag = 0;            
            NRF_LOG_DEBUG("main new msg, len = %d\r\n", length);
            NRF_LOG_HEXDUMP_DEBUG(ble_uart_rx_msg, length);
            ble_uart_data_send(ble_uart_rx_msg, length);
        }
    }
}
