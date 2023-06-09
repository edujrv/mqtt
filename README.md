## Requerimientos
### WiringPi
Update your system's package list:
```bash
    $ sudo apt-get update
``` 
Install the necessary build tools:
```bash
    $ sudo apt-get install build-essential
``` 
Clone the WiringPi repository:
```bash
    $ git clone https://github.com/WiringPi/WiringPi
``` 
Navigate to the WiringPi directory:
```bash
    $ cd WiringPi
``` 
Build and install WiringPi:
```bash
    $ ./build
``` 

## Building the project
Para compilar:
```bash
    $ make project
``` 
Se creara el archivo ejecutable del proyecto, y para correrlo:
```bash
    $ ./project
```

Cabe aclarar que el projecto publica en:
   - HOST: 192.168.192.1
   - PORT: 1883
   - topic: 'Mediciones'

Todos los datos estan definidos al principio del archivo
```bash
    mqtt_publisher.c
```
