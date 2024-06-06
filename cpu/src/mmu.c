#include "../include/mmu.h"

#define ERROR_VALUE 0xFFFF

double floor(double x)
{
    int result = (int)x; // Truncate the decimal part
    // Check if x is negative and has a non-zero fractional part
    if (x < 0 && x != result)
    {
        result -= 1;
    }
    return (double)result;
}

Direccion_t _componer_direccion_logica(void *registroDireccion)
{

    uint32_t valor_registro_direccion = *(uint32_t *)registroDireccion;

    Direccion_t direccion_logica;

    direccion_logica.numero_pagina = floor(valor_registro_direccion / tam_pagina);

    direccion_logica.numero_pagina = valor_registro_direccion - (direccion_logica.numero_pagina * tam_pagina);

    return direccion_logica;
}

uint16_t solicitar_marco_a_memoria(uint16_t pid, uint16_t pagina)
{

    t_buffer *bufferx = crear_buffer();
    cargar_uint16_al_buffer(bufferx, pid);
    cargar_uint16_al_buffer(bufferx, pagina);
    t_paquete *a_enviar = crear_paquete(ACCESO_A_TABLA_DE_PAG, bufferx);

    //  [uint16 PID] [uint16 PAGINA]

    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);

    int cod_op = recibir_operacion(fd_memoria);

    switch (cod_op)
    {
    case ACCESO_A_TABLA_DE_PAG:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        uint16_t marco_consultado = extraer_uint16_al_buffer(recibido);

        destruir_buffer(recibido);

        return marco_consultado;

        break;
    default:
    
        log_error(cpu_log_debug, "solicitar_marco_a_memoria rompio");
        return 0;
        break;
    }
}

Direccion_t traducir_direccion_logica_a_fisica(void *registroDireccion)
{

    Direccion_t direccion_logica = _componer_direccion_logica(registroDireccion);

    uint16_t marco = consultar_tlb(PID, direccion_logica.numero_pagina);

    Direccion_t direccion_fisica;

    if (marco != ERROR_VALUE)
    {
        uint16_t pagina = floor(*(uint32_t *)registroDireccion / tam_pagina);

        log_info(cpu_logger, "PID: <%u> - OBTENER MARCO - Página: <%u> - Marco: <%u>", PID, pagina, marco);
        direccion_fisica.numero_pagina = marco;
        direccion_fisica.desplazamiento = direccion_logica.desplazamiento;

        return direccion_fisica;
    }

    marco = solicitar_marco_a_memoria(PID, direccion_logica.numero_pagina);

    uint16_t pagina = floor(*(uint32_t *)registroDireccion / tam_pagina);

    log_info(cpu_logger, "PID: <%u> - OBTENER MARCO - Página: <%u> - Marco: <%u>", PID, pagina, marco);

    actualizar_tlb(PID, direccion_logica.numero_pagina, marco);

    direccion_fisica.numero_pagina = marco;
    direccion_fisica.desplazamiento = direccion_logica.desplazamiento;

    return direccion_fisica;
}