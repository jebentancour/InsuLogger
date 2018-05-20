/*
 * Modulo GPIO 
 *
 * Modulo de abstraccion de Hardware de las entradas y salidas.
 *
 * Entradas:
 *  - Boton Select
 *  - Boton UP
 *  - Boton DOWN
 *
 * Salidas:
 *  - LED
 *
 * LED 0
 * BTN_OK 2
 * BTN_DOWN 3
 * BTN_UP 4
 *
 
 * 
 * -------------------------  Interrupt Number Definition  ------------------------ *
 * typedef enum {
 * 		----------------------  nrf51 Specific Interrupt Numbers  ---------------------- *
 * 		GPIOTE_IRQn                   =   6,              !<   6  GPIOTE
 * } IRQn_Type;
 *
 * ================================================================================
 * ================                     GPIOTE                     ================
 * ================================================================================


 *
 * @brief GPIO tasks and events. (GPIOTE)
 *

 * typedef struct {                                    !< GPIOTE Structure                                                    
 *  __O  uint32_t  TASKS_OUT[4];                       !< Tasks asssociated with GPIOTE channels.                               
 *  __I  uint32_t  RESERVED0[60];
 *  __IO uint32_t  EVENTS_IN[4];                      !< Tasks asssociated with GPIOTE channels.                               
 *  __I  uint32_t  RESERVED1[27];
 *  __IO uint32_t  EVENTS_PORT;                       !< Event generated from multiple pins.                                   
 *  __I  uint32_t  RESERVED2[97];
 *  __IO uint32_t  INTENSET;                          !< Interrupt enable set register.                                        
 *  __IO uint32_t  INTENCLR;                          !< Interrupt enable clear register.                                      
 *  __I  uint32_t  RESERVED3[129];
 *  __IO uint32_t  CONFIG[4];                         !< Channel configuration registers.                                      
 *  __I  uint32_t  RESERVED4[695];
 *  __IO uint32_t  POWER;                             !< Peripheral power control.                                             
 * } NRF_GPIOTE_Type;

typedef struct {                                    !< GPIO Structure                                                        
  __I  uint32_t  RESERVED0[321];
  __IO uint32_t  OUT;                               !< Write GPIO port.                                                      
  __IO uint32_t  OUTSET;                            !< Set individual bits in GPIO port.                                     
  __IO uint32_t  OUTCLR;                            !< Clear individual bits in GPIO port.                                   
  __I  uint32_t  IN;                                !< Read GPIO port.                                                       
  __IO uint32_t  DIR;                               !< Direction of GPIO pins.                                               
  __IO uint32_t  DIRSET;                            !< DIR set register.                                                     
  __IO uint32_t  DIRCLR;                            !< DIR clear register.                                                   
  __I  uint32_t  RESERVED1[120];
  __IO uint32_t  PIN_CNF[32];                       !< Configuration of GPIO pins.                                           
} NRF_GPIO_Type;
 
*/
 
 /**
 
 
 
 */
#include "nrf.h"
#include "nrf_peripherals.h"
#include "nrf_assert.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

 /**
 
 */
/* GPIO */
#define m_led_dir               GPIO_PIN_CNF_DIR_Output
#define m_led_pull              (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos)
#define m_led_drive             (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_led_sense             (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
#define m_led_gpio_config       (m_led_dir | m_led_pull | m_led_drive | m_led_sense)

#define m_boton_ok_dir          GPIO_PIN_CNF_DIR_Input
#define m_boton_ok_pull         (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
#define m_boton_ok_drive        (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_boton_ok_sense        (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
#define m_boton_ok_gpio_config  (m_boton_ok_dir | m_boton_ok_pull | m_boton_ok_drive | m_boton_ok_sense)

#define m_boton_down_dir        GPIO_PIN_CNF_DIR_Input
#define m_boton_down_pull       (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
#define m_boton_down_drive      (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_boton_down_sense      (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
#define m_boton_down_gpio_config (m_boton_down_dir | m_boton_down_pull | m_boton_down_drive | m_boton_down_sense)

#define m_boton_up_dir          GPIO_PIN_CNF_DIR_Input
#define m_boton_up_pull         (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos)
#define m_boton_up_drive        (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
#define m_boton_up_sense        (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos)
#define m_boton_up_gpio_config  (m_boton_up_dir | m_boton_up_pull | m_boton_up_drive | m_boton_up_sense)

