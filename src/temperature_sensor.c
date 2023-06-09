#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPiI2C.h>

#define BME280_ADDRESS 0x76 // Replace with your sensor's I2C address

// BME280 registers
#define BME280_REG_ID 0xD0
#define BME280_REG_CTRL_HUM 0xF2
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_TEMP_MSB 0xFA
#define BME280_REG_TEMP_LSB 0xFB
#define BME280_REG_TEMP_XLSB 0xFC

// Calibration data structure
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
} bme280_calib_data;

int16_t concat_bytes(uint8_t msb, uint8_t lsb) {
    return ((int16_t)msb << 8) | (int16_t)lsb;
}

int16_t concat_bytes_unsigned(uint8_t msb, uint8_t lsb) {
    return ((int16_t)((uint16_t)msb << 8) | (uint16_t)lsb);
}

int fd;
uint8_t id;
bme280_calib_data calib;

void setup_temperature_sensor(){
    fd = wiringPiI2CSetup(BME280_ADDRESS);
    if (fd < 0) {
        printf("Failed to open I2C device\n");
        return;
    }

    // Check the sensor ID
    id = wiringPiI2CReadReg8(fd, BME280_REG_ID);
    if (id != 0x60) {
        printf("Invalid sensor ID\n");
        return;
    }

    // Read calibration data
    calib.dig_T1 = wiringPiI2CReadReg16(fd, 0x88);
    calib.dig_T2 = wiringPiI2CReadReg16(fd, 0x8A);
    calib.dig_T3 = wiringPiI2CReadReg16(fd, 0x8C);

    // Set oversampling settings for temperature and humidity
    wiringPiI2CWriteReg8(fd, BME280_REG_CTRL_HUM, 0x01); // Humidity oversampling x1
    wiringPiI2CWriteReg8(fd, BME280_REG_CTRL_MEAS, 0x25); // Temperature oversampling x1, pressure oversampling x1, normal mode
}

float obtener_temperatura(){
        // Read temperature data
        uint8_t msb = wiringPiI2CReadReg8(fd, BME280_REG_TEMP_MSB);
        uint8_t lsb = wiringPiI2CReadReg8(fd, BME280_REG_TEMP_LSB);
        uint8_t xlsb = wiringPiI2CReadReg8(fd, BME280_REG_TEMP_XLSB);
        int32_t adc_T = (int32_t)((msb << 16) | (lsb << 8) | xlsb) >> 4;

        // Compensation algorithm
        int32_t var1, var2, T;
        var1 = ((((adc_T >> 3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
        var2 = (((((adc_T >> 4) - ((int32_t)calib.dig_T1)) * ((adc_T >> 4) - ((int32_t)calib.dig_T1))) >> 12) * ((int32_t)calib.dig_T3)) >> 14;
        T = var1 + var2;

        float temperature = (float)((T * 5 + 128) >> 8) / 100.0f; // Temperature in degrees Celsius
        return temperature;
}

// int main(void) {
//     setup_temperature_sensor();
//     while (1) {
//         float temperature = obtener_temperatura();
//         printf("Temperature: %.2f °C\n", temperature);
//         sleep(1); // Wait for 1 second before reading again
//     }

//     return 0;
// }

