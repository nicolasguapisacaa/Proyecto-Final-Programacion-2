#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ============================================================================
// NUVAS ESTRUCTURAS: LISTAS ENLAZADAS ANIDADAS
// ============================================================================

// 1. Nodo para la lista de canciones
typedef struct NodoCancion {
	char nombre[50];//titulo de la cancion
	char artista[50];//autor de la cancion (cambiar en el resto de las funciones)
	struct NodoCancion *siguiente;
	struct NodoCancion *anterior;
} NodoCancion;

NodoCancion *InicioC =NULL; //primera cancion en la lista de canciones disponibles
NodoCancion *FinC =NULL;//ultima cancion en la lista de canciones disponibles
// 2. Nodo para la lista de playlists.
// Cada playlist ahora contiene su propia lista de canciones.
typedef struct NodoPlaylist {
	char nombre[50];
	NodoCancion *listaCanciones; //Puntero al inicio de su lista de canciones
	struct NodoPlaylist *siguiente;
} NodoPlaylist;

// 4. Nodo para la lista de amigos de un usuario.
// Guarda un puntero directo al usuario amigo dentro del arbol de usuarios.
typedef struct NodoAmigo {
	struct NodoUsuario *amigo;
	struct NodoAmigo *siguiente;
} NodoAmigo;

// 3. Nodo para el árbol de usuarios.
// Cada usuario ahora es dueño de su propia lista de playlists y de amigos.
typedef struct NodoUsuario {
	char correo[50];
	char usuario[50];
	char paisOrigen[50];
	char contrasena[50];
	char tipo[10];				// "premium" o "free"
	NodoPlaylist *misPlaylists; // Puntero al inicio de sus playlists
	NodoAmigo *misAmigos;		 // Puntero al inicio de su lista de amigos
	struct NodoUsuario *izq;
	struct NodoUsuario *der;
} NodoUsuario;

// Prototipos de funciones
NodoUsuario *insertarUsuario(NodoUsuario *raiz, char paisOrigen[],char correo[],char user[],char pass[],char tipo[]);//no cambiar de lugar
NodoUsuario *buscarUsuario(NodoUsuario *raiz, char correo[]);
void crearPlaylist(NodoUsuario *usuarioActual);		  // Recibe el usuario actual
void agregarCancionAPlaylist(NodoPlaylist *playlist); // Recibe la playlist destino
void verPlaylists(NodoUsuario *usuarioActual);		  // Nueva función para mostrar datos
int verificacionCorreo(char correo[]);
int verificacionPoF(char tipo[]);
NodoUsuario *cargarArchivoU(NodoUsuario *raiz);
NodoCancion *insertarCancion (char cancion[],char artista[]);
NodoCancion *archivoCancionesGenerales(NodoCancion *cancion);
NodoCancion *buscarCancion( char busqueda[]);
void agregarCancionAPlaylist(NodoPlaylist *playlist);
NodoPlaylist *buscarPlaylist(char nombre[],NodoUsuario *usuarioAct);
void reproducirPlaylist(NodoUsuario *usuarioActual);
NodoPlaylist *eliminarPlaylist(NodoUsuario *usuarioActual);
void submenuPlaylists(NodoUsuario *usuarioActual);
void submenuCanciones(NodoUsuario *usuarioActual);
void mostrarArtistas();
void buscarPorArtista();
void submenuArtistas();
void verAmigos(NodoUsuario *usuarioActual);
void agregarAmigo(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios);
void eliminarAmigo(NodoUsuario *usuarioActual);
void submenuAmigos(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios);
void menuPrincipal(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios);
void GuardarDatos(NodoUsuario *raiz);
void eliminarABBUsuario(NodoUsuario *raiz, FILE *archivo);
// ============================================================================
// FUNCIONES DE PLAYLISTS Y CANCIONES (LOGICA IMPLEMENTADA)
// ============================================================================

