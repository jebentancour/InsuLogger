#include <stdio.h>
#include "shell.h"
#include "estados.h"
#include "rtc.h"

enum insulina{A, B} tipo_insulina;

struct registro{
	tipo_insulina logger_tipo_insulina;
	uint32_t logger_glicemia;
	uint32_t logger_dosis_insulina;
	uint32_t logger_tiempo_evento;
}logger_registro


//cambio codigo

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
}