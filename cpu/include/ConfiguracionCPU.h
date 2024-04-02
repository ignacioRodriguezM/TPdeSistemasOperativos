#ifndef ConfiguracionCPU_H_
#define ConfiguracionCPU_H_
#include <commons/collections/list.h>
  typedef struct
{
    char* ip_memoria;                   //IP a la cual se deberá conectar con la Memoria    
    uint16_t puerto_memoria;            //Puerto al cual se deberá conectar con la Memoria
    uint16_t puerto_escucha_dispach;    //Puerto en el cual se escuchará la conexión del Kernel para mensajes de dispatch
    uint16_t puerto_escucha_interrupt;  //Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    uint16_t cantidad_entradas_TLB;     //Cantidad de entradas que tendrá la TLB
    char* algoritmo_TLB;                //algoritmo de reemplazo de la TLB (FIFO / LRU)

}APP_config;

void cargar_configuracion (void);
    

#endif