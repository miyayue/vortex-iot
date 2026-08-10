from flask import Flask, request
import json

app = Flask(__name__, static_url_path="", static_folder="../frontend")

# classe com as medidas
class Measurement:
    def __init__(self, temperature: float, humidity: float, luminosity: float, timestamp: str):
        self.temperature = temperature
        self.humidity = humidity
        self.luminosity = luminosity
        self.timestamp = timestamp

# um dicionário com o nome do dispositivo e uma lista de medidas relacionada à ele (histórico)
per_device_measurements: dict[str, list[Measurement]] = {}
# histórico geral de medidas 
measurements: list[tuple[str, Measurement]] = []

try:
    # tentando abrir o arquivo com os dados salvos
    with open("database", 'r') as file:
        # lendo as medidas 
            for data_dict in json.loads(file.read()):
                # identificando o dispositivo
                device = data_dict.pop("device")
                # transformando o restante do dicionário em um objeto Measurement
                measurement = Measurement(**data_dict)

                # adicionando as medidas ao histórico do dispositivo
                if device in per_device_measurements:
                    per_device_measurements.get(device, []).append(measurement)
                else:
                    per_device_measurements.update({device: [measurement]})

                # adicionando as medidas ao histórico geral de medidas do sistema
                measurements.append((device, measurement))
except FileNotFoundError: 
    # criando o arquivo caso ainda não exista
    file = open("database", "x")
    file.close()


# rota da página principal
@app.route('/')
def index():
    return app.send_static_file("index.html")

# rota para listar dispositivos
@app.route("/devices")
def route_list_devices():
    global per_device_measurements
    devices = []

    # listando todos os dispositivos 
    for key in per_device_measurements.keys():
        devices.append(key)

    return devices

# rota para listar as medidas de um dispositivo
@app.route("/devices/<device>")
def route_device_measurements(device):
    global per_device_measurements

    # pegando todas as medidas para o dispositivo solicitado
    device_history = []
    device_measurements = per_device_measurements.get(device, [])
    for measurement in device_measurements:
        # transformando em dicionário e colocando no histórico do dispositivo
        data_dict = {"device": device, "temperature": measurement.temperature, "humidity": measurement.humidity, "luminosity": measurement.luminosity, "timestamp": measurement.timestamp}
        device_history.append(data_dict)

    # serializando as medidas enviadas pelo dispositivo desejado
    json_data = json.dumps(device_history)
    return json_data

# rota de medidas
@app.route("/measurements", methods=["GET", "POST"])
def route_measurements():
    global measurements
    global per_device_measurements

    if request.method == "POST":
        # deserializando o json recebido
        data_dict = request.json
        # retirando e guarando o identificador do device
        device = data_dict.pop("device")
        # transformando o restante do dicionário em um objeto Measurement
        new_measurements = Measurement(**data_dict)

        # adicionando as novas medidas ao histórico do dispositivo
        if device in per_device_measurements:
            per_device_measurements.get(device, []).append(new_measurements)
        else:
            per_device_measurements.update({device: [new_measurements]})

        # adicionando as novas medidas ao histórico geral de medidas do sistema
        measurements.append((device, new_measurements))

        file = open("database", "w")
        file.write(measurement_history(measurements))

        return dict()
    else:
        return measurement_history(measurements)

def measurement_history(measurements):
    # lista para guardar os dicionários de cada medida
    measurements_history_list = []

    for (device, measurement) in measurements:
        # organiza os dados em um dicionário
        data_dict = {"device": device, "temperature": measurement.temperature, "humidity": measurement.humidity, "luminosity": measurement.luminosity, "timestamp": measurement.timestamp}
        # adiciona na lista
        measurements_history_list.append(data_dict)

    # serializa o json para enviar os dados requisitados como resposta
    return json.dumps(measurements_history_list)

app.run(host="0.0.0.0", port = 5000)