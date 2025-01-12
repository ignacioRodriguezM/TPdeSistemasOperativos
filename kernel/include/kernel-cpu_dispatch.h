#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include "k_gestor.h"

void atender_kernel_cpu_dispatch();
bool _chequear_la_io(char *nombre_interfaz, char *operacion);
void _manejar_desalojo ();
void _manejar_exit();
void _manejar_bloqueo ();
void _manejar_wait();
void _manejar_signal();
void _manejar_out_of_memory();
void _manejar_interrupcion_de_usuario();
void mandar_a_io_o_cola_bloqueados(char * nombre_interfaz);


#endif