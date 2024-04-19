#ifndef CONFIGURACION_ENTRADA_SALIDA_H_
#define CONFIGURACION_ENTRADA_SALIDA_H_
#include "./es_gestor.h"
  typedef struct
{
    char* nombre_identificador;
    interfaces_io tipo_interfaz;                   //Indica el tipo de Interfaz de I/O que estamos creando. GENERICA / STDIN / STDOUT / DIALFS   
    uint8_t tiempo_unidad_trabajo;        //Tiempo en milisegundos que dura cada unidad de trabajo
    char* ip_kernel;                    //IP a la cual se deberá conectar con el Kernel
    char* puerto_kernel;                   //Puerto al cual se deberá conectar con el Kernel
    char* ip_memoria;                      //IP a la cual se deberá conectar con la Memoria
    char* puerto_memoria;               //Puerto al cual se deberá conectar con la Memoria
    char* path_base_dialfs;                //Path a partir del cual van a encontrarse los archivos de DialFS.
    uint16_t block_size;                   //Tamaño de los bloques del FS
    uint16_t block_count;                  //Cantidad de bloques del FS

}APP_config;

APP_config cargar_configuracion_entrada_salida(void);


#endif