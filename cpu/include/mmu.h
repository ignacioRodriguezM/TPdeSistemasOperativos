#ifndef CPU_MMU_H_
#define CPU_MMU_H_

#include "./c_gestor.h"

uint32_t traducir_direccion_logica_a_fisica (void* registroDireccion);

#endif