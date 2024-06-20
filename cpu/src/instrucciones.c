#include "../include/instrucciones.h"

void SET(void *registro, void *parametro, uint8_t tam_registro)
{
    if (tam_registro == sizeof(uint8_t))
    {
        uint8_t valor = *(uint8_t *)parametro;
        *(uint8_t *)registro = valor;
    }
    else
    {
        uint32_t valor = *(uint32_t *)parametro;
        *(uint32_t *)registro = valor;
    }

    PC_registro++;
}

void SUM(void *registroDestino, void *registroOrigen, uint8_t tamDestino, uint8_t tamOrigen)
{
    if (tamDestino == sizeof(uint8_t) && tamOrigen == sizeof(uint8_t))
    {
        *(uint8_t *)registroDestino += *(uint8_t *)registroOrigen;
    }
    else if (tamDestino == sizeof(uint8_t) && tamOrigen == sizeof(uint32_t))
    {
        *(uint8_t *)registroDestino += *(uint32_t *)registroOrigen;
    }
    else if (tamDestino == sizeof(uint32_t) && tamOrigen == sizeof(uint8_t))
    {
        *(uint32_t *)registroDestino += *(uint8_t *)registroOrigen;
    }
    else
    {
        *(uint32_t *)registroDestino += *(uint32_t *)registroOrigen;
    }

    PC_registro++;
}

void SUB(void *registroDestino, void *registroOrigen, uint8_t tamDestino, uint8_t tamOrigen)
{
    if (tamDestino == sizeof(uint8_t) && tamOrigen == sizeof(uint8_t))
    {
        *(uint8_t *)registroDestino -= *(uint8_t *)registroOrigen;
    }
    else if (tamDestino == sizeof(uint8_t) && tamOrigen == sizeof(uint32_t))
    {
        *(uint8_t *)registroDestino -= *(uint32_t *)registroOrigen;
    }
    else if (tamDestino == sizeof(uint32_t) && tamOrigen == sizeof(uint8_t))
    {
        *(uint32_t *)registroDestino -= *(uint8_t *)registroOrigen;
    }
    else
    {
        *(uint32_t *)registroDestino -= *(uint32_t *)registroOrigen;
    }

    PC_registro++;
}