// Función para crear la playlist asignada al usuario actual y luego preguntar si desea agregar canciones
void crearPlaylist(NodoUsuario *usuarioActual) {
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
	if (usuarioActual->misPlaylists == NULL) {
		usuarioActual->misPlaylists = nuevaPlaylist;
	} else {
		nuevaPlaylist->siguiente = usuarioActual->misPlaylists;
		usuarioActual->misPlaylists = nuevaPlaylist;
	}
	printf("Playlist '%s' creada exitosamente.\n", nombrePlaylist);
	getchar();
	// Preguntar si desea añadir canciones en este momento
	char respuesta;
	printf("Desea agregar canciones a la playlist? (s/n): ");
	scanf("%c", &respuesta);

	while (respuesta == 's' || respuesta == 'S') {
		agregarCancionAPlaylist(nuevaPlaylist); // Le pasamos la playlist que acabamos de crear
		printf("Desea agregar otra cancion? (s/n):");
		scanf(" %c", &respuesta);
	}
}

// Nueva función para recorrer e imprimir las playlists y canciones del usuario
void verPlaylists(NodoUsuario *usuarioActual) {
	if (usuarioActual == NULL || usuarioActual->misPlaylists == NULL) {
		printf("No tienes playlists creadas aun.\n");
		return;
	}

	// Recorremos la lista de playlists del usuario
	NodoPlaylist *actualPL = usuarioActual->misPlaylists;
	printf("\n=== TUS PLAYLISTS ===\n");
	while (actualPL != NULL) {
		printf("- Playlist: %s\n", actualPL->nombre);

		// Por cada playlist, recorremos su sublista de canciones
		NodoCancion *actualCancion = actualPL->listaCanciones;
		if (actualCancion == NULL) {
			printf("  [Esta playlist no tiene canciones]\n");
		} else {
			while (actualCancion != NULL) {
				printf("    > %s\n", actualCancion->nombre);
				actualCancion = actualCancion->siguiente; // Avanzar a la siguiente canción
			}
		}
		actualPL = actualPL->siguiente; // Avanzar a la siguiente playlist
	}
}

void mostrarCanciones() {

	NodoCancion *temp=InicioC;
	printf("==Canciones disponibles==\n");
	while(temp!=NULL) {
		printf("%s-%s\n",temp->nombre,temp->artista);
		temp= temp->siguiente;
	}

}

// Recorre la lista de canciones e imprime cada artista una sola vez
void mostrarArtistas() {
	if (InicioC == NULL) {
		printf("No hay artistas cargados\n");
		return;
	}

	char vistos[200][50];
	int cantidad = 0;
	NodoCancion *temp = InicioC;

	printf("==Artistas disponibles==\n");
	while (temp != NULL) {
		int repetido = 0;
		int i;
		for (i = 0; i < cantidad; i++) {
			if (strcmp(vistos[i], temp->artista) == 0) {
				repetido = 1;
				break;
			}
		}
		if (!repetido && cantidad < 200) {
			strcpy(vistos[cantidad], temp->artista);
			cantidad++;
			printf("- %s\n", temp->artista);
		}
		temp = temp->siguiente;
	}
}

// Busca todas las canciones que pertenecen a un artista especifico
void buscarPorArtista() {
	if (InicioC == NULL) {
		printf("No se encuentran canciones cargadas\n");
		return;
	}

	char artista[50];
	getchar();
	printf("Escriba el nombre del artista a buscar: ");
	fgets(artista, sizeof(artista), stdin);
	artista[strcspn(artista, "\n")] = '\0';

	NodoCancion *temp = InicioC;
	int encontrado = 0;
	printf("\n=== Canciones de %s ===\n", artista);
	while (temp != NULL) {
		if (strcmp(temp->artista, artista) == 0) {
			printf("- %s\n", temp->nombre);
			encontrado = 1;
		}
		temp = temp->siguiente;
	}
	if (!encontrado) {
		printf("No se encontraron canciones de ese artista\n");
	}
}

void mostrarPlaylist(NodoUsuario *usuarioActual) {
	NodoPlaylist *temp;
	temp=usuarioActual->misPlaylists;

	while(temp!=NULL) {

		printf("Playlist: %s",temp->nombre);
		temp=temp->siguiente;
	}

}

