#ifndef M_GESTOR_H_
#define M_GESTOR_H_

#include <utils/shared.h>

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>


//Variables GLOBALES
extern t_log* memoria_logger;
extern t_log* memoria_log_debug;

extern int fd_memoria;         //file descriptor de memoria
extern int fd_entrada_salida;
extern int fd_cpu;
extern int fd_kernel;

extern void* espacio_memoria_usuario;
extern int tam_pagina;
extern int retardo_respuesta;
extern int cantidad_de_paginas;
extern void** tabla_de_paginas;

#endif