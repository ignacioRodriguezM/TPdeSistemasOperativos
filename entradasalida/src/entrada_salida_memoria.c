#include "../include/entrada_salida_memoria.h"

void atender_entrada_salida_memoria(){
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
                log_error(entrada_salida_logger, "Desconexion de MEMORIA");
                control_key = 0;
                break;
            default:
                log_warning(entrada_salida_logger,"Operacion desconocida de MEMORIA");
                break;
            }
	}
}