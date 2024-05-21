#include "../include/planificador.h"

// PLANIFICADOR DE LARGO PLAZO

void esperar (uint16_t* quantum){
    esperarMilisegundos(*quantum);
    log_trace(kernel_log_debug, "Fin quantum");
    _enviar_interrupcion_quantum();
}
void _enviar_interrupcion_quantum(){
    t_buffer* buff = crear_buffer();
    t_paquete* paq = crear_paquete(FIN_QUANTUM, buff);
    enviar_paquete(paq, fd_cpu_interrupt);
    destruir_paquete(paq);
}
void manejar_quantum(PCB* proceso){
    pthread_create(&hilo_quantum, NULL, (void *)esperar, (uint16_t*) &(proceso->quantum));
    pthread_detach(hilo_quantum);
}

PCB *_crear_pcb(uint16_t pid)
{
    PCB *pcb_creado = malloc(sizeof(PCB));
    pcb_creado->pc = 0;
    pcb_creado->pid = pid;
    pcb_creado->quantum = quantum;
    return pcb_creado;
}

void crear_proceso(char *path)
{

    t_buffer *buffer = crear_buffer();
    printf("Ingresaste: %s\n", path);
    uint16_t pid = asignar_pid();
    cargar_uint16_al_buffer(buffer, pid);
    cargar_string_al_buffer(buffer, path);

    t_paquete *a_enviar = crear_paquete(INICIAR_PROCESO, buffer); // [PID] [PATH]

    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);

    PCB *proceso_creado = _crear_pcb(pid);

    pthread_mutex_lock(&mutex_procesos);
    queue_push(procesos_new, proceso_creado);
    pthread_mutex_unlock(&mutex_procesos);

    log_info(kernel_logger, "Se crea el proceso %u en NEW", pid);

    sem_post(&proceso_creado_en_new_semaforo);
}

void mover_procesos_de_new_a_ready()
{
    while (1)
    {

        sem_wait(&proceso_creado_en_new_semaforo);
        sem_wait(&grado_multiprogramacion_semaforo);
        sem_wait(&planificacion_activa_semaforo);
        sem_post(&planificacion_activa_semaforo);

        pthread_mutex_lock(&mutex_procesos);

        PCB *proceso_movido = queue_pop(procesos_new);
        queue_push(procesos_ready, proceso_movido);

        procesos_en_programacion++;

        log_info(kernel_logger, "PID: %u - Estado Anterior: NEW - Estado Actual: READY", proceso_movido->pid);

        // TERMINAR ESTE LOG
        log_info(kernel_logger, "Cola Ready procesos_ready: [<LISTA DE PIDS>]");
        // puedo crear un buffer y ahi ir guardandno todos lo s pids

        pthread_mutex_unlock(&mutex_procesos);

        sem_post(&algun_ready);
    }
}

void iniciar_planificador_de_largo_plazo()
{
    pthread_t hilo_planificador_largo_plazo;
    pthread_create(&hilo_planificador_largo_plazo, NULL, (void *)mover_procesos_de_new_a_ready, NULL);
    pthread_detach(hilo_planificador_largo_plazo);
}

// PLANIFICADOR DE CORTO PLAZO

