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
                esperarMilisegundos (unidades_de_trabajo * tiempo_unidad_trabajo);
                //DEVOLVER CONFIRMACION AL KERNEL
                t_buffer *buffer_a_enviar = crear_buffer();
                cargar_string_al_buffer(buffer_a_enviar, nombre); //[nombre][pid]
                cargar_uint16_al_buffer(buffer_a_enviar, pid);
                t_paquete *a_enviar = crear_paquete(FIN_DE_EJECUCION_DE_IO, buffer_a_enviar);

                enviar_paquete(a_enviar, fd_kernel);

                destruir_paquete(a_enviar);
            }
            else if (strcmp(operacion_a_realizar, "IO_STDIN_READ") == 0){
                uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
                uint8_t  tamanio = extraer_uint8_al_buffer(buffer_recibido);
                void* registro_direccion = extraer_choclo_al_buffer(buffer_recibido);

                destruir_buffer(buffer_recibido);
                
                log_info(entrada_salida_logger, "PID: %u - Operacion: %s", pid, operacion_a_realizar);
//aca va lo que hay que hacer digamos
            // primero debo enviar lo que se necesite a memoria
                t_buffer *buffer_a_enviar_a_memoria = crear_buffer();

                cargar_uint8_al_buffer(buffer_a_enviar_a_memoria, tamanio); //[nombre][pid]
                cargar_choclo_al_buffer(buffer_a_enviar_a_memoria, registro_direccion);
                
                t_paquete *a_enviar_a_memoria = crear_paquete(IO_STDIN_READ, buffer_a_enviar_a_memoria);

                enviar_paquete(a_enviar_a_memoria, fd_memoria);

                destruir_paquete(a_enviar_a_memoria); 
//espero a que memoria me de el ok para mandar confirmacion a kernel luego
    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case RESPUESTA_MEMORIA:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        char *mensaje_de_respuesta = extraer_string_al_buffer(recibido);
        // log_info(cpu_log_debug, "RESPUESTA MEMORIA : %s", mensaje_de_respuesta);
        destruir_buffer(recibido);
                

                //DEVOLVER CONFIRMACION AL KERNEL
                t_buffer *buffer_a_enviar = crear_buffer();
                cargar_string_al_buffer(buffer_a_enviar, nombre); //[nombre][pid]
                cargar_uint16_al_buffer(buffer_a_enviar, pid);
                t_paquete *a_enviar = crear_paquete(FIN_DE_EJECUCION_DE_IO, buffer_a_enviar);

                enviar_paquete(a_enviar, fd_kernel);

                destruir_paquete(a_enviar);                
            }
            else if (strcmp(operacion_a_realizar, "IO_STDOUT_WRITE") == 0){

            }
            else if (strcmp(operacion_a_realizar, "IO_FS_CREATE") == 0){

            }
            else if (strcmp(operacion_a_realizar, "IO_FS_DELETE") == 0){

            }
            else if (strcmp(operacion_a_realizar, "IO_FS_TRUNCATE") == 0){

            }
            else if (strcmp(operacion_a_realizar, "IO_FS_WRITE") == 0){

            }
            else if (strcmp(operacion_a_realizar, "IO_FS_READ") == 0){

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
