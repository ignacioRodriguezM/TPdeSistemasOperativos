#ifndef C_GESTOR_H_
#define C_GESTOR_H_

#include <utils/shared.h>

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#include "./configuracion_cpu.h"

//Variables GLOBALES
extern t_log* cpu_logger;
extern t_log* cpu_log_debug;

extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;
extern int fd_kernel_dispatch;
extern int fd_kernel_interrupt;
extern int fd_memoria;         
#endif