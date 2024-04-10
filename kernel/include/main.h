#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include "./k_gestor.h"
#include "./configuracion_kernel.h"
#include "./kernel-Entrada_Salida.h"
#include "./kernel-memoria.h"
#include "./kernel-cpu_dispatch.h"
#include "./kernel-cpu_interrupt.h"
#include "./consola.h"
#include "./servicios_kernel.h"




//Variables GLOBALES
t_log* kernel_logger;
t_log* kernel_log_debug;

int fd_kernel;
int fd_memoria;         
int fd_entrada_salida;
int fd_cpu_dispatch;
int fd_cpu_interrupt;

int identificador_PID = 1;
int contador_pcbs = 1;

pthread_mutex_t mutex_pid;

#endif