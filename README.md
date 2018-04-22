# InsuLogger

SoC [nRF51822](https://www.nordicsemi.com/eng/nordic/download_resource/62726/14/39584073/13358) QFAAH10

Code size: 256kB

RAM size: 16 kB

[INFOCENTER](http://infocenter.nordicsemi.com)

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

# SoftDevice Specification
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

# Hardware

Placa circular:

```c
#define LED NRF_GPIO_PIN_MAP(0, 29)
```

Placa InsuLogger:

```c
#define LED NRF_GPIO_PIN_MAP(0, 0)
```