void reproducirPlaylist(NodoUsuario *usuarioActual){
	if (usuarioActual == NULL || usuarioActual -> misPlaylists ==NULL){
		printf ("No existen playlists creadas\n");
		return;
	}

	char nombreP[50];
	getchar();
	printf ("Ingrese el nombre de la playlist: ");
	fgets (nombreP, sizeof(nombreP), stdin);
	nombreP[strcspn(nombreP, "\n")]= '\0';

	NodoPlaylist *playlist = buscarPlaylist(nombreP, usuarioActual);

	if (playlist == NULL){
		return;

	}

	if (playlist -> listaCanciones==NULL){
		printf ("Esta playlist no contiene canciones para reproducir\n");
		return;
	}

	NodoCancion *cancionActual = playlist -> listaCanciones;
	int opcion = 0;
	int enReproduccion = 1; //Si esta con 1 esta reproduciendo, en 0 en pausa

	do {
		printf("\n=== Reproduciendo: %s - %s ===\n", cancionActual->nombre, cancionActual->artista);
		printf("1. Siguiente\n");
		printf("2. Anterior\n");
		printf("3. Detener reproduccion\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch(opcion) {
			case 1:
				if (cancionActual->siguiente != NULL) {
					cancionActual = cancionActual->siguiente;
				} else {
					printf("No hay mas canciones siguientes.\n");
				}
				break;
			case 2:
				if (cancionActual->anterior != NULL) {
					cancionActual = cancionActual->anterior;
				} else {
					printf("No hay canciones anteriores.\n");
				}
				break;
			case 3:
				printf("Reproduccion detenida.\n");
				enReproduccion = 0;
				break;
			default:
				printf("Opcion no valida\n");
		}
	} while (opcion != 3);
}

NodoPlaylist *eliminarPlaylist(NodoUsuario *usuarioActual) {
	NodoPlaylist *temp;
	NodoPlaylist *act=NULL;
	temp=usuarioActual->misPlaylists;

	char Dplay[50];
	if(usuarioActual->misPlaylists==NULL) {

		printf("No se encuentran playlist\n");
		return NULL;
	}

	mostrarPlaylist(usuarioActual);
	getchar();
	printf("Escriba el nombre de la playlist a borrar:");
	fgets(Dplay,sizeof(Dplay),stdin);
	Dplay[strcspn(Dplay,"\n")]='\0';
	while(temp!=NULL) {
		if(strcmp(Dplay,temp->nombre)==0) {
			NodoCancion *borrarC=temp->listaCanciones;
			while(borrarC!=NULL) {
				NodoCancion *cancionAct=borrarC;
				borrarC=borrarC->siguiente;
				free(cancionAct);
			}
			if(act==NULL) {

				usuarioActual->misPlaylists=temp->siguiente;

			} else {
				act->siguiente=temp->siguiente;
			}
			free(temp);
			printf("Playlist eliminada correctamenten\n");
			return usuarioActual->misPlaylists;

		}

		act=temp;
		temp=temp->siguiente;
	}
	printf("No se encontro la playlist\n");
	return usuarioActual->misPlaylists;

}

// ============================================================================
// FUNCIONES DE AMIGOS
// ============================================================================

// Revisa si "candidato" ya esta en la lista de amigos de "usuarioActual"
int existeAmigo(NodoUsuario *usuarioActual, NodoUsuario *candidato) {
	NodoAmigo *temp = usuarioActual->misAmigos;
	while (temp != NULL) {
		if (temp->amigo == candidato) {
			return 1;
		}
		temp = temp->siguiente;
	}
	return 0;
}

// Muestra la lista de amigos del usuario actual
void verAmigos(NodoUsuario *usuarioActual) {
	if (usuarioActual->misAmigos == NULL) {
		printf("No tienes amigos agregados aun.\n");
		return;
	}

	printf("\n=== TUS AMIGOS ===\n");
	NodoAmigo *temp = usuarioActual->misAmigos;
	while (temp != NULL) {
		printf("- %s (%s)\n", temp->amigo->usuario, temp->amigo->correo);
		temp = temp->siguiente;
	}
}

