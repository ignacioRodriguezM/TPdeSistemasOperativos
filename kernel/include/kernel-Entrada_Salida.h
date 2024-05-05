#ifndef KERNEL_ENTRADA_SALIDA_H_
#define KERNEL_ENTRADA_SALIDA_H_

#include "k_gestor.h"

void atender_kernel_entrada_salida ();

void atender_multiples_entrada_salida(int *socket_ptr);
void _mover_de_cola_bloqueados_a_ready_o_aux(char* nombre_de_io, uint16_t pid);

#endif