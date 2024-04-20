#include "../include/main.h"


int main() {
    //incializo kernel;
    inicializar_logger (&kernel_logger, "kernel_logs.log");
    inicializar_logger_debug (&kernel_log_debug, "kernel_debug.log");
    APP_config config_valores = cargar_configuracion_kernel();
    // POR SI NECESITO PASAR EL ARRAY DE STRINGS A ARRAY DE INTS: uint16_t* instancias_Recursos = convertirArrayCharAUInt16 (config_valores.instancias_recursos);

    grado_multiprogramacion = config_valores.grado_multiprogramacion;
    algoritmo_planificacion = config_valores.algoritmo_planificacion;
    contador_de_colas_bloqueados = 0;
    procesos_en_programacion = 0;
    inicializar_colas_sin_bloqueados(); //INICIALIZA LAS COLAS



    //iniciar_planificador_de_corto_plazo ();

    //iniciar servidor de kernel
    fd_kernel = iniciar_servidor (config_valores.puerto_escucha, kernel_logger, "KERNEL INCIADO !!!");

    while(1){
        log_info(kernel_logger, "Esperando a  algun entrada_salida...");
        fd_entrada_salida = esperar_cliente (fd_kernel, kernel_logger, "ENTRADA SALIDA");
    }

    
        
    //Atender i/o PROBANDO
    pthread_t hilo_kernel_entrada_salida;
    pthread_create (&hilo_kernel_entrada_salida, NULL, (void*)atender_kernel_entrada_salida, NULL);
    pthread_join(hilo_kernel_entrada_salida, NULL);

    
    
    
   

    /*
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
    

    //atender memoria
    pthread_t hilo_kernel_memoria;
    pthread_create (&hilo_kernel_memoria, NULL, (void*)atender_kernel_memoria, NULL);
    pthread_detach(hilo_kernel_memoria);

    //atender cpu dispatch
    pthread_t hilo_kernel_cpu_dispatch;
    pthread_create (&hilo_kernel_cpu_dispatch, NULL, (void*)atender_kernel_cpu_dispatch, NULL);
    pthread_detach(hilo_kernel_cpu_dispatch);


    //atender cpu interrupt
    pthread_t hilo_kernel_cpu_interrupt;
    pthread_create (&hilo_kernel_cpu_interrupt, NULL, (void*)atender_kernel_cpu_inerrupt, NULL);
    pthread_detach(hilo_kernel_cpu_interrupt);


    //Atender i/o
    pthread_t hilo_kernel_entrada_salida;
    pthread_create (&hilo_kernel_entrada_salida, NULL, (void*)atender_kernel_entrada_salida, NULL);
    pthread_detach(hilo_kernel_entrada_salida);

    //Iniciar consola interactiva
    iniciar_consola_interactiva (); //si queremos probar los comandos sin antes correr todos los modulos, lo ponemos arriba

    */


    log_debug(kernel_log_debug, "Advertencia de salida de Kernel");
    finalizar_colas_sin_bloqueados ();
    config_destroy(config_valores.config);
    
    return 0;
}