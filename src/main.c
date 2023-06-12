#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "mqtt_publisher.c"
#include "distance_sensor.c"
#include "temperature_sensor.c"

// Variables globales compartidas
float distance = 0.0;
float temperature = 0.0;
pthread_mutex_t distance_mutex;
pthread_mutex_t temperature_mutex;
pthread_mutex_t acquisition_completed;
pthread_cond_t data_ready_cond;
// int acquisition_completed = 0;

// Estructura para pasar múltiples argumentos al hilo publish_thread
struct publish_thread_args {
    struct mqtt_client* client;
    int sockfd;
    pthread_t* client_daemon;
};

// Prototipos de funciones
void* publish_thread(void* arg);
// void* distance_thread(void* arg);
// void* temperature_thread(void* arg);

int main() {
    int sockfd;
    open_socket(&sockfd);

    struct mqtt_client client;
    connect_socket(&sockfd, &client);

    pthread_t client_daemon;
    start_thread(&sockfd, &client, &client_daemon);

    // Inicializar los mutex y la variable de condición
    int dis_mux = pthread_mutex_init(&distance_mutex, NULL);
    int temp_mux = pthread_mutex_init(&temperature_mutex, NULL);
    int adq_mux = pthread_mutex_init(&acquisition_completed, NULL);

    int data_mux = pthread_cond_init(&data_ready_cond, NULL);

    if (dis_mux != 0 || temp_mux != 0 || data_mux != 0 ){
        printf("ERROR AL INICIALIZAR MUTEX\n");

    }
    printf("DESPUES DE INICIALIZAR MUTEX\n");
    // Crear hilos
    pthread_t publish_tid, distance_tid, temperature_tid, measurement_tid;
    
    // Estructura para los argumentos del hilo publish_thread
    struct publish_thread_args publish_args;
    publish_args.client = &client;
    publish_args.sockfd = sockfd;
    publish_args.client_daemon = &client_daemon;
    
    pthread_create(&publish_tid, NULL, publish_thread, (void*)&publish_args);
    // pthread_create(&distance_tid, NULL, distance_thread, NULL);
    // pthread_create(&temperature_tid, NULL, temperature_thread, NULL);
    pthread_create(&measurement_tid, NULL, measurement_thread, NULL);

    
    printf("DESPUES DE INICIALIZAR hilos\n");

    // Esperar a que los hilos terminen
    pthread_join(publish_tid, NULL);
    pthread_join(distance_tid, NULL);
    pthread_join(temperature_tid, NULL);
    printf("DESPUES DE FINALIZAR HILOS\n");

    // Liberar recursos
    pthread_mutex_destroy(&distance_mutex);
    pthread_mutex_destroy(&temperature_mutex);
    pthread_cond_destroy(&data_ready_cond);
    printf("DESPUES DE FINALIZAR MUTEX\n");

    exit_example(EXIT_SUCCESS, sockfd, &client_daemon);
    return 0;
}

void* publish_thread(void* arg) {
    struct publish_thread_args* args = (struct publish_thread_args*)arg;
    struct mqtt_client* client = args->client;
    int sockfd = args->sockfd;
    pthread_t* client_daemon = args->client_daemon;
    printf("publish thread \n");

    while (1) {
        char application_message[256];
        float d, t;

        // Esperar a que los otros hilos completen sus tareas
        // pthread_mutex_lock(&distance_mutex);
        // pthread_mutex_lock(&temperature_mutex);
        pthread_mutex_lock(&acquisition_completed);

        // while (!acquisition_completed) {
        //     printf("while \n");
        //     // pthread_cond_wait(&data_ready_cond, &distance_mutex);
        //     printf("waiting cond \n");
        // }
        // acquisition_completed = 0;
        // pthread_mutex_unlock(&distance_mutex);
        // pthread_mutex_unlock(&temperature_mutex);

        pthread_mutex_lock(&distance_mutex);
        printf("publish mutex1 \n");

        d = distance;
        pthread_mutex_unlock(&distance_mutex);

        pthread_mutex_lock(&temperature_mutex);
        printf("publish mutex2 \n");

        t = temperature;
        pthread_mutex_unlock(&temperature_mutex);

        snprintf(application_message, sizeof(application_message), "{ \"distancia\": %f, \"temperatura\": %f }", d, t);

        publish(client, application_message);

        printf("\npublished : \"%s\" \n", application_message);
        pthread_mutex_unlock(&acquisition_completed);

        // Realizar check_error
        check_error(client, sockfd, client_daemon);
        printf("publish antes de dormir \n");
        sleep(2);
    }
    return NULL;
}

// void* distance_thread(void* arg) {

//     while (1) {
//         printf("distance thread \n");
//         float d;

//         // Simulación de obtención de la distancia
//         // d = obtener_distancia();

//         pthread_mutex_lock(&distance_mutex);
//         printf("distance mutex1 \n");
//         distance = obtener_distancia();
//         pthread_mutex_unlock(&distance_mutex);

//         pthread_mutex_lock(&temperature_mutex);
//         printf("distance mutex1 \n");
//         acquisition_completed = 1;
//         pthread_cond_broadcast(&data_ready_cond);
//         pthread_mutex_unlock(&temperature_mutex);

//         sleep(1);
//     }
//     return NULL;
// }

// void* temperature_thread(void* arg) {
//     while (1) {
//         float t;
//         printf("temperature thread \n");
//         // Simulación de obtención de la temperatura
//         // t = obtener_temperatura();
//         printf("temperature mutex0 \n");
//         pthread_mutex_lock(&temperature_mutex);
//         temperature = obtener_temperatura();
//         printf("temperature mutex1 \n");
//         pthread_mutex_unlock(&temperature_mutex);

//         pthread_mutex_lock(&distance_mutex);
//         acquisition_completed = 1;
//         printf("temperature mutex2 \n");
//         pthread_cond_broadcast(&data_ready_cond);
//         pthread_mutex_unlock(&distance_mutex);

//         sleep(1);
//     }
    void* measurement_thread(void* arg) {
    while (1) {
        float t;
        printf("temperature thread \n");
        // Simulación de obtención de la temperatura
        // t = obtener_temperatura();
        printf("temperature mutex0 \n");
        pthread_mutex_lock(&temperature_mutex);
        temperature = obtener_temperatura();
        printf("temperature mutex1 \n");
        pthread_mutex_unlock(&temperature_mutex);


        pthread_mutex_lock(&distance_mutex);
        printf("distance mutex1 \n");
        distance = obtener_distancia();
        pthread_mutex_unlock(&distance_mutex);


        // pthread_mutex_lock(&distance_mutex);
        // acquisition_completed = 1;
        // printf("temperature mutex2 \n");
        // pthread_cond_broadcast(&data_ready_cond);
        pthread_mutex_unlock(&acquisition_completed);

        sleep(1);
    }
    return NULL;
}