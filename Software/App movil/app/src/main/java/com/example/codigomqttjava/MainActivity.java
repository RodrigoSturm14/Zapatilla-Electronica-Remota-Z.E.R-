package com.example.codigomqttjava;
// linearlayout sirve para que todos los elementos de la app esten alineados en una dirrecion: vert o hori
//android:gravity te permite mover un texto hacia alguna parte de la app, como centrarlo


import static android.graphics.Color.GREEN;
import static android.graphics.Color.RED;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    String clientID = "";

    //Conexion server

    static String MQTTHOST = "tcp://prueba-fablocal.cloud.shiftr.io:1883";
    static String MQTTUSER = "prueba-fablocal";
    static String MQTTPASS = "BfCZIqyLhsBGzDZ5";

    static String TOPIC1 = "TOMA1";
    static String TOPIC2 = "TOMA2";
    static String TOPIC3 = "TOMA3";
    static String TOPIC4 = "TOMA4";

    //En esta parte se elige el topic que queremos usar

    static String TOPIC_MSG_ON = "1";
    static String TOPIC_MSG_OFF = "0";
    // Puede ser "ON" "OFF", "ENCENDER" "APAGAR" o "1" "0".

    MqttAndroidClient clientex;
    MqttConnectOptions opciones;

    Boolean permisoPublicar;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getClientName();
        connectBroker();

        // Botones de encendido y apagado para cada TOMA

        Button buttonON1 = findViewById(R.id.buttonON_ID_1);
        buttonON1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC1, TOPIC_MSG_ON);

            }
        });

        Button buttonON2 = findViewById(R.id.buttonON_ID_2);
        buttonON2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC2, TOPIC_MSG_ON);

            }
        });

        Button buttonON3 = findViewById(R.id.buttonON_ID_3);
        buttonON3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC3, TOPIC_MSG_ON);

            }
        });

        Button buttonON4 = findViewById(R.id.buttonON_ID_4);
        buttonON4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC4, TOPIC_MSG_ON);

            }
        });


        Button buttonOFF1 = findViewById(R.id.buttonOFF_ID_1);
        buttonOFF1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC1, TOPIC_MSG_OFF);

            }
        });


        Button buttonOFF2 = findViewById(R.id.buttonOFF_ID_2);
        buttonOFF2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC2, TOPIC_MSG_OFF);


            }
        });

        Button buttonOFF3 = findViewById(R.id.buttonOFF_ID_3);
        buttonOFF3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC3, TOPIC_MSG_OFF);


            }
        });

        Button buttonOFF4 = findViewById(R.id.buttonOFF_ID_4);
        buttonOFF4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                enviarMensaje(TOPIC4, TOPIC_MSG_OFF);


            }
        });


    }

    // Funcion automatica al inicializar la app
    // Pregunta si estoy conectado, y da permiso para publicar. Si no estoy conectado, no tengo permiso de publicar, intenta reconectar.

    private void checkConnection() {

        if (this.clientex.isConnected()) {

            this.permisoPublicar = true;
        } else {
            this.permisoPublicar = false;
            connectBroker();

        }

    }

    private void enviarMensaje(String topic, String msg) {

        // define parametros, topic y msg. Usar try-catch debido a posibles fallos de conexion.

        checkConnection();
        if (this.permisoPublicar)
            try {

                int qos = 0;
                this.clientex.publish(topic, msg.getBytes(), qos, false);
                Toast.makeText(getBaseContext(), topic + " : " + msg, Toast.LENGTH_SHORT).show();

                // envia el msg al servidor y podemos visualizarlo en el celular

            } catch (Exception e) {

                e.printStackTrace();

            }

    }


    private void connectBroker() {
        this.clientex = new MqttAndroidClient(this.getApplicationContext(), MQTTHOST, this.clientID);
        this.opciones = new MqttConnectOptions();
        this.opciones.setUserName(MQTTUSER);
        this.opciones.setPassword(MQTTPASS.toCharArray());

        try {
            IMqttToken token = this.clientex.connect(opciones);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Toast.makeText(getBaseContext(), "CONECTADO", Toast.LENGTH_SHORT).show();

                    //suscripcion a cada topic despues de conexion exitosa

                    SuscripcionesYmanejodemensajes();


                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Toast.makeText(getBaseContext(), "ERROR DE CONEXION", Toast.LENGTH_SHORT).show();

                }
            });


        } catch (MqttException e) {
            e.printStackTrace();


        }
    }

    //SUB TOMA 1
    private void SuscripcionesYmanejodemensajes() { //esta, igual que las otras, es una funcion creada por mi que dentro contiene los comandos necesarios (exportados de PAHO) para cumplir la funcion que quiero.

        this.clientex.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {
                Toast.makeText(getBaseContext(), "Se desconecto el servidor", Toast.LENGTH_SHORT).show();
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {

                // Handle message arrival for all topics here

                if (topic.matches(TOPIC1)) {
                    TextView txtDATA1 = findViewById(R.id.txtDATA1_ID);
                    String msg = new String(message.getPayload());

                    if (msg.equals(TOPIC_MSG_ON)) {
                        txtDATA1.setText(R.string.ENCENDIDO);
                        txtDATA1.setBackgroundColor(GREEN);
                    }
                    else if (msg.equals(TOPIC_MSG_OFF)) {
                        txtDATA1.setText(R.string.APAGADO);
                        txtDATA1.setBackgroundColor(RED);
                    }
                }


                else if (topic.matches(TOPIC2)) {
                    TextView txtDATA2 = findViewById(R.id.txtDATA2_ID);
                    String msg = new String(message.getPayload());

                    if (msg.equals(TOPIC_MSG_ON)) {
                        txtDATA2.setText(R.string.ENCENDIDO);
                        txtDATA2.setBackgroundColor(GREEN);
                    }
                    else if (msg.equals(TOPIC_MSG_OFF)) {
                        txtDATA2.setText(R.string.APAGADO);
                        txtDATA2.setBackgroundColor(RED);
                    }
                }

                else if (topic.matches(TOPIC3)) {
                    TextView txtDATA3 = findViewById(R.id.txtDATA3_ID);
                    String msg = new String(message.getPayload());

                    if (msg.equals(TOPIC_MSG_ON)) {
                        txtDATA3.setText(R.string.ENCENDIDO);
                        txtDATA3.setBackgroundColor(GREEN);
                    }
                    else if (msg.equals(TOPIC_MSG_OFF)) {
                        txtDATA3.setText(R.string.APAGADO);
                        txtDATA3.setBackgroundColor(RED);
                    }
                }

                else if (topic.matches(TOPIC4)) {
                    TextView txtDATA4 = findViewById(R.id.txtDATA4_ID);
                    String msg = new String(message.getPayload());

                    if (msg.equals(TOPIC_MSG_ON)) {
                        txtDATA4.setText(R.string.ENCENDIDO);
                        txtDATA4.setBackgroundColor(GREEN);
                    }
                    else if (msg.equals(TOPIC_MSG_OFF)) {
                        txtDATA4.setText(R.string.APAGADO);
                        txtDATA4.setBackgroundColor(RED);
                    }
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

                // Handle delivery completion if needed
            }
        });

// Subscribe to all topics
        try {
            this.clientex.subscribe(TOPIC1, 0);
            this.clientex.subscribe(TOPIC2, 0);
            this.clientex.subscribe(TOPIC3, 0);
            this.clientex.subscribe(TOPIC4, 0);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

    }


    private void getClientName() {
        String manufacturer = Build.MANUFACTURER;
        String modelName = Build.MODEL;
        this.clientID = manufacturer + " " + modelName;

        TextView txtIdClient = findViewById(R.id.textIdClient);
        txtIdClient.setText(this.clientID);


    }
}

