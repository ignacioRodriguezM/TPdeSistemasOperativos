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

uint16_t identificador_PID = 1;
uint16_t contador_pcbs = 1;

pthread_mutex_t mutex_pid;

typedef struct{
    uint8_t ax;
    uint8_t bx;
    uint8_t cx;
    uint8_t dx;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t si;
    uint32_t di;
}registros;


typedef struct{
    uint32_t pid;
    uint32_t* pc;
    uint32_t quantum;
    registros registro;
}PCB;

#endif