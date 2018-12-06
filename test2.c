#include"k.h"
#include<stdlib.h>
#include<stdio.h>

int main(){
  int c=khp("localhost",5001);                         //connect
    char *line= "heyo2:([]name:`symbol$(); num:`int$())";
    k(c,line,(K)0);
	k(c, "save `heyo2", (K)0);
	printf("heregood\n");
  K row; 
  K result;
	int i;
  for (i=0; i<4 ; i++){
//	row= knk(2, ks((S) "NewGuy"), ki(i));
	row= knk(2, ktn(KS,3), ktn(KI,3));

	for (i=0; i<3;i++){
		kS(kK(row)[0])[i]= "test";
		kI(kK(row)[1])[i]= i;
}	
	
	result = k(c, "insert", ks((S) "heyo2"), row, (K) 0);
	  r0(result);
//	 k(c, "{[x] `heyo2 insert x}", row, (K) 0);
	printf("done: %d", i);                             
	  
}
char *saver= "save `heyo2";
k(c,saver, (K)0);
printf("wait 1 of 4"); getchar();
printf("wait 2 of 4"); getchar();
printf("wait 3 of 4"); getchar();
printf("wait 4 of 4"); getchar();
//k(c,"", (K)0);
kclose(c);
printf("done, press enter to finish"); getchar();
}
