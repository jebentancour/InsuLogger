#include <stdio.h>
#include <stdlib.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "shell.h"
#include "rtc.h"

#define MAX_REG 100

typedef struct {
	uint8_t type_insulin;
	uint32_t glicemia;
	uint8_t dosis_insulin;
	uint32_t time_stamp;
}register_t;

struct {
	register_t logger_array[MAX_REG];
	uint8_t head;
	uint8_t tail;
	uint8_t registers_to_send;
}logger_register;

static char* date_reference;
static volatile uint8_t * m_logger_send_flag;
static uint8_t n_registers;
static uint8_t index;
static uint32_t last_a;
static uint32_t last_b;

void logger_init(void)
{
    logger_register.head = 0;
    logger_register.tail = 0;	
    logger_register.registers_to_send = 0;
    date_reference = "DD/MM/AAAA-HH:MM:SS";
    NRF_LOG_DEBUG("logger_init\r\n");
}

void logger_new_register(uint32_t glicemia, uint8_t type, uint8_t dosis, uint32_t timestamp)
{
    NRF_LOG_DEBUG("logger_new_register glicemia %d, type %d, dosis %d, timestamp %d\r\n", glicemia, type, dosis, timestamp);
    logger_register.logger_array[logger_register.head].type_insulin = type;
    logger_register.logger_array[logger_register.head].glicemia = glicemia;
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

void logger_set_flag(volatile uint8_t * m_send_flag)
{
    m_logger_send_flag = m_send_flag;
}

int logger_get(unsigned int argc, char** argv)
{
    if (argc == 2)
    {  
        NRF_LOG_DEBUG("logger_get %s\r\n", (uint32_t) *(argv+1));
        NRF_LOG_FLUSH();
      
        uint8_t num_registers = strtol(*(argv+1), NULL, 10);
        
        NRF_LOG_DEBUG("strtol %d\r\n", num_registers);
        NRF_LOG_FLUSH();
        
        if (num_registers > logger_register.registers_to_send){
            n_registers = logger_register.registers_to_send;
        }
        else
        {
            n_registers = num_registers;
        }
        
        index = 0;
        
        NRF_LOG_DEBUG("logger_get n_registers %d\r\n", n_registers);
        NRF_LOG_FLUSH();
    
        *m_logger_send_flag = 1;
        return 1;
    }
    return 0;
}

uint8_t logger_send(uint8_t * p_uart)
{
    NRF_LOG_DEBUG("logger_send index %d\r\n",index);
    NRF_LOG_FLUSH();
    
    if (index == 0){
        sprintf((char*)p_uart, "{registros %u, referencia %s}\n", n_registers, date_reference);
        NRF_LOG_DEBUG("p_uart %s\r\n", (uint32_t)p_uart);
        NRF_LOG_FLUSH();
    }else{
        char m_type;
        if (logger_register.logger_array[logger_register.tail].type_insulin == 0){
            m_type = 'A';
        }else{
            m_type = 'B';
        }
        sprintf((char*)p_uart,"{tipo %c, dosis  %u, glicemia %lu, timestamp %lu}\n", m_type, logger_register.logger_array[logger_register.tail].dosis_insulin, logger_register.logger_array[logger_register.tail].glicemia, logger_register.logger_array[logger_register.tail].time_stamp);
        logger_register.tail = (logger_register.tail+1)%MAX_REG;
        logger_register.registers_to_send--;
        NRF_LOG_DEBUG("p_uart %s\r\n", (uint32_t)p_uart);
        NRF_LOG_FLUSH();
    }
    index++;
    if (index == n_registers+1){
        *m_logger_send_flag = 0;
    }
    return strlen((char*)p_uart);
}	
		
int logger_reset(unsigned int argc, char** argv)
{
    if (argc == 2)
    {
        NRF_LOG_DEBUG("logger_reset %d %s\r\n", argc, (uint32_t) *(argv+1));
        logger_init();
        date_reference = *(argv+1);
        return 1;
    }
    return 0;
}

uint32_t logger_get_last_a(void)
{
    return rtc_get() - last_a;
}

uint32_t logger_get_last_b(void)
{
    return rtc_get() - last_b;
}