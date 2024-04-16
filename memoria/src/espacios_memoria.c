#include "../include/espacios_memoria.h"

void _asignar_valores_a_variables_globales_de_memoria (APP_config config_valores){
    
    espacio_memoria_usuario = malloc(config_valores.tam_memoria);
    tam_pagina = config_valores.tam_pagina;
    retardo_respuesta = config_valores.retardo_respuesta;

    cantidad_de_paginas = config_valores.tam_memoria / tam_pagina;
    tabla_de_paginas = malloc (cantidad_de_paginas * sizeof(void*));


    for (int i =0; i < cantidad_de_paginas ; i++){
        tabla_de_paginas[i] = espacio_memoria_usuario + tam_pagina * i;
    }
}

void _liberar_memoria_asignada_a_usuario (){
    
    free(espacio_memoria_usuario);
}