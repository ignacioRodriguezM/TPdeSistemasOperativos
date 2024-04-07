#include "../include/main.h"


int main() {
    inicializar_logger (&kernel_logger);
    inicializar_logger_debug (&kernel_log_debug);

    APP_config config_valores = cargar_configuracion_kernel();
    // Imprime los valores utilizando printf y los especificadores de formato adecuados
    printf("Puerto CPU Dispatch: %d\n", config_valores.puerto_cpu_dispatch); //lineas solo para testear funcionamienito, dsp borrar
    
    log_trace (kernel_log_debug, "Recursos: %s|%s|%s",config_valores.recursos[0], config_valores.recursos[1], config_valores.recursos[2]);
    log_debug (kernel_log_debug, "Instancias Recursos: %s|%s|%s",config_valores.instancias_recursos  [0], config_valores.instancias_recursos  [1], config_valores.instancias_recursos  [2]);

    return 0;
}