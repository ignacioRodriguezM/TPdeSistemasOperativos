#include "../include/servicios_kernel.h"

int asignar_pid (){ //asigno un id al proceso que se crea, evita pid`s iguales si llegan 2 procesos
    int valor_pid;
    
    pthread_mutex_lock(&mutex_pid);
    valor_pid = identificador_PID;
    identificador_PID++;
    pthread_mutex_unlock(&mutex_pid);

    return valor_pid;
}