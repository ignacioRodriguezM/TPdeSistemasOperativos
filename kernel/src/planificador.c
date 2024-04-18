#include "../include/planificador.h"

void iniciar_planificador_de_corto_plazo (){
    
    if(strcmp(algoritmo_planificacion, "FIFO") == 0 ){
        _planificar_por_fifo ();
        log_info(kernel_logger,"Se ha iniciado la planificacion FIFO");
    }   
    

    if (strcmp (algoritmo_planificacion, "RR") == 0){
        _planificar_por_rr ();


    }

    if (strcmp(algoritmo_planificacion, "VRR") !=-1 ){
        _planificar_por_vrr ();


    }

}


void _mover_el_primero_de_lista_a_otra_lista (t_queue* fuente, t_queue* destino){
    void* element = queue_pop(fuente);
    queue_push(destino, element);
}


    //pthread_t hilo_pasaje_de_new_a_ready;
    //pthread_create(&hilo_pasaje_de_new_a_ready,NULL,(void*)_planificar_por_fifo,NULL);

    void _planificar_por_fifo (){ //creamos hilos para mover cada proceso a una cola distinta simultaneamente
    //muevo todos los procesos que esten en new y que entren en ready dependiendo del grado de multi
    
    while (planificacion_activa){
        bool first_process = false;
        if (procesos_ready->elements->elements_count == 0 ){
            first_process = true;
        }
        while  (procesos_en_programacion < grado_multiprogramacion){
            if (procesos_new->elements->elements_count > 0){
            _mover_el_primero_de_lista_a_otra_lista (procesos_new, procesos_ready);
            procesos_en_programacion++;
            }
        }
        if (first_process && (procesos_ready->elements->elements_count != 0)){
            t_buffer* buffer = crear_buffer();
            PCB* proceso_a_enviar = queue_peek(procesos_ready);
            cargar_choclo_al_buffer (buffer, proceso_a_enviar,sizeof(PCB));
            t_paquete* a_enviar = crear_paquete (PROCESO_A_EJECUTAR, buffer);
            enviar_paquete (a_enviar,fd_cpu_dispatch);
            destruir_paquete (a_enviar);

            _mover_el_primero_de_lista_a_otra_lista (procesos_ready, procesos_excec);
            
        }

        

    } 

    
}




void _planificar_por_rr (){
    while (planificacion_activa){
        
    }
}

void _planificar_por_vrr (){
    while (planificacion_activa){
        
    }
}

