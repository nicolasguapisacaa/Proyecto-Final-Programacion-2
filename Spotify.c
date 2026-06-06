//Proyecto Spotify Guapisaca_Mendez

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//hail grasa
typedef struct {
    char nombre[50];
    char artista[50];
    char album[50];
    int duracion; // Duración en segundos
} Cancion;

void agregarCancion(Cancion *canciones, int *numCanciones) {
    if (*numCanciones >= 100) {
        printf("No se pueden agregar mas canciones. Capacidad maxima alcanzada.\n");
        return;
    }

    printf("Ingrese el nombre de la cancion: ");
    scanf("%s", canciones[*numCanciones].nombre);
    printf("Ingrese el nombre del artista: ");
    scanf("%s", canciones[*numCanciones].artista);
    printf("Ingrese el nombre del album: ");
    scanf("%s", canciones[*numCanciones].album);
    printf("Ingrese la duracion de la cancion en segundos: ");
    scanf("%d", &canciones[*numCanciones].duracion);

    (*numCanciones)++;
    printf("Cancion agregada exitosamente.\n");
}

void mostrarCanciones(Cancion *canciones, int numCanciones) {
    if (numCanciones == 0) {
        printf("No hay canciones en la lista.\n");
        return;
    }

    printf("Lista de canciones:\n");
    for (int i = 0; i < numCanciones; i++) {
        printf("%d. %s - %s (%s) [%d segundos]\n", i + 1, canciones[i].nombre, canciones[i].artista, canciones[i].album, canciones[i].duracion);
    }
}

int main() {
    Cancion canciones[100];
    int numCanciones = 0;
    int opcion;

    do {
        printf("\nMenu:\n");
        printf("1. Agregar cancion\n");
        printf("2. Mostrar canciones\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                agregarCancion(canciones, &numCanciones);
                break;
            case 2:
                mostrarCanciones(canciones, numCanciones);
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
        }
    } while (opcion != 3);

    return 0;
}