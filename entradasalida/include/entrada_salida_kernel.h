#ifndef ENTRADA_SALIDA_KERNEL_H_
#define ENTRADA_SALIDA_KERNEL_H_

#include "./es_gestor.h"
#include "./configuracion_entrada_salida.h"

void atender_entrada_salida_kernel();
void presentarse_con_kernel();

void caso_io_gen_sleep(t_buffer *buffer_recibido);

void caso_io_stdin_read(t_buffer* buffer_recibido);
void caso_io_stdout_write(t_buffer* buffer_recibido);

void caso_io_fs_create(t_buffer* buffer_recibido);
void caso_io_fs_delete(t_buffer *buffer_recibido);
void caso_io_fs_truncate (t_buffer* buffer_recibido);
void caso_io_fs_read(t_buffer *buffer_recibido);
void caso_io_fs_write(t_buffer *buffer_recibido);

#endif