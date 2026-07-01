#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLECCION 1000   

typedef struct Fecha {
    int dia, mes, anio;
} Fecha;

typedef struct NodoCancion {
    char nombre[60];
    char artista[50];          
    int duracionSeg;          
    char archivoOrigen[120]; 
    char tipoArchivo[10];     
    int reproducciones;        
    int enPlaylists;           
    struct NodoCancion *siguiente;
} NodoCancion;

typedef struct NodoDisco {
    char nombre[60];
    Fecha fechaLanzamiento;
    NodoCancion *canciones;    
    struct NodoDisco *siguiente;
} NodoDisco;

typedef struct NodoArtista {
    char nombre[50];
    NodoDisco *discos;          
    struct NodoArtista *izq, *der;
} NodoArtista;

typedef struct NodoPlaylistCancion {
    NodoCancion *cancion;
    struct NodoPlaylistCancion *siguiente;
    struct NodoPlaylistCancion *anterior;
} NodoPlaylistCancion;

typedef struct NodoPlaylist {
    char nombre[50];
    NodoPlaylistCancion *canciones; 
    struct NodoPlaylist *siguiente;
} NodoPlaylist;

typedef struct NodoAmigo {
    struct NodoUsuario *amigo;
    struct NodoAmigo *siguiente;
} NodoAmigo;

typedef struct NodoHistorial {
    NodoCancion *cancion;
    Fecha fecha;
    struct NodoHistorial *siguiente; 
} NodoHistorial;

typedef struct NodoPlanPremium {
    Fecha fechaCompra;
    int validezDias;
    float valor;
    struct NodoPlanPremium *siguiente;
} NodoPlanPremium;

typedef struct NodoUsuario {
    char correo[50];
    char nombre[50];        
    char nickname[50];     
    char paisOrigen[50];
    char contrasena[50];
    char plan[10];           
    int  cancionesDesdeAnuncio;
    int  anunciosEscuchados;   
    int  tiempoTotalSeg;       
    NodoPlaylist     *misPlaylists;
    NodoAmigo        *misAmigos;
    NodoHistorial    *historial;       
    NodoPlanPremium  *historialPlanes; 
    struct NodoUsuario *izq, *der;
} NodoUsuario;

typedef struct NodoAnuncio {
    char contenido[120];   
    char anunciante[50];
    struct NodoAnuncio *siguiente;
} NodoAnuncio;

typedef struct {
    NodoAnuncio *frente; 
    NodoAnuncio *final;  
} ColaAnuncios;

/* VARIABLES GLOBALES*/

NodoArtista  *raizArtistas = NULL; 
NodoUsuario  *raizUsuariosGlobal = NULL;
ColaAnuncios  colaAnuncios = {NULL, NULL};

NodoCancion *coleccionTemp[MAX_COLECCION];
int totalColeccionTemp = 0;

/* Prototipos de funciones a usar */
int  verificacionCorreo(char correo[]);
int  verificacionPoF(char tipo[]);
int  pedirFechaValida(const char *mensaje, Fecha *f);

NodoUsuario *cargarArchivoU(NodoUsuario *raiz);
void cargarCatalogo(void);
void cargarAnuncios(void);
void guardarUsuarios(NodoUsuario *raiz);
void guardarUsuariosRec(NodoUsuario *raiz, FILE *archivo);
void guardarCatalogo(void);
//manejo datos usuario
NodoUsuario *insertarUsuario(NodoUsuario *raiz, char nombreReal[], char paisOrigen[], char correo[], char nick[], char pass[], char tipo[]);
NodoUsuario *buscarUsuario(NodoUsuario *raiz, char correo[]);
NodoUsuario *eliminarUsuario(NodoUsuario *raiz, char correo[]);
void actualizarUsuario(NodoUsuario *u); /* delega en Configuracion() */

//anejo canciones
NodoArtista *buscarArtista(NodoArtista *raiz, char nombre[]);
NodoArtista *insertarArtista(NodoArtista *raiz, char nombre[]);
NodoDisco   *buscarDisco(NodoArtista *artista, char nombreDisco[]);
NodoDisco   *insertarDisco(NodoArtista *artista, char nombreDisco[], Fecha fecha);
NodoCancion *insertarCancion(char nombreArtista[], char nombreDisco[], Fecha fechaDisco, char nombreCancion[], int duracion, char archivo[], char tipo[]);
NodoCancion *buscarCancionPorNombre(NodoArtista *raiz, char nombre[]);
NodoCancion *buscarCancionEnArtista(NodoArtista *art, char nombre[]);
int eliminarCancion(char nombreCancion[]);
void actualizarCancion(NodoCancion *c);
void mostrarArtistas(NodoArtista *raiz);
void mostrarDiscosDeArtista(NodoArtista *art);
void mostrarCancionesDeDisco(NodoDisco *d);
void buscarPorArtista(void);
void crearCancionInteractivo(void);

void encolarAnuncio(char contenido[], char anunciante[]);
NodoAnuncio *desencolarAnuncio(void);
void mostrarAnuncio(NodoAnuncio *a);

// Playlists 
void crearPlaylist(NodoUsuario *usuarioActual);
void agregarCancionAPlaylist(NodoPlaylist *playlist);
void quitarCancionDePlaylist(NodoPlaylist *playlist);
void verPlaylists(NodoUsuario *usuarioActual);
void mostrarPlaylist(NodoUsuario *usuarioActual);
NodoPlaylist *buscarPlaylist(char nombre[], NodoUsuario *usuarioAct);
void eliminarPlaylist(NodoUsuario *usuarioActual);

void reproducirPlaylist(NodoUsuario *usuarioActual);
void registrarReproduccion(NodoUsuario *usuarioActual, NodoCancion *c);
void mostrarHistorial(NodoUsuario *usuarioActual);

//amigos
int  existeAmigo(NodoUsuario *usuarioActual, NodoUsuario *candidato);
void verAmigos(NodoUsuario *usuarioActual);
void agregarAmigo(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios);
void eliminarAmigo(NodoUsuario *usuarioActual);
void recomendarPorAmigos(NodoUsuario *usuarioActual);

void comprarPremium(NodoUsuario *usuarioActual);
void renovarPremium(NodoUsuario *usuarioActual);

// Estadisticas
void recolectarCanciones(NodoArtista *raiz);
void reporteTop5Canciones(void);
void reporteArtistaPreferido(NodoUsuario *u);
void reporteTiempoTotal(NodoUsuario *u);
void reporteAnunciosMostrados(NodoUsuario *u);
void generarTodosLosReportes(NodoUsuario *u);

//Menus 
void submenuPlaylists(NodoUsuario *usuarioActual);
void submenuCatalogo(NodoUsuario *usuarioActual);
void submenuAmigos(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios);
void submenuPremium(NodoUsuario *usuarioActual);
void Configuracion(NodoUsuario *usuarioActual);
void menuPrincipal(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios);
NodoUsuario *IniciarSesion(NodoUsuario *raiz);
NodoUsuario *Registrarse(NodoUsuario *raiz);

