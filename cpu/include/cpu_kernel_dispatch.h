#ifndef CPU_KERNEL_DISPATCH_H_
#define CPU_KERNEL_DISPATCH_H_

#include "./c_gestor.h"

void _extraer_contexto_de_ejecucion();
void _desalojar_proceso_por_quantum();

void atender_cpu_kernel_dispatch ();


#endif