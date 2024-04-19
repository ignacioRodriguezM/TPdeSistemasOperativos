#include "../include/colas_estados.h"

void inicializar_colas (){
    procesos_new = queue_create();
    procesos_ready = queue_create();
    procesos_excec = queue_create();
    procesos_exit = queue_create();


} 

 void finalizar_colas (){

    queue_destroy(procesos_new);
    queue_destroy(procesos_ready);
    queue_destroy(procesos_excec);
    queue_destroy(procesos_exit);
    
} 
