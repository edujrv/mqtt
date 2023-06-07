// #include <stdio.h>
// #include <wiringPi.h>

// #define TRIG_PIN 23  // Pin 16 (GPIO23)
// #define ECHO_PIN 24  //Pin 18 (GPIO24)

// float medir_distancia() {
//     printf("------- 1 -------.\n");

//     digitalWrite(TRIG_PIN, LOW);
//     delayMicroseconds(2);
//     digitalWrite(TRIG_PIN, HIGH);

//     delayMicroseconds(10);
//     printf("------- 2 -------.\n");

//     digitalWrite(TRIG_PIN, LOW);

//     printf("------- 3 -------.\n");

//     long now = micros();
//     long timeout = 30000;

//     while ((digitalRead(ECHO_PIN) == LOW) && (micros() - now < timeout)){
//     // while ((digitalRead(ECHO_PIN) == LOW)){
//     //     if ((micros() - now < timeout)){
//     //         printf("TIMEOUT");
//     //         break;
//     //     }
//     }
//     long startTime = micros();

//     printf("------- 4 -------.\n");
//     while (digitalRead(ECHO_PIN) == HIGH);
//     long travelTime = micros() - startTime;

//     printf("------- 5 -------.\n");
//     float distancia = travelTime / 58.0;  // Velocidad del sonido en cm/µs
//         printf("\n DISTANCIA : \"%f\" \n", distancia);
//     return distancia;
// }

// float medir_distancia_mock() {
//     return 56.8;
// }


// void setup_distance_sensor() {
//         printf("INCIALIZANDO DISTANCE SENSOR.\n");
//     if (wiringPiSetup() == -1) {
//         printf("Error al inicializar WiringPi.\n");
//         return;
//     }

//     pinMode(TRIG_PIN, OUTPUT);
//     pinMode(ECHO_PIN, INPUT);
//         printf("DISTANCE SENSOR INICIALIZADO.\n");

// }

#include <stdio.h>
#include <wiringPi.h>

#define TRIG_PIN 4  // GPIO pin 4
#define ECHO_PIN 5  // GPIO pin 5

void setup_distance_sensor() {
    wiringPiSetup();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

float medir_distancia() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    unsigned int timeout = 5000;  // Timeout in microseconds
    unsigned int pulseStartTime, pulseEndTime;
    float distance;

    while (digitalRead(ECHO_PIN) == LOW) {
        if (--timeout == 0)
            return -1.0;  // Return -1 if measurement timed out
    }

    pulseStartTime = micros();

    while (digitalRead(ECHO_PIN) == HIGH) {
        if (--timeout == 0)
            return -1.0;  // Return -1 if measurement timed out
    }

    pulseEndTime = micros();

    unsigned int pulseDuration = pulseEndTime - pulseStartTime;
    distance = pulseDuration / 58.0;  // Divide by 58 to get distance in centimeters

    return distance;
}

// int main() {
//     setup();

//     while (1) {
//         float distance = measureDistance();

//         if (distance != -1.0)
//             printf("Distance: %.2f cm\n", distance);
//         else
//             printf("Measurement timed out.\n");

//         delay(1000);  // Delay 1 second before the next measurement
//     }

//     return 0;
// }