int main(void) {
    NodoUsuario *raizUsuarios = NULL;
    NodoUsuario *usuarioActual = NULL;
    int opcion;

    raizUsuarios = cargarArchivoU(raizUsuarios);
    cargarCatalogo();
    cargarAnuncios();

    do {
        printf("\n========== SPOTIFY Chino ==========\n");
        printf("1. Iniciar Sesion\n");
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
                printf("Guardando datos y saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 3);

    guardarUsuarios(raizUsuarios);
    guardarCatalogo();

    return 0;
}

int verificacionCorreo(char correo[]) {
    int j;
    for (j = 0; correo[j] != '\0'; j++) {
        if (correo[j] == '@') {
            return 1;
        }
    }
    printf("Correo no valido (debe contener '@').\n");
    return 0;
}

int verificacionPoF(char tipo[]) {
    if (strcmp(tipo, "premium") == 0 || strcmp(tipo, "free") == 0) {
        return 1;
    }
    printf("Escriba correctamente el tipo de cuenta (premium/free)\n");
    return 0;
}

int pedirFechaValida(const char *mensaje, Fecha *f) {
    int ok = 0;
    do {
        printf("%s (formato: dia mes anio): ", mensaje);
        scanf("%d %d %d", &f->dia, &f->mes, &f->anio);
        if (f->mes < 1 || f->mes > 12 || f->dia < 1 || f->dia > 31 || f->anio < 1900 || f->anio > 2100) {
            printf("Fecha no valida, intente de nuevo.\n");
        } else {
            ok = 1;
        }
    } while (!ok);
    return 1;
}

NodoUsuario *cargarArchivoU(NodoUsuario *raiz) {
    FILE *archivo = fopen("Usuarios.txt", "r");
    if (archivo == NULL) {
        printf("No se encontro Usuarios.txt (se creara uno nuevo al salir)\n");
        return raiz;
    }

    char linea[300];
    char *correo, *nombre, *nick, *pais, *contrasena, *tipo;

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';
        correo     = strtok(linea, ";");
        nombre     = strtok(NULL, ";");
        nick       = strtok(NULL, ";");
        pais       = strtok(NULL, ";");
        contrasena = strtok(NULL, ";");
        tipo       = strtok(NULL, ";");

        if (correo && nombre && nick && pais && contrasena && tipo) {
            raiz = insertarUsuario(raiz, nombre, pais, correo, nick, contrasena, tipo);
        }
    }

    fclose(archivo);
    printf("Usuarios cargados correctamente\n");
    return raiz;
}

/* Carga el catalogo de artistas/discos/canciones desde "Canciones.txt".
  artista;disco;diaDisco;mesDisco;anioDisco;cancion;duracionSeg;archivo;tipo*/
void cargarCatalogo(void) {
    FILE *archivo = fopen("Canciones.txt", "r");
    if (archivo == NULL) {
        printf("No se encontro Canciones.txt (se creara uno nuevo al salir)\n");
        return;
    }

    char linea[400];
    char *artista, *disco, *sDia, *sMes, *sAnio, *cancion, *sDuracion, *archivoOrigen, *tipo;

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';
        artista       = strtok(linea, ";");
        disco         = strtok(NULL, ";");
        sDia          = strtok(NULL, ";");
        sMes          = strtok(NULL, ";");
        sAnio         = strtok(NULL, ";");
        cancion       = strtok(NULL, ";");
        sDuracion     = strtok(NULL, ";");
        archivoOrigen = strtok(NULL, ";");
        tipo          = strtok(NULL, ";");

        if (artista && disco && sDia && sMes && sAnio && cancion && sDuracion && archivoOrigen && tipo) {
            Fecha f;
            f.dia = atoi(sDia);
            f.mes = atoi(sMes);
            f.anio = atoi(sAnio);
            insertarCancion(artista, disco, f, cancion, atoi(sDuracion), archivoOrigen, tipo);
        }
    }
    fclose(archivo);
    printf("Catalogo de artistas/discos/canciones cargado correctamente\n");
}

void cargarAnuncios(void) {
    FILE *archivo = fopen("Anuncios.txt", "r");
    if (archivo == NULL) {
        printf("No se encontro Anuncios.txt (se usaran anuncios por defecto)\n");
        encolarAnuncio("Compra ya el nuevo telefono XPhone", "XPhone Inc.");
        encolarAnuncio("Pide tu comida favorita con DeliApp", "DeliApp");
        return;
    }
    char linea[200];
    char *contenido, *anunciante;
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';
        contenido  = strtok(linea, ";");
        anunciante = strtok(NULL, ";");
        if (contenido && anunciante) {
            encolarAnuncio(contenido, anunciante);
        }
    }
    fclose(archivo);
    printf("Anuncios cargados correctamente\n");
}

void guardarUsuariosRec(NodoUsuario *raiz, FILE *archivo) {
    if (raiz == NULL) return;
    guardarUsuariosRec(raiz->izq, archivo);
    fprintf(archivo, "%s;%s;%s;%s;%s;%s\n",
            raiz->correo, raiz->nombre, raiz->nickname,
            raiz->paisOrigen, raiz->contrasena, raiz->plan);
    guardarUsuariosRec(raiz->der, archivo);
}

void guardarUsuarios(NodoUsuario *raiz) {
    FILE *archivo = fopen("Usuarios.txt", "w");
    if (archivo != NULL) {
        guardarUsuariosRec(raiz, archivo);
        fclose(archivo);
    }
}


void guardarCatalogoRec(NodoArtista *raiz, FILE *archivo) {
    if (raiz == NULL) return;
    guardarCatalogoRec(raiz->izq, archivo);

    NodoDisco *d = raiz->discos;
    while (d != NULL) {
        NodoCancion *c = d->canciones;
        while (c != NULL) {
            fprintf(archivo, "%s;%s;%d;%d;%d;%s;%d;%s;%s\n",
                    raiz->nombre, d->nombre,
                    d->fechaLanzamiento.dia, d->fechaLanzamiento.mes, d->fechaLanzamiento.anio,
                    c->nombre, c->duracionSeg, c->archivoOrigen, c->tipoArchivo);
            c = c->siguiente;
        }
        d = d->siguiente;
    }
    guardarCatalogoRec(raiz->der, archivo);
}

void guardarCatalogo(void) {
    FILE *archivo = fopen("Canciones.txt", "w");
    if (archivo != NULL) {
        guardarCatalogoRec(raizArtistas, archivo);
        fclose(archivo);
    }
}

 // ABB DE USUARIOS (CRUD)
 

NodoUsuario *insertarUsuario(NodoUsuario *raiz, char nombreReal[], char paisOrigen[], char correo[], char nick[], char pass[], char tipo[]) {
    if (raiz == NULL) {
        NodoUsuario *nuevo = (NodoUsuario *)malloc(sizeof(NodoUsuario));
        strcpy(nuevo->correo, correo);
        strcpy(nuevo->nombre, nombreReal);
        strcpy(nuevo->nickname, nick);
        strcpy(nuevo->paisOrigen, paisOrigen);
        strcpy(nuevo->contrasena, pass);
        strcpy(nuevo->plan, tipo);
        nuevo->cancionesDesdeAnuncio = 0;
        nuevo->anunciosEscuchados = 0;
        nuevo->tiempoTotalSeg = 0;
        nuevo->misPlaylists = NULL;
        nuevo->misAmigos = NULL;
        nuevo->historial = NULL;
        nuevo->historialPlanes = NULL;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        return nuevo;
    }

    int comparacion = strcmp(correo, raiz->correo);
    if (comparacion < 0) {
        raiz->izq = insertarUsuario(raiz->izq, nombreReal, paisOrigen, correo, nick, pass, tipo);
    } else if (comparacion > 0) {
        raiz->der = insertarUsuario(raiz->der, nombreReal, paisOrigen, correo, nick, pass, tipo);
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
    }
    return buscarUsuario(raiz->der, correo);
}

