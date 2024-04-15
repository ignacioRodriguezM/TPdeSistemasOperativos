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
                t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_kernel);

                uint16_t pid_recibido = extraer_uint16_al_buffer (buffer_recibido);
                char* path_recibido = extraer_string_al_buffer (buffer_recibido);
                destruir_buffer (buffer_recibido);
                
                // Imprimir, solo para verificar que lleguen bien, dsp borrar
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
