#include "../include/main.h"


int main() {
    APP_config config_valores = cargar_configuracion_kernel();
    // Imprime los valores utilizando printf y los especificadores de formato adecuados
    printf("Puerto CPU Dispatch: %d\n", config_valores.puerto_cpu_dispatch);
    printf("Algoritmo de planificación: %s\n", config_valores.algoritmo_planificacion);    
    printf("IP de Memoria: %s\n", config_valores.ip_memoria);
    
    return 0;
}