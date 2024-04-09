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
    
    //Esperiar conexion de kernel a dispatch
    log_info(cpu_logger, "Esperando a kernel en dispatch...");
    fd_kernel_dispatch = esperar_cliente (fd_cpu_dispatch, cpu_logger, "KERNEL EN DISPACH");

    //Esperiar conexion de kernel a interrupt
    log_info(cpu_logger, "Esperando a kernel en interrupt...");
    fd_kernel_interrupt = esperar_cliente (fd_cpu_interrupt, cpu_logger, "KERNEL EN INTERRUPT");

    //Atender los mensajes de Kernel - Dispatch
    while (1) {
		int cod_op = recibir_operacion(fd_kernel_dispatch);
		switch (cod_op) {
            case MENSAJE:
                //
            case PAQUETE:
                //
            case -1:
                log_error(cpu_logger, "el Kernel se desconecto de dispatch. Terminando servidor");
                return EXIT_FAILURE;
            default:
                log_warning(cpu_logger,"Operacion desconocida. No quieras meter la pata");
                break;
            }
	}
    //Atender los mensajes de Kernel - Interrupt

    //Atender mensajes de Memoria





    return 0;
}