/* GPIO Task and Event */
#define m_boton_ok_psel_pos     (2UL << GPIOTE_CONFIG_PSEL_Pos)                 /* Boton OK : Posicion 1*/
#define m_boton_ok_polarity     (2UL << GPIOTE_CONFIG_POLARITY_Pos)               /* Polaridad : HiToLo */
#define m_boton_ok_mode         (0x1UL << GPIOTE_CONFIG_MODE_Pos)                 /* Modo : Event */
#define m_boton_ok_config       (m_boton_ok_psel_pos | m_boton_ok_polarity | m_boton_ok_mode)
#define m_boton_ok_intenset     GPIOTE_INTENSET_IN0_Msk                         /*  */

#define m_boton_down_psel_pos   (3UL << GPIOTE_CONFIG_PSEL_Pos)                   /* Boton DOWN : Posicion 2*/
#define m_boton_down_polarity   (2UL << GPIOTE_CONFIG_POLARITY_Pos)               /* Polaridad : HiToLo */
#define m_boton_down_mode       (0x1UL << GPIOTE_CONFIG_MODE_Pos)                 /* Modo : Event */
#define m_boton_down_config     (m_boton_down_psel_pos | m_boton_down_polarity | m_boton_down_mode)
#define m_boton_down_intenset   GPIOTE_INTENSET_IN1_Msk                         /*  */

#define m_boton_up_psel_pos     (4UL << GPIOTE_CONFIG_PSEL_Pos)                   /* Boton UP : Posicion 3*/
#define m_boton_up_polarity     (2UL << GPIOTE_CONFIG_POLARITY_Pos)               /* Polaridad : HiToLo */
#define m_boton_up_mode         (0x1UL << GPIOTE_CONFIG_MODE_Pos)                 /* Modo : Event */
#define m_boton_up_config       (m_boton_up_psel_pos | m_boton_up_polarity | m_boton_up_mode)
#define m_boton_up_intenset     GPIOTE_INTENSET_IN2_Msk                         /*  */

#define gpio_intenset           (m_boton_ok_intenset | m_boton_down_intenset | m_boton_up_intenset)

#define gpio_intenclr           (GPIOTE_INTENCLR_IN3_Msk | GPIOTE_INTENCLR_PORT_Msk)

static volatile uint8_t* m_gpio_boton_ok_flag;
static volatile uint8_t* m_gpio_boton_down_flag;
static volatile uint8_t* m_gpio_boton_up_flag;

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


void gpio_inti()
{
    /* LED */
    NRF_GPIO->PIN_CNF[0] = m_led_gpio_config;

    /* BOTON OK */
    NRF_GPIO->PIN_CNF[1] = m_boton_ok_gpio_config;
    NRF_GPIOTE->CONFIG[0] = m_boton_ok_config;
    
    /* BOTON DOWN */
    NRF_GPIO->PIN_CNF[2] = m_boton_down_gpio_config;
    NRF_GPIOTE->CONFIG[1] = m_boton_down_config;

    /* BOTON UP */
    NRF_GPIO->PIN_CNF[3] = m_boton_up_gpio_config;
    NRF_GPIOTE->CONFIG[2] = m_boton_up_config;
    
    NRF_GPIOTE->INTENSET = gpio_intenset;
    NRF_GPIOTE->INTENCLR = gpio_intenclr;
	    
}

void gpio_boton_ok_set_flag(volatile uint8_t* main_boton_ok_flag)
{
    m_gpio_boton_ok_flag = main_boton_ok_flag;
}

void gpio_boton_down_set_flag(volatile uint8_t* main_boton_down_flag)
{
    m_gpio_boton_down_flag = main_boton_down_flag;
}

void gpio_boton_up_set_flag(volatile uint8_t* main_boton_up_flag)
{
    m_gpio_boton_up_flag = main_boton_up_flag;
}

void gpio_led_on()
{
    //NRF_GPIO->OUT = GPIO_OUT_PIN0_High
    NRF_GPIO->OUTCLR = GPIO_OUTCLR_PIN0_Low
    NRF_GPIO->OUTSET = GPIO_OUTSET_PIN0_High
}

void gpio_led_off()
{
    //NRF_GPIO->OUT = GPIO_OUT_PIN0_Low
    NRF_GPIO->OUTSET = GPIO_OUTSET_PIN0_Low
    NRF_GPIO->OUTCLR = GPIO_OUTCLR_PIN0_High
}


