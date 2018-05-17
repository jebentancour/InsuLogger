# Módulos

## Generalidades
Los nombres de los módulos serán escritos en minúsculas y _ en lugar de espacios. Todas las funciones del módulo comenzarán con su nombre ```modulo_```.

Los tipos de datos serán los de la librería ```stdint.h```.

Todo módulo deberá tener la función de inicialización:

```c
/**@brief Funcion de inicializacion del modulo.
 */
void modulo_init(void);
```

## Interrupciones

Las interrupciones son controladas por el NVIC (Nested Vector Interrupt Controller), el mismo forma parte del procesador CorteX-M0 que hay en el SoC. 

Para habilitar una interrupción hay que hacer dos cosas:

- Habilitar el vector de interrupciones en el NVIC
- Habilitar la interrupción en el periférico

Para habilitar el vector de interrupciones se debe llamar a la función ```sd_nvic_EnableIRQ(IRQn_Type IRQn)```, donde ```IRQn_Type``` es uno de los declarados en el archvo ```components/device/nrf51.h```.

Las rutinas de atención a interrupciones están declaradas en el archivo ```components/toolchain/iar/startup_nrf51.s``` en la sección bajo el comentario
 ```External Interrupts```. Deben ser declaradas en el código con la forma: 
 
 ```c
 void PERIFERICOX_IRQHandler(void)
 {
    /* ... */
 }
 ```
 
 Ejemplo con ADC (ojo no considera al SoftDevice):
 
  ```c
static uint8_t result = 0;

void adc_conf(void);

/**
 * main() function
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
  adc_conf();	
  while(true)
  {
    // Wait for interrupt
    __WFI();
    // When interrupt is done, read result for debugging purpose
    result = result;
  }
}

void ADC_IRQHandler(void)
{
	if (NRF_ADC->EVENTS_END != 0)
	{
		NRF_ADC->EVENTS_END = 0;
		result = NRF_ADC->RESULT;
		NRF_ADC->TASKS_STOP = 1;
		NRF_ADC->TASKS_START = 1;
	}
}

void adc_conf(void)
{
    // Configure ADC
    NRF_ADC->INTENSET   = ADC_INTENSET_END_Msk;
    NRF_ADC->CONFIG     = (ADC_CONFIG_RES_8bit << ADC_CONFIG_RES_Pos) |
                          (ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling << ADC_CONFIG_INPSEL_Pos) |
                          (ADC_CONFIG_REFSEL_SupplyOneHalfPrescaling << ADC_CONFIG_REFSEL_Pos) |
                          (ADC_CONFIG_PSEL_AnalogInput5 << ADC_CONFIG_PSEL_Pos) |
                          (ADC_CONFIG_EXTREFSEL_AnalogReference0 << ADC_CONFIG_EXTREFSEL_Pos);
    NRF_ADC->EVENTS_END = 0;
    NRF_ADC->ENABLE     = 1;
    NRF_ADC->TASKS_START = 1;
    // Enable ADC interrupt    
    NVIC_ClearPendingIRQ(ADC_IRQn);    		
    NVIC_EnableIRQ(ADC_IRQn);
}
 ``` 

### Prioridades

Hay que tener cuidado al setear el nivel de prioridad de las interrupciones.

