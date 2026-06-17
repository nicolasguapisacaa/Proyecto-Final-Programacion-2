#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Verificacion.c" //forma mas rapida
// #include "Verificacion.h"//forma profesional pero no termino de entender xddddd
// se supone que ahora en tu vscode puedes correr el programa solo con el play

typedef struct NodoUsuario
{ // Nodo para el árbol de usuarios
	char correo[50];
	char usuario[50];
	char paisOrigen[50];
	char contrasena[50];
	char tipo[10]; // "premium" o "free"
	struct NodoUsuario *izq;
	struct NodoUsuario *der;
} NodoUsuario;

// Prototipos de funciones
NodoUsuario *insertarUsuario(NodoUsuario *raiz, char paisOrigen[], char correo[], char user[], char pass[], char tipo[]); // Función para insertar un nuevo usuario en el árbol
NodoUsuario *buscarUsuario(NodoUsuario *raiz, char correo[]);															  // Función para buscar un usuario en el árbol por su correo

NodoUsuario *IniciarSesion(NodoUsuario *raiz) // Ya funciona bien
{											  // Función para iniciar sesión, que solicita el nombre de usuario y la contraseña, y verifica si son correctos
	char correo[50];
	char contrasena[50];
	printf("Ingrese su correo: "); // se verifica por el correo y el usuario se muestra en la pantalla principal
	scanf("%s", correo);
	printf("Ingrese su contrasena: ");
	scanf("%s", contrasena);

	NodoUsuario *usuarioEncontrado = buscarUsuario(raiz, correo);

	if (usuarioEncontrado != NULL && strcmp(usuarioEncontrado->contrasena, contrasena) == 0)
	{ // Verificar que la contraseña coincida
		printf("Inicio de sesion exitoso. Bienvenido, %s!\n", usuarioEncontrado->usuario);
		return usuarioEncontrado;
	}
	else
	{
		printf("Usuario o contrasena incorrectos.\n");
		return NULL;
	}
}

NodoUsuario *Registrarse(NodoUsuario *raiz) // Ya funciona bien
{											// Función para registrarse, que solicita el nombre de usuario, la contraseña y el tipo de cuenta (premium o free), y luego inserta el nuevo usuario en el árbol
	char usuario[50];
	char contrasena[50];
	char paisOrigen[50];
	char tipo[10];
	char respuesta;
	char correo[50];
	int i = 0;
	do
	{
		printf("Escriba su correo electronico\n");
		fgets(correo, 50, stdin);
		correo[strcspn(correo, "\n")] = '\0';
		i = verificacionCorreo(correo);
	} while (i != 1);
	printf("Ingrese su usuario: ");
	scanf("%s", usuario);
	printf("Ingrese su contrasena: ");
	scanf("%s", contrasena);
	printf("Ingrese su pais de origen: ");
	scanf("%s", paisOrigen);
	i = 0;
	do
	{
		printf("Ingrese el tipo de cuenta (premium o free): ");
		scanf("%s", tipo);
		i = verificacionPoF(tipo);
	} while (i != 1);
	if (strcmp(tipo, "premium") == 0)
	{
		getchar();
		printf("La cuenta premium tiene un costo de $9.99 al mes y puedes disfrutar de una experiencia sin anuncios.\n");
		printf("¿Desea continuar con la cuenta premium? (s/n): ");
		scanf("%c", &respuesta);
		if (respuesta != 's' && respuesta != 'S')
		{
			printf("Registro cancelado. Volviendo al menu principal.\n");
			return raiz;
		}
	}

	raiz = insertarUsuario(raiz, paisOrigen, correo, usuario, contrasena, tipo);
	printf("Registro exitoso. Bienvenido, %s!\n", usuario);
	return raiz;
}

void menuPrincipal(NodoUsuario *usuarioActual) // Falta agregar las funciones de cada case, pero el menu ya funciona bien
{
	int opcion;
	do
	{
		printf("\n===Hola %s===\n", usuarioActual->usuario);
		printf("1. Crear playlist\n");
		printf("2. Eliminar playlist\n");
		printf("3. Ver playlist\n");
		printf("4. Agregar amigos\n");
		printf("5. Ver canciones\n");
		printf("6. Ver artistas\n");
		printf("7. Ver estadisticas\n");
		printf("8. Configuracion\n"); // crud del usuario
		printf("9. Cerrar sesion\n");
		scanf("%d", &opcion);
		switch (opcion)
		{
		case 1:
		{
			crearPlaylist();
			break;
		}
		case 2:
		{
			printf("Eliminando playlist..\n");
			break;
		}
		case 3:
		{
			printf("Mostrando playlist..\n");
			break;
		}
		case 4:
		{
			printf("Agregando amigos..\n");
			break;
		}
		case 5:
		{
			printf("Mostrando canciones..\n");
			break;
		}
		case 6:
		{
			printf("Mostrando artistas..\n");
			break;
		}
		case 7:
		{
			printf("Mostrando estadisticas..\n");
			break;
		}
		case 8:
		{
			printf("Mostrando configuracion..\n");
			break;
		}
		case 9:
		{
			printf("Volviendo al menu principal..\n");
			break;
		}
		}
	} while (opcion != 9);
}