void mover_procesos_de_ready_a_excecute()
{   
    

    if (strcmp(algoritmo_planificacion, "VRR") != 0)
    {
        while (1)
        {
            sem_wait(&algun_ready);
            sem_wait(&cpu_vacia_semaforo);
            sem_wait(&planificacion_activa_semaforo);
            sem_post(&planificacion_activa_semaforo);
            if (procesos_excec->elements->elements_count == 0 && procesos_ready->elements->elements_count > 0)
            {
                pthread_mutex_lock(&mutex_procesos);
                PCB *proceso_movido = queue_pop(procesos_ready);
                queue_push(procesos_excec, proceso_movido);
                pthread_mutex_unlock(&mutex_procesos);

                t_buffer *buffer = crear_buffer();

                cargar_uint16_al_buffer(buffer, proceso_movido->pid);
                cargar_uint32_al_buffer(buffer, proceso_movido->pc);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.ax);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.bx);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.cx);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.dx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.eax);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.ebx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.ecx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.edx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.si);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.di);

                t_paquete *a_enviar = crear_paquete(PROCESO_A_EJECUTAR, buffer);

                log_info(kernel_logger, "PID: %u - Estado Anterior: READY - Estado Actual: EXCEC", proceso_movido->pid);
                enviar_paquete(a_enviar, fd_cpu_dispatch);
                if(strcmp(algoritmo_planificacion, "RR") == 0){
                manejar_quantum(proceso_movido);
                }
                // enviamos el proceso de ready a execute primero y luego lo enviamos a cpu
                destruir_paquete(a_enviar);
            }
            
        }
    }
    else if (strcmp(algoritmo_planificacion, "VRR") == 0)
    {
        while (1)
        {
            sem_wait(&algun_ready);
            sem_wait(&cpu_vacia_semaforo);
            sem_wait(&planificacion_activa_semaforo);
            sem_post(&planificacion_activa_semaforo);
            if (procesos_excec->elements->elements_count == 0 && procesos_ready_con_prioridad->elements->elements_count > 0)
            {
                pthread_mutex_lock(&mutex_procesos);
                PCB *proceso_movido = queue_pop(procesos_ready_con_prioridad);
                queue_push(procesos_excec, proceso_movido);
                pthread_mutex_unlock(&mutex_procesos);

                t_buffer *buffer = crear_buffer();

                cargar_uint16_al_buffer(buffer, proceso_movido->pid);
                cargar_uint32_al_buffer(buffer, proceso_movido->pc);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.ax);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.bx);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.cx);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.dx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.eax);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.ebx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.ecx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.edx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.si);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.di);

                t_paquete *a_enviar = crear_paquete(PROCESO_A_EJECUTAR, buffer);

                log_info(kernel_logger, "PID: %u - Estado Anterior: READY - Estado Actual: EXCEC", proceso_movido->pid);
                enviar_paquete(a_enviar, fd_cpu_dispatch);
                manejar_quantum(proceso_movido);
                // enviamos el proceso de ready a execute primero y luego lo enviamos a cpu
                destruir_paquete(a_enviar);
            }
            else if (procesos_excec->elements->elements_count == 0 && procesos_ready->elements->elements_count > 0)
            {
                pthread_mutex_lock(&mutex_procesos);
                PCB *proceso_movido = queue_pop(procesos_ready);
                queue_push(procesos_excec, proceso_movido);
                pthread_mutex_unlock(&mutex_procesos);

                t_buffer *buffer = crear_buffer();

                cargar_uint16_al_buffer(buffer, proceso_movido->pid);
                cargar_uint32_al_buffer(buffer, proceso_movido->pc);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.ax);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.bx);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.cx);
                cargar_uint8_al_buffer(buffer, proceso_movido->registros.dx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.eax);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.ebx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.ecx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.edx);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.si);
                cargar_uint32_al_buffer(buffer, proceso_movido->registros.di);

                t_paquete *a_enviar = crear_paquete(PROCESO_A_EJECUTAR, buffer);

                log_info(kernel_logger, "PID: %u - Estado Anterior: READY - Estado Actual: EXCEC", proceso_movido->pid);
                enviar_paquete(a_enviar, fd_cpu_dispatch);
                manejar_quantum(proceso_movido);
                // enviamos el proceso de ready a execute primero y luego lo enviamos a cpu
                destruir_paquete(a_enviar);
            }
            
        }
    }
}

void iniciar_planificador_de_corto_plazo()
{
    pthread_t hilo_planificador_corto_plazo;
    pthread_create(&hilo_planificador_corto_plazo, NULL, (void *)mover_procesos_de_ready_a_excecute, NULL);
    pthread_detach(hilo_planificador_corto_plazo);
}

void mover_de_excec_a_cola_bloqueado(char *nombre_de_la_io)
{
    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);

    for (int i = 0; i < contador_de_colas_bloqueados; i++)
    {

        if (strcmp(colas_bloqueados[i]->nombre, nombre_de_la_io) == 0)
        {
            pthread_mutex_lock(&mutex_procesos);
            PCB *proceso_movido = queue_pop(procesos_excec);
            queue_push(colas_bloqueados[i]->cola, proceso_movido);
            pthread_mutex_unlock(&mutex_procesos);
            log_info(kernel_logger, "PID: %u - Estado Anterior: EXCEC - Estado Actual: BLOQUEADO", proceso_movido->pid);
        }
    }

    sem_post(&cpu_vacia_semaforo);
    
}

