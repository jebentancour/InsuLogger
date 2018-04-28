#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "ble_uart.h"

/* Placa circular */
#define LED NRF_GPIO_PIN_MAP(0, 29)
#define BTN NRF_GPIO_PIN_MAP(0, 28)

ble_uart_status_t ble_uart_status;
uint32_t btn_new;
uint32_t btn_old;
uint8_t ble_uart_rx_flag;
uint8_t ble_uart_tx_flag;
uint8_t ble_uart_tx_flag_old;
uint8_t ble_uart_rx_msg[20];

/**@brief Application main function.
 */
int main(void)
{    
    nrf_gpio_cfg_input(BTN, NRF_GPIO_PIN_PULLUP);
    
    nrf_gpio_pin_dir_set(LED, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_clear(LED);
    
    NRF_LOG_INIT(NULL);
    NRF_LOG_INFO("main\r\n");

    /* Initialize. */
    ble_uart_rx_set_flag(&ble_uart_rx_flag);
    ble_uart_tx_set_flag(&ble_uart_tx_flag);
    ble_uart_init();
    
    /* Enter main loop. */
    for (;;)
    {
        ble_uart_status = ble_uart_get_status();
        btn_new = nrf_gpio_pin_read(BTN);
        if (btn_new != btn_old) {
            if (!btn_new) {
                if (ble_uart_status.connected) {
                    ble_uart_disconnect();
                } else {
                    if (!ble_uart_status.advertising){
                        ble_uart_advertising_start();
                    }
                }
            }
        }
        btn_old = btn_new;
        
        if (ble_uart_status.connected) {
            nrf_gpio_pin_set(LED);
        } else {
            if (ble_uart_status.advertising) {
                nrf_gpio_pin_toggle(LED);
                nrf_delay_ms(100);
            } else {
                nrf_gpio_pin_clear(LED);
            }
        }
        
        if (ble_uart_rx_flag) {
            uint16_t length = ble_uart_get_msg(ble_uart_rx_msg);
            ble_uart_rx_flag = 0;            
            NRF_LOG_DEBUG("main_new_msg! len = %d\r\n", length);
            NRF_LOG_HEXDUMP_DEBUG(ble_uart_rx_msg, length);
            ble_uart_data_send(ble_uart_rx_msg, length);
        }
        
        if (ble_uart_tx_flag != ble_uart_tx_flag_old)
        {
            if (ble_uart_tx_flag)
            {
                NRF_LOG_DEBUG("main tx complete!\r\n");
            }
        }
        ble_uart_tx_flag_old = ble_uart_tx_flag;
        
        
        NRF_LOG_FLUSH();
    }
}
