#ifndef INSTRUCCIONES_CPU_H_
#define INSTRUCCIONES_CPU_H_

#include "./c_gestor.h"

void SET (void* registro, int valor);
void SUM(void *registroDestino, void *registroOrigen);
void SUB(void *registroDestino, void *registroOrigen);
void JNZ(void* registro, unsigned int valor_salto);
void IO_GEN_SLEEP (char* nombre_interfaz, uint8_t unidades_de_trabajo);
void EXIT();
void WAIT(char *nombre_recurso);
void SIGNAL (char *nombre_recurso);


#endif