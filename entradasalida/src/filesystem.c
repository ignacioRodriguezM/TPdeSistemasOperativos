#include "../include/filesystem.h"
void _crear_archivo_que_seria_el_disco(){ //primer tipo de archivo
    //int tamanio_de_bloque = config_valores.block_size;
    //int cantidad_de_bloques = config_valores.block_count;

    //int tamanio_del_archivo = tamanio_de_bloque * cantidad_de_bloques;

    //crear_archivo("disco simulado", tamanio_del_archivo);
}

void crear_archivo (char *nombre, int tamanio_del_archivo) {
    FILE *archivo = fopen(nombre, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    // Reserva un bloque de memoria del tamaño especificado
    char *buffer = (char*)malloc(tamanio_del_archivo);
    if (buffer == NULL) {
        perror("Error al reservar memoria");
        fclose(archivo);
        return;
    }

    // Rellena el buffer con ceros
    memset(buffer, 0, tamanio_del_archivo);

    // Escribe el buffer en el archivo
    //size_t escrito = fwrite(buffer, 1, tamanio_del_archivo, archivo);
    //if (escrito != tamanio) {
    //    perror("Error al escribir en el archivo");
    //}

    // Libera la memoria reservada
    free(buffer);

    // Cierra el archivo
    fclose(archivo);

    //printf("Archivo '%s' creado con tamaño %zu bytes.\n", nombre, tamanio_del_archivo);
}