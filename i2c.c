#include "i2c.h"

#include <stdint.h>

#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_nvic.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_gpio.h"

#define I2C_PRIORITY    1
#define SCL_PIN         5
#define SDA_PIN         6

static volatile uint8_t*         m_tx_flag;              /* Bandera que indica que un byte fue enviado. */

/**@brief Rutina de atencion a la interrupcion del periferico TWI.
 */ 
void SPI0_TWI0_IRQHandler(void)
{
    *m_tx_flag = 1;                     /* Aviso incluso si hay error, el error se muestra en el LOG */
    if (NRF_TWI0->EVENTS_TXDSENT != 0)
    {
        NRF_TWI0->EVENTS_TXDSENT = 0;   /* Event clear. */
    }
    if (NRF_TWI0->EVENTS_ERROR != 0)
    {
        uint32_t error_src = NRF_TWI0->ERRORSRC;
        if(error_src & 0x01)
        {
            NRF_LOG_ERROR("A start condition is received while the previous data still lies in RXD (Previous data is lost)\r\n");
        }
        if(error_src & 0x02)
        {
            NRF_LOG_ERROR("NACK NOT received after sending the addres\r\n");
        }
        if(error_src & 0x04)
        {
            NRF_LOG_ERROR("NACK NOT received after sending a data byte\r\n");
        }
        NRF_TWI0->ERRORSRC = 0x07;      /* Errors clear. */
        NRF_TWI0->EVENTS_ERROR = 0;     /* Event clear. */
    }
}

/**@brief Funcion para configurar el periferico TWI como un master I2C a 100kbps.
 */
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

/**@brief Funcion de inicializacion del modulo.
 */
void i2c_init(){
    NRF_LOG_INFO("i2c_init\r\n");
    nrf_gpio_cfg(SCL_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT,  NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0D1, NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg(SDA_PIN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT,  NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_S0D1, NRF_GPIO_PIN_NOSENSE);
    twi_conf();                                         /* Configuro el periferico TWI. */
    sd_nvic_SetPriority(SPI0_TWI0_IRQn, I2C_PRIORITY);  /* Seteo la prioridad de la interrupcion. */
    sd_nvic_EnableIRQ(SPI0_TWI0_IRQn);                  /* Habilito la interrupcion. */
}

/**@brief Funcion para setear la flag donde indicar el fin de trasnmision de un byte.
 *
 * @param main_tx_flag    Puntero a una flag donde se indicara el fin de trasnmision de un byte.
 */
void i2c_tx_set_flag(volatile uint8_t* main_tx_flag)
{
    m_tx_flag = main_tx_flag;
}

/**@brief Funcion para iniciar una transferencia de datos a un esclavo.
 *
 * @param address    La direccion de 7 bits del esclavo.
 */
void i2c_begin_transmission(uint8_t address)
{
    NRF_TWI0->ADDRESS = address;        /* Address used in the two-wire transfer. */
}

/**@brief Funcion para trasnmitir un byte a un esclavo.
 *
 * @param value    El dato a trasnmitir.
 */
void i2c_write(uint8_t value)
{
    NRF_TWI0->TXD = value;              /* TX data register. */
    NRF_TWI0->TASKS_STARTTX = 1;        /* Start 2-Wire master transmit sequence. */
}

/**@brief Funcion para terminar la trasnsaccion.
 */
void i2c_end_transmission(void)
{
    NRF_TWI0->TASKS_STOP = 1;           /* top 2-Wire transaction. */
}