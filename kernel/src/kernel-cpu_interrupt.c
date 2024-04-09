#include "../include/kernel-cpu_interrupt.h"

void atender_kernel_cpu_inerrupt(){
    bool control_key = 1; 
    while(control_key){
        int cod_op = recibir_operacion(fd_cpu_interrupt);
        switch (cod_op)
        {
        case MENSAJE:
            /* code */
            break;
        case PAQUETE:
        //codigo
            break;
        case -1:
        log_error(kernel_logger, "Desconexion de KERNEL-CPU_INTERRUPT");
        control_key = 0;
            break;
        default:
        log_warning(kernel_logger,"Operacion desconocida de KERNEL-CPU_INTERRUPT");
            break;
        }
    }

}