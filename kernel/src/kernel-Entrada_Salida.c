#include "../include/kernel-Entrada_Salida.h"
void atender_kernel_entrada_salida (){
    while(1){
        
        fd_entrada_salida = esperar_cliente (fd_kernel, kernel_logger, "ENTRADA SALIDA");

        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = fd_entrada_salida;

        pthread_t hilo_multipes_entrada_salida;
        pthread_create (&hilo_multipes_entrada_salida, NULL, (void*)atender_multiples_entrada_salida, (int*)client_socket_ptr);
        pthread_detach(hilo_multipes_entrada_salida);
    }
}


void atender_multiples_entrada_salida(int *socket_ptr)
{   
    int client_socket = *socket_ptr;
    bool control_key = 1;
    while (control_key)
    {   
        int posicion;
        int cod_op = recibir_operacion(client_socket);
        switch (cod_op)
        {

        case PRESENTACION:
            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(client_socket);

            char *nombre_del_io_presentado = extraer_string_al_buffer(buffer_recibido);
            interfaces_io *puntero_recibido = extraer_choclo_al_buffer(buffer_recibido);


            interfaces_io tipo_interfaz = *puntero_recibido;

            destruir_buffer (buffer_recibido);
            free(puntero_recibido);

            bool check = false;
            if (contador_de_colas_bloqueados > 0)
            {

                for (int i = 0; i < contador_de_colas_bloqueados; i++)
                {
                    if (strcmp(colas_bloqueados[i]->nombre, nombre_del_io_presentado) == 0)
                    {
                        colas_bloqueados[i]->fd = client_socket;
                        colas_bloqueados[i]->conectado = true;
                        posicion = i;
                        check = true;
                        log_info(kernel_log_debug, "VOLVI A ASIGNAR A %s A SU COLA DE BLOQUEADOS ", nombre_del_io_presentado);
                        break;
                    }
                }
            }

            pthread_mutex_lock(&mutex_colas);

            if (contador_de_colas_bloqueados == 0)
            {   
                contador_de_colas_bloqueados++;
                colas_bloqueados = malloc(sizeof(Colas_bloqueados *));  // Reservar memoria para un solo puntero a Colas_bloqueados
                colas_bloqueados[0] = malloc(sizeof(Colas_bloqueados)); // Reservar memoria para el primer elemento de Colas_bloqueados
                colas_bloqueados[0]->nombre = nombre_del_io_presentado;
                colas_bloqueados[0]->tipo_interfaz = tipo_interfaz;
                colas_bloqueados[0]->cola = queue_create();
                colas_bloqueados[0]->fd = client_socket;
                colas_bloqueados[0]->conectado = true;
                posicion = 0;
                log_info(kernel_log_debug, "CREE UNA COLA DE BLOQUEADOS PARA %s ", nombre_del_io_presentado);
                check = true;
            }

            if (check == false)
            {
                contador_de_colas_bloqueados++;
                colas_bloqueados = realloc(colas_bloqueados, contador_de_colas_bloqueados * sizeof(Colas_bloqueados *));
                colas_bloqueados[contador_de_colas_bloqueados - 1] = malloc(sizeof(Colas_bloqueados));
                colas_bloqueados[contador_de_colas_bloqueados - 1]->nombre = nombre_del_io_presentado;
                colas_bloqueados[contador_de_colas_bloqueados - 1]->tipo_interfaz = tipo_interfaz;
                colas_bloqueados[contador_de_colas_bloqueados - 1]->cola = queue_create();
                colas_bloqueados[contador_de_colas_bloqueados - 1]->fd = client_socket;
                colas_bloqueados[contador_de_colas_bloqueados - 1]->conectado = true;
                posicion = contador_de_colas_bloqueados - 1;
                log_info(kernel_log_debug, "CREE UNA COLA DE BLOQUEADOS PARA %s ", nombre_del_io_presentado);
                check = true;
            }
            
            pthread_mutex_unlock(&mutex_colas);
            
            break;

        case FIN_DE_EJECUCION_DE_IO:
                //[nombre][pid]
                t_buffer* buffer_recibido = recibir_buffer_sin_cod_op(client_socket);
                char* nombre_de_io = extraer_string_al_buffer(buffer_recibido);
                uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);

                
            break;

        case -1:
            log_error(kernel_logger, "Desconexion de KERNEL-i/o ");
            colas_bloqueados[posicion]->conectado = false;
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de KERNEL-i/o ");
            break;
        }
    }
}