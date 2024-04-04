#include "../include/main.h"


int main() {
    APP_config config_valores = cargar_configuracion_entrada_salida ();
    printf("Tiempo unidad de trabajo: %d\n", config_valores.tiempo_unidad_trabajo);//linea solo para testear funcionamienito, dsp borrar
 
    return 0;
}