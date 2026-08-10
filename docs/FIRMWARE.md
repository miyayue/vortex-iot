# Firmware
O firmware é um programa em C++ que roda em um microcontrolador Esp32, simulando o recebimento de dados de sensores de temperatura, umidade e luminosidade e realiza o envio desses dados periodicamente ao servidor via protocolo HTTP.

O firmware conecta o dispositivo à rede WiFi por meio de nome e senha da rede fornecidos pelo usuário e faz o envio dos dados para o servidor através de uma requisição `POST` utilizando seu endereço de IP, também fornecido pelo usuário. O dispositivo também realiza o envio dos dados para um broker MQTT através de um tópico especificado no código.

Os dados simulados são obtidos de forma aleatória com o uso da própria função `random` do Esp32, estando os valores de temperatura entre 16 e 33 °C, os de umidade entre 0 a 100% e os de luminosidade entre 0 e 1024. Entre os dados enviados estão também o nome do dispositivo para identificá-lo junto à data e hora da medição.