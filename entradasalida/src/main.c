#include "../include/main.h"


int main() {
    //Inicializar entrada salida
    inicializar_logger (&entrada_salida_logger, "entrada_salida_logs.log");
    inicializar_logger_debug (&entrada_salida_log_debug, "entrada_salida_debug.log");

    APP_config config_valores = cargar_configuracion_entrada_salida();

    switch (config_valores.tipo_interfaz){
        case GEN:
            nombre = config_valores.nombre_identificador;
            tipo_interfaz = config_valores.tipo_interfaz;
            tiempo_unidad_trabajo = config_valores.tiempo_unidad_trabajo;
            

            //Conectarse como cliente a Kernel
            log_info(entrada_salida_logger, "Conectandose a kernel...");
            fd_kernel = crear_conexion (config_valores.ip_kernel, config_valores.puerto_kernel);
            log_info (entrada_salida_logger,"Conexion a kernel Exitosa" );

            //AVISARLE A KERNEL QUE LLEGO EL KING
            presentarse_con_kernel ();

            /*
            // QUITAR
            log_info(entrada_salida_logger, "Conectandose a memoria...");
            fd_memoria = crear_conexion (config_valores.ip_memoria, config_valores.puerto_memoria);
            log_info (entrada_salida_logger,"Conexion a memoria Exitosa" );

            // QUITAR
            pthread_t hilo_memoria;
            pthread_create(&hilo_memoria, NULL, (void*) atender_entrada_salida_memoria, NULL);
            pthread_detach (hilo_memoria);
            */

            //Atender los mensajes de KERNEL
            pthread_t hilo_kernel;
            pthread_create(&hilo_kernel, NULL, (void*) atender_entrada_salida_kernel, NULL);
            pthread_join (hilo_kernel, NULL);


            break;


        case STDIN:
            log_info (entrada_salida_logger,"INTERFAZ AUN NO DESARROLLADA" );
            break;
        case STDOUT:
            log_info (entrada_salida_logger,"INTERFAZ AUN NO DESARROLLADA" );
            break;
        case DIALFS:
            log_info (entrada_salida_logger,"INTERFAZ AUN NO DESARROLLADA" );
            break;
    }
    return 0;
}