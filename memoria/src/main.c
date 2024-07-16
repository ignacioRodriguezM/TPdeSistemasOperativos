#include "../include/main.h"
#include "../include/tabla_de_paginas.h"

int main() {
    //Inicializar memmoria
    inicializar_logger (&memoria_logger, "memoria_logs.log");
    inicializar_logger_debug (&memoria_log_debug, "memoria_debug.log");
    APP_config config_valores = cargar_configuracion_memoria();
    tam_memoria = config_valores.tam_memoria;
    tam_pagina = config_valores.tam_pagina;
    retardo_respuesta = config_valores.retardo_respuesta;
    path_base = config_valores.path_instrucciones;
    lista_procesos = list_create();


    crear_memoria_y_dividir_en_marcos();

    //iniciar servidor de memoria
    fd_memoria = iniciar_servidor (config_valores.puerto_escucha, memoria_logger, "MEMORIA INCIADA !!!");
 
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

    //Atender I/Os
    pthread_t hilo_entrada_salida;
    pthread_create(&hilo_entrada_salida, NULL, (void*) atender_memoria_entrada_salida, NULL);
    pthread_detach(hilo_entrada_salida);
        
    //Atender los mensajes de KERNEL
    pthread_t hilo_kernel;
    pthread_create(&hilo_kernel, NULL, (void*) atender_memoria_kernel, NULL);
    pthread_join (hilo_kernel, NULL);

    config_destroy(config_valores.config);

    return 0;
}