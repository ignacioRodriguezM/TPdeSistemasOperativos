#ifndef UTILS_SHARED_H_
#define UTILS_SHARED_H_


#include <stdarg.h>



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
#include <commons/temporal.h>
#include <commons/memory.h>


typedef enum
{
	TAM_PAG,
	
    //------------------------KERNEL- MEMORIA
    INICIAR_PROCESO,
	FINALIZAR_PROCESO,
  
	

	//-------------------KERNEL-CPU DIS
	PROCESO_A_EJECUTAR,
	DESALOJO_POR_EXIT,
	INVALID_RESOURCE,
	INVALID_WRITE,
	LLAMADA_A_IO,
	DESALOJO,
	INTERRUPT,
	WAIT_op,
	SIGNAL_op,
	OUT_OF_MEMORY,
	RESPUESTA_RECURSO,

	//-------------------KERNEL-CPU INT
	FIN_QUANTUM,
	INTERRUPTED_BY_USER,

	//----------------IO-KERNEL
	PRESENTACION,
	FIN_DE_EJECUCION_DE_IO,
	TAREA,

    

	//---------------- CPU-MEMORIA
	SOLICITUD_DE_PROXIMA_INSTRUCCION,
	PROXIMA_INSTRUCCION,
	AJUSTAR_TAMANIO_PROCESO,
	RESPUESTA_RESIZE,
	ACCESO_A_TABLA_DE_PAG,
	LECTURA,
	ESCRITURA

}op_code;

typedef enum
{
	
	GEN,
	STDIN,
	STDOUT,
	DIALFS
    
}interfaces_io;

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


int division_entera_redondear_arriba(int numerador, int denominador);



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
void cargar_uint8_al_buffer (t_buffer* buffer, uint8_t valor);
void cargar_int8_al_buffer (t_buffer* buffer, int8_t valor);
void cargar_string_al_buffer (t_buffer* buffer, char* string);

void* extraer_choclo_al_buffer (t_buffer* un_buffer);
uint32_t extraer_uint32_al_buffer (t_buffer* un_buffer);
uint16_t extraer_uint16_al_buffer (t_buffer* un_buffer);
uint8_t extraer_uint8_al_buffer (t_buffer* un_buffer);
int8_t extraer_int8_al_buffer (t_buffer* un_buffer);
char* extraer_string_al_buffer (t_buffer* un_buffer);


t_paquete* crear_paquete (op_code cod_op, t_buffer* un_buffer);
void destruir_paquete (t_paquete* un_paquete);
void* serializar_paquete (t_paquete* un_paquete);
void enviar_paquete (t_paquete* paquete, int conexion);


int recibir_size_del_buffer(int socket_cliente);
t_buffer* recibir_buffer_sin_cod_op (int socket);

void esperarMilisegundos(uint16_t milisegundos);


#endif