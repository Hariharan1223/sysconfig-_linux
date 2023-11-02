#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mosquitto.h>
#include <json-c/json.h>

#define SUB_TOPIC       "config/JSHxxxxxxxx"
#define PUB_TOPIC       "request_sysconfig"
#define PUB_MESSAGE     "{\"did\": \"JSHxxxxxxx\"}"
#define HOST            "axxxxxxxxxxxxxxxxxx.amazonaws.com"
#define PORT             xxx

#define MOSQ_CA_FILE    "./certs/ca.pem"
#define MOSQ_CERT_FILE  "./certs/client.crt"
#define MOSQ_KEY_FILE   "./certs/client.key"

static void on_connect(struct mosquitto *mosq, void *obj, int rc) {

        if(rc) {
                printf("Error with result code %s: %d  \n", __func__, rc);
                exit(-1);
        }
}
static void on_disconnect(struct mosquitto *mosq, void *obj, int rc) {

        if(rc) {
                printf("Error with result code %s: %d\n",  __func__, rc);
                exit(-1);
        }
}
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {

        char *json = (char *) msg->payload;
        struct json_object *j = json_tokener_parse(json);
     const char	*m =json_object_to_json_string_ext(j, JSON_C_TO_STRING_PRETTY);
        printf("payload: %s \n",m);

	FILE *p;
        p = fopen("json_message.json","w");
        if(p == NULL){
                printf("download failed \n");
        }
        fprintf(p,"%s",json_object_to_json_string_ext(j, JSON_C_TO_STRING_PRETTY));
        fclose(p);	
}
void mqtt_pub_sub()
{
	int rc;
        struct mosquitto * mosq = NULL;
        char msg[512] = PUB_MESSAGE ;

        mosquitto_lib_init();

        mosq = mosquitto_new(NULL, true, NULL);

        mosquitto_connect_callback_set(mosq, on_connect);
        mosquitto_message_callback_set(mosq, on_message);
        mosquitto_disconnect_callback_set(mosq, on_disconnect);

        rc = mosquitto_tls_set(mosq, MOSQ_CA_FILE, NULL, MOSQ_CERT_FILE, MOSQ_KEY_FILE, NULL);
        if (rc != MOSQ_ERR_SUCCESS) {
                printf("%d",rc);
                fprintf(stderr, "Publish: Unable to set TLS: %s\n", strerror(errno));
                exit(-1);
        }

        rc = mosquitto_tls_opts_set(mosq, 1, NULL, NULL);
        if (rc != MOSQ_ERR_SUCCESS) {
                fprintf(stderr, "Publish: Unable to set TLS opts: %s\n", strerror(errno));
                exit(-1);
        }

        rc = mosquitto_connect(mosq, HOST, PORT, 10);
        if(rc) {
                printf("Could not connect to Broker with return code %d\n", rc);
                exit(-1);
        }
	rc =  mosquitto_subscribe(mosq, NULL, SUB_TOPIC, 1);
        if(rc){
                printf("Unable to subscribe topic (%d)\n", rc);
                exit(-1);
       }
        rc = mosquitto_publish(mosq, NULL, PUB_TOPIC,strlen(msg), msg , 1, false);
        mosquitto_loop_start(mosq);
                if(rc){
                        printf("Unable to publish to the topic (%d)\n", rc);
                        exit(-1);
        }
		
		sleep(10);
		
	mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);

        mosquitto_lib_cleanup();
}        
