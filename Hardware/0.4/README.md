# Placa Master

En esta imagen se puede observar la placa principal del proyecto, la cual cumple el rol de recibir y confirmar este recibimiento los datos provenientes del broquer mqtt y encender o no cada uno de los tomas en consecuencia.
Principalmente está conformada por:
* **El microcontrolador Esp32:** Encargado de la comunicación con el broker mqtt y el proceso lógico en consecuencia.
* **Los Relay:** Principales responsables del encendido y/o apagado de los tomas del proyecto.

![image](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/blob/main/Hardware/0.4/Imagen%203d%20con%20esp.jpg)


## Funcionamiento:

Como se ve en el esquemático, la placa se separa en 4 partes:

### Esp32:

El cual permite la comunicacíon entre el broker mqtt y el la placa. Siendo el encargado de indicarle al resto de la placa las acciones a realizar.

### Relay:

Aqui se utiliza la señal generada por el microcontrolador para accionar los relay y encender o apagar cada tomacorriete. 
Para esto se utilizan 2 componentes cruciales:
* **El octoacoplador:** Quien separa las tenciones del microcontrolador de las de actuación del relay.
* **El transistor:** Quien permite accionar el relay utilizando la señal del octoacoblador:


### Alimentación:
Aqui se utiliza una fuente switchin generica de 5v 700mA para poder alimentar el proyecto utilizando directamente la red doméstica.

### Pulsador de configuracion Wifi:
Aquí se ubica una coneccion la cual permitirá comunicar la placa master con el periferico restante.
