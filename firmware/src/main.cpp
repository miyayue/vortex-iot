#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ezTime.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

// variáveis para conexão WiFi (nome da rede e senha)
String ssid = "";
String pswd = "";

// dados do broker mqtt
const char *mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
// criando um client para a comunicação mqtt
WiFiClient esp_client;
PubSubClient client(esp_client);
// tópico mqtt
const char *topic = "vortex-iot/esp32-VTX01";

// ip para a aplicação web
String addr = "";
// caminho para fazer o post
String server_name = "http://";
// criando um client http
HTTPClient http;

// criando variável para armazenar o timestamp
String timestamp;
// criando variável para definir a timezone
Timezone brasil;
// momento desde a última atualização do dashboard desde o início do funcionamento do dispositivo
unsigned long last_update_time = 0;

void connectBroker();
void reconnectBroker();
float getTemperature();
float getHumidity();
float getLuminosity();
void update();

void setup()
{
  // inicializando a comunicação serial
  Serial.begin(115200);
  // setando tempo para esperar por dados no serial port
  Serial.setTimeout(20000);

  // recebendo o nome da rede e a senha do wifi através do terminal serial
  /*Serial.println("Insira o nome da rede Wifi: ");
  ssid = Serial.readStringUntil('\n');
  ssid.trim();

  Serial.println("Insira a senha da rede Wifi: ");
  pswd = Serial.readStringUntil('\n');
  pswd.trim();

  Serial.println("Insira o endereço IP para o dashboard (formato IP:port): ");
  addr = Serial.readStringUntil('\n');
  addr.trim();

  */

  // inicializando a comunicação wifi
  WiFi.begin(ssid, pswd);
  // aguardando a conexão para continuar o programa
  Serial.print("Conectando com a rede WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado à rede Wifi.");

  connectBroker();

  // adicionando a rota
  addr.concat("/measurements");
  // completando a url
  server_name.concat(addr);
  // conexão http
  http.begin(esp_client, server_name);

  // sincronizando o tempo
  waitForSync();
  // definindo a timezone
  brasil.setLocation("America/Fortaleza");
}

void loop()
{
  // verificando se a conexão foi perdida
  if (WiFi.status() == WL_CONNECTION_LOST)
  {
    // tentando reconectar
    Serial.print("Conexão com rede WiFi perdida. Tentando reconectar");
    WiFi.begin(ssid, pswd);
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
    }
    Serial.println("\nReconectado à rede Wifi.");
  }

  reconnectBroker();
  client.loop();

  // tempo desde o início do programa
  unsigned long now = millis();
  if (last_update_time == 0 || now - last_update_time >= 1800000)
  {
    // atualizando o valor momento de update do dashboard para fazer um novo update
    last_update_time = now;
    update();
  }
}

// função para conectar ao broker mqtt
void connectBroker()
{
  // configurando o client
  client.setServer(mqtt_broker, mqtt_port);
  String client_id = "esp32-client-VTX01";

  // conectando ao broker
  Serial.println("Conectando ao broker MQTT...");
  while (!client.connected())
  {
    if (client.connect(client_id.c_str()))
    {
      Serial.println("Conectado ao broker MQTT.");
    }
    else
    {
      Serial.print("A conexão falhou com estado: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// função para reconectar ao broker mqtt caso a conexão seja perdida
void reconnectBroker()
{
  if (!client.connected())
  {
    Serial.println("Conexão com o broker MQTT perdida. Tentando reconectar...");
    connectBroker();
  }
}

// função para gerar um valor de temperatura
float getTemperature()
{
  // gerando um valor aleatório entre 160 (inclusivo) e 331 (exclusivo) e dividindo por 10 para
  // transformar o inteiro em um float indicando uma temperatura com até uma casa decimal de precisão
  return random(160, 331) / 10.0;
}

// função para gerar um valor de umidade
float getHumidity()
{
  // gerando um valor aleatório até 1000 e dividindo por 10 para transformar o inteiro em um float e
  // indicar a porcentagem da umidade com até uma casa decimal de precisão
  return random(1000) / 10.0;
}

// função para gerar um valor da luminosidade
float getLuminosity()
{
  // gerando um valor aleatório até 1024 (exclusivo) para representar o valor medido pelo sensor LDR
  return random(1024);
}

// função que envia os dados atualizados
void update()
{
  // pegando os valores dos sensores e o timestamp
  float temperature = getTemperature();
  float humidity = getHumidity();
  float luminosity = getLuminosity();
  timestamp = brasil.dateTime(ISO8601);

  // string para enviar o payload
  String payload;

  // documento json para organizar os dados
  JsonDocument doc;

  // definindo os campos
  doc["device"] = "VTX01";
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["luminosity"] = luminosity;
  doc["timestamp"] = timestamp;

  // serializando o json para envio
  serializeJson(doc, payload);

  // enviando via mqtt
  client.publish(topic, payload.c_str());

  // enviando via http
  http.addHeader("Content-Type", "application/json");
  int http_response_code = http.POST(payload);
  Serial.print("Response code: ");
  Serial.println(http_response_code);
}