![interrupt_priority](https://github.com/jebentancour/InsuLogger/blob/master/interrupt_priority.PNG)

Podemos usar los niveles 1 o 3:

```c
#define I2C_PRIORITY	1

sd_nvic_SetPriority(SPI0_TWI0_IRQn, I2C_PRIORITY);
sd_nvic_EnableIRQ(SPI0_TWI0_IRQn);
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

## Funcionalidades

### SOFTDEVICE

Es el encargado de manejar la radio del SoC.
Es un archivo binario precompilado proporcionado por Nordic que implementa el stack Bluetooth.

### NUS

Librería proporcionada por Nordic.
BLE NUS es un servicio patentado BLE, que tiene un servicio llamado "UART Service" para imitar una comunicación serial sobre Bluetooth.
NUS configura un "RX" (característica con propiedades de "escritura") y un canal de transmisión "TX" (característica con propiedades de "notificación"), para adaptarse a las necesidades básicas de comunicación UART.
Depende de SOFTDEVICE y TIMER LIBRARY.

### UART

Módulo que enmascara y adapta al proyecto la librería NUS (Nordic UART Service).
Posee buffers de recepción y transmisión que permiten enviar y recibir más de 20 bytes (limitación de NUS).

```c
/* Estructura que repesenta el estado interno del modulo */
typedef struct {
    uint8_t advertising;
    uint8_t connected;
} ble_uart_status_t;

/**@brief Funcion de inicializacion del modulo.
 */
void ble_uart_init(void);

/**@brief Funcion para obtener el estado del modulo.
 */
ble_uart_status_t ble_uart_get_status(void);

/**@brief Funcion para setear la flag donde indicar la llegada de un mensaje completo.
 */
void ble_uart_rx_set_flag(uint8_t*);

/**@brief Funcion para setear la flag donde indicar el fin de trasnmision de un mensaje completo.
 */
void ble_uart_tx_set_flag(uint8_t*);

/**@brief Funcion para obtener el mensaje recibido.
 */
uint16_t ble_uart_get_msg(uint8_t*);

/**@brief Funcion para enviar un mensaje.
 */
void ble_uart_data_send(uint8_t * p_data, uint16_t length);

/**@brief Funcion iniciar el descubrimiento por bluetooth.
 */
void ble_uart_advertising_start(void);

/**@brief Funcion detener el descubrimiento por bluetooth.
 */
void ble_uart_advertising_stop(void);

/**@brief Funcion terminar la conexion bluetooth con otro dispositivo.
 */
void ble_uart_disconnect(void);
```

### TIMER LIBRARY

Es una librería proporcionada por Nordic que permite crear diferentes timers en software partiendo de un mismo recurso de hardware (RTC1).

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
/**@brief Funcion de inicializacion del modulo.
 */
void i2c_init(void);


/**@brief Funcion para setear la flag donde indicar el fin de trasnmision de un byte.
 *
 * @param main_tx_flag    Puntero a una flag donde se indicara el fin de trasnmision de un byte.
 */
void i2c_tx_set_flag(volatile uint8_t* main_tx_flag);


/**@brief Funcion para iniciar una transferencia de datos a un esclavo.
 *
 * @param address    La direccion de 7 bits del esclavo.
 */
void i2c_begin_transmission(uint8_t address);


/**@brief Funcion para trasnmitir un byte a un esclavo.
 *
 * @param value    El dato a trasnmitir.
 */
void i2c_write(uint8_t value);


/**@brief Funcion para terminar la trasnsaccion.
 */
void i2c_end_transmission(void);
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
} NRF_TWI_Type;

/* ... */

/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

/* ... */
#define NRF_TWI0                        ((NRF_TWI_Type            *) NRF_TWI0_BASE)
/* ... */
```

```NRF_TWI0``` es un puntero a una estructura ```NRF_TWI_Type```, para manipular los valores de dicha estructura se debe usar ```NRF_TWI0->miembro_de_la_estructura```.

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

/* ... */
```

Los pines usados para la comunicación son:

```c
#define SCL_PIN         5
#define SDA_PIN 	6
```

### GPIO

Proporciona funciones para manejar pines de entrada (botones) y salida (led y ON/OFF de display).

Poner debouncing de 1ms/5ms.

```c
/**
* Entradas: botones con pull up, el boton lleva el pin a 0 cuando se presiona
* Salidas: led (on/off del display)
*/
void gpio_init();

void gpio_led_on();

void gpio_led_off();

/**
* Conmutar estado led
*/
void gpio_led_toggle();

/**
*/
void gpio_boton_ok_set_flag(volatile uint8_t* gpio_boton_ok_flag);

/**
*/
void gpio_boton_up_set_flag(volatile uint8_t* gpio_boton_up_flag);

/**
*/
void gpio_boton_down_set_flag(volatile uint8_t* gpio_boton_down_flag);
```

InsuLogger:

```c
#define LED 		0
#define BTN_OK 		2
#define BTN_DOWN 	3
#define BTN_UP 		4
```

Circular:

```c
#define LED 		29
#define BTN_OK 		28
```

### ESTADOS

Es el encargado de gestionar el estado general del sistema, distribuir tareas y enviar el chip a dormir.

### LOGGER

Es el encargado de llevar el registro. 
Recibe comandos desde SHELL y ESTADOS pra registrar los diferentes eventos.
Utiliza los datos suministrados por RTC para registrar el momento en que dan los eventos.

### USER INTERFACE

Es el encargado de la interacción con el usuario. Muestra los diferentes menús en el display y toma como la entrada los botones presionados por el usuario.

```c

/* Estructura para representar los eventos */
enum event_type {
    wellcome,
    good_bye,
    pressed_ok,
    pressed_up,
    pressed_down
};

void ui_inti();

void ui_process_event(enum event_type);

```

### DISPLAY

Libreria que permite el manejo del display utilizado.
Proporciona funciones para la inicializar el hardware y controlar los píxeles.

### SHELL

Es capaz de procesar los comandos recibidos por UART, llamar a las funciones correspondientes y devolver el resultado.