void JNZ(void *registro, void *parametro)
{
    unsigned int valor_salto = *(unsigned int *)parametro;
    if (*(uint32_t *)registro != 0)
    {
        PC_registro = valor_salto;
    }
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

void WAIT(void *parametro)
{
    char *nombre_recurso = (char *)parametro;

    PC_registro++;
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
    int cod_op = recibir_operacion(fd_kernel_dispatch);
    switch (cod_op)
    {
    case RESPUESTA_RECURSO:
        t_buffer *buff = recibir_buffer_sin_cod_op(fd_kernel_dispatch);
        char *respuesta = extraer_string_al_buffer(buff);
        if (strcmp(respuesta, "BLOQUEADO") == 0)
        {
            bloq_flag = false;
            interrupt_flag = false;
        }
        free(respuesta);
        break;
    }
}
void SIGNAL(void *parametro)
{
    char *nombre_recurso = (char *)parametro;

    PC_registro++;
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

    int cod_op = recibir_operacion(fd_kernel_dispatch);
    switch (cod_op)
    {
    case RESPUESTA_RECURSO:
        t_buffer *buff = recibir_buffer_sin_cod_op(fd_kernel_dispatch);
        char *respuesta = extraer_string_al_buffer(buff);
        if (strcmp(respuesta, "BLOQUEADO") == 0)
        {
            bloq_flag = false;
            interrupt_flag = false;
        }
        free(respuesta);
        break;
    }
}
void RESIZE(void *parametro)
{

    uint16_t tamanio_ajustado = *(uint16_t *)parametro;

    PC_registro++;

    t_buffer *buffer_a_enviar = crear_buffer();
    cargar_uint16_al_buffer(buffer_a_enviar, PID);
    cargar_uint16_al_buffer(buffer_a_enviar, tamanio_ajustado);
    t_paquete *a_enviar = crear_paquete(AJUSTAR_TAMANIO_PROCESO, buffer_a_enviar);
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

        if (strcmp(mensaje_de_respuesta, "Out Of Memory") == 0)
        {
            bloq_flag = false;
            interrupt_flag = false;
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

        free(mensaje_de_respuesta);

        break;
    }
}

// Lee el valor de memoria correspondiente a la Dirección Lógica que se encuentra en el Registro Dirección y lo almacena en el Registro Datos.
void MOV_IN(void *registroDatos, void *registroDireccion, uint8_t tamanio_de_registro_datos, uint8_t tamanio_de_registro_direccion)
{ // lee de memoria y lo guarda en un registro
    // aca debemos agregar la cantidad de marcos, etc respetando la manera de leer que esta en memoria
    PC_registro++;
    Direcciones direcciones_fisicas = traducir_direccion_logica_a_fisicas(registroDireccion, tamanio_de_registro_direccion, tamanio_de_registro_datos);

    t_buffer *solicitud_de_lectura = crear_buffer();

    cargar_uint8_al_buffer(solicitud_de_lectura, tamanio_de_registro_datos);
    //  [Cantidad] [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .....
    uint8_t cantidad = direcciones_fisicas.cantidad_direcciones;

    cargar_uint8_al_buffer(solicitud_de_lectura, cantidad);

    for (int i = 0; i < cantidad; i++)
    {
        cargar_uint8_al_buffer(solicitud_de_lectura, direcciones_fisicas.direcciones[i].tamanio);
        cargar_uint16_al_buffer(solicitud_de_lectura, direcciones_fisicas.direcciones[i].numero_pagina);
        cargar_uint32_al_buffer(solicitud_de_lectura, direcciones_fisicas.direcciones[i].desplazamiento);
    }

    t_paquete *a_enviar = crear_paquete(LECTURA, solicitud_de_lectura);

    enviar_paquete(a_enviar, fd_memoria);

    destruir_paquete(a_enviar);

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case LECTURA:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);
        if (tamanio_de_registro_datos == sizeof(uint8_t))
        {
            *(uint8_t *)registroDatos = extraer_uint8_al_buffer(recibido);
            uint32_t direccion_para_loguear = tam_pagina * direcciones_fisicas.direcciones[0].numero_pagina + direcciones_fisicas.direcciones[0].desplazamiento;
            log_info(cpu_logger, "PID: <%u> - Acción: <LEER> - Dirección Física: <%u> - Valor: <%u>", PID, direccion_para_loguear, *(uint8_t *)registroDatos);
        }
        else
        {
            *(uint32_t *)registroDatos = extraer_uint32_al_buffer(recibido);
            uint32_t direccion_para_loguear = tam_pagina * direcciones_fisicas.direcciones[0].numero_pagina + direcciones_fisicas.direcciones[0].desplazamiento;
            log_info(cpu_logger, "PID: <%u> - Acción: <LEER> - Dirección Física: <%u> - Valor: <%u>", PID, direccion_para_loguear, *(uint32_t *)registroDatos);
        }

        destruir_buffer(recibido);
        break;
    }
    free(direcciones_fisicas.direcciones);
}

// MOV_OUT (Registro Dirección, Registro Datos):
// Lee el valor del Registro Datos y lo escribe en la dirección física de memoria obtenida a partir de la Dirección Lógica almacenada en el Registro Dirección.
void MOV_OUT(void *registroDireccion, void *registroDatos, uint8_t tamanio_de_registro_direccion, uint8_t tamanio_de_registro_datos)
{
    PC_registro++;
    Direcciones direcciones_fisicas = traducir_direccion_logica_a_fisicas(registroDireccion, tamanio_de_registro_direccion, tamanio_de_registro_datos);

    t_buffer *solicitud_de_escritura = crear_buffer();

    //  [tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....

    cargar_uint8_al_buffer(solicitud_de_escritura, tamanio_de_registro_datos);

    // Cargar la cantidad de direcciones
    uint8_t cantidad = direcciones_fisicas.cantidad_direcciones;
    cargar_uint8_al_buffer(solicitud_de_escritura, cantidad);

    void *datos;
    if (tamanio_de_registro_datos == sizeof(uint8_t))
    {
        datos = (uint8_t *)registroDatos;
        uint32_t direccion_para_loguear = tam_pagina * direcciones_fisicas.direcciones[0].numero_pagina + direcciones_fisicas.direcciones[0].desplazamiento;

        log_info(cpu_logger, "PID: <%u> - Acción: <ESCRIBIR> - Dirección Física: <%u> - Valor: <%u>", PID, direccion_para_loguear, *(uint8_t *)datos);
    }
    else
    {
        datos = (uint32_t *)registroDatos;
        uint32_t direccion_para_loguear = tam_pagina * direcciones_fisicas.direcciones[0].numero_pagina + direcciones_fisicas.direcciones[0].desplazamiento;

        log_info(cpu_logger, "PID: <%u> - Acción: <ESCRIBIR> - Dirección Física: <%u> - Valor: <%u>", PID, direccion_para_loguear, *(uint32_t *)datos);
    }

    // [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....

    for (int i = 0; i < cantidad; i++)
    {
        cargar_uint8_al_buffer(solicitud_de_escritura, direcciones_fisicas.direcciones[i].tamanio);
        cargar_uint16_al_buffer(solicitud_de_escritura, direcciones_fisicas.direcciones[i].numero_pagina);
        cargar_uint32_al_buffer(solicitud_de_escritura, direcciones_fisicas.direcciones[i].desplazamiento);

        if (tamanio_de_registro_datos == sizeof(uint8_t))
        {
            cargar_uint8_al_buffer(solicitud_de_escritura, *(uint8_t *)datos);
        }
        else
        {
            cargar_uint32_al_buffer(solicitud_de_escritura, *(uint32_t *)datos);
        }

        datos += direcciones_fisicas.direcciones[i].tamanio;
    }

    t_paquete *a_enviar = crear_paquete(ESCRITURA, solicitud_de_escritura);
    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case ESCRITURA:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        char *respuesta = extraer_string_al_buffer(recibido);

        if (strcmp(respuesta, "OK") != 0)
        {
            log_error(cpu_log_debug, "MEMORIA NO PUDO REALIZAR EL MOV OUT");
        }

        free(respuesta);
        destruir_buffer(recibido);

        break;
    }
    free(direcciones_fisicas.direcciones);
}