int main()
{
	// MAIN
	NodoUsuario *raizUsuarios = NULL;
	NodoUsuario *usuarioActual = NULL;
	int opcion;

	do
	{
		printf("1. Iniciar Sesion\n");
		printf("2. Registrarse\n");
		printf("3. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);

		switch (opcion)
		{
		case 1:
			usuarioActual = IniciarSesion(raizUsuarios);
			if (usuarioActual != NULL)
			{
				menuPrincipal(usuarioActual);
			}
			break;
		case 2:
			getchar();
			raizUsuarios = Registrarse(raizUsuarios);
			break;
		case 3:
			printf("Saliendo del programa...\n");
			break;
		default:
			printf("Opcion no valida. Intente de nuevo.\n");
		}
	} while (opcion != 3);

	return 0;
}

NodoUsuario *insertarUsuario(NodoUsuario *raiz, char paisOrigen[], char correo[], char user[], char pass[], char tipo[]) // Ya funciona bien
{																														 // Función para insertar un nuevo usuario en el árbol
	if (raiz == NULL)
	{
		NodoUsuario *nuevo = (NodoUsuario *)malloc(sizeof(NodoUsuario));
		strcpy(nuevo->paisOrigen, paisOrigen);
		strcpy(nuevo->correo, correo);
		strcpy(nuevo->usuario, user);
		strcpy(nuevo->contrasena, pass);
		strcpy(nuevo->tipo, tipo);
		nuevo->izq = NULL;
		nuevo->der = NULL;
		return nuevo;
	}

	int comparacion = strcmp(user, raiz->usuario);
	if (comparacion < 0)
	{
		raiz->izq = insertarUsuario(raiz->izq, paisOrigen, correo, user, pass, tipo);
	}
	else if (comparacion > 0)
	{
		raiz->der = insertarUsuario(raiz->der, paisOrigen, correo, user, pass, tipo);
	}
	else
	{
		printf("El usuario ya existe.\n");
	}
	return raiz;
}

NodoUsuario *buscarUsuario(NodoUsuario *raiz, char correo[]) // Ya funciona bien

{ // Función para buscar un usuario en el árbol por su correo
	if (raiz == NULL || strcmp(correo, raiz->correo) == 0)
	{
		return raiz;
	}

	if (strcmp(correo, raiz->correo) < 0)
	{
		return buscarUsuario(raiz->izq, correo);
	}
	else
	{
		return buscarUsuario(raiz->der, correo);
	}
}

// Funcion para crear playlists, funciona bien pero falta agregar la logica para almacenar las canciones en la playlist, y tambien falta agregar la logica para mostrar las playlists creadas por el usuario
void crearPlaylist()
{
	char nombrePlaylist[50];
	printf("Ingrese el nombre de la playlist: ");
	scanf("%s", nombrePlaylist);
	printf("Playlist '%s' creada exitosamente.\n", nombrePlaylist);
	printf("Desea agregar canciones a la playlist? (s/n): ");
	char respuesta;
	scanf(" %c", &respuesta);
	if (respuesta == 's' || respuesta == 'S')
	{
		printf("Agregando canciones a la playlist '%s'...\n", nombrePlaylist);
		agregarCanciones();

		almacenarCanciones();
	}
	else
	{
		printf("Playlist '%s' creada sin canciones.\n", nombrePlaylist);
	}
}

//Funcion para agregar canciones a la playlist
void agregarCanciones()
{
	char nombreCancion[50];
	printf("Ingrese el nombre de la cancion: ");
	scanf("%s", nombreCancion);
	printf("Cancion '%s' agregada a la playlist.\n", nombreCancion);
	// Aquí puedes agregar la lógica para almacenar las canciones en la playlist
}

void almacenarCanciones()
{
	// Aquí puedes agregar la lógica para almacenar las canciones en la playlist
}

typedef struct NodoPlaylist
{ // Nodo para la lista de playlists
	char nombre[50];
	struct NodoPlaylist *siguiente;
} NodoPlaylist;

typedef struct NodoCancion
{ // Nodo para la lista de canciones
	char nombre[50];
	struct NodoCancion *siguiente;
} NodoCancion;

