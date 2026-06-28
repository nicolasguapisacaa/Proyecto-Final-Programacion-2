#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Verificacion.c" // forma mas rapida, segun gemini esta bien xddd
// ============================================================================
// NUVAS ESTRUCTURAS: LISTAS ENLAZADAS ANIDADAS
// ============================================================================

// 1. Nodo para la lista de canciones
typedef struct NodoCancion
{
	char nombre[50];//titulo de la cancion
	char artista[50];//autor de la cancion (cambiar en el resto de las funciones)
	struct NodoCancion *siguiente;
	struct NodoCancion *anterior;
} NodoCancion;

NodoCancion *InicioC =NULL; //primera cancion en la lista de canciones disponibles
NodoCancion *FinC =NULL;//ultima cancion en la lista de canciones disponibles
// 2. Nodo para la lista de playlists.
// Cada playlist ahora contiene su propia lista de canciones.
typedef struct NodoPlaylist
{
	char nombre[50];
	NodoCancion *listaCanciones; //Puntero al inicio de su lista de canciones
	struct NodoPlaylist *siguiente;
} NodoPlaylist;

// 3. Nodo para el árbol de usuarios.
// Cada usuario ahora es dueño de su propia lista de playlists.
typedef struct NodoUsuario
{
	char correo[50];
	char usuario[50];
	char paisOrigen[50];
	char contrasena[50];
	char tipo[10];				// "premium" o "free"
	NodoPlaylist *misPlaylists; // Puntero al inicio de sus playlists
	struct NodoUsuario *izq;
	struct NodoUsuario *der;
} NodoUsuario;

// Prototipos de funciones
NodoUsuario *insertarUsuario(NodoUsuario *raiz, char paisOrigen[],char correo[],char user[],char pass[],char tipo[]);//no cambiar de lugar
NodoUsuario *buscarUsuario(NodoUsuario *raiz, char correo[]);
void crearPlaylist(NodoUsuario *usuarioActual);		  // Recibe el usuario actual
void agregarCancionAPlaylist(NodoPlaylist *playlist); // Recibe la playlist destino
void verPlaylists(NodoUsuario *usuarioActual);		  // Nueva función para mostrar datos
//Funcion para cargar usuarios ya existentes
NodoUsuario *cargarArchivoU(NodoUsuario *raiz)//cada usuario que lea se manda a la funcion insercion para crear el abb
{
	FILE *archivo = fopen("Usuarios.txt", "r");

	if (archivo == NULL) {
		printf("Error no se pudo abrir el archivo\n");
		return raiz;
	}

	char linea[1000];
	char *paisOrigen, *correo, *usuario, *contrasena, *tipo;

	while (fgets(linea, sizeof(linea), archivo)) {
		linea[strcspn(linea, "\n")] = '\0';
		paisOrigen = strtok(linea, ";");
		correo     = strtok(NULL, ";");
		usuario    = strtok(NULL, ";");
		contrasena = strtok(NULL, ";");
		tipo       = strtok(NULL, ";");

		if (paisOrigen && correo && usuario && contrasena && tipo) {
			raiz = insertarUsuario(raiz, paisOrigen, correo, usuario, contrasena, tipo);
		}
	}

	fclose(archivo);
	printf("Usuarios cargados correctamente\n");
	return raiz;
}

NodoCancion *insertarCancion (char cancion[],char artista[]){
	NodoCancion *nueva = malloc(sizeof(NodoCancion));
 	strcpy(nueva->nombre,cancion);
	strcpy(nueva->artista,artista);
 	nueva->anterior=NULL;
 	nueva->siguiente=NULL;

	if(InicioC==NULL){
		InicioC=nueva;
		FinC=nueva;
	}else{

		nueva->anterior=FinC;
		FinC->siguiente=nueva;
		FinC=nueva;
	}

	return InicioC;
}


