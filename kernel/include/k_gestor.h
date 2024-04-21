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
extern pthread_mutex_t mutex_colas;
extern pthread_mutex_t mutex_procesos;


typedef struct{
    uint32_t pc;
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
}Registros;


typedef struct{
    uint16_t pid;
    uint16_t pc;
    uint8_t quantum;
    Registros registros;
}PCB;
typedef struct{
    char* nombre;
    interfaces_io tipo_interfaz;
    t_queue* cola;
    int fd;
    bool conectado;
}Colas_bloqueados;



extern t_queue* procesos_new;
extern t_queue* procesos_ready;
extern t_queue* procesos_excec;
extern t_queue* procesos_exit;
extern Colas_bloqueados** colas_bloqueados;
extern int contador_de_colas_bloqueados;


extern int grado_multiprogramacion;
extern int procesos_en_programacion;
extern bool planificacion_activa;
extern bool proceso_ejecutando;
extern char* algoritmo_planificacion;


#endif