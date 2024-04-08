#include "../include/main.h"


int main() {
    //Inicializar entrada salida
    inicializar_logger (&entrada_salida_logger);
    inicializar_logger_debug (&entrada_salida_log_debug);
    APP_config config_valores = cargar_configuracion_entrada_salida ();
    
    //Conectarse como cliente a memoria
    log_info(entrada_salida_logger, "Conectandose a memoria");
    fd_entrada_salida = crear_conexion (config_valores.ip_memoria, config_valores.puerto_memoria);


    //Conectarse como cliente a Kernel
    log_info(entrada_salida_logger, "Conectandose a kernel");
    fd_entrada_salida = crear_conexion (config_valores.ip_kernel, config_valores.puerto_kernel);

    return 0;
}