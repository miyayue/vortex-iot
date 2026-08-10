const measurements_div = document.getElementById("measurements");
const device_select = document.getElementById("device-select");
const measurements_count = document.getElementById("measurements_count");

// função para solicitar as medidas
function get_measurements() {
    // checando qual dispositivo está selecionado 
    const selected = device_select.value;

    // se "Todos" estiver selecionado, serão requisitadas as medidas enviadas por todos os
    // dispositivos. se um dispositivo específico estiver selecionado, serão solicitadas 
    // apenas as medidas enviadas por este dispositivo
    let request_info;
    if (selected == "Todos") {
        request_info = "/measurements"
    } else {
        request_info = "/devices/".concat(selected);
    }

    const request = new Request(request_info);

    // retornando a resposta à requisição
    return fetch(request).then((response) => response.json());
}

// função para atualizar as medidas na página
function update_measurements(measurements) {
    // exibe a quantidade de medidas 
    measurements_count.innerText = measurements.length;

    measurements_div.innerHTML = "";
    measurements.reverse();
    for (let measurement of measurements) {
        // criando um div para cada instância de medida
        const measurement_div = document.createElement("div");
        measurement_div.className = "measurement";

        // função que cria os elementos 'nome da medida: medida' e os adiciona ao div
        const create_property = (name, value, unit) => {
            const div = document.createElement("div");
            const name_label = document.createElement("label");
            const value_label = document.createElement("label");

            div.className = "property";
            div.appendChild(name_label);
            div.appendChild(value_label);

            name_label.innerText = name;
            value_label.innerText = value + unit;

            measurement_div.appendChild(div);
        }

        create_property("Dispositivo:", measurement.device, "");
        create_property("Temperatura:", measurement.temperature, " °C");
        create_property("Umidade:", measurement.humidity, " %");
        create_property("Luminosidade:", measurement.luminosity, "");
        create_property("Horário:", measurement.timestamp, "");

        // adiciona as medidas à lista de medidas da página
        measurements_div.appendChild(measurement_div);
    }
}

// função que atualiza os dispositivos disponíveis
function update_select() {
    // realiza a requisição dos dispositivos conhecidos pelo sistema
    const request = new Request("/devices");
    fetch(request)
        .then((response) => response.json())
        .then((devices) => {
            // armazena qual dispositivo está selecionado atualmente
            const selected_device = device_select.selectedIndex;

            device_select.innerHTML = "";

            // cria novamente a opção todos
            const option_all = document.createElement("option");
            option_all.innerText = "Todos";
            device_select.appendChild(option_all);

            // cria a opção de cada um dos dispositivos registrados
            for (let device of devices) {
                const option = document.createElement("option");
                option.innerText = device;

                device_select.appendChild(option);
            }

            // coloca o dispositivo selecionado novamente para que a página não volte
            // para o dispositivo padrão ("Todos")
            device_select.selectedIndex = selected_device;
        })
}

// ao iniciar a página, requisita as medidas e os dispositivos
get_measurements().then((measurements) => update_measurements(measurements));
update_select();

// ao modificar o dispositivo selecionado, atualiza as medidas na página para mostrar
// apenas as medidas enviadas por ele
device_select.addEventListener("change", () => {
    get_measurements().then((measurements) => update_measurements(measurements));
});

// faz com que a página atualize as medidas e dispositivos a cada 5 segundos para manter a lista 
// atualizada 
setInterval(() => {
    get_measurements().then((measurements) => update_measurements(measurements));
    update_select();
}, 5000);
