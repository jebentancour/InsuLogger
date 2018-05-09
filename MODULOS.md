# Módulos


## Funcionalidades

### RTC

Módulo que implementa un reloj de tiempo real (Real Time Clock), utilizado para llevar la cuenta del tiempo entre los eventos.
El SoC cuenta con dos Real Time Counters (RTC0 y RTC1) uno de ellos es utilizado por SOFTDEVICE y el otro por TIMER LIBRARY.
Por esto el módulo RTC utiliza la librería TIMER LIBRARY de la capa de abstracción de hardware.

Primero hay que inicializar el Modulo UART antes del Modulo RTC, ya que el primero es el que inicializa TIMER LIBRARY, con el cual se comparte el RTC1 (llamar a **ble_uart_init**). Hay que leer el infocenter sobre Timer Library (Software Development Kit > nRF5 SDK > nRF5 SDK v12.3.0 > Libraries > Timer Library).

```c

define tiempo

/** brief 
* Setar el timer por software con el tiempo que querramos. Se pasa una funcion para hacer un callback
*/
void rtc_init()

/** brief 
* Funcion a la que se llamará cuando se genera una interrupcion del RTC
* Incrementa el tiempo
*/
void rtc_callback()

/** brief 
*/
void rtc_reset()

/** brief 
*/
void rtc_set()

/** brief 
*/
void rtc_get()

```

### I2C

Proporciona funciones para manejar el periférico TWI (Two Wire Interface, compatible con I2C) del SoC.
Imitar comportamiento de libreria: [Wire](https://www.arduino.cc/en/Reference/Wire)
```c
/** brief 
*/
void i2c_init()

/** brief 
*/
requestFrom()

beginTransmission()

endTransmission()

write()

available()

read()
SetClock()
onReceive()
onRequest()
```

### GPIO

Proporciona funciones para manejar pines de entrada (botones) y salida (led y ON/OFF de display).
Poner debouncing de 1ms/5ms
```c
/** brief
Botones con Pull up porque estan cortocicuitados a + (Apretado = 0)
Salidas: LED y Pin On/Off del display
*/
void gpio_inti()

void gpio_display_on()

void gpio_display_off()

void gpio_led_on()

void gpio_led_off()

/**
Conmutar estado led
*/
void gpio_led_toggle()

/**
*/
void gpio_boton_ok_set_flag(uint8_t *gpio_boton_ok_flag)

/**
*/
void gpio_boton_up_set_flag(uint8_t)

/**
*/
void gpio_boton_down_set_flag(uint8_t)

/**
ACA VAN LAS ISR TMAMBÉN
*/
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

