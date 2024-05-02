#include "../include/instrucciones.h"

void SET_uint8(uint8_t *registro, int valor)
{
    *registro = valor;
}
void SET_uint32(uint32_t *registro, int valor)
{
    *registro = valor;
}

void SET(void *registro, int valor)
{

    if (sizeof(*(uint8_t *)registro) == sizeof(uint8_t))
    {
        SET_uint8((uint8_t *)registro, valor);
    }
    else
    {
        SET_uint32((uint32_t *)registro, valor);
    }

    PC_registro++;
}

void SUM_8_a_8(uint8_t *destino, uint8_t *origen)
{
    *destino += *origen;
}

void SUM_32_a_32(uint32_t *destino, uint32_t *origen)
{
    *destino += *origen;
}

void SUM_8_a_32(uint32_t *destino, uint8_t *origen)
{
    *destino += (uint32_t)(*origen);
}

void SUM_32_a_8(uint8_t *destino, uint32_t *origen)
{
    *destino += (uint8_t)(*origen);
}

void SUM(void *registroDestino, void *registroOrigen)
{
    if (sizeof(*(uint8_t *)registroDestino) == sizeof(uint8_t) && sizeof(*(uint8_t *)registroOrigen) == sizeof(uint8_t))
    {
        SUM_8_a_8((uint8_t *)registroDestino, (uint8_t *)registroOrigen);
    }
    else if (sizeof(*(uint32_t *)registroDestino) == sizeof(uint32_t) && sizeof(*(uint32_t *)registroOrigen) == sizeof(uint32_t))
    {
        SUM_32_a_32((uint32_t *)registroDestino, (uint32_t *)registroOrigen);
    }
    else if (sizeof(*(uint8_t *)registroDestino) == sizeof(uint8_t) && sizeof(*(uint32_t *)registroOrigen) == sizeof(uint32_t))
    {
        SUM_8_a_32((uint32_t *)registroDestino, (uint8_t *)registroOrigen);
    }
    else if (sizeof(*(uint32_t *)registroDestino) == sizeof(uint32_t) && sizeof(*(uint8_t *)registroOrigen) == sizeof(uint8_t))
    {
        SUM_32_a_8((uint8_t *)registroDestino, (uint32_t *)registroOrigen);
    }

    PC_registro++;
}

void SUB_8_a_8(uint8_t *destino, uint8_t *origen)
{
    *destino -= *origen;
}

void SUB_32_a_32(uint32_t *destino, uint32_t *origen)
{
    *destino -= *origen;
}

void SUB_8_a_32(uint32_t *destino, uint8_t *origen)
{
    *destino -= (uint32_t)(*origen);
}

void SUB_32_a_8(uint8_t *destino, uint32_t *origen)
{
    *destino -= (uint8_t)(*origen);
}

void SUB(void *registroDestino, void *registroOrigen)
{
    if (sizeof(*(uint8_t *)registroDestino) == sizeof(uint8_t) && sizeof(*(uint8_t *)registroOrigen) == sizeof(uint8_t))
    {
        SUB_8_a_8((uint8_t *)registroDestino, (uint8_t *)registroOrigen);
    }
    else if (sizeof(*(uint32_t *)registroDestino) == sizeof(uint32_t) && sizeof(*(uint32_t *)registroOrigen) == sizeof(uint32_t))
    {
        SUB_32_a_32((uint32_t *)registroDestino, (uint32_t *)registroOrigen);
    }
    else if (sizeof(*(uint8_t *)registroDestino) == sizeof(uint8_t) && sizeof(*(uint32_t *)registroOrigen) == sizeof(uint32_t))
    {
        SUB_8_a_32((uint32_t *)registroDestino, (uint8_t *)registroOrigen);
    }
    else if (sizeof(*(uint32_t *)registroDestino) == sizeof(uint32_t) && sizeof(*(uint8_t *)registroOrigen) == sizeof(uint8_t))
    {
        SUB_32_a_8((uint8_t *)registroDestino, (uint32_t *)registroOrigen);
    }

    PC_registro++;
}

void JNZ(void *registro, unsigned int valor_salto)
{
    if (*(uint32_t *)registro != 0)
    {
        PC_registro = valor_salto;
    }
}

void IO_GEN_SLEEP(char *nombre_interfaz, uint8_t unidades_de_trabajo)
{

    PC_registro++;
    bloq_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [quantum] [registros] [nombre_interfaz] [operacion] [unidades_de_trabajo]

    cargar_uint16_al_buffer(buffer, PID);
    cargar_uint32_al_buffer(buffer, PC_registro);
    cargar_int8_al_buffer(buffer, QUANTUM);
    cargar_uint8_al_buffer(buffer, AX_registro);
    cargar_uint8_al_buffer(buffer, BX_registro);
    cargar_uint8_al_buffer(buffer, CX_registro);
    cargar_uint8_al_buffer(buffer, DX_registro);
    cargar_uint32_al_buffer(buffer, EAX_registro);
    cargar_uint32_al_buffer(buffer, EBX_registro);
    cargar_uint32_al_buffer(buffer, ECX_registro);
    cargar_uint32_al_buffer(buffer, EDX_registro);
    cargar_uint32_al_buffer(buffer, SI_registro);
    cargar_uint32_al_buffer(buffer, DI_registro);
    cargar_string_al_buffer(buffer, nombre_interfaz);
    cargar_string_al_buffer(buffer, "IO_GEN_SLEEP");
    cargar_uint8_al_buffer(buffer, unidades_de_trabajo);

    t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);

    // SETEAR REGISTROS EN CERO???
}
void EXIT()
{
    bloq_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [quantum] [registros]

    cargar_uint16_al_buffer(buffer, PID);
    cargar_uint32_al_buffer(buffer, PC_registro);
    cargar_int8_al_buffer(buffer, QUANTUM);
    cargar_uint8_al_buffer(buffer, AX_registro);
    cargar_uint8_al_buffer(buffer, BX_registro);
    cargar_uint8_al_buffer(buffer, CX_registro);
    cargar_uint8_al_buffer(buffer, DX_registro);
    cargar_uint32_al_buffer(buffer, EAX_registro);
    cargar_uint32_al_buffer(buffer, EBX_registro);
    cargar_uint32_al_buffer(buffer, ECX_registro);
    cargar_uint32_al_buffer(buffer, EDX_registro);
    cargar_uint32_al_buffer(buffer, SI_registro);
    cargar_uint32_al_buffer(buffer, DI_registro);

    t_paquete *a_enviar = crear_paquete(DESALOJO_POR_EXIT, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);
}
