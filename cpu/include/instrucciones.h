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
void MOV_IN (void *registroDatos, void* registroDireccion, uint8_t tamanio_de_registro_datos, uint8_t tamanio_de_registro_direccion);
void MOV_OUT(void *registroDireccion, void *registroDatos, uint8_t tamanio_de_registro_direccion, uint8_t tamanio_de_registro_datos);
void COPY_STRING(uint8_t tamanio_a_copiar);
void IO_STDIN_READ(void *nombre_de_la_interfaz, void *registro_direccion, void* registro_tamanio,uint8_t tamanio_de_registro_direccion, uint8_t tamanio_del_registro);
void IO_STDOUT_WRITE(void *nombre_de_la_interfaz, void *registro_direccion, void *registro_tamanio,uint8_t tamanio_de_registro_direccion, uint8_t tamanio_del_registro);


void IO_FS_CREATE(void *nombre_de_la_interfaz, void *nombre_del_archivo);
void IO_FS_DELETE(void *nombre_de_la_interfaz, void *nombre_del_archivo);
void IO_FS_TRUNCATE(void *nombre_de_la_interfaz, void *nombre_del_archivo, void *registro_tamanio, uint8_t tamanio_del_registro);
void IO_FS_WRITE(void *nombre_de_la_interfaz, void *nombre_del_archivo, void *registro_direccion, void *registro_tamanio, void *registro_puntero_de_archivo, uint8_t tam_registro_dir, uint8_t tam_registro_tam, uint8_t tam_registro_puntero);
void IO_FS_READ(void *nombre_de_la_interfaz, void *nombre_del_archivo, void *registro_direccion, void *registro_tamanio, void *registro_puntero_de_archivo, uint8_t tam_registro_dir, uint8_t tam_registro_tam, uint8_t tam_registro_puntero);


#endif