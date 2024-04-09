#include "../include/cpu_kernel_interrupt.h"

void atender_cpu_kernel_interrupt (){
    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_interrupt);
		switch (cod_op) {
            case MENSAJE:
                //
                break;
            case PAQUETE:
                //
                break;
            case -1:
                log_error(cpu_logger, "Desconexion de KERNEL - interrupt");
                control_key = 0;
                break;
            default:
                log_warning(cpu_logger,"Operacion desconocida de KERNEL - interrupt");
                break;
            }
	}
}