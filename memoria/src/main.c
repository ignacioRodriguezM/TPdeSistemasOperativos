#include "../include/main.h"


int main() {
    APP_config config_valores = cargar_configuracion_memoria();
    printf("Puerto Escucha: %d\n", config_valores.puerto_escucha); //linea solo para testear funcionamienito, dsp borrar
    return 0;
}