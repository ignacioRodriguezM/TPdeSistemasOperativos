#include "../include/main.h"


int main() {
    //incializo kerne;
    inicializar_logger (&kernel_logger, "kernel_logs.log");
    inicializar_logger_debug (&kernel_log_debug, "kernel_debug.log");
    APP_config config_valores = cargar_configuracion_kernel();
    // POR SI NECESITO PASAR EL ARRAY DE STRINGS A ARRAY DE INTS: uint16_t* instancias_Recursos = convertirArrayCharAUInt16 (config_valores.instancias_recursos);
   
    //iniciar servidor de kernel
    fd_kernel = iniciar_servidor (config_valores.puerto_escucha, kernel_logger, "KERNEL INCIADO !!!");

    //Conectarse como cliente a memoria
    log_info(kernel_logger, "Conectandose a memoria...");
    fd_memoria = crear_conexion (config_valores.ip_memoria, config_valores.puerto_memoria);
    log_info(kernel_logger, "Conexion a memoria Exitosa");

    //Conectarse como cliente a cpu dispatch
    log_info(kernel_logger, "Conectandose a cpu dispatch...");
    fd_cpu_dispatch = crear_conexion (config_valores.ip_cpu, config_valores.puerto_cpu_dispatch);
    log_info(kernel_logger, "Conexion a cpu dispatch Exitosa");

    //Conectarse como cliente a cpu interrupt
    log_info(kernel_logger, "Conectandose a cpu interrupt...");
    fd_cpu_interrupt = crear_conexion (config_valores.ip_cpu, config_valores.puerto_cpu_interrupt);
    log_info(kernel_logger, "Conexion a cpu interrupt Exitosa");
    
    //Esperiar conexion de i/o
    log_info(kernel_logger, "Esperando a entrada_salida...");
    fd_entrada_salida = esperar_cliente (fd_kernel, kernel_logger, "ENTRADA SALIDA");


    return 0;
}