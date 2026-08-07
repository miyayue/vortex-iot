#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

float getTemperature();
float getHumidity();
float getLuminosity();
void verifyAndConnect();

// variáveis para conexão WiFi (nome da rede e senha) -> pegar wifi e senha a partir da aplicacao web depois? por seguranca
String ssid;
String pswd;

// dados do broker mqtt
const char *mqtt_broker = "0892fa44f5e445fea74c933892517dea.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
// credenciais do dispositivo
const char *mqtt_username = "VTX01";
const char *mqtt_pswd = "vortexiot";

void setup()
{
  // inicializando a comunicação serial
  Serial.begin(115200);
  // setando tempo para esperar por dados no serial port
  Serial.setTimeout(20000);

  // recebendo o nome da rede e a senha do wifi através do terminal serial (temporario?)
  Serial.println("Insira o nome da rede Wifi: ");
  ssid = Serial.readStringUntil('\n');
  ssid.trim();

  Serial.println("Insira a senha da rede Wifi: ");
  pswd = Serial.readStringUntil('\n');
  pswd.trim();

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
