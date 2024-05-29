#include "../include/diccionario.h"

t_dictionary *instrucciones;
t_dictionary *parametros;

void incializar_diccionarios(){


    // DICCIONARIO DE INSTRUCCIONES////
    instrucciones = dictionary_create();


    dictionary_put(instrucciones, "SUM", (void*)SUM);
    dictionary_put(instrucciones, "SUB", (void*)SUB);
    dictionary_put(instrucciones, "SET", (void*)SET);
    dictionary_put(instrucciones, "JNZ", (void*)JNZ);
    dictionary_put(instrucciones, "IO_GEN_SLEEP", (void*)IO_GEN_SLEEP);
    dictionary_put(instrucciones, "EXIT", (void*)EXIT);
    //dictionary_put(instrucciones, "MOV_IN", (void*)MOV_IN);
    //dictionary_put(instrucciones, "MOV_OUT", (void*)MOV_OUT);
    dictionary_put(instrucciones, "RESIZE", (void*)RESIZE);
    //dictionary_put(instrucciones, "IO_STDIN_READ", (void*)IO_STDIN_READ);
    //dictionary_put(instrucciones, "IO_STDOUT_WRITE", (void*)IO_STDOUT_WRITE);
    dictionary_put(instrucciones, "WAIT", (void*)WAIT);
    dictionary_put(instrucciones, "SIGNAL", (void*)SIGNAL);
    //dictionary_put(instrucciones, "IO_FS_CREATE", (void*)IO_FS_CREATE);
    //dictionary_put(instrucciones, "IO_FS_DELETE", (void*)IO_FS_CREATE);
    //dictionary_put(instrucciones, "IO_FS_TRUNCATE", (void*)IO_FS_TRUNCATE);
    //dictionary_put(instrucciones, "IO_FS_WRITE", (void*)IO_FS_WRITE);
    //dictionary_put(instrucciones, "IO_FS_READ", (void*)IO_FS_READE);
    

    //// DICCIONARIO DE REGISTROS////
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
    typedef void (*OperationFunc)(void*, ...);
    OperationFunc operacion = (OperationFunc)dictionary_get(instrucciones, funcion);
    if (operacion == NULL) {
        printf("Operación no encontrada: %s\n", funcion);
        return;
    }

    // Convertir cadenas a punteros a void y llamar a la operación con los parámetros
    void* void_params[num_params];
    for (int i = 0; i < num_params; ++i) {
        void_params[i] = dictionary_get(parametros, funcion);
    }

    // Llamar a la operación con los parámetros como punteros a void
    switch (num_params) {
        case 0: operacion(NULL); break;
        case 1: operacion(void_params[0]); break;
        case 2: operacion(void_params[0], void_params[1]); break;
        case 3: operacion(void_params[0], void_params[1], void_params[2]); break;
        case 4: operacion(void_params[0], void_params[1], void_params[2], void_params[3]); break;
        case 5: operacion(void_params[0], void_params[1], void_params[2], void_params[3], void_params[4]); break;
        default: printf("Número inválido de parámetros: %d\n", num_params); break;
    }
}

