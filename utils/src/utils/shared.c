#include <utils/shared.h>

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
//	int socket_cliente = 0;
    int socket_cliente = socket (server_info->ai_family,
                                    server_info->ai_socktype,
                                    server_info->ai_protocol );

	// Ahora que tenemos el socket, vamos a conectarlo
    
	while (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
            perror("Error al conectarse");
			 sleep(5);
			 }
	
	freeaddrinfo(server_info);

	return socket_cliente;
}

int iniciar_servidor(char* puerto, t_log* un_log, char* msj_server)
{

	struct addrinfo hints, *servinfo;
	//struct addrinfo *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
    int socket_servidor = socket (servinfo->ai_family,
                                    servinfo->ai_socktype,
                                    servinfo->ai_protocol );
	// Asociamos el socket a un puerto
    bind (socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
	// Escuchamos las conexiones entrantes
    listen (socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	log_info(un_log, "SERVER: %s" , msj_server);

	return socket_servidor;
}
int esperar_cliente(int socket_servidor, t_log* un_log, char* nombre_cliente)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept (socket_servidor, NULL, NULL);
	log_info(un_log, "Se conecto el cliente: %s !!!", nombre_cliente);

	return socket_cliente;
}
int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}
int recibir_size_del_buffer(int socket_cliente)
{
	int size_del_buffer;
	if(recv(socket_cliente, &size_del_buffer, sizeof(int), MSG_WAITALL) > 0)
		return size_del_buffer;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void inicializar_logger (t_log** logger, char* nombre){
    *logger = log_create (nombre,"CL_LOG", 1, LOG_LEVEL_INFO);
    if (*logger == NULL){
        perror ("No se pudo crear el log");
        exit (EXIT_FAILURE);
    }
}
void inicializar_logger_debug (t_log** logger, char* nombre){
    *logger = log_create (nombre,"CL_LOG", 1, LOG_LEVEL_TRACE);
    if (*logger == NULL){
        perror ("No se pudo crear el log");
        exit (EXIT_FAILURE);
    }
}

//======================================

t_buffer* crear_buffer (){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size=0;
	buffer->stream=NULL;
	return buffer;
}

void destruir_buffer (t_buffer* buffer){
	if(buffer->stream != NULL){
		free(buffer->stream);
	}
	free (buffer);
}
//====================================================

void cargar_choclo_al_buffer (t_buffer* buffer, void* un_choclo, int size_of_choclo){
	if(buffer->size==0){
		// Si el buffer está vacío, asignar memoria para el stream y copiar los datos
		buffer->stream = malloc (sizeof(int) + size_of_choclo);
		memcpy(buffer->stream, &size_of_choclo, sizeof(int));
		memcpy(buffer->stream + sizeof(int), un_choclo, size_of_choclo);
	}
	else{
		// Si el buffer ya contiene datos, realojar memoria para el stream y concatenar los nuevos datos
		buffer->stream = realloc(buffer->stream, buffer->size + sizeof(int) + size_of_choclo);
		memcpy(buffer->stream + buffer->size, &size_of_choclo, sizeof(int));
		memcpy(buffer->stream + buffer->size + sizeof(int), un_choclo, size_of_choclo);
	}
	// Actualizar el tamaño del buffer
	buffer->size += sizeof(int) + size_of_choclo;
}

void cargar_int_al_buffer (t_buffer* buffer, int valor){
	cargar_choclo_al_buffer (buffer, &valor, sizeof(int));
}
void cargar_uint32_al_buffer (t_buffer* buffer, uint32_t valor){
	cargar_choclo_al_buffer (buffer, &valor, sizeof(uint32_t));
}
void cargar_uint16_al_buffer (t_buffer* buffer, uint16_t valor){
	cargar_choclo_al_buffer (buffer, &valor, sizeof(uint16_t));
}
void cargar_string_al_buffer (t_buffer* buffer, char* string){
	size_t len = strlen(string) + 1;
	cargar_choclo_al_buffer(buffer, (void*)string, len);
}

//=================================================================

void* extraer_choclo_al_buffer(t_buffer* un_buffer) {
    // Verificar si el buffer está vacío
    if(un_buffer->size == 0) {
        printf("\n[ERROR] Al intentar extraer contenido de un t_buffer vacio\n\n");
        exit(EXIT_FAILURE);
    }
    // Verificar si el tamaño del buffer es negativo (lo cual sería inusual y un error)
    if(un_buffer->size < 0) {
        printf("\n[ERROR] Esto es raro. El t_buffer contiene un size NEGATIVO\n\n");
        exit(EXIT_FAILURE);
    }

    // Obtener el tamaño del dato del buffer
    int size_choclo;
    memcpy(&size_choclo, un_buffer->stream, sizeof(int));  //EL PROBLEMA ESTA AQUI
    
    // Asignar memoria para el dato
    void* choclo = malloc(size_choclo);
    if(choclo == NULL) {
        perror("Error al asignar memoria para choclo");
        exit(EXIT_FAILURE);
    }
    
    // Copiar el dato desde el buffer
    memcpy(choclo, un_buffer->stream + sizeof(int), size_choclo);

    // Calcular el nuevo tamaño del buffer después de extraer el dato
    int nuevo_size = un_buffer->size - sizeof(int) - size_choclo;
    
    // Verificar si el nuevo tamaño del buffer es 0
    if(nuevo_size == 0) {
        // Si es 0, liberar el stream del buffer y establecer el tamaño del buffer en 0
        un_buffer->size = 0;
        free(un_buffer->stream);
        un_buffer->stream = NULL;
        return choclo; // Devolver el dato extraído
    }
    // Verificar si el nuevo tamaño del buffer es negativo (lo cual sería un error)
    if(nuevo_size < 0) {
        perror("/n[ERROR_CHOCLO] BUFFER con tamanio negativo");
        exit(EXIT_FAILURE);
    }
    
    // Si el nuevo tamaño del buffer es mayor que 0, realojar memoria para el nuevo stream del buffer
    void* nuevo_stream = malloc(nuevo_size);
    if(nuevo_stream == NULL) {
        perror("Error al asignar memoria para nuevo_stream");
        exit(EXIT_FAILURE);
    }
    // Copiar el resto del buffer al nuevo stream
    memcpy(nuevo_stream, un_buffer->stream + sizeof(int) + size_choclo, nuevo_size);
    // Liberar el stream original y actualizar el stream y tamaño del buffer
    free(un_buffer->stream);
    un_buffer->size = nuevo_size;
    un_buffer->stream = nuevo_stream;

    return choclo; // Devolver el dato extraído
}


int extraer_int_al_buffer (t_buffer* un_buffer){
	int* un_entero = extraer_choclo_al_buffer(un_buffer);
	int valor_retorno = *un_entero;
	free(un_entero);
	return valor_retorno;
}
uint32_t extraer_uint32_al_buffer (t_buffer* un_buffer){
	uint32_t* un_entero = extraer_choclo_al_buffer(un_buffer);
	uint32_t valor_retorno = *un_entero;
	free(un_entero);
	return valor_retorno;
}
uint16_t extraer_uint16_al_buffer (t_buffer* un_buffer){
	uint16_t* un_entero = extraer_choclo_al_buffer(un_buffer);
	uint16_t valor_retorno = *un_entero;
	free(un_entero);
	return valor_retorno;
}
char* extraer_string_al_buffer (t_buffer* un_buffer){
	char* un_string = extraer_choclo_al_buffer(un_buffer);
	return un_string;
}

t_paquete* crear_paquete (op_code cod_op, t_buffer* un_buffer){
	t_paquete* un_paquete = malloc (sizeof(t_paquete));
	un_paquete->codigo_operacion = cod_op;
	un_paquete->buffer = un_buffer;
	return un_paquete;
}

void destruir_paquete (t_paquete* un_paquete){
	destruir_buffer(un_paquete->buffer);
	free(un_paquete);
}
void* serializar_paquete (t_paquete* un_paquete){
	int size_coso = un_paquete->buffer->size + 2* sizeof(int);
	void* coso = malloc(size_coso);
	int desplazamiento = 0;
	memcpy(coso + desplazamiento, &(un_paquete->codigo_operacion), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(coso + desplazamiento, &(un_paquete->buffer->size), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(coso + desplazamiento, un_paquete->buffer->stream, un_paquete->buffer->size);

	return coso;
}

void enviar_paquete (t_paquete* paquete, int conexion){
	void* a_enviar = serializar_paquete(paquete);

	int bytes_enviados = 0;
    int bytes_a_enviar = paquete->buffer->size + 2 * sizeof(int);

    while (bytes_enviados < bytes_a_enviar) {
        int bytes_enviados_actual = send(conexion, a_enviar + bytes_enviados, bytes_a_enviar - bytes_enviados, 0);
        if (bytes_enviados_actual == -1) {
            perror("Error al enviar el paquete");
            exit(EXIT_FAILURE);
        }
        bytes_enviados += bytes_enviados_actual;
    }

    free(a_enviar);

    // Verificar si se enviaron todos los bytes del paquete
    if (bytes_enviados == bytes_a_enviar) {
        printf("El paquete se ha enviado completamente.\n");
    } else {
        printf("Error: No se han enviado todos los bytes del paquete.\n");
    }
}




// Función para extraer un entero del paquete
int extraer_int_del_paquete(t_paquete* paquete) {
    return extraer_int_al_buffer(paquete->buffer);
}

// Función para extraer un uint32_t del paquete
uint32_t extraer_uint32_del_paquete(t_paquete* paquete) {
    return extraer_uint32_al_buffer(paquete->buffer);
}

// Función para extraer un uint16_t del paquete
uint16_t extraer_uint16_del_paquete(t_paquete* paquete) {
    return extraer_uint16_al_buffer(paquete->buffer);
}

// Función para extraer un string del paquete
char* extraer_string_del_paquete(t_paquete* paquete) {
    return extraer_string_al_buffer(paquete->buffer);
}
