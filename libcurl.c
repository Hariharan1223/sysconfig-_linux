#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>


void libcurl()
{

	CURL *curl;
	FILE *fp;
        int result;
	FILE *file_p;

        file_p = fopen("json_url.txt","r");
        if(file_p == NULL){
                printf("Unable to open the file\n");
                return;
        }
        fseek(file_p, 0, SEEK_END);
        long fsize = ftell(file_p);
        fseek(file_p, 0, SEEK_SET);

        if(file_p == NULL){
                printf("File cannot be open \n");

        }
        char *url;

        url = malloc(fsize);

        memset(url, 0, fsize);


        int rc = fread(url, 1, fsize, file_p);

        fclose(file_p);
	printf("url is %s\n",url);

	fp = fopen ("encrypt.txt","wb");

	curl =curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,url);
       
        curl_easy_setopt(curl,CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl,CURLOPT_FAILONERROR, 1L);

        result = curl_easy_perform(curl);

        if(result == CURLE_OK)
                printf("Download sucessfully \n");
        else
                printf("ERROR:%s\n",curl_easy_strerror(result));
        fclose(fp);

        curl_easy_cleanup(curl);
}
