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
#include "./colas_estados.h"
#include "./planificador.h"


//Variables GLOBALES
t_log* kernel_logger;
t_log* kernel_log_debug;

int fd_kernel;
int fd_memoria;         
int fd_entrada_salida;
int fd_cpu_dispatch;
int fd_cpu_interrupt;

uint16_t identificador_PID = 1;
uint16_t contador_pcbs = 1;

pthread_mutex_t mutex_pid;

t_queue* procesos_new;
t_queue* procesos_ready;
t_queue* procesos_excec;
t_queue* procesos_exit;
Colas_bloqueados** colas_bloqueados;
int contador_de_colas_bloqueados;


int grado_multiprogramacion;
int procesos_en_programacion;
bool planificacion_activa;
char* algoritmo_planificacion;


#endif