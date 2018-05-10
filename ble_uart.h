#include <stdint.h>

#define MAX_LEN                         50                      /* Largo de mensaje maximo */

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
 *
 * @details Se considera que un mensaje esta completo cuando se recibe el caracter '\n'.
 *
 * @param main_rx_flag    Puntero a una flag donde se indicara la llegada de un mensaje completo.
 */
void ble_uart_rx_set_flag(uint8_t*);


/**@brief Funcion para setear la flag donde indicar el fin de trasnmision de un mensaje completo.
 *
 * @param main_tx_flag    Puntero a una flag donde se indicara la llegada de un mensaje completo.
 */
void ble_uart_tx_set_flag(uint8_t*);


/**@brief Funcion para obtener el mensaje recibido.
 *
 * @param msg_main    Puntero al arreglo donde se copiara el mensaje.
 *
 * @return len        El largo del mensaje.
 */
uint16_t ble_uart_get_msg(uint8_t*);

/**@brief Funcion para enviar un mensaje.
 *
 * @param msg_main  Puntero al arreglo donde se encuentra el mensaje.
 * @param length    El largo del mensaje.
 *
 */
void ble_uart_data_send(uint8_t * p_data, uint16_t length);


/**@brief Funcion iniciar el descubrimiento por bluetooth.
 *
 * @warning Se debe esperar a que transcurra el tiempo APP_ADV_TIMEOUT_IN_SECONDS para volver a llamarla.
 */
void ble_uart_advertising_start(void);


/**@brief Funcion detener el descubrimiento por bluetooth.
 */
void ble_uart_advertising_stop(void);


/**@brief Funcion terminar la conexion bluetooth con otro dispositivo.
 */
void ble_uart_disconnect(void);
