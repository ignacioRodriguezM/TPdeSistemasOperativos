#include "../include/tabla_de_paginas.h"

// esto iria en el case cuando me pide un marco la cpu en base a un numero de pagina
//          t_buffer *buffer_recibido = recibir_buffer_sin_cod_op(fd_cpu);
//          uint16_t pid_recibido = extraer_uint16_al_buffer(buffer_recibido);
//          int pagina_a_consultar= extraer_int_al_buffer(buffer_recibido);
//          destruir_buffer(buffer_recibido);

int marco_correspondiente_a_pagina_consultada(uint16_t pid_recibido, int pagina_a_consultar)
{

    t_link_element *current = lista_procesos->head;
    for (int i = 0; i < lista_procesos->elements_count; i++)
    {
        Proceso *proceso = (Proceso *)current->data;
        if (proceso->PID == pid_recibido)
        {
            for (int j = 0; j < proceso->cantidad_paginas; j++)
            {
                if (proceso->tabla_de_paginas[j]->pagina == pagina_a_consultar)
                {
                    return proceso->tabla_de_paginas[j]->marco;
                }
            }

            log_error(memoria_logger, "No existe la pagina que se quiere saber el marco");

            // debo recorrer todas las tablas de paginas hasta encontrar la que coincida con el int de la pagina que me mandan, ahi devuelvo el marco correspondiente a esa tabla
        }
        else
        {
            current = current->next;
        }
    }
    return -1;
}
/*
void devolver_valor_de_memoria(void registroDeDatos , int pagina){ //mov_in
    //hay que escribir el valor de la pagina en el registroDeDatos y devolver este registro
}
void escribir_valor_en_memoria(int pagina , void registroDatos{ //mov_out
    //hay que escribir el valor de registroDeDatos en la direccion de pagina q me mandan
}
*/
void crear_memoria_y_dividir_en_marcos()
{
    memoria_usuario = malloc(tam_memoria);
    cantidad_de_marcos = tam_memoria / tam_pagina;

    marcos_memoria = malloc(cantidad_de_marcos * sizeof(Marco *));

    for (int i = 0; i < cantidad_de_marcos; i++)
    {
        marcos_memoria[i] = malloc(sizeof(Marco));
        marcos_memoria[i]->esta_libre = true;
        marcos_memoria[i]->inicio_marco = memoria_usuario + i * tam_pagina;
    }
}
void crear_tabla_de_paginas(Proceso *proceso)
{
    proceso->tabla_de_paginas = malloc(sizeof(Tabla_paginas *));
    proceso->cantidad_paginas = 0;
}

int division_entera_redondear_arriba(int numerador, int denominador)
{
    // Verificar que el denominador no sea 0 para evitar la división por cero
    if (denominador == 0)
    {
        printf("Error: división por cero.\n");
        return -1; // O cualquier valor de error adecuado
    }

    // División entera redondeando hacia arriba
    int resultado = (numerador + denominador - 1) / denominador;

    return resultado;
}

char *ajustar_tam_proceso(uint16_t PID, uint16_t nuevo_tam_en_bytes)
{
    int paginas_ocupadas;
    int paginas_ajustado = division_entera_redondear_arriba(nuevo_tam_en_bytes, tam_pagina);
    t_link_element *current = lista_procesos->head;
    for (int i = 0; i < lista_procesos->elements_count; i++)
    {
        Proceso *proceso = (Proceso *)current->data;
        if (proceso->PID == PID)
        {
            paginas_ocupadas = proceso->cantidad_paginas;
            break;
        }
        else
        {
            current = current->next;
        }
    }

    Proceso *proceso_a_ajustar = (Proceso *)current->data;

    if (paginas_ajustado > paginas_ocupadas)
    {
        int paginas_a_ocupar = paginas_ajustado - paginas_ocupadas;
        if (chequear_si_hay_marcos_libres(paginas_a_ocupar))
        {
            log_info(memoria_logger, "PID: %u - Tamaño Actual: %d - Tamaño a Ampliar: %d ", proceso_a_ajustar->PID, proceso_a_ajustar->cantidad_paginas, (paginas_a_ocupar * tam_pagina));
            ocupar_marcos(proceso_a_ajustar, paginas_a_ocupar);
            return "OK";
        }
        else
        {
            return "Out Of Memory";
        }
    }
    if (paginas_ajustado < paginas_ocupadas)
    {
        int paginas_a_desocupar = paginas_ocupadas - paginas_ajustado;
        log_info(memoria_logger, "PID: %u - Tamaño Actual: %d - Tamaño a Reducir: %d ", proceso_a_ajustar->PID, proceso_a_ajustar->cantidad_paginas, (paginas_a_desocupar * tam_pagina));
        desocupar_marcos(proceso_a_ajustar, paginas_a_desocupar);
        // acatanvien
        return "OK";
    }
    else
    {
        log_error(memoria_log_debug, "EL PROCESO: %u SOLICITA UN RESIZE DE SU MISMO TAMANIO!!", proceso_a_ajustar->PID);
        return "OK";
    }
}
bool chequear_si_hay_marcos_libres(int paginas_a_ocupar)
{
    int contador = 0;
    for (int i = 0; i < cantidad_de_marcos; i++)
    {
        if (marcos_memoria[i]->esta_libre)
        {
            contador++;
        }
    }

    return (contador >= paginas_a_ocupar);
}

void ocupar_marcos(Proceso *proceso, int paginas_a_ocupar)
{
    proceso->tabla_de_paginas = realloc(proceso->tabla_de_paginas, sizeof(Tabla_paginas *) * (proceso->cantidad_paginas + paginas_a_ocupar));

    for (int i = proceso->cantidad_paginas; i < (paginas_a_ocupar + proceso->cantidad_paginas); i++)
    {
        // Inicializar nuevos punteros de tabla de páginas
        proceso->tabla_de_paginas[i] = malloc(sizeof(Tabla_paginas));
        for (int j = 0; j < cantidad_de_marcos; j++)
        {
            if (marcos_memoria[j]->esta_libre)
            {
                proceso->tabla_de_paginas[i]->pagina = i + 1;
                proceso->tabla_de_paginas[i]->marco = j + 1;
                marcos_memoria[j]->esta_libre = false;
                break;
            }
        }
    }

    proceso->cantidad_paginas += paginas_a_ocupar;
}

void desocupar_marcos(Proceso *proceso, int paginas_a_desocupar)
{
    for (int i = 1; i <= paginas_a_desocupar; i++)
    {
        marcos_memoria[proceso->tabla_de_paginas[proceso->cantidad_paginas - i]->marco]->esta_libre = true;
    }

    proceso->cantidad_paginas -= paginas_a_desocupar;
    proceso->tabla_de_paginas = realloc(proceso->tabla_de_paginas, sizeof(Tabla_paginas *) * proceso->cantidad_paginas);
}
