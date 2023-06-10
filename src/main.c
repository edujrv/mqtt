#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "mqtt_publisher.c"
#include "distance_sensor.c"
#include "temperature_sensor.c"

// Para que sirve ??
void setup_distance_sensor_main();

// Variables globales compartidas
float distance = 0.0;
float temperature = 0.0;
pthread_mutex_t data_mutex;
pthread_cond_t data_ready_cond;
int acquisition_completed = 0;

// Estructura para pasar múltiples argumentos al hilo publish_thread
struct publish_thread_args
{
    struct mqtt_client *client;
    int sockfd;
    pthread_t *client_daemon;
};

// Hilos
void *publish_thread(void *arg);
void *acquisition_thread(void *arg);

int main()
{
    int sockfd;
    open_socket(&sockfd);

    struct mqtt_client client;
    connect_socket(&sockfd, &client);

    pthread_t client_daemon;
    start_thread(&sockfd, &client, &client_daemon);

    // start publishing the time
    printf("ready to begin publishing the time.\n");

    // Inicializar los sensores
    setup_distance_sensor();
    setup_temperature_sensor();

    // Inicializar el mutex y la variable de condición
    pthread_mutex_init(&data_mutex, NULL);
    pthread_cond_init(&data_ready_cond, NULL);

    // Crear hilos
    pthread_t publish_tid, acquisition_tid;

    // Estructura para los argumentos del hilo publish_thread
    struct publish_thread_args publish_args;
    publish_args.client = &client;
    publish_args.sockfd = sockfd;
    publish_args.client_daemon = &client_daemon;

    pthread_create(&publish_tid, NULL, publish_thread, (void *)&publish_args);
    pthread_create(&acquisition_tid, NULL, acquisition_thread, NULL);

    // Esperar a que los hilos terminen
    pthread_join(publish_tid, NULL);
    pthread_join(acquisition_tid, NULL);

    // Liberar recursos
    pthread_mutex_destroy(&data_mutex);
    pthread_cond_destroy(&data_ready_cond);
    exit_example(EXIT_SUCCESS, sockfd, &client_daemon);
    return 0;
}

void *publish_thread(void *arg)
{
    struct publish_thread_args *args = (struct publish_thread_args *)arg;
    struct mqtt_client *client = args->client;
    int sockfd = args->sockfd;
    pthread_t *client_daemon = args->client_daemon;

    while (1)
    {
        char application_message[256];
        float d, t;

        // Esperar a que los otros hilos completen sus tareas
        pthread_mutex_lock(&data_mutex);
        while (!acquisition_completed)
        {
            pthread_cond_wait(&data_ready_cond, &data_mutex);
        }
        acquisition_completed = 0;
        pthread_mutex_unlock(&data_mutex);

        pthread_mutex_lock(&data_mutex);
        d = distance;
        t = temperature;
        pthread_mutex_unlock(&data_mutex);

        snprintf(application_message, sizeof(application_message), "{ \"distancia\": %f, \"temperatura\": %f }", d, t);

        publish(client, application_message);

        printf("\npublished : \"%s\" \n", application_message);

        // Realizar check_error
        check_error(client, sockfd, client_daemon);

        sleep(2);
    }
    return NULL;
}

void *acquisition_thread(void *arg)
{
    while (1)
    {
        float d, t;

        // Simulación de obtención de datos de los sensores
        d = obtener_distancia();
        t = obtener_temperatura();

        pthread_mutex_lock(&data_mutex);
        distance = d;
        temperature = t;
        acquisition_completed = 1;
        pthread_cond_broadcast(&data_ready_cond);
        pthread_mutex_unlock(&data_mutex);

        sleep(1);
    }
    return NULL;
}