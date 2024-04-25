#include "../include/entrada_salida_kernel.h"

void atender_entrada_salida_kernel()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel);
        switch (cod_op)
        {
        case GEN_SLEEP:
            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_kernel);
            // [nombre io][operacion][pid][unidades de trabajo]
            char *nombre_io_llegado_del_kernel = extraer_string_al_buffer(buffer_recibido);
            char *operacion_a_realizar = extraer_string_al_buffer(buffer_recibido);
            uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
            uint8_t unidades_de_trabajo = extraer_uint8_al_buffer(buffer_recibido);

            if (strcmp(nombre_io_llegado_del_kernel, nombre) != 0)
            {
                log_error(entrada_salida_log_debug, "ME LLEGO UNA OPERACION QUE NO DEBIA, YA QUE NO ESTA A MI NOMBRE");
            }

            if (strcmp(operacion_a_realizar, "IO_GEN_SLEEP") != 0)
            {
                log_error(entrada_salida_log_debug, "ME LLEGO UNA OPERACION QUE NO DEBIA, YA QUE NO LA PUEDO HACER");
            }
            log_info(entrada_salida_logger, "PID: %u - Operacion: %s", pid, operacion_a_realizar);
            esperarMilisegundos (unidades_de_trabajo * tiempo_unidad_trabajo);
            //DEVOLVER CONFIRMACION AL KERNEL
            t_buffer *buffer_a_enviar = crear_buffer();
            cargar_string_al_buffer(buffer_a_enviar, nombre); //[nombre][pid]
            cargar_uint16_al_buffer(buffer_a_enviar, pid);
            t_paquete *a_enviar = crear_paquete(FIN_DE_EJECUCION_DE_IO, buffer_a_enviar);

            enviar_paquete(a_enviar, fd_kernel);

            destruir_paquete(a_enviar);

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

void esperarMilisegundos(unsigned int milisegundos)
{
    usleep(milisegundos * 1000); // Convertimos milisegundos a microsegundos
}