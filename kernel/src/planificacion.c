#include "../include/planificador.h"

//PLANIFICADOR DE LARGO PLAZO

PCB* _crear_pcb (uint16_t pid, uint32_t pc){
    PCB* pcb_creado = malloc(sizeof(PCB));
    pcb_creado->pc = pc;
    pcb_creado->pid = pid;
    pcb_creado->quantum = quantum;

    return pcb_creado;
}

void crear_proceso (char* path){

        t_buffer *buffer = crear_buffer();
            printf("Ingresaste: %s\n", path);
            uint16_t pid = asignar_pid();
            cargar_uint16_al_buffer(buffer, pid);
            cargar_string_al_buffer(buffer, path);

            t_paquete *a_enviar = crear_paquete(INICIAR_PROCESO, buffer); // [PID] [PATH]

            enviar_paquete(a_enviar, fd_memoria);
            destruir_paquete(a_enviar);
            
            //RECIBIR DE MEMORIA EL OK, Y RECIBIR EL PC
            uint32_t pc_recibido_de_memoria = 0;//recibir

            PCB* proceso_creado = _crear_pcb (pid, pc_recibido_de_memoria);

            pthread_mutex_lock(&mutex_procesos);
            queue_push(procesos_new, proceso_creado);
            pthread_mutex_unlock(&mutex_procesos);

            log_info(kernel_logger, "Se crea el proceso %u en NEW", pid);
    
}


void mover_procesos_de_new_a_ready (){
    while (planificacion_activa == true){
        if (procesos_en_programacion < grado_multiprogramacion){
            pthread_mutex_lock(&mutex_procesos);

            PCB* proceso_movido = queue_pop(procesos_new);
            queue_push(procesos_ready, proceso_movido);

            pthread_mutex_unlock(&mutex_procesos);

            log_info(kernel_logger, "PID: %u - Estado Anterior: NEW - Estado Actual: READY", proceso_movido->pid);


            //TERMINAR ESTE LOG
            log_info(kernel_logger, "Cola Ready procesos_ready: [<LISTA DE PIDS>]");

        }
    }
}
void mover_proceso_a_exit (){

}
void iniciar_planificador_de_largo_plazo (){
    pthread_t hilo_planificador_largo_plazo;
    pthread_create (&hilo_planificador_largo_plazo, NULL, (void*)mover_procesos_de_new_a_ready, NULL);
    pthread_detach(hilo_planificador_largo_plazo);
}

//PLANIFICADOR DE CORTO PLAZO