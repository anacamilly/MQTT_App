#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
 
 
/* Definições do LED */
#define PIN_LED 23
 
/* Definicoes do sensor DHT22 */
#define DHTPIN 2     //GPIO que está ligado o pino de dados do sensor
 
//#define DHTTYPE DHT11
#define DHTTYPE DHT11   //sensor em utilização: DHT22
//#define DHTTYPE DHT21

// Pino do sensor de luz (LDR)
const int ldrPin = 34;
 
// Define do MQTT 
#define TOPICO_SUBSCRIBE_LED         "topico_liga_desliga_led"
#define TOPICO_PUBLISH_TEMPERATURA   "sensor/temperatura"  
#define TOPICO_PUBLISH_UMIDADE       "sensor/umidade" 
#define TOPICO_PUBLISH_LDR           "sensor/LDR"

 
#define ID_MQTT  "esp32_mqtt"     //id mqtt (para identificação de sessão)
 
/* Variaveis, constantes e objetos globais */
DHT dht(DHTPIN, DHTTYPE);
 
const char* SSID = "YHWH"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "TSR182905"; // Senha da rede WI-FI que deseja se conectar
const char* BROKER_MQTT = "mqtt.internetecoisas.com.br"; //URL do broker MQTT que se deseja utilizar
const int BROKER_PORT = 1883; // Porta do Broker MQTT
const char* mqttUser = "d452be90@454";
const char* mqttPassword = "391aeb33e323";
const char* mqttTopic = "sensor/umidade";
const char* mqttTopic2 = "sensor/temperatura";
   
//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
 
/* Prototypes */
/*void initWiFi(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);*/
 
/*
 * Implementações

/* Função: inicializa e conecta-se na rede WI-FI desejada
*/
void initWiFi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
      
    reconnectWiFi();
}
 
/* Função: inicializa parâmetros de conexão MQTT(endereço do 
 *         broker, porta e seta função de callback)
 */
void initMQTT(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
 
/* Função: função de callback 
 *         esta função é chamada toda vez que uma informação de 
 *         um dos tópicos subescritos chega)
 */
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
  
    /* obtem a string do payload recebido */
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
 
    Serial.print("\nChegou a seguinte string via MQTT: ");
    Serial.println(msg);
    
    /* toma ação dependendo da string recebida */
    if (msg.equals("ON"))
    {
        digitalWrite(PIN_LED, HIGH);
        Serial.print("LED aceso mediante comando MQTT\n");
        Serial.print("");
    }
  
    if (msg.equals("OFF"))
    {
        digitalWrite(PIN_LED, LOW);    
        Serial.print("LED apagado mediante comando MQTT\n"); 
        
    }
}
 
/* Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
 *         em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
 */
void reconnectMQTT(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);

        if (MQTT.connect(ID_MQTT, mqttUser, mqttPassword)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE_LED);
            
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
 
/* Função: verifica o estado das conexões WiFI e ao broker MQTT. 
 *         Em caso de desconexão (qualquer uma das duas), a conexão
 *         é refeita.
 */
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
      
     reconnectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
/* Função: reconecta-se ao WiFi
 */
void reconnectWiFi(void) 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
          
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
      
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

  
/* Função de setup */
void setup() 
{
    MQTT.subscribe(TOPICO_PUBLISH_UMIDADE);
    MQTT.subscribe(TOPICO_PUBLISH_TEMPERATURA);
    Serial.begin(9600);  
 
    /* Configuração do pino ligado ao LED como output 
       e inicialização do mesmo em LOW */
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED,LOW);
    pinMode(ldrPin, INPUT);
 
    /* Inicializacao do sensor de temperatura */
    dht.begin();  
 
    /* Inicializa a conexao wi-fi */
    initWiFi();
 
    /* Inicializa a conexao ao broker MQTT */
    initMQTT();
}
 
/* Loop principal */
void loop() {

  
     
    /* garante funcionamento das conexões WiFi e ao broker MQTT */
    VerificaConexoesWiFIEMQTT();

    

   //-------------------------------------------------------------------

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler dados do sensor DHT!");
    return;
  }

  String payload1 = String(temperature);
  String payload2 = String(humidity);

  Serial.print("Umidade: ");
  Serial.print(humidity);
  Serial.print("%\tTemperatura: ");
  Serial.print(temperature);
  Serial.println("°C");
  
  MQTT.publish(TOPICO_PUBLISH_TEMPERATURA, payload1.c_str());
  MQTT.publish(TOPICO_PUBLISH_UMIDADE, payload2.c_str());
   
   //-----------------------------------------------------------------------

   //-----------------------------------------------------------------------

   // Lendo o valor do sensor de luz (LDR)
  int lightValue = analogRead(ldrPin);

  // Publicando o valor do sensor no tópico MQTT
  char lightString[100];
  snprintf(lightString, sizeof(lightString), "%d", lightValue);
  MQTT.publish(TOPICO_PUBLISH_LDR, lightString);

  // Acendendo ou apagando o LED com base no valor do sensor

  
  /*if (lightValue < 500 ) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }*/

  int ledIntensity = map(lightValue, 0, 4095, 255, 0);  // Mapeia o valor lido para a faixa de 0 a 255
  analogWrite(PIN_LED, ledIntensity);

  Serial.print("Sensor LDR: ");
  Serial.print(lightValue);
  Serial.print("\n");
  

  //-------------------------------------------------------------------------

    /* keep-alive da comunicação com broker MQTT */
    MQTT.loop();
 
    /* Refaz o ciclo após 2 segundos */
    delay(5000);
    
}
