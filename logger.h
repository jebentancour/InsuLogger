/**@brief Funcion que inicializa al modulo logger, setea los punteros y el numero de registros a 0.
 */
void logger_init(void);


/**@brief Funcion para ingresar un nuevo registro a la memoria.
 *
 * @param glicemia  Variable que guarda el valor de glucosa en sangre.
 * @param type      Variable que guarda el tipo de insulina inyectada.
 * @param dosis     Variable que guarda el valor de dosis inyectada.
 * @param timestamp Variable que guarda el tiempo offset en el momento de la nueva inyeccion.
 */ 
void logger_new_register(uint32_t glicemia, uint8_t type, uint8_t dosis, uint32_t timestamp);


/**@brief Funcion que setea la flag para avisar que hay nuevos mensajes para mandar.
 */
void logger_set_flag(volatile uint8_t * m_send_flag);


/**@brief funcion que llama el shell cuando pide un numero x de registros.
 */
int logger_get(unsigned int argc, char** argv);


/**@brief Funcion que llama el main para enviar por la uart los registros solicitados.
 *
 * @param p_uart    Puntero que apunta al mensaje a ser transmitido por la uart.
 * @return          Devuelve un entero que es el largo del mensaje a ser transmitido.
 */
uint8_t logger_send(uint8_t * p_uart);


/**@brief Funcion que se llama desde el shell y reseta la fecha y hora de referencia, se borran los registros.
 */
int logger_reset(unsigned int argc, char** argv);


/**@brief Funcion que devuelve el tiempo trasncurrido en ms desde el ultimo registro tipo a.
 *
 * @return  Tiempo trasncurrido en ms desde el ultimo registro tipo a.
 */
uint32_t logger_get_last_a(void);


/**@brief Funcion que devuelve el tiempo trasncurrido en ms desde el ultimo registro tipo b.
 *
 * @return  Tiempo trasncurrido en ms desde el ultimo registro tipo b.
 */
uint32_t logger_get_last_b(void);