// Busca un usuario por correo en el arbol y lo agrega como amigo (de forma reciproca)
void agregarAmigo(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios) {
	char correoAmigo[50];
	getchar();
	printf("Ingrese el correo del amigo que desea agregar: ");
	fgets(correoAmigo, sizeof(correoAmigo), stdin);
	correoAmigo[strcspn(correoAmigo, "\n")] = '\0';

	if (strcmp(correoAmigo, usuarioActual->correo) == 0) {
		printf("No puedes agregarte a ti mismo como amigo.\n");
		return;
	}

	NodoUsuario *encontrado = buscarUsuario(raizUsuarios, correoAmigo);
	if (encontrado == NULL) {
		printf("No se encontro ningun usuario con ese correo.\n");
		return;
	}

	if (existeAmigo(usuarioActual, encontrado)) {
		printf("Ya tienes a %s en tu lista de amigos.\n", encontrado->usuario);
		return;
	}

	// Se agrega en la lista del usuario actual
	NodoAmigo *nuevo = (NodoAmigo *)malloc(sizeof(NodoAmigo));
	nuevo->amigo = encontrado;
	nuevo->siguiente = usuarioActual->misAmigos;
	usuarioActual->misAmigos = nuevo;

	// Se agrega reciprocamente en la lista del otro usuario
	NodoAmigo *nuevoReciproco = (NodoAmigo *)malloc(sizeof(NodoAmigo));
	nuevoReciproco->amigo = usuarioActual;
	nuevoReciproco->siguiente = encontrado->misAmigos;
	encontrado->misAmigos = nuevoReciproco;

	printf("%s ahora es tu amigo.\n", encontrado->usuario);
}

// Elimina la amistad en ambos sentidos
void eliminarAmigo(NodoUsuario *usuarioActual) {
	if (usuarioActual->misAmigos == NULL) {
		printf("No tienes amigos para eliminar.\n");
		return;
	}

	verAmigos(usuarioActual);

	char correoAmigo[50];
	getchar();
	printf("Ingrese el correo del amigo a eliminar: ");
	fgets(correoAmigo, sizeof(correoAmigo), stdin);
	correoAmigo[strcspn(correoAmigo, "\n")] = '\0';

	NodoAmigo *temp = usuarioActual->misAmigos;
	NodoAmigo *ant = NULL;

	while (temp != NULL) {
		if (strcmp(temp->amigo->correo, correoAmigo) == 0) {
			// Eliminar de la lista del usuario actual
			if (ant == NULL) {
				usuarioActual->misAmigos = temp->siguiente;
			} else {
				ant->siguiente = temp->siguiente;
			}

			// Eliminar reciprocamente de la lista del otro usuario
			NodoAmigo *t2 = temp->amigo->misAmigos;
			NodoAmigo *a2 = NULL;
			while (t2 != NULL) {
				if (t2->amigo == usuarioActual) {
					if (a2 == NULL) {
						temp->amigo->misAmigos = t2->siguiente;
					} else {
						a2->siguiente = t2->siguiente;
					}
					free(t2);
					break;
				}
				a2 = t2;
				t2 = t2->siguiente;
			}

			printf("Amistad eliminada.\n");
			free(temp);
			return;
		}
		ant = temp;
		temp = temp->siguiente;
	}
	printf("No se encontro ese amigo en tu lista.\n");
}

// ============================================================================
// SUBMENUS DEL MENU PRINCIPAL
// ============================================================================

// Submenu de playlists: mostrar, crear, reproducir, eliminar y volver
void submenuPlaylists(NodoUsuario *usuarioActual) {
	int opcion;
	do {
		printf("\n=== PLAYLISTS ===\n");
		printf("1. Mostrar playlists\n");
		printf("2. Crear playlist\n");
		printf("3. Reproducir playlist\n");
		printf("4. Eliminar playlist\n");
		printf("5. Volver al menu anterior\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion) {
			case 1:
				verPlaylists(usuarioActual);
				break;
			case 2:
				crearPlaylist(usuarioActual);
				break;
			case 3:
				reproducirPlaylist(usuarioActual);
				break;
			case 4:
				usuarioActual->misPlaylists = eliminarPlaylist(usuarioActual);
				break;
			case 5:
				printf("Volviendo al menu anterior..\n");
				break;
			default:
				printf("Opcion no valida\n");
		}
	} while (opcion != 5);
}

