#ifndef MEMORIA_CPU_H_
#define MEMORIA_CPU_H_

#include "./m_gestor.h"

void enviar_tam_paginas_a_cpu();
void atender_memoria_cpu ();
char *ajustar_tam_proceso(uint16_t PID, uint16_t nuevo_tam_en_bytes);
void _ajustar_tamanio_proceso();
void _acceder_a_tabla_de_pagina();
void _solicitud_de_proxima_instruccion();
void _escribir_en_direccion_de_memoria();
void _leer_una_determinada_direccion();

#include "./tabla_de_paginas.h"


#endif