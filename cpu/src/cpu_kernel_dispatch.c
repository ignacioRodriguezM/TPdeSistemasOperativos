#include "../include/cpu_kernel_dispatch.h"
void IO_GEN_SLEEP_funcion (char* nombre_interfaz, uint8_t unidades_de_trabajo){




    t_buffer *buffer = crear_buffer();
        //[nombre_interfaz] [operacion] [unidades_de_trabajo] [pid] [pc] [quantum] [registros]
        
        cargar_string_al_buffer(buffer, nombre_interfaz);
        cargar_string_al_buffer(buffer, "IO_GEN_SLEEP");
        cargar_uint8_al_buffer(buffer, unidades_de_trabajo);
        cargar_uint16_al_buffer(buffer, PID);
        cargar_uint32_al_buffer(buffer, PC_registro);
        cargar_uint8_al_buffer(buffer, QUANTUM);
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


        t_paquete *a_enviar = crear_paquete(LLAMADA_A_IO, buffer);

        enviar_paquete(a_enviar, fd_kernel_dispatch);
        
        destruir_paquete(a_enviar);
        
        //SETEAR REGISTROS EN CERO???
            

}

void atender_cpu_kernel_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_dispatch);
        switch (cod_op)
        {
        case PAQUETE:
            //
            break;

        case PROCESO_A_EJECUTAR:

            _extraer_contexto_de_ejecucion();
            log_info(cpu_logger, "LLego %u", PID);
            //_ejecutar_ciclo_de_cpu ();
            IO_GEN_SLEEP_funcion ("mouse", 10);

            
            break;

        case -1:
            log_error(cpu_logger, "Desconexion de KERNEL - dispatch");
            control_key = 0;
            break;
        default:
            log_warning(cpu_logger, "Operacion desconocida de KERNEL - dispatch");
            break;
        }
    }
}







void _extraer_contexto_de_ejecucion()
{

    t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_kernel_dispatch);
    // EXTRAIGO EN ORDEN [PID] [PC] [QUANTUUM] [REGISTROS]
    PID = extraer_uint16_al_buffer(buffer_recibido);
    PC_registro = extraer_uint32_al_buffer(buffer_recibido);
    QUANTUM = extraer_uint8_al_buffer(buffer_recibido);
    AX_registro = extraer_uint8_al_buffer(buffer_recibido);
    BX_registro = extraer_uint8_al_buffer(buffer_recibido);
    CX_registro = extraer_uint8_al_buffer(buffer_recibido);
    DX_registro = extraer_uint8_al_buffer(buffer_recibido);
    EAX_registro = extraer_uint32_al_buffer(buffer_recibido);
    EBX_registro = extraer_uint32_al_buffer(buffer_recibido);
    ECX_registro = extraer_uint32_al_buffer(buffer_recibido);
    EDX_registro = extraer_uint32_al_buffer(buffer_recibido);
    SI_registro = extraer_uint32_al_buffer(buffer_recibido);
    DI_registro = extraer_uint32_al_buffer(buffer_recibido);

    destruir_buffer(buffer_recibido);
}