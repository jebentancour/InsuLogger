/**
 * @defgroup UI
 * @{
 *
 * @paragraph 
 * 
 * Muestra los diferentes menús en el display y toma como la entrada los botones presionados por el usuario.
 * 
 * @file ui.h
 * 
 * @version 1.0
 * @author  Rodrigo De Soto, Maite Gil, José Bentancour.
 * @date 12 Julio 2018
 * 
 * @brief Módulo encargado de la interacción con el usuario.
 */

#include <stdint.h>

/** Estructura para representar los eventos */
typedef enum {
    pressed_ok,
    pressed_up,
    pressed_down,
    time_update
} event_t;


/**@brief Función de Inicialización del módulo.
 *
 * @warning Se debe inicializar el módulo gpio antes de llamar a esta función.
 */
void ui_init(void);


/**@brief Función procesar los eventos.
 *
 * @param event    Evento del tipo event_t.
 */
void ui_process_event(event_t event);


/**@brief Función para apagar ui.
 */
void ui_off(void);