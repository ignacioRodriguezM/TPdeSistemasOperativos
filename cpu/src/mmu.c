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



Direcciones _componer_direcciones(void *registroDireccion, uint8_t tam_direccion, uint8_t espacio)
{

    Direcciones direcciones;
    uint32_t valor_registro_direccion;
    
    if(tam_direccion == sizeof(uint32_t)){
    valor_registro_direccion = *(uint32_t *)registroDireccion;
    }
    else{
    valor_registro_direccion = *(uint8_t *)registroDireccion;
    }


    direcciones.cantidad_direcciones = 1;

    Direccion_t direccion_fisica;
    direccion_fisica.numero_pagina = floor(valor_registro_direccion / tam_pagina);
    direccion_fisica.desplazamiento = valor_registro_direccion - (direccion_fisica.numero_pagina * tam_pagina);

    if (tam_pagina - direccion_fisica.desplazamiento < espacio)
    {
        direccion_fisica.tamanio = tam_pagina - direccion_fisica.desplazamiento;
        uint8_t cantidad_de_paginas_a_ocupar = division_entera_redondear_arriba((espacio - direccion_fisica.tamanio) , tam_pagina) + 1;

        direcciones.direcciones = malloc((cantidad_de_paginas_a_ocupar) * sizeof(Direccion_t));
       
        direcciones.cantidad_direcciones = cantidad_de_paginas_a_ocupar;

        uint8_t desplazamiento_final = (espacio - direccion_fisica.tamanio) % tam_pagina;

        direccion_fisica.tamanio = tam_pagina - direccion_fisica.desplazamiento;
        
        direcciones.direcciones[0] = direccion_fisica;

        for (int i = 0; i < (cantidad_de_paginas_a_ocupar - 1); i++)
        {

            Direccion_t siguiente_direccion;
            siguiente_direccion.numero_pagina = i + 1 + direccion_fisica.numero_pagina;

            if (i == (cantidad_de_paginas_a_ocupar - 2))
            {
                siguiente_direccion.desplazamiento = 0;
                siguiente_direccion.tamanio = desplazamiento_final;
            }

            else
            {
                siguiente_direccion.desplazamiento = 0;
                siguiente_direccion.tamanio = tam_pagina;
            }

            direcciones.direcciones[i + 1] = siguiente_direccion;
        }
    }
    else
    {
        
        direccion_fisica.tamanio = espacio;

        direcciones.direcciones = malloc( sizeof(Direccion_t));
        direcciones.direcciones[0] = direccion_fisica;
    }

    return direcciones;
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

Direcciones traducir_direccion_logica_a_fisicas(void *registroDireccion, uint8_t tam_registro_direccion, uint8_t espacio)
{

    Direcciones direcciones = _componer_direcciones(registroDireccion, tam_registro_direccion, espacio);

    
    for (int i = 0; i < direcciones.cantidad_direcciones; i++)
    {
        uint16_t marco = consultar_tlb(PID, direcciones.direcciones[i].numero_pagina);

        if (marco != 65535)
        {
            
            log_info(cpu_logger, "PID: <%u> - MARCO OBTENIDO POR TLB - Página: <%u> - Marco: <%u>", PID, direcciones.direcciones[i].numero_pagina, marco);
            direcciones.direcciones[i].numero_pagina = marco;
            direcciones.direcciones[i].desplazamiento = direcciones.direcciones[i].desplazamiento;
        }
        else
        {

            marco = solicitar_marco_a_memoria(PID, direcciones.direcciones[i].numero_pagina);


            log_info(cpu_logger, "PID: <%u> - OBTENER MARCO - Página: <%u> - Marco: <%u>", PID, direcciones.direcciones[i].numero_pagina, marco);

            actualizar_tlb(PID, direcciones.direcciones[i].numero_pagina, marco);

            direcciones.direcciones[i].numero_pagina = marco;
            direcciones.direcciones[i].desplazamiento = direcciones.direcciones[i].desplazamiento;
        }
    }

    return direcciones;
}
