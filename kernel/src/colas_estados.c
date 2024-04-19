#include "../include/colas_estados.h"

void inicializar_colas_sin_bloqueados (){
    procesos_new = queue_create();
    procesos_ready = queue_create();
    procesos_excec = queue_create();
    procesos_exit = queue_create();


} 

 void finalizar_colas_sin_bloqueados(){

    queue_destroy_and_destroy_elements(procesos_new);
    queue_destroy_and_destroy_elements(procesos_ready);
    queue_destroy_and_destroy_elements(procesos_excec);
    queue_destroy_and_destroy_elements(procesos_exit);
    
} 
