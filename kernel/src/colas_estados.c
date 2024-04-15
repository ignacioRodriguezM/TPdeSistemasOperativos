#include "../include/colas_estados.h"

// int contarElementos(char **arreglo) {
//     int contador = 0;
    
//     // Iterar sobre el arreglo hasta encontrar un puntero nulo
//     while (arreglo[contador] != NULL) {
//         contador++;
//     }
    
//     return contador;
// }

void inicializar_colas (APP_config config_valores, int cantidad_recursos){
    t_queue *procesos_new = queue_create();
    t_queue *procesos_ready = queue_create();
    t_queue *procesos_excec = queue_create();
    t_queue *procesos_exit = queue_create();

    //CREAR COLAS DENTRO DE COLA BLOQUEADOS
        t_queue **procesos_bloqueados = malloc(cantidad_recursos * sizeof(t_queue*));
        if (procesos_bloqueados == NULL) {
        // Manejar el error si no se pudo asignar memoria
        printf("Error: No se pudo asignar memoria para el arreglo de colas bloqueadas\n");
        exit(EXIT_FAILURE);
    }
    
    //int cantidad_recursos = contarElementos(config_valores.recursos);

    // Crear cada cola dentro del arreglo
    for (int i = 0; i < cantidad_recursos; i++) {
        procesos_bloqueados[i] = queue_create();
    }

} 

// void finalizar_colas (t_queue colas, int cantidad_recursos){
//     queue_clean(procesos_new);
//     queue_destroy(procesos_new);
//     queue_clean(procesos_ready);
//     queue_destroy(procesos_ready);
//     queue_clean(procesos_excec);
//     queue_destroy(procesos_excec);
//     queue_clean(procesos_exit);
//     queue_destroy(procesos_exit);

//     //int cantidad_recursos = contarElementos(config_valores.recursos);

//     for (int i = 0; i < cantidad_recursos; i++){
//         queue_clean(procesos_bloqueados[i]);
//         queue_destroy(procesos_bloqueados[i]);
//     }
    
// } 
