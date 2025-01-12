#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include "./k_gestor.h"
#include "./configuracion_kernel.h"
#include "./kernel-Entrada_Salida.h"
#include "./kernel-memoria.h"
#include "./kernel-cpu_dispatch.h"
#include "./consola.h"
#include "./colas_estados.h"
#include "./semaforos.h"



//Variables GLOBALES
t_log* kernel_logger;
t_log* kernel_log_debug;

int fd_kernel;
int fd_memoria;         
int fd_entrada_salida;
int fd_cpu_dispatch;
int fd_cpu_interrupt;

uint16_t identificador_PID = 1;

pthread_mutex_t mutex_pid;
pthread_mutex_t mutex_colas;
pthread_mutex_t mutex_procesos;
pthread_mutex_t mutex_recursos;

t_queue* procesos_new;
t_queue* procesos_ready;
t_queue* procesos_ready_con_prioridad;
t_queue* procesos_excec;
t_queue* procesos_exit;
Colas_bloqueados** colas_bloqueados;
int contador_de_colas_bloqueados = 0;


int grado_multiprogramacion;
uint16_t quantum; //SI ES FIFO PONER -1
int procesos_en_programacion = 0;
bool planificacion_activa;
char* algoritmo_planificacion;


Recursos** recursos;



sem_t planificacion_activa_semaforo;
sem_t grado_multiprogramacion_semaforo;
sem_t proceso_creado_en_new_semaforo;
sem_t cpu_vacia_semaforo;
sem_t algun_ready;


pthread_t hilo_quantum;
t_temporal* timer_quantum;

int cantidad_de_recursos;

#endif