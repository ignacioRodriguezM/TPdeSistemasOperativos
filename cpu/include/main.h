#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include "./c_gestor.h"



#include "./cpu_kernel_dispatch.h"
#include "./cpu_kernel_interrupt.h"
#include "./cpu_memoria.h"

//Variables GLOBALES
t_log* cpu_logger;
t_log* cpu_log_debug;

int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_dispatch;
int fd_kernel_interrupt;
int fd_memoria;   

bool aviso_de_interrupt;
bool bloq_flag;
bool interrupt_flag;

//COSAS RECIVIDAS DEL KERNEL QUE NO SON REGISTROS
uint16_t PID;


//                      ..-------------------.__
//                    .'_______                  `-.
//                   // .-----.\.--------..--------.\
//                   ||'    __'||        ||   ||   || __
//                   ||' .=(_ )|| ====== || ==.|   ||( _)
//                   ||'|    \\||________||________||//
//                   ||'------\) ,--======\\======-._/
//               ____||        |/           = =      `-.
//     _____________ ||'==.    ||   ......      = =     `-.
//    `=============`||        ||_ /////.--.        = =    `. .--.
//    |    .---.     ||        |   .----|==|     \    \ \    \|==|
//    | .'       `.  ||        | .'     '--'.    |-.  | |  .-|'--'`. 
//    .'           \ ||        .'            `.  |-.'-|=|-'.-|      \
//   /    .-==-.    \||       /  _.----.       \ |-.'-|=|-'.-|      |
//  |  .' .---. `.  |||       .-' .---. `.     | |-.'-|=|-'.-|      |
//  | / .`.- -.`. \ '\_`---- /  .`.- -.`. \    | '  '-|=|-'  '_____/
// [|_|/ /  _  \ \|__________| / /  _  \ \ `_____.......-----'_____]
//     ; : / \ : ;'----------'; :  / \  : ;[_____.......-----'; :
//     : ; \_/ ; : \     / /  ; :  \_/  ; :          \ \     / /
//     \ \     / /. `- -` .    \ \     / / LGB        . `- -` .
//      . `- -` .  `-----`      . `- -` .              `-----`
//       `-----`                 `-----`


//REGISTROS
uint8_t AX_registro;
uint8_t BX_registro;
uint8_t CX_registro;
uint8_t DX_registro;
uint32_t EAX_registro;
uint32_t EBX_registro;
uint32_t ECX_registro;
uint32_t EDX_registro;
uint32_t SI_registro;
uint32_t DI_registro;
uint32_t PC_registro;

#endif