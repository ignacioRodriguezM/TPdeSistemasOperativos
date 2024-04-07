#ifndef ENTRADA_SALIDA_GESTOR_H_
#define ENTRADA_SALIDA_GESTOR_H_

#include "/home/utnso/Desktop/tp-2024-1c-SOn-mbulos/utils/src/utils/shared.h"

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

//Variables GLOBALES
extern t_log* entrada_salida_logger;
extern t_log* entrada_salida_log_debug;

extern int fd_entrada_salida;
extern int fd_memoria;
extern int fd_kernel;
#endif