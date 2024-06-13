#include "../include/entrada_salida_kernel.h"

void atender_entrada_salida_kernel()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel);
        switch (cod_op)
        {
        case TAREA:
            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_kernel);
            // [nombre io][operacion]
            char *nombre_io_llegado_del_kernel = extraer_string_al_buffer(buffer_recibido);
            char *operacion_a_realizar = extraer_string_al_buffer(buffer_recibido);

            if (strcmp(nombre_io_llegado_del_kernel, nombre) != 0)
            {
                log_error(entrada_salida_log_debug, "ME LLEGO UNA OPERACION QUE NO DEBIA, YA QUE NO ESTA A MI NOMBRE");
            }

            if (strcmp(operacion_a_realizar, "IO_GEN_SLEEP") == 0)
            {
                //[pid][unidades de trabajo]
                uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
                uint16_t unidades_de_trabajo = extraer_uint8_al_buffer(buffer_recibido);
                destruir_buffer(buffer_recibido);
                log_info(entrada_salida_logger, "PID: %u - Operacion: %s", pid, operacion_a_realizar);
                esperarMilisegundos(unidades_de_trabajo * tiempo_unidad_trabajo);
                // DEVOLVER CONFIRMACION AL KERNEL
                t_buffer *buffer_a_enviar = crear_buffer();
                cargar_string_al_buffer(buffer_a_enviar, nombre); //[nombre][pid]
                cargar_uint16_al_buffer(buffer_a_enviar, pid);
                t_paquete *a_enviar = crear_paquete(FIN_DE_EJECUCION_DE_IO, buffer_a_enviar);

                enviar_paquete(a_enviar, fd_kernel);

                destruir_paquete(a_enviar);
            }
            else if (strcmp(operacion_a_realizar, "IO_STDIN_READ") == 0)
            {
                caso_io_stdin_read(buffer_recibido);
                
            }
            else if (strcmp(operacion_a_realizar, "IO_STDOUT_WRITE") == 0)
            {
                caso_io_stdout_write(buffer_recibido);
            }
            else if (strcmp(operacion_a_realizar, "IO_FS_CREATE") == 0)
            {
            }
            else if (strcmp(operacion_a_realizar, "IO_FS_DELETE") == 0)
            {
            }
            else if (strcmp(operacion_a_realizar, "IO_FS_TRUNCATE") == 0)
            {
            }
            else if (strcmp(operacion_a_realizar, "IO_FS_WRITE") == 0)
            {
            }
            else if (strcmp(operacion_a_realizar, "IO_FS_READ") == 0)
            {
            }

            break;
        case -1:
            log_error(entrada_salida_logger, "Desconexion de KERNEL");
            control_key = 0;
            break;
        default:
            log_warning(entrada_salida_logger, "Operacion desconocida de KERNEL");
            break;
        }
    }
}
void presentarse_con_kernel()
{

    t_buffer *buffer = crear_buffer();
    cargar_string_al_buffer(buffer, nombre);
    cargar_choclo_al_buffer(buffer, &tipo_interfaz, sizeof(interfaces_io));

    t_paquete *a_enviar = crear_paquete(PRESENTACION, buffer);

    enviar_paquete(a_enviar, fd_kernel);

    destruir_paquete(a_enviar);
}

char* obtener_mensaje_de_stdin(uint8_t tamanio){
    
    char* mensaje_final = malloc(tamanio);

    char* leido = readline("> ");

    memcpy((char*)mensaje_final, (char*)leido, tamanio);
    
    free(leido);
    
    return mensaje_final;
}

