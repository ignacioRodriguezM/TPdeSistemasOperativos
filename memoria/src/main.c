#include "../include/main.h"


int main() {
    //Inicializar memmoria
    inicializar_logger (&memoria_logger);
    inicializar_logger_debug (&memoria_log_debug);
    APP_config config_valores = cargar_configuracion_memoria();

    //iniciar servidor de memoria
    fd_memoria = iniciar_servidor (config_valores.puerto_escucha, memoria_logger, "MEMORIA INCIADA !!!");
   
    //Esperar conexion CPU
    log_info(memoria_logger, "Esperando a CPU...");
    fd_cpu = esperar_cliente (fd_memoria, memoria_logger, "CPU");

    //Esperar conexion KERNEL
    log_info(memoria_logger, "Esperando a KERNEL...");
    fd_kernel = esperar_cliente (fd_memoria, memoria_logger, "KERNEL");

    //Esperiar conexion de i/o
    log_info(memoria_logger, "Esperando a entrada_salida...");
    fd_enrada_salida = esperar_cliente (fd_memoria, memoria_logger, "ENTRADA SALIDA");

    //Finalizar Memoria

    return 0;
}