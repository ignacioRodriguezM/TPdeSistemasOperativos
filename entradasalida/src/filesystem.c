#include "../include/filesystem.h"
#include "../include/configuracion_entrada_salida.h"
#include <sys/stat.h> // SOLO PARA MKDIR

void agregar_a_archivos(char *nombre_archivo)
{
    archivos.cantidad_archivos++;
    list_add(archivos.lista_archivos, nombre_archivo);

    //agregarlo al file de _creados
    
}
void quitar_de_archivos(char *nombre_archivo)
{

    t_link_element *current = archivos.lista_archivos->head;
    for (int i = 0; i < archivos.cantidad_archivos; i++)
    {
        char *string_a_comparar = (char *)current->data;
        if (strcmp(nombre_archivo, string_a_comparar) == 0)
        {
            list_remove(archivos.lista_archivos, i);
            archivos.cantidad_archivos--;
            return;
        }
        else
        {
            current = current->next;
        }
    }

    current = NULL;
}

void compactar_hacia_archivo(char* nombre_del_archivo)
{
    //esto seria como crear un nuevo disco temporal
    int tamanio_del_archivo = tamanio_de_bloque * cantidad_de_bloques;
    char *buffer = (char *)malloc(tamanio_del_archivo);

    int* puntero_buffer = malloc(sizeof(int));
    *puntero_buffer = 0;

    for(int i=0; i<archivos.cantidad_archivos; i++)
    {
        t_link_element *current = archivos.lista_archivos->head;
        char *string_a_comparar = (char *)current->data;
        if(strcmp(nombre_del_archivo, string_a_comparar) != 0)
        {
            compactar(string_a_comparar, buffer, puntero_buffer);
        }
        else
        {
            current = current->next;
        }
    }

    compactar(nombre_del_archivo, buffer, puntero_buffer);

    int bloques_ocupados = division_entera_redondear_arriba(*puntero_buffer, tamanio_de_bloque);

    char *bitarray = (char *)malloc(tamanio_bitarray_en_bytes);
    memset(bitarray, 0, tamanio_bitarray_en_bytes);

    t_bitarray *bitmap = bitarray_create_with_mode(bitarray, tamanio_bitarray_en_bytes, LSB_FIRST);
    
    for(int i=0 ; i < bloques_ocupados; i++)
    {
        bitarray_set_bit(bitmap, i);
    }

    actualizar_archivo_bitmap(bitmap);

    free(bitmap);    

    free(puntero_buffer);

    FILE *archivo = fopen(nombre, "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }

    // Escribir el buffer en el archivo
    size_t escrito = fwrite(buffer, 1, tamanio_del_archivo, archivo);
    if (escrito != tamanio_del_archivo)
    {
        perror("Error al escribir en el archivo");
    }

    // Liberar la memoria reservada
    free(buffer);

    // Cerrar el archivo
    fclose(archivo);

    printf("Archivos COMPACTADOS.\n");

}

void compactar (char *nombre_archivo, char* buffer, int *puntero_buffer){

    int tamanio_del_archivo = tamanio_de_bloque * cantidad_de_bloques;

    METADATA info_archivo = leer_metadata(nombre_archivo);

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%sbloques.dat", path_base);
    FILE *file;
    file = fopen(full_path, "rb");
    char *buffer_viejo = (char *)malloc(tamanio_del_archivo);

    size_t leido = fread(buffer_viejo, 1, tamanio_del_archivo, file);
    if (leido != tamanio_bitarray_en_bytes)
    {
        perror("Error al leer el archivo bitmap");
        free(buffer_viejo);
        fclose(file);
        return;
    }

    memcpy(buffer + *puntero_buffer , buffer_viejo + (info_archivo.bloque_inicial * tamanio_de_bloque) , info_archivo.tam_bytes);   

    actualizar_archivo_metadata(nombre_archivo, *puntero_buffer, info_archivo.tam_bytes);

    *puntero_buffer += info_archivo.tam_bytes; // +1?

    free(buffer_viejo);
}
// Función para leer el bitmap desde el archivo
t_bitarray *leer_bitmap()
{

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%sbitmap.dat", path_base);

    FILE *archivo = fopen(full_path, "rb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo bitmap");
        
    }

    char *bitarray = (char *)malloc(tamanio_bitarray_en_bytes);
    if (bitarray == NULL)
    {
        perror("Error al reservar memoria para el bitmap");
        fclose(archivo);
        
    }

    size_t leido = fread(bitarray, 1, tamanio_bitarray_en_bytes, archivo);
    if (leido != tamanio_bitarray_en_bytes)
    {
        perror("Error al leer el archivo bitmap");
        free(bitarray);
        fclose(archivo);
        
    }

    t_bitarray *bitmap = bitarray_create_with_mode(bitarray, tamanio_bitarray_en_bytes, LSB_FIRST);

    fclose(archivo);
    return bitmap;
}

