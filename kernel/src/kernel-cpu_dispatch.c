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

            pthread_t hilo_que_maneja_bloqueos;
            pthread_create(&hilo_que_maneja_bloqueos, NULL, (void *)_manejar_bloqueo, NULL);
            pthread_join(hilo_que_maneja_bloqueos, NULL);

            break;
        case INTERRUPT:

            break;

        case DESALOJO_POR_QUANTUM:

            _manejar_desalojo_por_quantum ();


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


void _manejar_desalojo_por_quantum (){
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
            uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
            uint32_t pc = extraer_uint32_al_buffer(buffer_recibido);
            int8_t quantum_rec = extraer_int8_al_buffer(buffer_recibido);
            uint8_t ax = extraer_uint8_al_buffer(buffer_recibido);
            uint8_t bx = extraer_uint8_al_buffer(buffer_recibido);
            uint8_t cx = extraer_uint8_al_buffer(buffer_recibido);
            uint8_t dx = extraer_uint8_al_buffer(buffer_recibido);
            uint32_t eax = extraer_uint32_al_buffer(buffer_recibido);
            uint32_t ebx = extraer_uint32_al_buffer(buffer_recibido);
            uint32_t ecx = extraer_uint32_al_buffer(buffer_recibido);
            uint32_t edx = extraer_uint32_al_buffer(buffer_recibido);
            uint32_t si = extraer_uint32_al_buffer(buffer_recibido);
            uint32_t di = extraer_uint32_al_buffer(buffer_recibido);

            // EXTRAIGO EL ELEMENTO DE EXCEC PERO SIN QUITARLO DE EXCEC
            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            if (pid != pcb_a_editar->pid)
            {
                log_error(kernel_log_debug, "ERROR, EL PID QUE SE EXTRAJO EN EL BUFFER QUE VINO DE CPU NO COINCIDE CON EL DE LA COLA EXCEC");
            }
            pcb_a_editar->pc = pc;
            pcb_a_editar->quantum = quantum;
            pcb_a_editar->registros.ax = ax;
            pcb_a_editar->registros.bx = bx;
            pcb_a_editar->registros.cx = cx;
            pcb_a_editar->registros.dx = dx;
            pcb_a_editar->registros.eax = eax;
            pcb_a_editar->registros.ebx = ebx;
            pcb_a_editar->registros.ecx = ecx;
            pcb_a_editar->registros.edx = edx;
            pcb_a_editar->registros.si = si;
            pcb_a_editar->registros.di = di;

            mover_de_excec_a_ready();
            
}
void _manejar_bloqueo()
{
    bool a = true;
    while (a)
    {
        while (planificacion_activa == true && a)
        {
            t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
            //[nombre_interfaz] [operacion] [unidades_de_trabajo] [pid] [pc] [quantum] [registros]

            char *nombre_interfaz = extraer_string_al_buffer(buffer_recibido);
            char *operacion_a_realizar = extraer_string_al_buffer(buffer_recibido);
            if (_chequear_la_io(nombre_interfaz, operacion_a_realizar))
            {
                if (strcmp(operacion_a_realizar, "IO_GEN_SLEEP") == 0)
                {

                    uint8_t unidades_de_trabajo = extraer_uint8_al_buffer(buffer_recibido);
                    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
                    uint32_t pc = extraer_uint32_al_buffer(buffer_recibido);
                    int8_t quantum_rec = extraer_int8_al_buffer(buffer_recibido);
                    uint8_t ax = extraer_uint8_al_buffer(buffer_recibido);
                    uint8_t bx = extraer_uint8_al_buffer(buffer_recibido);
                    uint8_t cx = extraer_uint8_al_buffer(buffer_recibido);
                    uint8_t dx = extraer_uint8_al_buffer(buffer_recibido);
                    uint32_t eax = extraer_uint32_al_buffer(buffer_recibido);
                    uint32_t ebx = extraer_uint32_al_buffer(buffer_recibido);
                    uint32_t ecx = extraer_uint32_al_buffer(buffer_recibido);
                    uint32_t edx = extraer_uint32_al_buffer(buffer_recibido);
                    uint32_t si = extraer_uint32_al_buffer(buffer_recibido);
                    uint32_t di = extraer_uint32_al_buffer(buffer_recibido);

                    // EXTRAIGO EL ELEMENTO DE EXCEC PERO SIN QUITARLO DE EXCEC
                    PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);

                    if (pid != pcb_a_editar->pid)
                    {
                        log_error(kernel_log_debug, "ERROR, EL PID QUE SE EXTRAJO EN EL BUFFER QUE VINO DE CPU NO COINCIDE CON EL DE LA COLA EXCEC");
                    }
                    pcb_a_editar->pc = pc;
                    pcb_a_editar->quantum = quantum;
                    pcb_a_editar->registros.ax = ax;
                    pcb_a_editar->registros.bx = bx;
                    pcb_a_editar->registros.cx = cx;
                    pcb_a_editar->registros.dx = dx;
                    pcb_a_editar->registros.eax = eax;
                    pcb_a_editar->registros.ebx = ebx;
                    pcb_a_editar->registros.ecx = ecx;
                    pcb_a_editar->registros.edx = edx;
                    pcb_a_editar->registros.si = si;
                    pcb_a_editar->registros.di = di;
                    pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();
                    cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
                    cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
                    cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pid);
                    cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, unidades_de_trabajo);
                    // [nombre io][operacion][pid][unidades de trabajo]
                    for (int i = 0; i < contador_de_colas_bloqueados; i++)
                    {

                        if ((strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0) && (colas_bloqueados[i]->cola->elements->elements_count == 0))
                        {

                            // [nombre io][operacion][pid][unidades de trabajo]
                            t_paquete *a_enviar_a_io = crear_paquete(TAREA, pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado);

                            enviar_paquete(a_enviar_a_io, colas_bloqueados[i]->fd);

                            free(a_enviar_a_io);
                            break;
                        }
                    }

                    mover_de_excec_a_cola_bloqueado(nombre_interfaz);
                    log_info(kernel_logger, "PID: %u - Bloqueado por: INTERFAZ : %s", pid, nombre_interfaz);
                }

                if (strcmp(operacion_a_realizar, "IO_STDIN_READ") == 0)
                {
                }
                if (strcmp(operacion_a_realizar, "IO_STDOUT_WRITE") == 0)
                {
                }

                // HACER RESTO DE IFs
            }
            else
            {

                // MANDAR A EXIT
            }

            destruir_buffer(buffer_recibido);
            a = false;
        }
    }
}