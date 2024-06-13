#ifndef K_GESTOR_H_
#define K_GESTOR_H_

#include <utils/shared.h>

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>
#include <semaphore.h>

#include "./servicios_kernel.h"

typedef struct
{
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
} Registros;
typedef struct
{
    uint16_t pid;
    uint32_t pc;
    uint16_t quantum;
    Registros registros;
    char** recursos_asignados;
    int cantidad_recursos_asignados;
    t_buffer *operacion_de_io_por_la_que_fue_bloqueado;
} PCB;

#include "./planificador.h"


typedef struct
{
    char *nombre;
    interfaces_io tipo_interfaz;
    t_queue *cola;
    int fd;
    bool conectado;
} Colas_bloqueados;

typedef struct
{
    char *nombre;
    int instancias;
    t_queue* cola_bloqueados_por_recursos;

} Recursos;


// Variables GLOBALES
extern t_log *kernel_logger;
extern t_log *kernel_log_debug;

extern int fd_kernel;
extern int fd_memoria;
extern int fd_entrada_salida;
extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;


extern uint16_t identificador_PID;


extern pthread_mutex_t mutex_pid;
extern pthread_mutex_t mutex_colas;
extern pthread_mutex_t mutex_procesos;
extern pthread_mutex_t mutex_recursos;

extern t_queue *procesos_new;
extern t_queue *procesos_ready;
extern t_queue *procesos_ready_con_prioridad;
extern t_queue *procesos_excec;
extern t_queue *procesos_exit;
extern Colas_bloqueados **colas_bloqueados;
extern int contador_de_colas_bloqueados;

extern int grado_multiprogramacion;
extern int procesos_en_programacion;
extern uint16_t quantum;
extern bool planificacion_activa;
extern char *algoritmo_planificacion;

extern Recursos** recursos;


extern sem_t planificacion_activa_semaforo;
extern sem_t grado_multiprogramacion_semaforo;
extern sem_t proceso_creado_en_new_semaforo;
extern sem_t cpu_vacia_semaforo;
extern sem_t algun_ready;


extern pthread_t hilo_quantum;
extern t_temporal* timer_quantum;

extern int cantidad_de_recursos;

#endif