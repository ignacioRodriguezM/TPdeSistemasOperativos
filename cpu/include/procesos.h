#ifndef CPU_PROCESOS_H_
#define CPU_PROCESOS_H_

#include "./c_gestor.h"


void ejecutar_ciclo_de_cpu ();



void _enviar_pid_y_pc_a_memoria ();
char* _esperar_respuesta_de_memoria ();

void _desalojar_proceso();
void _desalojar_proceso_interrumpido_por_usuario();

void decodear(char *palabra);

#endif