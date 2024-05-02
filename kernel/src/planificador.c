#include "../include/planificador.h"

// PLANIFICADOR DE LARGO PLAZO

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
}

void mover_procesos_de_new_a_ready()
{
    while (planificacion_activa == true)
    {
        if (procesos_en_programacion < grado_multiprogramacion && procesos_new->elements->elements_count > 0)
        {
            pthread_mutex_lock(&mutex_procesos);

            PCB *proceso_movido = queue_pop(procesos_new);
            queue_push(procesos_ready, proceso_movido);

            procesos_en_programacion++;

            log_info(kernel_logger, "PID: %u - Estado Anterior: NEW - Estado Actual: READY", proceso_movido->pid);

            // TERMINAR ESTE LOG
            log_info(kernel_logger, "Cola Ready procesos_ready: [<LISTA DE PIDS>]");
            // puedo crear un buffer y ahi ir guardandno todos lo s pids

            pthread_mutex_unlock(&mutex_procesos);
        }
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

    while (planificacion_activa == true)
    {
        if (procesos_excec->elements->elements_count == 0 && procesos_ready->elements->elements_count > 0)
        {
            pthread_mutex_lock(&mutex_procesos);
            PCB *proceso_movido = queue_pop(procesos_ready);
            queue_push(procesos_excec, proceso_movido);
            pthread_mutex_unlock(&mutex_procesos);

            t_buffer *buffer = crear_buffer();

            cargar_uint16_al_buffer(buffer, proceso_movido->pid);
            cargar_uint32_al_buffer(buffer, proceso_movido->pc);
            cargar_int8_al_buffer(buffer, proceso_movido->quantum);
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
            // enviamos el proceso de ready a execute primero y luego lo enviamos a cpu
            destruir_paquete(a_enviar);
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
    bool a = true;
    while (a)
    {
        while (planificacion_activa && a)
        {
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
            a = false;
        }
    }
}

void mover_a_io_si_hay_algun_proceso_encolado(char *nombre_io) //verificar si hay algun proceso en su cola de bloqueados, si hay, lo manda a 
{
    for(int i=0; i<contador_de_colas_bloqueados; i++){
        
        if ((strcmp(colas_bloqueados[i]->nombre, nombre_io) == 0 ) && (colas_bloqueados[i]->cola->elements->elements_count > 0 )){
            PCB* primer_pcb_de_la_cola = (PCB *)queue_peek(procesos_excec);
            // [nombre io][operacion][pid][unidades de trabajo]
            t_paquete *a_enviar_a_io = crear_paquete(TAREA, primer_pcb_de_la_cola->operacion_de_io_por_la_que_fue_bloqueado);

            enviar_paquete(a_enviar_a_io, colas_bloqueados[i]->fd);

            destruir_paquete(a_enviar_a_io);

            break;
        }
    }
}