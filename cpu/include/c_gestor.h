#ifndef C_GESTOR_H_
#define C_GESTOR_H_

#include "/home/utnso/Desktop/tp-2024-1c-SOn-mbulos/utils/src/utils/shared.h"

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#include "./configuracion_cpu.h"

//Variables GLOBALES
extern t_log* kernel_logger;
extern t_log* kernel_log_debug;

extern int fd_cpu;
extern int fd_kernel;
extern int fd_memoria;         
#endif