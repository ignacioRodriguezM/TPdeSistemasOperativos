#include "../include/kernel-cpu_dispatch.h"

void atender_kernel_cpu_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_cpu_dispatch);
        switch (cod_op)
        {
        case DESALOJO_POR_EXIT:

            _manejar_exit();

            break;

        case OUT_OF_MEMORY:
            _manejar_out_of_memory();
            break;

        case INVALID_WRITE:

            break;
        case LLAMADA_A_IO:

            _manejar_bloqueo();

            break;
        case DESALOJO:
            _manejar_desalojo();
            break;
        case WAIT_op:
            _manejar_wait();

            break;
        case SIGNAL_op:
            _manejar_signal();
            break;

        case INTERRUPTED_BY_USER:
            _manejar_interrupcion_de_usuario();
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

void extraer_y_actualizar_pcb_en_excecute(t_buffer *buffer_recibido)
{

    uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
    uint32_t pc = extraer_uint32_al_buffer(buffer_recibido);
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

void _manejar_desalojo()
{
    if (strcmp(algoritmo_planificacion, "VRR") == 0)
    {
        temporal_stop(timer_quantum);
        temporal_destroy(timer_quantum);
    }
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);

    mover_de_excec_a_ready();

    destruir_buffer(buffer_recibido);
}
void _manejar_exit()
{
    if (strcmp(algoritmo_planificacion, "FIFO") != 0)
    {
        pthread_cancel(hilo_quantum);
        if (strcmp(algoritmo_planificacion, "VRR") == 0)
        {
            temporal_stop(timer_quantum);
            temporal_destroy(timer_quantum);
        }
    }
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);

    _mandar_de_excec_a_exit("SUCCES");

    destruir_buffer(buffer_recibido);
}

void _manejar_interrupcion_de_usuario(){
    if (strcmp(algoritmo_planificacion, "FIFO") != 0)
    {
        pthread_cancel(hilo_quantum);
        if (strcmp(algoritmo_planificacion, "VRR") == 0)
        {
            temporal_stop(timer_quantum);
            temporal_destroy(timer_quantum);
        }
    }
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);

    _mandar_de_excec_a_exit("INTERRUPTED_BY_USER");

    destruir_buffer(buffer_recibido);
}

void _manejar_wait()
{
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);
    // [nombre_recurso]
    char *nombre_recurso_recibido = extraer_string_al_buffer(buffer_recibido);

    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);

    int i = 0;
    bool chequeo_si_alguna_coincide_nombre = true;
    while (recursos[i]->nombre != NULL)
    {
        if (strcmp(recursos[i]->nombre, nombre_recurso_recibido) == 0)
        {

            pthread_mutex_lock(&mutex_recursos);
            recursos[i]->instancias--;
            pthread_mutex_unlock(&mutex_recursos);

            log_info(kernel_log_debug, "SE RESTA UNA INSTANCIA AL RECURSO %s", nombre_recurso_recibido);

            chequeo_si_alguna_coincide_nombre = false;
            if (recursos[i]->instancias < 0)
            {
                if (strcmp(algoritmo_planificacion, "FIFO") != 0)
                {
                    pthread_cancel(hilo_quantum);
                    if (strcmp(algoritmo_planificacion, "VRR") == 0)
                    {
                        temporal_stop(timer_quantum);
                        uint16_t quantum_usado = temporal_gettime(timer_quantum);
                        if (quantum_usado != quantum)
                        {
                            PCB *pcb_que_ejecuto_menos_quantum = (PCB *)queue_peek(procesos_excec);
                            pcb_que_ejecuto_menos_quantum->quantum -= quantum_usado;
                        }
                        temporal_destroy(timer_quantum);
                    }
                }

                bloquear_proceso_en_ejecucion_por_recurso(i);
                free(nombre_recurso_recibido);
                destruir_buffer(buffer_recibido);
                return;
            }
            break;
        }
        i++;
    }
    if (chequeo_si_alguna_coincide_nombre)
    {
        // MANDAR A EXIT
        _mandar_de_excec_a_exit("INVALID_RESOURCE");
        // _LIBERAR RECURSOS USADOS FALTA!!!!
    }

    else
    {
        pthread_mutex_lock(&mutex_procesos);
        PCB *pcb_a_devolver_a_cpu = (PCB *)queue_pop(procesos_excec);
        queue_push(procesos_excec, pcb_a_devolver_a_cpu);
        pthread_mutex_unlock(&mutex_procesos);
    }

    free(nombre_recurso_recibido);
    destruir_buffer(buffer_recibido);
}