NodoUsuario *minimoUsuario(NodoUsuario *raiz) {
    while (raiz->izq != NULL) raiz = raiz->izq;
    return raiz;
}


NodoUsuario *eliminarUsuario(NodoUsuario *raiz, char correo[]) {
    if (raiz == NULL) {
        printf("No se encontro un usuario con ese correo.\n");
        return NULL;
    }
    int comparacion = strcmp(correo, raiz->correo);
    if (comparacion < 0) {
        raiz->izq = eliminarUsuario(raiz->izq, correo);
    } else if (comparacion > 0) {
        raiz->der = eliminarUsuario(raiz->der, correo);
    } else {
        
        if (raiz->izq == NULL) {
            NodoUsuario *temp = raiz->der;
            free(raiz);
            printf("Usuario eliminado correctamente.\n");
            return temp;
        } else if (raiz->der == NULL) {
            NodoUsuario *temp = raiz->izq;
            free(raiz);
            printf("Usuario eliminado correctamente.\n");
            return temp;
        }
        NodoUsuario *sucesor = minimoUsuario(raiz->der);
        strcpy(raiz->correo, sucesor->correo);
        strcpy(raiz->nombre, sucesor->nombre);
        strcpy(raiz->nickname, sucesor->nickname);
        strcpy(raiz->paisOrigen, sucesor->paisOrigen);
        strcpy(raiz->contrasena, sucesor->contrasena);
        strcpy(raiz->plan, sucesor->plan);
        raiz->der = eliminarUsuario(raiz->der, sucesor->correo);
    }
    return raiz;
}


NodoArtista *buscarArtista(NodoArtista *raiz, char nombre[]) {
    if (raiz == NULL || strcmp(nombre, raiz->nombre) == 0) {
        return raiz;
    }
    if (strcmp(nombre, raiz->nombre) < 0) {
        return buscarArtista(raiz->izq, nombre);
    }
    return buscarArtista(raiz->der, nombre);
}


NodoArtista *insertarArtista(NodoArtista *raiz, char nombre[]) {
    if (raiz == NULL) {
        NodoArtista *nuevo = (NodoArtista *)malloc(sizeof(NodoArtista));
        strcpy(nuevo->nombre, nombre);
        nuevo->discos = NULL;
        nuevo->izq = NULL;
        nuevo->der = NULL;
        return nuevo;
    }
    int comparacion = strcmp(nombre, raiz->nombre);
    if (comparacion < 0) {
        raiz->izq = insertarArtista(raiz->izq, nombre);
    } else if (comparacion > 0) {
        raiz->der = insertarArtista(raiz->der, nombre);
    }
    return raiz;
}

NodoDisco *buscarDisco(NodoArtista *artista, char nombreDisco[]) {
    if (artista == NULL) return NULL;
    NodoDisco *d = artista->discos;
    while (d != NULL) {
        if (strcmp(d->nombre, nombreDisco) == 0) return d;
        d = d->siguiente;
    }
    return NULL;
}


NodoDisco *insertarDisco(NodoArtista *artista, char nombreDisco[], Fecha fecha) {
    NodoDisco *nuevo = (NodoDisco *)malloc(sizeof(NodoDisco));
    strcpy(nuevo->nombre, nombreDisco);
    nuevo->fechaLanzamiento = fecha;
    nuevo->canciones = NULL;
    nuevo->siguiente = artista->discos;
    artista->discos = nuevo;
    return nuevo;
}


NodoCancion *insertarCancion(char nombreArtista[], char nombreDisco[], Fecha fechaDisco, char nombreCancion[], int duracion, char archivo[], char tipo[]) {
    NodoArtista *artista = buscarArtista(raizArtistas, nombreArtista);
    if (artista == NULL) {
        raizArtistas = insertarArtista(raizArtistas, nombreArtista);
        artista = buscarArtista(raizArtistas, nombreArtista);
    }

    NodoDisco *disco = buscarDisco(artista, nombreDisco);
    if (disco == NULL) {
        disco = insertarDisco(artista, nombreDisco, fechaDisco);
    }

    NodoCancion *nueva = (NodoCancion *)malloc(sizeof(NodoCancion));
    strcpy(nueva->nombre, nombreCancion);
    strcpy(nueva->artista, nombreArtista);
    nueva->duracionSeg = duracion;
    strcpy(nueva->archivoOrigen, archivo);
    strcpy(nueva->tipoArchivo, tipo);
    nueva->reproducciones = 0;
    nueva->enPlaylists = 0;
    nueva->siguiente = disco->canciones;
    disco->canciones = nueva;

    return nueva;
}


NodoCancion *buscarCancionEnArtista(NodoArtista *art, char nombre[]) {
    if (art == NULL) return NULL;
    NodoDisco *d = art->discos;
    while (d != NULL) {
        NodoCancion *c = d->canciones;
        while (c != NULL) {
            if (strcmp(c->nombre, nombre) == 0) return c;
            c = c->siguiente;
        }
        d = d->siguiente;
    }
    return NULL;
}


NodoCancion *buscarCancionPorNombre(NodoArtista *raiz, char nombre[]) {
    if (raiz == NULL) return NULL;
    NodoCancion *enEsteArtista = buscarCancionEnArtista(raiz, nombre);
    if (enEsteArtista != NULL) return enEsteArtista;
    NodoCancion *izq = buscarCancionPorNombre(raiz->izq, nombre);
    if (izq != NULL) return izq;
    return buscarCancionPorNombre(raiz->der, nombre);
}


void actualizarCancion(NodoCancion *c) {
    if (c == NULL) return;
    printf("Editando '%s' de %s\n", c->nombre, c->artista);
    printf("Nueva duracion en segundos: ");
    scanf("%d", &c->duracionSeg);
    getchar();
    printf("Nuevo archivo de origen (ruta/MP3/URL): ");
    fgets(c->archivoOrigen, sizeof(c->archivoOrigen), stdin);
    c->archivoOrigen[strcspn(c->archivoOrigen, "\n")] = '\0';
    printf("Nuevo tipo (MP3/URL/OTRO): ");
    scanf("%9s", c->tipoArchivo);
    printf("Cancion actualizada.\n");
}


