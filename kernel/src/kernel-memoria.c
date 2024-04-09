#include "../include/kernel-memoria.h"

void atender_kernel_memoria(){
    bool control_key = 1; 
    while(control_key){
        int cod_op = recibir_operacion(fd_memoria);
        switch (cod_op)
        {
        case MENSAJE:
            /* code */
            break;
        case PAQUETE:
        //codigo
            break;
        case -1:
        log_error(kernel_logger, "Desconexion de KERNEL-MEMORIA");
        control_key = 0;
            break;
        default:
        log_warning(kernel_logger,"Operacion desconocida de KERNEL-MEMORIA");
            break;
        }
    }

}