#include "../include/memoria_entrada_salida.h"

void atender_memoria_entrada_salida()
{
    while (1)
    {

        fd_entrada_salida = esperar_cliente(fd_kernel, memoria_logger, "ENTRADA SALIDA");

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

        case PRESENTACION:
            break;
        }
    }
}