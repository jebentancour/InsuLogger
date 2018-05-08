# InsuLogger

SoC [nRF51822](https://www.nordicsemi.com/eng/nordic/download_resource/62726/14/39584073/13358) QFAAH10 (256kB flash, 16 kB RAM)

[INFOCENTER](http://infocenter.nordicsemi.com)

|ID |Base address |Peripheral  |Instance    |Description                                  |Used by           | 
|---|-------------|------------|------------|---------------------------------------------|------------------|
|0  |0x40000000   |POWER       |POWER       |Power Control.                               |SoftDevice        |
|0  |0x40000000   |CLOCK       |CLOCK       |Clock Control.                               |SoftDevice        |
|0  |0x40000000   |MPU         |MPU         |Memory Protection Unit.                      |SoftDevice        |
|1  |0x40001000   |RADIO       |RADIO       |2.4 GHz Radio.                               |SoftDevice        |
|2  |0x40002000   |UART        |UART0       |Universal Asynchronous Receiver/Transmitter. |                  |
|3  |0x40003000   |SPI         |SPI0        |SPI Master.                                  |                  |
|3  |0x40003000   |TWI         |TWI0        |I2C compatible Two-Wire Interface 0.         |                  |
|4  |0x40004000   |SPIS        |SPIS1       |SPI Slave.                                   |                  |
|4  |0x40004000   |SPI         |SPI1        |SPI Master.                                  |                  |
|4  |0x40004000   |TWI         |TWI1        |I2C compatible Two-Wire Interface 1.         |                  |
|6  |0x40006000   |GPIOTE      |GPIOTE      |GPIO Task and Events.                        |                  |
|7  |0x40007000   |ADC         |ADC         |Analog to Digital Converter.                 |                  |
|8  |0x40008000   |TIMER       |TIMER0      |Timer/Counter 0.                             |SoftDevice        |
|9  |0x40009000   |TIMER       |TIMER1      |Timer/Counter 1.                             |                  |
|10 |0x4000A000   |TIMER       |TIMER2      |Timer/Counter 2.                             |                  |
|11 |0x4000B000   |RTC         |RTC0        |Real Time Counter 0.                         |SoftDevice        |
|12 |0x4000C000   |TEMP        |TEMP        |Temperature Sensor.                          |SoftDevice        |
|13 |0x4000D000   |RNG         |RNG         |Random Number Generator.                     |SoftDevice        |
|14 |0x4000E000   |ECB         |ECB         |Crypto AES ECB.                              |SoftDevice        |
|15 |0x4000F000   |CCM         |CCM         |AES Crypto CCM.                              |SoftDevice        |
|15 |0x4000F000   |AAR         |AAR         |Accelerated Address Resolver.                |SoftDevice        |
|16 |0x40010000   |WDT         |WDT         |Watchdog Timer.                              |                  |
|17 |0x40011000   |RTC         |RTC1        |Real Time Counter 1.                         |Timer library     |
|18 |0x40012000   |QDEC        |QDEC        |Quadrature Decoder.                          |                  |
|19 |0x40013000   |LPCOMP      |LPCOMP      |Low Power Comparator.                        |                  |
|20 |0x40014000   |SWI         |SWI0        |Software interrupt.                          |Timer library     |
|21 |0x40015000   |SWI         |SWI1        |Software interrupt.                          |SoftDevice        |
|22 |0x40016000   |SWI         |SWI2        |Software interrupt                           |SoftDevice        |
|23 |0x40017000   |SWI         |SWI3        |Software interrupt.                          |                  |
|24 |0x40018000   |SWI         |SWI4        |Software interrupt.                          |SoftDevice        |
|25 |0x40019000   |SWI         |SWI5        |Software interrupt.                          |SoftDevice        |
|30 |0x4001E000   |NVMC        |NVMC        |Non-Volatile Memory Controller.              |SoftDevice        |
|31 |0x4001F000   |PPI         |PPI         |Programmable Peripheral Interconnect.        |                  |
|NA |0x50000000   |GPIO        |GPIO        |General Purpose Input and Output.            |                  |
|NA |0x10000000   |FICR        |FICR        |Factory Information Configuration Registers. |SoftDevice        |
|NA |0x10001000   |UICR        |UICR        |User Information Configuration Registers.    |SoftDevice        |

## SoftDevices
Nordic Semiconductor protocol stacks are known as SoftDevices.
SoftDevices are pre-compiled, pre-linked binary files. 
SoftDevices can be programmed in nRF5 series devices, and are freely downloadable from the Nordic website.

|Code	          |Name                                               |Version  | 
|---------------|---------------------------------------------------|---------|
|S110-SD-v5	    |S110 nRF51822 SoftDevice (Production ready)	      |5.2.1    |    
|S110-SD-v6	    |S110 nRF51822 SoftDevice (Production ready)	      |6.2.1    |    
|S110-SD-v7	    |S110 nRF51822 SoftDevice (Production ready)	      |7.3.0    |    
|S110-SD-v8	    |S110 nRF51 SoftDevice (Production ready)	          |8.0.0    |    
|S120-SD-v1	    |S120 nRF51822 SoftDevice v1.0.1 (Production ready)	|1.0.1    |    
|S120-SD-v2	    |S120 nRF51 SoftDevice (Production ready)	          |2.1.0    |    
|S130-SD	      |S130 nRF51 Series SoftDevice (production ready)	  |1.0.0    |    
|**S130-SD-v2**	|**S130 nRF51 SoftDevice (Production ready)**	      |**2.0.1**|    

## Software Development Kit
Nordic Semiconductor's Software Development Kits (SDK) are your starting point for software 
development on the nRF51 and nRF52 Series. 
It contains source code libraries and example applications covering wireless functions, 
libraries for all peripherals, bootloaders, Wired and OTA FW upgrades, RTOS examples, 
serialization libraries and more.

[SOFTWARE DEVELOPMENT KIT](https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v12.x.x/)

|Code	               |Name	                                                |Version   |
|--------------------|------------------------------------------------------|----------|
|nRF5-SDK-for-Mesh	 |nRF5 SDK for Mesh zip file	                          |1.0.1     |
|**nRF5-SDK-v12-zip**|**nRF5 SDK Zip File - works with S132 v3 and S130 v2**|**12.3.0**|

## nRF5 SDK v12.3.0

This version of the SDK supports the following SoftDevices:

|Name               |Version   |
|-------------------|----------|
|**S130 SoftDevice**|**v2.0.1**|
|S132 SoftDevice    |v3.0.0    |
|S212 SoftDevice    |v2.0.0    |
|S332 SoftDevice    |v2.0.0    |

## nRF DK
The following boards are supported:

|Board       |SoC                                                     |
|------------|--------------------------------------------------------|	    
|nRF6310     |(part of nRFgo Starter Kit)                             |                    
|**PCA10000**|**(nRF51822 USB dongle)**                               |
|**PCA10001**|**(part of nRF51822 Evaluation Kit)**                   |
|PCA10002    |(nRF51422 USB dongle)                                   |
|PCA10003    |(part of nRF51422 Evaluation Kit, BLE + ANT)            |
|PCA10028    |(part of nRF51422 Evaluation Kit, Arduino form factor)  |
|PCA10031    |(nRF51422 USB dongle)	                                  |
|PCA10036    |(part of nRF52832 Preview Development Kit)              |
|PCA10040    |(part of nRF52832 Development Kit)                      |
|PCA10056    |(part of nRF52840 Preview Development Kit)              |
|**PCA20006**|**(nRF51822 Beacon board)**                             |
|D52DK1      |(Dynastream D52 Starter Kit)                            |
|WT51822     |(Wavetek shield)                                        |   

## Nordic Uart Service

The Nordic UART Service is a proprietary bluetooth service. This essentially means that it has a 128 bit UUID base, which is random generated (according to UUID generation spec), as compared to adopted profiles and services from Bluetooth SIG which are addressed using a 16 bit UUID.

BLE NUS is a proprietary BLE service, which has a service named "UART service" to mimic the older Bluetooth classic RFCOMM profile (UART over BT). This is one of the proprietary profiles included in the nRF5x SDK, and it is provided with support both on the nRF side, as well as the central side (Android/PC/iOS).

NUS sets up one "RX" (characteristic with "write" properties) and one "TX" (characteristic with "notify" properties) datachannel, to fit basic UART communication needs.

NUS uses the SoftDevice and Timer library.

### S130 SoftDevice Specification

[S130_SDS_v2.0](http://infocenter.nordicsemi.com/pdf/S130_SDS_v2.0.pdf)

Peripheral protection and usage by SoftDevice:

|ID| Base address | Instance                    | Enabled       | Disabled |
|--|--------------|-----------------------------|---------------|----------|
|0 |0x40000000    | CLOCK                       |Restricted     |Open      |
|0 |0x40000000    | POWER                       |Restricted     |Open      |
|0 |0x40000000    | MPU                         |Restricted     |Open      |
|1 |0x40001000    | RADIO                       |Blocked        |Open      |
|2 |0x40002000    | UART0                       |Open           |Open      |
|3 |0x40003000    | SPI0 / TWI0                 |Open           |Open      |
|4 |0x40004000    | SPI1 / SPIS1 / TWI1         |Open           |Open      |
|6 |0x40006000    | GPIOTE                      |Open           |Open      |
|7 |0x40007000    | ADC                         |Open           |Open      |
|8 |0x40008000    | TIMER0                      |Blocked        |Open      |
|9 |0x40009000    | TIMER1                      |Open           |Open      |
|10|0x4000A000    | TIMER2                      |Open           |Open      |
|11|0x4000B000    | RTC0                        |Blocked        |Open      |
|12|0x4000C000    | TEMP                        |Restricted     |Open      |
|13|0x4000D000    | RNG                         |Restricted     |Open      |
|14|0x4000E000    | ECB                         |Restricted     |Open      |
|15|0x4000F000    | CCM                         |Blocked        |Open      |
|15|0x4000F000    | AAR                         |Blocked        |Open      |
|16|0x40010000    | WDT                         |Open           |Open      |
|17|0x40011000    | RTC1                        |Open           |Open      |
|18|0x40012000    | QDEC                        |Open           |Open      |
|19|0x40013000    | LPCOMP                      |Open           |Open      |
|20|0x40014000    | SWI0                        |Open           |Open      |
|21|0x40015000    | SWI1 / Radio Notification   |Restricted     |Open      |
|22|0x40016000    | SWI2 / SoftDevice Event     |Blocked        |Open      |
|23|0x40017000    | SWI3                        |Open           |Open      |
|24|0x40018000    | SWI4                        |Blocked        |Open      |
|25|0x40019000    | SWI5                        |Blocked        |Open      |
|30|0x4001E000    | NVMC                        |Restricted     |Open      |
|31|0x4001F000    | PPI                         |Open           |Open      |
|NA|0x10000000    | FICR                        |Blocked        |Blocked   |
|NA|0x10001000    | UICR                        |Restricted     |Open      |
|NA|0x50000000    | GPIO P0                     |Open           |Open      |
|NA|0xE000E100    | NVIC                        |Restricted     |Open      |

### Timer library

The timer library enables the application to create multiple timer instances based on the **RTC1** peripheral. Checking for time-outs and invoking the user time-out handlers is performed in the RTC1 interrupt handler. List handling is done using a software interrupt (**SWI0**). Both interrupt handlers are running in ```APP_LOW``` priority level.

Use the macro ```APP_TIMER_INIT``` to initialize the library. This macro allocates memory for internal queues and performs the library initialization. **Because of the memory initialization, it should not be called more than once**. During initialization, you can provide a pointer to a scheduler function that will be called when any timer expires. This function can forward the handling of timer expiration to a different context.

```c
APP_TIMER_INIT(PRESCALER, OP_QUEUE_SIZE, scheduler_function)
```

To define a timer, use the APP_TIMER_DEF macro. This macro allocates memory for the timer instance and declares an identifier that can later on be used to refer to the specific instance. Before starting a timer, the timer must be created.

```c
APP_TIMER_DEF(my_timer_id);
err_code = app_timer_create(&my_timer_id, mode, timeout_handler)
```

After the timer is created, it can be controlled using ```app_timer_start``` and ```app_timer_stop```.

## GPIOs

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
