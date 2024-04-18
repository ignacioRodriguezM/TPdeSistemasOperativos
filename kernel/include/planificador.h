#ifndef PLANIFICADOR_KERNEL_H_
#define PLANIFICADOR_KERNEL_H_

#include "k_gestor.h"

void iniciar_planificador_de_corto_plazo ();
void _planificar_por_fifo ();
void _planificar_por_rr ();
void _planificar_por_vrr ();


#endif