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
   - HOST: 172.18.0.2
   - PORT: 1883
   - topic: 'prueba'

Todos los puertos estan definidos al principio del archivo
```bash
    mqtt_publisher.c
```