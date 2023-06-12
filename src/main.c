#include <stdio.h>
#include "mqtt_publisher.c" 
#include "distance_sensor.c"
#include "temperature_sensor.c"


void setup_distance_sensor_main();


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
    printf("ready to begin publishing the measurement.\n");
    
    //TODO: UNCOMMENT THIS

    setup_distance_sensor();
    setup_temperature_sensor();

    while (1) {
        float d, t;

        d = obtener_distancia();
        t = obtener_temperatura();

        char application_message[256];
        // snprintf(application_message, sizeof(application_message), "Distancia: %f\nTemperatura: %f", d, t);
        snprintf(application_message, sizeof(application_message), "{ \"distancia\": %f, \"temperatura\": %f }", d, t);

        publish(&client, application_message);

        printf("\npublished : \"%s\" \n", application_message);
        check_error(&client, sockfd, &client_daemon);
        sleep(2);
    }

    /* disconnect */
    printf("\n disconnecting from %s\n", addr);
    sleep(5);
    /* exit */ 
    exit_example(EXIT_SUCCESS, sockfd, &client_daemon);
    return 0;
}