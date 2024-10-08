#include "client.h"

// #include "readline/readline.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char *ip;
	char *puerto;
	char *valor;

	t_log *logger;
	t_config *config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	// Loggeamos el valor de config
	log_info(logger, ip);
	log_info(logger, puerto);
	log_info(logger, valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	// leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion, logger);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log *iniciar_logger(void)
{
	t_log *nuevo_logger;

	if ((nuevo_logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO)) == NULL)
	{
		printf("Hubo un error");
		exit(1);
	}
	return nuevo_logger;
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config;
	char *current_dir = getcwd(NULL, 0);
	char *file_name = "/cliente.config";
	printf("Current dir: %s\n", current_dir);
	printf("file's name: %s\n", file_name);

	size_t len1 = strlen(current_dir);
	size_t len2 = strlen(file_name);
	char *file_dirr = (char *)malloc(len1 + len2 + 1);

	strcpy(file_dirr, current_dir);
	strcat(file_dirr, file_name);

	printf("Concatenated string: %s\n", file_dirr);

	if ((nuevo_config = config_create(file_dirr)) == NULL)
	{
		printf("Fallo al inicializar la config");
		exit(1);
	}

	free(file_dirr);
	free(current_dir);
	return nuevo_config;
}

void leer_consola(t_log *logger)
{
	char *leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

	// ¡No te olvides de liberar las lineas antes de regresar!

	while (!(strcmp(leido, "") == 0))
	{
		log_info(logger, leido);

		free(leido);
		leido = readline("> ");
	}
}

void paquete(int conexion, t_log *logger)
{
	// Ahora toca lo divertido!
	char *leido;
	t_paquete *paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();
	leido = readline("> ");
	while (*leido != '\0')
	{
		log_info(logger, "agregando al paquete...");
		agregar_a_paquete(paquete, leido, sizeof(leido));

		free(leido);
		leido = readline("> ");
	}
	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log *logger, t_config *config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	if (logger != NULL)
	{
		log_destroy(logger);
	}
	if (config != NULL)
	{
		config_destroy(config);
	}
	if (conexion)
	{
		liberar_conexion(conexion);
	}
}
