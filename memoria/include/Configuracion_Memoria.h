#ifndef ConfiguracionMemoria_H_
#define ConfiguracionMemoria_H_
#include <commons/collections/list.h>
  typedef struct
{
    uint16_t puerto_escucha;            //Puerto al cual se escuchara la conexion de modulo
    uint16_t tam_memoria;               //Tamaño expresado en bytes del espacio de usuario de la memoria.
    uint16_t tam_pagina;               //Tamaño de las paginas en bytes
    uint16_t retardo_respuesta;         //Tiempo en milisegundos que se deberá esperar antes de responder a las solicitudes de CPU.
    char* path_instrucciones;           //Carpeta donde se encuentran los archivos de pseudocódigo.

}APP_config;

void cargar_configuracion (APP_config);
    

#endif