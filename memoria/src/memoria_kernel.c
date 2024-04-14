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
                log_debug(memoria_log_debug, "LLEGO EL MENSAJE"); // [PID] [PATH]
                int size;
            if (recv(fd_kernel, &size, sizeof(int), 0) != sizeof(int)){
                perror( "ERROR al recibir el tamaño del buffer");
                // Manejo de error
            }
            
            void* stream = malloc(size);
            if (recv(fd_kernel, stream, size, 0) != size){
                perror( "ERROR al recibir el stream del buffer");
                // Manejo de error
            }

            // Crear un buffer e inicializarlo con el stream recibido
            t_buffer* buffer = crear_buffer();
            buffer->size = size;
            buffer->stream = stream;

            // Extraer los datos del buffer
            uint16_t pid_recibido = extraer_uint16_del_paquete(crear_paquete(INICIAR_PROCESO, buffer));
            char* path_recibido = extraer_string_del_paquete(crear_paquete(INICIAR_PROCESO, buffer));
            
            // Imprimir
            printf("PID recibido: %hu\n", pid_recibido);            
            printf("Path recibido: %s\n", path_recibido);

            // Liberar la memoria utilizada
            free(stream);
            free(buffer);

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
