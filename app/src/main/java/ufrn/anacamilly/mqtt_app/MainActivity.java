package ufrn.anacamilly.mqtt_app;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MainActivity extends AppCompatActivity {

    private MqttClient client;
    private boolean isLightOn = false;
    private double temperature = 0.0;
    private double luminosity = 0.0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setupMqtt();

        Switch lightSwitch = findViewById(R.id.lightSwitch);
        lightSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                handleLightSwitch(isChecked);
            }
        });
    }

    private void setupMqtt() {
        try {
            // Configurações do servidor MQTT
            String broker = "tcp://mqtt.internetecoisas.com.br:1883";
            String clientId = "client_identifier";
            MemoryPersistence persistence = new MemoryPersistence();
            client = new MqttClient(broker, clientId, persistence);

            MqttConnectOptions connectOptions = new MqttConnectOptions();
            connectOptions.setCleanSession(true);
            connectOptions.setUserName("d452be90@454");
            connectOptions.setPassword("391aeb33e323".toCharArray());

            // Callbacks para eventos MQTT
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    System.out.println("Desconectado do servidor MQTT");
                }

                @Override
                public void messageArrived(String topic, MqttMessage message) throws Exception {
                    // Trata mensagens recebidas nos tópicos "temperature_topic" e "luminosity_topic"
                    if (topic.equals("temperature_topic")) {
                        temperature = Double.parseDouble(message.toString());
                    } else if (topic.equals("luminosity_topic")) {
                        luminosity = Double.parseDouble(message.toString());
                    }

                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            // Atualiza as TextViews com os valores recebidos
                            TextView temperatureTextView = findViewById(R.id.temperatureTextView);
                            temperatureTextView.setText("Temperatura: " + temperature);

                            TextView luminosityTextView = findViewById(R.id.luminosityTextView);
                            luminosityTextView.setText("Luminosidade: " + luminosity);
                        }
                    });
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                }
            });

            // Conecta ao servidor MQTT
            client.connect(connectOptions);
            Toast.makeText(this, "Conectado ao servidor MQTT", Toast.LENGTH_SHORT).show();

            // Subscreve nos tópicos "temperature_topic" e "luminosity_topic"
            client.subscribe("temperature_topic", 1);
            client.subscribe("luminosity_topic", 1);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void publishLightState(boolean isOn) {
        // Publica o estado da luz (ON ou OFF) no tópico "light_topic"
        String payload = isOn ? "ON" : "OFF";
        MqttMessage message = new MqttMessage(payload.getBytes());
        message.setQos(1);

        try {
            client.publish("LED", message);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void handleLightSwitch(boolean value) {
        // Lida com a mudança do estado do switch
        isLightOn = value;
        publishLightState(value);
    }
}