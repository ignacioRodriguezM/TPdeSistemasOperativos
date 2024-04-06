#include <stdint.h>
#include <commons/config.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/configuracion_memoria.h"

APP_config cargar_configuracion_memoria (void)
{
    APP_config config_valores;
    t_config* config = config_create ("memoria.config"); //leo el archivo de configuracion
    if (config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
    }

    //config_valores.ruta_logger                    =       config_get_x (config, "texto anterior")
    config_valores.puerto_escucha                   =       config_get_int_value (config,"PUERTO_ESCUCHA");
    config_valores.tam_memoria                      =       config_get_int_value (config,"TAM_MEMORIA");
    config_valores.tam_pagina                       =       config_get_int_value (config, "TAM_PAGINA");
    config_valores.retardo_respuesta                =       config_get_int_value (config, "RETARDO_RESPUESTA");
    config_valores.path_instrucciones               =       config_get_string_value (config,"PATH_INSTRUCCIONES");
    
    //config_destroy(config);
    return config_valores;
}