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

void nameGen (char *sym, char *date, char *tableName){
	char res [15];
        strcpy(res, sym);
	strcat(res, "_");
//	strcat(res, date);
	strcat(res,"2018"); // use this as example, because the hyphens in the API date are causing problems in KDB
	strcpy(tableName, res);
}

void saveGen (char *input, char *saveName){
	char res [70];
        strcpy(res, input);
	strcat(res, ":([]sym:`symbol$(); price:`float$(); time:`time$())");
	strcpy(saveName, res);
}




int main () {

	int c=khp("localhost", 5001);
	char *url = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=XMR&tsyms=CAD";
	char *priceS; char *date;
	float price;
	int timee=0; int lastTime;//needs to be milliseconds since midnight
	int go=0; int go2=0; int refresh = 5; char tableName[15]; char saveName[60];
	char *sym="XMR";	
	char *conversion= "CAD"; //not necessary to do this, since it's already set in the string var "url"
	
	CURL *curl;
	curl = curl_easy_init();
	json_object *obj;
	
	time_t t = time(NULL);
        struct tm tm = *localtime(&t);
	
while (go==0){
	
	curl_easy_setopt(curl, CURLOPT_URL, "http://worldclockapi.com/api/json/est/now");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
	curl_easy_perform(curl);
	curl_easy_reset(curl);
	obj= json_tokener_parse(data);
	json_object *dtime = json_object_object_get(obj, "currentDateTime");
	date= json_object_get_string(dtime);
	date [10]= '\0'; // I think this will be reight
	printf("check1: %s", date); getchar();
	nameGen (sym, date, tableName);// how to get date data into a string??
        printf("checking sym, date: %s, %s", sym, date); getchar();        
	printf("checking tableName: %s", tableName); getchar();
	saveGen(tableName, saveName);
        printf("checking saveName: %s", saveName); getchar();
	k(-c,saveName,(K)0);
	printf("date is: %s", date);
	getchar();
	int i;

	for (i=0; i<5 ; i++){

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
		printf("price is = %f\n", price); 

		K row = knk(3, ks((S)"XMR"), kf(price), kt(timee));
		k(c, "{[x] `XMR_2018 insert x}", row, (K)0);
		
		/*char saveFile [30];
		strcpy (saveFile,"save `");
		strcat (saveFile, tableName);
		k(c,saveFile,(K)0); // shouldnt do this each time.. look into this
		k(c,"",(K)0);*/
	sleep (refresh);
	}
	char saveFile [30];
	strcpy (saveFile, "save `");
	strcat (saveFile, tableName);
	k(c, saveFile, (K) 0);
	k(c, "", (K)0);
	printf("should be saved to: %s\n", saveFile); getchar();
}
return 0;

}




