/**
 * \defgroup sisem_shell
 * @{
 * Modulo shell
 */

/*
 * Sistemas Embebidos. Curso 2012
 * Obligatorio.
 * Modulo shell

Recibe un string de comando.
Lo parsea en palabras.
Toma el primero como comado.
Lo busca en una lista
	Si no esta devuelve ErrCod: NOTFOUND
	Si esta devuelve puntero a la funcion a ejecutar, argc y argv con la cmdline parseada
Ademas reconoce cmds internos:
	Help: devuelve lista de comandos
	Help comando conocido: devuelve string asociada
	Help comando desconocido: devuelve notfound
	Exit: devuelve ErrCod: EXIT
Si tiene exito devuelve ErrCod: OK

 */

/**
 * \file
 *         modulo shell
 * \author
 *         Conrado Rossi, marzo 2012
 */

#ifndef SHELL_H_INCLUDED
#define SHELL_H_INCLUDED

#include <stdint.h>

#define LENNAME		32		//Largo max del nombre de funcion
#define LENDESC		256		//Largo max de la descripcion


// Defino tipo para funciones a llamar con interface std
typedef  int (*ShellFunPtr)(unsigned int argc, char** argv);

// Defino tipo para cada entrada a la tabla de funciones
typedef struct fundesc
{
    char funname[LENNAME];			//nombre del comando
    ShellFunPtr pfun;				//puntero a la funcion
} shell_command_t;

// Tabla de comandos en shellcmds.h
// extern shell_command_t commands[];


int sisem_shell(char* cmdline, ShellFunPtr* quefuncion, unsigned int* pargc, char** argv);
// Recibe:
//	cmdline : 	string con una linea de comando
// Devuelve:
//	quefuncion : 	puntero a la funcion a ejecutar
//	pargc:			puntero a cantidad de argumentos+1
//	argv:			puntero a array de punteros a los args
// Retorna:
//	un codigo de error segun las defs:

#define OK		0
#define EXIT		1
#define NOTFOUND	2
#define ERROR		99


#endif // SHELL_H_INCLUDED
