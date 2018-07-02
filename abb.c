#include "abb.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "pila.h"


typedef struct nodo{
	struct nodo *izq;
	struct nodo *der;
	const char* clave;
	void* dato;
} nodo_t;


struct abb{
	abb_destruir_dato_t abb_destruir_dato;
	abb_comparar_clave_t abb_comparar_clave;
	struct nodo *raiz;
	size_t cantidad;
};

struct abb_iter{
	const abb_t *arbol;
	pila_t *pila;
};

nodo_t* nodo_crear(const char* clave, void* dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return NULL;

	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = strdup(clave);
	nodo->dato = dato;

	return nodo;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t));
	if(!abb) return NULL;
	abb->abb_comparar_clave = cmp;
	abb->abb_destruir_dato = destruir_dato;
	abb->raiz = NULL;
	abb->cantidad = 0;
	return abb;
}


bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	nodo_t* n_guardar = nodo_crear(clave,dato);
	if (!n_guardar) return false;

	nodo_t* anterior = NULL;
	nodo_t* actual = arbol->raiz;

	while(actual){
		anterior = actual;
		if(arbol->abb_comparar_clave(n_guardar->clave,actual->clave)<0){ // va a la rama izq
			actual = actual->izq;
		}
		else if (arbol->abb_comparar_clave(n_guardar->clave,actual->clave)>0){ //va a la rama der
			actual = actual->der;
		}
		else {
			if(actual->dato){
				if(arbol->abb_destruir_dato!=NULL) arbol->abb_destruir_dato(actual->dato);
			}
			actual->dato = n_guardar->dato;
			free((char*)n_guardar->clave);
			free(n_guardar);
			return true;
		}
	}
	//Hay que decirle a n_guardar que anterior es su padre.
	//n_guardar.padre = anterior

	if(!anterior) arbol->raiz = n_guardar;
	else if((arbol->abb_comparar_clave(n_guardar->clave,anterior->clave)<0)){
		anterior->izq = n_guardar;
	}
	else anterior->der = n_guardar;
	arbol->cantidad++;
	return true;
}


void *abb_obtener(const abb_t *arbol, const char *clave){
	nodo_t* actual = arbol->raiz;

	while(actual){
		if(arbol->abb_comparar_clave(clave,actual->clave)<0){ // va a la rama izq
			actual = actual->izq;
		} else if (arbol->abb_comparar_clave(clave,actual->clave)>0){ //va a la rama der
			actual = actual->der;
			} else {
			return actual->dato;
		}
	}
	return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	nodo_t* actual = arbol->raiz;

	while(actual){
		if(arbol->abb_comparar_clave(clave,actual->clave)<0){ // va a la rama izq
			actual = actual->izq;
		}
		else if (arbol->abb_comparar_clave(clave,actual->clave)>0){ //va a la rama der
			actual = actual->der;
		} else return true;
	}
	return false;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}


