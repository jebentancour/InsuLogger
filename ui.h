#include <stdint.h>

/* Estructura para representar los eventos */
typedef enum {
    pressed_ok,
    pressed_up,
    pressed_down,
    time_update
} event_t;


/**@brief Funcion de inicializacion del modulo.
 *
 * @warning Se debe inicializar el modulo gpio antes de llamar a esta funcion
 */
void ui_init(void);


/**@brief Funcion procesar los eventos.
 *
 * @param event    Evento del tipo event_t.
 */
void ui_process_event(event_t event);


/**@brief Funcion para apagar ui.
 */
void ui_off(void);