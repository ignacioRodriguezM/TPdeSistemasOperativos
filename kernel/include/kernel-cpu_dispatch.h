#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include "k_gestor.h"

void atender_kernel_cpu_dispatch();
bool _chequear_la_io(char *nombre_interfaz, char *operacion);
void _manejar_desalojo_por_quantum ();
void _manejar_exit();
void _manejar_bloqueo ();



#endif