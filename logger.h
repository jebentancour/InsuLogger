//ifndef
//define LOGGER_H
/*
@brief funcion que inicializa al modulo logger, setea los punteros y el numero de registros a 0
*/
void logger_init(void);

/*
 *@brief funcion para ingresar un nuevo registro a la memoria
 *@param glicemia: variable que guarda el valor de glucosa en sangre
 *@param type: variable que guarda el tipo de insulina inyectada
 *@param dosis: variable que guarda el valor de dosis inyectada
 *@param timestamp: variable que guarda el tiempo offset en el momento de la nueva inyeccion
*/ 
void logger_new_register(uint32_t glicemia, uint8_t type, uint8_t dosis, uint32_t timestamp);

/*
 *@brief funcion que setea la flag para avisar que hay nuevos mensajes para mandar 
*/
void logger_set_flag(volatile uint8_t * m_send_flag);

/*
 *@brief funcion que llama el shell cuando pide un numero 'x' de registros
 *@param registros solicitados para ser enviados
*/
int logger_get(unsigned int argc, char** argv);

/*
 *@brief funcion que llama el main para enviar por la uart los registros solicitados
 *@param * p_uart puntero que apunta al mensaje a ser transmitido por la uart
 *@return uint8_t devuelve un entero que es el largo del mensaje a ser transmitido
*/
uint8_t logger_send(uint8_t * p_uart);

/*
 *@brief funcion que se llama desde el shell y reseta la fecha y hora de referencia, se borran los registros
 *@param date_ref nueva fecha de referencia para los proximos registros
 *@param time_ref nueva hora de referencia para los proximos registros
*/
int logger_reset(unsigned int argc, char** argv);

uint32_t logger_get_last_a(void);

uint32_t logger_get_last_b(void);

//endif