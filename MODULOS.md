# Módulos


## Funcionalidades

### RTC

Módulo que implementa un reloj de tiempo real (Real Time Clock), utilizado para llevar la cuenta del tiempo entre los eventos.
El SoC cuenta con dos Real Time Counters (RTC0 y RTC1) uno de ellos es utilizado por SOFTDEVICE y el otro por TIMER LIBRARY.
Por esto el módulo RTC utiliza la librería TIMER LIBRARY de la capa de abstracción de hardware.

Primero hay que inicializar el Modulo UART antes del Modulo RTC, ya que el primero es el que inicializa TIMER LIBRARY, con el cual se comparte el RTC1 (llamar a **ble_uart_init**). Hay que leer el infocenter sobre Timer Library (Software Development Kit > nRF5 SDK > nRF5 SDK v12.3.0 > Libraries > Timer Library).

```c

/* Definir una estructura para el tiempo */

/**
* Setar el timer por software con el tiempo que querramos. Se pasa una funcion para hacer un callback
*/
void rtc_init();

/** 
* Funcion a la que se llamará cuando se genera una interrupcion del RTC
* Incrementa el tiempo
*/
void rtc_callback();

/**
*/
void rtc_reset();

/**
*/
void rtc_set();

/**
*/
void rtc_get();

```

### I2C

Proporciona funciones para manejar el periférico TWI (Two Wire Interface, compatible con I2C) del SoC.

