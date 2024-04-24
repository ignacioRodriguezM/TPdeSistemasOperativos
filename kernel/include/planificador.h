#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "k_gestor.h"

void crear_proceso (char* path);
void iniciar_planificador_de_largo_plazo ();
void iniciar_planificador_de_corto_plazo ();
void mover_de_excec_a_cola_bloqueado(char *nombre_de_la_io);


#endif