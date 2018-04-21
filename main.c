#include <stdbool.h>
#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "nrf_gpio.h"
#include "nrf_delay.h"

/* Placa circular */
#define LED NRF_GPIO_PIN_MAP(0, 29)

static uint32_t i = 0;
uint32_t timestamp(){
  return 0;
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    uint32_t err_code;
    // Initialize.
    err_code = NRF_LOG_INIT(timestamp);
    APP_ERROR_CHECK(err_code);

    // Start execution.
    NRF_LOG_INFO("Started!\r\n");
    NRF_LOG_FLUSH();
    
    nrf_gpio_pin_dir_set(LED,NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_clear(LED);

    // Enter main loop.
    while(true)
    {     
        nrf_gpio_pin_toggle(LED);
        nrf_delay_ms(100);
        i++;
    }
}
/**
 * @}
 */
