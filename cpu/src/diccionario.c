#include "../include/diccionario.h"

// Declaración de diccionarios
t_dictionary* operation_switcher;
t_dictionary* parametros;

void inicializar_diccionarios() {
    // DICCIONARIO DE INSTRUCCIONES
    operation_switcher = dictionary_create();

    // Inserción de instrucciones en el diccionario utilizando uintptr_t para almacenar enteros como punteros
    dictionary_put(operation_switcher, "SET", (void*)(uintptr_t)0);
    dictionary_put(operation_switcher, "MOV_IN", (void*)(uintptr_t)1);
    dictionary_put(operation_switcher, "MOV_OUT", (void*)(uintptr_t)2);
    dictionary_put(operation_switcher, "SUM", (void*)(uintptr_t)3);
    dictionary_put(operation_switcher, "SUB", (void*)(uintptr_t)4);
    dictionary_put(operation_switcher, "JNZ", (void*)(uintptr_t)5);
    dictionary_put(operation_switcher, "RESIZE", (void*)(uintptr_t)6);
    //dictionary_put(operation_switcher, "COPY_STRING", (void*)(uintptr_t)7);
    dictionary_put(operation_switcher, "WAIT", (void*)(uintptr_t)8);
    dictionary_put(operation_switcher, "SIGNAL", (void*)(uintptr_t)9);
    dictionary_put(operation_switcher, "IO_GEN_SLEEP", (void*)(uintptr_t)10);
    //dictionary_put(operation_switcher, "IO_STDIN_READ", (void*)(uintptr_t)11);
    //dictionary_put(operation_switcher, "IO_STDOUT_WRITE", (void*)(uintptr_t)12);
    //dictionary_put(operation_switcher, "IO_FS_CREATE", (void*)(uintptr_t)13);
    //dictionary_put(operation_switcher, "IO_FS_DELETE", (void*)(uintptr_t)14);
    //dictionary_put(operation_switcher, "IO_FS_TRUNCATE", (void*)(uintptr_t)15);
    //dictionary_put(operation_switcher, "IO_FS_WRITE", (void*)(uintptr_t)16);
    //dictionary_put(operation_switcher, "IO_FS_READ", (void*)(uintptr_t)17);
    dictionary_put(operation_switcher, "EXIT", (void*)(uintptr_t)18);

    // DICCIONARIO DE REGISTROS
    parametros = dictionary_create();
    dictionary_put(parametros, "AX", &AX_registro);
    dictionary_put(parametros, "BX", &BX_registro);
    dictionary_put(parametros, "CX", &CX_registro);
    dictionary_put(parametros, "DX", &DX_registro);
    dictionary_put(parametros, "EAX", &EAX_registro);
    dictionary_put(parametros, "EBX", &EBX_registro);
    dictionary_put(parametros, "ECX", &ECX_registro);
    dictionary_put(parametros, "EDX", &EDX_registro);
    dictionary_put(parametros, "SI", &SI_registro);
    dictionary_put(parametros, "DI", &DI_registro);
    dictionary_put(parametros, "PC", &PC_registro);

}

void decode(char* funcion, int num_params, char** params) {
    
    // Obtener la operación
    void* result = dictionary_get(operation_switcher, funcion);
    /*
    if (result == NULL) {
        fprintf(stderr, "Error: Operación '%s' no encontrada.\n", funcion);
        return;
    }*/
    int op_switcher = (int)(uintptr_t)result;

    // Obtener los parámetros
    void* void_params[num_params];
    for (int i = 0; i < num_params; ++i) {
        void_params[i] = dictionary_get(parametros, params[i]);
    }
    
    // Ejecutar la operación correspondiente
    switch (op_switcher) {
        case 0: {
            // SET
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s, %s>", PID, funcion,  params[0], params[1]);
            int valor0 = atoi(params[1]);
            SET(void_params[0], &valor0);
            break;
        }
        case 1: {
            // MOV_IN
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s, %s>", PID, funcion, params[0], params[1]);
            break;
        }
        case 2:
            // MOV_OUT
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s, %s>", PID, funcion, params[0], params[1]);
            break;
        case 3:
            // SUM
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s, %s>", PID, funcion, params[0], params[1]);
            SUM(void_params[0], void_params[1]);
            break;
        case 4:
            // SUB
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s, %s>", PID, funcion, params[0], params[1]);
            SUB(void_params[0], void_params[1]);
            break;
        case 5: {
            // JNZ
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s, %s>", PID, funcion, params[0], params[1]);
            int valor5 = atoi(params[1]);
            JNZ(void_params[0], &valor5);
            break;
        }
        case 6: {
            // RESIZE
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s>", PID, funcion, params[1]);
            int valor6 = atoi(params[0]);
            RESIZE(&valor6);
            break;
        }
        case 7:
            // COPY_STRING
            break;
        case 8: {
            // WAIT
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s>", PID, funcion, params[0]);
            char* nombre_recurso8 = params[0];
            WAIT(nombre_recurso8);
            break;
        }
        case 9: {
            // SIGNAL
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s>", PID, funcion, params[0]);
            char* nombre_recurso9 = params[0];
            SIGNAL(nombre_recurso9);
            break;
        }
        case 10: {
            // IO_GEN_SLEEP
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s> - <%s, %s>", PID, funcion, params[0], params[1]);
            char* nombre_interfaz = params[0];
            int valor10 = atoi(params[1]);
            IO_GEN_SLEEP(nombre_interfaz, &valor10);
            break;
        }
        case 11:
            // IO_STDIN_READ
            break;
        case 12:
            // IO_STDOUT_WRITE
            break;
        case 13:
            // IO_FS_CREATE
            break;
        case 14:
            // IO_FS_DELETE
            break;
        case 15:
            // IO_FS_TRUNCATE
            break;
        case 16:
            // IO_FS_WRITE
            break;
        case 17:
            // IO_FS_READ
            break;
        case 18:
            // EXIT
            log_info(cpu_logger, "PID: <%u> - Ejecutando: <%s>", PID, funcion);
            EXIT();
            break;
        default:
            fprintf(stderr, "Error: Operación '%d' desconocida.\n", op_switcher);
            break;
    }
}