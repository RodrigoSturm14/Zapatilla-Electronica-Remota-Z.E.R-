#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <PubSubClient.h>
#include "SPIFFS.h"

// ---------  Depuracion  ---------
#define DEBUG 1  // Cambiar el 0 por un 1 si se hacen prueb  as con el esp conectado al puerto serial.

// ---------  Componentes OUTPUT & INPUT de Control  ---------
#define CANT_OUTPUT 7
#define PIN_BOTON 22
#define PIN_LED_V 18
#define PIN_LED_R 21
#define PIN_LED_ESP 2
#define PIN_RELAY_1 16
#define PIN_RELAY_2 27
#define PIN_RELAY_3 33
#define PIN_RELAY_4 17
bool button_state;

int pines_output[CANT_OUTPUT] = { PIN_RELAY_1, PIN_RELAY_2, PIN_RELAY_3, PIN_RELAY_4, PIN_LED_V, PIN_LED_R, PIN_LED_ESP };

// ---------  WiFi & Broker MQTT  ---------
WiFiClient espClient;                        // Definir espClient como un objeto de la clase WiFiClient. c6ea7c
PubSubClient client(espClient);              // Definir espClient como objeto de la clase PubSubClient. 271193841
const char *mqtt_broker = "broker.emqx.io";  // Direccion del broker (SHIFTR, EMQX U OTRO BROKER).
const char *mqtt_username = "RodSturm";      // Username para autenticacion.
const char *mqtt_password = "racing2018";    // Password para autenticacion.
const int mqtt_port = 1883;                  // Puerto MQTT sobre TCP.
bool wifi_state;
int PAYLOADINT;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";

//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/ip.txt";
const char* gatewayPath = "/gateway.txt";

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

// --- Funcion Iniciar SPIFFS ---
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    if (DEBUG) Serial.println("An error has occurred while mounting SPIFFS");
  }
  if (DEBUG) Serial.println("SPIFFS mounted successfully");
}

// --- Funcion Leer archivo SPIFFS ---
String readFile(fs::FS& fs, const char* path) {
  if (DEBUG) Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    if (DEBUG) Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available()) {
    fileContent = file.readStringUntil('\n');
    break;
  }
  return fileContent;
}

// --- Funcion Escribir en archivo SPIFFS ---
void writeFile(fs::FS& fs, const char* path, const char* message) {
  if (DEBUG) Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    if (DEBUG) Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    if (DEBUG) Serial.println("- file written");
  } else {
    if (DEBUG) Serial.println("- write failed");
  }
}

// --- Funcion Iniciar conexion red WiFi ---
bool initWiFi() {
  digitalWrite(PIN_LED_R, HIGH);
  delay(500);
  if (ssid == "" || ip == "") {
    if (DEBUG) Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.begin(ssid.c_str(), pass.c_str());
  if (DEBUG) Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while (WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      if (DEBUG)Serial.println("Failed to connect.");
      return false;
    }
  }

  if (DEBUG) Serial.println(WiFi.localIP());
  digitalWrite(PIN_LED_R, LOW);
  delay(500);
  return true;
}

// --- Funcion Iniciar conexion broker MQTT ---
void initMQTT(String mqtt_url_broker, int mqtt_puerto) {

  if (WiFi.status() == WL_CONNECTED) {
    if (DEBUG) Serial.println("Conectado a WiFi network");  // Si el ESP logro conectarse a la red WiFi, intntara conectarse al broker MQTT.

    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);

    while (!client.connected()) {
      
      digitalWrite(PIN_LED_R, HIGH);
      delay(500);
      
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      if (DEBUG) Serial.printf("The client ------ %s ------ connects to the public mqtt broker\n", client_id.c_str());  // here the macAddress of the esp32 (which is a unique identifier for the esp32) is printed in the string variable client_id. then client_id is passed to a char arrays with the function c_str().


      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {  // here the esp32 will try to connect with the broker
        if (DEBUG) Serial.println("Public emqx mqtt broker connected");
      } else {
        if (DEBUG) {
          Serial.print("failed with state ");
          Serial.print(client.state());
        }
        digitalWrite(PIN_LED_R, LOW);
        delay(2000);
      }
    }

    // --- Funciones Publish & Subscribe ---
    client.publish("TOMA1", "Hola Shiftr y Celular soy ESP32 :] , escribiendo en TOMA1.");
    client.subscribe("TOMA1");
    client.subscribe("TOMA2");
    client.subscribe("TOMA3");
    client.subscribe("TOMA4");
    delay(1000);
    digitalWrite(PIN_LED_V, HIGH);
    digitalWrite(PIN_LED_R, LOW);
  }
}

