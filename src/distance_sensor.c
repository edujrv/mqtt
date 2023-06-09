#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/time.h>

#define TRIG_PIN 23  // Pin 16 (GPIO23)
#define ECHO_PIN 24  // Pin 18 (GPIO24)

void setup_distance_sensor() {
    wiringPiSetupGpio();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

float obtener_distancia() { 
    // Envia un pulso de disparo al sensor
    digitalWrite(TRIG_PIN, HIGH);
    usleep(10);
    digitalWrite(TRIG_PIN, LOW);

    // Espera a que el pin ECHO se active
    struct timeval inicio, fin;
    while (digitalRead(ECHO_PIN) == LOW) {
        gettimeofday(&inicio, NULL);
    }

    // Espera a que el pin ECHO se desactive
    while (digitalRead(ECHO_PIN) == HIGH) {
        gettimeofday(&fin, NULL);
    }

    // Calcula la duracion del pulso
    long duracion = (fin.tv_sec - inicio.tv_sec) * 1000000 + (fin.tv_usec - inicio.tv_usec);

    // Calcula la distancia (suponiendo una velocidad de sonido de 343 m/s)
    float distancia = duracion * 34300 / 2000000.0;

    return distancia;
}
