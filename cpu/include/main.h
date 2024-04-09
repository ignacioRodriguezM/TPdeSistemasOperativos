#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include "./c_gestor.h"


#include "./configuracion_cpu.h"
#include "./cpu_kernel_dispatch.h"
#include "./cpu_kernel_interrupt.h"
#include "./cpu_memoria.h"

//Variables GLOBALES
t_log* cpu_logger;
t_log* cpu_log_debug;

int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_dispatch;
int fd_kernel_interrupt;
int fd_memoria;   
#endif