void _manejar_signal()
{
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);

    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);

    // [nombre_recurso]
    char *nombre_recurso_recibido = extraer_string_al_buffer(buffer_recibido);

    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);

    int i = 0;
    bool chequeo_si_alguna_coincide_nombre = true;
    while (recursos[i]->nombre != NULL)
    {
        if (strcmp(recursos[i]->nombre, nombre_recurso_recibido) == 0)
        {

            pthread_mutex_lock(&mutex_recursos);
            recursos[i]->instancias++;
            pthread_mutex_unlock(&mutex_recursos);

            log_info(kernel_log_debug, "SE SUMA UNA INSTANCIA AL RECURSO %s", nombre_recurso_recibido);

            chequeo_si_alguna_coincide_nombre = false;
            if (recursos[i]->instancias <= 0)
            {
                desbloquear_proceso_bloqueado_por_recurso(i);
            }
            break;
        }
        i++;
    }
    if (chequeo_si_alguna_coincide_nombre)
    {
        // MANDAR A EXIT
        _mandar_de_excec_a_exit("INVALID_RESOURCE");
        // LIBERAR RECURSOS USADOS FALTA!!!!
    }

    else
    {
        pthread_mutex_lock(&mutex_procesos);
        PCB *pcb_a_devolver_a_cpu = (PCB *)queue_pop(procesos_excec);
        queue_push(procesos_excec, pcb_a_devolver_a_cpu);
        pthread_mutex_unlock(&mutex_procesos);
    }

    free(nombre_recurso_recibido);
    destruir_buffer(buffer_recibido);
}
void _manejar_bloqueo()
{
    if (strcmp(algoritmo_planificacion, "FIFO") != 0)
    {
        pthread_cancel(hilo_quantum);
        if (strcmp(algoritmo_planificacion, "VRR") == 0)
        {
            temporal_stop(timer_quantum);
            uint16_t quantum_usado = temporal_gettime(timer_quantum);
            if (quantum_usado != quantum)
            {
                PCB *pcb_que_ejecuto_menos_quantum = (PCB *)queue_peek(procesos_excec);
                pcb_que_ejecuto_menos_quantum->quantum -= quantum_usado;
            }
            temporal_destroy(timer_quantum);
        }
    }

    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
    //[pid] [pc] [registros] [nombre_interfaz] [operacion]

    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);

    char *nombre_interfaz = extraer_string_al_buffer(buffer_recibido);
    char *operacion_a_realizar = extraer_string_al_buffer(buffer_recibido);
    // EXTRAIGO EL ELEMENTO DE EXCEC PERO SIN QUITARLO DE EXCEC

    if (_chequear_la_io(nombre_interfaz, operacion_a_realizar))
    {
        if (strcmp(operacion_a_realizar, "IO_GEN_SLEEP") == 0)
        {
            //[unidades_de_trabajo]
            uint8_t unidades_de_trabajo = extraer_uint8_al_buffer(buffer_recibido);

            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);

            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, unidades_de_trabajo);
            // [nombre io][operacion][pid][unidades de trabajo]
            for (int i = 0; i < contador_de_colas_bloqueados; i++)
            {

                if ((strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0) && (colas_bloqueados[i]->cola->elements->elements_count == 0))
                {
                    sem_wait(&planificacion_activa_semaforo);
                    sem_post(&planificacion_activa_semaforo);
                    // [nombre io][operacion][pid][unidades de trabajo]
                    t_paquete *a_enviar_a_io = crear_paquete(TAREA, pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado);

                    enviar_paquete(a_enviar_a_io, colas_bloqueados[i]->fd);

                    free(a_enviar_a_io);

                    break;
                }
            }

            mover_de_excec_a_cola_bloqueado(nombre_interfaz);
            log_info(kernel_logger, "PID: %u - Bloqueado por: INTERFAZ : %s", pcb_a_editar->pid, nombre_interfaz);
        }

        else if (strcmp(operacion_a_realizar, "IO_STDIN_READ") == 0)
        {
        }
        else if (strcmp(operacion_a_realizar, "IO_STDOUT_WRITE") == 0)
        {
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

        // HACER RESTO DE IFs
    }
    else
    {

        _mandar_de_excec_a_exit("INVALID_INTERFACE");
    }

    destruir_buffer(buffer_recibido);
}

void _manejar_out_of_memory()
{
    if (strcmp(algoritmo_planificacion, "FIFO") != 0)
    {
        pthread_cancel(hilo_quantum);
        if (strcmp(algoritmo_planificacion, "VRR") == 0)
        {
            temporal_stop(timer_quantum);
            temporal_destroy(timer_quantum);
        }
    }
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);
    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);

    _mandar_de_excec_a_exit("OUT_OF_MEMORY");

    destruir_buffer(buffer_recibido);
}
