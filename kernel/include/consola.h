#ifndef CONSOLA_KERNEL_H_
#define CONSOLA_KERNEL_H_

#include "k_gestor.h"
#include "../include/servicios_kernel.h"

void iniciar_consola_interactiva ();
bool _validacion_de_instruccion_de_consola (char* leido);
void _atender_instruccion_validada (char* leido);
bool buscar_en_colas_y_eliminar_el_proceso(int pid_a_finalizar);


#endif