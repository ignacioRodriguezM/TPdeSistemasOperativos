#include "../include/procesos.h"

void ejecutar_ciclo_fifo_de_cpu (){
    
        /////////////////////////  FETCH   /////////////////////////

        _enviar_pid_y_pc_a_memoria ();

        char* instruccion = _esperar_respuesta_de_memoria();

        log_info(cpu_logger, "PID: %u - FETCH - Program Counter: %u", PID, PC_registro);

        /////////////////////////  DECODE && EXCECUTE   /////////////////////////

        _decode_and_excecute (instruccion);


        /////////////////////////  CHECK INTERRUPT   /////////////////////////

        if(aviso_de_interrupt){
            //protocolo_de_interrupcion
            bloq_flag = false;
        }


}

void _enviar_pid_y_pc_a_memoria (){

    t_buffer* buffer_a_enviar = crear_buffer(); //[pid][pc]
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
            t_buffer* recibido = recibir_buffer_sin_cod_op(fd_memoria);

            char* instruccion = extraer_string_al_buffer(recibido);

            return instruccion;
            break;

        case -1:
            log_error(cpu_logger, "Desconexion de memoria");
            return "Errror";
            break;

        default:
            log_warning(cpu_logger,"Operacion desconocida de memoria");
            return "Errror";
            break;
    }
}
void _aplicar_sum_a_registro (char** comandos, void* regist){
    if(strcmp(comandos[2], "AX") == 0) SUM(regist, &AX_registro);
    else if(strcmp(comandos[2], "BX") == 0) SUM(regist, &BX_registro);
    else if(strcmp(comandos[2], "CX") == 0) SUM(regist, &CX_registro);
    else if(strcmp(comandos[2], "DX") == 0) SUM(regist, &DX_registro);
    else if(strcmp(comandos[2], "EAX") == 0) SUM(regist, &EAX_registro);
    else if(strcmp(comandos[2], "EBX") == 0) SUM(regist, &EBX_registro);
    else if(strcmp(comandos[2], "ECX") == 0) SUM(regist, &ECX_registro);
    else if(strcmp(comandos[2], "EDX") == 0) SUM(regist, &EDX_registro);
    else if(strcmp(comandos[2], "SI") == 0) SUM(regist, &SI_registro);
    else if(strcmp(comandos[2], "DI") == 0) SUM(regist, &DI_registro);
    else if(strcmp(comandos[2], "PC") == 0) SUM(regist, &DI_registro);
}
void _aplicar_sub_a_registro (char** comandos, void* regist){
    if(strcmp(comandos[2], "AX") == 0) SUB(regist, &AX_registro);
    else if(strcmp(comandos[2], "BX") == 0) SUB(regist, &BX_registro);
    else if(strcmp(comandos[2], "CX") == 0) SUB(regist, &CX_registro);
    else if(strcmp(comandos[2], "DX") == 0) SUB(regist, &DX_registro);
    else if(strcmp(comandos[2], "EAX") == 0) SUB(regist, &EAX_registro);
    else if(strcmp(comandos[2], "EBX") == 0) SUB(regist, &EBX_registro);
    else if(strcmp(comandos[2], "ECX") == 0) SUB(regist, &ECX_registro);
    else if(strcmp(comandos[2], "EDX") == 0) SUB(regist, &EDX_registro);
    else if(strcmp(comandos[2], "SI") == 0) SUB(regist, &SI_registro);
    else if(strcmp(comandos[2], "DI") == 0) SUB(regist, &DI_registro);
    else if(strcmp(comandos[2], "PC") == 0) SUB(regist, &DI_registro);
}

