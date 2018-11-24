#include "k.h"
#include <json/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>


char * data

void curl_write(void *ptr, site_t size, size_t nmemb, void *userp){
	data=ptr;
}

void checkF (char **p){
	int len = *p.strlen();
	int check=0;
	for (int i=0; i<len; i++){
		if (*p[i]=='.') {
			check++;
		}
	}
	if (check==0){
		char *temp [len+2];
		srtcpy (temp, *p);
		strcat (temp, ".0");
		*p= temp;
	}
}

int main () {

	int c=khp("localhost", 5001);
	char *url = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=XMR&tsyms=CAD";
	char *price;
	int go=0;
	int refresh = 10;
	k = (c, "MONERO:([]sym:`XMR`XMR; price:00.00 00.00; dtime:.z.z)"); //remember to disregard first two

	CURL *curl;
	curl = curl_easy_init();
	json_object *obj;

	while (go==0){

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
		curl_easy_perform(curl);
		curl_easy_reset(curl);

		obj= json_tokener_parse(data);
		json_object *yo1 = json_object_object_get(obj, "RAW");
		json_object *yo2 = json_object_object_get(yo1, "XMR");
		json_object *yo3 = json_object_object_get(yo2, "CAD");
		json_object *yo1 = json_object_object_get(yo3, "PRICE");

		price = json_object_get_string(yo1);
		
		checkF (&price);

		char *quer [30];
		srtcpy (quer, "MONERO insert (`XMR;");
		strcat (quer, price);
		strcat (quer, ";.z.z)");

		k(c, quer, (K)0)
		k(c,"save `MONERO",(K)0);
		k(c,"",(K)0);
	sleep (refresh);
	}
return 0;

}




