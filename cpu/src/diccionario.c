#include "../include/diccionario.h"

t_dictionary * operation_switcher;
t_dictionary *parametros;

void incializar_diccionarios(){


    // DICCIONARIO DE INSTRUCCIONES////
    operation_switcher = dictionary_create();


    dictionary_put(operation_switcher, "SET", (int*)0);
    dictionary_put(operation_switcher, "MOV_IN", (int*)1);
    dictionary_put(operation_switcher, "MOV_OUT", (int*)2);
    dictionary_put(operation_switcher, "SUM", (int*)3);
    dictionary_put(operation_switcher, "SUB", (int*)4);
    dictionary_put(operation_switcher, "JNZ", (int*)5);
    dictionary_put(operation_switcher, "RESIZE", (int*)6);
    dictionary_put(operation_switcher, "COPY_STRING", (int*)7);
    dictionary_put(operation_switcher, "WAIT", (int*)8);
    dictionary_put(operation_switcher, "SIGNAL", (int*)9);
    dictionary_put(operation_switcher, "IO_GEN_SLEEP", (int*)10);
    //dictionary_put(operation_switcher, "IO_STDIN_READ", (int*)11);
    //dictionary_put(operation_switcher, "IO_STDOUT_WRITE", (int*)12);
    //dictionary_put(operation_switcher, "IO_FS_CREATE", (int*)13);
    //dictionary_put(operation_switcher, "IO_FS_DELETE", (int*)14);
    //dictionary_put(operation_switcher, "IO_FS_TRUNCATE", (int*)15);
    //dictionary_put(operation_switcher, "IO_FS_WRITE", (int*)16);
    //dictionary_put(operation_switcher, "IO_FS_READ", (int*)17);
    dictionary_put(operation_switcher, "EXIT", (int*)18);

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
    int op_switcher = *(int*)dictionary_get(operation_switcher, funcion);

    void* void_params[num_params];
    for (int i = 0; i < num_params; ++i) {
        void_params[i] = dictionary_get(parametros, params[i]);
    }

    switch (op_switcher){
        case 0:
        //SET
            int valor0 = atoi(params[1]);
            SET(void_params[0], &valor0);
            break;
        case 1:
        //MOV_IN
            int direccion_fisica = atoi(params[1]);
            MOV_IN(params[0], &direccion_fisica);
            break;
        case 2:
        //MOV_OUT
 
            break;
        case 3:
        //SUM
            SUM(void_params[0], void_params[1]);
            break;
        case 4:
        //SUB
            SUB(void_params[0], void_params[1]);
            break;
        case 5:
        //JNZ
            int valor5 = atoi(params[1]);
            JNZ(void_params[0], &valor5);
            break;
        case 6:
        //RESIZE
            int valor6 = atoi(params[0]);
            RESIZE(&valor6);
            break;
        case 7:
        //COPY_STRING

            break;
        case 8:
        //WAIT
            char *nombre_recurso8 = params[0];
            WAIT(nombre_recurso8);
            break;
        case 9:
        //SIGNAL
            char *nombre_recurso9 = params[0];
            SIGNAL(nombre_recurso9);
            break;
        case 10:
        //IO_GEN_SLEEP
            char *nombre_interfaz = params[0];
            int valor10 = atoi(params[1]);
            IO_GEN_SLEEP(nombre_interfaz, &valor10);
            break;
        case 11:
        //IO_STDIN_READ

            break;
        case 12:
        //IO_STDOUT_WRITE

            break;
        case 13:
        //IO_FS_DELETE

            break;
        case 14:
        //IO_FS_DELETE

            break;
        case 15:
        //IO_FS_TRUNCATE

            break;
        case 16:
        //IO_FS_WRITE

            break;
        case 17:
        //IO_FS_READ

            break;
        case 18:
        //EXIT
            EXIT();
            break;
    }
    
}