void _decode_and_excecute (char* palabra){
    char **comandos = string_split(palabra, " ");
    if(strcmp(comandos[0], "SET") == 0){
        int valor = atoi(comandos[2]);
        if(strcmp(comandos[1], "AX") == 0) SET(&AX_registro, valor);
        else if(strcmp(comandos[1], "BX") == 0) SET(&BX_registro, valor);
        else if(strcmp(comandos[1], "CX") == 0) SET(&CX_registro, valor);
        else if(strcmp(comandos[1], "DX") == 0) SET(&DX_registro, valor);
        else if(strcmp(comandos[1], "EAX") == 0) SET(&EAX_registro, valor);
        else if(strcmp(comandos[1], "EBX") == 0) SET(&EBX_registro, valor);
        else if(strcmp(comandos[1], "ECX") == 0) SET(&ECX_registro, valor);
        else if(strcmp(comandos[1], "EDX") == 0) SET(&EDX_registro, valor);
        else if(strcmp(comandos[1], "SI") == 0) SET(&SI_registro, valor);
        else if(strcmp(comandos[1], "DI") == 0) SET(&DI_registro, valor);
        else if(strcmp(comandos[1], "PC") == 0) SET(&PC_registro, valor);

        log_info(cpu_logger,  "PID: %u - Ejecutando: SET - %s, %d", PID, comandos[1], valor);
    }
    else if(strcmp(comandos[0], "SUM" ) == 0){

        if(strcmp(comandos[1], "AX") == 0){
            _aplicar_sum_a_registro (comandos, &AX_registro);
        }
        else if(strcmp(comandos[1], "BX") == 0){
            _aplicar_sum_a_registro (comandos, &BX_registro);
        }
        else if(strcmp(comandos[1], "CX") == 0){
            _aplicar_sum_a_registro (comandos, &CX_registro);
        }
        else if(strcmp(comandos[1], "DX") == 0){
            _aplicar_sum_a_registro (comandos, &DX_registro);
        }
        else if(strcmp(comandos[1], "EAX") == 0){
            _aplicar_sum_a_registro (comandos, &EAX_registro);
        }
        else if(strcmp(comandos[1], "EBX") == 0){
            _aplicar_sum_a_registro (comandos, &EBX_registro);
        }
        else if(strcmp(comandos[1], "ECX") == 0){
            _aplicar_sum_a_registro (comandos, &ECX_registro);
        }
        else if(strcmp(comandos[1], "EDX") == 0){
            _aplicar_sum_a_registro (comandos, &EDX_registro);
        }
        else if(strcmp(comandos[1], "SI") == 0){
            _aplicar_sum_a_registro (comandos, &SI_registro);
        }
        else if(strcmp(comandos[1], "DI") == 0){
            _aplicar_sum_a_registro (comandos, &DI_registro);
        }
        else if(strcmp(comandos[1], "PC") == 0){
            _aplicar_sum_a_registro (comandos, &PC_registro);
        }
        log_info(cpu_logger,  "PID: %u - Ejecutando: SUM - %s, %s", PID, comandos[1], comandos[2]);

    }
    else if(strcmp(comandos[0], "SUB") == 0){
        if(strcmp(comandos[1], "AX") == 0){
            _aplicar_sub_a_registro (comandos, &AX_registro);
        }
        else if(strcmp(comandos[1], "BX") == 0){
            _aplicar_sub_a_registro (comandos, &BX_registro);
        }
        else if(strcmp(comandos[1], "CX") == 0){
            _aplicar_sub_a_registro (comandos, &CX_registro);
        }
        else if(strcmp(comandos[1], "DX") == 0){
            _aplicar_sub_a_registro (comandos, &DX_registro);
        }
        else if(strcmp(comandos[1], "EAX") == 0){
            _aplicar_sub_a_registro (comandos, &EAX_registro);
        }
        else if(strcmp(comandos[1], "EBX") == 0){
            _aplicar_sub_a_registro (comandos, &EBX_registro);
        }
        else if(strcmp(comandos[1], "ECX") == 0){
            _aplicar_sub_a_registro (comandos, &ECX_registro);
        }
        else if(strcmp(comandos[1], "EDX") == 0){
            _aplicar_sub_a_registro (comandos, &EDX_registro);
        }
        else if(strcmp(comandos[1], "SI") == 0){
            _aplicar_sub_a_registro (comandos, &SI_registro);
        }
        else if(strcmp(comandos[1], "DI") == 0){
            _aplicar_sub_a_registro (comandos, &DI_registro);
        }
        else if(strcmp(comandos[1], "PC") == 0){
            _aplicar_sub_a_registro (comandos, &PC_registro);
        }
        log_info(cpu_logger,  "PID: %u - Ejecutando: SUB - %s, %s", PID, comandos[1], comandos[2]); 
    }
    else if(strcmp(comandos[0], "JNZ") == 0){
        unsigned int valor = atoi(comandos[2]);
        if(strcmp(comandos[1], "AX") == 0){
            JNZ(&AX_registro, valor);        }
        else if(strcmp(comandos[1], "BX") == 0){
            JNZ(&BX_registro, valor);        }
        else if(strcmp(comandos[1], "CX") == 0){
            JNZ(&CX_registro, valor);        }
        else if(strcmp(comandos[1], "DX") == 0){
            JNZ(&DX_registro, valor);        }
        else if(strcmp(comandos[1], "EAX") == 0){
            JNZ(&EAX_registro, valor);        }
        else if(strcmp(comandos[1], "EBX") == 0){
            JNZ(&EBX_registro, valor);        }
        else if(strcmp(comandos[1], "ECX") == 0){
            JNZ(&ECX_registro, valor);        }
        else if(strcmp(comandos[1], "EDX") == 0){
            JNZ(&EDX_registro, valor);        }
        else if(strcmp(comandos[1], "SI") == 0){
            JNZ(&SI_registro, valor);        }
        else if(strcmp(comandos[1], "DI") == 0){
            JNZ(&DI_registro, valor);        }
        else if(strcmp(comandos[1], "PC") == 0){
            JNZ(&PC_registro, valor);
        }
        log_info(cpu_logger,  "PID: %u - Ejecutando: JNZ - %s, %s", PID, comandos[1], comandos[2]);
    }
    else if(strcmp(comandos[0], "IO_GEN_SLEEP") == 0){
        uint8_t valor = atoi(comandos[2]);
        IO_GEN_SLEEP(comandos[1], valor);
        log_info(cpu_logger,  "PID: %u - Ejecutando: IO_GEN_SLEEP - ", PID);
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    else if(strcmp(comandos[0], "MOV_IN") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "MOV_OUT") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "RESIZE") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "COPY_STRING") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "IO_STDIN_READ") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    
    else if(strcmp(comandos[0], "IO_STDOUT_WRITE") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "WAIT") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "SIGNAL") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "IO_FS_CREATE") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "IO_FS_DELETE") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "IO_FS_TRUNCATE") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "IO_FS_WRITE") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "IO_FS_READ") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    else if(strcmp(comandos[0], "EXIT") == 0){
        printf("CASO NO DESARROLLADO \n");
    }
    
    
    string_array_destroy(comandos);
}