Se imitará comportamiento de libreria [Wire](https://www.arduino.cc/en/Reference/Wire) de Arduino.

```c
begin();
requestFrom();
beginTransmission();
endTransmission();
write();
available();
read();
setClock();
onReceive();
onRequest();
```

Circuito integrado utilizado para manejar el display [SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf).

La librería Arduino de referencia es [ACROBOTIC_SSD1306](https://github.com/kodera2t/nRF51822_sample/tree/master/ACROBOTIC_SSD1306). Esta solo llama a las funciones de Wire en las siguientes funciones:

```c
void ACROBOTIC_SSD1306::sendCommand(unsigned char command)
{
  Wire.beginTransmission(SSD1306_Address);    // begin I2C communication
  Wire.write(SSD1306_Command_Mode);           // Set OLED Command mode
  Wire.write(command);
  Wire.endTransmission();                       // End I2C communication
}

void ACROBOTIC_SSD1306::sendData(unsigned char Data)
{
     Wire.beginTransmission(SSD1306_Address); // begin I2C transmission
     Wire.write(SSD1306_Data_Mode);            // data mode
     Wire.write(Data);
     Wire.endTransmission();                    // stop I2C transmission
}
```

Se plantea implementar las siguientes funciones:

```c
/**
* Initiate the moduley and join the I2C bus as a master.
*/
void i2c_init();

/**
* Begin a transmission to the I2C slave device with the given address. 
* Subsequently, queue bytes for transmission with the i2c_write() 
* function and transmit them by calling i2c_end_transmission().
* address: the 7-bit address of the device to transmit to
*/
void i2c_begin_transmission(uint8_t address);

/**
* Queues bytes for transmission from a master to slave device.
* Must be called in-between calls to i2c_begin_transmission() and i2c_end_transmission().
* value: a value to send as a single byte
*/
void i2c_write(uint8_t value);

/**
* Ends a transmission to a slave device that was begun by i2c_begin_transmission() 
* and transmits the bytes that were queued by i2c_write().
*/
void i2c_end_transmission();
```

Los registros se encuentran declarados en el archivo ```components/device/nrf51.h```.

```c
/* ... */

/* ================================================================================ */
/* ================                       TWI                      ================ */
/* ================================================================================ */


/**
  * @brief Two-wire interface master 0. (TWI)
  */

typedef struct {                                    /*!< TWI Structure */
  /* ... */
  __IO uint32_t  ENABLE;                            /*!< Enable two-wire master. */
  __I  uint32_t  RESERVED12;
  __IO uint32_t  PSELSCL;                           /*!< Pin select for SCL. */
  __IO uint32_t  PSELSDA;                           /*!< Pin select for SDA. */
  __I  uint32_t  RESERVED13[2];
  __I  uint32_t  RXD;                               /*!< RX data register. */
  __IO uint32_t  TXD;                               /*!< TX data register. */
  __I  uint32_t  RESERVED14;
  __IO uint32_t  FREQUENCY;                         /*!< Two-wire frequency. */
  __I  uint32_t  RESERVED15[24];
  __IO uint32_t  ADDRESS;                           /*!< Address used in the two-wire transfer. */
  __I  uint32_t  RESERVED16[668];
  __IO uint32_t  POWER;                             /*!< Peripheral power control. */
} NRF_TWI_Type;

/* ... */

/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

/* ... */
#define NRF_TWI0                        ((NRF_TWI_Type            *) NRF_TWI0_BASE)
/* ... */
```

```NRF_TWI0``` es un puntero a una estructura ```NRF_TWI_Type```, para manipular los valores de ducha estructura se debe usar ```NRF_TWI0->```.

Por ejemplo:

```c
NRF_TWI0->ENABLE = X;
```

Otro archivo para mirar es el ```components/device/nrf51_bitfields.h```, en este se definen los distintos valores que pueden llegar a tener los registros.

```c
/* ... */

/* Peripheral: TWI */
/* Description: Two-wire interface master 0. */

/* ... */

/* Register: TWI_ENABLE */
/* Description: Enable two-wire master. */

/* Bits 2..0 : Enable or disable W2M */
#define TWI_ENABLE_ENABLE_Pos (0UL) /*!< Position of ENABLE field. */
#define TWI_ENABLE_ENABLE_Msk (0x7UL << TWI_ENABLE_ENABLE_Pos) /*!< Bit mask of ENABLE field. */
#define TWI_ENABLE_ENABLE_Disabled (0x00UL) /*!< Disabled. */
#define TWI_ENABLE_ENABLE_Enabled (0x05UL) /*!< Enabled. */

/* Register: TWI_RXD */
/* Description: RX data register. */

/* Bits 7..0 : RX data from last transfer. */
#define TWI_RXD_RXD_Pos (0UL) /*!< Position of RXD field. */
#define TWI_RXD_RXD_Msk (0xFFUL << TWI_RXD_RXD_Pos) /*!< Bit mask of RXD field. */

/* Register: TWI_TXD */
/* Description: TX data register. */

/* Bits 7..0 : TX data for next transfer. */
#define TWI_TXD_TXD_Pos (0UL) /*!< Position of TXD field. */
#define TWI_TXD_TXD_Msk (0xFFUL << TWI_TXD_TXD_Pos) /*!< Bit mask of TXD field. */

/* Register: TWI_FREQUENCY */
/* Description: Two-wire frequency. */

/* Bits 31..0 : Two-wire master clock frequency. */
#define TWI_FREQUENCY_FREQUENCY_Pos (0UL) /*!< Position of FREQUENCY field. */
#define TWI_FREQUENCY_FREQUENCY_Msk (0xFFFFFFFFUL << TWI_FREQUENCY_FREQUENCY_Pos) /*!< Bit mask of FREQUENCY field. */
#define TWI_FREQUENCY_FREQUENCY_K100 (0x01980000UL) /*!< 100 kbps. */
#define TWI_FREQUENCY_FREQUENCY_K250 (0x04000000UL) /*!< 250 kbps. */
#define TWI_FREQUENCY_FREQUENCY_K400 (0x06680000UL) /*!< 400 kbps (actual rate 410.256 kbps). */

/* Register: TWI_ADDRESS */
/* Description: Address used in the two-wire transfer. */

/* Bits 6..0 : Two-wire address. */
#define TWI_ADDRESS_ADDRESS_Pos (0UL) /*!< Position of ADDRESS field. */
#define TWI_ADDRESS_ADDRESS_Msk (0x7FUL << TWI_ADDRESS_ADDRESS_Pos) /*!< Bit mask of ADDRESS field. */

/* Register: TWI_POWER */
/* Description: Peripheral power control. */

/* Bit 0 : Peripheral power control. */
#define TWI_POWER_POWER_Pos (0UL) /*!< Position of POWER field. */
#define TWI_POWER_POWER_Msk (0x1UL << TWI_POWER_POWER_Pos) /*!< Bit mask of POWER field. */
#define TWI_POWER_POWER_Disabled (0UL) /*!< Module power disabled. */
#define TWI_POWER_POWER_Enabled (1UL) /*!< Module power enabled. */

/* ... */

```

### GPIO

Proporciona funciones para manejar pines de entrada (botones) y salida (led y ON/OFF de display).

Poner debouncing de 1ms/5ms.

```c
/**
* Entradas: botones con pull up, el boton lo lleva a 0 cuando se presiona
* Salidas: led y pin on/off del display
*/
void gpio_inti();

void gpio_display_on();

void gpio_display_off();

void gpio_led_on();

void gpio_led_off();

/**
* Conmutar estado led
*/
void gpio_led_toggle();

/**
*/
void gpio_boton_ok_set_flag(uint8_t* gpio_boton_ok_flag);

/**
*/
void gpio_boton_up_set_flag(uint8_t* gpio_boton_up_flag);

/**
*/
void gpio_boton_down_set_flag(uint8_t* gpio_boton_down_flag);

/**
* ACA VAN LAS ISR TMAMBÉN (son static)
*/
```

InsuLogger:

```c
#define LED NRF_GPIO_PIN_MAP(0, 0)
#define BTN_OK NRF_GPIO_PIN_MAP(0, 2)
#define BTN_DOWN NRF_GPIO_PIN_MAP(0, 3)
#define BTN_UP NRF_GPIO_PIN_MAP(0, 4)
```

Circular:

```c
#define LED NRF_GPIO_PIN_MAP(0, 29)
#define BTN_OK NRF_GPIO_PIN_MAP(0, 28)
```

### ESTADOS

Es el encargado de gestionar el estado general del sistema, distribuir tareas y enviar el chip a dormir.

### LOGGER

Es el encargado de llevar el registro. 
Recibe comandos desde SHELL y ESTADOS pra registrar los diferentes eventos.
Utiliza los datos suministrados por RTC para registrar el momento en que dan los eventos.

### USER INTERFACE

Es el encargado de la interacción con el usuario. Muestra los diferentes menús en el display y toma como la entrada los botones presionados por el usuario.

### DISPLAY

Libreria que permite el manejo del display utilizado.
Proporciona funciones para la inicializar el hardware y controlar los píxeles.

### SHELL

Es capaz de procesar los comandos recibidos por UART, llamar a las funciones correspondientes y devolver el resultado.

### UART

Módulo que enmascara y adapta al proyecto la librería NUS (Nordic UART Service).
Posee buffers de recepción y transmisión que permiten enviar y recibir más de 20 bytes (limitación de NUS).

### NUS

Librería proporcionada por Nordic.
BLE NUS es un servicio patentado BLE, que tiene un servicio llamado "UART Service" para imitar una comunicación serial sobre Bluetooth.
NUS configura un "RX" (característica con propiedades de "escritura") y un canal de transmisión "TX" (característica con propiedades de "notificación"), para adaptarse a las necesidades básicas de comunicación UART.
Depende de SOFTDEVICE y TIMER LIBRARY.

### SOFTDEVICE

Es el encargado de manejar la radio del SoC.
Es un archivo binario precompilado proporcionado por Nordic que implementa el stack Bluetooth.

### TIMER LIBRARY

Es una librería proporcionada por Nordic que permite crear diferentes timers en software partiendo de un mismo recurso de hardware.

## Generalidades
Los nombres de los módulos serán escritos en minúsculas y _ en lugar de espacios. Todas las funciones del módulo comenzarán con su nombre ```modulo_```.

Los tipos de datos serán los de la librería ```stdint.h```.

Todo módulo deberá tener la función de inicialización:

```c
/**@brief Funcion de inicializacion del modulo.
 */
void modulo_init(void);
```

## Secciones críticas

Las secciones críticas del código deben manejarse de la siguiente forma:

```c
CRITICAL_REGION_ENTER();
    /* Seccion critica */
CRITICAL_REGION_EXIT();
```

## Flags

Las banderas que sean necesarias usar en el Round Robin y sean modificadas por el módulo serán pasadas al mismo utilizando el mecanismo:

```c
/**@brief Funcion para setear una flag.
 *
 * @details Que indica la falg.
 *
 * @param main_flag    Puntero a una flag.
 */
void modulo_nombre_set_flag(uint8_t*);
```

Internamente serán manejadas de la siguiente forma:

```c
static uint8_t* m_nombre_flag; /* Declaracion interna de la flag */

/**@brief Funcion para setear una flag.
 *
 * @details Que indica la falg.
 *
 * @param main_flag    Puntero a una flag.
 */
void modulo_nombre_set_flag(uint8_t* main_flag)
{
    m_nombre_flag = main_flag;
}

/**@brief Funcion de ejemplo que moddifica una flag.
 */
void funcion_que_modfica_flag(void){
    *m_nombre_flag = X;
}
```

En el main:

```c
uint8_t modulo_nombre_flag; /* Declaracion global de la flag */

int main(void)
{
    /* Inicializacion */
    modulo_nombre_set_flag(&modulo_nombre_flag);
    modulo_init();
    
    /* Loop principal */
    for (;;)
    {
        if (modulo_nombre_flag)
        {
            /* Rutina de atencion a la flag */
        }
    }
}
```
