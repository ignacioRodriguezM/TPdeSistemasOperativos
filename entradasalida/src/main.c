#include "../include/main.h"

int main()
{
    // Inicializar entrada salida
    inicializar_logger(&entrada_salida_logger, "entrada_salida_logs.log");
    inicializar_logger_debug(&entrada_salida_log_debug, "entrada_salida_debug.log");

    APP_config config_valores = cargar_configuracion_entrada_salida();

    switch (config_valores.tipo_interfaz)
    {
    case GEN:
        nombre = config_valores.nombre_identificador;
        tipo_interfaz = config_valores.tipo_interfaz;
        tiempo_unidad_trabajo = config_valores.tiempo_unidad_trabajo;

        // Conectarse como cliente a Kernel
        log_info(entrada_salida_logger, "Conectandose a kernel...");
        fd_kernel = crear_conexion(config_valores.ip_kernel, config_valores.puerto_kernel);
        log_info(entrada_salida_logger, "Conexion a kernel Exitosa");

        // AVISARLE A KERNEL QUE LLEGO EL KING
        presentarse_con_kernel();

        // Atender los mensajes de KERNEL
        pthread_t hilo_kernel;
        pthread_create(&hilo_kernel, NULL, (void *)atender_entrada_salida_kernel, NULL);
        pthread_join(hilo_kernel, NULL);

        break;

    case STDIN:
        nombre = config_valores.nombre_identificador;
        tipo_interfaz = config_valores.tipo_interfaz;

        // Conectarse como cliente a Kernel
        log_info(entrada_salida_logger, "Conectandose a kernel...");
        fd_kernel = crear_conexion(config_valores.ip_kernel, config_valores.puerto_kernel);
        log_info(entrada_salida_logger, "Conexion a kernel Exitosa");

        presentarse_con_kernel();

        // Conectarse como cliente a Memoria
        log_info(entrada_salida_logger, "Conectandose a memoria...");
        fd_memoria = crear_conexion(config_valores.ip_memoria, config_valores.puerto_memoria);
        log_info(entrada_salida_logger, "Conexion a memoria Exitosa");

        // Atender los mensajes de KERNEL
        pthread_t hilo_kernel_stdin;
        pthread_create(&hilo_kernel_stdin, NULL, (void *)atender_entrada_salida_kernel, NULL);
        pthread_join(hilo_kernel_stdin, NULL);

        // Atender los mensajes de MEMORIA
        //pthread_t hilo_memoria_stdin;
        //pthread_create(&hilo_memoria_stdin, NULL, (void *)atender_entrada_salida_memoria, NULL);
        //pthread_join(hilo_memoria_stdin, NULL);

        break;
        
    case STDOUT:
        nombre = config_valores.nombre_identificador;
        tipo_interfaz = config_valores.tipo_interfaz;
        tiempo_unidad_trabajo = config_valores.tiempo_unidad_trabajo;

        // Conectarse como cliente a Kernel
        log_info(entrada_salida_logger, "Conectandose a kernel...");
        fd_kernel = crear_conexion(config_valores.ip_kernel, config_valores.puerto_kernel);
        log_info(entrada_salida_logger, "Conexion a kernel Exitosa");

        presentarse_con_kernel();

        // Conectarse como cliente a Memoria
        log_info(entrada_salida_logger, "Conectandose a memoria...");
        fd_memoria = crear_conexion(config_valores.ip_memoria, config_valores.puerto_memoria);
        log_info(entrada_salida_logger, "Conexion a memoria Exitosa");

        // Atender los mensajes de KERNEL
        pthread_t hilo_kernel_stdout;
        pthread_create(&hilo_kernel_stdout, NULL, (void *)atender_entrada_salida_kernel, NULL);
        pthread_join(hilo_kernel_stdout, NULL);

        // Atender los mensajes de MEMORIA
        //pthread_t hilo_memoria_stdout;
        //pthread_create(&hilo_memoria_stdout, NULL, (void *)atender_entrada_salida_memoria, NULL);
        //pthread_join(hilo_memoria_stdout, NULL);

        break;

    case DIALFS:

        nombre = config_valores.nombre_identificador;
        tipo_interfaz = config_valores.tipo_interfaz;
        tiempo_unidad_trabajo = config_valores.tiempo_unidad_trabajo;

        //agregar Mas cosas

        break;
    }
    return 0;
}