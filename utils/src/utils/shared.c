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
		buffer->stream = malloc (sizeof(int) + size_of_choclo);
		memcpy(buffer->stream, &size_of_choclo, sizeof(int));
		memcpy(buffer->stream + sizeof(int), un_choclo, size_of_choclo);
	}
	else{
		buffer->stream = realloc(buffer->stream, buffer->size + sizeof(int) + size_of_choclo);
		memcpy(buffer->stream + buffer->size, &size_of_choclo, sizeof(int));
		memcpy(buffer->stream + buffer->size + sizeof(int), un_choclo, size_of_choclo);
	}
	buffer->size += sizeof(int);
	buffer->size += size_of_choclo;
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
	cargar_choclo_al_buffer (buffer, &string, strlen(string) + 1);
}

//=================================================================

void* extraer_choclo_al_buffer (t_buffer* un_buffer){
	if(un_buffer->size==0){
		printf("\n[ERROR] Al intentar extraer contenido de un t_buffer vacio\n\n");
		exit(EXIT_FAILURE);
	}
	if(un_buffer->size<0){
		printf("\n[ERROR] Esto es raro. El t_buffer contiene un size NEGATIVO\n\n");
		exit(EXIT_FAILURE);
	}

	int size_choclo;
	memcpy(&size_choclo, un_buffer->stream, sizeof(int));
	void* choclo = malloc(size_choclo);
	memcpy(choclo, un_buffer->stream + sizeof(int), size_choclo);

	int nuevo_size = un_buffer->size - sizeof(int) - size_choclo;
	if(nuevo_size==0){
		un_buffer->size=0;
		free(un_buffer->stream);
		un_buffer->stream=NULL;
		return choclo;
	}
	if(nuevo_size<0){
		perror("/n[ERROR_CHOCLO] BUFFER con tamanio negativo");
		exit(EXIT_FAILURE);
	}
	void* nuevo_stream = malloc(nuevo_size);
	memcpy(nuevo_stream, un_buffer->stream + sizeof(int) + size_choclo, nuevo_size);
	free(un_buffer->stream);
	un_buffer->size = nuevo_size;
	un_buffer->stream = nuevo_stream;

	return choclo;
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
	memcpy(coso + desplazamiento, &(un_paquete->buffer->stream), un_paquete->buffer->size);
	desplazamiento += un_paquete->buffer->size;

	return coso;
}

void enviar_paquete (t_paquete* paquete, int conexion){
	void* a_enviar = serializar_paquete(paquete);

	int bytes = paquete->buffer->size + 2*sizeof(int);
	send (conexion, a_enviar, bytes, 0);

	free (a_enviar);
}