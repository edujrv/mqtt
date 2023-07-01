#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "mqtt_publisher.c"
#include "distance_sensor.c"
#include "temperature_sensor.c"
#include <signal.h>
#include <sys/time.h>

// Variables globales compartidas
float distance = 0.0;
float temperature = 0.0;
pthread_mutex_t distance_mutex;
pthread_mutex_t temperature_mutex;
pthread_mutex_t acquisition_completed;
pthread_cond_t measurement_cond;
// Estructura para pasar múltiples argumentos al hilo publish_thread
struct publish_thread_args {
    struct mqtt_client* client;
    int sockfd;
    pthread_t* client_daemon;
};

// Prototipos de funciones
void* publish_thread(void* arg);
void* measurement_thread(void* arg);
void measurement();


int main() {
    int sockfd;
    open_socket(&sockfd);

    struct mqtt_client client;
    connect_socket(&sockfd, &client);

    pthread_t client_daemon;
    start_thread(&sockfd, &client, &client_daemon);

    setup_distance_sensor();
    setup_temperature_sensor();

    // Inicializar los mutex y la variable de condición
    pthread_mutex_init(&distance_mutex, NULL);
    pthread_mutex_init(&temperature_mutex, NULL);
    pthread_mutex_init(&acquisition_completed, NULL);
    pthread_cond_init(&measurement_cond, NULL);
    // Crear hilos
    pthread_t publish_tid, measurement_tid;
    
    // Estructura para los argumentos del hilo publish_thread
    struct publish_thread_args publish_args;
    publish_args.client = &client;
    publish_args.sockfd = sockfd;
    publish_args.client_daemon = &client_daemon;
    
    pthread_create(&publish_tid, NULL, publish_thread, (void*)&publish_args);
    pthread_create(&measurement_tid, NULL, measurement_thread, NULL);

    // Esperar a que los hilos terminen
    pthread_join(publish_tid, NULL);
    pthread_join(measurement_tid, NULL);

    // Liberar recursos
    pthread_mutex_destroy(&distance_mutex);
    pthread_mutex_destroy(&temperature_mutex);
    pthread_cond_destroy(&measurement_cond);

    //Salgo del mqtt
    exit_socket(EXIT_SUCCESS, sockfd, &client_daemon);
    return 0;
}

void* publish_thread(void* arg) {
    struct publish_thread_args* args = (struct publish_thread_args*)arg;
    struct mqtt_client* client = args->client;
    int sockfd = args->sockfd;
    pthread_t* client_daemon = args->client_daemon;

    while (1) {
        char application_message[256];
        float d, t;

        pthread_mutex_lock(&acquisition_completed);

        pthread_mutex_lock(&distance_mutex);
        d = distance;
        pthread_mutex_unlock(&distance_mutex);

        pthread_mutex_lock(&temperature_mutex);
        t = temperature;
        pthread_mutex_unlock(&temperature_mutex);

        snprintf(application_message, sizeof(application_message), "{ \"distancia\": %f, \"temperatura\": %f }", d, t);
        publish(client, application_message);
        printf("\npublished : \"%s\" \n", application_message);

        pthread_mutex_unlock(&acquisition_completed);

        // Realizar check_error
        check_error(client, sockfd, client_daemon);
        sleep(2);
    }
    return NULL;
}
void* measurement_thread(void* arg) {
    struct itimerval timer;

    // Set up timer to call measurement() periodically
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000000;  // Call measurement() every 1 second
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000000;
    setitimer(ITIMER_REAL, &timer, NULL);

    while (1) {
        pthread_cond_wait(&measurement_cond, &acquisition_completed);
        measurement();
    }
    return NULL;
}

void measurement() {
    printf("measurement\n");
    pthread_mutex_lock(&temperature_mutex);
    temperature = obtener_temperatura();
    pthread_mutex_unlock(&temperature_mutex);

    pthread_mutex_lock(&distance_mutex);
    distance = obtener_distancia();
    pthread_mutex_unlock(&distance_mutex);

    pthread_cond_signal(&measurement_cond);
}