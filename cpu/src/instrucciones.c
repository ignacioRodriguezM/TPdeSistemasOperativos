#include "../include/instrucciones.h"

void SET_uint8 (uint8_t* registro, int valor){
    *registro = valor;
}
void SET_uint32 (uint32_t* registro, int valor){
    *registro = valor;
}

void SET (void* registro, int valor)
{
    if(sizeof(*registro)==sizeof(uint8_t)){
        SET_uint8(registro, valor);
    }
    else{
        SET_uint32(registro, valor);
    }
}


void SUM_8_a_8 (uint8_t* destino, uint8_t* origen){
    destino = destino + origen;
}
void SUM_32_a_32(uint32_t* destino, uint32_t* origen){
    destino = destino + origen;
}
void SUM_8_a_32(uint32_t* destino, uint8_t* origen){
    destino = destino + origen;
}
void SUM_32_a_8(uint8_t* destino, uint32_t* origen){
    destino = destino + origen;
}

void SUM (void* registroDestino, void* registroOrigen){
    if(sizeof(*registroDestino)==sizeof(uint8_t) && sizeof(*registroOrigen)==sizeof(uint8_t)){
        SUM_8_a_8(registroDestino, registroOrigen);
    }
    else if(sizeof(*registroDestino)==sizeof(uint32_t) && sizeof(*registroOrigen)==sizeof(uint32_t)){
        SUM_32_a_32(registroDestino, registroOrigen);
    }
    else if(sizeof(*registroDestino)==sizeof(uint8_t) && sizeof(*registroOrigen)==sizeof(uint32_t)){
        SUM_32_a_8(registroDestino, registroOrigen);
    }
    else if(sizeof(*registroDestino)==sizeof(uint32_t) && sizeof(*registroOrigen)==sizeof(uint8_t)){
        SUM_8_a_32(registroDestino, registroOrigen);
    }
}