// --- Funcion Callback MQTT ---
void callback(char *topic, byte *payload, unsigned int length) {  // here the message from the broker is trasmitted to the esp32 with the variable payload

  if (strcmp(topic, "TOMA1") == 0) {
    digitalWrite(PIN_LED_ESP, HIGH);
    delay(500);
    digitalWrite(PIN_LED_ESP, LOW);
    if (DEBUG) {
      Serial.print("Message arrived in topic: ");
      Serial.println(topic);
      Serial.print("Message: ");
    }
    for (int i = 0; i < length; i++) {
      if (DEBUG) Serial.print((char)payload[i]);
    }
    char payload_string[length + 1];
    memcpy(payload_string, payload, length);
    payload_string[length] = '\0'; // se le agrega un NULL TERMINATED al final de la string, ya que de esa forma el programa sabe en donde termina la string.
    PAYLOADINT = atoi(payload_string);
    if (DEBUG) {
      Serial.println();
      Serial.print("Payload as a integer:");
      Serial.print(PAYLOADINT);
      Serial.println();
      Serial.println("-----------------------");
    }

    if (PAYLOADINT == 1) {
      digitalWrite(PIN_RELAY_1, LOW);  // PRENDER RELAY 1
      client.publish("NOTIFICACION", "TOMA 1 ACTIVADO - ESP32");
    } else if (PAYLOADINT == 0) {
      digitalWrite(PIN_RELAY_1, HIGH);  // APAGAR RELAY 1
      client.publish("NOTIFICACION", "TOMA 1 DESACTIVADO - ESP32");
    }
  }

  else if (strcmp(topic, "TOMA2") == 0) {
    digitalWrite(PIN_LED_ESP, HIGH);
    delay(500);
    digitalWrite(PIN_LED_ESP, LOW);
    if (DEBUG) {
      Serial.print("Message arrived in topic: ");
      Serial.println(topic);
      Serial.print("Message:");
    }
    for (int i = 0; i < length; i++) {
      if (DEBUG) Serial.print((char)payload[i]);
    }
    char payload_string[length + 1];
    memcpy(payload_string, payload, length);
    payload_string[length] = '\0';
    PAYLOADINT = atoi(payload_string);
    if (DEBUG) {
      Serial.println();
      Serial.print("Payload as a integer:");
      Serial.print(PAYLOADINT);
      Serial.println();
      Serial.println("-----------------------");
    }

    if (PAYLOADINT == 1) {
      digitalWrite(PIN_RELAY_2, LOW);  // PRENDER RELAY 2.
      client.publish("NOTIFICACION", "TOMA 2 ACTIVADO - ESP32");
    } else if (PAYLOADINT == 0) {
      digitalWrite(PIN_RELAY_2, HIGH);  // APAGAR RELAY 2.
      client.publish("NOTIFICACION", "TOMA 2 DESACTIVADO - ESP32");
    }
  }

  else if (strcmp(topic, "TOMA3") == 0) {
    digitalWrite(PIN_LED_ESP, HIGH);
    delay(500);
    digitalWrite(PIN_LED_ESP, LOW);
    if (DEBUG) {
      Serial.print("Message arrived in topic: ");
      Serial.println(topic);
      Serial.print("Message:");
    }
    for (int i = 0; i < length; i++) {
      if (DEBUG) Serial.print((char)payload[i]);
    }
    char payload_string[length + 1];
    memcpy(payload_string, payload, length);
    payload_string[length] = '\0';
    PAYLOADINT = atoi(payload_string);
    if (DEBUG) {
      Serial.println();
      Serial.print("Payload as a integer:");
      Serial.print(PAYLOADINT);
      Serial.println();
      Serial.println("-----------------------");
    }

    if (PAYLOADINT == 1) {
      digitalWrite(PIN_RELAY_3, LOW);  // PRENDER RELAY 3.
      client.publish("NOTIFICACION", "TOMA 3 ACTIVADO - ESP32");
    } else if (PAYLOADINT == 0) {
      digitalWrite(PIN_RELAY_3, HIGH);  // APAGAR RELAY 3.
      client.publish("NOTIFICACION", "TOMA 3 DESACTIVADO - ESP32");
    }
  }

  else if (strcmp(topic, "TOMA4") == 0) {
    digitalWrite(PIN_LED_ESP, HIGH);
    delay(500);
    digitalWrite(PIN_LED_ESP, LOW);
    if (DEBUG) {
      Serial.print("Message arrived in topic: ");
      Serial.println(topic);
      Serial.print("Message:");
    }
    for (int i = 0; i < length; i++) {
      if (DEBUG) Serial.print((char)payload[i]);
    }
    char payload_string[length + 1];
    memcpy(payload_string, payload, length);
    payload_string[length] = '\0';
    PAYLOADINT = atoi(payload_string);
    if (DEBUG) {
      Serial.println();
      Serial.print("Payload as a integer:");
      Serial.print(PAYLOADINT);
      Serial.println();
      Serial.println("-----------------------");
    }

    if (PAYLOADINT == 1) {
      digitalWrite(PIN_RELAY_4, LOW);  // PRENDER RELAY 4.
      client.publish("NOTIFICACION", "TOMA 4 ACTIVADO - ESP32");
    } else if (PAYLOADINT == 0) {
      digitalWrite(PIN_RELAY_4, HIGH);  // APAGAR RELAY 4.
      client.publish("NOTIFICACION", "TOMA 4 DESACTIVADO - ESP32");
    }
  }
}

