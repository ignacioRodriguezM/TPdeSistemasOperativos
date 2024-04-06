#ifndef UTILS_SHARED_H_
#define UTILS_SHARED_H_

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <commons/log.h>
#include <string.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>
#include <sys/types.h>

typedef enum
{
	MENSAJE,
	PAQUETE,
    HANDSHAKE,
    RTA_HANDSHAKE,
    //------------------------------------
    CREAR_PROCESO,
    RTA_CREAR_PROCESO
    
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


//int crear_conexion(char *ip, char* puerto);
//int iniciar_servidor(char* puerto, t_log* un_log, char* msj_server);
//int esperar_cliente(int socket_servidor, t_log* un_log);
//int recibir_operacion(int socket_cliente);
void inicializar_logger (t_log** logger);
void inicializar_logger_debug (t_log** logger);

#endif