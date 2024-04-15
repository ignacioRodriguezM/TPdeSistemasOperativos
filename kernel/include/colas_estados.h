#ifndef COLAS_ESTADO_KERNEL_H_
#define COLAS_ESTADO_KERNEL_H_

#include "k_gestor.h"
#include "./configuracion_kernel.h"
#include <commons/collections/queue.h>

int contarElementos(char **arreglo) {
    int contador = 0;
    
    // Iterar sobre el arreglo hasta encontrar un puntero nulo
    while (arreglo[contador] != NULL) {
        contador++;
    }
    
    return contador;
}

void inicializar_colas (APP_config config_valores, int cantidad_recursos);
// void finalizar_colas (t_queue** colas, int cantidad_recursos);


#endif