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
                log_debug(memoria_log_debug, "LLEGO EL MENSAJE");
                int size;
	            void* buffer;
                buffer = recibir_buffer(&size, fd_kernel);
                if(buffer== NULL){
                    printf("SOY NULO");
                }
                uint16_t pid = extraer_uint16_al_buffer (buffer);
                char* path = extraer_string_al_buffer(buffer);
                control_key = 0;
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