// Pone ceros en las posiciones correspondientes del bitmap
void liberar_bloques_del_bitmap(t_bitarray *bitmap, int bloque_inicio_barrido, int longitud)
{
    for (int i = 0; i < longitud; i++)
    {
        if (!bitarray_test_bit(bitmap, bloque_inicio_barrido + i))
        {
            log_error(entrada_salida_log_debug, "El bloque %d ya estaba libre (ERROR GRAVE, REVISAR) \n", bloque_inicio_barrido + i);
        }
        else
        {
            bitarray_clean_bit(bitmap, bloque_inicio_barrido + i);
            // Limpia el bit correspondiente en el bitmap
        }
    }
}

// Escribir el bitmap actualizado de vuelta al archivo
void actualizar_archivo_bitmap(t_bitarray *bitmap)
{
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%sbitmap.dat", path_base);

    FILE *archivo = fopen(full_path, "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo bitmap para escritura");
        free(bitmap);
        return;
    }

    fwrite(bitmap->bitarray, 1, tamanio_bitarray_en_bytes, archivo);
    fclose(archivo);
}

int buscar_secuencia_libre(t_bitarray *bitmap, int longitud)
{
    for (int i = 0; i <= cantidad_de_bloques - longitud; i++)
    {
        bool encontrado = true;
        for (int j = 0; j < longitud; j++)
        {
            if (bitarray_test_bit(bitmap, i + j))
            {
                encontrado = false;
                break;
            }
        }
        if (encontrado)
        {
            return i;
        }
    }
    return -1; // No se encontró una secuencia libre de la longitud requerida
}

bool chquear_bloques_libres_contiguos(t_bitarray *bitmap, int primer_bloque_a_chequear, int cantidad)
{
    int bloque_que_estoy_chequeando = primer_bloque_a_chequear;

    for (int i = 0; i < cantidad; i++)
    {
        if (bloque_que_estoy_chequeando >= cantidad_de_bloques)
        {
            return false;
        }
        if (bitarray_test_bit(bitmap, bloque_que_estoy_chequeando))
        {
            return false;
        }
        bloque_que_estoy_chequeando++;
    }

    return true;
}

int cantidad_de_bloques_libres(t_bitarray *bitmap)
{
    int contador = 0;
    for (int i = 0; i < cantidad_de_bloques; i++)
    {
        if (!bitarray_test_bit(bitmap, i))
        {
            contador++;
        }
    }

    return contador;
}

// Marcar los bloques como ocupados en el bitmap
void marcar_bloques_ocupados(t_bitarray *bitmap, int bloque_inicio, int longitud)
{
    for (int i = 0; i < longitud; i++)
    {
        if (bitarray_test_bit(bitmap, bloque_inicio + i))
        {
            log_error(entrada_salida_log_debug, "EL BIT YA ESTABA OCUPADO, FALLO GRAVE");
        }
        bitarray_set_bit(bitmap, bloque_inicio + i);
    }
}

// Crear metadata para un archivo
void crear_metadata(const char *nombre_archivo, int bloque_inicial, int tamanio_archivo_en_bytes)
{

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", path_base, nombre_archivo);

    FILE *archivo = fopen(full_path, "w");
    if (archivo == NULL)
    {
        perror("Error al crear el archivo de metadata");
        return;
    }

    // Escribir metadata
    fprintf(archivo, "BLOQUE_INICIAL=%d\n", bloque_inicial);
    fprintf(archivo, "TAMANIO_ARCHIVO=%d\n", tamanio_archivo_en_bytes);

    // Cerrar el archivo
    fclose(archivo);

    printf("Archivo de metadata '%s' creado.\n", full_path);
}
void borrar_archivo_metadata(char *nombre_del_archivo)
{
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", path_base, nombre_del_archivo);

    // Eliminar el archivo
    if (remove(full_path) != 0)
    {
        perror("Error al borrar el archivo de metadata");
    }
    else
    {
        printf("Archivo de metadata '%s' borrado.\n", full_path);
    }
}

void actualizar_archivo_metadata(char *nombre_archivo, int bloque_inicial, int nuevo_tamanio_archivo_en_bytes)
{
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", path_base, nombre_archivo);

    FILE *archivo = fopen(full_path, "w");
    if (archivo == NULL)
    {
        perror("Error al crear el archivo de metadata");
        return;
    }

    // Escribir metadata
    fprintf(archivo, "BLOQUE_INICIAL=%d\n", bloque_inicial);
    fprintf(archivo, "TAMANIO_ARCHIVO=%d\n", nuevo_tamanio_archivo_en_bytes);

    // Cerrar el archivo
    fclose(archivo);

    printf("Archivo de metadata '%s' actualizado.\n", full_path);
}

