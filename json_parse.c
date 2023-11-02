#include<stdio.h>
#include<json.h>

const char* json_parse()
{

	char parse_buffer[512];
	char* url;
	struct json_object *obj =json_object_from_file("json_message.json");

        struct json_object *location;
        struct json_object *obj1=NULL;
        struct json_object *host;
        struct json_object *protocol;
        struct json_object *path;
        struct json_object *ptkey;

        json_object_object_get_ex(obj, "location", &obj1);
        json_object_object_get_ex(obj1, "protocol", &location);
        json_object_object_get_ex(obj1, "host", &protocol);
        json_object_object_get_ex(obj1, "path", &host);

        sprintf(parse_buffer,"https://%s/%s\n", json_object_get_string(protocol),json_object_get_string(host));
        printf("%s",parse_buffer);
	FILE *pr;
        pr = fopen("json_url.txt", "w");
        if(pr == NULL){
                printf("download failed \n");
        }
        fprintf(pr,"%s",parse_buffer);

        fclose(pr);

        json_object_object_get_ex(obj, "ptkey", &ptkey);
	const char* parse_out = json_object_get_string(ptkey);
	printf("key: %s\n", parse_out);
	FILE *pr;
        pr = fopen("json_key.txt", "w");
        if(pr == NULL){
                printf("download failed \n");
        }
        fprintf(pr,"%s",json_object_get_string(ptkey));

        fclose(pr);


	return pr;
}

