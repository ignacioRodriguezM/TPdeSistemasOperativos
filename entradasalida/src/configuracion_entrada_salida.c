#include "../include/configuracion_entrada_salida.h"
interfaces_io convertirCharAEnum(char* str);

uint16_t int_to_uint16(int num) {
    if (num < 0 || num > UINT16_MAX) {
        // Manejar caso de desbordamiento o valor fuera de rango
        fprintf(stderr, "Error: El valor %d no se puede convertir a uint16_t\n", num);
        exit(EXIT_FAILURE);
    }
    
    return (uint16_t)num;
}

APP_config cargar_configuracion_entrada_salida (void)
{   
    APP_config config_valores;
    

    t_config* config = config_create ("entrada_salida.config"); //leo el archivo de configuracion
    if (config == NULL){
        perror("Archivo de configuracion de APP no encontrado");
    }
    config_valores.config = config;

    printf ("Decime tu nombre identificador \n");
    scanf("%ms", &config_valores.nombre_identificador); // La bandera '%ms' indica a scanf() que asigne memoria dinámica para almacenar la cadena

    char* tip_inter = config_get_string_value(config,"TIPO_INTERFAZ");
    config_valores.tipo_interfaz = convertirCharAEnum (tip_inter);

    switch (config_valores.tipo_interfaz){
        case GEN:

            int aux_var = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
            config_valores.tiempo_unidad_trabajo            =       int_to_uint16(aux_var);
            config_valores.ip_kernel                        =       config_get_string_value(config,"IP_KERNEL");
            config_valores.puerto_kernel                    =       config_get_string_value(config,"PUERTO_KERNEL");
            //VEER COMO BORRAR MAS ADELANTE
                        config_valores.ip_memoria                       =       config_get_string_value(config,"IP_MEMORIA");
                        config_valores.puerto_memoria                   =       config_get_string_value(config,"PUERTO_MEMORIA");
            break;
        

        case STDIN:
            config_valores.ip_kernel                        =       config_get_string_value(config,"IP_KERNEL");
            config_valores.puerto_kernel                    =       config_get_string_value(config,"PUERTO_KERNEL");
            config_valores.ip_memoria                       =       config_get_string_value(config,"IP_MEMORIA");
            config_valores.puerto_memoria                   =       config_get_string_value(config,"PUERTO_MEMORIA");

            break;
             
        

        case STDOUT:
            config_valores.ip_kernel                        =       config_get_string_value(config,"IP_KERNEL");
            config_valores.puerto_kernel                    =       config_get_string_value(config,"PUERTO_KERNEL");
            config_valores.ip_memoria                       =       config_get_string_value(config,"IP_MEMORIA");
            config_valores.puerto_memoria                   =       config_get_string_value(config,"PUERTO_MEMORIA");

            break;
        

        case DIALFS:
            int aux_var2 = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
            config_valores.tiempo_unidad_trabajo            =       int_to_uint16(aux_var2);


            config_valores.ip_kernel                        =       config_get_string_value(config,"IP_KERNEL");
            config_valores.puerto_kernel                    =       config_get_string_value(config,"PUERTO_KERNEL");
            config_valores.ip_memoria                       =       config_get_string_value(config,"IP_MEMORIA");
            config_valores.puerto_memoria                   =       config_get_string_value(config,"PUERTO_MEMORIA");
            config_valores.path_base_dialfs                 =       config_get_string_value(config,"PATH_BASE_DIALFS");

            int aux4 =       config_get_int_value(config,"BLOCK_SIZE");
            config_valores.block_size                       =       int_to_uint16(aux4);

            int aux5 = config_get_int_value(config,"BLOCK_COUNT");
            config_valores.block_count                      =     int_to_uint16(aux5);  

            int aux6 = config_get_int_value(config,"RETRASO_COMPACTACION");
            config_valores.retraso_compactacion             =       int_to_uint16(aux6);
            break;
        
    }
    
    free(tip_inter);
    
    //config_destroy(config);
    return config_valores;
}

interfaces_io convertirCharAEnum(char* str){
    if (strcmp(str, "GEN")==0){
        return GEN;
    }
    if (strcmp(str, "STDIN")==0){
        return STDIN;
    }
    if (strcmp(str, "STDOUT")==0){
        return STDOUT;
    }
    if (strcmp(str, "DIALFS")==0){
        return DIALFS;
    }
    else{
        return -1;
    }
}
