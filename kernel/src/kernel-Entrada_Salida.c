#include "../include/kernel-Entrada_Salida.h"

void atender_kernel_entrada_salida(int *socket_ptr)
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

            log_info(kernel_log_debug, "LLEGO");
            interfaces_io tipo_interfaz = *puntero_recibido;
            destruir_buffer (buffer_recibido);
            free(puntero_recibido);

            log_info(kernel_log_debug, "%s", nombre_del_io_presentado);

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
                        log_info(kernel_log_debug, "TE VOLVI A ASIGNAR A TU COLA DE BLOQUEADOS");
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
                log_info(kernel_log_debug, "TE CREE PRIMERO");
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
                log_info(kernel_log_debug, "TE CREE AL FINAL");
            }

            thread_mutex_unlock(&mutex_colas);
            
            break;

        case -1:
            log_error(kernel_logger, "Desconexion de KERNEL-i/o");
            colas_bloqueados[posicion]->conectado = false;
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de KERNEL-i/o");
            break;
        }
    }
}