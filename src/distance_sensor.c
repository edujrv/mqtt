#include <stdio.h>
#include <wiringPi.h>

#define TRIG_PIN 18  // Pin 12 (GPIO18)
#define ECHO_PIN 24  //Pin 18 (GPIO24)

float medir_distancia() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    while (digitalRead(ECHO_PIN) == LOW);
    long startTime = micros();

    while (digitalRead(ECHO_PIN) == HIGH);
    long travelTime = micros() - startTime;

    float distancia = travelTime / 58.0;  // Velocidad del sonido en cm/µs
        printf("\n DISTANCIA : \"%f\" \n", distancia);
    return distancia;
}

float medir_distancia_mock() {
    return 56.8;
}


void setup_distance_sensor() {
        printf("INCIALIZANDO DISTANCE SENSOR.\n");
    if (wiringPiSetup() == -1) {
        printf("Error al inicializar WiringPi.\n");
        return;
    }

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
        printf("DISTANCE SENSOR INICIALIZADO.\n");

}