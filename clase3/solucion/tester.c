#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include "checkpoints.h"

#define ARR_LENGTH	4
#define ROLL_LENGTH 10

#define RUN(filename, action) pfile=fopen(filename,"a"); action; fclose(pfile);
#define NL(filename) pfile=fopen(filename,"a"); fprintf(pfile,"\n"); fclose(pfile);

char *filename_chk2 =  "salida.propios.chk2.txt";
char *filename_chk3 =  "salida.propios.chk3.txt";
char *filename_chk4 =  "salida.propios.chk4.txt";

void test_checkpoint2(char* filename);
void test_checkpoint3(char* filename);
void test_checkpoint4(char* filename);


int main() {
	srand(0);
    remove(filename_chk2);
    remove(filename_chk3);
    remove(filename_chk4);

	test_checkpoint2(filename_chk2);
	test_checkpoint3(filename_chk3);
    test_checkpoint4(filename_chk4);
	return 0;
}


static uint32_t x[ROLL_LENGTH];
static float f[ROLL_LENGTH];

void shuffle(uint32_t max){
	for(int i = 0; i < ROLL_LENGTH; i++){
		x[i] = (uint32_t) rand() % max;
        f[i] = ((float)rand()/(float)(RAND_MAX)) * max;
	}
}

void test_checkpoint2(char* filename) {
    FILE* pfile;

	shuffle(1000);

    RUN(filename, fprintf(pfile, "== Checkpoint 2 ==\n");) NL(filename)
    RUN(filename, fprintf(pfile, "alternate_sum_4\n");) NL(filename)
	
	for(int i = 0; i < 100; i++){	
		shuffle(1000);
	    RUN(filename, fprintf(pfile, "alternate_sum_4(%u,%u,%u,%u) -> %u", x[0],x[1],x[2],x[3], alternate_sum_4(x[0],x[1],x[2],x[3]));) NL(filename)
	}

	NL(filename) RUN(filename, fprintf(pfile, "alternate_sum_4_using_c\n");) NL(filename)
	
	for(int i = 0; i < 100; i++){	
		shuffle(1000);
	    RUN(filename, fprintf(pfile, "alternate_sum_4_using_c(%u,%u,%u,%u) -> %u", x[0],x[1],x[2],x[3], alternate_sum_4_using_c(x[0],x[1],x[2],x[3]));) NL(filename)
	}
	
	NL(filename) RUN(filename, fprintf(pfile, "alternate_sum_4_simplified\n");) NL(filename)
	
	for(int i = 0; i < 100; i++){	
		shuffle(1000);
	    RUN(filename, fprintf(pfile, "alternate_sum_4_simplified(%u,%u,%u,%u) -> %u", x[0],x[1],x[2],x[3], alternate_sum_4_simplified(x[0],x[1],x[2],x[3]));) NL(filename)
	}
	
	NL(filename) RUN(filename, fprintf(pfile, "alternate_sum_8\n");) NL(filename)
	
	for(int i = 0; i < 100; i++){	
		shuffle(1000);
	    RUN(filename, fprintf(pfile, "alternate_sum_8(%u,%u,%u,%u,%u,%u,%u,%u) -> %u", x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7], alternate_sum_8(x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7]));) NL(filename)
	}
	
	
	NL(filename) RUN(filename, fprintf(pfile, "product_2_f\n");) NL(filename)
	
	for(int i = 0; i < 100; i++){	
		shuffle(1000);
		uint32_t result;
		product_2_f(&result, x[0], f[0]);
	    RUN(filename, fprintf(pfile, "product_2_f(&dest,%u,%.2f) -> dest: %u", x[0],f[0],result);) NL(filename)
	}
}	