int eliminarCancion(char nombreCancion[]) {
    
    NodoArtista *pilaArtistas[200];
    int tope = -1;
    pilaArtistas[++tope] = raizArtistas;

    while (tope >= 0) {
        NodoArtista *art = pilaArtistas[tope--];
        if (art == NULL) continue;
        if (art->izq) pilaArtistas[++tope] = art->izq;
        if (art->der) pilaArtistas[++tope] = art->der;

        NodoDisco *d = art->discos;
        while (d != NULL) {
            NodoCancion *c = d->canciones;
            NodoCancion *ant = NULL;
            while (c != NULL) {
                if (strcmp(c->nombre, nombreCancion) == 0) {
                    if (c->enPlaylists > 0) {
                        printf("No se puede eliminar: la cancion esta en %d playlist(s).\n", c->enPlaylists);
                        return 0;
                    }
                    if (ant == NULL) {
                        d->canciones = c->siguiente;
                    } else {
                        ant->siguiente = c->siguiente;
                    }
                    free(c);
                    printf("Cancion eliminada correctamente.\n");
                    return 1;
                }
                ant = c;
                c = c->siguiente;
            }
            d = d->siguiente;
        }
    }
    printf("No se encontro la cancion.\n");
    return 0;
}

void crearCancionInteractivo(void) {
    char artista[50], disco[60], cancion[60], archivo[120], tipo[10];
    int duracion;
    Fecha fechaDisco;

    getchar();
    printf("Nombre del artista: ");
    fgets(artista, sizeof(artista), stdin); artista[strcspn(artista, "\n")] = '\0';
    printf("Nombre del disco: ");
    fgets(disco, sizeof(disco), stdin); disco[strcspn(disco, "\n")] = '\0';
    pedirFechaValida("Fecha de lanzamiento del disco", &fechaDisco);
    printf("Nombre de la cancion: ");
    fgets(cancion, sizeof(cancion), stdin); cancion[strcspn(cancion, "\n")] = '\0';
    printf("Duracion en segundos: ");
    scanf("%d", &duracion);
    getchar();
    printf("Archivo de origen (ruta MP3 / URL): ");
    fgets(archivo, sizeof(archivo), stdin); archivo[strcspn(archivo, "\n")] = '\0';
    printf("Tipo de archivo (MP3/URL/OTRO): ");
    scanf("%9s", tipo);

    insertarCancion(artista, disco, fechaDisco, cancion, duracion, archivo, tipo);
    printf("Cancion agregada al catalogo.\n");
}


void mostrarArtistas(NodoArtista *raiz) {
    if (raiz == NULL) return;
    mostrarArtistas(raiz->izq);
    printf("- %s\n", raiz->nombre);
    mostrarArtistas(raiz->der);
}


void mostrarDiscosDeArtista(NodoArtista *art) {
    if (art == NULL) {
        printf("Artista no encontrado.\n");
        return;
    }
    NodoDisco *d = art->discos;
    if (d == NULL) {
        printf("Este artista no tiene discos.\n");
        return;
    }
    printf("\n=== Discos de %s ===\n", art->nombre);
    while (d != NULL) {
        printf("- %s (%02d/%02d/%d)\n", d->nombre, d->fechaLanzamiento.dia, d->fechaLanzamiento.mes, d->fechaLanzamiento.anio);
        d = d->siguiente;
    }
}


void mostrarCancionesDeDisco(NodoDisco *d) {
    if (d == NULL) {
        printf("Disco no encontrado.\n");
        return;
    }
    NodoCancion *c = d->canciones;
    printf("\n=== Canciones de %s ===\n", d->nombre);
    while (c != NULL) {
        printf("- %s (%d seg) [%s]\n", c->nombre, c->duracionSeg, c->tipoArchivo);
        c = c->siguiente;
    }
}