void *obtener_string_de_memoria(Direcciones direcciones_fisicas, uint8_t tamanio_a_copiar)
{
    t_buffer *solicitud_de_lectura = crear_buffer();

    cargar_uint8_al_buffer(solicitud_de_lectura, tamanio_a_copiar);
    //  [Cantidad] [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_LEER] [MARCO] [DESPLAZAMIENTO] .....
    uint8_t cantidad = direcciones_fisicas.cantidad_direcciones;

    cargar_uint8_al_buffer(solicitud_de_lectura, cantidad);

    for (int i = 0; i < cantidad; i++)
    {
        cargar_uint8_al_buffer(solicitud_de_lectura, direcciones_fisicas.direcciones[i].tamanio);
        cargar_uint16_al_buffer(solicitud_de_lectura, direcciones_fisicas.direcciones[i].numero_pagina);
        cargar_uint32_al_buffer(solicitud_de_lectura, direcciones_fisicas.direcciones[i].desplazamiento);
    }

    t_paquete *a_enviar = crear_paquete(LECTURA, solicitud_de_lectura);

    enviar_paquete(a_enviar, fd_memoria);

    destruir_paquete(a_enviar);

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case LECTURA:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);
        void *string_copiado = extraer_choclo_al_buffer(recibido);
        uint32_t direccion_para_loguear = tam_pagina * direcciones_fisicas.direcciones[0].numero_pagina + direcciones_fisicas.direcciones[0].desplazamiento;
        log_info(cpu_logger, "PID: <%u> - Acción: <LEER> - Dirección Física: <%u> - Valor: <%s>", PID, direccion_para_loguear, (char *)string_copiado);

        destruir_buffer(recibido);

        return string_copiado;
        break;
    }
    return NULL;
}
void escribir_string_en_memoria(Direcciones direcciones_fisicas, void *string_a_escribir, uint8_t tamanio)
{
    t_buffer *solicitud_de_escritura = crear_buffer();

    //  [tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....

    cargar_uint8_al_buffer(solicitud_de_escritura, tamanio);

    // Cargar la cantidad de direcciones
    uint8_t cantidad = direcciones_fisicas.cantidad_direcciones;
    cargar_uint8_al_buffer(solicitud_de_escritura, cantidad);

    void *datos = string_a_escribir;

    uint32_t direccion_para_loguear = tam_pagina * direcciones_fisicas.direcciones[0].numero_pagina + direcciones_fisicas.direcciones[0].desplazamiento;

    log_info(cpu_logger, "PID: <%u> - Acción: <ESCRIBIR> - Dirección Física: <%u> - Valor: <%s>", PID, direccion_para_loguear, (char *)string_a_escribir);

    // [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [DATO] .....

    for (int i = 0; i < cantidad; i++)
    {
        cargar_uint8_al_buffer(solicitud_de_escritura, direcciones_fisicas.direcciones[i].tamanio);
        cargar_uint16_al_buffer(solicitud_de_escritura, direcciones_fisicas.direcciones[i].numero_pagina);
        cargar_uint32_al_buffer(solicitud_de_escritura, direcciones_fisicas.direcciones[i].desplazamiento);

        cargar_choclo_al_buffer(solicitud_de_escritura, datos, direcciones_fisicas.direcciones[i].tamanio);

        datos += direcciones_fisicas.direcciones[i].tamanio;
    }

    t_paquete *a_enviar = crear_paquete(ESCRITURA, solicitud_de_escritura);
    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case ESCRITURA:

        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        char *respuesta = extraer_string_al_buffer(recibido);

        if (strcmp(respuesta, "OK") != 0)
        {
            log_error(cpu_log_debug, "MEMORIA NO PUDO REALIZAR EL COPY STRING");
        }

        free(respuesta);
        destruir_buffer(recibido);

        break;
    }
}

