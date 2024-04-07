#include <stdint.h>
#include <commons/config.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/configuracion_cpu.h"

APP_config cargar_configuracion_cpu (void)
{
    APP_config config_valores;
    t_config* config = config_create ("cpu.config"); //leo el archivo de configuracion
    if (config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
    }

    //config_valores.ruta_logger                    =       config_get_x (config, "texto anterior")
    config_valores.puerto_memoria                   =       config_get_string_value (config,"PUERTO_MEMORIA");
    config_valores.ip_memoria                       =       config_get_string_value (config,"IP_MEMORIA");
    config_valores.puerto_escucha_dispach           =       config_get_string_value (config, "PUERTO_ESCUCHA_DISPATCH");
    config_valores.puerto_escucha_interrupt         =       config_get_string_value (config, "PUERTO_ESCUCHA_INTERRUPT");
    config_valores.algoritmo_TLB                    =       config_get_string_value (config,"ALGORITMO_TLB");
    config_valores.cantidad_entradas_TLB            =       config_get_int_value (config,"CANTIDAD_ENTRADAS_TLB");
    
    //config_destroy(config);


    return config_valores;

}