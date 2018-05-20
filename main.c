#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_delay.h"

#include "ble_uart.h"
#include "gpio.h"
#include "i2c.h"
#include "rtc.h"

ble_uart_status_t ble_uart_status;

uint8_t ble_uart_rx_flag;
uint8_t ble_uart_tx_flag;
uint8_t ble_uart_tx_flag_old;
uint8_t ble_uart_rx_msg[MAX_LEN];

volatile uint8_t gpio_ok_flag;
volatile uint8_t gpio_up_flag;
volatile uint8_t gpio_down_flag;

//volatile uint8_t i2c_tx_flag;

/**@brief Application main function.
 */
int main(void)
{
    /* Initialize. */
    ble_uart_rx_set_flag(&ble_uart_rx_flag);
    ble_uart_tx_flag = 1;
    ble_uart_tx_set_flag(&ble_uart_tx_flag);
    ble_uart_init();
    
    rtc_init();
    
    gpio_boton_ok_set_flag(&gpio_ok_flag);
    gpio_boton_up_set_flag(&gpio_up_flag);
    gpio_boton_down_set_flag(&gpio_down_flag);
    gpio_init();
    gpio_led_off();
    
    //i2c_tx_set_flag(&i2c_tx_flag);
    //i2c_tx_flag = 0;
    //i2c_init();

    NRF_LOG_INIT(NULL);
    NRF_LOG_INFO("main init\r\n");
    
    /* Enter main loop. */
    for (;;)
    {
        NRF_LOG_FLUSH();
        
        ble_uart_status = ble_uart_get_status();
        
        if (gpio_ok_flag)
        {
            NRF_LOG_INFO("gpio OK\r\n");
            nrf_delay_ms(50);
            gpio_ok_flag = 0;
            
            if (ble_uart_status.connected) {
                ble_uart_disconnect();
            } else {
                if (!ble_uart_status.advertising){
                    ble_uart_advertising_start();
                }
            }
        }
        
        if (gpio_up_flag)
        {
            NRF_LOG_INFO("gpio UP %d\r\n", rtc_get());
            nrf_delay_ms(50);
            gpio_up_flag = 0;
            
            if (ble_uart_status.connected) {
                while(!ble_uart_tx_flag){};
                ble_uart_data_send("BTN_UP\n", 7);
            }
        }
        
        if (gpio_down_flag)
        {
            NRF_LOG_INFO("gpio DOWN %d\r\n", rtc_get());
            nrf_delay_ms(50);
            gpio_down_flag = 0;
            
            if (ble_uart_status.connected) {
                while(!ble_uart_tx_flag){};
                ble_uart_data_send("BTN_DOWN\n", 9);
            }
        }
        
        if (ble_uart_status.connected) {
            gpio_led_on();
        } else {
            if (ble_uart_status.advertising) {
                gpio_led_off();
                nrf_delay_ms(50);
                gpio_led_on();
                nrf_delay_ms(50);
            } else {
                gpio_led_off();
            }
        }
        
        if (ble_uart_rx_flag) {
            uint16_t length = ble_uart_get_msg(ble_uart_rx_msg);
            ble_uart_rx_flag = 0;            
            NRF_LOG_DEBUG("main new msg, len = %d\r\n", length);
            NRF_LOG_HEXDUMP_DEBUG(ble_uart_rx_msg, length);
            ble_uart_data_send(ble_uart_rx_msg, length);
        }
        
        /*
        nrf_gpio_pin_set(LED);
        
        i2c_begin_transmission(0x48);
        
        i2c_tx_flag = 0;
        i2c_write(0x01);
        while(!i2c_tx_flag){};
        
        i2c_tx_flag = 0;
        i2c_write(0x55);
        while(!i2c_tx_flag){};
        
        i2c_end_transmission();
        
        nrf_delay_ms(500);
        nrf_gpio_pin_clear(LED);
        nrf_delay_ms(500);
        */
    }
}
