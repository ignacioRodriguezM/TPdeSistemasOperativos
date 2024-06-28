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
bool chequear_la_io_en_colas(char *nombre_interfaz, int tipo_interfaz)
{
    for (int i = 0; i < contador_de_colas_bloqueados; i++)
    {
        if (strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0)
        {
            if (colas_bloqueados[i]->conectado == true)
            {
                if (colas_bloqueados[i]->tipo_interfaz == tipo_interfaz)
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
bool _chequear_la_io(char *nombre_interfaz, char *operacion)
{
    if (strcmp(operacion, "IO_GEN_SLEEP") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 0);
    }
    else if (strcmp(operacion, "IO_STDIN_READ") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 1);
    }
    else if (strcmp(operacion, "IO_STDOUT_WRITE") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 2);
    }
    else if (strcmp(operacion, "IO_FS_CREATE") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 3);
    }
    else if (strcmp(operacion, "IO_FS_DELETE") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 3);
    }
    else if (strcmp(operacion, "IO_FS_TRUNCATE") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 3);
    }
    else if (strcmp(operacion, "IO_FS_READ") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 3);
    }
    else if (strcmp(operacion, "IO_FS_WRITE") == 0)
    {
        return chequear_la_io_en_colas(nombre_interfaz, 3);
    }

    return false;
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

void _manejar_interrupcion_de_usuario()
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

    _mandar_de_excec_a_exit("INTERRUPTED_BY_USER");

    destruir_buffer(buffer_recibido);
}

void agregar_recurso_en_pcb(PCB *pcb, const char *nombre_recurso)
{
    // Copia del nombre del recurso
    char *nombre_copiado = malloc(strlen(nombre_recurso) + 1);
    strcpy(nombre_copiado, nombre_recurso);

    if (pcb->cantidad_recursos_asignados == 0)
    {
        pcb->cantidad_recursos_asignados = 1;
        pcb->recursos_asignados = malloc(sizeof(char *));
        pcb->recursos_asignados[0] = nombre_copiado;
    }
    else
    {
        pcb->cantidad_recursos_asignados++;
        pcb->recursos_asignados = realloc(pcb->recursos_asignados, pcb->cantidad_recursos_asignados * sizeof(char *));
        pcb->recursos_asignados[pcb->cantidad_recursos_asignados - 1] = nombre_copiado;
    }
}

