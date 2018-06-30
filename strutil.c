#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include "strutil.h"
#include <stdio.h>
#include <stdbool.h>

char** split(const char* str, char sep){
	size_t longitud = strlen(str);
	size_t contador_sep = 0;
 
	for (int i = 0; i < longitud; i++){
		if (str[i] == sep) contador_sep++;
	}
	char** resultado = malloc(sizeof(char*) * (contador_sep + 2));
	if (!resultado) return NULL;
	size_t posicion_resultado = 0;
	size_t inicio_palabra = 0;
	size_t final_palabra = 0;
	for (size_t i = 0; i < longitud; i++){
		if (str[i] == sep){
			resultado[posicion_resultado] = strndup(&str[inicio_palabra], final_palabra - inicio_palabra);
			final_palabra++;
			inicio_palabra = final_palabra;
			posicion_resultado++;
		} else {
			final_palabra++;
		}
	}
	resultado[posicion_resultado] = strndup(&str[inicio_palabra], final_palabra - inicio_palabra);
	resultado[posicion_resultado + 1] = '\0';
	return resultado;
}


char* join(char** strv, char sep){
	size_t cantidad_caracteres = 0;
	size_t posicion_strv = 0;

	if (!strv[0]){
		char* str = malloc(sizeof(char));
		str[0] = '\0';
		return str;
	}

	while (strv[posicion_strv] != NULL){
		cantidad_caracteres += strlen(strv[posicion_strv]);
		posicion_strv++;
	}

	char* str = malloc((cantidad_caracteres + posicion_strv) * sizeof(char));
	posicion_strv = 0;
	size_t inicio_palabra = 0;

	while (strv[posicion_strv] != NULL){
		size_t actual = 0;
		size_t longitud = strlen(strv[posicion_strv]);
		while(actual < longitud){
			str[actual + inicio_palabra] = strv[posicion_strv][actual];
			actual++;
		}
		if (strv[posicion_strv + 1] != NULL){
			str[actual + inicio_palabra] = sep;
			inicio_palabra += actual + 1;
		} else {
			str[actual + inicio_palabra] = '\0';
		}
		posicion_strv++;
	}
	return str;
}



void free_strv(char* strv[]){
	size_t posicion = 0;
	while (strv[posicion] != NULL){
		free(strv[posicion]);
		posicion++;
	}
	free(strv[posicion]);
	free(strv);
}
