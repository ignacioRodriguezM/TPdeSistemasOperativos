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

#endif