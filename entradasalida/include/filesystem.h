#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "./es_gestor.h"
#include "./configuracion_entrada_salida.h"

///////////// funciones para crear archivos ///////
void crear_archivo(char *nombre, int tamanio_del_archivo);
void crear_metadata(const char *nombre_archivo, int bloque_inicial, int tamanio_archivo_en_bytes);


///////////// funciones para operar el bitmap ///////
t_bitarray* leer_bitmap();
void actualizar_archivo_bitmap (t_bitarray* bitmap);

int buscar_secuencia_libre(t_bitarray* bitmap, int longitud);
void marcar_bloques_ocupados(t_bitarray* bitmap, int inicio, int longitud);


///////////// inicializaciones ///////
void inicializar_estructuras_filesystem ();

void _crear_archivo_que_seria_el_disco();
void _crear_archivo_que_contiene_el_bitmap();

#endif