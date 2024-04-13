#ifndef K_GESTOR_H_
#define K_GESTOR_H_

#include <utils/shared.h>

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>


//Variables GLOBALES
extern t_log* kernel_logger;
extern t_log* kernel_log_debug;

extern int fd_kernel;
extern int fd_memoria;         
extern int fd_entrada_salida;
extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;

extern uint16_t identificador_PID;
extern uint16_t contador_pcbs;

extern pthread_mutex_t mutex_pid;

/*
extern struct REGISTROS{
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
};

extern struct PCB{
    uint32_t pid;
    uint32_t* pc;
    uint32_t quantum;
    REGISTROS registros;
}; */

#endif