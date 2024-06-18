#include "../include/filesystem.h"
#include "../include/configuracion_entrada_salida.h"

// Función para leer el bitmap desde el archivo
t_bitarray* leer_bitmap() {

    FILE *archivo = fopen("bitmap.dat", "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo bitmap");
        return NULL;
    }

    char *bitarray = (char*)malloc(tamanio_bitarray_en_bytes);
    if (bitarray == NULL) {
        perror("Error al reservar memoria para el bitmap");
        fclose(archivo);
        return NULL;
    }

    size_t leido = fread(bitarray, 1, tamanio_bitarray_en_bytes, archivo);
    if (leido != tamanio_bitarray_en_bytes) {
        perror("Error al leer el archivo bitmap");
        free(bitarray);
        fclose(archivo);
        return NULL;
    }

    t_bitarray* bitmap = bitarray_create_with_mode(bitarray, tamanio_bitarray_en_bytes, LSB_FIRST);

    fclose(archivo);
    return bitmap;
}

// Escribir el bitmap actualizado de vuelta al archivo
void actualizar_archivo_bitmap (t_bitarray* bitmap)
{
        FILE *archivo = fopen("bitmap.dat", "wb");
        if (archivo == NULL) {
            perror("Error al abrir el archivo bitmap para escritura");
            free(bitmap);
            return;
        }

        fwrite(bitmap->bitarray, 1, tamanio_bitarray_en_bytes, archivo);
        fclose(archivo);

        printf("Bloques marcados como ocupados y bitmap actualizado.\n");
}


// Buscar la primera secuencia de `longitud` ceros consecutivos en el bitmap
int buscar_secuencia_libre(t_bitarray* bitmap, int longitud) {
    for (int i = 0; i <= cantidad_de_bloques - longitud; i++) {
        bool encontrado = true;
        for (int j = 0; j < longitud; j++) {
            if (bitarray_test_bit(bitmap, i + j)) {
                encontrado = false;
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
void marcar_bloques_ocupados(t_bitarray* bitmap, int bloque_inicio, int longitud) {
    for (int i = 0; i < longitud; i++) {
        bitarray_set_bit(bitmap, bloque_inicio + i);
    }
}


// Crear metadata para un archivo
void crear_metadata(const char *nombre_archivo, int bloque_inicial, int tamanio_archivo_en_bytes) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (archivo == NULL) {
        perror("Error al abrir el archivo de metadata");
        return;
    }

    // Escribir metadata
    fprintf(archivo, "BLOQUE_INICIAL=%d\n", bloque_inicial);
    fprintf(archivo, "TAMANIO_ARCHIVO=%d\n", tamanio_archivo_en_bytes);

    // Cerrar el archivo
    fclose(archivo);

    printf("Archivo de metadata '%s' creado.\n", nombre_archivo);
}

// Crear un archivo con un tamaño específico
void crear_archivo(char *nombre, int tamanio_del_archivo) {
    FILE *archivo = fopen(nombre, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    // Reservar memoria del tamaño especificado
    char *buffer = (char*)malloc(tamanio_del_archivo);
    if (buffer == NULL) {
         perror("Error al reservar memoria");
         fclose(archivo);
         return;
     }

    // Rellenar el buffer con ceros
    memset(buffer, 0, tamanio_del_archivo); // es necesario?

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



/////////////////////////////////////////////////////////////////////
///////////////////////// INICIALIZACIONES///////////////////////////
/////////////////////////////////////////////////////////////////////



void inicializar_estructuras_filesystem (){
    _crear_archivo_que_seria_el_disco();
    _crear_archivo_que_contiene_el_bitmap();
    return;
}

// Crear archivo simulado del disco
void _crear_archivo_que_seria_el_disco() {
    int tamanio_del_archivo = tamanio_de_bloque * cantidad_de_bloques;
    crear_archivo("bloques.dat", tamanio_del_archivo);
}

// Crear archivo que contiene el bitmap
void _crear_archivo_que_contiene_el_bitmap() {

    int cantidad_de_bits = cantidad_de_bloques;
    tamanio_bitarray_en_bytes = division_entera_redondear_arriba(cantidad_de_bits, 8); // 8 bits son 1 byte.
    char* bitarray = (char*)malloc(tamanio_bitarray_en_bytes);

    memset(bitarray, 0, tamanio_bitarray_en_bytes);

    t_bitarray* bitmap = bitarray_create_with_mode(bitarray, tamanio_bitarray_en_bytes, LSB_FIRST);

    
    FILE *archivo = fopen("bitmap.dat", "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo bitmap");
        return;
    }

    size_t escrito = fwrite(bitmap->bitarray, 1, bitmap->size, archivo);
    if (escrito != bitmap->size) {
        perror("Error al escribir en el archivo bitmap");
    }

    printf("Archivo 'bitmap.dat' creado con tamaño %lu bytes.\n", bitmap->size);

    bitarray_destroy(bitmap);
    fclose(archivo);

}

