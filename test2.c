#include"k.h"
#include<stdlib.h>

int main(){
  int c=khp("localhost",5001);                         //connect
 // k(c,"heyo:([]name:`symbol$(); num:`float$(); timeF:`time$())",(K)0);//insert
    k(c,"heyo:([]name:`symbol$(); num:`int$())",(K)0);//insert
//  k(c,"heyo:([]name:`Time`Ross; num:22 13)",(K)0);//insert
//  k(c,"save `heyo",(K)0);//insert
  int i;
  for (i=0; i<4 ; i++){
//	K row= knk(3, ks((S) "NewGuy"), kf(20.00), kt(100000));
	K row= knk(2, ks((S) "NewGuy"), ki(i));
//	k(c, "`heyo insert (`niggerJim; 22)", (K)0);
      k(c, "{[x] `heyo insert x}", row, (K) 0);
	//k(c,"",(K)0); // flush 
	printf("done: %d", i);                             
	  
}
k(c, "save `heyo", (K)0);
k(c, "", (K)0);
printf("done, press enter to finish"); getchar();
}
