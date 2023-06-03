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