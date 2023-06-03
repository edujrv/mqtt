#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

#define TEMPERATURE_SENSOR_PATH "/sys/bus/w1/devices/28-*/w1_slave"  // Cambia esta ruta según tu configuración

float medir_temperatura() {
    FILE *fp;
    char path[100];
    char line[100];
    char *token;
    float temperatura = 0.0;

    // Buscar el archivo del sensor de temperatura
    strcpy(path, TEMPERATURE_SENSOR_PATH);
    fp = popen("ls /sys/bus/w1/devices/ | grep 28-", "r");
    if (fp == NULL) {
        perror("Error al buscar el sensor de temperatura");
        return temperatura;
    }

    if (fgets(line, sizeof(line), fp) != NULL) {
        strtok(line, "\n");
        strcat(path, line);
        strcat(path, "/w1_slave");
    } else {
        pclose(fp);
        return temperatura;
    }

    pclose(fp);

    // Leer el archivo del sensor de temperatura
    fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo del sensor de temperatura");
        return temperatura;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        token = strtok(line, "=");
        if (token != NULL && strcmp(token, "t") == 0) {
            token = strtok(NULL, "\n");
            temperatura = atof(token) / 1000.0;
            break;
        }
    }

    fclose(fp);
    return temperatura;
}
