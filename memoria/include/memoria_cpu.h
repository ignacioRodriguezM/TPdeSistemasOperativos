#ifndef MEMORIA_CPU_H_
#define MEMORIA_CPU_H_

#include "./m_gestor.h"

void atender_memoria_cpu ();
char *ajustar_tam_proceso(uint16_t PID, uint16_t nuevo_tam_en_bytes);

#endif