NodoCancion *archivoCancionesGenerales(NodoCancion *cancion){
FILE *archivoC= fopen("Canciones.txt","r");
	if(archivoC==NULL){
	printf("Error no se pudieron cargar las canciones disponibles\n");
	return NULL;
	}
	char linea[100];
	char *Titulo,*Artista;

	while(fgets(linea,sizeof(linea),archivoC)){

		linea[strcspn(linea,"\n")]= '\0';
		Titulo= strtok(linea,";");
		Artista= strtok(NULL,";");
		if(Titulo){
			InicioC = insertarCancion (Titulo,Artista);
		}
	}
	printf("Canciones cargadas correctamente\n");

	return InicioC;
}

// ============================================================================
// FUNCIONES DE PLAYLISTS Y CANCIONES (LOGICA IMPLEMENTADA)
// ============================================================================

// Función para agregar canciones directamente a una playlist específica
void agregarCancionAPlaylist(NodoPlaylist *playlist)
{

	char nombreCancion[50];
	printf("Ingrese el nombre de la cancion: ");
	scanf("%s", nombreCancion);

	// Reservar memoria para la nueva canción
	NodoCancion *nuevaCancion = (NodoCancion *)malloc(sizeof(NodoCancion));
	strcpy(nuevaCancion->nombre, nombreCancion);
	nuevaCancion->siguiente = NULL;

	// Insertar la canción al inicio de la lista de canciones de esta playlist
	if (playlist->listaCanciones == NULL)
	{
		playlist->listaCanciones = nuevaCancion;
	}
	else
	{
		nuevaCancion->siguiente = playlist->listaCanciones;
		playlist->listaCanciones = nuevaCancion;
	}
	printf("Cancion '%s' agregada a la playlist.\n", nombreCancion);
}

// Función para crear la playlist asignada al usuario actual y luego preguntar si desea agregar canciones
void crearPlaylist(NodoUsuario *usuarioActual)
{
	if (usuarioActual == NULL)
		return;

	char nombrePlaylist[50];
	printf("Ingrese el nombre de la playlist: ");
	scanf("%s", nombrePlaylist);

	// Reservar memoria para la nueva playlist
	NodoPlaylist *nuevaPlaylist = (NodoPlaylist *)malloc(sizeof(NodoPlaylist));
	strcpy(nuevaPlaylist->nombre, nombrePlaylist);
	nuevaPlaylist->listaCanciones = NULL; // Inicialmente no tiene canciones
	nuevaPlaylist->siguiente = NULL;

	// Insertar la playlist al inicio de la lista de playlists del usuario actual
	if (usuarioActual->misPlaylists == NULL)
	{
		usuarioActual->misPlaylists = nuevaPlaylist;
	}
	else
	{
		nuevaPlaylist->siguiente = usuarioActual->misPlaylists;
		usuarioActual->misPlaylists = nuevaPlaylist;
	}
	printf("Playlist '%s' creada exitosamente.\n", nombrePlaylist);
getchar();
	// Preguntar si desea añadir canciones en este momento
	char respuesta;
	printf("Desea agregar canciones a la playlist? (s/n): ");
	scanf("%c", &respuesta);

	while (respuesta == 's' || respuesta == 'S')
	{
		agregarCancionAPlaylist(nuevaPlaylist); // Le pasamos la playlist que acabamos de crear
		printf("¿Desea agregar otra cancion? (s/n): ");
		scanf("%c", &respuesta);
	}
}

