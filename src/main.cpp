#include <Arduino.h>

float getTemperature();
float getHumidity();
float getLuminosity();

void setup()
{
}

void loop()
{
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
