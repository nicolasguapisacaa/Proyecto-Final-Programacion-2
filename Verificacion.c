#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int verificacionCorreo(char correo[]){
	
	int i,j;
	
	for(j=0;correo[j]!='\0';j++){
		
		if(correo[j]== '@'){
			return 1;
		}
		
	}
	printf("Correo no valido\n");
	return 0;
}
