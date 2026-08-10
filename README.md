# Projeto IoT Vortex
Esse projeto foi realizado para a inscrição em uma vaga de estágio em IoT no Vortex. O processo seletivo exige a criação de um sistema de controle ambiental para um ambiente de pesquisa com o uso de um Esp32, sensores de temperatura, umidade e luminosidade, um backend com API Rest e um frontend responsivo para mostrar as informações. 

# Instruções
## Firmware
No VSCode, instale a extensão `PlatformIO` e rode o comando `Upload and Monitor` para enviar o programa ao Esp32 e monitorar a saída serial. 

Serão solicitadas 3 informações através do terminal integrado do VSCode: o nome da rede WiFi, a senha e o endereço de IP do servidor.

## Servidor
Para rodar o servidor, é necessário possuir o gerenciador de projetos python `uv` ([instruções de instalação](https://docs.astral.sh/uv/#installation)). 

Com o `uv` instalado, rode, dentro da pasta do backend, o seguinte comando no terminal:
```
uv run main.py
```
Por padrão, o servidor irá rodar no `localhost` na porta `5000` (que deve estar aberta para comunicação TCP).

## Frontend
Para visualizar o frontend, é necessário ter o servidor em execução e acessar `localhost:5000` ou `IPdoServidor:5000` no seu navegador.