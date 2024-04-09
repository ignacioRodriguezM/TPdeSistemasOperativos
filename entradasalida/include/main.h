#ifndef ENTRADA_SALIDA_MAIN_H_
#define ENTRADA_SALIDA_MAIN_H_

#include "./es_gestor.h"

#include "./configuracion_entrada_salida.h"

//Variables GLOBALES
t_log* entrada_salida_logger;
t_log* entrada_salida_log_debug;

int fd_entrada_salida;
int fd_memoria;
int fd_kernel;
#endif