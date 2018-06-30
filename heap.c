#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define COEF_CANT_MINIMA 4
#define COEF_REDIM 2
const size_t TAM_DEFAULT = 20;

struct heap{
  void** arreglo;
  size_t capacidad;
  size_t cantidad;
  cmp_func_t cmp;
};

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo){
  void* arreglo_nuevo = realloc(heap->arreglo,tam_nuevo * sizeof(void*));

  if(tam_nuevo > 0 && ! arreglo_nuevo) return false;

  heap->arreglo = arreglo_nuevo;
  heap->capacidad = tam_nuevo;
  return true;
}

void swap(void* arreglo[],size_t pos1,size_t pos2){
  void* aux = arreglo[pos1];
  arreglo[pos1]=arreglo[pos2];
  arreglo[pos2]=aux;
}

void upheap(heap_t* heap,size_t pos){
  if(pos==0) return;
  size_t padre = (pos-1)/2;
  if(heap->cmp(heap->arreglo[pos],heap->arreglo[padre])<0){
    
    return;
  }
  swap(heap->arreglo,pos,padre);
  upheap(heap,padre);
}

void downheap(void* arr[],size_t pos,size_t cantidad,cmp_func_t cmp){
  if(pos>cantidad-1) return;

  size_t izq = (pos*2)+1;
  size_t der = izq+1;
  size_t mayor = pos;

  if((izq<cantidad-1) && cmp(arr[izq],arr[pos])>0) mayor = izq;

  if((der<cantidad-1) && cmp(arr[der],arr[mayor])>0) mayor = der;

  if(mayor!=pos){
    swap(arr,pos,mayor);
    downheap(arr,mayor,cantidad,cmp);
  }
}

void heapify(void* arr[],size_t cantidad,cmp_func_t cmp){
    for(size_t i=cantidad-1;i>0;i--){
      downheap(arr,i,cantidad,cmp);
    }
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heapify(elementos,cant,cmp);

    for(size_t i=cant-1;i>0;i--){
      swap(elementos,0,i);
      cant--;
      downheap(elementos,0,cant,cmp);
    }
}

heap_t *heap_crear(cmp_func_t cmp){
  heap_t* heap = malloc(sizeof(heap_t));
  if(!heap) return NULL;

  heap->arreglo = malloc(sizeof(void*)*TAM_DEFAULT);
  if(!heap->arreglo){
    free(heap);
    return NULL;
  }

  heap->cmp = cmp;
  heap->capacidad = TAM_DEFAULT;
  heap->cantidad = 0;

  return heap;
}

heap_t *heap_crear_arr(void* arreglo[],size_t n,cmp_func_t cmp){
  if(!arreglo || n==0) return NULL;

  heap_t* heap = malloc(sizeof(heap_t));
  if(!heap) return NULL;

  heap->arreglo = malloc(sizeof(void*)*(n+TAM_DEFAULT));
  if(!heap->arreglo){
    free(heap);
    return NULL;
  }

  heap->cmp = cmp;
  heap->capacidad = n+TAM_DEFAULT;
  heap->cantidad = n;

  for(size_t i=0;i<n;i++){
    heap->arreglo[i]=arreglo[i];
  }

  heapify(heap->arreglo,heap->cantidad,heap->cmp);

  return heap;
}

void *heap_ver_max(const heap_t *heap){
  if(heap_esta_vacio(heap)) return NULL;
  return (heap->arreglo[0]);
}

bool heap_encolar(heap_t* heap,void* elem){
  if(!elem) return false;

  if (heap->cantidad == heap->capacidad){
    if(!heap_redimensionar(heap,(heap->capacidad)*COEF_REDIM)) return false;
  }

  if(heap_esta_vacio(heap)) heap->arreglo[0]=elem;

  else{
    heap->arreglo[heap->cantidad]=elem;
    upheap(heap,heap->cantidad);
  }


  heap->cantidad++;
  return true;
}

void* heap_desencolar(heap_t* heap){
  if(heap_esta_vacio(heap)) return NULL;

  void* dato;

  if(heap->cantidad==1){
    dato = heap->arreglo[0];
  }
  else{
    swap(heap->arreglo,0,heap->cantidad-1);
    downheap(heap->arreglo,0,heap->cantidad,heap->cmp);
  }

  dato = heap->arreglo[heap->cantidad-1];
  heap->cantidad --;

  if(heap->cantidad < heap->capacidad/COEF_CANT_MINIMA){
    heap_redimensionar(heap,heap->capacidad/COEF_REDIM);
  }
  return dato;
}

size_t heap_cantidad(const heap_t* heap){
  return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
  return (heap_cantidad(heap)==0);
}

void heap_destruir(heap_t* heap,void destruir_elemento(void *e)){
  for(size_t i=0;i<heap->capacidad;i++){
    if(heap->arreglo[i] && destruir_elemento) destruir_elemento(heap->arreglo[i]);
  }
  free(heap->arreglo);
  free(heap);
}