// Submenu de canciones: mostrar y buscar por nombre de cancion
void submenuCanciones(NodoUsuario *usuarioActual) {
	int opcion;
	do {
		printf("\n=== CANCIONES ===\n");
		printf("1. Mostrar canciones disponibles\n");
		printf("2. Buscar cancion\n");
		printf("3. Volver al menu anterior\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion) {
			case 1:
				mostrarCanciones();
				break;
			case 2: {
				char busqueda[50];
				printf("Escriba la cancion a buscar\n");
				getchar();
				fgets(busqueda, sizeof(busqueda), stdin);
				busqueda[strcspn(busqueda, "\n")] = '\0';
				NodoCancion *temp = buscarCancion(busqueda);
				if (temp != NULL) {
					printf("Encontrado: %s - %s\n", temp->nombre, temp->artista);
				}
				break;
			}
			case 3:
				printf("Volviendo al menu anterior..\n");
				break;
			default:
				printf("Opcion no valida\n");
		}
	} while (opcion != 3);
}

// Submenu de artistas: mostrar artistas disponibles y buscar canciones por artista
void submenuArtistas() {
	int opcion;
	do {
		printf("\n=== ARTISTAS ===\n");
		printf("1. Mostrar artistas disponibles\n");
		printf("2. Buscar canciones por artista\n");
		printf("3. Volver al menu anterior\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion) {
			case 1:
				mostrarArtistas();
				break;
			case 2:
				buscarPorArtista();
				break;
			case 3:
				printf("Volviendo al menu anterior..\n");
				break;
			default:
				printf("Opcion no valida\n");
		}
	} while (opcion != 3);
}

// Submenu de amigos: ver, agregar, eliminar y volver
void submenuAmigos(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios) {
	int opcion;
	do {
		printf("\n=== AMIGOS ===\n");
		printf("1. Ver mis amigos\n");
		printf("2. Agregar amigo\n");
		printf("3. Eliminar amigo\n");
		printf("4. Volver al menu anterior\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion) {
			case 1:
				verAmigos(usuarioActual);
				break;
			case 2:
				agregarAmigo(usuarioActual, raizUsuarios);
				break;
			case 3:
				eliminarAmigo(usuarioActual);
				break;
			case 4:
				printf("Volviendo al menu anterior..\n");
				break;
			default:
				printf("Opcion no valida\n");
		}
	} while (opcion != 4);
}

// ============================================================================
// FLUJO PRINCIPAL Y CONFIGURACIÓN DE USUARIOS
// ============================================================================

NodoUsuario *IniciarSesion(NodoUsuario *raiz) {
	char correo[50];
	char contrasena[50];
	printf("Ingrese su correo: ");
	scanf("%s", correo);
	printf("Ingrese su contrasena: ");
	scanf("%s", contrasena);

	NodoUsuario *usuarioEncontrado = buscarUsuario(raiz, correo);

	if (usuarioEncontrado != NULL && strcmp(usuarioEncontrado->contrasena, contrasena) == 0) {
		printf("Inicio de sesion exitoso. Bienvenido, %s!\n", usuarioEncontrado->usuario);
		return usuarioEncontrado;
	} else {
		printf("Usuario o contrasena incorrectos.\n");
		return NULL;
	}
}


