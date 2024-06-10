#ifndef INSTRUCCIONES_CPU_H_
#define INSTRUCCIONES_CPU_H_

#include "./c_gestor.h"
#include "./mmu.h"

void SET(void *registro, void *parametro, uint8_t tam_registro);
void SUM(void *registroDestino, void *registroOrigen, uint8_t tamDestino, uint8_t tamOrigen);
void SUB(void *registroDestino, void *registroOrigen, uint8_t tamDestino, uint8_t tamOrigen);
void JNZ(void *registro, void* parametro);
void IO_GEN_SLEEP(void* parametro, void* parametro2);
void EXIT();
void WAIT(void* parametro);
void SIGNAL (void* parametro);
void RESIZE(void* parametro);
void MOV_IN (void *registroDatos, void* registroDireccion, uint8_t tamanio_de_registro_datos);
void MOV_OUT(void *registroDireccion, void *registroDatos, uint8_t tamanio_de_registro_datos);
void IO_STDIN_READ(void *interfaz, void *registroDireccion, void* registroTam, uint8_t tam_registroTam);
void IO_STDOUT_WRITE(void *interfaz, void *registroDireccion, void* registroTam, uint8_t tam_registroTam);



#endif