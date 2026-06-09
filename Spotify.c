#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NodoUsuario { // Nodo para el árbol de usuarios
    char usuario[50];
    char contrasena[50];
    char tipo[10];       // "premium" o "free"
    struct NodoUsuario *izq;
    struct NodoUsuario *der
} NodoUsuario;

//Prototipos de funciones
NodoUsuario* insertarUsuario (NodoUsuario *raiz, char user[], char pass [], char tipo[]); // Función para insertar un nuevo usuario en el árbol
NodoUsuario* buscarUsuario(NodoUsuario *raiz, char user[]); // Función para buscar un usuario en el árbol por su nombre de usuario

NodoUsuario* IniciarSesion(NodoUsuario *raiz){ // Función para iniciar sesión, que solicita el nombre de usuario y la contraseña, y verifica si son correctos
    char usuario[50];
    char contrasena[50];
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);
    printf("Ingrese su contrasena: ");
    scanf("%s", contrasena);
   
    NodoUsuario *usuarioEncontrado = buscarUsuario(raiz, usuario);

    if (usuarioEncontrado != NULL && strcmp(usuarioEncontrado->contrasena, contrasena) == 0) { // Verificar que la contraseña coincida
        printf("Inicio de sesion exitoso. Bienvenido, %s!\n", usuarioEncontrado->usuario);
        return usuarioEncontrado;
    } else {
        printf("Usuario o contrasena incorrectos.\n");
        return NULL;
    }

}

NodoUsuario* Registrarse(NodoUsuario *raiz){ // Función para registrarse, que solicita el nombre de usuario, la contraseña y el tipo de cuenta (premium o free), y luego inserta el nuevo usuario en el árbol
    char usuario[50];
    char contrasena[50];
    char tipo[10];
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);
    printf("Ingrese su contrasena: ");
    scanf("%s", contrasena);
    printf("Ingrese el tipo de cuenta (premium o free): ");
    scanf("%s", tipo);

    raiz = insertarUsuario(raiz, usuario, contrasena, tipo);
    printf("Registro exitoso. Bienvenido, %s!\n", usuario);
    return raiz;
}

int main(){
    //PREGUNTAS PARA EL PROYECTO
    /* Porque usamos los arboles abb y como se definen los valores de cada nodo ya sea para artistas y usuarios
    
    */
   //entrar al programa, iniciar sesion o registrarse(pedir datos y premium o free, valor y duracion)
   //mandar al menu de inicio(return)
   // menu de agregar canciones,eliminar,playlist,verplan,amigos,artistas,
   // estadisticas(canciones mas escucuahdadas,artistaspreferidos,anuncion mostrados,tiempototal))

   //MAIN
   NodoUsuario *raizUsuarios = NULL;
   NodoUsuario *usuarioActual = NULL;
   int opcion;

   do {
         printf("1. Iniciar Sesion\n");
         printf("2. Registrarse\n");
         printf("3. Salir\n");
         printf("Seleccione una opcion: ");
         scanf("%d", &opcion);
    
         switch (opcion) {
              case 1:
                usuarioActual = IniciarSesion(raizUsuarios);
                if (usuarioActual != NULL) {
                    // código para mostrar el menú principal después de iniciar sesión
                }
                break;
              case 2:
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


NodoUsuario* insertarUsuario (NodoUsuario *raiz, char user[], char pass [], char tipo[]){ // Función para insertar un nuevo usuario en el árbol
    if (raiz == NULL) {
        NodoUsuario *nuevo = (NodoUsuario*)malloc(sizeof(NodoUsuario));
        strcpy(nuevo->usuario, user);
        strcpy(nuevo->contrasena, pass);
        strcpy(nuevo->tipo, tipo);
        nuevo->izq = NULL;
        nuevo->der = NULL;
        return nuevo;
    }
   
    int comparacion = strcmp(user, raiz->usuario);
    if (comparacion < 0) {
        raiz->izq = insertarUsuario(raiz->izq, user, pass, tipo);
    } else if (comparacion > 0) {
        raiz->der = insertarUsuario(raiz->der, user, pass, tipo);
    } else {
        printf("El usuario ya existe.\n");
    }
}

NodoUsuario* buscarUsuario(NodoUsuario *raiz, char user[]) { // Función para buscar un usuario en el árbol por su nombre de usuario
    if(raiz == NULL || strcmp(user, raiz ->usuario)==0){
        return raiz;
    }

    if (strcmp(user, raiz->usuario) < 0) {
        return buscarUsuario(raiz->izq, user);
    } else {
        return buscarUsuario(raiz->der, user);
    }
}


