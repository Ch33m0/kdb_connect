#include "k.h"
#include <json/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

char * data;

void curl_write(void *ptr, size_t size, size_t nmemb, void *userp){
	data=ptr;
}

void nameGen (char *sym, char *year, char *month, char *day, char *tableName){
	char res [15];
        strcpy(res, sym);
	strcat(res, "_");
	strcat(res, year);
	strcat(res, "_");
	strcat(res, month);
	strcat(res, "_");
	strcat(res, day);
	strcpy(tableName, res);
}

void saveGen (char *input, char *saveName){
	char res [60];
        strcpy(res, input);
	strcat(res, ":([]sym:`symbol$(); price:`float$(); time:`time$())");
	strcpy(saveName, res);
}


int main () {

	int c=khp("localhost", 5001);
	char *url = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=XMR&tsyms=CAD";
	char *priceS; char year[6]; char month[3]; char day[3];
	float price; int yearr;
	int timee=0; int lastTime;//needs to be milliseconds since midnight for KDB+
	int go=0; int go2=0; int refresh = 2; char tableName[15]; char saveName[60]; char tableNameCpy[15];
	char *sym="XMR";	
	char *conversion= "CAD"; //not necessary to do this, since it's already set in the string var "url"
	
	CURL *curl;
	curl = curl_easy_init();
	json_object *obj;
	
	time_t t = time(NULL);
        struct tm tm = *localtime(&t);
	
	K result; K row;
	
while (go==0){
	
	t = time(NULL);
        tm = *localtime(&t);
	yearr= tm.tm_year; yearr+=1900; printf("checking integer year: %d\n", yearr); getchar();
	sprintf(year, "%d", yearr); sprintf(month, "%d", tm.tm_mon);  sprintf(day, "%d", tm.tm_mday); 
	printf("SYSTEM: year, month, day: %d, %d, %d\n", tm.tm_year, tm.tm_mon, tm.tm_mday);
	printf("STRING_CONV: year, month, day: %s, %s, %s\n", year, month, day);
	nameGen (sym, year, month, day, tableName);      
	strcpy(tableNameCpy, tableName);
	printf("checking tableName: %s\n", tableName); 
	saveGen(tableName, saveName);
        printf("checking saveName: %s\n", saveName); 
	result= k(c,saveName,(K)0);
 printf("checking response from q: %d\n", result->t);
   r0(result);
	char saveFile [30]; strcpy (saveFile, "save`"); strcat (saveFile, tableNameCpy);
	printf("what is tableNameCpy rn?: %s\n", tableNameCpy);
	printf("checking saveFile: %send\n", saveFile); getchar();
	result= k(c, saveFile, (K)0);	
 printf("checking response from q: %d\n", result->t);
	r0(result);
int saveRate=0;

	while (go==0){
		
		lastTime= timee;
		t = time(NULL);
     	        tm = *localtime(&t);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
		curl_easy_perform(curl);
		curl_easy_reset(curl);
		
		timee= (((tm.tm_hour*60)+tm.tm_min)*60+tm.tm_sec)*1000; // check this one to make sure its right
		printf("system time (hour) is: %d\n", tm.tm_hour); 
		if (timee<lastTime) { break;} // if time resets, it is a new day
		printf("timee= %d\n", timee);		
		
		obj= json_tokener_parse(data);
		json_object *yo1 = json_object_object_get(obj, "RAW");
		json_object *yo2 = json_object_object_get(yo1, sym);
		json_object *yo3 = json_object_object_get(yo2, conversion);
		json_object *yo = json_object_object_get(yo3, "PRICE");

		priceS = json_object_get_string(yo);
		price= atof(priceS);  // does this work?
		printf("sym, price, time = %s, %f, %d\n", sym, price, timee); 

		row = knk(3, ks((S) sym), kf(price), kt(timee));
		result = k(c, "insert", ks((S) tableNameCpy), row, (K) 0);

 printf("checking response from q: %d\n", result->t);
		r0(result);		

		
/*		if (saveRate==3){
			result=k(c, saveFile, (K) 0); r0(result);
 printf("checking response from q: %d\n", result->t);
			printf("should be saved to: %s\n", saveFile); getchar();	
			saveRate=0;
		}*/
		saveRate++;
		sleep (refresh);
	}
	
	k(c, saveFile, (K) 0);
//	k(c, "", (K)0);
	kclose(c);
	printf("done!\n");
}
return 0;

}