METADATA leer_metadata(char *nombre_archivo)
{
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", path_base, nombre_archivo);

    METADATA informacion_archivo;

    FILE *archivo = fopen(full_path, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo para lectura.\n");
        return informacion_archivo;
    }

    // Leer los enteros del archivo
    if (fscanf(archivo, "BLOQUE_INICIAL=%d\n", &(informacion_archivo.bloque_inicial)) != 1)
    {
        printf("Error al leer BLOQUE_INICIAL.\n");
        fclose(archivo);
        return informacion_archivo;
    }
    if (fscanf(archivo, "TAMANIO_ARCHIVO=%d\n", &(informacion_archivo.tam_bytes)) != 1)
    {
        printf("Error al leer TAMANIO_ARCHIVO.\n");
        fclose(archivo);
        return informacion_archivo;
    }

    // Mostrar los enteros leídos
    printf("Leído del archivo: BLOQUE_INICIAL = %d, TAMANIO_ARCHIVO = %d\n", informacion_archivo.bloque_inicial, informacion_archivo.tam_bytes);

    // Cerrar el archivo
    fclose(archivo);

    return informacion_archivo;
}

// Crear un archivo con un tamaño específico
void crear_archivo(char *nombre, int tamanio_del_archivo)
{
    FILE *archivo = fopen(nombre, "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }

    // Reservar memoria del tamaño especificado
    char *buffer = (char *)malloc(tamanio_del_archivo);
    if (buffer == NULL)
    {
        perror("Error al reservar memoria");
        fclose(archivo);
        return;
    }

    // Rellenar el buffer con ceros
    memset(buffer, 0, tamanio_del_archivo); // es necesario?

    // Escribir el buffer en el archivo
    size_t escrito = fwrite(buffer, 1, tamanio_del_archivo, archivo);
    if (escrito != tamanio_del_archivo)
    {
        perror("Error al escribir en el archivo");
    }

    // Liberar la memoria reservada
    free(buffer);

    // Cerrar el archivo
    fclose(archivo);

    printf("Archivo '%s' creado con tamaño %d bytes.\n", nombre, tamanio_del_archivo);
}

/////////////////////////////////////////////////////////////////////
///////////////////////// INICIALIZACIONES///////////////////////////
/////////////////////////////////////////////////////////////////////

void inicializar_estructuras_filesystem()
{

    _verificar_que_la_ruta_exista();

    _verificar_si_existe_archivo_bloques();

    _verificar_si_existe_archivo_bitmap();


    
    return;
}

// Crear archivo simulado del disco
void _verificar_si_existe_archivo_bloques()
{

    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%sbloques.dat", path_base);

    FILE *file;
    // Intenta abrir el archivo en modo de lectura
    file = fopen(full_path, "r");

    if (file == NULL)
    {
        // Si el archivo no existe (fopen retorna NULL), entonces lo creamos
        int tamanio_del_archivo = tamanio_de_bloque * cantidad_de_bloques;
        crear_archivo(full_path, tamanio_del_archivo);
    }
    else
    {
        printf("El archivo ya existe en: %s\n", full_path);
        fclose(file);
    }
}

void _verificar_si_existe_archivo_bitmap()
{
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%sbitmap.dat", path_base);

    FILE *file;
    // Intenta abrir el archivo en modo de lectura
    file = fopen(full_path, "rb");

    if (file == NULL)
    {
        // Si el archivo no existe (fopen retorna NULL), entonces lo creamos
        _crear_archivo_que_contiene_el_bitmap(full_path);
    }
    else
    {
        printf("El archivo ya existe en: %s\n", full_path);
        fclose(file);
    }
}

// Crear archivo que contiene el bitmap
void _crear_archivo_que_contiene_el_bitmap(char *ruta)
{

    char *bitarray = (char *)malloc(tamanio_bitarray_en_bytes);

    memset(bitarray, 0, tamanio_bitarray_en_bytes);

    t_bitarray *bitmap = bitarray_create_with_mode(bitarray, tamanio_bitarray_en_bytes, LSB_FIRST);

    FILE *archivo = fopen(ruta, "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo bitmap");
        return;
    }

    size_t escrito = fwrite(bitmap->bitarray, 1, bitmap->size, archivo);
    if (escrito != bitmap->size)
    {
        perror("Error al escribir en el archivo bitmap");
    }

    printf("Archivo '%s' creado con tamaño %lu bytes.\n", ruta, bitmap->size);

    bitarray_destroy(bitmap);
    fclose(archivo);
}

void _verificar_que_la_ruta_exista()
{
    // Verificamos si el directorio base existe
    // Construimos una ruta para verificar la existencia del directorio
    FILE *file;
    char test_path[256];
    snprintf(test_path, sizeof(test_path), "%stestfile", path_base);

    // Intenta abrir un archivo de prueba en el directorio
    file = fopen(test_path, "w");
    if (file == NULL)
    {
        // Si no se puede abrir el archivo de prueba, intentamos crear el directorio
        if (mkdir(path_base, 0755) == -1)
        {
            printf("Error al crear el directorio %s\n", path_base);
            return;
        }
        else
        {
            printf("Directorio %s creado exitosamente.\n", path_base);
        }
    }
    else
    {
        // Si se pudo abrir el archivo de prueba, el directorio ya existía
        fclose(file);
        // Eliminamos el archivo de prueba
        remove(test_path);
        printf("El directorio %s ya existe.\n", path_base);
    }
}
