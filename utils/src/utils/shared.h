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
#include <commons/collections/list.h>
#include <commons/collections/queue.h>

typedef enum
{
	MENSAJE,
	PAQUETE,
    HANDSHAKE,
    RTA_HANDSHAKE,
    //------------------------------------
    INICIAR_PROCESO,
	FINALIZAR_PROCESO,
    RTA_INICIAR_PROCESO,
	ACCESO_A_TABLA,
	AMPLIACION_PROCESO,
	REDUCCION_PROCESO,
	ACCESO_A_ESPACIO_USUARIO,

    
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


int crear_conexion(char *ip, char* puerto);
int iniciar_servidor(char* puerto, t_log* un_log, char* msj_server);
int esperar_cliente(int socket_servidor, t_log* un_log,  char* nombre_cliente);
int recibir_operacion(int socket_cliente);


void inicializar_logger (t_log** logger, char* nombre);
void inicializar_logger_debug (t_log** logger, char* nombre);


t_buffer* crear_buffer ();
void destruir_buffer (t_buffer* buffer);

void cargar_choclo_al_buffer (t_buffer* buffer, void* un_choclo, int size_of_choclo);
void cargar_int_al_buffer (t_buffer* buffer, int valor);
void cargar_uint32_al_buffer (t_buffer* buffer, uint32_t valor);
void cargar_uint16_al_buffer (t_buffer* buffer, uint16_t valor);
void cargar_string_al_buffer (t_buffer* buffer, char* string);

void* extraer_choclo_al_buffer (t_buffer* un_buffer);
uint32_t extraer_uint32_al_buffer (t_buffer* un_buffer);
uint16_t extraer_uint16_al_buffer (t_buffer* un_buffer);
char* extraer_string_al_buffer (t_buffer* un_buffer);


t_paquete* crear_paquete (op_code cod_op, t_buffer* un_buffer);
void destruir_paquete (t_paquete* un_paquete);
void* serializar_paquete (t_paquete* un_paquete);
void enviar_paquete (t_paquete* paquete, int conexion);

int extraer_int_del_paquete(t_paquete* paquete);
uint32_t extraer_uint32_del_paquete(t_paquete* paquete);
uint16_t extraer_uint16_del_paquete(t_paquete* paquete);
char* extraer_string_del_paquete(t_paquete* paquete);

int recibir_size_del_buffer(int socket_cliente);
t_buffer* recibir_buffer_sin_cod_op (int socket);



#endif