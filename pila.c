#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
#define CAPACIDAD_INICIAL 8
#define CONSTANTE_REDIMENSION 2
#define CONSTANTE_CANTIDAD_MINIMA 4

pila_t* pila_crear(void){
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL){
		return NULL;
	}
	pila->capacidad = CAPACIDAD_INICIAL;
	pila->cantidad = 0;
	pila->datos = malloc(pila->capacidad * sizeof(void*));
	if (pila->datos == NULL) {
		free(pila);
		return NULL;	
	}
	return pila;
}

bool redimensionar_pila(pila_t *pila, size_t capacidad_nueva){
	void** datos_nuevo = realloc(pila->datos, capacidad_nueva * sizeof(void*));
	if (datos_nuevo == NULL){
		return false;
	}
	pila->capacidad = capacidad_nueva;
	pila->datos = datos_nuevo;
	return true;
}

void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void* valor){
	if (pila->cantidad == pila->capacidad){
		if (!redimensionar_pila(pila, pila->capacidad * CONSTANTE_REDIMENSION)){
			return false;
		}
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

void* pila_ver_tope(const pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	return pila->datos[pila->cantidad-1];
}

void* pila_desapilar(pila_t *pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	void* valor_desapilado = pila_ver_tope(pila);
	pila->cantidad --;
	if (CONSTANTE_CANTIDAD_MINIMA * pila->cantidad <= pila->capacidad && CAPACIDAD_INICIAL <= pila->cantidad){
		redimensionar_pila(pila, pila->capacidad / CONSTANTE_REDIMENSION);
	}
	return valor_desapilado;
}
