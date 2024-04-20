#include "../include/servicios_kernel.h"

uint16_t asignar_pid (){ //asigno un id al proceso que se crea, evita pid`s iguales si llegan 2 procesos
    uint16_t valor_pid;
    
    pthread_mutex_lock(&mutex_pid);
    valor_pid = identificador_PID;
    identificador_PID++;
    pthread_mutex_unlock(&mutex_pid);

    return valor_pid;
}

