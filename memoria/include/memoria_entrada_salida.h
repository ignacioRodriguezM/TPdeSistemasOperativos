#ifndef MEMORIA_ENTRADA_SALIDA_H_
#define MEMORIA_ENTRADA_SALIDA_H_

#include "./m_gestor.h"

void atender_memoria_entrada_salida ();
void atender_multiples_entrada_salida(int *socket_ptr);
void manejar_read(int fd_io);
void manejar_write(int fd_io);

#endif