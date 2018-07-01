#ifndef MODIFICACIONES_TDA_H
#define MODIFICACIONES_TDA_H

#include <stdbool.h>  /* bool */
#include <stddef.h>	  /* size_t */



typedef struct linea_registro {
  char* ip;
  char* fecha;
  char* recurso;
  char* ruta;
} linea_registro_t;

typedef struct registro {
  char* linea;
  FILE* fp;
} registro_t ;



linea_registro_t* crear_linea_registro();
void cargar_linea_registro(linea_registro_t* registro,char* linea);
void destruir_registro_individual(linea_registro_t* registro);
void linea_registro_destruir(linea_registro_t** registro,size_t TAM_REGISTRO);
registro_t* crear_registro(char* linea,FILE* fp);
void free_strvs(char** a, char** b);
int funcion_cmp_logs(void* log_a, void* log_b);
int funcion_cmp_registros(void* a,void* b);
time_t iso8601_to_time(const char* iso8601);
int funcion_cmp_ip(const char* ip1, const char* ip2);

#endif
