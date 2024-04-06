#include "../include/main.h"


int main() {
    inicializar_logger (&cpu_logger);
    inicializar_logger_debug (&cpu_log_debug);

    APP_config config_valores = cargar_configuracion_cpu();
    printf("Puerto Memoria: %d\n", config_valores.puerto_memoria); //linea solo para testear funcionamienito, dsp borrar
    
    log_info (cpu_logger,"IP_MEMORIA : %s", config_valores.ip_memoria );
    log_debug (cpu_log_debug,"PUERTO_MEMORIA : %d", config_valores.puerto_memoria );
    log_warning (cpu_log_debug,"puerto_escucha_dispatch: %d", config_valores.puerto_escucha_dispach );
    log_trace (cpu_log_debug,"ALGORITMO TLB : %s", config_valores.algoritmo_TLB );
    return 0;
}