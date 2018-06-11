/**
 * @defgroup LOGGER
 * @{
 *
 * @file logger.c
 * 
 * @version 1.0
 * @author  Rodrigo De Soto, Maite Gil, José Bentancour.
 * @date 12 Julio 2018
 * 
 * @brief Módulo encargado de llevar el registro.
 * 
 * @paragraph 
 * 
 * Recibe comandos desde SHELL y UI para registrar los diferentes eventos. 
 * Utiliza los datos suministrados por RTC para registrar el momento en que dan los eventos.
 */

#include <stdio.h>
#include <stdlib.h>

#include "nrf_log.h"

#include "shell.h"
#include "rtc.h"

#define MAX_REG 100

/** Estructura que representa un arreglo de datos en el registro. */
typedef struct {
	uint8_t type_insulin;
	uint32_t glucemia;
	uint8_t dosis_insulin;
	uint32_t time_stamp;
} register_t;

/** Estructura que representa la cola circular donde se guardan los datos. */
struct {
	register_t logger_array[MAX_REG];
	uint8_t head;
	uint8_t tail;
	uint8_t registers_to_send;
} logger_register;

static char date_reference[20];
static volatile uint8_t * m_logger_send_flag;
static uint8_t n_registers;
static uint8_t index;
static uint32_t last_a;
static uint32_t last_b;


/**@brief Función de Inicialización del módulo.
 *
 * @details Setea los punteros y el numero de registros a 0.
 */
void logger_init(void)
{
    logger_register.head = 0;
    logger_register.tail = 0;	
    logger_register.registers_to_send = 0;
}


/**@brief Función para ingresar un nuevo registro a la memoria.
 *
 * @param glucemia  Variable que guarda el valor de glucosa en sangre.
 * @param type      Variable que guarda el tipo de insulina inyectada.
 * @param dosis     Variable que guarda el valor de dosis inyectada.
 * @param timestamp Variable que guarda el tiempo offset en el momento de la nueva inyección.
 */ 
void logger_new_register(uint32_t glucemia, uint8_t type, uint8_t dosis, uint32_t timestamp)
{
    logger_register.logger_array[logger_register.head].type_insulin = type;
    logger_register.logger_array[logger_register.head].glucemia = glucemia;
    logger_register.logger_array[logger_register.head].dosis_insulin = dosis;
    logger_register.logger_array[logger_register.head].time_stamp = timestamp;
    logger_register.head = (logger_register.head+1)%MAX_REG;
    if (logger_register.head == logger_register.tail){
        logger_register.tail = (logger_register.tail+1)%MAX_REG;
    }else{
        logger_register.registers_to_send++;
    }
    if (type == 0)
    {
        last_a = timestamp;
    }
    else
    {
        last_b = timestamp;
    }
}


/**@brief Función que setea la flag para avisar que hay nuevos mensajes para mandar.
 */
void logger_set_flag(volatile uint8_t * m_send_flag)
{
    m_logger_send_flag = m_send_flag;
}


/**@brief Función que llama el shell cuando pide un numero x de registros.
 */
int logger_get(unsigned int argc, char** argv)
{
    if (argc == 2)
    {       
        uint8_t num_registers = strtol(*(argv+1), NULL, 10);      
        if (num_registers > logger_register.registers_to_send){
            n_registers = logger_register.registers_to_send;
        }
        else
        {
            n_registers = num_registers;
        }
        index = 0;
        *m_logger_send_flag = 1;
        return 1;
    }
    return 0;
}


/**@brief Función que llama el main para enviar por la uart los registros solicitados.
 *
 * @param p_uart    Puntero que apunta al mensaje a ser transmitido por la uart.
 * @return          Devuelve un entero que es el largo del mensaje a ser transmitido.
 */
uint8_t logger_send(uint8_t * p_uart)
{
    if (index == 0){
        sprintf((char*)p_uart, "{reg %u, ref %s, t %lu}\n", n_registers, date_reference, rtc_get());
    }else{
        char m_type;
        if (logger_register.logger_array[logger_register.tail].type_insulin == 0){
            m_type = 'A';
        }else{
            m_type = 'B';
        }
        sprintf((char*)p_uart,"{tp %c, ds %u, glu %lu, ts %lu}\n", m_type, logger_register.logger_array[logger_register.tail].dosis_insulin, logger_register.logger_array[logger_register.tail].glucemia, logger_register.logger_array[logger_register.tail].time_stamp);
        logger_register.tail = (logger_register.tail+1)%MAX_REG;
        logger_register.registers_to_send--;
    }
    index++;
    if (index == n_registers+1){
        *m_logger_send_flag = 0;
    }
    return strlen((char*)p_uart);
}	


/**@brief Función que se llama desde el shell y reseta la fecha y hora de referencia, se borran los registros.
 */
int logger_reset(unsigned int argc, char** argv)
{
    if (argc == 2)
    {
        rtc_reset();
        logger_register.head = 0;
        logger_register.tail = 0;	
        logger_register.registers_to_send = 0;
        strcpy(date_reference, *(argv+1));
        return 1;
    }
    return 0;
}


/**@brief Función que devuelve el tiempo transcurrido en ms desde el ultimo registro tipo a.
 *
 * @return  Tiempo transcurrido en ms desde el ultimo registro tipo a.
 */
uint32_t logger_get_last_a(void)
{
    return rtc_get() - last_a;
}


/**@brief Función que devuelve el tiempo transcurrido en ms desde el ultimo registro tipo b.
 *
 * @return  Tiempo transcurrido en ms desde el ultimo registro tipo b.
 */
uint32_t logger_get_last_b(void)
{
    return rtc_get() - last_b;
}
