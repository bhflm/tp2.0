#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>
#include <stdbool.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista es una lista de punteros. */

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero o falso, según si la lista tiene elementos o no.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso 
// de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento al comienzo de la lista. La lista tiene
// un elemento mas.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso 
// de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento al final de la lista. La lista tiene
// un elemento mas.
bool lista_insertar_ultimo(lista_t *lista, void *dato);


// Elimina el primer elemento de la lista y lo devuelve, si la lista
// tiene elementos se disminuye en uno la cantidad. Si esta vacia se devuelve NULL.
// Pre: la lista fue creada.
// Post: se elimino el primer elemento de la lista, se lo devolvió. 
// La lista tiene un elemento menos, cuando no está vacía. 
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

//Devuelve la cantidad de elemenos de la lista.
//Pre: la lista fue creada.
//Post: se devolvió la cantidad de elementos de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));



void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


lista_iter_t *lista_iter_crear(lista_t *lista);
bool lista_iter_avanzar(lista_iter_t *iter);
void *lista_iter_ver_actual(const lista_iter_t *iter);
bool lista_iter_al_final(const lista_iter_t *iter);
void lista_iter_destruir(lista_iter_t *iter);
bool lista_iter_insertar(lista_iter_t *iter, void *dato);
void *lista_iter_borrar(lista_iter_t *iter);




#endif // LISTA_H