void mover_a_io_si_hay_algun_proceso_encolado(char *nombre_io) // verificar si hay algun proceso en su cola de bloqueados, si hay, lo manda a
{
    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);
    for (int i = 0; i < contador_de_colas_bloqueados; i++)
    {

        if ((strcmp(colas_bloqueados[i]->nombre, nombre_io) == 0) && (colas_bloqueados[i]->cola->elements->elements_count > 0))
        {
            pthread_mutex_lock(&mutex_procesos);

            PCB *primer_pcb_de_la_cola = (PCB *)queue_peek(colas_bloqueados[i]->cola);
            // [nombre io][operacion][pid][unidades de trabajo]
            t_paquete *a_enviar_a_io = crear_paquete(TAREA, primer_pcb_de_la_cola->operacion_de_io_por_la_que_fue_bloqueado);

            enviar_paquete(a_enviar_a_io, colas_bloqueados[i]->fd);

            free(a_enviar_a_io);
            pthread_mutex_unlock(&mutex_procesos);

            break;
        }
    }
    
}

void mover_de_excec_a_ready()
{
    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);
    pthread_mutex_lock(&mutex_procesos);
    PCB *proceso_movido = queue_pop(procesos_excec);
    queue_push(procesos_ready, proceso_movido);
    pthread_mutex_unlock(&mutex_procesos);

    log_info(kernel_logger, "PID: %u - Desalojado por fin de Quantum", proceso_movido->pid);
    log_info(kernel_logger, "PID: %u - Estado Anterior: EXCEC - Estado Actual: READY", proceso_movido->pid);
    log_info(kernel_logger, "Cola Ready procesos_ready: [<LISTA DE PIDS>]");

    sem_post(&cpu_vacia_semaforo);
    sem_post(&algun_ready);
    
}

void avisarle_a_memoria_que_libere_recursos_de_proceso(uint16_t pid)
{
    t_buffer *buffer_fin_proceso = crear_buffer(); // [pid]
    cargar_uint16_al_buffer(buffer_fin_proceso, pid);
    t_paquete *a_enviar = crear_paquete(FINALIZAR_PROCESO, buffer_fin_proceso);

    enviar_paquete(a_enviar, fd_memoria);

    destruir_paquete(a_enviar);
}

void _mandar_de_excec_a_exit(char *motivo)
{
    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);
    pthread_mutex_lock(&mutex_procesos);
    PCB *proceso_movido = queue_pop(procesos_excec);
    queue_push(procesos_exit, proceso_movido);
    pthread_mutex_unlock(&mutex_procesos);

    avisarle_a_memoria_que_libere_recursos_de_proceso(proceso_movido->pid);
    // LIBERAR RECURSOS
    log_info(kernel_logger, "Finaliza el proceso %u - Motivo: %s", proceso_movido->pid, motivo);

    log_info(kernel_logger, "PID: %u - Estado Anterior: EXCEC - Estado Actual: EXIT", proceso_movido->pid);

    sem_post(&cpu_vacia_semaforo);
    sem_post(&grado_multiprogramacion_semaforo);
}

void bloquear_proceso_en_ejecucion_por_recurso (int index_cola){
    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);

    pthread_mutex_lock(&mutex_procesos);
    PCB *proceso_movido = queue_pop(procesos_excec);
    queue_push(recursos[index_cola]->cola_bloqueados_por_recursos, proceso_movido);
    pthread_mutex_unlock(&mutex_procesos);

    sem_post(&cpu_vacia_semaforo);
    log_info(kernel_logger, "PID: %u - Bloqueado por: %s", proceso_movido->pid, recursos[index_cola]->nombre);
    log_info(kernel_logger, "PID: %u - Estado Anterior: EXCEC - Estado Actual: BLOQ", proceso_movido->pid);
}

void desbloquear_proceso_bloqueado_por_recurso (int index_cola){
    sem_wait(&planificacion_activa_semaforo);
    sem_post(&planificacion_activa_semaforo);

    pthread_mutex_lock(&mutex_procesos);
    PCB *proceso_movido = queue_pop(recursos[index_cola]->cola_bloqueados_por_recursos);
    queue_push(procesos_ready, proceso_movido);
    pthread_mutex_unlock(&mutex_procesos);

    log_info(kernel_logger, "PID: %u - Desbloqueado ", proceso_movido->pid);
    log_info(kernel_logger, "PID: %u - Estado Anterior: BLOQ - Estado Actual: READY", proceso_movido->pid);
    log_info(kernel_logger, "Cola Ready procesos_ready: [<LISTA DE PIDS>]");

}