#include "../include/tlb.h"

#define ERROR_VALUE 0xFFFF

void inicializar_tlb()
{
    TLB = queue_create();
}

void finalizar_tlb()
{
    //queue_destroy_and_destroy_elements(TLB);
}

uint16_t consultar_tlb(uint16_t pid, uint16_t pagina)
{

    t_link_element *current = TLB->elements->head;
    for (int i = 0; i < TLB->elements->elements_count; i++)
    {
        TLB_ENTRY *entrada_tlb = (TLB_ENTRY *)current->data;
        if (entrada_tlb->pid == pid && entrada_tlb->numero_pagina == pagina)
        {
            if(strcmp(ALGORITMO_TLB, "LRU") == 0){
                TLB_ENTRY* a_reencolar = (TLB_ENTRY*)list_remove(TLB->elements, i);
                queue_push(TLB, a_reencolar);
            }
            // si no es LRU, es FIFO. Y con fifo no hay que hacer nada
            log_info(cpu_logger, "PID: <%u> - TLB HIT - Pagina: <%u>", pid, pagina);
            return entrada_tlb->numero_marco;
        }
        else
        {
            current = current->next;
        }
    }
    
    // error value funciona mal si el numero de marco es el 65535. !!!!!!!!!!!!!
    log_info(cpu_logger, "PID: <%u> - TLB MISS - Pagina: <%u>", pid, pagina);
    return ERROR_VALUE;
}

void actualizar_tlb(uint16_t pid, uint16_t pagina, uint16_t marco)
{
    TLB_ENTRY *nueva_entrada = malloc(sizeof(TLB_ENTRY));
    nueva_entrada->pid = pid;
    nueva_entrada->numero_pagina = pagina;
    nueva_entrada->numero_marco = marco;

    if (TLB->elements->elements_count == CANTIDAD_ENTRADAS_TLB)
    {

        TLB_ENTRY *entrada_removida = (TLB_ENTRY *)queue_pop(TLB);
        free(entrada_removida);
        queue_push(TLB, nueva_entrada);
        return;
    }

    else
    {
        queue_push(TLB, nueva_entrada);
        return;
    }
}