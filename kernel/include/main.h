#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include "shared.h"

#include <stdint.h>
#include <pthread.h>
#include <commons/txt.h>
#include <commons/collections/list.h>

#include "./configuracion_kernel.h"

//Variables GLOBALES
t_log* kernel_logger;
t_log* kernel_log_debug;

#endif