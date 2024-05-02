#include "../include/memoria_kernel.h"

void atender_memoria_kernel()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case PAQUETE:
            //
            break;

        case INICIAR_PROCESO:

    
            crear_proceso();
            
            break;

        case FINALIZAR_PROCESO:

            //finalizar_proceso ();
            
            break;

        case -1:
            log_error(memoria_logger, "Desconexion de KERNEL");
            control_key = 0;
            break;
        default:
            log_warning(memoria_logger, "Operacion desconocida de KERNEL");
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
    log_info(memoria_logger, "LLEGO : %u , %s", pid_recibido, path_recibido);

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
    proceso->instrucciones = malloc(100 * sizeof(char *));
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
        if (proceso->cantidad_instrucciones >= 100)
        {
            fprintf(stderr, "Se ha alcanzado el límite máximo de líneas\n");
            break;
        }
    }

    // Cerrar el archivo
    fclose(archivo);

    // Imprimir la primera instrucción (para depuración)


    // Enlistar el proceso
    pthread_mutex_lock(&mutex_procesos);
    if (list_add(lista_procesos, proceso) < 0)
    {
        printf("Error al agregar el elemento a la lista.\n");
    }
    pthread_mutex_unlock(&mutex_procesos);
    free(path_recibido);
}


