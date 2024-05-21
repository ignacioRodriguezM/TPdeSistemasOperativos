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
extern uint16_t retardo_respuesta;
extern char* path_base;

extern t_list* lista_procesos;


typedef struct{
    int pagina;
    int marco;
}Tabla_paginas;

typedef struct{

    uint16_t PID;
    char** instrucciones;
    int cantidad_instrucciones;
    Tabla_paginas** tabla_de_paginas;
    int cantidad_paginas;

}Proceso;


typedef struct{

    void* inicio_marco;
    bool esta_libre;

}Marco;


extern pthread_mutex_t mutex_procesos;
extern pthread_mutex_t mutex_tabla_paginas;

extern void* memoria_usuario;
extern int cantidad_de_marcos;
extern Marco** marcos_memoria;

#endif