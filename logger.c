#include <stdio.h>
#include "shell.h"
#include "rtc.h"
#define MAX_registers 100

typedef struct type_register{
	uint8_t type_insulin;
	uint32_t glicemia;
	uint8_t dosis_insulin;
	uint32_t time_stamp;
}register_type;

struct struct_register{
	register_type logger_array[MAX_registers];
	uint8_t head;
	uint8_t tail;
	uint8_t registers_to_send;
}logger_register;

static char* date_reference;

static volatile uint8_t * m_logger_send_flag;
uint8_t n_registers;
char m_type;
static uint8_t index;
char * p_header_to_send;
char * p_to_send;
static uint32_t last_a;
static uint32_t last_b;

void logger_init(void)
{
	logger_register.head = 0;
	logger_register.tail = 0;	
	logger_register.registers_to_send = 0;
    date_reference = "DD/MM/AAAA-HH:MM:SS";
}

void logger_new_register(uint32_t glicemia, uint8_t type, uint8_t dosis, uint32_t timestamp)
{
	logger_register.logger_array[logger_register.head].type_insulin = type;
	logger_register.logger_array[logger_register.head].glicemia = glicemia;
	logger_register.logger_array[logger_register.head].dosis_insulin = dosis;
	logger_register.logger_array[logger_register.head].time_stamp = timestamp;
	logger_register.head = (logger_register.head+1)%MAX_registers;
	if (logger_register.head == logger_register.tail){
		logger_register.tail = (logger_register.tail+1)%MAX_registers;
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
    uint8_t num_registers = 10;
	if (num_registers > logger_register.registers_to_send){
		n_registers=logger_register.registers_to_send;
	}
	else{
		n_registers=num_registers;
	}
	
	*m_logger_send_flag=1;
    return 1;
}

uint8_t logger_send(uint8_t * p_uart)
{
	if (index == 0){
		sprintf(p_header_to_send, "{\n \"Registros a mandar\":%d,\n \" Fecha de referencia\": %s,\n }", n_registers, date_reference);
		* p_uart = * p_header_to_send;
	}else{
		if (logger_register.logger_array[logger_register.tail].type_insulin == 0){
			m_type = 'A';
		}else{
			m_type = 'B';
		}
		sprintf(p_to_send,"{\n \" Tipo de insulina\":%c,\n \"Dosis de insulina\":%d,\n \"Glicemia [ml]\":%d,\n \" Tiempo offset\":%d,\n }", m_type, logger_register.logger_array[logger_register.tail].dosis_insulin, logger_register.logger_array[logger_register.head].glicemia, logger_register.logger_array[logger_register.head].time_stamp);
	
		logger_register.tail = (logger_register.tail+1)%MAX_registers;
		* p_uart = * p_to_send;
	}
	index++;
	if (index == n_registers+1){
		*m_logger_send_flag=0;
	}
	return (sizeof(*p_uart));
}	
		
int logger_reset(unsigned int argc, char** argv)
{
    // Pasar de formato CHAR** al correspondiente (uint32_t)
	logger_init();
    if (argc == 1)
    {
        date_reference = *argv;
        return 1;
    }
    else
        return 0;
}

uint32_t logger_get_last_a(void)
{
    return last_a;
}

uint32_t logger_get_last_b(void)
{
    return last_b;
}

/*
//CONVERSION ESTRUCTURA_TIEMPO/CONTADOR
struct estructura_tiempo{
	uint32_t tiempo_horas;
	uint32_t tiempo_minutos;
	uint32_t tiempo_segundos;
}estructura_tiempo;

estructura_tiempo contador_ms_a_estructura_tiempo(uint32_t contador_ms)
{
	estructura_tiempo.tiempo_horas=contador_ms/3600000;
	estructura_tiempo.tiempo_minutos=contador_ms/60000;
	estructura_tiempo.tiempo_segundos=contador_ms/1000;
	return estructura_tiempo;
}

uint32_t estructura_tiempo_a_contador_ms(estructura_tiempo)
{
	return ((estructura_tiempo.tiempo_horas * 3600000) + (estructura_tiempo.tiempo_minutos * 60000) + (estructura_tiempo.tiempo_segundos * 1000));
}*/