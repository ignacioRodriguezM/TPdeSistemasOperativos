#include "../include/filesystem.h"
#include "../include/configuracion_entrada_salida.h"

// Función para leer el bitmap desde el archivo
unsigned char* leer_bitmap(int cantidad_de_bloques) {
    int tamanio_bitmap = (cantidad_de_bloques + 7) / 8;
    FILE *archivo = fopen("bitmap.dat", "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo bitmap");
        return NULL;
    }

    unsigned char *bitmap = (unsigned char*)malloc(tamanio_bitmap);
    if (bitmap == NULL) {
        perror("Error al reservar memoria para el bitmap");
        fclose(archivo);
        return NULL;
    }

    size_t leido = fread(bitmap, 1, tamanio_bitmap, archivo);
    if (leido != tamanio_bitmap) {
        perror("Error al leer el archivo bitmap");
        free(bitmap);
        fclose(archivo);
        return NULL;
    }

    fclose(archivo);
    return bitmap;
}

// Función auxiliar para verificar si una posición en el bitmap está libre
int esta_libre(unsigned char *bitmap, int pos) {
    int byte_index = pos / 8;
    int bit_index = pos % 8;
    return !(bitmap[byte_index] & (1 << bit_index));
}

// Buscar la primera secuencia de `longitud` ceros consecutivos en el bitmap
int buscar_secuencia_libre(unsigned char *bitmap, int cantidad_de_bloques, int longitud) {
    for (int i = 0; i <= cantidad_de_bloques - longitud; i++) {
        int encontrado = 1;
        for (int j = 0; j < longitud; j++) {
            if (!esta_libre(bitmap, i + j)) {
                encontrado = 0;
                break;
            }
        }
        if (encontrado) {
            return i;
        }
    }
    return -1; // No se encontró una secuencia libre de la longitud requerida
}

// Marcar los bloques como ocupados en el bitmap
void marcar_bloques_ocupados(unsigned char *bitmap, int inicio, int longitud) {
    for (int i = 0; i < longitud; i++) {
        int pos = inicio + i;
        int byte_index = pos / 8;
        int bit_index = pos % 8;
        bitmap[byte_index] |= (1 << bit_index);
    }
}

// Crear archivo simulado del disco
void _crear_archivo_que_seria_el_disco() {
    int tamanio_del_archivo = tamanio_de_bloque * cantidad_de_bloques;
    crear_archivo("bloques.dat", tamanio_del_archivo);
}

// Crear archivo que contiene el bitmap
void _crear_archivo_que_contiene_el_bitmap(int cantidad_de_bloques) {
    int tamanio_bitmap = (cantidad_de_bloques + 7) / 8; // Redondeo hacia arriba
    FILE *archivo = fopen("bitmap.dat", "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo bitmap");
        return;
    }

    unsigned char *bitmap = (unsigned char*)malloc(tamanio_bitmap);
    if (bitmap == NULL) {
        perror("Error al reservar memoria para el bitmap");
        fclose(archivo);
        return;
    }

    memset(bitmap, 0, tamanio_bitmap);

    size_t escrito = fwrite(bitmap, 1, tamanio_bitmap, archivo);
    if (escrito != tamanio_bitmap) {
        perror("Error al escribir en el archivo bitmap");
    }

    free(bitmap);
    fclose(archivo);

    printf("Archivo 'bitmap.dat' creado con tamaño %d bytes.\n", tamanio_bitmap);
}

// Crear metadata para un archivo
void crear_metadata(const char *nombre_archivo, int bloque_inicial, int tamanio_archivo) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de metadata");
        return;
    }

    // Escribir metadata
    fprintf(archivo, "BLOQUE_INICIAL=%d\n", bloque_inicial);
    fprintf(archivo, "TAMANIO_ARCHIVO=%d\n", tamanio_archivo);

    // Cerrar el archivo
    fclose(archivo);

    printf("Archivo de metadata '%s' creado.\n", nombre_archivo);
}

// Crear un archivo con un tamaño específico
void crear_archivo(char *nombre, int tamanio_del_archivo) {
    FILE *archivo = fopen(nombre, "wb");
    // if (archivo == NULL) {
    //     perror("Error al abrir el archivo");
    //     return;
    // }

    // Reservar memoria del tamaño especificado
    char *buffer = (char*)malloc(tamanio_del_archivo);
    // if (buffer == NULL) {
    //     perror("Error al reservar memoria");
    //     fclose(archivo);
    //     return;
    // }

    // Rellenar el buffer con ceros
    memset(buffer, 0, tamanio_del_archivo);

    // Escribir el buffer en el archivo
    size_t escrito = fwrite(buffer, 1, tamanio_del_archivo, archivo);
    if (escrito != tamanio_del_archivo) {
        perror("Error al escribir en el archivo");
    }

    // Liberar la memoria reservada
    free(buffer);

    // Cerrar el archivo
    fclose(archivo);

    printf("Archivo '%s' creado con tamaño %d bytes.\n", nombre, tamanio_del_archivo);
}
