#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "mqtt_aws.h"
#include "json_parse.h"
#include "libcurl.h"
#include "decrypt.h"

int main()
{
       
	const char* j_msg;
	const char* parse_key;

       // mqtt_pub_sub();

	parse_key = json_parse();
	//printf("key :%s", parse_key);

	libcurl();
        
	decrypt_openssl(parse_key);


	
}