void buscarPorArtista(void) {
    char nombre[50];
    getchar();
    printf("Escriba el nombre del artista a buscar: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    NodoArtista *art = buscarArtista(raizArtistas, nombre);
    if (art == NULL) {
        printf("No se encontro ese artista.\n");
        return;
    }
    NodoDisco *d = art->discos;
    int encontrado = 0;
    printf("\n=== Canciones de %s ===\n", nombre);
    while (d != NULL) {
        NodoCancion *c = d->canciones;
        while (c != NULL) {
            printf("- %s (disco: %s)\n", c->nombre, d->nombre);
            encontrado = 1;
            c = c->siguiente;
        }
        d = d->siguiente;
    }
    if (!encontrado) printf("Este artista no tiene canciones registradas.\n");
}


void encolarAnuncio(char contenido[], char anunciante[]) {
    NodoAnuncio *nuevo = (NodoAnuncio *)malloc(sizeof(NodoAnuncio));
    strcpy(nuevo->contenido, contenido);
    strcpy(nuevo->anunciante, anunciante);
    nuevo->siguiente = NULL;

    if (colaAnuncios.final == NULL) {
        colaAnuncios.frente = nuevo;
        colaAnuncios.final = nuevo;
    } else {
        colaAnuncios.final->siguiente = nuevo;
        colaAnuncios.final = nuevo;
    }
}

NodoAnuncio *desencolarAnuncio(void) {
    if (colaAnuncios.frente == NULL) return NULL;
    NodoAnuncio *anuncio = colaAnuncios.frente;
    colaAnuncios.frente = colaAnuncios.frente->siguiente;
    if (colaAnuncios.frente == NULL) colaAnuncios.final = NULL;
    anuncio->siguiente = NULL;
    return anuncio;
}


void mostrarAnuncio(NodoAnuncio *a) {
    if (a == NULL) return;
    printf("\n>>> ANUNCIO de %s: %s <<<\n", a->anunciante, a->contenido);
}

void crearPlaylist(NodoUsuario *usuarioActual) {
    if (usuarioActual == NULL) return;

    char nombrePlaylist[50];
    getchar();
    printf("Ingrese el nombre de la playlist: ");
    fgets(nombrePlaylist, sizeof(nombrePlaylist), stdin);
    nombrePlaylist[strcspn(nombrePlaylist, "\n")] = '\0';

    NodoPlaylist *nuevaPlaylist = (NodoPlaylist *)malloc(sizeof(NodoPlaylist));
    strcpy(nuevaPlaylist->nombre, nombrePlaylist);
    nuevaPlaylist->canciones = NULL;
    nuevaPlaylist->siguiente = usuarioActual->misPlaylists;
    usuarioActual->misPlaylists = nuevaPlaylist;

    printf("Playlist '%s' creada exitosamente.\n", nombrePlaylist);

    char respuesta;
    printf("Desea agregar canciones a la playlist? (s/n): ");
    scanf(" %c", &respuesta);
    while (respuesta == 's' || respuesta == 'S') {
        agregarCancionAPlaylist(nuevaPlaylist);
        printf("Desea agregar otra cancion? (s/n): ");
        scanf(" %c", &respuesta);
    }
}


void agregarCancionAPlaylist(NodoPlaylist *playlist) {
    char nombreCancion[60];
    getchar();
    printf("Ingrese el nombre de la cancion: ");
    fgets(nombreCancion, sizeof(nombreCancion), stdin);
    nombreCancion[strcspn(nombreCancion, "\n")] = '\0';

    NodoCancion *encontrada = buscarCancionPorNombre(raizArtistas, nombreCancion);
    if (encontrada == NULL) {
        printf("No se encontro esa cancion en el catalogo.\n");
        return;
    }

    NodoPlaylistCancion *nuevoNodo = (NodoPlaylistCancion *)malloc(sizeof(NodoPlaylistCancion));
    nuevoNodo->cancion = encontrada;
    nuevoNodo->siguiente = playlist->canciones;
    nuevoNodo->anterior = NULL;
    if (playlist->canciones != NULL) playlist->canciones->anterior = nuevoNodo;
    playlist->canciones = nuevoNodo;

    encontrada->enPlaylists++;
    printf("Cancion '%s' agregada a la playlist.\n", nombreCancion);
}


void quitarCancionDePlaylist(NodoPlaylist *playlist) {
    if (playlist == NULL || playlist->canciones == NULL) {
        printf("La playlist no tiene canciones.\n");
        return;
    }
    char nombreCancion[60];
    getchar();
    printf("Ingrese el nombre de la cancion a quitar: ");
    fgets(nombreCancion, sizeof(nombreCancion), stdin);
    nombreCancion[strcspn(nombreCancion, "\n")] = '\0';

    NodoPlaylistCancion *temp = playlist->canciones;
    while (temp != NULL) {
        if (strcmp(temp->cancion->nombre, nombreCancion) == 0) {
            if (temp->anterior != NULL) temp->anterior->siguiente = temp->siguiente;
            else playlist->canciones = temp->siguiente;
            if (temp->siguiente != NULL) temp->siguiente->anterior = temp->anterior;

            temp->cancion->enPlaylists--;
            free(temp);
            printf("Cancion quitada de la playlist.\n");
            return;
        }
        temp = temp->siguiente;
    }
    printf("Esa cancion no esta en la playlist.\n");
}

void verPlaylists(NodoUsuario *usuarioActual) {
    if (usuarioActual == NULL || usuarioActual->misPlaylists == NULL) {
        printf("No tienes playlists creadas aun.\n");
        return;
    }
    NodoPlaylist *actualPL = usuarioActual->misPlaylists;
    printf("\n=== TUS PLAYLISTS ===\n");
    while (actualPL != NULL) {
        printf("- Playlist: %s\n", actualPL->nombre);
        NodoPlaylistCancion *actualCancion = actualPL->canciones;
        if (actualCancion == NULL) {
            printf("  [Esta playlist no tiene canciones]\n");
        } else {
            while (actualCancion != NULL) {
                printf("    > %s - %s\n", actualCancion->cancion->nombre, actualCancion->cancion->artista);
                actualCancion = actualCancion->siguiente;
            }
        }
        actualPL = actualPL->siguiente;
    }
}

void mostrarPlaylist(NodoUsuario *usuarioActual) {
    NodoPlaylist *temp = usuarioActual->misPlaylists;
    while (temp != NULL) {
        printf("Playlist: %s\n", temp->nombre);
        temp = temp->siguiente;
    }
}

NodoPlaylist *buscarPlaylist(char nombre[], NodoUsuario *usuarioAct) {
    if (usuarioAct->misPlaylists == NULL) {
        printf("No se encontro la playlist\n");
        return NULL;
    }
    NodoPlaylist *temp = usuarioAct->misPlaylists;
    while (temp != NULL) {
        if (strcmp(nombre, temp->nombre) == 0) return temp;
        temp = temp->siguiente;
    }
    printf("No se encontro la playlist\n");
    return NULL;
}

void eliminarPlaylist(NodoUsuario *usuarioActual) {
    if (usuarioActual->misPlaylists == NULL) {
        printf("No se encuentran playlists\n");
        return;
    }
    mostrarPlaylist(usuarioActual);
    char nombreBorrar[50];
    getchar();
    printf("Escriba el nombre de la playlist a borrar: ");
    fgets(nombreBorrar, sizeof(nombreBorrar), stdin);
    nombreBorrar[strcspn(nombreBorrar, "\n")] = '\0';

    NodoPlaylist *temp = usuarioActual->misPlaylists;
    NodoPlaylist *ant = NULL;
    while (temp != NULL) {
        if (strcmp(nombreBorrar, temp->nombre) == 0) {
            NodoPlaylistCancion *bc = temp->canciones;
            while (bc != NULL) {
                NodoPlaylistCancion *actual = bc;
                bc = bc->siguiente;
                actual->cancion->enPlaylists--;
                free(actual);
            }
            if (ant == NULL) usuarioActual->misPlaylists = temp->siguiente;
            else ant->siguiente = temp->siguiente;
            free(temp);
            printf("Playlist eliminada correctamente\n");
            return;
        }
        ant = temp;
        temp = temp->siguiente;
    }
    printf("No se encontro la playlist\n");
}

void registrarReproduccion(NodoUsuario *usuarioActual, NodoCancion *c) {
    Fecha f;
    pedirFechaValida("Fecha de esta reproduccion", &f);

    NodoHistorial *nuevo = (NodoHistorial *)malloc(sizeof(NodoHistorial));
    nuevo->cancion = c;
    nuevo->fecha = f;
    nuevo->siguiente = usuarioActual->historial; 
    usuarioActual->historial = nuevo;

    c->reproducciones++;
    usuarioActual->tiempoTotalSeg += c->duracionSeg;
}

void reproducirPlaylist(NodoUsuario *usuarioActual) {
    if (usuarioActual == NULL || usuarioActual->misPlaylists == NULL) {
        printf("No existen playlists creadas\n");
        return;
    }

    char nombreP[50];
    getchar();
    printf("Ingrese el nombre de la playlist: ");
    fgets(nombreP, sizeof(nombreP), stdin);
    nombreP[strcspn(nombreP, "\n")] = '\0';

    NodoPlaylist *playlist = buscarPlaylist(nombreP, usuarioActual);
    if (playlist == NULL) return;
    if (playlist->canciones == NULL) {
        printf("Esta playlist no contiene canciones para reproducir\n");
        return;
    }

    NodoPlaylistCancion *cancionActual = playlist->canciones;
    int opcion = 0;

    do {
        printf("\n=== Reproduciendo: %s - %s ===\n", cancionActual->cancion->nombre, cancionActual->cancion->artista);
        registrarReproduccion(usuarioActual, cancionActual->cancion);

        if (strcmp(usuarioActual->plan, "free") == 0) {
            usuarioActual->cancionesDesdeAnuncio++;
            if (usuarioActual->cancionesDesdeAnuncio >= 3) {
                NodoAnuncio *anuncio = desencolarAnuncio();
                if (anuncio != NULL) {
                    mostrarAnuncio(anuncio);
                    usuarioActual->anunciosEscuchados++;
                    encolarAnuncio(anuncio->contenido, anuncio->anunciante);
                    free(anuncio);
                }
                usuarioActual->cancionesDesdeAnuncio = 0;
            }
        }

        printf("1. Siguiente\n2. Anterior\n3. Detener reproduccion\nSeleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                if (cancionActual->siguiente != NULL) cancionActual = cancionActual->siguiente;
                else printf("No hay mas canciones siguientes.\n");
                break;
            case 2:
                if (cancionActual->anterior != NULL) cancionActual = cancionActual->anterior;
                else printf("No hay canciones anteriores.\n");
                break;
            case 3:
                printf("Reproduccion detenida.\n");
                break;
            default:
                printf("Opcion no valida\n");
        }
    } while (opcion != 3);
}


void mostrarHistorial(NodoUsuario *usuarioActual) {
    if (usuarioActual->historial == NULL) {
        printf("Aun no tienes historial de reproduccion.\n");
        return;
    }
    printf("\n=== HISTORIAL ===\n");
    NodoHistorial *temp = usuarioActual->historial;
    while (temp != NULL) {
        printf("- %s (%02d/%02d/%d)\n", temp->cancion->nombre, temp->fecha.dia, temp->fecha.mes, temp->fecha.anio);
        temp = temp->siguiente;
    }
}
//amigos
int existeAmigo(NodoUsuario *usuarioActual, NodoUsuario *candidato) {
    NodoAmigo *temp = usuarioActual->misAmigos;
    while (temp != NULL) {
        if (temp->amigo == candidato) return 1;
        temp = temp->siguiente;
    }
    return 0;
}

void verAmigos(NodoUsuario *usuarioActual) {
    if (usuarioActual->misAmigos == NULL) {
        printf("No tienes amigos agregados aun.\n");
        return;
    }
    printf("\n=== TUS AMIGOS ===\n");
    NodoAmigo *temp = usuarioActual->misAmigos;
    while (temp != NULL) {
        printf("- %s (%s)\n", temp->amigo->nickname, temp->amigo->correo);
        temp = temp->siguiente;
    }
}


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
        printf("Ya tienes a %s en tu lista de amigos.\n", encontrado->nickname);
        return;
    }

    NodoAmigo *nuevo = (NodoAmigo *)malloc(sizeof(NodoAmigo));
    nuevo->amigo = encontrado;
    nuevo->siguiente = usuarioActual->misAmigos;
    usuarioActual->misAmigos = nuevo;

    NodoAmigo *nuevoReciproco = (NodoAmigo *)malloc(sizeof(NodoAmigo));
    nuevoReciproco->amigo = usuarioActual;
    nuevoReciproco->siguiente = encontrado->misAmigos;
    encontrado->misAmigos = nuevoReciproco;

    printf("%s ahora es tu amigo.\n", encontrado->nickname);
}

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
            if (ant == NULL) usuarioActual->misAmigos = temp->siguiente;
            else ant->siguiente = temp->siguiente;

            NodoAmigo *t2 = temp->amigo->misAmigos;
            NodoAmigo *a2 = NULL;
            while (t2 != NULL) {
                if (t2->amigo == usuarioActual) {
                    if (a2 == NULL) temp->amigo->misAmigos = t2->siguiente;
                    else a2->siguiente = t2->siguiente;
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


void recomendarPorAmigos(NodoUsuario *usuarioActual) {
    if (usuarioActual->misAmigos == NULL) {
        printf("Agrega amigos para poder recibir recomendaciones.\n");
        return;
    }
    char yaRecomendadas[100][60];
    int total = 0;

    printf("\n=== RECOMENDACIONES BASADAS EN TUS AMIGOS ===\n");
    NodoAmigo *amigo = usuarioActual->misAmigos;
    while (amigo != NULL) {
        NodoHistorial *h = amigo->amigo->historial;
        while (h != NULL) {
            int repetida = 0;
            for (int i = 0; i < total; i++) {
                if (strcmp(yaRecomendadas[i], h->cancion->nombre) == 0) { repetida = 1; break; }
            }
            if (!repetida && total < 100) {
                printf("- %s (%s), escuchada por tu amigo %s\n", h->cancion->nombre, h->cancion->artista, amigo->amigo->nickname);
                strcpy(yaRecomendadas[total], h->cancion->nombre);
                total++;
            }
            h = h->siguiente;
        }
        amigo = amigo->siguiente;
    }
    if (total == 0) printf("Tus amigos aun no tienen historial de reproduccion.\n");
}

void comprarPremium(NodoUsuario *usuarioActual) {
    if (strcmp(usuarioActual->plan, "premium") == 0) {
        printf("Ya cuentas con el plan premium.\n");
        return;
    }
    NodoPlanPremium *nuevo = (NodoPlanPremium *)malloc(sizeof(NodoPlanPremium));
    pedirFechaValida("Fecha de compra", &nuevo->fechaCompra);
    printf("Validez del plan (en dias): ");
    scanf("%d", &nuevo->validezDias);
    printf("Valor pagado ($): ");
    scanf("%f", &nuevo->valor);

    nuevo->siguiente = usuarioActual->historialPlanes;
    usuarioActual->historialPlanes = nuevo;
    strcpy(usuarioActual->plan, "premium");

    printf("Felicidades, ahora tienes el plan premium sin anuncios.\n");
}


void renovarPremium(NodoUsuario *usuarioActual) {
    if (strcmp(usuarioActual->plan, "premium") != 0) {
        printf("Debes tener el plan premium activo para renovarlo.\n");
        return;
    }
    NodoPlanPremium *nuevo = (NodoPlanPremium *)malloc(sizeof(NodoPlanPremium));
    pedirFechaValida("Fecha de renovacion", &nuevo->fechaCompra);
    printf("Validez de la renovacion (en dias): ");
    scanf("%d", &nuevo->validezDias);
    printf("Valor pagado ($): ");
    scanf("%f", &nuevo->valor);

    nuevo->siguiente = usuarioActual->historialPlanes;
    usuarioActual->historialPlanes = nuevo;

    printf("Plan premium renovado exitosamente.\n");
}

void recolectarCanciones(NodoArtista *raiz) {
    if (raiz == NULL) return;
    recolectarCanciones(raiz->izq);
    NodoDisco *d = raiz->discos;
    while (d != NULL) {
        NodoCancion *c = d->canciones;
        while (c != NULL) {
            if (totalColeccionTemp < MAX_COLECCION) {
                coleccionTemp[totalColeccionTemp++] = c;
            }
            c = c->siguiente;
        }
        d = d->siguiente;
    }
    recolectarCanciones(raiz->der);
}

void reporteTop5Canciones(void) {
    totalColeccionTemp = 0;
    recolectarCanciones(raizArtistas);

    for (int i = 0; i < totalColeccionTemp - 1; i++) {
        for (int j = 0; j < totalColeccionTemp - 1 - i; j++) {
            if (coleccionTemp[j]->reproducciones < coleccionTemp[j + 1]->reproducciones) {
                NodoCancion *aux = coleccionTemp[j];
                coleccionTemp[j] = coleccionTemp[j + 1];
                coleccionTemp[j + 1] = aux;
            }
        }
    }

    FILE *f = fopen("Reporte_Top5.txt", "w");
    if (f == NULL) { printf("No se pudo crear el reporte.\n"); return; }
    fprintf(f, "===== TOP 5 CANCIONES MAS ESCUCHADAS =====\n");
    int limite = totalColeccionTemp < 5 ? totalColeccionTemp : 5;
    for (int i = 0; i < limite; i++) {
        fprintf(f, "%d. %s - %s (%d reproducciones)\n", i + 1,
                coleccionTemp[i]->nombre, coleccionTemp[i]->artista, coleccionTemp[i]->reproducciones);
    }
    fclose(f);
    printf("Reporte 'Reporte_Top5.txt' generado.\n");
}

void reporteArtistaPreferido(NodoUsuario *u) {
    char nombreArchivo[100];
    sprintf(nombreArchivo, "Reporte_ArtistaPreferido_%s.txt", u->nickname);

    char artistas[100][50];
    int conteos[100];
    int total = 0;

    NodoHistorial *h = u->historial;
    while (h != NULL) {
        int idx = -1;
        for (int i = 0; i < total; i++) {
            if (strcmp(artistas[i], h->cancion->artista) == 0) { idx = i; break; }
        }
        if (idx == -1 && total < 100) {
            strcpy(artistas[total], h->cancion->artista);
            conteos[total] = 1;
            total++;
        } else if (idx != -1) {
            conteos[idx]++;
        }
        h = h->siguiente;
    }

    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) { printf("No se pudo crear el reporte.\n"); return; }
    fprintf(f, "===== ARTISTAS PREFERIDOS DE %s =====\n", u->nickname);
    if (total == 0) {
        fprintf(f, "Aun no hay historial de reproduccion.\n");
    } else {
        int mejor = 0;
        for (int i = 1; i < total; i++) if (conteos[i] > conteos[mejor]) mejor = i;
        fprintf(f, "Artista preferido: %s (%d reproducciones)\n\n", artistas[mejor], conteos[mejor]);
        fprintf(f, "Detalle de reproducciones por artista:\n");
        for (int i = 0; i < total; i++) fprintf(f, "- %s: %d\n", artistas[i], conteos[i]);
    }
    fclose(f);
    printf("Reporte '%s' generado.\n", nombreArchivo);
}

void reporteTiempoTotal(NodoUsuario *u) {
    char nombreArchivo[100];
    sprintf(nombreArchivo, "Reporte_TiempoTotal_%s.txt", u->nickname);

    int horas = u->tiempoTotalSeg / 3600;
    int minutos = (u->tiempoTotalSeg % 3600) / 60;
    int segundos = u->tiempoTotalSeg % 60;

    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) { printf("No se pudo crear el reporte.\n"); return; }
    fprintf(f, "===== TIEMPO TOTAL DE REPRODUCCION DE %s =====\n", u->nickname);
    fprintf(f, "Total: %d segundos (%02d:%02d:%02d)\n", u->tiempoTotalSeg, horas, minutos, segundos);
    fclose(f);
    printf("Reporte '%s' generado.\n", nombreArchivo);
}

