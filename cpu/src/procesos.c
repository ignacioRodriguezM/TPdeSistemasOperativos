#include "../include/procesos.h"

void ejecutar_ciclo_fifo_de_cpu (){

    /////////////////////////  FETCH   /////////////////////////

    _enviar_pid_y_pc_a_memoria ();

    char* instruccion = _esperar_respuesta_de_memoria();

    /////////////////////////  DECODE   /////////////////////////

    char **comandos = string_split(instruccion, " ");
    if(strcmp(comandos[0], "SET") == 0){

    }
    else if(strcmp(comandos[0], "SUM" ) == 0){
        
    }
    else if(strcmp(comandos[0], "SUB") == 0){
        
    }
    else if(strcmp(comandos[0], "JNZ") == 0){
        
    }
    else if(strcmp(comandos[0], "IO_GEN_SLEEP") == 0){
        
    }
    


    /////////////////////////  EXCECUTE   /////////////////////////





    /////////////////////////  CHECK INTERRUPT   /////////////////////////

}

void _enviar_pid_y_pc_a_memoria (){

    t_buffer* buffer_a_enviar = crear_buffer();
    cargar_uint16_al_buffer(buffer_a_enviar, PID);
    cargar_uint32_al_buffer(buffer_a_enviar, PC_registro);
    t_paquete* a_enviar = crear_paquete(SOLICITUD_DE_PROXIMA_INSTRUCCION, buffer_a_enviar);
    enviar_paquete(a_enviar, fd_memoria);
    destruir_paquete(a_enviar);

}

char* _esperar_respuesta_de_memoria (){

    int cod_op = recibir_operacion(fd_memoria);
	switch (cod_op) {
        case PROXIMA_INSTRUCCION:

            break;

        case -1:
            log_error(cpu_logger, "Desconexion de memoria");
            break;

        default:
            log_warning(cpu_logger,"Operacion desconocida de memoria");
            break;
    }
}