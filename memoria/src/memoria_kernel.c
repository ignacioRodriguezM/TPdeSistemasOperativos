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
                
                //asignarle un espacio de memoria a instrucciones, y devolverle el PC al kernel
                
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
                        if (tabla_de_paginas[i]==NULL){
                            break;
                        }
                    }
                
                    

                }
                // Cerrar el archivo
                fclose(archivo);
                // Liberar memoria
                free(comando);
                

            break;

            case FINALIZAR_PROCESO:
            //MAL
                /*t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_kernel);

                uint16_t pid_recibido = extraer_uint16_al_buffer (buffer_recibido);
                char* path_recibido = extraer_string_al_buffer (buffer_recibido);
                destruir_buffer (buffer_recibido);

                //libera el espacio de memoria del proceso y devuelve la pc a kernel

                FILE* archivo = fopen(path_recibido, "r");
                if (archivo == NULL) {
                    printf("No se pudo abrir el archivo.\n");
                    exit(EXIT_FAILURE);
                }else{
                    free(path_recibido);
                    printf("Se libero el espacio en memoria de: %s", path_recibido);
                }
                fclose(archivo);
    
            */
            break;

            case ACCESO_A_TABLA:
            //REVISAR
            /*
                t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_kernel);

                uint16_t pid_recibido = extraer_uint16_al_buffer (buffer_recibido);
                uint16_t pagina_recibida = extraer_uint16_al_buffer (buffer_recibido);
                
                //convertir la dirección virtual de la página en la dirección física del marco correspondiente en la memoria.
                uint16_t num_marco = (uint16_t)((espacio_memoria_usuario + pagina_recibida * tam_pagina) / tam_pagina);
                printf("El número de marco correspondiente a la tabla consultada es: %s", num_marco)
    
            */
            break;



            case AMPLIACION_PROCESO:
            //REVISAR
            /*
                t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_kernel);

                uint16_t pid_recibido = extraer_uint16_al_buffer (buffer_recibido);
                char* path_recibido = extraer_string_al_buffer (buffer_recibido);
                uint16_t tamanio_a_ampliar = extraer_uint16_al_buffer (buffer_recibido);
                destruir_buffer (buffer_recibido);

                */
            break;



            case REDUCCION_PROCESO:
            //REVISAR
            /*
                t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_kernel);
            
                uint16_t pid_recibido = extraer_uint16_al_buffer (buffer_recibido);
                char* path_recibido = extraer_string_al_buffer (buffer_recibido);
                uint16_t tamanio_a_reducir = extraer_uint16_al_buffer(buffer_recibido)
                
                destruir_buffer (buffer_recibido);
                */
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