void COPY_STRING(uint8_t tamanio_a_copiar)
{
    // uint32_t registroOrigen = SI_registro;
    // uint32_t registroDestino = DI_registro;

    PC_registro++;
    uint8_t tamanio = sizeof(uint32_t);
    Direcciones direcciones_fisicas_origen = traducir_direccion_logica_a_fisicas(&SI_registro, tamanio, tamanio_a_copiar);

    void *string_a_escribir = obtener_string_de_memoria(direcciones_fisicas_origen, tamanio_a_copiar);

    Direcciones direcciones_fisicas_destino = traducir_direccion_logica_a_fisicas(&DI_registro, tamanio, tamanio_a_copiar);

    escribir_string_en_memoria(direcciones_fisicas_destino, string_a_escribir, tamanio_a_copiar);

    free(string_a_escribir);
    free(direcciones_fisicas_origen.direcciones);
    free(direcciones_fisicas_destino.direcciones);
}

// ---------------------------ENTRADA SALIDA----------------------------------------------
void IO_GEN_SLEEP(void *parametro, void *parametro2)
{
    char *nombre_interfaz = (char *)parametro;

    uint8_t unidades_de_trabajo = *(uint8_t *)parametro2;

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


void IO_STDIN_READ(void *nombre_de_la_interfaz, void *registro_direccion, void *registro_tamanio, uint8_t tamanio_de_registro_direccion, uint8_t tamanio_del_registro)
{

    char *nombre_interfaz = (char *)nombre_de_la_interfaz;

    uint8_t tamanio;

    if (sizeof(uint8_t) == tamanio_del_registro)
    {
        tamanio = *(uint8_t *)registro_tamanio;
    }
    else
    {
        tamanio = *(uint8_t *)registro_tamanio;
    }

    Direcciones direcciones_fisicas = traducir_direccion_logica_a_fisicas(registro_direccion, tamanio_de_registro_direccion, tamanio);

    PC_registro++;
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_interfaz] [operacion]

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
    cargar_string_al_buffer(buffer, "IO_STDIN_READ");

    //[tamanio registro datos] [Cantidad] [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [FALTA DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [FALTA DATO] .....

    cargar_uint8_al_buffer(buffer, tamanio);

    // Cargar la cantidad de direcciones
    uint8_t cantidad = direcciones_fisicas.cantidad_direcciones;
    cargar_uint8_al_buffer(buffer, cantidad);

    // [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [FALTA DATO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] [FALTA DATO] .....

    for (int i = 0; i < cantidad; i++)
    {
        cargar_uint8_al_buffer(buffer, direcciones_fisicas.direcciones[i].tamanio);
        cargar_uint16_al_buffer(buffer, direcciones_fisicas.direcciones[i].numero_pagina);
        cargar_uint32_al_buffer(buffer, direcciones_fisicas.direcciones[i].desplazamiento);
    }

    t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);

    free(direcciones_fisicas.direcciones);
}

void IO_STDOUT_WRITE(void *nombre_de_la_interfaz, void *registro_direccion, void *registro_tamanio, uint8_t tamanio_de_registro_direccion, uint8_t tamanio_del_registro)
{

    char *nombre_interfaz = (char *)nombre_de_la_interfaz;

    uint8_t tamanio;

    if (sizeof(uint8_t) == tamanio_del_registro)
    {
        tamanio = *(uint8_t *)registro_tamanio;
    }
    else
    {
        tamanio = *(uint8_t *)registro_tamanio;
    }

    Direcciones direcciones_fisicas = traducir_direccion_logica_a_fisicas(registro_direccion, tamanio_de_registro_direccion, tamanio);

    PC_registro++;
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_interfaz] [operacion]

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
    cargar_string_al_buffer(buffer, "IO_STDOUT_WRITE");

    //[tamanio registro datos] [Cantidad] [TAM_A_leer] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] .....

    cargar_uint8_al_buffer(buffer, tamanio);

    // Cargar la cantidad de direcciones
    uint8_t cantidad = direcciones_fisicas.cantidad_direcciones;
    cargar_uint8_al_buffer(buffer, cantidad);

    // [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] .....

    for (int i = 0; i < cantidad; i++)
    {
        cargar_uint8_al_buffer(buffer, direcciones_fisicas.direcciones[i].tamanio);
        cargar_uint16_al_buffer(buffer, direcciones_fisicas.direcciones[i].numero_pagina);
        cargar_uint32_al_buffer(buffer, direcciones_fisicas.direcciones[i].desplazamiento);
    }

    t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);

    free(direcciones_fisicas.direcciones);
}


