#include "../include/kernel-Entrada_Salida.h"

void atender_kernel_entrada_salida(){
    while (1){
        //Esperar conexion de i/o

        log_info(kernel_logger, "Esperando a  algun entrada_salida...");
        fd_entrada_salida = esperar_cliente (fd_kernel, kernel_logger, "ENTRADA SALIDA");


        bool control_key = 1; 
        while(control_key){
            int cod_op = recibir_operacion(fd_entrada_salida);
            switch (cod_op)
            {
            
            case PRESENTACION:
                t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_entrada_salida);

                char* nombre_del_io_presentado = extraer_string_al_buffer (buffer_recibido);
                interfaces_io* puntero_recibido = extraer_choclo_al_buffer (buffer_recibido);

                log_info (kernel_log_debug, "LLEGO");
                interfaces_io tipo_interfaz = *puntero_recibido;
                free(puntero_recibido);

                log_info (kernel_log_debug, "%s", nombre_del_io_presentado);

                bool check = false;
                if(contador_de_colas_bloqueados >0){

                    for(int i=0; i< contador_de_colas_bloqueados;i++){
                        if(colas_bloqueados[i]->nombre == nombre_del_io_presentado){
                            colas_bloqueados[i]->fd = fd_entrada_salida;
                            check = true;
                            log_info (kernel_log_debug, "TE VOLVI A ASIGNAR A TU COLA DE BLOQUEADOS");
                            break;
                        }
                    }
                if (contador_de_colas_bloqueados == 0){
                    contador_de_colas_bloqueados ++;
                    colas_bloqueados = malloc(sizeof(Colas_bloqueados*)); // Reservar memoria para un solo puntero a Colas_bloqueados
                    colas_bloqueados[0] = malloc(sizeof(Colas_bloqueados)); // Reservar memoria para el primer elemento de Colas_bloqueados
                    colas_bloqueados[0]->nombre = nombre_del_io_presentado;
                    colas_bloqueados[0]->tipo_interfaz = tipo_interfaz;
                    colas_bloqueados[0]->cola = queue_create();
                    colas_bloqueados[0]->fd = fd_entrada_salida;
                    log_info (kernel_log_debug, "TE CREE PRIMERO");
                }


                    if (check == false){
                        contador_de_colas_bloqueados++;
                        colas_bloqueados = realloc(colas_bloqueados, contador_de_colas_bloqueados * sizeof(Colas_bloqueados*));
                        colas_bloqueados[contador_de_colas_bloqueados - 1] = malloc(sizeof(Colas_bloqueados));
                        colas_bloqueados[contador_de_colas_bloqueados - 1]->nombre = nombre_del_io_presentado;
                        colas_bloqueados[contador_de_colas_bloqueados - 1]->tipo_interfaz = tipo_interfaz;
                        colas_bloqueados[contador_de_colas_bloqueados - 1]->cola = queue_create();
                        colas_bloqueados[contador_de_colas_bloqueados - 1]->fd = fd_entrada_salida;
                        log_info (kernel_log_debug, "TE CREE AL FINAL");
                    }
                }
                
                

                control_key = 0;
                break;

            case -1:
            log_error(kernel_logger, "Desconexion de KERNEL-i/o");
            control_key = 0;
                break;
            default:
            log_warning(kernel_logger,"Operacion desconocida de KERNEL-i/o");
                break;
            }
        }
    }
}