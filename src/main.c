#include <stdio.h>
#include "mqtt_publisher.c" // Include the file containing the prueba() function

// char msg_to_publish();


int main() {
    int sockfd;
    /* open the non-blocking TCP socket (connecting to the broker) */
    open_socket(&sockfd);

    struct mqtt_client client;
    connect_socket(&sockfd, &client);
    
    /* start a thread to refresh the client (handle egress and ingree client traffic) */
    pthread_t client_daemon;
    start_thread(&sockfd, &client, &client_daemon);

    /* start publishing the time */
    printf("ready to begin publishing the time.\n");
  
    while (1) {
        
        time_t timer;
        time(&timer);
        struct tm* tm_info = localtime(&timer);
        char timebuf[26];
        strftime(timebuf, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        /* print a message */
        char application_message[256];
        snprintf(application_message, sizeof(application_message), "The time is %s", timebuf);


        publish(&client, application_message);

        printf("published : \"%s\" \n", application_message);
        check_error(&client, sockfd, &client_daemon);
        sleep(3);
        // i++;
    }

    /* disconnect */
    printf("\n disconnecting from %s\n", addr);
    sleep(1);
    /* exit */ 
    exit_example(EXIT_SUCCESS, sockfd, &client_daemon);
    return 0;
}
