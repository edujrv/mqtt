// #include <stdio.h>
// #include <wiringPi.h>

// #define TRIG_PIN 23  // Pin 16 (GPIO23)
// #define ECHO_PIN 24  //Pin 18 (GPIO24)

// float medir_distancia() {
//     // printf("------- 1 -------.\n");

//     digitalWrite(TRIG_PIN, LOW);
//     delayMicroseconds(2);
//     digitalWrite(TRIG_PIN, HIGH);

//     delayMicroseconds(10);
//     // printf("------- 2 -------.\n");

//     digitalWrite(TRIG_PIN, LOW);

//     // printf("------- 3 -------.\n");

//     long startTime = micros();
//     long now = micros();
// //     long timeout = 30000;
//     delayMicroseconds(5);

// //     while ((digitalRead(ECHO_PIN) == LOW) && (micros() - now < timeout)){
//     // while ((digitalRead(ECHO_PIN) == LOW)){
//     //     if ((micros() - now < timeout)){
//     //         printf("TIMEOUT");
//     //         break;
//     //     }
// //     }
//     while (digitalRead(ECHO_PIN) == LOW);

//     // printf("------- 4 -------.\n");
//     while (digitalRead(ECHO_PIN) == HIGH);
//     long travelTime = micros() - startTime;

//     // printf("------- 5 -------.\n");
//     // float distancia = travelTime / 58.0;  // Velocidad del sonido en cm/µs 
//     float distancia = travelTime * 34300 / 2;

//         // printf("\n DISTANCIA : \"%f\" \n", distancia);
//     return distancia;
// }

// float medir_distancia_mock() {
//     return 56.8;
// }


// void setup_distance_sensor() {
//         printf("INCIALIZANDO DISTANCE SENSOR.\n");
// //     if (wiringPiSetup() == -1) {
// //         printf("Error al inicializar WiringPi.\n");
// //         return;
// //     }
//     wiringPiSetupGpio();

//     pinMode(TRIG_PIN, OUTPUT);
//     pinMode(ECHO_PIN, INPUT);
//         printf("DISTANCE SENSOR INICIALIZADO.\n");

// }
// #include <stdio.h>
// #include <pigpio.h>

// #define TRIG_PIN 23
// #define ECHO_PIN 24


// void setup_distance_sensor() {
//         printf("INCIALIZANDO DISTANCE SENSOR.\n");
//     if (gpioInitialise() < 0)
//     {
//         printf("Error al inicializar GPIO\n");
//         return 1;
//     }

//     gpioSetMode(TRIG_PIN, PI_OUTPUT);
//     gpioSetMode(ECHO_PIN, PI_INPUT);
//         printf("DISTANCE SENSOR INICIALIZADO.\n");

// }

// float medir_distancia()
// {
    

//     // while (1)
//     // {
//         gpioWrite(TRIG_PIN, 0); // Establecer el pin TRIG_PIN a bajo
//         gpioDelay(500);

//         gpioWrite(TRIG_PIN, 1); // Generar pulso de trigger
//         gpioDelay(10);
//         gpioWrite(TRIG_PIN, 0);

//         while (gpioRead(ECHO_PIN) == 0)
//             ;

//         long startTime = gpioTick();

//         while (gpioRead(ECHO_PIN) == 1)
//             ;

//         long endTime = gpioTick();
//         long travelTime = endTime - startTime;

//         float distance = (travelTime / 1000000.0) * 34000 / 2;

//         printf("Distancia: %.2f cm\n", distance);

//         // gpioDelay(500);
//     // }

#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <sys/time.h>

#define TRIG_PIN 23  // Pin 16 (GPIO23)
#define ECHO_PIN 24  // Pin 18 (GPIO24)

void setup_distance_sensor() {
//    if (wiringPiSetup() == -1) {
//        printf("Error al inicializar WiringPi.\n");
//        return;
//    }
    wiringPiSetupGpio();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

//obtener_distancia
float medir_distancia() { 
    // Env      a un pulso de disparo al sensor
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

    // Calcula la duraci      n del pulso
    long duracion = (fin.tv_sec - inicio.tv_sec) * 1000000 + (fin.tv_usec - inicio.tv_usec);

    // Calcula la distancia (suponiendo una velocidad de sonido de 343 m/s)
    float distancia = duracion * 34300 / 2000000.0;

    return distancia;
}

//     // gpioTerminate();

//     return 0;
// }

