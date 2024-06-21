#include "../include/memoria_entrada_salida.h"

void atender_memoria_entrada_salida()
{
    while (1)
    {

        fd_entrada_salida = esperar_cliente(fd_memoria, memoria_logger, "ENTRADA SALIDA");

        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = fd_entrada_salida;

        pthread_t hilo_multipes_entrada_salida;
        pthread_create(&hilo_multipes_entrada_salida, NULL, (void *)atender_multiples_entrada_salida, (int *)client_socket_ptr);
        pthread_detach(hilo_multipes_entrada_salida);
    }
}
void atender_multiples_entrada_salida(int *socket_ptr)
{
    int client_socket = *socket_ptr;
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(client_socket);
        switch (cod_op)
        {
        case ESCRITURA:
            manejar_read(client_socket);
            break;
        case LECTURA:
            manejar_write(client_socket);
            break;
        }
    }
}

void manejar_read(int fd_io)
{ // aka _escribir_en_una_determinada_direccion
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_io);

    //  [tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....

    // Recibo el tamaño del registro de datos
    uint8_t tamanio_de_registro_datos = extraer_uint8_al_buffer(buffer_recibido);
    // no se usa .....
    tamanio_de_registro_datos = 0;

    // Recibo la cantidad de direcciones
    uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);

    // Leer las direcciones y los datos del buffer y escribirlos en la memoria
    void *datos_a_escribir;
    for (int i = 0; i < cantidad_de_direcciones; i++)
    {
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

    // Enviar confirmación a IO
    t_buffer *buffer_confirmacion = crear_buffer();
    char *respuesta = "OK"; // no se bien como seria el tema de saber si lo pudo hacer o no
    cargar_string_al_buffer(buffer_confirmacion, respuesta);
    t_paquete *paquete_confirmacion = crear_paquete(ESCRITURA, buffer_confirmacion);
    enviar_paquete(paquete_confirmacion, fd_io);
    destruir_paquete(paquete_confirmacion);
}
void manejar_write(int fd_io)
{
    void *registroDatos;

    //  [tamanio registro datos] [Cantidad] [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .....
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_io);

    // recibo el tamanio del registro datos
    uint8_t tamanio_de_registro_datos = extraer_uint8_al_buffer(buffer_recibido);

    // inicializar el registroDatos con el tamaño adecuado
    registroDatos = malloc(tamanio_de_registro_datos);

    //  [CANTIDAD MARCOS] [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] / [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] / [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] ....

    uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);

    uint8_t offset = 0;
    for (int i = 0; i < cantidad_de_direcciones; i++)
    {

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
    enviar_paquete(a_enviar, fd_io);
    destruir_paquete(a_enviar);

    free(registroDatos);
}