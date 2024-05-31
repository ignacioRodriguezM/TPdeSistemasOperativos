#include "../include/main.h"

int main (){

    // Inicializo cpu
    inicializar_logger(&cpu_logger, "cpu_logs.log");           // loggers obligatorios
    inicializar_logger_debug(&cpu_log_debug, "cpu_debug.log"); // loggers para debug
    APP_config config_valores = cargar_configuracion_cpu();
    aviso_de_interrupt = false;

    inicializar_diccionarios();

    // iniciar servidor de cpu dispach
    fd_cpu_dispatch = iniciar_servidor(config_valores.puerto_escucha_dispatch, cpu_logger, "CPU DISPACH INCIADO !!!");

    // iniciar servidor dde cpu interrupt
    fd_cpu_interrupt = iniciar_servidor(config_valores.puerto_escucha_interrupt, cpu_logger, "CPU INTERRUPT INCIADO !!!");

    // Esperiar conexion de kernel a dispatch
    log_info(cpu_logger, "Esperando a kernel en dispatch...");
    fd_kernel_dispatch = esperar_cliente(fd_cpu_dispatch, cpu_logger, "KERNEL EN DISPACH");

    // Esperiar conexion de kernel a interrupt
    log_info(cpu_logger, "Esperando a kernel en interrupt...");
    fd_kernel_interrupt = esperar_cliente(fd_cpu_interrupt, cpu_logger, "KERNEL EN INTERRUPT");

    // Conectarse como cliente a memoria
    log_info(cpu_logger, "Conectandose a memoria...");
    fd_memoria = crear_conexion(config_valores.ip_memoria, config_valores.puerto_memoria);
    log_info(cpu_logger, "Conexion a memoria Exitosa");

    // Atender los mensajes de Kernel - Dispatch
    pthread_t hilo_kernel_dispatch;
    pthread_create(&hilo_kernel_dispatch, NULL, (void *)atender_cpu_kernel_dispatch, NULL);
    pthread_detach(hilo_kernel_dispatch);

    // Atender los mensajes de Kernel - Interrupt
    pthread_t hilo_kernel_interrupt;
    pthread_create(&hilo_kernel_interrupt, NULL, (void *)atender_cpu_kernel_interrupt, NULL);
    pthread_join(hilo_kernel_interrupt, NULL);



/*
    // Atender mensajes de Memoria
    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void *)atender_cpu_memoria, NULL);
    pthread_join(hilo_memoria, NULL);
*/


    return 0;
}