#include "../include/main.h"


int main() {
    inicializar_logger (&kernel_logger);
    inicializar_logger_debug (&kernel_log_debug);

    APP_config config_valores = cargar_configuracion_kernel();
    uint16_t* instancias_Recursos = convertirArrayCharAUInt16 (config_valores.instancias_recursos);
   
    log_trace (kernel_log_debug, "Recursos: %s|%s|%s",config_valores.recursos[0], config_valores.recursos[1], config_valores.recursos[2]);
    log_debug (kernel_log_debug, "Instancias Recursos: %s|%s|%s",config_valores.instancias_recursos  [0], config_valores.instancias_recursos  [1], config_valores.instancias_recursos  [2]);
    log_warning (kernel_log_debug,"Instancias Recursos como Int: %hu|%hu|%hu", instancias_Recursos[0], instancias_Recursos[1],  instancias_Recursos[2]);


    return 0;
}