#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "k_gestor.h"

void crear_proceso (char* path);
void iniciar_planificador_de_largo_plazo ();
void iniciar_planificador_de_corto_plazo ();
void mover_de_excec_a_cola_bloqueado(char *nombre_de_la_io);
void mover_a_io_si_hay_algun_proceso_encolado(char *nombre_io); //verificar si hay algun proceso en su cola de bloqueados, si hay, lo manda a 
void mover_de_excec_a_ready ();
void _mandar_de_excec_a_exit(char* motivo);
void avisarle_a_memoria_que_libere_recursos_de_proceso(uint16_t pid);
void bloquear_proceso_en_ejecucion_por_recurso (int index_cola);
void desbloquear_proceso_bloqueado_por_recurso (int index_cola);
void _enviar_interrupcion_quantum();
void manejar_quantum(PCB* proceso);
void liberar_recursos_asignados(PCB *pcb);
void borrar_pcbs_en_exit();



#endif