void reporteAnunciosMostrados(NodoUsuario *u) {
    char nombreArchivo[100];
    sprintf(nombreArchivo, "Reporte_Anuncios_%s.txt", u->nickname);

    FILE *f = fopen(nombreArchivo, "w");
    if (f == NULL) { printf("No se pudo crear el reporte.\n"); return; }
    fprintf(f, "===== ANUNCIOS MOSTRADOS A %s =====\n", u->nickname);
    if (strcmp(u->plan, "premium") == 0) {
        fprintf(f, "El usuario tiene plan premium: no escucha anuncios.\n");
    } else {
        fprintf(f, "Anuncios escuchados: %d\n", u->anunciosEscuchados);
    }
    fclose(f);
    printf("Reporte '%s' generado.\n", nombreArchivo);
}

void generarTodosLosReportes(NodoUsuario *u) {
    reporteTop5Canciones();
    reporteArtistaPreferido(u);
    reporteTiempoTotal(u);
    reporteAnunciosMostrados(u);
    printf("Todos los reportes fueron generados en archivos .txt\n");
}

//mas menus
void submenuPlaylists(NodoUsuario *usuarioActual) {
    int opcion;
    do {
        printf("\n=== PLAYLISTS ===\n");
        printf("1. Mostrar playlists\n");
        printf("2. Crear playlist\n");
        printf("3. Reproducir playlist\n");
        printf("4. Quitar cancion de una playlist\n");
        printf("5. Eliminar playlist\n");
        printf("6. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: verPlaylists(usuarioActual); break;
            case 2: crearPlaylist(usuarioActual); break;
            case 3: reproducirPlaylist(usuarioActual); break;
            case 4: {
                char nombreP[50];
                getchar();
                printf("Nombre de la playlist: ");
                fgets(nombreP, sizeof(nombreP), stdin);
                nombreP[strcspn(nombreP, "\n")] = '\0';
                NodoPlaylist *pl = buscarPlaylist(nombreP, usuarioActual);
                if (pl != NULL) quitarCancionDePlaylist(pl);
                break;
            }
            case 5: eliminarPlaylist(usuarioActual); break;
            case 6: printf("Volviendo al menu anterior..\n"); break;
            default: printf("Opcion no valida\n");
        }
    } while (opcion != 6);
}


