#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
//#include "nrf_gpio.h"
#include "nrf_delay.h"
//#include "ble_uart.h"
#include "gpio.h"
#include "i2c.h"

/* Placa circular */
//#define LED NRF_GPIO_PIN_MAP(0, 29)
//#define BTN_OK NRF_GPIO_PIN_MAP(0, 28)

/* Placa InsuLogger */
//#define LED NRF_GPIO_PIN_MAP(0, 0)
//#define BTN_OK NRF_GPIO_PIN_MAP(0, 2)
//#define BTN_DOWN NRF_GPIO_PIN_MAP(0, 3)
//#define BTN_UP NRF_GPIO_PIN_MAP(0, 4)

//ble_uart_status_t ble_uart_status;
//uint32_t btn_new;
//uint32_t btn_old;
//uint32_t btn_down_new;
//uint32_t btn_down_old;
//uint32_t btn_up_new;
//uint32_t btn_up_old;
//uint8_t ble_uart_rx_flag;
//uint8_t ble_uart_tx_flag;
//uint8_t ble_uart_tx_flag_old;
//uint8_t ble_uart_rx_msg[MAX_LEN];

volatile uint8_t gpio_ok_flag;
volatile uint8_t gpio_up_flag;
volatile uint8_t gpio_down_flag;

volatile uint8_t i2c_tx_flag;

/**@brief Application main function.
 */
int main(void)
{    
    //nrf_gpio_cfg_input(BTN_OK, NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(BTN_DOWN, NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(BTN_UP, NRF_GPIO_PIN_PULLUP);
    
    //nrf_gpio_pin_dir_set(LED, NRF_GPIO_PIN_DIR_OUTPUT);
    //nrf_gpio_pin_clear(LED);
    
    NRF_LOG_INIT(NULL);
    NRF_LOG_INFO("main init\r\n");

    /* Initialize. */
    //ble_uart_rx_set_flag(&ble_uart_rx_flag);
    //ble_uart_tx_flag = 1;
    //ble_uart_tx_set_flag(&ble_uart_tx_flag);
    //ble_uart_init();
    
    gpio_boton_ok_set_flag(&gpio_ok_flag);
    gpio_boton_up_set_flag(&gpio_up_flag);
    gpio_boton_down_set_flag(&gpio_down_flag);
    gpio_init();
    gpio_led_off();
    
    //i2c_tx_set_flag(&i2c_tx_flag);
    //i2c_tx_flag = 0;
    //i2c_init();
    
    /* Enter main loop. */
    for (;;)
    {
        NRF_LOG_FLUSH();
        
        if (gpio_ok_flag)
        {
            NRF_LOG_INFO("gpio OK\r\n");
            gpio_ok_flag = 0;
        }
        
        if (gpio_up_flag)
        {
            NRF_LOG_INFO("gpio UP\r\n");
            gpio_up_flag = 0;
        }
        
        if (gpio_down_flag)
        {
            NRF_LOG_INFO("gpio DOWN\r\n");
            gpio_down_flag = 0;
        }
        
        gpio_led_on();
        nrf_delay_ms(100);
        gpio_led_off();
        nrf_delay_ms(100);
        
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
        
        /*
        ble_uart_status = ble_uart_get_status();
        
        btn_new = nrf_gpio_pin_read(BTN_OK);
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
        
        btn_down_new = nrf_gpio_pin_read(BTN_DOWN);
        if (btn_down_new != btn_down_old) {
            if (!btn_down_new) {
                if (ble_uart_status.connected) {
                    while(!ble_uart_tx_flag){};
                    ble_uart_data_send("BTN_DOWN\n", 9);
                }
            }
        }
        btn_down_old = btn_down_new;
        
        btn_up_new = nrf_gpio_pin_read(BTN_UP);
        if (btn_up_new != btn_up_old) {
            if (!btn_up_new) {
                if (ble_uart_status.connected) {
                    while(!ble_uart_tx_flag){};
                    ble_uart_data_send("BTN_UP\n", 7);
                }
            }
        }
        btn_up_old = btn_up_new;
        
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
            NRF_LOG_DEBUG("main new msg, len = %d\r\n", length);
            NRF_LOG_HEXDUMP_DEBUG(ble_uart_rx_msg, length);
            ble_uart_data_send(ble_uart_rx_msg, length);
        }
        
        if (ble_uart_tx_flag != ble_uart_tx_flag_old)
        {
            if (ble_uart_tx_flag)
            {
                NRF_LOG_DEBUG("main tx complete\r\n");
            }
        }
        ble_uart_tx_flag_old = ble_uart_tx_flag;
        */
    }
}
