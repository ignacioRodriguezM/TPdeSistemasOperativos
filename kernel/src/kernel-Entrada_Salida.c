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

        case PRESENTACION:
            t_buffer* buffer_recibido = recibir_buffer_sin_cod_op (fd_entrada_salida);

            char* nombre_del_io_presentado = extraer_string_al_buffer (buffer_recibido);
            interfaces_io* puntero_recibido = extraer_choclo_al_buffer (buffer_recibido);
            
            interfaces_io tipo_interfaz = *puntero_recibido;
            free(puntero_recibido);

            if (contador_de_colas_bloqueados == 0){
                contador_de_colas_bloqueados ++;
                colas_bloqueados = malloc(sizeof(Colas_bloqueados));
                colas_bloqueados[0]->nombre = nombre_del_io_presentado;
                colas_bloqueados[0]->tipo_interfaz = tipo_interfaz;
                colas_bloqueados[0]->cola = queue_create();
            }
            else {
                contador_de_colas_bloqueados++;
                colas_bloqueados = realloc (contador_de_colas_bloqueados * sizeof(Colas_bloqueados));
                colas_bloqueados[contador_de_colas_bloqueados]->nombre = nombre_del_io_presentado;
                colas_bloqueados[contador_de_colas_bloqueados]->tipo_interfaz = tipo_interfaz;
                colas_bloqueados[contador_de_colas_bloqueados]->cola = queue_create();
            }

            
            


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