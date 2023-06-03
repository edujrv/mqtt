#include <stdio.h>
#include <wiringPi.h>

#define TRIG_PIN 0  // Cambia estos valores según los pines GPIO que estés utilizando
#define ECHO_PIN 1

float medir_distancia() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    while (digitalRead(ECHO_PIN) == LOW);
    long startTime = micros();

    while (digitalRead(ECHO_PIN) == HIGH);
    long travelTime = micros() - startTime;

    float distancia = travelTime / 58.0;  // Velocidad del sonido en cm/µs
    return distancia;
}

float medir_distancia_mock() {
    return 56.8;
}


void setup_distance_sensor() {
    if (wiringPiSetup() == -1) {
        printf("Error al inicializar WiringPi.\n");
        return;
    }

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}