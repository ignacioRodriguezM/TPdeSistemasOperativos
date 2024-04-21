#include "../include/entrada_salida_kernel.h"

void atender_entrada_salida_kernel(){
    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel);
		switch (cod_op) {
            case IO_GEN_SLEEP:
                t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_kernel); // [PID] [CANTIDAD DE UNIDADES DE TRABAJO]
                uint16_t pid = extraer_uint16_al_buffer(buffer_recibido);
                uint8_t cantidad_unidades_de_trabajo = extraer_uint8_al_buffer(buffer_recibido);
                destruir_buffer (buffer_recibido);

                log_info (entrada_salida_logger, "PID: %hu - Operacion: IO_GEN_SLEEP", pid);

                unsigned int tiempo_a_esperar = cantidad_unidades_de_trabajo * tiempo_unidad_trabajo;
                esperarMilisegundos(tiempo_a_esperar);

                //AVISAR A KERNEL QUE SE REALIZO LA OPERACION

                

                break;
            case -1:
                log_error(entrada_salida_logger, "Desconexion de KERNEL");
                control_key = 0;
                break;
            default:
                log_warning(entrada_salida_logger,"Operacion desconocida de KERNEL");
                break;
            }
	}
}
void presentarse_con_kernel(){

    t_buffer* buffer = crear_buffer();
    cargar_string_al_buffer (buffer, nombre);
    cargar_choclo_al_buffer (buffer, &tipo_interfaz, sizeof(interfaces_io));

    t_paquete *a_enviar = crear_paquete(PRESENTACION, buffer);

    enviar_paquete(a_enviar, fd_kernel);

    destruir_paquete(a_enviar);
    
}

void esperarMilisegundos(unsigned int milisegundos) {
    usleep(milisegundos * 1000); // Convertimos milisegundos a microsegundos
}