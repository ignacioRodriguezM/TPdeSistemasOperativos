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
                
                //asignarle un esppacio de memeoria a instrucciones, y devolverle el PC al kernel
                
                FILE *archivo;
                char* comando = malloc(100 * sizeof(char));
        
                archivo = fopen(path_recibido, "r");
                // Verificar si se pudo abrir el archivo
                if (archivo == NULL) {
                    printf("No se pudo abrir el archivo.\n");
                    exit(EXIT_FAILURE);
                }
                // 
                while (fgets(comando, 100 * sizeof(char), archivo) != NULL) {
                    for (int i=0; i<cantidad_de_paginas; i++){
                        if (*(tabla_de_paginas[i])==NULL){
                            break;
                        }
                    }
                
                    

                }
                // Cerrar el archivo
                fclose(archivo);
                // Liberar memoria
                free(comando);
                

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
