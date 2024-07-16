#include <stdint.h>
#include <commons/config.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/configuracion_kernel.h"

uint16_t int_to_uint16(int num) {
    if (num < 0 || num > UINT16_MAX) {
        // Manejar caso de desbordamiento o valor fuera de rango
        fprintf(stderr, "Error: El valor %d no se puede convertir a uint16_t\n", num);
        exit(EXIT_FAILURE);
    }
    
    return (uint16_t)num;
}

APP_config cargar_configuracion_kernel (void)
{   
    APP_config config_valores;
    

    config_valores.config = config_create ("kernel.config"); //leo el archivo de configuracion
    if (config_valores.config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
    }

    //config_valores.ruta_logger                    =       config_get_x (config, "texto anterior")
    config_valores.puerto_escucha                   =       config_get_string_value (config_valores.config,"PUERTO_ESCUCHA");
    config_valores.ip_memoria                       =       config_get_string_value (config_valores.config,"IP_MEMORIA");
    config_valores.puerto_memoria                   =       config_get_string_value (config_valores.config,"PUERTO_MEMORIA");
    config_valores.ip_cpu                           =       config_get_string_value (config_valores.config,"IP_CPU");
    config_valores.puerto_cpu_dispatch              =       config_get_string_value (config_valores.config,"PUERTO_CPU_DISPATCH");
    config_valores.puerto_cpu_interrupt             =       config_get_string_value (config_valores.config,"PUERTO_CPU_INTERRUPT");

    config_valores.algoritmo_planificacion          =       config_get_string_value (config_valores.config,"ALGORITMO_PLANIFICACION");


    int aux_var                                     =       config_get_int_value (config_valores.config,"QUANTUM");
    config_valores.quantum                          =       int_to_uint16(aux_var);


    config_valores.recursos                         =       config_get_array_value (config_valores.config,"RECURSOS");
    config_valores.instancias_recursos              =       config_get_array_value (config_valores.config,"INSTANCIAS_RECURSOS");
    
    int var_aux2                                    =       config_get_int_value (config_valores.config,"GRADO_MULTIPROGRAMACION");
    config_valores.grado_multiprogramacion          =       int_to_uint16(var_aux2);

    return config_valores;
}

int* convertirArrayCharAUInt16(char** array_char) {
    int longitud = 0;
    while (array_char[longitud] != NULL) {
        longitud++;
    }
    
    int* array_uint16 = malloc(sizeof(int) * longitud);
    if (array_uint16 == NULL) {
        perror("Error al asignar memoria para el array de enteros");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < longitud; i++) {
        array_uint16[i] = (int)atoi(array_char[i]);
    }
    
    return array_uint16;
}