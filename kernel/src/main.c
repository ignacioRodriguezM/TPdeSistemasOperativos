#include "../include/main.h"

int main()
{
    // incializo kernel;
    sem_init(&planificacion_activa_semaforo, 0, 0);
    sem_init(&proceso_creado_en_new_semaforo, 0, 0);
    sem_init(&cpu_vacia_semaforo, 0, 1);
    sem_init(&algun_ready, 0, 0);

    inicializar_logger(&kernel_logger, "kernel_logs.log");
    inicializar_logger_debug(&kernel_log_debug, "kernel_debug.log");
    APP_config config_valores = cargar_configuracion_kernel();
    // POR SI NECESITO PASAR EL ARRAY DE STRINGS A ARRAY DE INTS:

    int *instancias_recursos = convertirArrayCharAUInt16(config_valores.instancias_recursos);

    iniciar_semaforos_de_recursos(config_valores.recursos, instancias_recursos);

    grado_multiprogramacion = config_valores.grado_multiprogramacion;
    algoritmo_planificacion = config_valores.algoritmo_planificacion;

    sem_init(&grado_multiprogramacion_semaforo, 0, grado_multiprogramacion);

    planificacion_activa = false;
    uint16_t aux = config_valores.quantum;
    quantum = aux;
    inicializar_colas_sin_bloqueados(); // INICIALIZA LAS COLAS "NORMALES"

    iniciar_planificador_de_largo_plazo();
    iniciar_planificador_de_corto_plazo();

    // iniciar servidor de kernel
    fd_kernel = iniciar_servidor(config_valores.puerto_escucha, kernel_logger, "KERNEL INCIADO !!!");

    // Conectarse como cliente a memoria
    log_info(kernel_logger, "Conectandose a memoria...");
    fd_memoria = crear_conexion(config_valores.ip_memoria, config_valores.puerto_memoria);
    log_info(kernel_logger, "Conexion a memoria Exitosa");

    // Conectarse como cliente a cpu dispatch
    log_info(kernel_logger, "Conectandose a cpu dispatch...");
    fd_cpu_dispatch = crear_conexion(config_valores.ip_cpu, config_valores.puerto_cpu_dispatch);
    log_info(kernel_logger, "Conexion a cpu dispatch Exitosa");

    // Conectarse como cliente a cpu interrupt
    log_info(kernel_logger, "Conectandose a cpu interrupt...");
    fd_cpu_interrupt = crear_conexion(config_valores.ip_cpu, config_valores.puerto_cpu_interrupt);
    log_info(kernel_logger, "Conexion a cpu interrupt Exitosa");

    // atender memoria
    pthread_t hilo_kernel_memoria;
    pthread_create(&hilo_kernel_memoria, NULL, (void *)atender_kernel_memoria, NULL);
    pthread_detach(hilo_kernel_memoria);

    // atender cpu dispatch
    pthread_t hilo_kernel_cpu_dispatch;
    pthread_create(&hilo_kernel_cpu_dispatch, NULL, (void *)atender_kernel_cpu_dispatch, NULL);
    pthread_detach(hilo_kernel_cpu_dispatch);


    // atender entrada/salida
    pthread_t hilo_kernel_entrada_salida;
    pthread_create(&hilo_kernel_entrada_salida, NULL, (void *)atender_kernel_entrada_salida, NULL);
    pthread_detach(hilo_kernel_entrada_salida);

    // Iniciar consola interactiva
    iniciar_consola_interactiva(); // si queremos probar los comandos sin antes correr todos los modulos, lo ponemos arriba

    log_debug(kernel_log_debug, "Advertencia de salida de Kernel");
    finalizar_colas_sin_bloqueados();
    config_destroy(config_valores.config);

    return 0;
}