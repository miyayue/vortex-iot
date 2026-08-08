from flask import Flask, request

app = Flask(__name__)

# rota para a home da aplicação
@app.route("/")
def index():
    return "index page"

# rota para a página de dispositivos
@app.route("/devices")
def show_devices():
    return "devices page"

# rota para a página do dispositivo com o id especificado
@app.route("/devices/<int:device_id>")
def show_device(device_id):
    return f"device {device_id} page"

# rota de medidas
@app.route("/measurements", methods=["GET", "POST"])
def measurements():
    if request.method == "POST":
        return "measurements update"
    else:
        return "measurements page"

app.run(port = 5000)