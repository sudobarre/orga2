#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "checkpoints.h"

int main (void){
	/* Acá pueden realizar sus propias pruebas */

	//--------------checkpoint 2 ------------------------
	assert(alternate_sum_4(8,2,5,1) == 10 && printf("It Worked!!\n"));
	assert(alternate_sum_4_using_c(8,2,5,1) == 10 && printf("The second one worked too!!\n"));	
	assert(alternate_sum_4_simplified(8,2,5,1) == 10 && printf("The third one worked too!!\n"));
	assert(alternate_sum_8(8,2,5,1,5,1,8,2) == 20 && printf("The fourth one worked too!!\n"));
	uint32_t a;
	uint32_t x1 = 4;
	float f1 = 3.456;
	uint32_t * destination = &a;
	product_2_f(destination, x1, f1);
	printf("El product2f con un x1 = %d y f1 = %f da: %d\n", x1, f1, a);
	//--------------------checkpoint 3-------------------------------
	struct complex_item_str items[4];
     for(int i = 0; i < 4; i++) {
		 items[i].w=i;
		 items[i].x=i+1;
		 items[i].y=i+2;
		 items[i].z=i+3; //3+4+5+6
	 }
	printf("el complexSumZ da: %d\n", complex_sum_z(items, 4));

	struct complex_item_str itemsPacked[4];
     for(int i = 0; i < 4; i++) {
		 itemsPacked[i].w=i;
		 itemsPacked[i].x=i+1;
		 itemsPacked[i].y=i+2;
		 itemsPacked[i].z=i+3; //3+4+5+6
	 }
	printf("el packedComplexSumZ da: %d\n", complex_sum_z(itemsPacked, 4));
	double res;
	double *destination2 = &res;

	product_9_f(destination2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.33);
	printf("%f\n", *destination2);
	return 0;    
}


