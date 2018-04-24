#include <stdint.h>

typedef struct {
    int advertising;
    int connected;
} ble_uart_status_t;

void ble_uart_init(void);

ble_uart_status_t ble_uart_get_status(void);

void ble_uart_set_flag(uint8_t*);

uint16_t ble_uart_get_msg(uint8_t*);

void ble_uart_advertising_start(void);

void ble_uart_advertising_stop(void);

void ble_uart_disconnect(void);

uint32_t ble_uart_data_send(uint8_t * p_data, uint16_t length);