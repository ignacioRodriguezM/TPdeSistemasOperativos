#include "../include/memoria_cpu.h"

void atender_memoria_cpu()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_cpu);
        switch (cod_op)
        {
        case TAM_PAG:
            
            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);

            destruir_buffer(buffer_recibido);
            
            t_buffer *buff = crear_buffer();
            cargar_uint16_al_buffer(buff, tam_pagina);
            t_paquete *paq = crear_paquete(TAM_PAG, buff);
            enviar_paquete(paq, fd_cpu);
            destruir_paquete(paq);

            break;
        case SOLICITUD_DE_PROXIMA_INSTRUCCION:

            _solicitud_de_proxima_instruccion();

            break;

        case AJUSTAR_TAMANIO_PROCESO:

            _ajustar_tamanio_proceso();

            break;
            
        case ACCESO_A_TABLA_DE_PAG:

            _acceder_a_tabla_de_pagina();

            break;

        case LECTURA:

            _leer_una_determinada_direccion();

            break;
        case ESCRITURA:

            _escribir_una_determinada_direccion();

            break;

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
void _acceder_a_tabla_de_pagina()
{
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
    uint16_t pid_recibido = extraer_uint16_al_buffer(buffer_recibido);
    uint16_t pagina_a_consultar = extraer_uint16_al_buffer(buffer_recibido);
    destruir_buffer(buffer_recibido);
    uint16_t marco_respuesta = marco_correspondiente_a_pagina_consultada(pid_recibido, pagina_a_consultar);

    esperarMilisegundos(retardo_respuesta);


    t_buffer *buffer_a_enviar = crear_buffer(); //[instruccion]
    cargar_uint16_al_buffer(buffer_a_enviar, marco_respuesta);

    t_paquete *a_enviar = crear_paquete(ACCESO_A_TABLA_DE_PAG, buffer_a_enviar);
    enviar_paquete(a_enviar, fd_cpu);
    destruir_paquete(a_enviar);
}

void _solicitud_de_proxima_instruccion(){
    
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
}


void _leer_una_determinada_direccion (){
    void* registroDatos;

    //  [tamanio registro datos] [Cantidad] [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .....
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
    
    //recibo el tamanio del registro datos
    uint8_t tamanio_de_registro_datos = extraer_uint8_al_buffer(buffer_recibido);
    
    // inicializar el registroDatos con el tamaño adecuado
    registroDatos = malloc(tamanio_de_registro_datos);
    
    //  [CANTIDAD MARCOS] [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] / [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] / [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] ....
    
    uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);
    
    uint8_t offset = 0;
    for (int i = 0; i < cantidad_de_direcciones ; i++){
    
        uint8_t tamanio_de_direccion = extraer_uint8_al_buffer(buffer_recibido);
        uint16_t marco = extraer_uint16_al_buffer(buffer_recibido);
        uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);
        
        // Calcular la dirección física
        void *direccion_fisica = memoria_usuario + (marco * tam_pagina) + desplazamiento;

        memcpy(registroDatos + offset, direccion_fisica, tamanio_de_direccion);

        offset += tamanio_de_direccion;
    }
    
    destruir_buffer(buffer_recibido);  
    
    esperarMilisegundos(retardo_respuesta);

    t_buffer *buffer_a_enviar = crear_buffer();

    cargar_choclo_al_buffer(buffer_a_enviar, registroDatos, tamanio_de_registro_datos);

    t_paquete *a_enviar = crear_paquete(LECTURA, buffer_a_enviar);
    enviar_paquete(a_enviar, fd_cpu);
    destruir_paquete(a_enviar);

    free(registroDatos);

}

void _escribir_una_determinada_direccion() {
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);

    //  [tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....


    // Recibo el tamaño del registro de datos
    uint8_t tamanio_de_registro_datos = extraer_uint8_al_buffer(buffer_recibido);
    tamanio_de_registro_datos = 0;
    //NO SE USA

    // Recibo la cantidad de direcciones
    uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);

    
    // Leer las direcciones y los datos del buffer y escribirlos en la memoria
    void* datos_a_escribir;
    for (int i = 0; i < cantidad_de_direcciones; i++) {
        uint8_t tamanio_de_direccion = extraer_uint8_al_buffer(buffer_recibido);
        uint16_t marco = extraer_uint16_al_buffer(buffer_recibido);
        uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);

        
        datos_a_escribir = extraer_choclo_al_buffer(buffer_recibido);

        // Calcular la dirección física
        void *direccion_fisica = memoria_usuario + (marco * tam_pagina) + desplazamiento;

        // Escribir los datos en la memoria física
        memcpy(direccion_fisica, datos_a_escribir, tamanio_de_direccion);

        free(datos_a_escribir);
        
    }

    destruir_buffer(buffer_recibido);

    esperarMilisegundos(retardo_respuesta);
    

    // Enviar confirmación al CPU
    t_buffer *buffer_confirmacion = crear_buffer();
    char* respuesta = "OK"; //no se bien como seria el tema de saber si lo pudo hacer o no
    cargar_string_al_buffer(buffer_confirmacion, respuesta);
    t_paquete *paquete_confirmacion = crear_paquete(ESCRITURA, buffer_confirmacion);
    enviar_paquete(paquete_confirmacion, fd_cpu);
    destruir_paquete(paquete_confirmacion);
}

