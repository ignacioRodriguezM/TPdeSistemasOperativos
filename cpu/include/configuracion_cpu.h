#ifndef CONFIGURACION_CPU_H_
#define CONFIGURACION_CPU_H_
#include <commons/collections/list.h>
  typedef struct
{
    char* ip_memoria;                   //IP a la cual se deberá conectar con la Memoria    
    char* puerto_memoria;            //Puerto al cual se deberá conectar con la Memoria
    char* puerto_escucha_dispach;    //Puerto en el cual se escuchará la conexión del Kernel para mensajes de dispatch
    char* puerto_escucha_interrupt;  //Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    uint16_t cantidad_entradas_TLB;     //Cantidad de entradas que tendrá la TLB
    char* algoritmo_TLB;                //algoritmo de reemplazo de la TLB (FIFO / LRU)

}APP_config;

APP_config cargar_configuracion_cpu (void);
    

#endif