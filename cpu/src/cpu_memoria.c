#include "../include/cpu_memoria.h"

void pedir_tam_pagina_a_memoria()
{
    t_buffer *buff = crear_buffer();
    cargar_uint16_al_buffer(buff, 0);
    t_paquete *vacio = crear_paquete(TAM_PAG, buff);
    enviar_paquete(vacio, fd_memoria);
    destruir_paquete(vacio);

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case TAM_PAG:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);
        uint16_t tamanio = extraer_uint16_al_buffer(recibido);

        tam_pagina = tamanio;
        log_trace(cpu_log_debug, "EL TAM DE PAG ES %u", tam_pagina);

        destruir_buffer(recibido);

        break;
    }
}

