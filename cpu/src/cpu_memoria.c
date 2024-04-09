#include "../include/cpu_memoria.h"

void atender_cpu_memoria (){
    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_memoria);
		switch (cod_op) {
            case MENSAJE:
                //
                break;
            case PAQUETE:
                //
                break;
            case -1:
                log_error(cpu_logger, "Desconexion de memoria");
                control_key = 0;
                break;
            default:
                log_warning(cpu_logger,"Operacion desconocida de memoria");
                break;
            }
	}
}