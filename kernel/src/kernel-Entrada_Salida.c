#include "../include/kernel-Entrada_Salida.h"

void atender_kernel_entrada_salida(){
    bool control_key = 1; 
    while(control_key){
        int cod_op = recibir_operacion(fd_entrada_salida);
        switch (cod_op)
        {
        case MENSAJE:
            /* code */
            break;
        case PAQUETE:
        //codigo
            break;
        case -1:
        log_error(kernel_logger, "Desconexion de KERNEL-i/o");
        control_key = 0;
            break;
        default:
        log_warning(kernel_logger,"Operacion desconocida de KERNEL-i/o");
            break;
        }
    }

}