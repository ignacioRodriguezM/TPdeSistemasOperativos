#include "../include/diccionario.h"
#include <commons/collections/dictionary.h>

//// DICCIONARIO DE INSTRUCCIONES////
t_dictionary *instrucciones = dictionary_create();
dictionary_put(instrucciones, "SUM", _aplicar_sum_a_registro());
dictionary_put(instrucciones, "SUB", _aplicar_sub_a_registro());
dictionary_put(instrucciones, "SET", SET());
dictionary_put(instrucciones, "JNZ", JNZ());
dictionary_put(instrucciones, "IO_GEN_SLEEP", IO_GEN_SLEEP());
dictionary_put(instrucciones, "EXIT", EXIT());
dictionary_put(instrucciones, "MOV_IN", MOV_IN());
dictionary_put(instrucciones, "MOV_OUT", MOV_OUT());
dictionary_put(instrucciones, "RESIZE", RESIZE());
dictionary_put(instrucciones, "IO_STDIN_READ", IO_STDIN_READ());
dictionary_put(instrucciones, "IO_STDOUT_WRITE", IO_STDOUT_WRITE());
dictionary_put(instrucciones, "WAIT", WAIT());
dictionary_put(instrucciones, "SIGNAL", SIGNAL());
dictionary_put(instrucciones, "IO_FS_CREATE", IO_FS_CREATE());
dictionary_put(instrucciones, "IO_FS_DELETE", IO_FS_CREATE());
dictionary_put(instrucciones, "IO_FS_TRUNCATE", IO_FS_TRUNCATE());
dictionary_put(instrucciones, "IO_FS_WRITE", IO_FS_WRITE());
dictionary_put(instrucciones, "IO_FS_READ", IO_FS_READE());


//// DICCIONARIO DE REGISTROS////
t_dictionary *registros = dictionary_create();
dictionary_put(registros, "AX", &AX_registro);
dictionary_put(registros, "BX", &BX_registro);
dictionary_put(registros, "CX", &CX_registro);
dictionary_put(registros, "DX", &DX_registro);
dictionary_put(registros, "EAX", &EAX_registro);
dictionary_put(registros, "EBX", &EBX_registro);
dictionary_put(registros, "ECX", &ECX_registro);
dictionary_put(registros, "EDX", &EDX_registro);
dictionary_put(registros, "SI", &SI_registro);
dictionary_put(registros, "DI", &DI_registro);
dictionary_put(registros, "PC", &PC_registro);