void IO_FS_CREATE(void *nombre_de_la_interfaz, void *nombre_del_archivo)
{ 
    char *nombre_interfaz = (char *)nombre_de_la_interfaz;
    char *nombre_archivo = (char *)nombre_del_archivo;

    PC_registro++;
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_interfaz] [operacion]

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
    cargar_string_al_buffer(buffer, "IO_FS_CREATE");
    cargar_string_al_buffer(buffer, nombre_archivo);

    t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);
}
void IO_FS_DELETE(void *nombre_de_la_interfaz, void *nombre_del_archivo)
{
    char *nombre_interfaz = (char *)nombre_de_la_interfaz;
    char *nombre_archivo = (char *)nombre_del_archivo;

    PC_registro++;
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_interfaz] [operacion]

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
    cargar_string_al_buffer(buffer, "IO_FS_DELETE");
    cargar_string_al_buffer(buffer, nombre_archivo);

    t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);
}
void IO_FS_TRUNCATE(void *nombre_de_la_interfaz, void *nombre_del_archivo, void *registro_tamanio, uint8_t tamanio_del_registro)
{
    char *nombre_interfaz = (char *)nombre_de_la_interfaz;
    char *nombre_archivo = (char *)nombre_del_archivo;

    PC_registro++;
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_interfaz] [operacion]

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
    cargar_string_al_buffer(buffer, "IO_FS_TRUNCATE");
    cargar_string_al_buffer(buffer, nombre_archivo);

    uint8_t tamanio;
    if (tamanio_del_registro == sizeof(uint8_t))
    {
        tamanio = (uint8_t *)registro_tamanio;
    }
    else
    {
        tamanio = (uint32_t *)registro_tamanio;
    }

    cargar_uint8_al_buffer(buffer, tamanio);

    t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);

}

void IO_FS_WRITE(void *nombre_de_la_interfaz, void *nombre_del_archivo, void *registro_direccion, void *registro_tamanio, void *registro_puntero_de_archivo, uint8_t tam_registro_dir, uint8_t tam_registro_tam, uint8_t tam_registro_puntero)
{
    char *nombre_interfaz = (char *)nombre_de_la_interfaz;
    char *nombre_archivo = (char *)nombre_del_archivo;

    uint8_t tamanio = *(uint8_t *)registro_tamanio;

    Direcciones direcciones_fisicas = traducir_direccion_logica_a_fisicas(registro_direccion, tam_registro_dir, tamanio);

    PC_registro++;
    bloq_flag = false;
    interrupt_flag = false;
    t_buffer *buffer = crear_buffer();
    //[pid] [pc] [registros] [nombre_interfaz] [operacion]

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
    cargar_string_al_buffer(buffer, "IO_FS_TRUNCATE");
    cargar_string_al_buffer(buffer, nombre_archivo);
    cargar_uint8_al_buffer(buffer, tamanio);

    // Cargar la cantidad de direcciones
    uint8_t cantidad = direcciones_fisicas.cantidad_direcciones;
    cargar_uint8_al_buffer(buffer, cantidad);

    // [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] .. [TAM_A_escribir] [MARCO] [DESPLAZAMIENTO] .....

    for (int i = 0; i < cantidad; i++)
    {
        cargar_uint8_al_buffer(buffer, direcciones_fisicas.direcciones[i].tamanio);
        cargar_uint16_al_buffer(buffer, direcciones_fisicas.direcciones[i].numero_pagina);
        cargar_uint32_al_buffer(buffer, direcciones_fisicas.direcciones[i].desplazamiento);
    }

    t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);

    free(direcciones_fisicas.direcciones);
}

void IO_FS_READ(void *nombre_de_la_interfaz, void *nombre_del_archivo, void *registro_direccion, void *registro_tamanio, void *registro_puntero_de_archivo, uint8_t tam_registro_dir, uint8_t tam_registro_tam, uint8_t tam_registro_puntero)
{







}
