#ifndef CPU_TLB_H_
#define CPU_TLB_H_

#include "./c_gestor.h"

void inicializar_tlb();
void finalizar_tlb();
uint16_t consultar_tlb (uint16_t pid, uint16_t pagina);
void actualizar_tlb (uint16_t pid, uint16_t pagina, uint16_t marco);

#endif