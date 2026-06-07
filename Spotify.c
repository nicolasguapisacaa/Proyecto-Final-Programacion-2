#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu1(){
    int opcion;
   do {
     printf("Bienvenido a Spotify\n");
    printf("1. Iniciar sesion\n");
    printf("2. Registrarse\n");
    printf("3. Salir\n");
   } while (opcion < 1 || opcion > 3);
   switch (opcion) {
    case 1:
        //codigo para iniciar sesion
        break;
    case 2:
        //codigo para registrarse
        break;
    case 3:
        printf("Gracias por usar Spotify\n");
        break;
   }
}

void IniciarSesion(){
    char usuario[50];
    char contrasena[50];
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);
    printf("Ingrese su contrasena: ");
    scanf("%s", contrasena);
    //codigo para verificar si el usuario y la contrasena son correctos
}

void Registrarse(){
    char usuario[50];
    char contrasena[50];
    char tipo[10];
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);
    printf("Ingrese su contrasena: ");
    scanf("%s", contrasena);
    printf("Ingrese el tipo de cuenta (premium o free): ");
    scanf("%s", tipo);
    //codigo para guardar el nuevo usuario en la base de datos
}

void guardarUsuario(char usuario[50], char contrasena[50], char tipo[10]) {
    

}

int main(){

    /* Porque usamos los arboles abb y como se definen los valores de cada nodo ya sea para artistas y usuarios
    
    */
   //entrar al programa, iniciar sesion o registrarse(pedir datos y premium o free, valor y duracion)
   //mandar al menu de inicio(return)
   // menu de agregar canciones,eliminar,playlist,verplan,amigos,artistas,
   // estadisticas(canciones mas escucuahdadas,artistaspreferidos,anuncion mostrados,tiempototal))

}
