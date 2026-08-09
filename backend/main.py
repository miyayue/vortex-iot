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

@app.route('/')
def index():
    return app.send_static_file("index.html")

@app.route('/<path:path>')
def static_file(path):
    return app.send_static_file(path)

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

    device_history = []
    device_measurements = per_device_measurements.get(device, [])
    for measurement in device_measurements:
        data_dict = {"device": device, "temperature": measurement.temperature, "humidity": measurement.humidity, "luminosity": measurement.luminosity, "timestamp": measurement.timestamp}
        device_history.append(data_dict)

    # pegando e serializando as medidas enviadas pelo dispositivo desejado
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

        return dict()
    else:
        # lista para guardar os dicionários de cada medida
        measurements_history_list = []

        for (device, measurement) in measurements:
            # organiza os dados em um dicionário
            data_dict = {"device": device, "temperature": measurement.temperature, "humidity": measurement.humidity, "luminosity": measurement.luminosity, "timestamp": measurement.timestamp}
            # adiciona na lista
            measurements_history_list.append(data_dict)

        # serializa o json para enviar os dados requisitados como resposta
        json_data = json.dumps(measurements_history_list)
        return json_data

app.run(host="0.0.0.0", port = 5000)