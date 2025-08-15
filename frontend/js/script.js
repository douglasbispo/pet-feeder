// Configurações do MQTT
const host = "broker.hivemq.com";
const port = 8000;
const clientId = "web-client-" + parseInt(Math.random() * 100);
const topic = "pet_feeder/comando";
const message = "acionar";

// Elementos da interface
const statusText = document.getElementById("statusText");
const toggleBtn = document.getElementById("toggleBtn");
const feedBtn = document.getElementById("feedBtn");

let client;

// Função para iniciar a conexão MQTT
function connect() {
    client = new Paho.MQTT.Client(host, port, clientId);
    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;

    client.connect({ 
        onSuccess: onConnect, 
        onFailure: onFailure,
        useSSL: false
    });
}

// Callback para sucesso na conexão
function onConnect() {
    console.log("Conectado ao broker MQTT.");
    statusText.innerText = "Conectado";
    statusText.className = "status-connected";
    toggleBtn.innerText = "Desconectar";
    toggleBtn.className = "toggle-btn connected";
}

// Callback para falha na conexão
function onFailure(responseObject) {
    console.log("Conexão falhou: " + responseObject.errorMessage);
    statusText.innerText = "Falha na Conexão";
    statusText.className = "status-disconnected";
    toggleBtn.innerText = "Conectar";
    toggleBtn.className = "toggle-btn";
}

// Callback para conexão perdida
function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
        console.log("Conexão Perdida: " + responseObject.errorMessage);
    }
    statusText.innerText = "Desconectado";
    statusText.className = "status-disconnected";
    toggleBtn.innerText = "Conectar";
    toggleBtn.className = "toggle-btn";
}

// Callback para quando uma mensagem chega
function onMessageArrived(message) {
    console.log("Mensagem recebida: " + message.payloadString);
}

// Lógica para acionar o alimentador
function acionarAlimentador() {
    if (client && client.isConnected()) {
        var msg = new Paho.MQTT.Message(message);
        msg.destinationName = topic;
        client.send(msg);
        console.log("Comando enviado: " + message);
    } else {
        alert("Não foi possível enviar o comando. Conecte-se ao broker primeiro.");
    }
}

// Lógica para o botão conectar e desconectar
function toggleConnection() {
    if (client && client.isConnected()) {
        client.disconnect();
        statusText.innerText = "Desconectando...";
        statusText.className = "status-disconnected";
    } else {
        statusText.innerText = "Conectando...";
        statusText.className = "status-disconnected";
        connect();
    }
}