void submenuCatalogo(NodoUsuario *usuarioActual) {
    (void)usuarioActual; 
    int opcion;
    do {
        printf("\n=== CATALOGO (ARTISTAS / DISCOS / CANCIONES) ===\n");
        printf("1. Mostrar artistas\n");
        printf("2. Ver discos de un artista\n");
        printf("3. Ver canciones de un disco\n");
        printf("4. Buscar canciones por artista\n");
        printf("5. Buscar cancion por nombre\n");
        printf("6. Agregar cancion (crear artista/disco si hace falta)\n");
        printf("7. Editar cancion\n");
        printf("8. Eliminar cancion\n");
        printf("9. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: mostrarArtistas(raizArtistas); break;
            case 2: {
                char nombre[50];
                getchar();
                printf("Nombre del artista: ");
                fgets(nombre, sizeof(nombre), stdin); nombre[strcspn(nombre, "\n")] = '\0';
                mostrarDiscosDeArtista(buscarArtista(raizArtistas, nombre));
                break;
            }
            case 3: {
                char nombreArt[50], nombreDisco[60];
                getchar();
                printf("Nombre del artista: ");
                fgets(nombreArt, sizeof(nombreArt), stdin); nombreArt[strcspn(nombreArt, "\n")] = '\0';
                printf("Nombre del disco: ");
                fgets(nombreDisco, sizeof(nombreDisco), stdin); nombreDisco[strcspn(nombreDisco, "\n")] = '\0';
                NodoArtista *art = buscarArtista(raizArtistas, nombreArt);
                mostrarCancionesDeDisco(buscarDisco(art, nombreDisco));
                break;
            }
            case 4: buscarPorArtista(); break;
            case 5: {
                char nombre[60];
                getchar();
                printf("Nombre de la cancion: ");
                fgets(nombre, sizeof(nombre), stdin); nombre[strcspn(nombre, "\n")] = '\0';
                NodoCancion *c = buscarCancionPorNombre(raizArtistas, nombre);
                if (c != NULL) printf("Encontrada: %s - %s (%d seg)\n", c->nombre, c->artista, c->duracionSeg);
                else printf("No se encontro esa cancion.\n");
                break;
            }
            case 6: crearCancionInteractivo(); break;
            case 7: {
                char nombre[60];
                getchar();
                printf("Nombre de la cancion a editar: ");
                fgets(nombre, sizeof(nombre), stdin); nombre[strcspn(nombre, "\n")] = '\0';
                NodoCancion *c = buscarCancionPorNombre(raizArtistas, nombre);
                actualizarCancion(c);
                break;
            }
            case 8: {
                char nombre[60];
                getchar();
                printf("Nombre de la cancion a eliminar: ");
                fgets(nombre, sizeof(nombre), stdin); nombre[strcspn(nombre, "\n")] = '\0';
                eliminarCancion(nombre);
                break;
            }
            case 9: printf("Volviendo al menu anterior..\n"); break;
            default: printf("Opcion no valida\n");
        }
    } while (opcion != 9);
}

