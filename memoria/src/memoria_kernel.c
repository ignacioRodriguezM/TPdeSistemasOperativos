#include "../include/memoria_kernel.h"
#include "../include/tabla_de_paginas.h"

void atender_memoria_kernel()
{
    bool control_key = 1;

    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel);

        switch (cod_op)
        {
        
        case INICIAR_PROCESO:

            crear_proceso();

            break;

        case FINALIZAR_PROCESO:

            finalizar_proceso();

            break;

        case -1:
            log_error(memoria_log_debug, "Desconexion de KERNEL");
            control_key = 0;
            break;
        default:
            log_warning(memoria_log_debug, "Operacion desconocida de KERNEL");
            break;
        }
    }
}

void crear_proceso()
{
    // Recibir datos del kernel
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_kernel);
    uint16_t pid_recibido = extraer_uint16_al_buffer(buffer_recibido);
    char *path_recibido = extraer_string_al_buffer(buffer_recibido);
    destruir_buffer(buffer_recibido);

    // Log de información
    log_trace(memoria_log_debug, "LLEGO : %u , %s", pid_recibido, path_recibido);

    // Crear estructura de proceso
    Proceso *proceso = malloc(sizeof(Proceso));
    proceso->PID = pid_recibido;
    proceso->cantidad_instrucciones = 0;

    // Leer archivo de instrucciones
    char path_completo[100]; // Variable auxiliar para almacenar la concatenación
    strcpy(path_completo, path_base);
    strcat(path_completo, path_recibido);

    FILE *archivo = fopen(path_completo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    char linea[256];
    proceso->instrucciones = malloc(200 * sizeof(char *));
    while (fgets(linea, 256, archivo) != NULL)
    {
        // Quita el salto de línea si existe
        if (linea[strlen(linea) - 1] == '\n')
        {
            linea[strlen(linea) - 1] = '\0';
        }

        // Copia la línea en un nuevo espacio de memoria y agrega un puntero al arreglo de punteros
        proceso->instrucciones[proceso->cantidad_instrucciones] = strdup(linea);
        proceso->cantidad_instrucciones++;

        // Verifica si se ha alcanzado el límite máximo de líneas
        if (proceso->cantidad_instrucciones >= 200)
        {
            fprintf(stderr, "Se ha alcanzado el límite máximo de líneas\n");
            break;
        }
    }

    // Cerrar el archivo
    fclose(archivo);

    // Enlistar el proceso
    pthread_mutex_lock(&mutex_procesos);
    if (list_add(lista_procesos, proceso) < 0)
    {
        printf("Error al agregar el elemento a la lista.\n");
    }
    pthread_mutex_unlock(&mutex_procesos);
    free(path_recibido);

    crear_tabla_de_paginas(proceso);
}

void finalizar_proceso()
{
    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_kernel);
    uint16_t pid_a_finalizar = extraer_uint16_al_buffer(buffer_recibido);

    destruir_buffer(buffer_recibido);

    ///////////////// LIBERO MEMORIA DE INSTRUCCIONES

    t_link_element *current = lista_procesos->head;
    int index = 0;
    for (int i = 0; i < lista_procesos->elements_count; i++)
    {
        Proceso *proceso = (Proceso *)current->data;
        if (proceso->PID == pid_a_finalizar)
        {
            liberar_paginas_de_proceso(proceso); //LIBERO FRAMES DEL PROCESO (MARCO COMO LIBRES)
            
            proceso->PID = 0;
            for(int i=0; i<proceso->cantidad_instrucciones; i++){
                free(proceso->instrucciones[i]);
            }
            proceso->cantidad_instrucciones = 0;
            free(proceso->instrucciones);
            free(proceso);
            
            list_remove(lista_procesos, index);

            
            break;
        }
        else
        {
            current = current->next;
            index ++;
        }
    }
    current = NULL;
}
