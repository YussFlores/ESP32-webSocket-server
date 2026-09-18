
const ESP32_PORT = "81";

const connectionText = document.getElementById("connectionText");
const temperatureText = document.getElementById("temperatureText");
const humidityText = document.getElementById("humidityText");
const lightText = document.getElementById("lightText");
const soilText = document.getElementById("soilText");
const fanStatus = document.getElementById("fanStatus");
const ledStatus = document.getElementById("ledStatus");
const waterStatus = document.getElementById("waterStatus");

socket = new WebSocket(`ws://${window.location.hostname}:${ESP32_PORT}`);
 
socket.onopen = function(event){
    console.log("Connection succesful");
    connectionText.textContent = "connected";
    connectionText.style.color = "green";
}

socket.onclose = function(event){
    console.log("Connection disconnected");
    connectionText.textContent = "disconnected";
    connectionText.style.color = "red";
}

socket.onmessage = function(event){
    data = JSON.parse(event.data);
    temperatureText.textContent = `${data.temperature}°C`;
    humidityText.textContent = `${data.humidity}%`;
    lightText.textContent = `${data.light}%`;
    soilText.textContent = `${data.soil}%`;

    fanStatus.textContent = data.fanState ? "ON" : "OFF";
    ledStatus.textContent = data.ledState ? "ON" : "OFF";
    waterStatus.textContent = data.waterState ? "ON" : "OFF";
}

socket.onerror = function(error){
    console.error(error);
}

function fanON(){
    if(socket.readyState === WebSocket.OPEN){
        socket.send("FAN_ON");
    }

}

function fanOFF(){
    if(socket.readyState === WebSocket.OPEN){
        socket.send("FAN_OFF");
    }
}

function ledON(){
    if(socket.readyState === WebSocket.OPEN){
        socket.send("LED_ON");
    }
}

function ledOFF(){
    if(socket.readyState === WebSocket.OPEN){
        socket.send("LED_OFF");
    }

}

function waterON(){
    if(socket.readyState === WebSocket.OPEN){
        socket.send("WATER_ON");
    }
}

function waterOFF(){
    if(socket.readyState === WebSocket.OPEN){
        socket.send("WATER_OFF");
    }

}