#include "../include/main.h"


int main() {
    //Inicializar entrada salida
    inicializar_logger (&entrada_salida_logger, "entrada_salida_logs.log");
    inicializar_logger_debug (&entrada_salida_log_debug, "entrada_salida_debug.log");
    APP_config config_valores = cargar_configuracion_entrada_salida ();
    
    //Conectarse como cliente a memoria
    log_info(entrada_salida_logger, "Conectandose a memoria...");
    fd_memoria = crear_conexion (config_valores.ip_memoria, config_valores.puerto_memoria);
    log_info (entrada_salida_logger,"Conexion a memoria Exitosa" );

    //Conectarse como cliente a Kernel
    log_info(entrada_salida_logger, "Conectandose a kernel...");
    fd_kernel = crear_conexion (config_valores.ip_kernel, config_valores.puerto_kernel);
    log_info (entrada_salida_logger,"Conexion a kernel Exitosa" );

    return 0;
}