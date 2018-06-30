#include "hash.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "strutil.h"

#define TAM_HASH 30
#define CONST_REDIM 2
#define COEF_REDIM_UP 3/4
#define COEF_REDIM_DOWN 1/4

typedef enum{VACIO,OCUPADO,BORRADO}estado_t;

typedef struct nodo{
	const char* clave;
	void* dato;
	estado_t estado;
} nodo_t;

struct hash{
	nodo_t** tabla;
	size_t cant;
	size_t cap;
	hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
	const hash_t* hash;
	size_t pos;
};

//FUNCION SACADA DE INTERNET
unsigned long int fhash(const char* clave, size_t tam) {
	unsigned long int hash = 0;
	unsigned long int i = 0;
		
	while (clave[i] != '\0') { // Hash de tipo "Rotating/XOR" (intercambia posiciones entre bits y verifica los que se repiten con el tam.)
		hash = (hash << 2) ^ (hash >> 14) ^ (unsigned long int) clave[i];
		i++;
	}
	return hash%tam;
}

//FUNCION SACADA DE INTERNET
/*char* strdup(const char *anterior) {
	char* nuevo = malloc(sizeof(char) * (strlen(anterior) + 1));
	if (!nuevo) return NULL;
	strcpy(nuevo, anterior);
	return nuevo;
} */

nodo_t* crear_nodo(){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return NULL;
	nodo->dato = NULL;
	nodo->clave = NULL;
	nodo->estado = VACIO;
	return nodo;
}


size_t buscar_posicion(hash_t* hash, const char* clave){
	size_t posicion = fhash(clave, hash->cap);
	while (hash->tabla[posicion]->estado != VACIO){
		if (hash->tabla[posicion]->estado == OCUPADO){
			if (strcmp(hash->tabla[posicion]->clave, clave) == 0) break;
		}		
		posicion++;
		if (posicion == hash->cap) posicion = 0;
	}
	return posicion;
}

void hash_redimensionar(hash_t* hash, size_t tam_nuevo){
	nodo_t** nueva_tabla = malloc(sizeof(nodo_t*)*tam_nuevo);
	if(!nueva_tabla) return;
	for(size_t i=0; i<tam_nuevo; i++){
		nodo_t* nodo = crear_nodo();
		if(!nodo) return;
		nueva_tabla[i] = nodo;
	}
	//Una vez que tengo la nueva tabla empiezo a pasar a todos los campos ocupados de la vieja
	for(size_t i=0;i<hash->cap;i++){
		if(hash->tabla[i]->estado==OCUPADO){
			size_t posicion = fhash(hash->tabla[i]->clave,tam_nuevo);
			while(nueva_tabla[posicion]->estado==OCUPADO){
				posicion++;
				if(posicion == tam_nuevo) posicion = 0;
			}
			nueva_tabla[posicion]->estado=OCUPADO;
			nueva_tabla[posicion]->dato = hash->tabla[i]->dato;
			nueva_tabla[posicion]->clave = strdup(hash->tabla[i]->clave);

			free((char*)hash->tabla[i]->clave);
		}
		free(hash->tabla[i]);
	}
	nodo_t** tabla_aux = hash->tabla;
	hash->tabla = nueva_tabla;
	hash->cap = tam_nuevo;
	free(tabla_aux);
}


hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash) return NULL;

	hash->tabla = malloc(sizeof(nodo_t*) * TAM_HASH);
	if(!hash->tabla){
		free(hash);
		return NULL;
	}
	hash->cap = TAM_HASH;
	for(size_t i=0; i<hash->cap; i++){
		nodo_t* nodo = crear_nodo();
		if(!nodo) return NULL;
		hash->tabla[i] = nodo;
	}
	hash->cant = 0;
	hash->destruir_dato = destruir_dato;

	return hash;
}

bool hash_guardar(hash_t* hash, const char* clave, void *dato){
	if ((float)COEF_REDIM_UP * (float)hash->cap < hash->cant){
		hash_redimensionar(hash, hash->cap * CONST_REDIM);
	}
	char* clave_aux = strdup((char*)clave);
	if (!clave_aux) return false;
	size_t pos = fhash(clave_aux, hash->cap);
	while (hash->tabla[pos]->estado == OCUPADO){
		if (strcmp(hash->tabla[pos]->clave, clave_aux) == 0) break;
		pos++;
		if (pos == hash->cap) pos = 0;
	}
	if (hash->tabla[pos]->estado == OCUPADO){
		if (hash->destruir_dato){
			hash->destruir_dato(hash->tabla[pos]->dato);
		}
		free((char*)hash->tabla[pos]->clave);
	} else {
		hash->tabla[pos]->estado = OCUPADO;
		hash->cant ++;
	}
	hash->tabla[pos]->clave = clave_aux;
	hash->tabla[pos]->dato = dato;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	if(hash->cant == 0) return NULL;
	size_t pos = buscar_posicion(hash, clave);
	void* dato = NULL;
	if(hash->tabla[pos]->estado == OCUPADO){
		dato = hash->tabla[pos]->dato;
		if (hash->destruir_dato){
			hash->destruir_dato(hash->tabla[pos]->dato);
		}
		free((char*)hash->tabla[pos]->clave);
		hash->tabla[pos]->estado = BORRADO;
		hash->cant--;
	}
	if (hash->cant < (float)hash->cap*(float)COEF_REDIM_DOWN && hash->cap >= TAM_HASH){
		hash_redimensionar(hash, hash->cap / CONST_REDIM);
	}
	return dato;
}


void *hash_obtener(const hash_t *hash, const char *clave){
	if(hash->cant == 0) return NULL;
	void* dato = NULL;
	size_t pos = buscar_posicion((hash_t *)hash, clave);
	if(hash->tabla[pos]->estado == OCUPADO){
		dato = hash->tabla[pos]->dato;
	}
	return dato;
}

bool hash_pertenece(const hash_t *hash,const char *clave){
	if(hash->cant == 0) return false;
	bool encontro = false;
	size_t pos = buscar_posicion((hash_t *)hash, clave);
	if(hash->tabla[pos]->estado == OCUPADO){
		encontro = true;
	}
	return encontro;
}

size_t hash_cantidad(const hash_t* hash){
	return hash->cant;
}

void hash_destruir(hash_t* hash){
	for(size_t i = 0; i < hash->cap; i++){			
		if(hash->tabla[i]->estado == OCUPADO){
			if (hash->destruir_dato){
				hash->destruir_dato(hash->tabla[i]->dato);
			}
			free((char*)hash->tabla[i]->clave);
		}
		free(hash->tabla[i]);
	}
	free(hash->tabla);
	free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
	iter->hash = hash;
	size_t posicion = 0;
	while (posicion < iter->hash->cap && iter->hash->tabla[posicion]->estado != OCUPADO){
		posicion++;
	}
	iter->pos = posicion;
	return iter;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	return (iter->pos == iter->hash->cap);
}

bool hash_iter_avanzar(hash_iter_t *iter){
	if(!hash_iter_al_final(iter)){
		size_t posicion = iter->pos + 1;
		while(posicion < iter->hash->cap && iter->hash->tabla[posicion]->estado != OCUPADO){			
			posicion++;
		}
		iter->pos = posicion;
		return true;
	}
	return false;
}

const char* hash_iter_ver_actual(const hash_iter_t* iter){
	if (hash_iter_al_final(iter)) return NULL;
	const char* actual = iter->hash->tabla[iter->pos]->clave;	
	return actual;
}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}