// Submenu de amigos: ver, agregar, eliminar y recibir recomendaciones
void submenuAmigos(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios) {
    int opcion;
    do {
        printf("\n=== AMIGOS ===\n");
        printf("1. Ver mis amigos\n");
        printf("2. Agregar amigo\n");
        printf("3. Eliminar amigo\n");
        printf("4. Recomendaciones segun mis amigos\n");
        printf("5. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: verAmigos(usuarioActual); break;
            case 2: agregarAmigo(usuarioActual, raizUsuarios); break;
            case 3: eliminarAmigo(usuarioActual); break;
            case 4: recomendarPorAmigos(usuarioActual); break;
            case 5: printf("Volviendo al menu anterior..\n"); break;
            default: printf("Opcion no valida\n");
        }
    } while (opcion != 5);
}

void submenuPremium(NodoUsuario *usuarioActual) {
    int opcion;
    do {
        printf("\n=== PLAN PREMIUM ===\n");
        printf("Tu plan actual: %s\n", usuarioActual->plan);
        printf("1. Comprar plan premium\n");
        printf("2. Renovar plan premium\n");
        printf("3. Volver al menu anterior\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: comprarPremium(usuarioActual); break;
            case 2: renovarPremium(usuarioActual); break;
            case 3: printf("Volviendo al menu anterior..\n"); break;
            default: printf("Opcion no valida\n");
        }
    } while (opcion != 3);
}

// Permite editar los datos del perfil
void Configuracion(NodoUsuario *usuarioActual) {
    int opc;
    char ver[10], cambio[50];
    printf("\n==Configuracion del Usuario==\n");
    printf("1. Cambiar el nombre de usuario (nickname)\n");
    printf("2. Cambiar la contrasena\n");
    printf("3. Cambiar el pais\n");
    printf("4. Volver\n");
    scanf("%d", &opc);
    getchar();
    switch (opc) {
        case 1:
            printf("Escriba su nuevo nickname: ");
            fgets(cambio, sizeof(cambio), stdin); cambio[strcspn(cambio, "\n")] = '\0';
            printf("Seguro desea cambiar su nickname a %s? (s/n): ", cambio);
            fgets(ver, sizeof(ver), stdin); ver[strcspn(ver, "\n")] = '\0';
            if (ver[0] == 's' || ver[0] == 'S') strcpy(usuarioActual->nickname, cambio);
            break;
        case 2: {
            char contrasenaAnt[50];
            printf("Escriba su contrasena actual: ");
            fgets(contrasenaAnt, sizeof(contrasenaAnt), stdin); contrasenaAnt[strcspn(contrasenaAnt, "\n")] = '\0';
            if (strcmp(contrasenaAnt, usuarioActual->contrasena) == 0) {
                printf("Escriba la nueva contrasena: ");
                fgets(cambio, sizeof(cambio), stdin); cambio[strcspn(cambio, "\n")] = '\0';
                strcpy(usuarioActual->contrasena, cambio);
                printf("Contrasena actualizada.\n");
            } else {
                printf("Contrasena actual incorrecta.\n");
            }
            break;
        }
        case 3:
            printf("Escriba su nuevo pais de residencia: ");
            fgets(cambio, sizeof(cambio), stdin); cambio[strcspn(cambio, "\n")] = '\0';
            strcpy(usuarioActual->paisOrigen, cambio);
            printf("Pais actualizado.\n");
            break;
        case 4:
            printf("Volviendo al menu anterior..\n");
            break;
        default:
            printf("Opcion no valida\n");
    }
}

//menu principal
void menuPrincipal(NodoUsuario *usuarioActual, NodoUsuario *raizUsuarios) {
    raizUsuariosGlobal = raizUsuarios; 
    int opcion;
    do {
        printf("\n===Hola %s (%s)===\n", usuarioActual->nickname, usuarioActual->plan);
        printf("1. Playlists\n");
        printf("2. Catalogo (artistas/discos/canciones)\n");
        printf("3. Amigos\n");
        printf("4. Historial de reproduccion\n");
        printf("5. Plan Premium\n");
        printf("6. Ver y generar estadisticas (.txt)\n");
        printf("7. Configuracion de mi cuenta\n");
        printf("8. Cerrar sesion\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: submenuPlaylists(usuarioActual); break;
            case 2: submenuCatalogo(usuarioActual); break;
            case 3: submenuAmigos(usuarioActual, raizUsuarios); break;
            case 4: mostrarHistorial(usuarioActual); break;
            case 5: submenuPremium(usuarioActual); break;
            case 6: generarTodosLosReportes(usuarioActual); break;
            case 7: Configuracion(usuarioActual); break;
            case 8: printf("Cerrando sesion..\n"); break;
            default: printf("Opcion no valida\n");
        }
    } while (opcion != 8);
    guardarUsuarios(raizUsuarios);
    guardarCatalogo();
}

//registro
NodoUsuario *IniciarSesion(NodoUsuario *raiz) {
    char correo[50], contrasena[50];
    printf("Ingrese su correo: ");
    scanf("%s", correo);
    printf("Ingrese su contrasena: ");
    scanf("%s", contrasena);

    NodoUsuario *usuarioEncontrado = buscarUsuario(raiz, correo);
    if (usuarioEncontrado != NULL && strcmp(usuarioEncontrado->contrasena, contrasena) == 0) {
        printf("Inicio de sesion exitoso. Bienvenido, %s!\n", usuarioEncontrado->nickname);
        return usuarioEncontrado;
    }
    printf("Usuario o contrasena incorrectos.\n");
    return NULL;
}

NodoUsuario *Registrarse(NodoUsuario *raiz) {
    char correo[50], nombreReal[50], nickname[50], contrasena[50], paisOrigen[50], tipo[10];
    int i;

    do {
        printf("Escriba su correo electronico: ");
        fgets(correo, sizeof(correo), stdin); correo[strcspn(correo, "\n")] = '\0';
        i = verificacionCorreo(correo);
    } while (i != 1);

    printf("Ingrese su nombre completo: ");
    fgets(nombreReal, sizeof(nombreReal), stdin); nombreReal[strcspn(nombreReal, "\n")] = '\0';

    printf("Ingrese su nickname: ");
    fgets(nickname, sizeof(nickname), stdin); nickname[strcspn(nickname, "\n")] = '\0';

    printf("Ingrese su contrasena: ");
    fgets(contrasena, sizeof(contrasena), stdin); contrasena[strcspn(contrasena, "\n")] = '\0';

    printf("Ingrese su pais de origen: ");
    fgets(paisOrigen, sizeof(paisOrigen), stdin); paisOrigen[strcspn(paisOrigen, "\n")] = '\0';

    do {
        printf("Ingrese el tipo de cuenta (premium o free): ");
        fgets(tipo, sizeof(tipo), stdin); tipo[strcspn(tipo, "\n")] = '\0';
        i = verificacionPoF(tipo);
    } while (i != 1);

    raiz = insertarUsuario(raiz, nombreReal, paisOrigen, correo, nickname, contrasena, tipo);
    printf("Registro exitoso. Bienvenido, %s!\n", nickname);
    return raiz;
}