#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include "./c_gestor.h"


#include "./configuracion_cpu.h"

//Variables GLOBALES
t_log* cpu_logger;
t_log* cpu_log_debug;

int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_dispatch;
int fd_kernel_interrupt;
int fd_memoria;   
#endif