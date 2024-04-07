#include "../include/main.h"


int main() {
    //Inicializo cpu
    inicializar_logger (&cpu_logger); //loggers obligatorios
    inicializar_logger_debug (&cpu_log_debug); //loggers para debug
    APP_config config_valores = cargar_configuracion_cpu();
   
    //Conectarse como cliente a memoria
    log_info(cpu_logger, "Conectandose a memoria...");
    fd_cpu = crear_conexion (config_valores.ip_memoria, config_valores.puerto_memoria);

    //iniciar servidor de cpu dispach
    fd_cpu = iniciar_servidor (config_valores.puerto_escucha_dispach, cpu_logger, "CPU DISPACH INCIADO !!!");
    //Esperiar conexion de kernel a dispach
    log_info(cpu_logger, "Esperando a kernel en dispach...");
    fd_kernel = esperar_cliente (fd_cpu, cpu_logger, "KERNEL EN DISPACH");

    return 0;
}