// --- Funcion Declaracion pines ---
void pinSetup() {
  pinMode(PIN_BOTON, INPUT);
  // declaracion de outputs
  for (int i = 0; i < CANT_OUTPUT; i++) {
    pinMode(pines_output[i], OUTPUT);
  }
  // deshabilitar relays
  for (int i = 0; i < 4; i++) {
    digitalWrite(pines_output[i], HIGH);
  }
  digitalWrite(PIN_LED_R, LOW);
}


void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinSetup();

  initSPIFFS();

  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  ip = readFile(SPIFFS, ipPath);
  gateway = readFile(SPIFFS, gatewayPath);
  if (DEBUG) {
    Serial.println(ssid);
    Serial.println(pass);
    Serial.println(ip);
    Serial.println(gateway);
  }
  
  wifi_state = initWiFi();

  if (wifi_state) {
    initMQTT(mqtt_broker, mqtt_port);

    while (digitalRead(PIN_BOTON) == 1 && WiFi.status() == WL_CONNECTED) {  // EL PROGRAMA VA ENTRAR Y REPETIR EL WHILE SIMPRE Y CUANDO NO SE ESTE PRESIONANDO EL BOTON Y EL ESP32 ESTE CONECTADO A UNA RED WIFI.
      client.loop();
    }
  }

  if (WiFi.status() == WL_CONNECTED) WiFi.disconnect(true);
  digitalWrite(PIN_LED_V, LOW);
  digitalWrite(PIN_LED_R, HIGH);

  // Connect to Wi-Fi network with SSID and password
  if (DEBUG)Serial.println("Setting AP (Access Point)");
  // NULL sets an open Access Point
  WiFi.softAP("ESP-WIFI-MANAGER", NULL);

  IPAddress IP = WiFi.softAPIP();
  if (DEBUG) {
    Serial.print("AP IP address: ");
    Serial.println(IP);
  }

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/wifimanager.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/", HTTP_POST, [](AsyncWebServerRequest * request) {
    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {
        // HTTP POST ssid value
        if (p->name() == PARAM_INPUT_1) {
          ssid = p->value().c_str();
          if (DEBUG) {
            Serial.print("SSID set to: ");
            Serial.println(ssid);
          }
          // Write file to save value
          writeFile(SPIFFS, ssidPath, ssid.c_str());
        }
        // HTTP POST pass value
        if (p->name() == PARAM_INPUT_2) {
          pass = p->value().c_str();
          if (DEBUG) {
            Serial.print("Password set to: ");
            Serial.println(pass);
          }
          // Write file to save value
          writeFile(SPIFFS, passPath, pass.c_str());
        }
        // HTTP POST ip value
        if (p->name() == PARAM_INPUT_3) {
          ip = p->value().c_str();
          if (DEBUG) {
            Serial.print("IP Address set to: ");
            Serial.println(ip);
          }
          // Write file to save value
          writeFile(SPIFFS, ipPath, ip.c_str());
        }
        // HTTP POST gateway value
        if (p->name() == PARAM_INPUT_4) {
          gateway = p->value().c_str();
          if (DEBUG) {
            Serial.print("Gateway set to: ");
            Serial.println(gateway);
          }
          // Write file to save value
          writeFile(SPIFFS, gatewayPath, gateway.c_str());
        }
        //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + ip);
    delay(3000);
    ESP.restart();
  });
  server.begin();

}

void loop() {
}
