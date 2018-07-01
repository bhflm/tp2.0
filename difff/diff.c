#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define VACIO " "

//gcc -g -Wall -Wconversion -Wno-sign-conversion -Werror -o  *.c

int main(int argc, char* argv[]){
  if(argc !=3){
    fprintf(stderr, "Cantidad de parametros erronea\n");
    return 1;
  }

  const char* file_1 = argv[1];
  const char* file_2 = argv[2];

  FILE* archivo_1 = fopen(file_1,"r");
  FILE* archivo_2 = fopen(file_2,"r");
  if(!archivo_1){
    fprintf(stderr,"Archivo erroneo");
    return 1;
  }
  if(!archivo_2){
    fprintf(stderr,"Archivo erroneo");
    return 1;
  }

  char* linea_1 = NULL; size_t cap_1 = 0; ssize_t leidos_1; //combo getline
  char* linea_2 = NULL; size_t cap_2 = 0; ssize_t leidos_2; //combo getline
  leidos_1 = getline(&linea_1,&cap_1,archivo_1);
  leidos_2 = getline(&linea_2,&cap_2,archivo_2);

  size_t i=1;
  while(leidos_1!=-1 && leidos_2!=-1 ){
          char* a = strdup(linea_1);
          char* b = strdup(linea_2);
          if(strcmp(a,b)!=0){
            fprintf(stdout,"Diferencia en linea %zu\n",i);
            fprintf(stdout,"< %s",a);
            fprintf(stdout,"---\n");
            fprintf(stdout,"> %s",b);
          }
          free(a),free(b);
          leidos_1 = getline(&linea_1,&cap_1,archivo_1);
          leidos_2 = getline(&linea_2,&cap_2,archivo_2);
          i++;
  }

  while(leidos_1!=-1){
      char* a = strdup(linea_1);
        fprintf(stdout,"Diferencia en linea %zu\n",i);
        fprintf(stdout,"< %s",a);
        fprintf(stdout,"---\n");
        fprintf(stdout,"> %s\n",VACIO);
      i++;
      free(a);
      leidos_1 = getline(&linea_1,&cap_1,archivo_1);
  }

  while(leidos_2!=-1){
      char* b = strdup(linea_2);
        fprintf(stdout,"Diferencia en linea %zu\n",i);
        fprintf(stdout,"< %s\n",VACIO);
        fprintf(stdout,"---\n");
        fprintf(stdout,"> %s",b);
      i++;
      free(b);
      leidos_2 = getline(&linea_2,&cap_2,archivo_2);
  }
  free(linea_1);
  free(linea_2);
  fclose(archivo_1),fclose(archivo_2);
  return 0;
}
