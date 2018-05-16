#include "i2c.h"

#include <stdint.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_nvic.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_gpio.h"

#define IRQ_PRIORITY    1
#define SCL_PIN         NRF_GPIO_PIN_MAP(0, 5)
#define SDA_PIN         NRF_GPIO_PIN_MAP(0, 6)

void SPI0_TWI0_IRQHandler(void)
{
    //NRF_LOG_DEBUG("SPI0_TWI0_IRQHandler\r\n");
    if (NRF_TWI0->EVENTS_TXDSENT != 0)
    {
        NRF_LOG_DEBUG("EVENTS_TXDSENT\r\n");
        NRF_TWI0->EVENTS_TXDSENT = 0; /* Event clear. */
    }
    if (NRF_TWI0->EVENTS_ERROR != 0)
    {
        uint32_t error_src = NRF_TWI0->ERRORSRC;
        if(error_src & 0x01)
        {
            NRF_LOG_ERROR("A start condition is received while the previous data still lies in RXD (Previous data is lost)\r\n", error_src);
        }
        if(error_src & 0x02)
        {
            NRF_LOG_ERROR("NACK NOT received after sending the addres\r\n", error_src);
        }
        if(error_src & 0x04)
        {
            NRF_LOG_ERROR("NACK NOT received after sending a data byte\r\n", error_src);
        }
        NRF_TWI0->ERRORSRC = 0x07; /* Errors clear. */
        NRF_TWI0->EVENTS_ERROR = 0; /* Errors clear. */
    }
    NRF_LOG_FLUSH();
}

static void twi_conf(void){
    NRF_TWI0->INTENSET       = TWI_INTENSET_TXDSENT_Msk;        /* Enable interrupt on TXDSENT event. */
    NRF_TWI0->INTENSET       = TWI_INTENSET_ERROR_Msk;          /* Enable interrupt on ERROR event. */    
    NRF_TWI0->FREQUENCY      = TWI_FREQUENCY_FREQUENCY_K100;    /* 100 kbps. */
    NRF_TWI0->PSELSCL        = SCL_PIN;                         /* Pin select for SCL. */ 
    NRF_TWI0->PSELSDA        = SDA_PIN;                         /* Pin select for SDA. */
    NRF_TWI0->EVENTS_TXDSENT = 0;                               /* Event clear. */
    NRF_TWI0->EVENTS_ERROR   = 0;                               /* Event clear. */
    NRF_TWI0->POWER          = 1;                               /* Peripheral power control. */
    NRF_TWI0->ENABLE         = TWI_ENABLE_ENABLE_Enabled;       /* Enable two-wire master. */
}

/**
* Initiate the moduley and join the I2C bus as a master.
*/
void i2c_init(){
    NRF_LOG_INFO("i2c_init\r\n");
    nrf_gpio_cfg(SCL_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT,  NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0D1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg(SDA_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT,  NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0D1, NRF_GPIO_PIN_NOSENSE);
    twi_conf();
    sd_nvic_SetPriority(SPI0_TWI0_IRQn, IRQ_PRIORITY);
    sd_nvic_EnableIRQ(SPI0_TWI0_IRQn);
}

/**
* Begin a transmission to the I2C slave device with the given address. 
* Subsequently, queue bytes for transmission with the i2c_write() 
* function and transmit them by calling i2c_end_transmission().
* address: the 7-bit address of the device to transmit to
*/
void i2c_begin_transmission(uint8_t address)
{
    NRF_TWI0->ADDRESS = address; /* Address used in the two-wire transfer. */
}

/**
* Queues bytes for transmission from a master to slave device.
* Must be called in-between calls to i2c_begin_transmission() and i2c_end_transmission().
* value: a value to send as a single byte
*/
void i2c_write(uint8_t value)
{
    NRF_TWI0->TXD = value; /* TX data register. */
    NRF_TWI0->TASKS_STARTTX = 1; /* Start 2-Wire master transmit sequence. */
}

/**
* Ends a transmission to a slave device that was begun by i2c_begin_transmission() 
* and transmits the bytes that were queued by i2c_write().
*/
void i2c_end_transmission(void)
{
    NRF_TWI0->TASKS_STOP = 1; /* top 2-Wire transaction. */
}