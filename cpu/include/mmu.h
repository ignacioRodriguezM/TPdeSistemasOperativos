#ifndef CPU_MMU_H_
#define CPU_MMU_H_

#include "./c_gestor.h"
#include "./tlb.h"

Direcciones traducir_direccion_logica_a_fisicas(void *registroDireccion, uint8_t espacio);


#endif