#  Zapatilla Electrónica Remota (Z.E.R)
![zer_3](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/assets/105557226/2e0b8db4-eaf7-421f-905a-5cf753c8b182)

# Descripción del proyecto
Este repositorio se dedica a documentar y recopilar el desarrollo de **Z.E.R**; un proyecto de automatizacion IoT desarrollado por Montial Octavio, Pierini Franco, Vallone Fabrizio, Sturm Rodrigo y Zuñiga Juan Jose.


Z.E.R es una zapatilla eléctrica que fue diseñada para activar y desactivar sus tomacorrientes inalámbricamente mediante nuestra aplicación móvil a través del uso de comunicación MQTT.
Decidimos implementar el protocolo de comunicacion MQTT debido a su sensillo uso y accecible conectividad WiFi con el microcontrolador ESP32.
Gracias a este proyecto, ampliamos nuestro conocimiento en diferentes areas como en el **desarrollo de Hardware**, **diseño de PCBs**, **desarrollo de Software (JavaScript - C++)** y **diseño 3D (SolidWork)**.

# Caracteristicas de Z.E.R
* **Microcontrolador ESP32** para hacer posible la conectividad IoT y un alto nivel de procesamiento.
* **Protocolo de comunicación MQTT** para la transmisión de datos a tiempo real.
* **Aplicación móvil** para una interfaz gráfica de usuario sencilla y accesible.
*  **Aprovisionamiento WiFi** para la conexión automática a redes WiFi.
* **Almacenamiento no volátil (NVS)** para guardar información temporalmente en la memoria flash del microcontrolador.
* **Sistema embebido unificado** para mayor ahorro de espacio y material.

#  Estructura del Repositorio
* **/Software**, se hayan las carpetas y los archivos de la programación móvil y del sistema embebido.
* **/Hardware**, se haya los recursos para observar el progreso del diseño del PCB.
* **/Diseño 3D**, se encuentran los archivos .STL  utilizados en el proyecto.

# Archivos Multimedia

## Fotos
### Aplicacion Movil

Aqui se muestra la estructura de la app. En la seccion superior puede verse el nombre del dispositivo, mientras que en el resto de la pantalla se situa la informacion de los diferentes tomas. Se tienen los siguientes elementos para controlar cada uno de los tomacorriente: 

* **Botones de encender y apagar**
* **Tabla indicativa del estado del toma** (Su color y mensaje cambia depende el estado).

![image](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/assets/133114947/24666520-1219-45d1-9a69-8cec763d5a1d)

### Diagramas de conexion con el broker

En este diagrama se ven las conexiones entre el broker y los nodos de la aplicacion movil y el microcontrolador. El broker MQTT es un intermediario en la comunicacion entre la aplicacion movil y el ESP32. Para que este protocolo funcione, ambas partes deben estar conectadas a una red WiFi.

![zer_4](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/assets/105557226/a6852da1-2f48-4398-a0bb-47121b24b101)

### Interfaz grafica de conexiones en el broker

En esta imagen se ven representadas las conexiones que se realizan al usar el proyecto. El broker [Shiftr](https://www.shiftr.io/) permite ver un diagrama de conexiones a tiempo real de los clientes y los topicos. Por un lado, el "esp32-client" representa al microcontrolador de la zapatilla, el cual se encarga de recibir la informacion enviada por parte de la app. Esta ultima se ve en el otro extremo de la conexion. Por ultimo, se encuentran los "topicos" en donde se almacenan los mensajes enviados desde la app. Estos serian los tomas, y pueden adoptar un mensaje de "encendido" o "apagado".

![image](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/assets/133114947/0cc4edf8-5742-42b1-85bf-aca77e879125)


### Placa Master

En esta imagen se puede observar la placa principal del proyecto, la cual cumple el rol de recibir y confirmar este recibimiento los datos provenientes del broquer mqtt y encender o no cada uno de los tomas en consecuencia.
Principalmente está conformada por:
* **El microcontrolador Esp32:** Encargado de la comunicación con el broker mqtt y el proceso lógico en consecuencia
* **Los Relay:** Principales responsables del encendido y/o apagado de los tomas del proyecto.

![image](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/blob/main/Hardware/0.4/Imagen%203d%20con%20esp.jpg)
[image](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/tree/main/Hardware/0.4)

### Placa de Periféricos:

En esta imagen se puede observar la placa secundaria del proyecto, la cual cumple el rol de mostrarle al usuario de forma visual el estado de conexión del dispositivo y una ruta de fácil acceso para poder reconfigurar este (conectar a una nueva red wifi). 
Principalmente está conformada por:
* **LEDs:** Quienes se encargan de mostrar si el dispositivo está conectado al Wifi, y de ser así, también otro para el caso del broquer mqtt.
* **Pulsador:** Perimite entrar al estado de configuración de Wifi en el caso de que este no pueda conectarse o de cambiar de ubicación el dispositivo.

![image](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/blob/main/Hardware/Acople%20Pull%20y%20led/Imagen%203D%20superior.jpg)


## Videos
![zer_video_img](https://github.com/RodrigoSturm14/Zapatilla-Electronica-Remota-Z.E.R-/assets/105557226/a82fa8dc-b918-444d-a02f-bd17f430531e)

[Video](https://drive.google.com/file/d/11wxJRINLuGf1xzWTfpv2VvfbYe2Eymw4/view?usp=sharing)
