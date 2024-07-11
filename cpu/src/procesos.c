#include "../include/procesos.h"
#include "../include/diccionario.h"

void ejecutar_ciclo_de_cpu()
{

    /////////////////////////  FETCH   /////////////////////////

    _enviar_pid_y_pc_a_memoria();

    char *instruccion = _esperar_respuesta_de_memoria();

    log_info(cpu_logger, "PID: %u - FETCH - Program Counter: %u", PID, PC_registro);

    /////////////////////////  DECODE && EXCECUTE   /////////////////////////

    decodear(instruccion);

    free(instruccion);

    /////////////////////////  CHECK INTERRUPT   /////////////////////////
    if (interrupt_flag)
    {
        if (aviso_de_interrupt)
        {
            if(desalojo){
                _desalojar_proceso();
                desalojo = false;
            }
            if(interrupcion_de_usuarios){
                _desalojar_proceso_interrumpido_por_usuario();
                interrupcion_de_usuarios = false;
            }
            bloq_flag = false;
            aviso_de_interrupt = false;
        }
    }
}

void decodear(char *palabra)
{
    char **comandos = string_split(palabra, " ");

    int num_params = 0;
    while (comandos[num_params] != NULL)
    {
        num_params++;
    }

    decode(comandos[0], num_params - 1, &comandos[1]);
    
    string_array_destroy(comandos);
}

void _desalojar_proceso()
{
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

    t_paquete *a_enviar = crear_paquete(DESALOJO, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);
}

void _desalojar_proceso_interrumpido_por_usuario()
{
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

    t_paquete *a_enviar = crear_paquete(INTERRUPTED_BY_USER, buffer);

    enviar_paquete(a_enviar, fd_kernel_dispatch);

    destruir_paquete(a_enviar);
}

void _enviar_pid_y_pc_a_memoria()
{

    t_buffer *buffer_a_enviar = crear_buffer(); //[pid][pc]
    cargar_uint16_al_buffer(buffer_a_enviar, PID);
    cargar_uint32_al_buffer(buffer_a_enviar, PC_registro);
    t_paquete *a_enviar = crear_paquete(SOLICITUD_DE_PROXIMA_INSTRUCCION, buffer_a_enviar);
    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);
}

char *_esperar_respuesta_de_memoria()
{

    int cod_op = recibir_operacion(fd_memoria);
    switch (cod_op)
    {
    case PROXIMA_INSTRUCCION:
        t_buffer *recibido = recibir_buffer_sin_cod_op(fd_memoria);

        char *instruccion = extraer_string_al_buffer(recibido);

        destruir_buffer(recibido);

        return instruccion;
        break;

    case -1:
        log_error(cpu_logger, "Desconexion de memoria");
        return "Errror";
        break;

    default:
        log_warning(cpu_logger, "Operacion desconocida de memoria");
        return "Errror";
        break;
    }
}
