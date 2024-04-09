#include "../include/kernel-cpu_dispatch.h"

void atender_kernel_cpu_dispatch(){
    bool control_key = 1; 
    while(control_key){
        int cod_op = recibir_operacion(fd_cpu_dispatch);
        switch (cod_op)
        {
        case MENSAJE:
            /* code */
            break;
        case PAQUETE:
        //codigo
            break;
        case -1:
        log_error(kernel_logger, "Desconexion de KERNEL-CPU_DISPATCH");
        control_key = 0;
            break;
        default:
        log_warning(kernel_logger,"Operacion desconocida de KERNEL-CPU_DISPATCH");
            break;
        }
    }

}