void caso_io_stdin_read(t_buffer *buffer_recibido)
{   
    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
    uint8_t tamanio = extraer_uint8_al_buffer(buffer_recibido);
    uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);

    

    char* mensaje = obtener_mensaje_de_stdin(tamanio);

    log_trace(entrada_salida_log_debug, "Mensaje a escribir : %s", mensaje);

    t_buffer *buffer_a_enviar_a_memoria = crear_buffer();

    //  [tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....
    
    cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, tamanio);
    cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, cantidad_de_direcciones);
    
    void* dato = (char*) mensaje;
    for (int i = 0; i < cantidad_de_direcciones; i++)
    {
        uint8_t tam_a_escribir_por_pagina = extraer_uint8_al_buffer(buffer_recibido);
        uint16_t numero_de_pagina = extraer_uint16_al_buffer(buffer_recibido);
        uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);

        cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, tam_a_escribir_por_pagina);
        cargar_uint16_al_buffer(buffer_a_enviar_a_memoria, numero_de_pagina);
        cargar_uint32_al_buffer(buffer_a_enviar_a_memoria, desplazamiento);
        cargar_choclo_al_buffer(buffer_a_enviar_a_memoria, dato, tam_a_escribir_por_pagina);

        dato += tam_a_escribir_por_pagina;
    }

    free(mensaje);
    destruir_buffer(buffer_recibido);

    log_info(entrada_salida_logger, "PID: %u - Operacion: IO_STDIN_READ", pid);


    
    t_paquete *a_enviar_a_memoria = crear_paquete(ESCRITURA, buffer_a_enviar_a_memoria);

    enviar_paquete(a_enviar_a_memoria, fd_memoria);

    destruir_paquete(a_enviar_a_memoria);
    // espero a que memoria me de el ok para mandar confirmacion a kernel luego
    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case ESCRITURA:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        char *mensaje_de_respuesta = extraer_string_al_buffer(recibido);
        log_info(entrada_salida_logger, "RESPUESTA MEMORIA : %s", mensaje_de_respuesta);

        destruir_buffer(recibido);

        // DEVOLVER CONFIRMACION AL KERNEL
        t_buffer *buffer_a_enviar = crear_buffer();
        cargar_string_al_buffer(buffer_a_enviar, nombre); //[nombre][pid]
        cargar_uint16_al_buffer(buffer_a_enviar, pid);
        t_paquete *a_enviar = crear_paquete(FIN_DE_EJECUCION_DE_IO, buffer_a_enviar);

        enviar_paquete(a_enviar, fd_kernel);

        destruir_paquete(a_enviar);

        free(mensaje_de_respuesta);
    }
}

void caso_io_stdout_write(t_buffer *buffer_recibido)
{   
    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
    uint8_t tamanio = extraer_uint8_al_buffer(buffer_recibido);
    uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);

    

    t_buffer *buffer_a_enviar_a_memoria = crear_buffer();

    // [TAM_A_leer] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_leer] [MARCO] [DESPLAZAMIENTO] [DATO] .....
    
    cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, tamanio);
    cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, cantidad_de_direcciones);
    
    for (int i = 0; i < cantidad_de_direcciones; i++)
    {
        uint8_t tam_a_leer_por_pagina = extraer_uint8_al_buffer(buffer_recibido);
        uint16_t numero_de_pagina = extraer_uint16_al_buffer(buffer_recibido);
        uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);

        cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, tam_a_leer_por_pagina);
        cargar_uint16_al_buffer(buffer_a_enviar_a_memoria, numero_de_pagina);
        cargar_uint32_al_buffer(buffer_a_enviar_a_memoria, desplazamiento);
    }

    destruir_buffer(buffer_recibido);

    log_info(entrada_salida_logger, "PID: %u - Operacion: IO_STDOUT_WRITE", pid);


    
    t_paquete *a_enviar_a_memoria = crear_paquete(LECTURA, buffer_a_enviar_a_memoria);

    enviar_paquete(a_enviar_a_memoria, fd_memoria);

    destruir_paquete(a_enviar_a_memoria);
    // espero a que memoria me de el ok para mandar confirmacion a kernel luego
    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case LECTURA:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        char *mensaje_de_respuesta = extraer_string_al_buffer(recibido);
        log_info(entrada_salida_logger, "RESPUESTA MEMORIA : %s", mensaje_de_respuesta);
        
        printf("%s", mensaje_de_respuesta);

        destruir_buffer(recibido);

        // DEVOLVER CONFIRMACION AL KERNEL
        t_buffer *buffer_a_enviar = crear_buffer();
        cargar_string_al_buffer(buffer_a_enviar, nombre); //[nombre][pid]
        cargar_uint16_al_buffer(buffer_a_enviar, pid);
        t_paquete *a_enviar = crear_paquete(FIN_DE_EJECUCION_DE_IO, buffer_a_enviar);

        enviar_paquete(a_enviar, fd_kernel);

        destruir_paquete(a_enviar);

        free(mensaje_de_respuesta);
    }
}