#include "../include/memoria_kernel.h"

void atender_memoria_kernel (){
    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel);
		switch (cod_op) {
            case MENSAJE:
                //
                break;
            case PAQUETE:
                //
                break;

            case INICIAR_PROCESO:
                // Recibir el tamaño del buffer
                int size_of_stream = recibir_size_del_buffer(fd_kernel);
                
                t_buffer* buffer_recibido = crear_buffer();
                buffer_recibido -> size = size_of_stream;
                buffer_recibido->stream = malloc(size_of_stream);

                //recibo el buffer, teniendo como dato su tamanio
                if (recv(fd_kernel, buffer_recibido -> stream, size_of_stream, MSG_WAITALL) != size_of_stream ){
                perror("Error al recibir el buffer");
                exit(EXIT_FAILURE);
                }

                uint16_t pid_recibido = extraer_uint16_al_buffer (buffer_recibido);
                char* path_recibido = extraer_string_al_buffer (buffer_recibido);
            
                // Imprimir
                printf("SE RECIBIO: %hu\n", pid_recibido);            
                printf("SE RECIBIO: %s\n", path_recibido);

                

                

            break;


            case -1:
                log_error(memoria_logger, "Desconexion de KERNEL");
                control_key = 0;
                break;
            default:
                log_warning(memoria_logger,"Operacion desconocida de KERNEL");
                break;
            }
	}
}
