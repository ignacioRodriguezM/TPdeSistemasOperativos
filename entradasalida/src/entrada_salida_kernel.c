#include "../include/entrada_salida_kernel.h"
#include "../include/filesystem.h"
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
                caso_io_gen_sleep(buffer_recibido);

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
                caso_io_fs_create(buffer_recibido);
            }
            else if (strcmp(operacion_a_realizar, "IO_FS_DELETE") == 0)
            {
                caso_io_fs_delete(buffer_recibido);
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

            free(nombre_io_llegado_del_kernel);
            free(operacion_a_realizar);

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
void enviar_confirmacion_a_kernel (uint16_t pid)
{
    // DEVOLVER CONFIRMACION AL KERNEL
        t_buffer *buffer_a_enviar = crear_buffer();
        cargar_string_al_buffer(buffer_a_enviar, nombre); //[nombre][pid]
        cargar_uint16_al_buffer(buffer_a_enviar, pid);
        t_paquete *a_enviar = crear_paquete(FIN_DE_EJECUCION_DE_IO, buffer_a_enviar);

        enviar_paquete(a_enviar, fd_kernel);

        destruir_paquete(a_enviar);
}

void caso_io_gen_sleep(t_buffer *buffer_recibido)
{
    //[pid][unidades de trabajo]
    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
    uint16_t unidades_de_trabajo = extraer_uint8_al_buffer(buffer_recibido);
    destruir_buffer(buffer_recibido);
    log_info(entrada_salida_logger, "PID: %u - Operacion: IO_GEN_SLEEP", pid);

    esperarMilisegundos(unidades_de_trabajo * tiempo_unidad_trabajo);

    enviar_confirmacion_a_kernel(pid);
}

char *obtener_mensaje_de_stdin(uint8_t tamanio)
{

    char *mensaje_final = malloc(tamanio);

    char *leido = readline("> ");

    memcpy((char *)mensaje_final, (char *)leido, tamanio);

    free(leido);

    return mensaje_final;
}

void caso_io_stdin_read(t_buffer *buffer_recibido)
{
    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
    uint8_t tamanio = extraer_uint8_al_buffer(buffer_recibido);
    uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);

    char *mensaje = obtener_mensaje_de_stdin(tamanio);

    log_trace(entrada_salida_log_debug, "Mensaje a escribir : %s", mensaje);

    t_buffer *buffer_a_enviar_a_memoria = crear_buffer();

    //  [tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....

    cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, tamanio);
    cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, cantidad_de_direcciones);

    void *dato = (char *)mensaje;
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

        enviar_confirmacion_a_kernel(pid);

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
        log_trace(entrada_salida_log_debug, "RESPUESTA MEMORIA : %s", mensaje_de_respuesta);

        printf("\n %s \n", mensaje_de_respuesta);

        destruir_buffer(recibido);

        enviar_confirmacion_a_kernel(pid);

        free(mensaje_de_respuesta);
    }
}

/*
1ero abrir y leer para chequear en el archivo bitmap si hay lugar
    ahi determino cual es la primera posicion en el bitmap que me puede guardar el arhivo de manera contigua
2do cambiar la cantidad de bits que ocupe el archivo en el bitmap
3ero crear el archivo de metadata con el nombre especificado y escribir la información del bloque inicial y el tamaño del archivo.
4to actualizar el archivo de bloques
*/
void caso_io_fs_create(t_buffer *buffer_recibido)
{
    // [PID] [NOMBRE_ARCHIVO]
    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
    char *nombre_del_archivo = extraer_string_al_buffer(buffer_recibido);

    esperarMilisegundos(tiempo_unidad_trabajo);
    
    int tamanio_del_archivo_en_bloques = 1;

    // Leer el bitmap
    t_bitarray *bitmap = leer_bitmap();
    if (bitmap == NULL)
    {
        return;
    }

    // Buscar la primera secuencia de bloques libres
    int bloque_libre = buscar_secuencia_libre(bitmap, tamanio_del_archivo_en_bloques);

    if (bloque_libre == -1)
    {
        log_error(entrada_salida_logger, "Error: No hay suficiente espacio en el sistema de archivos.\n");
        return;
    }
    else
    {

        printf("La primera secuencia de %d bloques libres comienza en el bloque %d.\n", tamanio_del_archivo_en_bloques, bloque_libre);

        // Marcar los bloques como ocupados
        marcar_bloques_ocupados(bitmap, bloque_libre, tamanio_del_archivo_en_bloques);

        actualizar_archivo_bitmap(bitmap);

        // Crear metadata del archivo
        crear_metadata(nombre_del_archivo, bloque_libre, tamanio_de_bloque);
    }

    // Liberar memoria
    bitarray_destroy(bitmap);

    log_info(entrada_salida_logger, "PID: %d - Crear archivo: %s", pid, nombre_del_archivo);

    free(nombre_del_archivo);

    enviar_confirmacion_a_kernel(pid);
}

void caso_io_fs_delete(t_buffer *buffer_recibido)
{
    // [PID] [NOMBRE_ARCHIVO]
    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
    char *nombre_del_archivo = extraer_string_al_buffer(buffer_recibido);

    esperarMilisegundos(tiempo_unidad_trabajo);

    METADATA info_archivo = leer_metadata(nombre_del_archivo);

    t_bitarray *bitmap = leer_bitmap();
    if (bitmap == NULL)
    {
        return;
    }

    // calcular cantidad de bloques ocupados por archivo
    int bloques_ocupados = division_entera_redondear_arriba(info_archivo.tam_bytes, tamanio_de_bloque);

    liberar_bloques_del_bitmap(bitmap, info_archivo.bloque_inicial, bloques_ocupados);
    actualizar_archivo_bitmap(bitmap);
    bitarray_destroy(bitmap);

    borrar_archivo_metadata(nombre_del_archivo);

    log_info(entrada_salida_logger, "PID: %d - Eliminar archivo: %s", pid, nombre_del_archivo);

    free(nombre_del_archivo);

    enviar_confirmacion_a_kernel(pid);
}