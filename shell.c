/**
 * @defgroup SHELL
 * @{
 *
 * @paragraph 
 * 
 * Sistemas Embebidos. Curso 2012
 *
 * Obligatorio.
 *
 * Recibe un string de comando.
 *
 * Lo parsea en palabras.
 *
 * Toma el primero como comando.
 *
 * Lo busca en una lista:
 *
 * Si no esta devuelve ErrCod: NOTFOUND
 *
 * Si esta devuelve puntero a la función a ejecutar, argc y argv con la cmdline parseada
 *
 * Además reconoce cmds internos:
 *
 * Help: devuelve lista de comandos
 *
 * Help comando conocido: devuelve string asociada
 *
 * Help comando desconocido: devuelve notfound
 *
 * Exit: devuelve ErrCod: EXIT
 *
 * Si tiene éxito devuelve ErrCod: OK
 * 
 * @file    shell.c
 * @author  Conrado Rossi.
 * @date    Marzo 2012
 *
 * @brief Módulo capaz de procesar los comandos recibidos.
 */

#include "shell.h"
#include <string.h>
#include <stdio.h>
#include "logger.h"

// Private functions
shell_command_t* cmdsrch(char* pcmd, shell_command_t* lista);

// Comandos Internos
#define EXITNAME	"EXIT"	    /**< Comando para desconectarse */
#define GETREGISTER     "GR"	/**< Comando para obtener una cantidad X de registros: GR X */
#define RESET           "RS"	/**< Comando para setear la fecha de referencia: RS DD/MM/AAAA-HH:MM:SS */


shell_command_t internalcmds[] =
{
    { EXITNAME, (ShellFunPtr)0 },
    { RESET , logger_reset},
    { GETREGISTER , logger_get},
    { "", (ShellFunPtr)0 }
};
// Debe terminar con entrada vacia


int sisem_shell(char* cmdline, ShellFunPtr* quefuncion, unsigned int* pargc, char** argv)    //Se cambio el tipo de CMDLINE para que quede acorde con como se recibe
{

    // Definir delimiters para strtok:
    #define DELIMS	" "

    char* pch;				// Puntero temporal a tokens:
    char* pcmd;				// Puntero al comando

    shell_command_t* fundescptr;	// puntero a un renglon de una lista de comandos.

    // Falta: Distinguir caso de string vacia

    // Parsear la linea de comando
    // usando:
    //char * strtok ( char * str, const char * delimiters );

    *quefuncion=NULL;
    *pargc = 0;

    pch = strtok (cmdline, DELIMS);
    pcmd=pch;				//guardar el punt al comando
    while (pch != NULL)
    {
        argv[*pargc]=pch;
        (*pargc)++;
        pch = strtok (NULL, DELIMS);
    }


    // distinguir si es exit
    if  (!strcmp(pcmd, EXITNAME))
        return(EXIT);


    // Buscar en la lista de comandos internos

    fundescptr = cmdsrch(pcmd, internalcmds);

    if ( fundescptr == (shell_command_t*)0 )
        return(NOTFOUND);
    else
    {
        *quefuncion=fundescptr->pfun;
        return(OK);
    }
}


/*------------------------------------------------------------
Buscar comando en una lista

Recibe
	puntero al nombre de comando a buscar y
	puntero a una lista (array de descriptores de funciones)

Devuelve
	puntero a la estructura
	o NULL si no la encontro
------------------------------------------------------------*/
shell_command_t* cmdsrch(char* pcmd, shell_command_t* lista)
{

    for ( ; ( strlen(lista->funname) != 0 ) ; lista++ )
    {
        if (!strcmp(lista->funname, pcmd))	//strcmp devuelve negado
            break;
    }

    // distinguir como salio del for
    if ( strlen(lista->funname) == 0 )
        return( (shell_command_t*)0 );
    else
        return( lista );
}
