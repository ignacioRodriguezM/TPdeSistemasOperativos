#include "../include/semaforos.h"
void iniciar_semaforos_de_recursos(char** nombre_recursos, int* instancias_recursos){

    int cantidad = 0;
    while(nombre_recursos[cantidad] != NULL){
        cantidad++;
        
    }
    
    recursos = malloc(sizeof(Recursos*) * cantidad);

    for(int i=0; i < cantidad; i++){
        recursos[i] = malloc(sizeof(Recursos));
        recursos[i]->nombre = nombre_recursos[i];
        recursos[i]->instancias = instancias_recursos;
        recursos[i]->cola_bloqueados_por_recursos = queue_create();
    }

    //free(nombre_recursos);
}

