#ifndef TABLAS_MEMORIA_H_
#define TABLAS_MEMORIA_H_

#include "./m_gestor.h"

void crear_memoria_y_dividir_en_marcos();
void crear_tabla_de_paginas (Proceso* proceso);
char *ajustar_tam_proceso(uint16_t PID, uint16_t nuevo_tam_en_bytes);
bool chequear_si_hay_marcos_libres(int paginas_a_ocupar);
void ocupar_marcos(Proceso *proceso, int paginas_a_ocupar);
void desocupar_marcos(Proceso *proceso, int paginas_a_desocupar);


#endif