void test_checkpoint3(char* filename) {
    FILE* pfile;

    RUN(filename, fprintf(pfile, "== Checkpoint 3 ==\n");) NL(filename)
    RUN(filename, fprintf(pfile, "complex_sum_z\n");) NL(filename)

    complex_item complex_arr[ARR_LENGTH];
    
    for(int i=0; i< 100; i++){
        shuffle(1000);
        for(int j =  0; j < ARR_LENGTH; j++){ 
	 	    complex_arr[j].w = 0; 
	 	    complex_arr[j].x = 0; 
	 	    complex_arr[j].y = 0; 
	 	    complex_arr[j].z = x[j]; 
	    }   

        RUN(filename, fprintf(pfile, "complex_sum_z(%u,%u,%u,%u) -> %u", x[0],x[1],x[2],x[3], complex_sum_z(complex_arr,ARR_LENGTH));) NL(filename)

    }

	NL(filename) RUN(filename, fprintf(pfile, "packed_complex_sum_z\n");) NL(filename)
    
    packed_complex_item packed_complex_arr[ARR_LENGTH];

    for(int i=0; i< 100; i++){
        shuffle(1000);
  	
        for(int j =  0; j < ARR_LENGTH; j++){ 
            packed_complex_arr[j].w = 0; 
            packed_complex_arr[j].x = 0; 
            packed_complex_arr[j].y = 0; 
            packed_complex_arr[j].z = x[j]; 
        }

        RUN(filename, fprintf(pfile, "packed_complex_sum_z(%u,%u,%u,%u) -> %u", x[0],x[1],x[2],x[3], packed_complex_sum_z(packed_complex_arr, ARR_LENGTH));) NL(filename)
    }
	
	NL(filename) RUN(filename, fprintf(pfile, "product_9_f\n");) NL(filename)
	
	for(int i = 0; i < 100; i++){	
		shuffle(1000);
        double result;
        product_9_f(&result,x[0],f[0],x[1],f[1],x[2],f[2],x[3],f[3],x[4],f[4],x[5],f[5],x[6],f[6],x[7],f[7],x[8],f[8]);
	    RUN(filename, fprintf(pfile, "product_9_f(&dest,%u,%.2f,%u,%.2f,%u,%.2f,%u,%.2f,%u,%.2f,%u,%.2f,%u,%.2f,%u,%.2f,%u,%.2f) -> dest: %.2f", 
            x[0],f[0],x[1],f[1],x[2],f[2],x[3],f[3],x[4],f[4],x[5],f[5],x[6],f[6],x[7],f[7],x[8],f[8], result);) NL(filename)
	}
}

void test_checkpoint4(char* filename) {
     FILE* pfile;
     char* a = "Omega 4";
     char* b = "Palaven";
     char* c = "Feros";
     char* n = "";

    RUN(filename, fprintf(pfile, "== String ==\n");) NL(filename)

    // Clonar
    RUN(filename, fprintf(pfile, "Clonar\n");) NL(filename)
    char* ac = strClone(a);
    assert(a != ac); assert(strCmp(a,ac) == 0);
    RUN(filename, fprintf(pfile, "%i - ", strLen(ac));) RUN(filename, strPrint(ac,pfile);) NL(filename)
    char* nc = strClone(n);
    assert(n != nc); assert(strCmp(n,nc) == 0);
    RUN(filename, fprintf(pfile, "%i - ", strLen(nc));) RUN(filename, strPrint(nc,pfile);) NL(filename)
    NL(filename)

    //Borrar
    strDelete(ac);
    strDelete(nc);

    //Comparar
    RUN(filename, fprintf(pfile, "Comparar\n");) NL(filename)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", a, a, strCmp(a,a));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", n, n, strCmp(n,n));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", b, a, strCmp(b,a));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", a, b, strCmp(a,b));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", c, a, strCmp(c,a));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", b, c, strCmp(b,c));)
    RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n", c, b, strCmp(c,b));)
   
    char* texts[9] = {"sar","23","taaa","tbb","tix", "taaab", "taa0", "tbb", ""};
    for(int i=0; i<9; i++) {
        RUN(filename, fprintf(pfile,"strLen(%s) --> %d\n",texts[i],strLen(texts[i])))
        for(int j=0; j<9; j++) {
            RUN(filename, fprintf(pfile,"strCmp(%s,%s) --> %d\n",texts[i],texts[j],strCmp(texts[i],texts[j])))   
        }
    }
    NL(filename)
 }





