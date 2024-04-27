#ifndef M_GESTOR_H_
#define M_GESTOR_H_

#include <utils/shared.h>

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>


//Variables GLOBALES
extern t_log* memoria_logger;
extern t_log* memoria_log_debug;

extern int fd_memoria;         //file descriptor de memoria
extern int fd_entrada_salida;
extern int fd_cpu;
extern int fd_kernel;

extern uint16_t tam_memoria; 
extern uint16_t tam_pagina;
extern uint8_t retardo_respuesta;
extern char* path_base;

extern t_list* lista_procesos;

typedef struct{

    uint16_t PID;
    char** instrucciones;
    int cantidad_instrucciones;

}Proceso;

extern pthread_mutex_t mutex_procesos;



#endif