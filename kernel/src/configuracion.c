#include "../include/main.h"
#include "../include/configuracion.h"

APP_config config_valores;
void cargar_configuracion (void)
{
    t_config* config = config_create ("../kernel.config"); //leo el archivo de configuracion
    if (config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
        return;
    }

    //config_valores.ruta_logger                    =       config_get_x (config, "texto anterior")
    config_valores.puerto_escucha                   =       config_get_int_value (config,"PUERTO_ESCUCHA");
    config_valores.ip_memoria                       =       config_get_string_value (config,"IP_MEMORIA");
    config_valores.puerto_memoria                   =       config_get_int_value (config,"PUERTO_MEMORIA");
    config_valores.ip_cpu                           =       config_get_string_value (config,"IP_CPU");
    config_valores.puerto_cpu_dispatch              =       config_get_int_value (config,"PUERTO_CPU_DISPATCH");
    config_valores.puerto_cpu_interrupt             =       config_get_int_value (config,"PUERTO_CPU_INTERRUPT");

    config_valores.algoritmo_planificacion          =       config_get_string_value (config,"ALGORITMO_PLANIFICACION");
    config_valores.quantum                          =       config_get_int_value (config,"QUANTUM");
    //config_valores.recursos                         =       config_get_array_value (config,"RECURSOS");
    //config_valores.instancias_recursos              =       config_get_array_value (config,"INSTANCIAS_RECURSOS");
    config_valores.grado_multiprogramacion          =       config_get_int_value (config,"GRADO_MULTIPROGRAMACION");

    //config destroy(config);

}

