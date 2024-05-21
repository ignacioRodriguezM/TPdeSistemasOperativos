#include "../include/tabla_de_paginas.h"

void crear_memoria_y_dividir_en_marcos(){
    memoria_usuario = malloc(tam_memoria);
    cantidad_de_marcos = tam_memoria / tam_pagina;

    marcos_memoria = malloc( cantidad_de_marcos * sizeof(Marco*));

    for(int i=0; i<cantidad_de_marcos; i++){
        marcos_memoria[i] = malloc(sizeof(Marco));
        marcos_memoria[i]->esta_libre = true;
        marcos_memoria[i]->inicio_marco = memoria_usuario + i * tam_pagina;
    }
}
void crear_tabla_de_paginas (Proceso* proceso){
    proceso->tabla_de_paginas = malloc(sizeof(Tabla_paginas*));
    proceso->cantidad_paginas = 0;
}