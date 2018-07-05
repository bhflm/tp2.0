#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE


#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FIN_LINEA '\0'
#define VACIO ""

char** split(const char* str, char sep){
  size_t i=0,j=0,k=0,cant=2;

  if(str==NULL || strcmp(str,VACIO)==0){
    char** strv=malloc(sizeof(char*)*cant);
    strv[0]=strdup(VACIO);
    strv[1]=NULL;
    return strv;
  }

  //Cuento los separadores
  while(str[i]!=FIN_LINEA){
    if(str[i]==sep) cant++;
    i++;
  }

  char** strv=malloc(sizeof(char*)*cant);
  i=0;

  while(str[i]!=FIN_LINEA){
    if(str[i]==sep){
      strv[k]=strndup(&str[j],(i-j));
      j=i+1;
      k++;
    }
    i++;
    if(str[i]==FIN_LINEA){
      strv[k]=strndup(&str[j],(i-j));
      j=i+1;
      k++;
    }
  }
  strv[cant-1]=NULL;
  return strv;
}

char* join(char** strv, char sep){
  size_t i=0,j=0,k=0,len=0;

  if(strv[i]==NULL){
    char* str=malloc(sizeof(char));
    str[i]=FIN_LINEA;
    return str;
  }

  while(strv[i]!=NULL){
    len+=strlen(strv[i])+1;
    i++;
  }
  char* str=malloc(sizeof(char)*(len));
  i=0;
  while(strv[i]!=NULL){
    if(strv[i][j]==FIN_LINEA){
      if(k+1==len) str[k]=FIN_LINEA;
      else str[k]=sep;
      j=0;
      i++,k++;
    }
    else{
      str[k]=strv[i][j];
      j++,k++;
    }
  }
  return str;
}

void free_strv(char* strv[]){
  size_t i=0;
  while(strv[i]!=NULL){
    free(strv[i]);
    i++;
  }
  free(strv);
}
