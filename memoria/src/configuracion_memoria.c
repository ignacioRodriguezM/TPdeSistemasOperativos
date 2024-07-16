#include <stdint.h>
#include <commons/config.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/configuracion_memoria.h"

uint16_t int_to_uint16(int num) {
    if (num < 0 || num > UINT16_MAX) {
        // Manejar caso de desbordamiento o valor fuera de rango
        fprintf(stderr, "Error: El valor %d no se puede convertir a uint16_t\n", num);
        exit(EXIT_FAILURE);
    }
    
    return (uint16_t)num;
}

APP_config cargar_configuracion_memoria (void)
{
    APP_config config_valores;
    t_config* config = config_create ("memoria.config"); //leo el archivo de configuracion
    if (config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
    }

    //config_valores.ruta_logger                    =       config_get_x (config, "texto anterior")
    config_valores.puerto_escucha                   =       config_get_string_value (config,"PUERTO_ESCUCHA");

    int aux_var1                                     =       config_get_int_value (config, "TAM_MEMORIA");
    config_valores.tam_memoria = int_to_uint16(aux_var1);

    int aux_var2                                     =       config_get_int_value (config, "TAM_PAGINA");
    config_valores.tam_pagina  = int_to_uint16(aux_var2);


    int aux_var3                                     =       config_get_int_value (config, "RETARDO_RESPUESTA");
    config_valores.retardo_respuesta = int_to_uint16(aux_var3);


    config_valores.path_instrucciones               =       config_get_string_value (config,"PATH_INSTRUCCIONES");

    config_valores.config = config;
    
    //config_destroy(config);
    return config_valores;
}