#ifndef K_GESTOR_H_
#define K_GESTOR_H_

#include "/home/utnso/Desktop/tp-2024-1c-SOn-mbulos/utils/src/utils/shared.h"

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#include "./configuracion_kernel.h"

//Variables GLOBALES
extern t_log* kernel_logger;
extern t_log* kernel_log_debug;

extern int fd_kernel;
extern int fd_memoria;         
extern int fd_entrada_salida;
extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;
#endif