void bloquear_por_recurso(Recursos *un_recurso)
{
    pthread_mutex_lock(&mutex_procesos);

    PCB *pcb_de_cpu = (PCB *)queue_pop(procesos_excec);
    queue_push(un_recurso->cola_bloqueados_por_recursos, pcb_de_cpu);

    pthread_mutex_unlock(&mutex_procesos);

    log_info(kernel_logger, "PID: <%u> - Estado Anterior: <EXCECUTE> - Estado Actual: <BLOQUEADO>", pcb_de_cpu->pid);
    log_info(kernel_logger, "PID: <%u> - Bloqueado por: <%s>", pcb_de_cpu->pid, un_recurso->nombre);
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
    bool se_bloqueo = false;
    while (recursos[i]->nombre != NULL)
    {
        if (strcmp(recursos[i]->nombre, nombre_recurso_recibido) == 0)
        {

            pthread_mutex_lock(&mutex_recursos);
            recursos[i]->instancias--;
            pthread_mutex_unlock(&mutex_recursos);

            log_info(kernel_log_debug, "SE RESTA UNA INSTANCIA AL RECURSO %s", nombre_recurso_recibido);

            pthread_mutex_lock(&mutex_procesos);
            PCB *pcb_de_cpu = (PCB *)queue_peek(procesos_excec);
            agregar_recurso_en_pcb(pcb_de_cpu, nombre_recurso_recibido);
            pthread_mutex_unlock(&mutex_procesos);

            chequeo_si_alguna_coincide_nombre = false;
            if (recursos[i]->instancias < 0)
            {
                se_bloqueo = true;
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

                bloquear_por_recurso(recursos[i]);
            }
            break;
        }
        i++;
    }
    if (chequeo_si_alguna_coincide_nombre)
    {
        // MANDAR A EXIT
        _mandar_de_excec_a_exit("INVALID_RESOURCE");
    }

    if (se_bloqueo || chequeo_si_alguna_coincide_nombre)
    {
        // hacer funcion enviar mensaje !!!!
        t_buffer *buffer = crear_buffer();
        char *respuesta = "BLOQUEADO";
        cargar_string_al_buffer(buffer, respuesta);
        t_paquete *paq = crear_paquete(RESPUESTA_RECURSO, buffer);
        enviar_paquete(paq, fd_cpu_dispatch);
        destruir_paquete(paq);
        if (se_bloqueo)
        {
            sem_wait(&planificacion_activa_semaforo);
            sem_post(&planificacion_activa_semaforo);
            sem_post(&cpu_vacia_semaforo);
        }
    }
    if (!se_bloqueo)
    {
        // hacer funcion enviar mensaje !!!!
        t_buffer *buffer = crear_buffer();
        char *respuesta = "FUNCIONO";
        cargar_string_al_buffer(buffer, respuesta);
        t_paquete *paq = crear_paquete(RESPUESTA_RECURSO, buffer);
        enviar_paquete(paq, fd_cpu_dispatch);
        destruir_paquete(paq);
    }

    free(nombre_recurso_recibido);
    destruir_buffer(buffer_recibido);
}
// Función para eliminar un elemento del array
void deleteElement(char **array, int *size, int index)
{
    // Verificar que el índice esté en el rango válido
    if (index < 0 || index >= *size)
    {
        printf("Índice fuera de rango\n");
        return;
    }
    // Liberar la memoria del elemento a eliminar
    free(array[index]);
    // Mover los elementos posteriores una posición hacia adelante
    for (int i = index; i < *size - 1; i++)
    {
        array[i] = array[i + 1];
    }
    // Disminuir el tamaño del array
    (*size)--;
    // Opcional: Poner NULL en la última posición
    array[*size] = NULL;
}