void *abb_borrar(abb_t *arbol, const char *clave){
	nodo_t* actual = arbol->raiz;
	nodo_t* a_borrar = NULL;
	nodo_t* a_borrar_padre = NULL;

	void *dato;

	if (!abb_pertenece(arbol, clave))return NULL;

	while(actual){
		a_borrar_padre = a_borrar;
		a_borrar = actual;

		if(arbol->abb_comparar_clave(clave,actual->clave)<0){ // va a la rama izq
			actual = actual->izq;
		}
		else if (arbol->abb_comparar_clave(clave,actual->clave)>0){ //va a la rama der
			actual = actual->der;
		} else {
			break;
		}
	}

	//1) Borrar hoja
	if (!a_borrar->izq && !a_borrar->der){
		if(a_borrar == arbol->raiz){
			arbol->raiz = NULL;
		} else if(arbol->abb_comparar_clave(a_borrar_padre->clave, a_borrar->clave) > 0){
			a_borrar_padre->izq = NULL;
		} else {
			a_borrar_padre->der = NULL;
		}
		dato = a_borrar->dato;
		free((char*)a_borrar->clave);
		free(a_borrar);
	}

	//2) Borrar nodo con un hijo
	else if (!a_borrar->izq || !a_borrar->der){
		nodo_t *a_borrar_hijo;
		if (a_borrar->izq){
			a_borrar_hijo = a_borrar->izq;
		} else {
			a_borrar_hijo = a_borrar->der;
		}

		if (a_borrar == arbol->raiz){
			arbol->raiz = a_borrar_hijo;

		} else if(arbol->abb_comparar_clave(a_borrar_padre->clave, a_borrar->clave) > 0){
			a_borrar_padre->izq = a_borrar_hijo;
		} else {
			a_borrar_padre->der = a_borrar_hijo;
		}
		dato = a_borrar->dato;
		free((char*)a_borrar->clave);
		free(a_borrar);
	}


	//3) Borrar nodo con dos hijos
	else {
		nodo_t *a_reemplazar = a_borrar->der;
		nodo_t *a_reemplazar_padre = a_borrar;
		while (a_reemplazar->izq){
			a_reemplazar_padre = a_reemplazar;
			a_reemplazar = a_reemplazar->izq;
		}


		dato = a_borrar->dato;

		a_borrar->dato = a_reemplazar->dato;
		free((char*)a_borrar->clave);
		a_borrar->clave = strdup(a_reemplazar->clave);

		if (a_reemplazar_padre == a_borrar){
			 a_reemplazar_padre->der = a_reemplazar->der;
		} else {
			a_reemplazar_padre->izq = a_reemplazar->der;
		}
		free((char*)a_reemplazar->clave);
		free(a_reemplazar);
	}
	arbol->cantidad--;
	return dato;
}

void abb_destruir_rec(abb_t *arbol, nodo_t *actual){
	if(!actual) return;

	abb_destruir_rec(arbol, actual->izq);
	abb_destruir_rec(arbol, actual->der);

	if (arbol->abb_destruir_dato){
		arbol->abb_destruir_dato(actual->dato);
	}
	free((char*)actual->clave);
	free(actual);
}

void abb_destruir(abb_t *arbol){
	nodo_t *actual = arbol->raiz;
	abb_destruir_rec(arbol, actual);
	free(arbol);
}


//ITERADOR INTERNO

void _abb_in_order(nodo_t* nodo, bool visitar(const char*, void*, void*),void* extra,bool* continuar){
	if(!nodo) return;
	_abb_in_order(nodo->izq,visitar,extra,continuar);
	if(!*continuar) return;
	if(!visitar(nodo->clave,nodo->dato,extra)){

		*continuar = false;
	  return;
 	}
	_abb_in_order(nodo->der,visitar,extra,continuar);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	if(!arbol) return;
	bool continuar = true;
	_abb_in_order(arbol->raiz,visitar,extra,&continuar);
}



//ITERADOR EXTERNO

abb_iter_t* abb_iter_in_crear(const abb_t* arbol){
	abb_iter_t*iter = malloc(sizeof(abb_iter_t));
	pila_t* pila = pila_crear();
	if (!iter || !pila) return NULL;

	iter->arbol = arbol;
	iter->pila = pila;
	nodo_t *actual = arbol->raiz;

	while (actual){
		pila_apilar(iter->pila, actual);
		actual = actual->izq;
	}
	return iter;
}


bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila);
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if(abb_iter_in_al_final(iter)) return false;

	nodo_t *actual = pila_desapilar(iter->pila);

	if (actual->der) {
		actual = actual->der;
		pila_apilar(iter->pila, actual);
		while(actual->izq){
			actual = actual->izq;
			pila_apilar(iter->pila, actual);
		}
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	if(pila_esta_vacia(iter->pila)) return NULL;
	nodo_t * actual = pila_ver_tope(iter->pila);
	return actual->clave;
}


void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}
