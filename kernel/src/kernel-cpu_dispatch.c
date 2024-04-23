#include "../include/kernel-cpu_dispatch.h"

void atender_kernel_cpu_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_cpu_dispatch);
        switch (cod_op)
        {
        case EXIT:

            break;
        case INVALID_RESOURCE:

            break;
        case INVALID_WRITE:

            break;
        case LLAMADA_A_IO:
            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
            char *nombre_interfaz = extraer_string_al_buffer(buffer_recibido);
            char *operacion_a_realizar = extraer_string_al_buffer(buffer_recibido);
            if (_chequear_la_io(nombre_interfaz, operacion_a_realizar))
            {
                log_trace(kernel_log_debug, "LLEGUE ACA");
            }
            else
            {
               
                // MANDAR A EXIT
            }

            destruir_buffer(buffer_recibido);
            break;
        case INTERRUPT:

            break;

        case -1:
            log_error(kernel_logger, "Desconexion de KERNEL-CPU_DISPATCH");
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de KERNEL-CPU_DISPATCH");
            break;
        }
    }
}

bool _chequear_la_io(char *nombre_interfaz, char *operacion)
{

    if (strcmp(operacion, "IO_GEN_SLEEP") == 0)
    {
        for (int i = 0; i < contador_de_colas_bloqueados; i++)
        {

            if (strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0)
            {
                if (colas_bloqueados[i]->conectado == true)
                {
                    if (colas_bloqueados[i]->tipo_interfaz == 0)
                    {
                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }
    else if (strcmp(operacion, "IO_STDIN_READ ") == 0)
    {
        for (int i = 0; i < contador_de_colas_bloqueados; i++)
        {

            if (strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0)
            {
                if (colas_bloqueados[i]->conectado == true)
                {
                    if (colas_bloqueados[i]->tipo_interfaz == 1)
                    {
                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }
    else if (strcmp(operacion, "IO_STDOUT_WRITE ") == 0)
    {
        for (int i = 0; i < contador_de_colas_bloqueados; i++)
        {

            if (strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0)
            {
                if (colas_bloqueados[i]->conectado == true)
                {
                    if (colas_bloqueados[i]->tipo_interfaz == 2)
                    {
                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }
    else
    {
        for (int i = 0; i < contador_de_colas_bloqueados; i++)
        {

            if (strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0)
            {
                if (colas_bloqueados[i]->conectado == true)
                {
                    if (colas_bloqueados[i]->tipo_interfaz == 3)
                    {
                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        return false;
    }
}
