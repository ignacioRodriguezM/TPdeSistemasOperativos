#include "../include/memoria_cpu.h"

void atender_memoria_cpu (){
    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_cpu);
		switch (cod_op) {
            case MENSAJE:
                //
                break;
            case PAQUETE:
                //
                break;
            case -1:
                log_error(memoria_logger, "Desconexion de CPU");
                control_key = 0;
                break;
            default:
                log_warning(memoria_logger,"Operacion desconocida de CPU");
                break;
            }
	}
}