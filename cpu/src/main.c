#include "../include/main.h"


int main() {
    inicializar_logger (&cpu_logger);
    inicializar_logger (&cpu_log_debug);

    APP_config config_valores = cargar_configuracion_cpu();
    printf("Puerto Memoria: %d\n", config_valores.puerto_memoria); //linea solo para testear funcionamienito, dsp borrar
    
    return 0;
}