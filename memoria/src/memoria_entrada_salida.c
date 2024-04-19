#include "../include/memoria_entrada_salida.h"

void atender_memoria_entrada_salida (){
    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_entrada_salida);
		switch (cod_op) {
            case MENSAJE:
                //
                break;
            case PAQUETE:
                //
                break;

            case ACCESO_A_ESPACIO_USUARIO:
                //t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_entrada_salida);
            break;

            case -1:
                log_error(memoria_logger, "Desconexion de ENTRADA SALIDA");
                control_key = 0;
                break;
            default:
                log_warning(memoria_logger,"Operacion desconocida de ENTRADA SALIDA");
                break;
            }
	}
}