#include "../include/main.h"


int main() {
    inicializar_logger (&kernel_logger);
    inicializar_logger_debug (&kernel_log_debug);

    APP_config config_valores = cargar_configuracion_kernel();
    // Imprime los valores utilizando printf y los especificadores de formato adecuados
    printf("Puerto CPU Dispatch: %d\n", config_valores.puerto_cpu_dispatch); //linea solo para testear funcionamienito, dsp borrar
    
    return 0;
}