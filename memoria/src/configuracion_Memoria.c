#include "../include/main.h"
#include "../include/Configuracion_Memoria.h"

void cargar_configuracion (APP_config config_valores)
{
    t_config* config = config_create ("../CPU.config"); //leo el archivo de configuracion
    if (config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
        return;
    }

    //config_valores.ruta_logger                    =       config_get_x (config, "texto anterior")
    config_valores.puerto_escucha                   =       config_get_int_value (config,"PUERTO_ESCUCHA");
    config_valores.tam_memoria                      =       config_get_int_value (config,"TAM_MEMORIA");
    config_valores.tam_pagina                       =       config_get_int_value (config, "TAM_PAGINA");
    config_valores.retardo_respuesta                =       config_get_int_value (config, "RETARDO_RESPUESTA");
    config_valores.path_instrucciones               =       config_get_string_value (config,"PATH_INSTRUCCIONES");
    
    //config destroy(config);

}