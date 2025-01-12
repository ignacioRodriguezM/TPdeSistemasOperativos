#ifndef CONFIGURACION_KERNEL_H_
#define CONFIGURACION_KERNEL_H_
#include <commons/collections/list.h>
  typedef struct
{
    char* puerto_escucha;            //Puerto en el cual se escucharán las conexiones a este módulo.
    char* ip_memoria;                   //IP a la cual se deberá conectar con la Memoria    
    char* puerto_memoria;            //Puerto al cual se deberá conectar con la Memoria
    char* ip_cpu;                       //IP a la cual se deberá conectar con la CPU
    char* puerto_cpu_dispatch;       //Puerto de dispatch al cual se deberá conectar con la CPU
    char* puerto_cpu_interrupt;      //Puerto de interrupt al cual se deberá conectar con la CPU
    char* algoritmo_planificacion;      //Define el algoritmo de planificación de corto plazo. (FIFO / RR / VRR)
    uint16_t quantum;                   //Tiempo en milisegundos del quantum para utilizar bajo el algoritmo RR
    char** recursos;                    //Lista ordenada de los nombres de los recursos compartidos del sistema
    char** instancias_recursos;         //Lista ordenada de la cantidad de unidades por recurso
    uint16_t grado_multiprogramacion;   //Grado de multiprogramación del módulo
    t_config* config;

}APP_config;

APP_config cargar_configuracion_kernel (void);
    
int* convertirArrayCharAUInt16(char** array_char);

#endif