// Nueva función para recorrer e imprimir las playlists y canciones del usuario
void verPlaylists(NodoUsuario *usuarioActual)
{
	if (usuarioActual == NULL || usuarioActual->misPlaylists == NULL)
	{
		printf("No tienes playlists creadas aun.\n");
		return;
	}

	// Recorremos la lista de playlists del usuario
	NodoPlaylist *actualPL = usuarioActual->misPlaylists;
	printf("\n=== TUS PLAYLISTS ===\n");
	while (actualPL != NULL)
	{
		printf("- Playlist: %s\n", actualPL->nombre);

		// Por cada playlist, recorremos su sublista de canciones
		NodoCancion *actualCancion = actualPL->listaCanciones;
		if (actualCancion == NULL)
		{
			printf("  [Esta playlist no tiene canciones]\n");
		}
		else
		{
			while (actualCancion != NULL)
			{
				printf("    > %s\n", actualCancion->nombre);
				actualCancion = actualCancion->siguiente; // Avanzar a la siguiente canción
			}
		}
		actualPL = actualPL->siguiente; // Avanzar a la siguiente playlist
	}
}

void mostrarCanciones(){

	NodoCancion *temp=InicioC;
	printf("==Canciones disponibles==\n");
	while(temp!=NULL){
		printf("%s-%s\n",temp->nombre,temp->artista);
		temp= temp->siguiente;
	}

}
// ============================================================================
// FLUJO PRINCIPAL Y CONFIGURACIÓN DE USUARIOS
// ============================================================================

NodoUsuario *IniciarSesion(NodoUsuario *raiz)
{
	char correo[50];
	char contrasena[50];
	printf("Ingrese su correo: ");
	scanf("%s", correo);
	printf("Ingrese su contrasena: ");
	scanf("%s", contrasena);

	NodoUsuario *usuarioEncontrado = buscarUsuario(raiz, correo);

	if (usuarioEncontrado != NULL && strcmp(usuarioEncontrado->contrasena, contrasena) == 0)
	{
		printf("Inicio de sesion exitoso. Bienvenido, %s!\n", usuarioEncontrado->usuario);
		return usuarioEncontrado;
	}
	else
	{
		printf("Usuario o contrasena incorrectos.\n");
		return NULL;
	}
}
//terminado

NodoUsuario *Registrarse(NodoUsuario *raiz)
{
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

void menuPrincipal(NodoUsuario *usuarioActual)
{
	int opcion;
	do
	{
		printf("\n===Hola %s===\n", usuarioActual->usuario);
		printf("1. Crear playlist\n");
		printf("2. Eliminar playlist\n");
		printf("3. Ver playlists y canciones\n"); 
		printf("4. Agregar amigos\n");
		printf("5. Ver canciones generales\n");
		printf("6. Ver artistas\n");
		printf("7. Ver estadisticas\n");
		printf("8. Configuracion\n");
		printf("9. Cerrar sesion\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion)
		{
		case 1:
		{
			crearPlaylist(usuarioActual); // Enviamos el usuario logueado para modificar sus listas
			break;
		}
		case 2:
		{
			printf("Eliminando playlist..\n");
			break;
		}
		case 3:
		{
			verPlaylists(usuarioActual); // Llama a la nueva función de visualización
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
			mostrarCanciones();
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
	NodoUsuario *raizUsuarios = NULL;
	NodoUsuario *usuarioActual = NULL;
	int opcion;
raizUsuarios=cargarArchivoU(raizUsuarios);
InicioC=archivoCancionesGenerales(InicioC);
	do
	{
		printf("\n1. Iniciar Sesion\n");
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

NodoUsuario *insertarUsuario(NodoUsuario *raiz, char paisOrigen[], char correo[], char user[], char pass[], char tipo[])
{
	if (raiz == NULL)
	{
		NodoUsuario *nuevo = (NodoUsuario *)malloc(sizeof(NodoUsuario));
		strcpy(nuevo->paisOrigen, paisOrigen);
		strcpy(nuevo->correo, correo);
		strcpy(nuevo->usuario, user);
		strcpy(nuevo->contrasena, pass);
		strcpy(nuevo->tipo, tipo);
		nuevo->misPlaylists = NULL; // CAMBIO CLAVE: Inicializar el puntero de playlists en NULL
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
//terminado

NodoUsuario *buscarUsuario(NodoUsuario *raiz, char correo[])
{
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
//terminado