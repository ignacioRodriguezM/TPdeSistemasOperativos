#ifndef TABLAS_MEMORIA_H_
#define TABLAS_MEMORIA_H_

#include "./m_gestor.h"

uint16_t marco_correspondiente_a_pagina_consultada(uint16_t pid_recibido, uint16_t pagina_a_consultar);
void crear_memoria_y_dividir_en_marcos();
void crear_tabla_de_paginas (Proceso* proceso);
char *ajustar_tam_proceso(uint16_t PID, uint16_t nuevo_tam_en_bytes);
bool chequear_si_hay_marcos_libres(int paginas_a_ocupar);
void ocupar_marcos(Proceso *proceso, int paginas_a_ocupar);
void desocupar_marcos(Proceso *proceso, int paginas_a_desocupar);
void liberar_paginas_de_proceso (Proceso* proceso);


#endif