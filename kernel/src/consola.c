#include "../include/consola.h"


void iniciar_consola_interactiva (){
    char* leido;
    leido = readline("> ");
    bool validacion_leido;

    while (strcmp(leido,"\0") != 0){
        validacion_leido = _validacion_de_instruccion_de_consola (leido);
        if (!validacion_leido){
            log_error(kernel_logger, "Comando de CONSOLA no reconocido");
            free(leido);
            leido = readline("> ");
            continue; //saltar y continuara con el resto de la iteracion
        }

        _atender_instruccion_validada(leido);
        free(leido);
        leido = readline("> ");
    }

    free(leido);
}    

bool _validacion_de_instruccion_de_consola (char* leido){
    bool resultado_validacion= false;

    //[FALTA] Hacer mas controles de vaidacion
    char** comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0){
        resultado_validacion = true;
        }
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0){
        resultado_validacion = true;
        }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0){
        resultado_validacion = true;
        }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0){
        resultado_validacion = true;
        }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0){
        resultado_validacion = true;
        }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0){
        resultado_validacion = true;
        }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0){
        resultado_validacion = true;
        }
    else{
        log_error(kernel_logger, "Comando no reconocido. ");
        resultado_validacion = false;
        }

    string_array_destroy(comando_consola);

    return resultado_validacion;
}

void _atender_instruccion_validada (char* leido){
    char** comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0){
        
        }
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0){
        
        }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0){
        
        }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0){
        
        }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0){
        
        }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0){
        
        }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0){
        
        }
    else{
        log_error(kernel_logger, "Comando no reconocido, por que paso la validacion?! ");
        
        }

    string_array_destroy(comando_consola);

}