#define _XOPEN_SOURCE
#define TIME_FORMAT "%FT%T%z"

#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "heap.h"
#include "strutil.h"

#include "modificaciones_tda.h"

#define TAB '\t'


time_t iso8601_to_time(const char* iso8601){
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}


linea_registro_t* crear_linea_registro(){
  linea_registro_t* linea_registro = malloc(sizeof(linea_registro_t));
  if(!linea_registro) return NULL;
  linea_registro->ip = NULL;
  linea_registro->fecha = NULL;
  linea_registro->recurso = NULL;
  linea_registro->ruta = NULL;
  return linea_registro;
}

void cargar_linea_registro(linea_registro_t* registro,char* linea){
  char** campos = split(linea,'\t');
  registro->ip=strdup(campos[0]);
  registro->fecha=strdup(campos[1]);
  registro->recurso=strdup(campos[2]);
  registro->ruta=strdup(campos[3]);
  free_strv(campos);
}

void destruir_registro_individual(linea_registro_t* registro){
      free((char*)registro->ip);
      free((char*)registro->fecha);
      free((char*)registro->recurso);
      free((char*)registro->ruta);
}

void linea_registro_destruir(linea_registro_t** registro,size_t TAM_REGISTRO){
  for(size_t i=0;i<TAM_REGISTRO;i++){
     if(registro[i]){
         destruir_registro_individual(registro[i]);
      }
    free(registro[i]);
  }
  free(registro);
}

registro_t* crear_registro(char* linea,FILE* fp){
  registro_t* registro = malloc(sizeof(registro_t));
  if(!registro) return NULL;

  registro->linea = strdup(linea);
  registro->fp = fp;
  return registro;
}

void registro_destruir(registro_t** registro,size_t TAM_REGISTRO){
  for(size_t i=0;i<TAM_REGISTRO;i++){
    if(registro[i]){
      free((char*)registro[i]->linea);
    }
    free(registro[i]);
  }
  free(registro);
}

void free_strvs(char** a, char** b){
  free_strv(a);
  free_strv(b);
}

int funcion_cmp_logs(void* log_a, void* log_b){

  char* tiempo_a =  ((linea_registro_t*)log_a)->fecha;
  char * tiempo_b = ((linea_registro_t*)log_a)->fecha;

  char* ip_a = ((linea_registro_t*)log_a)->ip;
  char* ip_b = ((linea_registro_t*)log_b)->ip;

  char* recurso_a = ((linea_registro_t*)log_a)->recurso;
  char* recurso_b = ((linea_registro_t*)log_b)->recurso;

  time_t a = iso8601_to_time(tiempo_a);
  time_t b = iso8601_to_time(tiempo_b);



  double diferencia = difftime(a,b);
  /*
  Comparacion de diffs de tiempos.

  Estamos buscando un heap armado de minimos, significa, que vamos a tener algo del
  estilo (menor horario, mh+1, mh+2, .... mh+n);

  Si a es mas temprano que b, diff devuelve < 0, entonces tiene q subir en upheap. >
  > para que suba en upheap, tiene que entrar al swap, esto pasa si en la comparacion
  de upheap se devuelve > 0.

  Si a es mas tarde que b, diff devuelve>0.

  Si a es == b, diff devuelve 0.
  */

  if(diferencia>0){
    return 1;
  }
  else if(diferencia<0){
    return -1;
  }
   else{
       //Tiempos son iguales. Comparo por IPS.
       if (strcmp(ip_a,ip_b)<0){
         return 1;
       }
       else if(strcmp(ip_a,ip_b)>0){
         return -1;
       }
       else{
         //Tiempos e IPS son iguales. Comparo por recursos.
         if (strcmp(recurso_a,recurso_b)<0){
          return 1;
         }



         else{
         return -1;
         }
       }
  }
}


int funcion_cmp_registros(void* a,void* b){
  //PSEUDO WRAPPER

  char* log_a = strdup(((registro_t*)a)->linea);
  char* log_b = strdup(((registro_t*)b)->linea);

  linea_registro_t** registros;
  registros = malloc(sizeof(linea_registro_t*)*2);
  if(!registros) return 0;

  registros[0] = crear_linea_registro();
  registros[1] = crear_linea_registro();

  cargar_linea_registro(registros[0],log_a);
  cargar_linea_registro(registros[1],log_b);

  int comparacion = funcion_cmp_logs((void*)registros[0],(void*)registros[1]);

  linea_registro_destruir(registros,2);

  free(log_a);
  free(log_b);

  return comparacion;
}

int funcion_cmp_ip(const char* ip1, const char* ip2){		
	
	char** array_ip1 = split(ip1,'.');
	char** array_ip2 = split(ip2,'.');
	int resultado = 0;

	for (int i = 0; i < 4; i++){
		int actual_1 = atoi(array_ip1[i]);
		int actual_2 = atoi(array_ip2[i]);
		if (actual_1 > actual_2){
			resultado = 1;
			break;
		} else if (actual_1 < actual_2){
			resultado = -1;
			break;
		}
	}

	free_strv(array_ip1);
	free_strv(array_ip2);
	return resultado;
}
