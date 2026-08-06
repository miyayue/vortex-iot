#include <Arduino.h>
#include <WiFi.h>

float getTemperature();
float getHumidity();
float getLuminosity();

// variáveis para conexão WiFi (nome da rede e senha) -> pegar wifi e senha a partir da aplicacao web depois? por seguranca
String ssid;
String pswd;

void setup()
{
  // inicializando a comunicação serial
  Serial.begin(115200);

  // recebendo o nome da rede e a senha do wifi através do monitor serial (temporario?)
  Serial.println("Insira o nome da rede Wifi: ");
  if (Serial.available())
  {
    ssid = Serial.readStringUntil('\n');
  }
  Serial.println("Insira a senha da rede Wifi: ");
  if (Serial.available())
  {
    pswd = Serial.readStringUntil('\n');
  }

  // inicializando a comunicação wifi
  WiFi.begin(ssid, pswd);
  // aguardando a conexão para continuar o programa
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.println("Conectando com a rede WiFi...");
  }
  Serial.println("Conectado à rede Wifi.");
}

void loop()
{
  // verificando se a conexão foi perdida
  if (WiFi.status() == WL_CONNECTION_LOST)
  {
    // tentando reconectar
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pswd);
      Serial.println("Conexão com a rede WiFi perdida. Tentando reconectar...");
    }
    Serial.println("Reconectado à rede Wifi.");
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
