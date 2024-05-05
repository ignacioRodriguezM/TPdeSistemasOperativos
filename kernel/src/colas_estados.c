#include "../include/colas_estados.h"

void inicializar_colas_sin_bloqueados()
{
    procesos_new = queue_create();
    procesos_ready = queue_create();
    procesos_excec = queue_create();
    procesos_exit = queue_create();
    
}

void finalizar_colas_sin_bloqueados()
{

    queue_destroy(procesos_new);
    queue_destroy(procesos_ready);
    queue_destroy(procesos_excec);
    queue_destroy(procesos_exit);
}
