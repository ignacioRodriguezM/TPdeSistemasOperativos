#ifndef CPU_PROCESOS_H_
#define CPU_PROCESOS_H_

#include "./c_gestor.h"


void ejecutar_ciclo_de_cpu ();



void _enviar_pid_y_pc_a_memoria ();
char* _esperar_respuesta_de_memoria ();
void _aplicar_sum_a_registro (char** comandos, void* regist);
void _aplicar_sub_a_registro (char** comandos, void* regist);
void _decode_and_excecute (char* palabra);
void _desalojar_proceso();
void _desalojar_proceso_interrumpido_por_usuario();


#endif