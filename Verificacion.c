#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int verificacionCorreo(char correo[]){
	
	int j;
	
	for(j=0;correo[j]!='\0';j++){
		
		if(correo[j]== '@'){
			return 1;
		}
		
	}
	printf("Correo no valido\n");
	return 0;
}

int verificacionPoF(char tipo[]){

if(strcmp(tipo,"premium")==0 || strcmp(tipo,"free")==0){
	return 1;
}
else{
		printf("Escriba correctamente el tipo de cuenta\n");
		return 0;
}

}

