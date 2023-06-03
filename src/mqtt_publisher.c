#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqtt.h>
#include "templates/posix_sockets.h"

// #define addr "172.18.0.2"
#define addr "192.168.192.1"
#define port "1883"
#define topic "Mediciones"


void publish_callback(void** unused, struct mqtt_response_publish *published);
void* client_refresher(void* client);
void exit_example(int status, int sockfd, pthread_t *client_daemon);

void open_socket(int* sockfd);
void connect_socket(int* sockfd, struct mqtt_client* client);
void start_thread(int* sockfd,  struct mqtt_client* client, pthread_t* client_daemon);
void publish(struct mqtt_client* client, char application_message[256]);
void check_error( struct mqtt_client* client, int sockfd, pthread_t* client_daemon);

int prueba ();


// struct mqtt_client client;
uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */

void open_socket(int* sockfd){
    /* open the non-blocking TCP socket (connecting to the broker) */
    *sockfd = open_nb_socket(addr, port);
    if ((*sockfd) == -1) {
        perror("Failed to open socket: ");
        exit_example(EXIT_FAILURE, *sockfd, NULL);
    }
}

// void connect_socket(int* sockfd, struct mqtt_client* client, uint8_t* sendbuf, uint8_t* recvbuf){
void connect_socket(int* sockfd, struct mqtt_client* client){
    // uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
    // uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */
    mqtt_init(client, *sockfd, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), publish_callback);
    mqtt_connect(client, "publishing_client", NULL, NULL, 0, NULL, NULL, 0, 400);
}

void start_thread(int* sockfd,  struct mqtt_client* client, pthread_t* client_daemon){
    /* start a thread to refresh the client (handle egress and ingree client traffic) */
    if(pthread_create(client_daemon, NULL, client_refresher, client)) {
        fprintf(stderr, "Failed to start client daemon.\n");
        exit_example(EXIT_FAILURE, *sockfd, NULL);
    }
}

// void publish(char data[], struct mqtt_client* client) {
void publish(struct mqtt_client* client, char application_message[256]) {
    
        // time_t timer;
        // time(&timer);
        // struct tm* tm_info = localtime(&timer);
        // char timebuf[26];
        // strftime(timebuf, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        // /* print a message */
        // char application_message[256];
        // snprintf(application_message, sizeof(application_message), "The time is %s", timebuf);
        // printf("published : \"%s\" \n", application_message);

        /* publish the time */
        mqtt_publish(client, topic, application_message, strlen(application_message) + 1, MQTT_PUBLISH_QOS_0);
}


void check_error( struct mqtt_client* client, int sockfd, pthread_t *client_daemon)
{
    if ((*client).error != MQTT_OK) {
        fprintf(stderr, "error: %s\n", mqtt_error_str((*client).error));
        exit_example(EXIT_FAILURE, sockfd, client_daemon);
    }
}

void exit_example(int status, int sockfd, pthread_t *client_daemon)
{
    if (sockfd != -1) close(sockfd);
    if ((client_daemon) != NULL) pthread_cancel(*client_daemon);
    exit(status);
}
void publish_callback(void** unused, struct mqtt_response_publish *published) 
{
    /* not used in this example */
}
void* client_refresher(void* client)
{
    while(1) 
    {
        mqtt_sync((struct mqtt_client*) client);
        usleep(100000U);
    }
    return NULL;
}