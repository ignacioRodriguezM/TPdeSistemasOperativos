#ifndef ENTRADA_SALIDA_GESTOR_H_
#define ENTRADA_SALIDA_GESTOR_H_

#include <utils/shared.h>

#include <dirent.h>
#include <commons/bitarray.h>
#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

typedef struct{
    int bloque_inicial;
    int tam_bytes;
}METADATA;

typedef struct{
    int cantidad_archivos;
    t_list* lista_archivos;
}Archivos;


//Variables GLOBALES
extern t_log* entrada_salida_logger;
extern t_log* entrada_salida_log_debug;

extern int fd_entrada_salida;
extern int fd_memoria;
extern int fd_kernel;

extern char* nombre;
extern interfaces_io tipo_interfaz;
extern uint8_t tiempo_unidad_trabajo;


extern int tamanio_de_bloque;
extern int cantidad_de_bloques;
extern int tamanio_bitarray_en_bytes;
extern char* path_base;

extern Archivos archivos;

#endif