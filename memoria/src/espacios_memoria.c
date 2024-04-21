#include "../include/espacios_memoria.h"

// Función para inicializar la memoria y la tabla de páginas
Memory* inicializar_memoria() {
    Memory* mem = malloc(sizeof(Memory));
    mem->memory = malloc(tam_memoria);
    mem->tabla_de_paginas.num_paginas = tam_memoria / tam_pagina;
    mem->tabla_de_paginas.paginas = malloc(mem->tabla_de_paginas.num_paginas * sizeof(Page));
    
    // Inicializar cada página en la tabla de páginas
    for (int i = 0; i < mem->tabla_de_paginas.num_paginas; i++) {
        mem->tabla_de_paginas.paginas[i].numero_de_esta_pagina = i;
        mem->tabla_de_paginas.paginas[i].data = mem->memory + (i * tam_pagina);
    }
    
    return mem;
}

// Función para liberar la memoria y la tabla de páginas
void free_memory(Memory* mem) {
    free(mem->memory);
    free(mem->tabla_de_paginas.paginas);
    free(mem);
}