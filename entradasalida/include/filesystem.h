#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "./es_gestor.h"
#include "./configuracion_entrada_salida.h"

///////////// funcion para crear archivos ///////
void crear_archivo(char *nombre, int tamanio_del_archivo);

///////////// funciones para operar metadatas  ///////
void crear_metadata(const char *nombre_archivo, int bloque_inicial, int tamanio_archivo_en_bytes);
void borrar_archivo_metadata(char* nombre_del_archivo);
void actualizar_archivo_metadata (char * nombre_archivo, int bloque_inicial, int nuevo_tamanio_archivo_en_bytes);

METADATA leer_metadata(char* nombre_archivo);

////////////////////compactar archivos/////////
void compactar_hacia_archivo(char *nombre_del_archivo);
void compactar (char *nombre_archivo, char* buffer, int *puntero_buffer);


///////////// funciones para operar el bitmap ///////
t_bitarray* leer_bitmap();
void actualizar_archivo_bitmap (t_bitarray* bitmap);

int cantidad_de_bloques_libres(t_bitarray* bitmap);
int buscar_secuencia_libre(t_bitarray* bitmap, int longitud);
void marcar_bloques_ocupados(t_bitarray* bitmap, int inicio, int longitud);
void liberar_bloques_del_bitmap (t_bitarray *bitmap, int bloque_inicio_barrido, int longitud);
bool chquear_bloques_libres_contiguos(t_bitarray *bitmap, int primer_bloque_a_chequear, int cantidad);


///////////// inicializaciones ///////
void inicializar_estructuras_filesystem ();

void _verificar_si_existe_archivo_bloques();
void _verificar_si_existe_archivo_bitmap();
void _verificar_que_la_ruta_exista();

void _crear_archivo_que_contiene_el_bitmap(char* ruta);

#endif