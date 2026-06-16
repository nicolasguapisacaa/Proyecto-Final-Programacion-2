#include <stdio.h>
#include <stdlib.h>

typedef struct Nodo {
    int valor;
    struct Nodo* izq;
    struct Nodo* der;
} Nodo;

Nodo *crearNodo (int valor){
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->valor = valor;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

Nodo *insertar (Nodo* raiz, int valor){
    if (raiz == NULL) {
        return crearNodo(valor);
    }
    if (valor < raiz->valor) {
        raiz->izq = insertar(raiz->izq, valor);
    } else if (valor > raiz->valor) {
        raiz->der = insertar(raiz->der, valor);
    }
    return raiz;
}

void unRecorrido(Nodo* raiz) {
    if (raiz != NULL) {
    printf("%d ", raiz->valor);
    unRecorrido(raiz->izq);
    unRecorrido(raiz->der);
    }
}