void liberar_si_tenia_asignado(PCB *pcb, char *nombre_interfaz)
{
    for (int i = 0; i < pcb->cantidad_recursos_asignados; i++)
    {
        if (strcmp(pcb->recursos_asignados[i], nombre_interfaz) == 0)
        {
            deleteElement(pcb->recursos_asignados, &(pcb->cantidad_recursos_asignados), i);
            return;
        }
    }
}
void _manejar_signal()
{
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu_dispatch);

    extraer_y_actualizar_pcb_en_excecute(buffer_recibido);

    // [nombre_recurso]
    char *nombre_recurso_recibido = extraer_string_al_buffer(buffer_recibido);

    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);

    bool chequeo_si_alguna_coincide_nombre = true;
    bool se_bloqueo = false;
    for (int i = 0; i < cantidad_de_recursos; i++)
    {
        if (strcmp(recursos[i]->nombre, nombre_recurso_recibido) == 0)
        {

            pthread_mutex_lock(&mutex_recursos);
            recursos[i]->instancias++;
            pthread_mutex_unlock(&mutex_recursos);

            pthread_mutex_lock(&mutex_procesos);
            PCB *pcb_de_cpu = (PCB *)queue_peek(procesos_excec);
            liberar_si_tenia_asignado(pcb_de_cpu, nombre_recurso_recibido);
            pthread_mutex_unlock(&mutex_procesos);

            log_info(kernel_log_debug, "SE SUMA UNA INSTANCIA AL RECURSO %s", nombre_recurso_recibido);

            chequeo_si_alguna_coincide_nombre = false;
            if (recursos[i]->instancias <= 0)
            {
                desbloquear_proceso_bloqueado_por_recurso(i);
            }
            break;
        }
    }
    if (chequeo_si_alguna_coincide_nombre)
    {
        // MANDAR A EXIT
        _mandar_de_excec_a_exit("INVALID_RESOURCE");
        se_bloqueo = true;
        // LIBERAR RECURSOS USADOS FALTA!!!!
    }

    if (se_bloqueo)
    {
        // hacer funcion enviar mensaje !!!!
        t_buffer *buffer = crear_buffer();
        char *respuesta = "BLOQUEADO";
        cargar_string_al_buffer(buffer, respuesta);
        t_paquete *paq = crear_paquete(RESPUESTA_RECURSO, buffer);
        enviar_paquete(paq, fd_cpu_dispatch);
        destruir_paquete(paq);
    }
    if (!se_bloqueo)
    {
        // hacer funcion enviar mensaje !!!!
        t_buffer *buffer = crear_buffer();
        char *respuesta = "FUNCIONO";
        cargar_string_al_buffer(buffer, respuesta);
        t_paquete *paq = crear_paquete(RESPUESTA_RECURSO, buffer);
        enviar_paquete(paq, fd_cpu_dispatch);
        destruir_paquete(paq);
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

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);
        }

        else if (strcmp(operacion_a_realizar, "IO_STDIN_READ") == 0)
        {

            uint8_t tamanio_total_a_escribir = extraer_uint8_al_buffer(buffer_recibido);
            uint8_t cantidad_de_direccions = extraer_uint8_al_buffer(buffer_recibido);

            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);

            //[tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [FALTA DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [FALTA DATO] .....
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tamanio_total_a_escribir);
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, cantidad_de_direccions);

            for (int i = 0; i < cantidad_de_direccions; i++)
            {
                uint8_t tam_a_escribir_por_pagina = extraer_uint8_al_buffer(buffer_recibido);
                uint16_t numero_de_pagina = extraer_uint16_al_buffer(buffer_recibido);
                uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);

                cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tam_a_escribir_por_pagina);
                cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, numero_de_pagina);
                cargar_uint32_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, desplazamiento);
            }

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);
        }
        else if (strcmp(operacion_a_realizar, "IO_STDOUT_WRITE") == 0)
        {

            uint8_t tamanio_total_a_leer = extraer_uint8_al_buffer(buffer_recibido);
            uint8_t cantidad_de_direccions = extraer_uint8_al_buffer(buffer_recibido);

            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);

            //[tamanio registro datos] [Cantidad] [TAM_A_leer] [MARCO] [DESPLAZAMIENTO]  .. [TAM_A_leer] [MARCO] [DESPLAZAMIENTO] .....
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tamanio_total_a_leer);
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, cantidad_de_direccions);

            for (int i = 0; i < cantidad_de_direccions; i++)
            {
                uint8_t tam_a_leer_por_pagina = extraer_uint8_al_buffer(buffer_recibido);
                uint16_t numero_de_pagina = extraer_uint16_al_buffer(buffer_recibido);
                uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);

                cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tam_a_leer_por_pagina);
                cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, numero_de_pagina);
                cargar_uint32_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, desplazamiento);
            }

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);
        }
        else if (strcmp(operacion_a_realizar, "IO_FS_CREATE") == 0)
        {
            char *nombre_del_archivo = extraer_string_al_buffer(buffer_recibido);

            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_del_archivo);

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);
        }
        else if (strcmp(operacion_a_realizar, "IO_FS_DELETE") == 0)
        {
            char *nombre_del_archivo = extraer_string_al_buffer(buffer_recibido);

            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_del_archivo);

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);

        }
        else if (strcmp(operacion_a_realizar, "IO_FS_TRUNCATE") == 0)
        {
            char *nombre_del_archivo = extraer_string_al_buffer(buffer_recibido);

            uint8_t tamanio = extraer_uint8_al_buffer(buffer_recibido);

            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_del_archivo);


            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tamanio);

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);

        }
        else if (strcmp(operacion_a_realizar, "IO_FS_WRITE") == 0)
        {
            //[nombre_archivo] [puntero archivo] [tam_total]
            // [cantidad] [tam_parcial] [marco] [des] [tam_parcial] [marco] [des] ....
            char *nombre_del_archivo = extraer_string_al_buffer(buffer_recibido);
            uint16_t puntero = extraer_uint16_al_buffer(buffer_recibido);
            uint8_t tamanio_a_escribir = extraer_uint8_al_buffer(buffer_recibido);
            uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);


            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_del_archivo);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, puntero);
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tamanio_total);
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, cantidad_de_direcciones);

            for(int i=0; i<cantidad_de_direcciones; i++)
            {
                uint8_t tam_a_escribir_o_leer_por_pagina = extraer_uint8_al_buffer(buffer_recibido);
                uint16_t numero_de_pagina = extraer_uint16_al_buffer(buffer_recibido);
                uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);

                cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tam_a_escribir_o_leer_por_pagina);
                cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, numero_de_pagina);
                cargar_uint32_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, desplazamiento);
            }

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);
            
        }
        else if (strcmp(operacion_a_realizar, "IO_FS_READ") == 0)
        {
            //[nombre_archivo] [puntero archivo] [tam_total]
            // [cantidad] [tam_parcial] [marco] [des] [tam_parcial] [marco] [des] ....
            char *nombre_del_archivo = extraer_string_al_buffer(buffer_recibido);
            uint16_t puntero = extraer_uint16_al_buffer(buffer_recibido);
            uint8_t tamanio_total = extraer_uint8_al_buffer(buffer_recibido);
            uint8_t cantidad_de_direcciones = extraer_uint8_al_buffer(buffer_recibido);


            PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);
            pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado = crear_buffer();

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_interfaz);
            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, operacion_a_realizar);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, pcb_a_editar->pid);

            cargar_string_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, nombre_del_archivo);
            cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, puntero);
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tamanio_total);
            cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, cantidad_de_direcciones);

            for(int i=0; i<cantidad_de_direcciones; i++)
            {
                uint8_t tam_a_escribir_o_leer_por_pagina = extraer_uint8_al_buffer(buffer_recibido);
                uint16_t numero_de_pagina = extraer_uint16_al_buffer(buffer_recibido);
                uint32_t desplazamiento = extraer_uint32_al_buffer(buffer_recibido);

                cargar_uint8_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, tam_a_escribir_o_leer_por_pagina);
                cargar_uint16_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, numero_de_pagina);
                cargar_uint32_al_buffer(pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado, desplazamiento);
            }

            mandar_a_io_o_cola_bloqueados(nombre_interfaz);
        }
    }
    else
    {
        free(nombre_interfaz);
        free(operacion_a_realizar);
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
void mandar_a_io_o_cola_bloqueados(char *nombre_interfaz)
{

    PCB *pcb_a_editar = (PCB *)queue_peek(procesos_excec);

    for (int i = 0; i < contador_de_colas_bloqueados; i++)
    {

        if ((strcmp(colas_bloqueados[i]->nombre, nombre_interfaz) == 0) && (colas_bloqueados[i]->cola->elements->elements_count == 0))
        {
            sem_wait(&planificacion_activa_semaforo);
            sem_post(&planificacion_activa_semaforo);
            // [nombre io][operacion][pid]
            t_paquete *a_enviar_a_io = crear_paquete(TAREA, pcb_a_editar->operacion_de_io_por_la_que_fue_bloqueado);

            enviar_paquete(a_enviar_a_io, colas_bloqueados[i]->fd);

            free(a_enviar_a_io);

            break;
        }
    }

    mover_de_excec_a_cola_bloqueado(nombre_interfaz);
    log_info(kernel_logger, "PID: %u - Bloqueado por: INTERFAZ : %s", pcb_a_editar->pid, nombre_interfaz);
}