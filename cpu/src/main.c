#include "../include/main.h"


int main() {
    //Inicializo cpu
    inicializar_logger (&cpu_logger); //loggers obligatorios
    inicializar_logger_debug (&cpu_log_debug); //loggers para debug
    APP_config config_valores = cargar_configuracion_cpu();
   
    //iniciar servidor de cpu dispach
    fd_cpu_dispatch = iniciar_servidor (config_valores.puerto_escucha_dispatch, cpu_logger, "CPU DISPACH INCIADO !!!");

    //iniciar servidor dde cpu interrupt
    fd_cpu_interrupt = iniciar_servidor (config_valores.puerto_escucha_interrupt, cpu_logger, "CPU INTERRUPT INCIADO !!!");

    //Conectarse como cliente a memoria
    log_info(cpu_logger, "Conectandose a memoria...");
    fd_memoria = crear_conexion (config_valores.ip_memoria, config_valores.puerto_memoria);
    log_info(cpu_logger, "Conexion a memoria Exitosa");
    
    //Esperiar conexion de kernel a dispach
    log_info(cpu_logger, "Esperando a kernel en dispach...");
    fd_kernel_dispatch = esperar_cliente (fd_cpu_dispatch, cpu_logger, "KERNEL EN DISPACH");

    //Esperiar conexion de kernel a interrupt
    log_info(cpu_logger, "Esperando a kernel en interrupt...");
    fd_kernel_interrupt = esperar_cliente (fd_cpu_interrupt, cpu_logger, "KERNEL EN INTERRUPT");

    return 0;
}