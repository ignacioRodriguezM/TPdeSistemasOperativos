#include "../include/colas_estados.h"

 int contarElementos(char **arreglo) {
     int contador = 0;
    
     // Iterar sobre el arreglo hasta encontrar un puntero nulo
    while (arreglo[contador] != NULL) {
         contador++;
    }
    
     return contador;
 }

void inicializar_colas (){
    procesos_new = queue_create();
    procesos_ready = queue_create();
    procesos_excec = queue_create();
    procesos_exit = queue_create();


    //CREAR COLAS DENTRO DE COLA BLOQUEADOS
        procesos_bloqueados = malloc(catidad_de_colas_bloqueados * sizeof(t_queue*));
        if (procesos_bloqueados == NULL) {
        // Manejar el error si no se pudo asignar memoria
        printf("Error: No se pudo asignar memoria para el arreglo de colas bloqueadas\n");
        exit(EXIT_FAILURE);
    }
    

    // Crear cada cola dentro del arreglo
    for (int i = 0; i < catidad_de_colas_bloqueados; i++) {
        procesos_bloqueados[i] = queue_create();
    }

} 

 void finalizar_colas (){

    queue_destroy(procesos_new);
    queue_destroy(procesos_ready);
    queue_destroy(procesos_excec);
    queue_destroy(procesos_exit);



    for (int i = 0; i < catidad_de_colas_bloqueados; i++){
         queue_destroy(procesos_bloqueados[i]);
    }
    
    free(procesos_bloqueados);
    
} 
