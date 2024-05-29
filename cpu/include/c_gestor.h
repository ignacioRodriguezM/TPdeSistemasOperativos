#ifndef C_GESTOR_H_
#define C_GESTOR_H_

#include <utils/shared.h>

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#include <commons/collections/dictionary.h>

#include "./configuracion_cpu.h"
#include "./instrucciones.h"
#include "./procesos.h"





//Variables GLOBALES
extern t_log* cpu_logger;
extern t_log* cpu_log_debug;

extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;
extern int fd_kernel_dispatch;
extern int fd_kernel_interrupt;
extern int fd_memoria;

extern bool aviso_de_interrupt;
extern bool bloq_flag;
extern bool interrupt_flag;



//COSAS RECIVIDAS DEL KERNEL QUE NO SON REGISTROS
extern uint16_t PID;


//REGISTROS
extern uint32_t PC_registro;
extern uint8_t AX_registro;
extern uint8_t BX_registro;
extern uint8_t CX_registro;
extern uint8_t DX_registro;
extern uint32_t EAX_registro;
extern uint32_t EBX_registro;
extern uint32_t ECX_registro;
extern uint32_t EDX_registro;
extern uint32_t SI_registro;
extern uint32_t DI_registro;


#endif