NodoUsuario *Registrarse(NodoUsuario *raiz) {
	char usuario[50];
	char contrasena[50];
	char paisOrigen[50];
	char tipo[10];
	char respuesta[10];
	char correo[50];
	int i = 0;
	do {
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
	do {
		printf("Ingrese el tipo de cuenta (premium o free): ");
		scanf("%s", tipo);
		i = verificacionPoF(tipo);
	} while (i != 1);
	if (strcmp(tipo, "premium") == 0) {
		getchar();
		printf("La cuenta premium tiene un costo de $9.99 al mes y puedes disfrutar de una experiencia sin anuncios.\n");
		printf("Desea continuar con la cuenta premium? (s/n): ");
		fgets(respuesta,10,stdin);
		respuesta[strcspn(respuesta,"\n")]='\0';
		if (respuesta[0] != 's' && respuesta[0] != 'S') {
			printf("Registro cancelado. Volviendo al menu principal.\n");
			return raiz;
		}
	}

	raiz = insertarUsuario(raiz, paisOrigen, correo, usuario, contrasena, tipo);
	printf("Registro exitoso. Bienvenido, %s!\n", usuario);
	return raiz;
}

void Configuracion(NodoUsuario *usuarioActual) {
	system("cls");
	int opc;
	char ver[10];
	char cambio[50];
	char respuesta[10];
	printf("==Configuracion del Usuario==\n");
	printf("1. Cambiar el nombre de usuario\n");
	printf("2. Cambiar la contrasena\n");
	printf("3. Cambiar el pais\n");
	printf("4. Actualizar el plan\n");
	printf("5. Volver\n");
	scanf("%d", &opc);
	getchar();
	switch(opc) {
		case 1: {
			printf("Escriba su nuevo usuario\n");
			fgets(cambio,50,stdin);
			cambio[strcspn(cambio,"\n")]='\0';
			printf("Seguro desea cambiar su usuario a %s", cambio);
			fgets(ver,10,stdin);
			ver[strcspn(ver,"\n")]='\0';
			if(ver[0]=='s' || ver[0]=='S') {
				strcpy(usuarioActual->usuario,cambio);
			}
			break;
		}
		case 2: {
			char contrasenaAnt[50];
			printf("Escriba su contrasena actual:");
			fgets(contrasenaAnt,50,stdin);
			contrasenaAnt[strcspn(contrasenaAnt,"\n")]='\0';
			if(strcmp(contrasenaAnt,usuarioActual->contrasena)==0) {
				printf("\nEscriba la nueva contrasena:");
				fgets(cambio,50,stdin);
				cambio[strcspn(cambio,"\n")]='\0';
				printf("Seguro desea cambiar su contrasena");
				fgets(ver,10,stdin);
				ver[strcspn(ver,"\n")]='\0';
				if(ver[0]=='s' || ver[0]=='S') {
					strcpy(usuarioActual->contrasena,cambio);
				}
			}
			break;
		}
		case 3: {
			printf("Escriba su actual pais de residencia\n");
			fgets(cambio,50,stdin);
			cambio[strcspn(cambio,"\n")]='\0';
			printf("Seguro desea cambiar su pais a %s", cambio);
			fgets(ver,10,stdin);
			ver[strcspn(ver,"\n")]='\0';
			if(ver[0]=='s' || ver[0]=='S') {
				strcpy(usuarioActual->paisOrigen,cambio);
			}
			break;
		}
		case 4: {
			if(strcmp(usuarioActual->tipo,"premium")==0) {
				printf("Desea cancelar su suscripcion?\n");
				fgets(ver,10,stdin);
				ver[strcspn(ver,"\n")]='\0';
				if(ver[0]=='s' || ver[0]=='S') {
					strcpy(usuarioActual->tipo,"free");
				}
				printf("Suscripcion cancelada\n");
			} else if(strcmp(usuarioActual->tipo,"free")==0) {
				printf("Desea comprar el plan premium?:");
				fgets(ver,10,stdin);
				ver[strcspn(ver,"\n")]='\0';
				if(ver[0]=='s' || ver[0]=='S') {
					printf("La cuenta premium tiene un costo de $9.99 al mes y puedes disfrutar de una experiencia sin anuncios.\n");
					printf("Desea continuar con la cuenta premium? (s/n):");
					fgets(respuesta,10,stdin);
					respuesta[strcspn(respuesta,"\n")]='\0';
					if(respuesta[0]=='s' || respuesta[0]=='S') {
						strcpy(usuarioActual->tipo,"premium");
						printf("Registro cpmletado disfruta tus beneficios premium\n");
					} else {
						printf("Suscripcion cancelada\n");
					}
				}
			}

			break;
		}
		case 5: {

			break;
		}
		default: {
			printf("Opcion no valida\n");
		}

	}
}

// ============================================================================
// MENU PRINCIPAL (REESTRUCTURADO)
// ============================================================================
void menuPrincipal(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios) {
	int opcion;
	do {
		printf("\n===Hola %s===\n", usuarioActual->usuario);
		printf("1. Playlists\n");
		printf("2. Canciones\n");
		printf("3. Artistas\n");
		printf("4. Amigos\n");
		printf("5. Ver estadisticas\n");
		printf("6. Configuracion\n");
		printf("7. Cerrar sesion\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);
		switch (opcion) {
			case 1: {
				submenuPlaylists(usuarioActual);
				break;
			}
			case 2: {
				submenuCanciones(usuarioActual);
				break;
			}
			case 3: {
				submenuArtistas();
				break;
			}
			case 4: {
				submenuAmigos(usuarioActual, raizUsuarios);
				break;
			}
			case 5: {
				printf("Mostrando estadisticas..\n");
				break;
			}
			case 6: {
				getchar();
				Configuracion(usuarioActual);
				break;
			}
			case 7: {
				printf("Cerrando sesion..\n");
				break;
			}
			default: {
				printf("Opcion no valida\n");
			}
		}
	} while (opcion != 7);
}

int main() {
	NodoUsuario *raizUsuarios = NULL;
	NodoUsuario *usuarioActual = NULL;
	int opcion;
	raizUsuarios=cargarArchivoU(raizUsuarios);
	InicioC=archivoCancionesGenerales(InicioC);
	do {
		printf("\n1. Iniciar Sesion\n");
		printf("2. Registrarse\n");
		printf("3. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &opcion);

		switch (opcion) {
			case 1:
				usuarioActual = IniciarSesion(raizUsuarios);
				if (usuarioActual != NULL) {
					menuPrincipal(usuarioActual, raizUsuarios);
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

//Verificaciones
int verificacionCorreo(char correo[]) {

	int j;

	for(j=0; correo[j]!='\0'; j++) {

		if(correo[j]== '@') {
			return 1;
		}

	}
	printf("Correo no valido\n");
	return 0;
}

int verificacionPoF(char tipo[]) {

	if(strcmp(tipo,"premium")==0 || strcmp(tipo,"free")==0) {
		return 1;
	} else {
		printf("Escriba correctamente el tipo de cuenta\n");
		return 0;
	}

}

//Cargar usuarios ya existentes 
NodoUsuario *cargarArchivoU(NodoUsuario *raiz) { //cada usuario que lea se manda a la funcion insercion para crear el abb
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

NodoCancion *insertarCancion (char cancion[],char artista[]) {
	NodoCancion *nueva = malloc(sizeof(NodoCancion));
	strcpy(nueva->nombre,cancion);
	strcpy(nueva->artista,artista);
	nueva->anterior=NULL;
	nueva->siguiente=NULL;

	if(InicioC==NULL) {
		InicioC=nueva;
		FinC=nueva;
	} else {

		nueva->anterior=FinC;
		FinC->siguiente=nueva;
		FinC=nueva;
	}

	return InicioC;
}

NodoCancion *archivoCancionesGenerales(NodoCancion *cancion) {
	FILE *archivoC= fopen("Canciones.txt","r");
	if(archivoC==NULL) {
		printf("Error no se pudieron cargar las canciones disponibles\n");
		return NULL;
	}
	char linea[100];
	char *Titulo,*Artista;

	while(fgets(linea,sizeof(linea),archivoC)) {

		linea[strcspn(linea,"\n")]= '\0';
		Titulo= strtok(linea,";");
		Artista= strtok(NULL,";");
		if(Titulo) {
			InicioC = insertarCancion (Titulo,Artista);
		}
	}
	printf("Canciones cargadas correctamente\n");

	return InicioC;
}
NodoCancion *buscarCancion( char busqueda[]) {
	NodoCancion *temp = InicioC;
	if(InicioC==NULL) {
		printf("No se encuentran canciones cargadas\n");
		return NULL;
	}
	while(temp!=NULL) {
		if(strcmp(temp->nombre, busqueda) == 0 || strcmp(temp->artista, busqueda) == 0) {
			return temp;
		}
		temp=temp->siguiente; //Faltaba avanzar de cancion, sino se queda en bucle


	}
	printf("Cancion o artista no encontrado\n");

	return NULL;
}
void agregarCancionAPlaylist(NodoPlaylist *playlist) {

	char nombreCancion[50];
	printf("Ingrese el nombre de la cancion: ");
	scanf("%s", nombreCancion);

	// Se busca la cancion en la lista de canciones disponibles
	NodoCancion *encontrada = buscarCancion(nombreCancion);

	if(encontrada!=NULL) {
		//Aqui reservamos memoria para el nodo independiente en la playlist
		NodoCancion *nuevaCancion = (NodoCancion *) malloc (sizeof(NodoCancion));
		strcpy(nuevaCancion->nombre, encontrada->nombre);
		strcpy (nuevaCancion->artista, encontrada->artista);
		nuevaCancion->siguiente = NULL;
		nuevaCancion->anterior = NULL;

		// Insertar la canción al inicio de la lista de canciones de esta playlist
		if (playlist->listaCanciones == NULL) {
			playlist->listaCanciones = nuevaCancion;
		} else {
			nuevaCancion->siguiente = playlist->listaCanciones;
			playlist->listaCanciones->anterior = nuevaCancion;
			playlist->listaCanciones = nuevaCancion;
		}
		printf("Cancion '%s' agregada a la playlist.\n", nombreCancion);
	}

}
NodoPlaylist *buscarPlaylist(char nombre[],NodoUsuario *usuarioAct) {

	if(usuarioAct->misPlaylists==NULL) {

		printf("No se encontro la playlist\n");
		return NULL;
	}

	NodoPlaylist *temp=usuarioAct->misPlaylists;

	while(temp!=NULL) {

		if(strcmp(nombre,temp->nombre)==0) {
			return temp;

		}
		temp=temp->siguiente; //Lo mismo, se quedaba en bucle porque no avanzaba

	}
	printf("No se encontro la playlist\n");
	return NULL;
}

NodoUsuario *insertarUsuario(NodoUsuario *raiz, char paisOrigen[], char correo[], char user[], char pass[], char tipo[]) {
	if (raiz == NULL) {
		NodoUsuario *nuevo = (NodoUsuario *)malloc(sizeof(NodoUsuario));
		strcpy(nuevo->paisOrigen, paisOrigen);
		strcpy(nuevo->correo, correo);
		strcpy(nuevo->usuario, user);
		strcpy(nuevo->contrasena, pass);
		strcpy(nuevo->tipo, tipo);
		nuevo->misPlaylists = NULL;
		nuevo->misAmigos = NULL;
		nuevo->izq = NULL;
		nuevo->der = NULL;
		return nuevo;
	}


	//Aqui cambie para que se busque por correo, porque despues igual en buscar
	//Usuario estas buscando con correo xd para que todo sea general
	int comparacion = strcmp(correo, raiz->correo);
	if (comparacion < 0) {
		raiz->izq = insertarUsuario(raiz->izq, paisOrigen, correo, user, pass, tipo);
	} else if (comparacion > 0) {
		raiz->der = insertarUsuario(raiz->der, paisOrigen, correo, user, pass, tipo);
	} else {
		printf("El correo ya esta registrado.\n");
	}
	return raiz;
}


NodoUsuario *buscarUsuario(NodoUsuario *raiz, char correo[]) {
	if (raiz == NULL || strcmp(correo, raiz->correo) == 0) {
		return raiz;
	}

	if (strcmp(correo, raiz->correo) < 0) {
		return buscarUsuario(raiz->izq, correo);
	} else {
		return buscarUsuario(raiz->der, correo);
	}
}
void GuardarDatos(NodoUsuario *raiz){
FILE *archivo=fopen("Usuarios.txt","w");

if(archivo!=NULL){
eliminarABBUsuario(raiz,archivo);
fclose(archivo);
raiz=NULL;
}

}

void eliminarABBUsuario(NodoUsuario *raiz, FILE *archivo){

if(raiz==NULL){

	return;
}

eliminarABBUsuario(raiz->izq,archivo);
eliminarABBUsuario(raiz->der,archivo);

fprintf(archivo,"%s;",raiz->paisOrigen);

fprintf(archivo,"%s;",raiz->correo);

fprintf(archivo,"%s;",raiz->usuario);

fprintf(archivo,"%s;",raiz->contrasena);

fprintf(archivo,"%s\n",raiz->tipo);

free(raiz);

}
