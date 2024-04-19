#include "../include/main.h"


int main() {
    //Inicializar memmoria
    inicializar_logger (&memoria_logger, "memoria_logs.log");
    inicializar_logger_debug (&memoria_log_debug, "memoria_debug.log");
    APP_config config_valores = cargar_configuracion_memoria();
    
    _asignar_valores_a_variables_globales_de_memoria(config_valores);

    //iniciar servidor de memoria
    fd_memoria = iniciar_servidor (config_valores.puerto_escucha, memoria_logger, "MEMORIA INCIADA !!!");


    
    //Esperiar conexion de i/o
    log_info(memoria_logger, "Esperando a entrada_salida...");
    fd_entrada_salida = esperar_cliente (fd_memoria, memoria_logger, "ENTRADA SALIDA");


    //Esperar conexion KERNEL
    log_info(memoria_logger, "Esperando a KERNEL...");
    fd_kernel = esperar_cliente (fd_memoria, memoria_logger, "KERNEL");

    //Esperar conexion CPU
    log_info(memoria_logger, "Esperando a CPU...");
    fd_cpu = esperar_cliente (fd_memoria, memoria_logger, "CPU");

    
    //Atender los mensajes de CPU
    pthread_t hilo_cpu;
    pthread_create(&hilo_cpu, NULL, (void*) atender_memoria_cpu, NULL);
    pthread_detach (hilo_cpu);

    //Atender los mensajes de KERNEL
    pthread_t hilo_kernel;
    pthread_create(&hilo_kernel, NULL, (void*) atender_memoria_kernel, NULL);
    pthread_detach (hilo_kernel);

    //Atender los mensajes de I/O
    pthread_t hilo_entrada_salida;
    pthread_create(&hilo_entrada_salida, NULL, (void*) atender_memoria_entrada_salida, NULL);
    pthread_join(hilo_entrada_salida, NULL);
    





   _liberar_memoria_asignada_a_usuario ();
    return 0;
}