#include "../include/memoria_cpu.h"

void atender_memoria_cpu (){
    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_cpu);
		switch (cod_op) {
            case SOLICITUD_DE_PROXIMA_INSTRUCCION:
                t_buffer* buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
                uint16_t pid_recibido = extraer_uint16_al_buffer(buffer_recibido);
                uint32_t pc_recibido = extraer_uint32_al_buffer(buffer_recibido);

                t_link_element* current = lista_procesos->head;
                for(int i=0;i<lista_procesos->elements_count;i++){
                    Proceso* proceso = (Proceso*)current->data;
                    if(proceso->PID == pid_recibido){
                        if (proceso->cantidad_instrucciones <= pc_recibido){
                            perror("CPU SOLICITA UN PC POSTERIOR AL ULTIMO PC DEL PROCESO");
                        }
                        esperarMilisegundos(retardo_respuesta);
                        t_buffer* buffer_a_enviar = crear_buffer(); //[instruccion]
                        cargar_string_al_buffer(buffer_a_enviar, proceso->instrucciones[pc_recibido]);
                        t_paquete* a_enviar = crear_paquete(PROXIMA_INSTRUCCION, buffer_a_enviar);
                        enviar_paquete(a_enviar, fd_cpu);
                        destruir_paquete(a_enviar);
                    }
                    else{
                        current = current->next;
                    }
                }
                destruir_buffer(buffer_recibido);


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
void chequear_pid (){

}