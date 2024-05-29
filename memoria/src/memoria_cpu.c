#include "../include/memoria_cpu.h"

void atender_memoria_cpu()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_cpu);
        switch (cod_op)
        {
        case SOLICITUD_DE_PROXIMA_INSTRUCCION:
            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
            uint16_t pid_recibido = extraer_uint16_al_buffer(buffer_recibido);
            uint32_t pc_recibido = extraer_uint32_al_buffer(buffer_recibido);
            destruir_buffer(buffer_recibido);

            t_link_element *current = lista_procesos->head;
            for (int i = 0; i < lista_procesos->elements_count; i++)
            {
                Proceso *proceso = (Proceso *)current->data;
                if (proceso->PID == pid_recibido)
                {
                    if (proceso->cantidad_instrucciones <= pc_recibido)
                    {
                        perror("CPU SOLICITA UN PC POSTERIOR AL ULTIMO PC DEL PROCESO");
                        exit(EXIT_FAILURE);
                    }
                    esperarMilisegundos(retardo_respuesta);
                    t_buffer *buffer_a_enviar = crear_buffer(); //[instruccion]
                    cargar_string_al_buffer(buffer_a_enviar, proceso->instrucciones[pc_recibido]);
                    t_paquete *a_enviar = crear_paquete(PROXIMA_INSTRUCCION, buffer_a_enviar);
                    enviar_paquete(a_enviar, fd_cpu);
                    destruir_paquete(a_enviar);
                    break;
                }
                else
                {
                    current = current->next;
                }
            }
            current = NULL;

            break;

        case AJUSTAR_TAMANIO_PROCESO:
        
            _ajustar_tamanio_proceso();

            break;

        case MARCO_DE_PAGINA:

            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
            uint16_t pid_recibido = extraer_uint16_al_buffer(buffer_recibido);
            int pagina_a_consultar= extraer_int_al_buffer(buffer_recibido);
            destruir_buffer(buffer_recibido);
            marco_correspondiente_a_pagina_consultada(pid_recibido, pagina_a_consultar);

            break;

        // case MOVE_IN:
            
        //     t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
        //     void registroDatos = extraer_void_al_buffer(buffer_recibido);
        //     int registroDireccion = extraer_int_al_buffer(buffer_recibido);
        //     destruir_buffer(buffer_recibido);
            // devolver_valor_de_memoria(registroDatos, RegistroDireccion);

        // case MOVE_OUT:
            
        //     t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
        //     void registroDatos = extraer_void_al_buffer(buffer_recibido);
        //     int registroDireccion = extraer_int_al_buffer(buffer_recibido);
        //     destruir_buffer(buffer_recibido);
            // escribir_valor_en_memoria(registroDireccion, registroDatos);

        case -1:
            log_error(memoria_logger, "Desconexion de CPU");
            control_key = 0;
            break;
        default:
            log_warning(memoria_logger, "Operacion desconocida de CPU");
            break;
        }
    }
}

void _ajustar_tamanio_proceso()
{
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
    uint16_t pid_recibido = extraer_uint16_al_buffer(buffer_recibido);
    uint16_t nuevo_tam_en_bytes = extraer_uint16_al_buffer(buffer_recibido);
    destruir_buffer(buffer_recibido);
    esperarMilisegundos(retardo_respuesta);
    t_buffer *buff = crear_buffer();
    cargar_string_al_buffer(buff, ajustar_tam_proceso(pid_recibido, nuevo_tam_en_bytes));
    t_paquete *a_enviar = crear_paquete(RESPUESTA_RESIZE, buff);
    enviar_paquete(a_enviar, fd_cpu);
    destruir_paquete(a_enviar);
}
