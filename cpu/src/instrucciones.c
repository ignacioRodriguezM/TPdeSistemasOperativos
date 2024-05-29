#include "../include/instrucciones.h"

void SET_uint8(uint8_t *registro, int valor)
{
    *registro = valor;
}
void SET_uint32(uint32_t *registro, int valor)
{
    *registro = valor;
}

void SET(void *registro, void* parametro)
{
    int valor = *(int*)parametro;

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

void JNZ(void *registro, void* parametro)
{
    unsigned int valor_salto = *(unsigned int*)parametro;
    if (*(uint32_t *)registro != 0)
    {
        PC_registro = valor_salto;
    }
}

void IO_GEN_SLEEP(void* parametro, void* parametro2)
{
    char* nombre_interfaz = (char *) parametro;

    uint8_t unidades_de_trabajo = *(uint8_t*)parametro2;

    PC_registro++;
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_interfaz] [operacion] [unidades_de_trabajo]

    cargar_uint16_al_buffer(buffer, PID);
    cargar_uint32_al_buffer(buffer, PC_registro);
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

    
}
void EXIT()
{
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc]  [registros]

    cargar_uint16_al_buffer(buffer, PID);
    cargar_uint32_al_buffer(buffer, PC_registro);
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

void WAIT(void* parametro)
{
    char* nombre_recurso = (char *) parametro;

    PC_registro++;
    bloq_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_recurso]

    cargar_uint16_al_buffer(buffer, PID);
    cargar_uint32_al_buffer(buffer, PC_registro);
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
    cargar_string_al_buffer(buffer, nombre_recurso);
    

    t_paquete *a_enviar = crear_paquete(WAIT_op, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);

    
}
void SIGNAL (void* parametro)
{
    char* nombre_recurso = (char *) parametro;

    PC_registro++;
    bloq_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc]  [registros] [nombre_recurso]

    cargar_uint16_al_buffer(buffer, PID);
    cargar_uint32_al_buffer(buffer, PC_registro);
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
    cargar_string_al_buffer(buffer, nombre_recurso);
    

    t_paquete *a_enviar = crear_paquete(SIGNAL_op, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);

    
}
void RESIZE(void* parametro){

    uint16_t tamanio_ajustado = *(uint16_t*)parametro;

    PC_registro++;

    t_buffer* buffer_a_enviar = crear_buffer();
    cargar_uint16_al_buffer(buffer_a_enviar, PID);
    cargar_uint16_al_buffer(buffer_a_enviar, tamanio_ajustado);
    t_paquete* a_enviar = crear_paquete(AJUSTAR_TAMANIO_PROCESO, buffer_a_enviar);
    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case RESPUESTA_RESIZE:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        char *mensaje_de_respuesta = extraer_string_al_buffer(recibido);
        log_info(cpu_log_debug, "RESPUESTA MEMORIA : %s", mensaje_de_respuesta);
        destruir_buffer(recibido);

        if (strcmp(mensaje_de_respuesta, "Out Of Memory") == 0){
            t_buffer *buffer = crear_buffer();
            //[pid] [pc] [registros]

            cargar_uint16_al_buffer(buffer, PID);
            cargar_uint32_al_buffer(buffer, PC_registro);
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

            t_paquete *a_enviar = crear_paquete(OUT_OF_MEMORY, buffer);

            enviar_paquete(a_enviar, fd_kernel_dispatch);

            destruir_paquete(a_enviar);
        }
        
        break;
    }
}


//MOV_IN (void registroDatos, int registroDirección): 
//Lee el valor de memoria correspondiente a la Dirección Lógica que se encuentra en el Registro Dirección y lo almacena en el Registro Datos.
void MOV_IN (void *registroDatos, void* direccion_fisica){ //lee de memoria y lo guarda en un registro

    PC_registro++;
    uint16_t registroDireccion = *(uint16_t*)direccion_fisica;
    uint8_t tam_reg = sizeof(*registroDatos);

    t_buffer* buffer_a_enviar = crear_buffer();
    cargar_uint16_al_buffer(buffer_a_enviar, registroDireccion);
    cargar_uint8_al_buffer(buffer_a_enviar, tam_reg);
    t_paquete* a_enviar = crear_paquete(MOVE_IN, buffer_a_enviar);
    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case RESPUESTA_MOVES:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);
        
        registroDatos = extraer_choclo_al_buffer(recibido);
        
        destruir_buffer(recibido);
        
        break;
    }
}

//MOV_OUT (Registro Dirección, Registro Datos): 
//Lee el valor del Registro Datos y lo escribe en la dirección física de memoria obtenida a partir de la Dirección Lógica almacenada en el Registro Dirección.
void MOV_OUT (void *direccion_fisica, void* registroDatos){ //escribe en memoria
    PC_registro++;

    
}

