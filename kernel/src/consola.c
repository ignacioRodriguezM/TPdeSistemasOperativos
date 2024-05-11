#include "../include/consola.h"

int esNumero(const char *cadena)
{
    // Iterar sobre cada carácter del string
    while (*cadena)
    {
        // Verificar si el carácter actual no es un dígito numérico
        if (!isdigit(*cadena))
        {
            return 0; // No es un número
        }
        cadena++; // Mover al siguiente carácter
    }
    return 1; // Todos los caracteres son dígitos numéricos
}

void iniciar_consola_interactiva()
{
    char *leido;
    leido = readline("> ");
    bool validacion_leido;

    while (strcmp(leido, "\0") != 0)
    {
        validacion_leido = _validacion_de_instruccion_de_consola(leido);
        if (!validacion_leido)
        {
            free(leido);
            leido = readline("> ");
            continue; // saltar al inicio
        }

        _atender_instruccion_validada(leido);
        free(leido);
        leido = readline("> ");
    }

    free(leido);
}

bool _validacion_de_instruccion_de_consola(char *leido)
{
    bool resultado_validacion = false;

    char **comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    { // valida que haya escrito ejecutar script
        if (access(comando_consola[1], F_OK) != -1)
        { // valida que exista el path
            resultado_validacion = true;
        }
        else
        {
            log_error(kernel_logger, "El path no es válido o no se tiene permiso para acceder.");
        }
    }

    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        resultado_validacion = true;
        //  INICIAR_PROCESO /Pruebas_bascas/Prueba_01  proceso de ejemplo
    }

    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        if (esNumero(comando_consola[1]) == 1)
        { // validamos que haya ingresado un numero como process id
            resultado_validacion = true;
        }
        else
        {
            log_error(kernel_logger, "Asegurate de incluir un PID numerico. ");
        }
    }

    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        if (comando_consola[1] == NULL)
        {
            resultado_validacion = true;
        }
        else
        {
            log_error(kernel_logger, "Comando incorrecto, asegurese de escribir solo DETENER_PLANIFICACION");
        }
    }

    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        if (comando_consola[1] == NULL)
        {
            resultado_validacion = true;
        }
        else
        {
            log_error(kernel_logger, "Comando incorrecto, asegurese de escribir solo INICIAR_PLANIFICACION");
        }
    }

    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        if (esNumero(comando_consola[1]) == 1)
        {
            resultado_validacion = true;
        }
        else
        {
            log_error(kernel_logger, "Asegurate de incluir un valor de multiprogramacion correcto. ");
        }
    }

    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        if (comando_consola[1] == NULL)
        {
            resultado_validacion = true;
        }
        else
        {
            log_error(kernel_logger, "Comando incorrecto, asegurese de escribir solo PROCESO_ESTADO");
        }
    }

    else
    {
        log_error(kernel_logger, "Comando de CONSOLA no reconocido ");
        resultado_validacion = false;
    }

    string_array_destroy(comando_consola);

    return resultado_validacion;
}

