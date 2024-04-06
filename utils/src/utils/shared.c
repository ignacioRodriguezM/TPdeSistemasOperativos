#include "/home/utnso/Desktop/tp-2024-1c-SOn-mbulos/utils/src/utils/shared.h"
/*
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
    connect (socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

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
	log_trace(un_log, "SERVER: %s" , msj_server);

	return socket_servidor;
}
int esperar_cliente(int socket_servidor, t_log* un_log)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept (socket_servidor, NULL, NULL);
	log_info(un_log, "Se conecto un cliente!");

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
*/
void inicializar_logger (t_log* logger){
    logger = log_create ("cliente1.log","CL_LOG", 1, LOG_LEVEL_INFO);
    if (logger == NULL){
        perror ("No se pudo crear el log");
        exit (EXIT_FAILURE);
    }
}
void inicializar_logger_debug (t_log* logger){
    logger = log_create ("cliente2.log","CL_LOG", 1, LOG_LEVEL_TRACE);
    if (logger == NULL){
        perror ("No se pudo crear el log");
        exit (EXIT_FAILURE);
    }
}