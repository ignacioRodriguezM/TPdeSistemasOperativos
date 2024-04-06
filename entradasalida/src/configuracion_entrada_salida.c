#include <stdint.h>
#include <commons/config.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/configuracion_entrada_salida.h"


APP_config cargar_configuracion_entrada_salida (void)
{   
    APP_config config_valores;
    

    t_config* config = config_create ("entrada_salida.config"); //leo el archivo de configuracion
    if (config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
    }

    //config_valores.ruta_logger                    =       config_get_x (config, "texto anterior")
    config_valores.tipo_interfaz                    =       config_get_string_value(config,"TIPO_INTERFAZ");
    config_valores.tiempo_unidad_trabajo            =       config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
    config_valores.ip_kernel                        =       config_get_int_value(config,"IP_KERNEL");
    config_valores.puerto_kernel                    =       config_get_string_value(config,"PUERTO_KERNEL");
    config_valores.ip_memoria                       =       config_get_string_value(config,"IP_MEMORIA");

    config_valores.puerto_memoria                   =       config_get_int_value(config,"PUERTO_MEMORIA");
    config_valores.path_base_dialfs                 =       config_get_string_value(config,"PATH_BASE_DIALFS");
    config_valores.block_size                       =       config_get_int_value(config,"BLOCK_SIZE");
    config_valores.block_count                      =       config_get_int_value(config,"BLOCK_COUNT");

    config_destroy(config);
    return config_valores;
}

