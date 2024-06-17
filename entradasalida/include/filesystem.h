#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "./es_gestor.h"
#include "./configuracion_entrada_salida.h"

void _crear_archivo_que_seria_el_disco();
void _crear_archivo_que_contiene_el_bitmap(int cantidad_de_bloques);
void crear_metadata(const char *nombre_archivo, int bloque_inicial, int tamanio_archivo);
void crear_archivo(char *nombre, int tamanio_del_archivo);

unsigned char* leer_bitmap(int cantidad_de_bloques);
int buscar_secuencia_libre(unsigned char *bitmap, int cantidad_de_bloques, int longitud);
void marcar_bloques_ocupados(unsigned char *bitmap, int inicio, int longitud);
int esta_libre(unsigned char *bitmap, int pos);

#endif