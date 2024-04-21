#ifndef M_GESTOR_H_
#define M_GESTOR_H_

#include <utils/shared.h>

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>


//Variables GLOBALES
extern t_log* memoria_logger;
extern t_log* memoria_log_debug;

extern int fd_memoria;         //file descriptor de memoria
extern int fd_entrada_salida;
extern int fd_cpu;
extern int fd_kernel;

extern uint16_t tam_memoria; 
extern uint16_t tam_pagina;
extern uint8_t retardo_respuesta;

// Estructura para representar una página de memoria
typedef struct {
    int numero_de_esta_pagina;    // Número de página
    void* data;         // Datos de la página
} Page;

// Estructura para representar una tabla de páginas
typedef struct {
    Page* paginas;        // Array de páginas
    int num_paginas;      // Número total de páginas
} PageTable;

// Estructura para representar la memoria
typedef struct {
    void* memory;       // Puntero al espacio de memoria de usuario
    PageTable tabla_de_paginas; // Tabla de páginas
} Memory;




#endif