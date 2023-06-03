UNAME = $(shell uname -o)

CC = gcc
CFLAGS = -Wextra -Wall -std=gnu99 -Iinclude -Wno-unused-parameter -Wno-unused-variable -Wno-duplicate-decl-specifier

ifeq ($(UNAME), Msys)
MSFLAGS = -lws2_32
endif

MQTT_C_SOURCES = src/libs/mqtt.c src/libs/mqtt_pal.c

MQTT_C_SOURCES2 = src/libs/mqtt.c src/libs/mqtt_pal.c 


# project: src/mqtt_publisher.c $(MQTT_C_SOURCES)
# 	$(CC) $(CFLAGS) $^ -lpthread $(MSFLAGS) -o $@

project: src/sensor.c $(MQTT_C_SOURCES2)
	$(CC) $(CFLAGS) $^ -lpthread $(MSFLAGS) -o $@