/**
 * @defgroup GPIO
 * @{
 *
 * @paragraph 
 * 
 * Este módulo pertenece a la capa de abstracción de hardware y proporciona interfaces
 * para generar los eventos con los botones y manipular un led.
 *
 * @file gpio.c
 * 
 * @version 1.0
 * @author  Rodrigo De Soto, Maite Gil, José Bentancour.
 * @date 12 Julio 2018
 * 
 * @brief Módulo que proporciona funciones para manejar pines de entrada y salida.
 */

#include "gpio.h"

#include <stdint.h>

#include "nrf.h"
#include "nrf_nvic.h"

/* GPIO */
#define LED 		        0       /**< Pin donde se conecta el LED. */
#define BTN_OK 		        2       /**< Pin donde se conecta el botón OK. */
#define BTN_DOWN 	        3       /**< Pin donde se conecta el botón DOWN. */
#define BTN_UP 		        4       /**< Pin donde se conecta el botón UP  */

#define m_led_dir                   GPIO_PIN_CNF_DIR_Output
#define m_led_pull                  (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
#define m_led_drive                 (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_led_sense                 (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
#define m_led_gpio_config           (m_led_dir | m_led_pull | m_led_drive | m_led_sense)

#define m_boton_ok_dir              GPIO_PIN_CNF_DIR_Input
#define m_boton_ok_pull             (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
#define m_boton_ok_drive            (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_boton_ok_sense            (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
#define m_boton_ok_gpio_config      (m_boton_ok_dir | m_boton_ok_pull | m_boton_ok_drive | m_boton_ok_sense)

#define m_boton_down_dir            GPIO_PIN_CNF_DIR_Input
#define m_boton_down_pull           (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
#define m_boton_down_drive          (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_boton_down_sense          (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
#define m_boton_down_gpio_config    (m_boton_down_dir | m_boton_down_pull | m_boton_down_drive | m_boton_down_sense)

#define m_boton_up_dir              GPIO_PIN_CNF_DIR_Input
#define m_boton_up_pull             (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
#define m_boton_up_drive            (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_boton_up_sense            (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
#define m_boton_up_gpio_config      (m_boton_up_dir | m_boton_up_pull | m_boton_up_drive | m_boton_up_sense)

/* GPIO Task and Event */   
#define m_boton_ok_psel_pos         (BTN_OK << GPIOTE_CONFIG_PSEL_Pos)                                      /**< Boton OK : Posicion 1. */
#define m_boton_ok_polarity         (2UL << GPIOTE_CONFIG_POLARITY_Pos)                                     /**< Polaridad : HiToLo. */
#define m_boton_ok_mode             (0x1UL << GPIOTE_CONFIG_MODE_Pos)                                       /**< Modo : Event. */
#define m_boton_ok_config           (m_boton_ok_psel_pos | m_boton_ok_polarity | m_boton_ok_mode)
#define m_boton_ok_intenset         GPIOTE_INTENSET_IN0_Msk

#define m_boton_down_psel_pos       (BTN_DOWN << GPIOTE_CONFIG_PSEL_Pos)                                    /**< Boton DOWN : Posicion 2. */
#define m_boton_down_polarity       (2UL << GPIOTE_CONFIG_POLARITY_Pos)                                     /**< Polaridad : HiToLo. */
#define m_boton_down_mode           (0x1UL << GPIOTE_CONFIG_MODE_Pos)                                       /**< Modo : Event. */
#define m_boton_down_config         (m_boton_down_psel_pos | m_boton_down_polarity | m_boton_down_mode)
#define m_boton_down_intenset       GPIOTE_INTENSET_IN1_Msk

#define m_boton_up_psel_pos         (BTN_UP << GPIOTE_CONFIG_PSEL_Pos)                                      /**< Boton UP : Posicion 3. */
#define m_boton_up_polarity         (2UL << GPIOTE_CONFIG_POLARITY_Pos)                                     /**< Polaridad : HiToLo. */
#define m_boton_up_mode             (0x1UL << GPIOTE_CONFIG_MODE_Pos)                                       /**< Modo : Event. */
#define m_boton_up_config           (m_boton_up_psel_pos | m_boton_up_polarity | m_boton_up_mode)
#define m_boton_up_intenset         GPIOTE_INTENSET_IN2_Msk

#define gpio_intenset               (m_boton_ok_intenset | m_boton_down_intenset | m_boton_up_intenset)
#define gpio_intenclr               (GPIOTE_INTENCLR_IN3_Msk | GPIOTE_INTENCLR_PORT_Msk)
    