void _atender_instruccion_validada(char *leido)
{
    char **comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        FILE *archivo;
        char *comando = malloc(100 * sizeof(char));

        // Abrir el archivo de comandos
        archivo = fopen(comando_consola[1], "r");
        // Verificar si se pudo abrir el archivo
        if (archivo == NULL)
        {
            printf("No se pudo abrir el archivo.\n");
            exit(EXIT_FAILURE);
        }
        // Leer y ejecutar los comandos uno por uno
        while (fgets(comando, 100 * sizeof(char), archivo) != NULL)
        {
            printf("%s", comando); // borrar
            _atender_instruccion_validada(comando);
        }
        // Cerrar el archivo
        fclose(archivo);
        // Liberar memoria
        free(comando);
    }

    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        crear_proceso(comando_consola[1]);
    }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        int pid_a_finalizar = atoi(comando_consola[1]); // HAY QUE TRANSFORMAR EL INT A UINT16_T
        if (pid_a_finalizar < 0)
        {
            log_error(kernel_logger, "El PID no puede ser : %d", pid_a_finalizar);
        }
        if(!buscar_en_colas_y_eliminar_el_proceso(pid_a_finalizar)){
            log_error(kernel_log_debug, "EL PID A FINALIZAR NO SE ENCUENTRA EN LAS COLAS");
        }
        
    }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        if (planificacion_activa == false)
        {

            log_info(kernel_logger, "La planificacion ya estaba detenida");
        }
        else
        {
            planificacion_activa = false;
            for(int i=0; i< 9; i++){
                sem_wait(&planificacion_activa_semaforo);
            }
            log_info(kernel_logger, "La planificacion fue detenida");
        }
    }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {

        if (planificacion_activa == true)
        {
            log_info(kernel_logger, "La planificacion ya estaba activa");
        }
        else
        {   
            planificacion_activa = true;
            log_info(kernel_logger, "La planificacion fue activada");
            for(int i=0; i< 9; i++){
                sem_post(&planificacion_activa_semaforo);
            }
            
        }
    }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        int nuevo_grado_multiprogramacion = atoi(comando_consola[1]);
        if (nuevo_grado_multiprogramacion < 1)
        {
            log_error(kernel_logger, "El grado de multiprogramacion no puede ser : %d", nuevo_grado_multiprogramacion);
        }
        if (nuevo_grado_multiprogramacion == grado_multiprogramacion)
        {
            log_info(kernel_logger, "El grado de multiprogramacion ya es : %d", grado_multiprogramacion);
        }
        else
        {
            grado_multiprogramacion = nuevo_grado_multiprogramacion;
            log_info(kernel_logger, "El grado de multiprogramacion cambio a %d", grado_multiprogramacion);
        }
    }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {

        void imprimo_elemento(void *elemento)
        {
            // Assuming your process structure has a field 'pid'
            printf("%d\n", ((PCB *)elemento)->pid);
        }

        // Imprimo los elementos de la cola de cada proceso
        printf("PROCESOS NEW : \n");
        list_iterate(procesos_new->elements, imprimo_elemento);
        printf("PROCESOS READY : \n");
        list_iterate(procesos_ready->elements, imprimo_elemento);
        printf("PROCESOS EXCECUTE : \n");
        list_iterate(procesos_excec->elements, imprimo_elemento);
        printf("PROCESOS EXIT : \n");
        list_iterate(procesos_exit->elements, imprimo_elemento);
        for (int i = 0; i < contador_de_colas_bloqueados; i++)
        {
            printf("PROCESOS BLOQUEADOS POR %s: \n", colas_bloqueados[i]->nombre);
            list_iterate(colas_bloqueados[i]->cola->elements, imprimo_elemento);
        }
    }
    else
    {
        log_error(kernel_logger, "Comando no reconocido, por que paso la validacion?! ");
    }

    string_array_destroy(comando_consola);
}



bool buscar_en_cola (int pid_a_finalizar, t_queue* cola, char* nombre_cola){
    t_link_element *actual = cola->elements->head;
        int index = 0;
        for (int i = 0; i < cola->elements->elements_count; i++)
        {
            PCB *proceso = (PCB *)actual->data;
            if (proceso->pid == pid_a_finalizar)
            { // MOVER PROCESO A EXIT (eliminarlo)
                pthread_mutex_lock(&mutex_procesos);
                list_remove(cola->elements, index);
                queue_push(procesos_exit, proceso);
                pthread_mutex_unlock(&mutex_procesos);


                avisarle_a_memoria_que_libere_recursos_de_proceso(proceso->pid);

                log_info(kernel_logger, "Finaliza el proceso %u - Motivo: FINALIZADO_POR_CONSOLA", proceso->pid);

                log_info(kernel_logger, "PID: %u - Estado Anterior: %s - Estado Actual: EXIT", proceso->pid, nombre_cola);

                return true;
                break;
            }
            else
            {
                actual = actual->next;
                index++;
            }
        }
        return false;
        actual = NULL;
}


bool buscar_en_colas_y_eliminar_el_proceso(int pid_a_finalizar)
{

    ///////////////// BUSCAR EN NEW///////////////////////
    if(buscar_en_cola (pid_a_finalizar, procesos_new, "NEW")){
        sem_post(&grado_multiprogramacion_semaforo);
        return true;
    }
    
    ////////////////// BUSCAR EN EXCECUTE /////////////////

    //HAY Q INTERRUMPIR CPU Y "DESALOJAR EL PROCESO"
    // TERMINAR

    /////////////////// BUSCAR EN READY///////////////
    if(buscar_en_cola (pid_a_finalizar, procesos_ready, "READY")){
        sem_post(&grado_multiprogramacion_semaforo);
        return true;
    }
    

    ////////////////// BUSCAR EN BLOQ////////////////
    for (int i=0; i<contador_de_colas_bloqueados; i++){
        if(colas_bloqueados[i]->conectado){
            if(buscar_en_cola (pid_a_finalizar, colas_bloqueados[i]->cola, "BLOQUEADO")){
                sem_post(&grado_multiprogramacion_semaforo);
                return true;
            }
        }
    }

    /////////////////// BUSCAR EN READY AUX///////////////
    if(strcmp(algoritmo_planificacion, "VRR") == 0){
        if(buscar_en_cola (pid_a_finalizar, procesos_ready_con_prioridad, "READY-PRIORIDAD")){
            sem_post(&grado_multiprogramacion_semaforo);
            return true;
        }
    }

    return false;
}
