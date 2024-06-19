#ifndef ENTRADA_SALIDA_MAIN_H_
#define ENTRADA_SALIDA_MAIN_H_

#include "./es_gestor.h"

#include "./configuracion_entrada_salida.h"
#include "./entrada_salida_kernel.h"
#include "./entrada_salida_memoria.h"
#include "./filesystem.h"

//Variables GLOBALES
t_log* entrada_salida_logger;
t_log* entrada_salida_log_debug;

int fd_entrada_salida;
int fd_memoria;
int fd_kernel;

char* nombre;
interfaces_io tipo_interfaz;
uint8_t tiempo_unidad_trabajo;

// FS
int tamanio_de_bloque;
int cantidad_de_bloques;
int tamanio_bitarray_en_bytes;
char* path_base;

Archivos archivos;

#endif