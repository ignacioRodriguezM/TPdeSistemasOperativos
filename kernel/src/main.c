#include "../include/main.h"


int main() {
    //incializo kerne;
    inicializar_logger (&kernel_logger);
    inicializar_logger_debug (&kernel_log_debug);
    APP_config config_valores = cargar_configuracion_kernel();
    // POR SI NECESITO PASAR EL ARRAY DE STRINGS A ARRAY DE INTS: uint16_t* instancias_Recursos = convertirArrayCharAUInt16 (config_valores.instancias_recursos);
   
   //Conectarse como cliente a memoria
    log_info(kernel_logger, "Conectandose a memoria...");
    fd_kernel = crear_conexion (config_valores.ip_memoria, config_valores.puerto_memoria);

    //Conectarse como cliente a cpu
    log_info(kernel_logger, "Conectandose a cpu dispach...");
    fd_kernel = crear_conexion (config_valores.ip_cpu, config_valores.puerto_cpu_dispatch);

    //iniciar servidor de kernel
    fd_kernel = iniciar_servidor (config_valores.puerto_escucha, kernel_logger, "KERNEL INCIADO !!!");
    //Esperiar conexion de i/o
    log_info(kernel_logger, "Esperando a entrada_salida...");
    fd_enrada_salida = esperar_cliente (fd_kernel, kernel_logger, "ENTRADA SALIDA");


    return 0;
}