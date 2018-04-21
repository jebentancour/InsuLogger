# InsuLogger

SoC nRF51822 QFAAH10

Code size: 256kB

RAM size: 16 kB

## SoftDevices
Nordic Semiconductor protocol stacks are known as SoftDevices.
SoftDevices are pre-compiled, pre-linked binary files. 
SoftDevices can be programmed in nRF5 series devices, and are freely downloadable from the Nordic website.

|Code	    |Name                                               |Version|
|-----------|---------------------------------------------------|-------|
|S110-SD-v5	|S110 nRF51822 SoftDevice (Production ready)	    |5.2.1  |    
|S110-SD-v6	|S110 nRF51822 SoftDevice (Production ready)	    |6.2.1  |    
|S110-SD-v7	|S110 nRF51822 SoftDevice (Production ready)	    |7.3.0  |    
|S110-SD-v8	|S110 nRF51 SoftDevice (Production ready)	        |8.0.0  |    
|S120-SD-v1	|S120 nRF51822 SoftDevice v1.0.1 (Production ready)	|1.0.1  |    
|S120-SD-v2	|S120 nRF51 SoftDevice (Production ready)	        |2.1.0  |    
|S130-SD	|S130 nRF51 Series SoftDevice (production ready)	|1.0.0  |    
|**S130-SD-v2**	|**S130 nRF51 SoftDevice (Production ready)**	        |**2.0.1**  |    

## Software Development Kit
Nordic Semiconductor's Software Development Kits (SDK) are your starting point for software 
development on the nRF51 and nRF52 Series. 
It contains source code libraries and example applications covering wireless functions, 
libraries for all peripherals, bootloaders, Wired and OTA FW upgrades, RTOS examples, 
serialization libraries and more.

[SOFTWARE DEVELOPMENT KIT](https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v12.x.x/)

|Code	            |Name	                                            |Version|
|-------------------|---------------------------------------------------|-------|
|nRF5-SDK-for-Mesh	|nRF5 SDK for Mesh zip file	                        |1.0.1  |
|**nRF5-SDK-v12-zip**	|**nRF5 SDK Zip File - works with S132 v3 and S130 v2**	|**12.3.0** |

## nRF5 SDK v12.3.0
This version of the SDK supports the following SoftDevices:

|Name            |Version|
|----------------|-------|
|**S130 SoftDevice** |**v2.0.1** |
|S132 SoftDevice |v3.0.0 |
|S212 SoftDevice |v2.0.0 |
|S332 SoftDevice |v2.0.0 |

## nRF DK
The following boards are supported:

|Board       |SoC                                                     |
|------------|--------------------------------------------------------|	    
|nRF6310     |(part of nRFgo Starter Kit)                             |                    
|**PCA10000**|**(nRF51822 USB dongle) **                                  |
|**PCA10001**|**(part of nRF51822 Evaluation Kit)**                       |
|PCA10002    |(nRF51422 USB dongle)                                   |
|PCA10003    |(part of nRF51422 Evaluation Kit, BLE + ANT)            |
|PCA10028    |(part of nRF51422 Evaluation Kit, Arduino form factor)  |
|PCA10031    |(nRF51422 USB dongle)	                                  |
|PCA10036    |(part of nRF52832 Preview Development Kit)              |
|PCA10040    |(part of nRF52832 Development Kit)                      |
|PCA10056    |(part of nRF52840 Preview Development Kit)              |
|**PCA20006**|**(nRF51822 Beacon board)**                                 |
|D52DK1      |(Dynastream D52 Starter Kit)                            |
|WT51822     |(Wavetek shield)                                        |   

# Hardware

Placa circular:

```c
#define LED NRF_GPIO_PIN_MAP(0, 29)
```

Placa InsuLogger:

```c
#define LED NRF_GPIO_PIN_MAP(0, 0)
```