#define GPIOTE_PRIORITY             1

static volatile uint8_t* m_gpio_boton_ok_flag;
static volatile uint8_t* m_gpio_boton_down_flag;
static volatile uint8_t* m_gpio_boton_up_flag;

static uint8_t led_status;          /**< Sombra del estado del led para el toggle. */


/**@brief Rutina de atención a interrupciones del GPIOTE. 
 */
void GPIOTE_IRQHandler(void)
{
    if (NRF_GPIOTE->EVENTS_IN[0] != 0)
    {
        *m_gpio_boton_ok_flag = 1;
        NRF_GPIOTE->EVENTS_IN[0] = 0;
    }
    if (NRF_GPIOTE->EVENTS_IN[1] != 0)
    {
        *m_gpio_boton_down_flag = 1; 
        NRF_GPIOTE->EVENTS_IN[1] = 0;
    }
    if (NRF_GPIOTE->EVENTS_IN[2] != 0)
    {
        *m_gpio_boton_up_flag = 1;
        NRF_GPIOTE->EVENTS_IN[2] = 0;
    }
}


/**@brief Función de inicialización del módulo.
 */
void gpio_init()
{
    /* Desconecto el buffer de entrada para no tener pines con estado indefinido */
    int i;
    for(i=0;i<28;i++)
    {
        NRF_GPIO->PIN_CNF[i] = GPIO_PIN_CNF_INPUT_Disconnect <<  GPIO_PIN_CNF_INPUT_Pos;
    }
    
    /* LED */
    NRF_GPIO->PIN_CNF[LED] = m_led_gpio_config;
    
    /* BOTON OK */
    NRF_GPIO->PIN_CNF[BTN_OK] = m_boton_ok_gpio_config;
    NRF_GPIOTE->CONFIG[0] = m_boton_ok_config;
    
    /* BOTON DOWN */
    NRF_GPIO->PIN_CNF[BTN_DOWN] = m_boton_down_gpio_config;
    NRF_GPIOTE->CONFIG[1] = m_boton_down_config;
    
    /* BOTON UP */
    NRF_GPIO->PIN_CNF[BTN_UP] = m_boton_up_gpio_config;
    NRF_GPIOTE->CONFIG[2] = m_boton_up_config;
    
    /* INTERRUPCIONES */
    NRF_GPIOTE->INTENSET = gpio_intenset;
    NRF_GPIOTE->INTENCLR = gpio_intenclr;
    
    sd_nvic_SetPriority(GPIOTE_IRQn, GPIOTE_PRIORITY);       /* Seteo la prioridad de la interrupción. */
    sd_nvic_EnableIRQ(GPIOTE_IRQn);                          /* Habilito la interrupción. */
}

/**@brief Función para setear la flag donde indicar que el botón OK fue presionado.
 *
 * @param main_boton_ok_flag    Puntero a una flag donde se indicara que el botón OK fue presionado.
 */
void gpio_boton_ok_set_flag(volatile uint8_t* main_boton_ok_flag)
{
    m_gpio_boton_ok_flag = main_boton_ok_flag;
}

/**@brief Función para setear la flag donde indicar que el botón DOWN fue presionado.
 *
 * @param main_boton_down_flag    Puntero a una flag donde se indicara que el botón DOWN fue presionado.
 */
void gpio_boton_down_set_flag(volatile uint8_t* main_boton_down_flag)
{
    m_gpio_boton_down_flag = main_boton_down_flag;
}

/**@brief Función para setear la flag donde indicar que el botón UP fue presionado.
 *
 * @param main_boton_up_flag    Puntero a una flag donde se indicara que el botón UP fue presionado.
 */
void gpio_boton_up_set_flag(volatile uint8_t* main_boton_up_flag)
{
    m_gpio_boton_up_flag = main_boton_up_flag;
}

/**@brief Función para encender el LED.
 */
void gpio_led_on()
{
    led_status = 1;
    NRF_GPIO->OUTSET = 0x1UL << LED;
}

/**@brief Función para apagar el LED.
 */
void gpio_led_off()
{
    led_status = 0;
    NRF_GPIO->OUTCLR = 0x1UL << LED;
}

/**@brief Función para conmutar el estado del LED.
 */
void gpio_led_toggle()
{
    if(led_status)
    {
        gpio_led_off();
    }
    else
    {
        gpio_led_on();
    }
}