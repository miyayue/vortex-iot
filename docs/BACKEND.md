# Backend
O backend é um script python que utiliza a biblioteca de servidor HTTP `Flask` para lidar com as requisições e fazer a comunicação entre firmware e frontend. 

O backend possui uma API Rest com quatro requisições:
- `GET /measurements`: solicita todas as medidas já enviadas ao servidor.
- `POST /measurements`: envia novas medidas ao servidor.
- `GET /devices`: solicita o identificador de todos os dispositivos conhecidos pelo servidor.
- `GET /devices/{id}`: solicita todas as medidas enviadas ao servidor pelo dispositivo de identificador `{id}`.

O servidor também armazena as medidas enviadas a ele em um arquivo sempre que recebe uma nova medida para que possa manter um histórico. Ao ser iniciado, o servidor lê todas as medidas armazenadas no arquivo para enviar o histórico ao frontend.