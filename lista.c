#include "lista.h"
#include <stdlib.h>

//DEFINICION DEL TIPO NODO

typedef struct nodo {
	void* dato;
	struct nodo* proximo;
} nodo_t;

//FUNCION AUXILIAR NODO

nodo_t* nodo_crear_lista(void* valor) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) {
		return NULL;
	}
	nodo->dato = valor;
	nodo->proximo = NULL;
	return nodo;
}

//DEFINICION DEL STRUCT LISTA

struct lista {
	struct nodo* primero;
	struct nodo* ultimo;
	size_t cantidad;
};

//PRIMITIVAS DEL TIPO LISTA

lista_t* lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) {
		return NULL;
	}
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->cantidad = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	return lista->cantidad == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo = nodo_crear_lista(dato);
	if (nodo == NULL){
		return false;
	}
	if (lista_esta_vacia(lista)){
		lista->primero = nodo;
		lista->ultimo = nodo;
	} else {
		nodo->proximo = lista->primero;
		lista->primero = nodo;
	}
	lista->cantidad ++;
	return true;
}


bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nodo = nodo_crear_lista(dato);
	if (nodo == NULL){
		return false;
	}
	if (lista_esta_vacia(lista)){
		lista->primero = nodo;
		lista->ultimo = nodo;
	} else {
		lista->ultimo->proximo = nodo;
		lista->ultimo = nodo;
	}
	lista->cantidad ++;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	nodo_t* nodo_a_eliminar = lista->primero;
	void* borrado;	
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	if (nodo_a_eliminar == lista->ultimo){
		borrado = lista->primero->dato;
		lista->primero = NULL;
		lista->ultimo = NULL;
	} else {
		borrado = lista->primero->dato;
		lista->primero = lista->primero->proximo;
	}
	lista->cantidad--;
	free(nodo_a_eliminar);
	return borrado;
}

void *lista_ver_primero(const lista_t *lista){
	if (!lista->primero){
		return NULL;
	}
	return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if (!lista->ultimo){
		return NULL;
	}
	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->cantidad;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	nodo_t* nodo_actual = lista->primero;
	nodo_t* nodo_a_destruir;
	for (int i = 0; i < lista->cantidad; i++){
		if (destruir_dato){
			destruir_dato(nodo_actual->dato);
		}
		nodo_a_destruir = nodo_actual;
		nodo_actual = nodo_a_destruir->proximo;
		free(nodo_a_destruir);
	}
	free(lista);
}

//PRIMITIVAS DEL ITERADOR INTERNO

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t* actual = lista->primero;
	while(actual){
		if (!visitar(actual->dato, extra)) break;
		actual = actual->proximo;
	}
}

//DEFINICION DEL TIPO LISTA_ITER

struct lista_iter{
	struct lista* lista;
	struct nodo* anterior;	
	struct nodo* actual;
};


//PRIMITIVAS DEL TIPO LISTA_ITER


lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL){
		return NULL;
	}
	iter->lista = lista;
	iter->anterior = NULL;
	iter->actual = lista->primero;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if (iter->actual){
		iter->anterior = iter->actual;
		iter->actual = iter->actual->proximo;	
		return true;
	}
	return false;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (!iter->actual){
		return NULL;
	}
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	bool resultado = true;	
	if (iter->anterior == NULL){
		resultado = lista_insertar_primero(iter->lista, dato);
		iter->actual = iter->lista->primero;
	} else if (iter->actual == NULL){
		resultado = lista_insertar_ultimo(iter->lista, dato);
		iter->actual = iter->anterior->proximo;
	} else {
		nodo_t* nodo = nodo_crear_lista(dato);
		if (nodo == NULL){
			return false;
		}
		nodo->proximo = iter->actual;
		iter->anterior->proximo = nodo;
		iter->actual = nodo;
		iter->lista->cantidad++;
	}
	return resultado;
}

void *lista_iter_borrar(lista_iter_t *iter){
	void* borrado;
	if(!iter->actual){
		return NULL;
	}
	if (iter->anterior == NULL){
		borrado = lista_borrar_primero(iter->lista);
		iter->actual = iter->lista->primero;
	} else {
		nodo_t* nodo_a_eliminar = iter->actual;
		borrado = nodo_a_eliminar->dato;
		iter->anterior->proximo = iter->actual->proximo;
		iter->actual = iter->anterior->proximo;
		free(nodo_a_eliminar);
		if(iter->actual == NULL){
			iter->lista->ultimo = iter->anterior;
		}
		iter->lista->cantidad--;
	}
	return borrado;
}
