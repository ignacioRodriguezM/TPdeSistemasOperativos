#include "../include/cpu_kernel_dispatch.h"

void atender_cpu_kernel_dispatch()
{


    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_dispatch);
        aviso_de_interrupt = false;
        bloq_flag = true;
        interrupt_flag = true;
        switch (cod_op)
        {
        
        case PROCESO_A_EJECUTAR:

            _extraer_contexto_de_ejecucion();

            while (bloq_flag)
            {

                
                ejecutar_ciclo_de_cpu();
                
            }

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
    log_trace(cpu_log_debug, "LLEGO EL PROCESO : %u", PID);
    PC_registro = extraer_uint32_al_buffer(buffer_recibido);
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


