#include "../include/consola.h"

int esNumero(const char *cadena) {
    // Iterar sobre cada carácter del string
    while (*cadena) {
        // Verificar si el carácter actual no es un dígito numérico
        if (!isdigit(*cadena)) {
            return 0; // No es un número
        }
        cadena++; // Mover al siguiente carácter
    }
    return 1; // Todos los caracteres son dígitos numéricos
}

void iniciar_consola_interactiva (){
    char* leido;
    leido = readline("> ");
    bool validacion_leido;
    

    while (strcmp(leido,"\0") != 0){
        validacion_leido = _validacion_de_instruccion_de_consola (leido);
        if (!validacion_leido){
            free(leido);
            leido = readline("> ");
            continue; //saltar al inicio
        }

        _atender_instruccion_validada(leido);
        free(leido);
        leido = readline("> ");
    }

    free(leido);
}    

bool _validacion_de_instruccion_de_consola (char* leido){
    bool resultado_validacion= false;

    
    char** comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0){        //valida que haya escrito ejecutar script
        if (access(comando_consola[1], F_OK) != -1) { //valida que exista el path
        resultado_validacion= true;}
        else {
        log_error(kernel_logger, "El path no es válido o no se tiene permiso para acceder.");
        }
    }
    
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0){
        if (access(comando_consola[1], F_OK) != -1) {
        resultado_validacion= true;}
        else {
        log_error(kernel_logger, "El path no es válido o no se tiene permiso para acceder.");
        }
    }
    
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0){
        if(esNumero(comando_consola[1]) == 1){        //validamos que haya ingresado un numero como process id
            resultado_validacion= true;
        }
        else{
            log_error(kernel_logger, "Asegurate de incluir un PID numerico. ");
        }
        
    }
    
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0){
        if (comando_consola[1]==NULL){
            resultado_validacion = true;
        }
        else {
        log_error(kernel_logger, "Comando incorrecto, asegurese de escribir solo DETENER_PLANIFICACION");}
    }

    
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0){
        if (comando_consola[1]==NULL){
            resultado_validacion = true;
        }
        else {
        log_error(kernel_logger, "Comando incorrecto, asegurese de escribir solo INICIAR_PLANIFICACION");}
    }
    
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0){
        if(esNumero(comando_consola[1]) == 1){
            resultado_validacion= true;
        }
        else{
            log_error(kernel_logger, "Asegurate de incluir un valor de multiprogramacion correcto. ");
        }
    }
    
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0){
        if (comando_consola[1]==NULL){
            resultado_validacion = true;
        }
        else {
        log_error(kernel_logger, "Comando incorrecto, asegurese de escribir solo PROCESO_ESTADO");}
    }
    
    else{
        log_error(kernel_logger, "Comando de CONSOLA no reconocido ");
        resultado_validacion = false;
    }

    string_array_destroy(comando_consola);

    return resultado_validacion;
}


void _atender_instruccion_validada (char* leido){
    char** comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0){
        FILE *archivo;
        char* comando = malloc(100 * sizeof(char));
        
        // Abrir el archivo de comandos
        archivo = fopen(comando_consola[1], "r");
        // Verificar si se pudo abrir el archivo
        if (archivo == NULL) {
            printf("No se pudo abrir el archivo.\n");
            exit(EXIT_FAILURE);
        }
        // Leer y ejecutar los comandos uno por uno
        while (fgets(comando, 100 * sizeof(char), archivo) != NULL) {
            printf("%s", comando); //borrar
            _atender_instruccion_validada(comando);
        }
        // Cerrar el archivo
        fclose(archivo);
        // Liberar memoria
        free(comando);
    }
    
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0){
        t_buffer* buffer = crear_buffer();
        printf("Ingresaste: %s\n", comando_consola[1]);
        uint16_t pid = asignar_pid();
        cargar_uint16_al_buffer(buffer, pid);
        cargar_string_al_buffer(buffer, comando_consola[1]);

        t_paquete* a_enviar = crear_paquete (INICIAR_PROCESO, buffer); // [PID] [PATH]
        
        enviar_paquete(a_enviar, fd_memoria);
        
        destruir_paquete(a_enviar);

        }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0){
        
        }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0){
        
        }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0){
        if (planificacion_activa == true){
            log_info(kernel_logger, "La planificacion ya está"); 
            }
        else{
            l
            }
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
