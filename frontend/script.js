const measurements_div = document.getElementById("measurements");
const device_select = document.getElementById("device-select");
const measurements_count = document.getElementById("measurements_count");

function get_measurements() {
    const selected = device_select.value;

    let request_info;
    if (selected == "Todos") {
        request_info = "/measurements"
    } else {
        request_info = "/devices/".concat(selected);
    }

    const request = new Request(request_info);

    return fetch(request).then((response) => response.json());
}

function update_measurements(measurements) {
    measurements_count.innerText = measurements.length;

    measurements_div.innerHTML = "";

    measurements.reverse();
    for (let measurement of measurements) {
        const measurement_div = document.createElement("div");
        measurement_div.className = "measurement";

        const create_property = (name, value) => {
            const div = document.createElement("div");
            const name_label = document.createElement("label");
            const value_label = document.createElement("label");

            div.className = "property";
            div.appendChild(name_label);
            div.appendChild(value_label);

            name_label.innerText = name;
            value_label.innerText = value;

            measurement_div.appendChild(div);
        }

        create_property("Dispositivo:", measurement.device);
        create_property("Temperatura:", measurement.temperature);
        create_property("Umidade:", measurement.humidity);
        create_property("Luminosidade:", measurement.luminosity);
        create_property("Horário:", measurement.timestamp);

        measurements_div.appendChild(measurement_div);
    }
}


function update_select() {
    const request = new Request("/devices");
    fetch(request)
        .then((response) => response.json())
        .then((devices) => {
            const selected_device = device_select.selectedIndex;

            device_select.innerHTML = "";

            const option_all = document.createElement("option");
            option_all.innerText = "Todos";
            device_select.appendChild(option_all);

            for (let device of devices) {
                const option = document.createElement("option");
                option.innerText = device;

                device_select.appendChild(option);
            }

            device_select.selectedIndex = selected_device;
        })
}


get_measurements().then((measurements) => update_measurements(measurements));
update_select();

device_select.addEventListener("change", () => {
    get_measurements().then((measurements) => update_measurements(measurements));
});

setInterval(() => {
    get_measurements().then((measurements) => update_measurements(measurements));
    update_select();
}, 5000);
