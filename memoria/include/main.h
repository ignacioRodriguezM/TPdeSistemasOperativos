#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include "./m_gestor.h"
#include "./configuracion_memoria.h"
#include "./memoria_cpu.h"
#include "./memoria_kernel.h"
#include "./memoria_entrada_salida.h"


//Variables GLOBALES
t_log* memoria_logger; //log obligatorio
t_log* memoria_log_debug; //log debug

int fd_memoria;         //file descriptor de memoria
int fd_entrada_salida;
int fd_cpu;
int fd_kernel;

#endif