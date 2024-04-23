#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "k_gestor.h"

void crear_proceso (char* path);
void mover_proceso_a_exit ();
void iniciar_planificador_de_largo_plazo ();
void iniciar_planificador_de_corto_plazo ();


#endif