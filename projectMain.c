#include "k.h"
#include <json/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

char * data;

void curl_write(void *ptr, site_t size, size_t nmemb, void *userp){
	data=ptr;
}

char *nameGen (char *sym, char *date){
	//int len = strlen(one) + strlen(two);
	char *res [70];
	strcpy(res, sym);
	strcat(res, ".");
	strcat(res, date);
	strcat(res, ":([]sym:`symbol$(); price:`float$(); time:`time$())"
	return res;
}

int main () {

	int c=khp("localhost", 5001);
	char *url = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=XMR&tsyms=CAD";
	char *priceS; char *date; char *tableName
	float price;
	int timee=0; int lastTime;//needs to be milliseconds since midnight
	int go=0; int refresh = 10;
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
	tableName = nameGen (sym, date);// how to get date data into a string??
	
	while (go==0){

		lastTime= timee;
		t = time(NULL);
        tm = *localtime(&t);
		
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
		curl_easy_perform(curl);
		curl_easy_reset(curl);
		
		timee= (((tm.tm_hour*60)+tm.tm_min)*60+tm.tm_sec)*1000; // check this one to make sure its right
		if (timee<lastTime) { break;} // if time resets, it is a new day
		
		obj= json_tokener_parse(data);
		json_object *yo1 = json_object_object_get(obj, "RAW");
		json_object *yo2 = json_object_object_get(yo1, sym);
		json_object *yo3 = json_object_object_get(yo2, conversion);
		json_object *yo1 = json_object_object_get(yo3, "PRICE");

		priceS = json_object_get_string(yo1);
		price= atof(priceS);  // does this work?

		K row = knk(3, ks((S)"XMR"), kf(price), ki(timee));
		k(c, "insert",ks((S) tableName), row, (K)0);
		
		k(c,"save `MONERO",(K)0); // these two lines aint good
		k(c,"",(K)0);
	sleep (refresh);
	}
}
return 0;

}




