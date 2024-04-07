#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include "./k_gestor.h"


#include "./configuracion_kernel.h"

//Variables GLOBALES
t_log* kernel_logger;
t_log* kernel_log_debug;

int fd_kernel;
int fd_memoria;         
int fd_enrada_salida;
int fd_cpu;
#endif