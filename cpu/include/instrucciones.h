#ifndef INSTRUCCIONES_CPU_H_
#define INSTRUCCIONES_CPU_H_

#include "./c_gestor.h"
#include "./mmu.h"

void SET (void* registro, void* parametro);
void SUM(void *registroDestino, void *registroOrigen);
void SUB(void *registroDestino, void *registroOrigen);
void JNZ(void *registro, void* parametro);
void IO_GEN_SLEEP(void* parametro, void* parametro2);
void EXIT();
void WAIT(void* parametro);
void SIGNAL (void* parametro);
void RESIZE(void* parametro);
void MOV_IN (void *registroDatos, void* registroDireccion, uint8_t tamanio_de_registro_datos);
void MOV_OUT (void *registroDireccion, void* registroDatos, uint8_t tamanio_de_registro_datos);

#endif