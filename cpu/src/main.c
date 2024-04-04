#include "../include/main.h"


int main() {
    APP_config config_valores = cargar_configuracion_cpu();
    printf("Puerto Memoria: %d\n", config_valores.puerto_memoria);
    return 0;
}