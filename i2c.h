#include <stdint.h>

/**
* Initiate the moduley and join the I2C bus as a master.
*/
void i2c_init(void);

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
void i2c_end_transmission(void);