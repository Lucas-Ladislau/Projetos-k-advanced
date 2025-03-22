const express = require("express");
const app = express();

let ambulances = {}; // Armazena a última localização de cada ambulância
let routes = {}; // Histórico de rota por ambulância
const colors = ["red", "blue", "green", "purple", "orange", "brown"]; // Cores para os rastros

app.get("/", (req, res) => {
  res.send(`
        <html lang="pt-BR">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Monitoramento de Ambulâncias</title>
                <link rel="stylesheet" href="https://unpkg.com/leaflet/dist/leaflet.css" />
                <script src="https://unpkg.com/leaflet/dist/leaflet.js"></script>
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        background-color: #f4f4f4;
                        color: #333;
                        margin: 0;
                        padding: 0;
                        display: flex;
                        flex-direction: column;
                        align-items: center;
                    }

                    h1 {
                        text-align: center;
                        color: #333;
                        margin-top: 20px;
                    }

                    #infoBox {
                        background: rgba(255, 255, 255, 0.8);
                        padding: 15px;
                        border-radius: 8px;
                        box-shadow: 0px 0px 15px rgba(0, 0, 0, 0.1);
                        position: relative;  /* Alterado de 'absolute' para 'relative' */
                        margin-top: 20px;    /* Adiciona um espaço acima do card */
                        width: 80%;
                        max-width: 500px;
                        font-size: 14px;
                        overflow-y: auto;
                        max-height: 300px;
                        z-index: 2;
                    }

                    #map {
                        width: 100%;
                        height: 500px;
                        margin-top: 20px;
                        z-index: 1; /* Mantém o mapa abaixo do card */
                    }

                    .ambulance-info {
                        padding: 8px;
                        margin: 4px 0;
                        background: #f9f9f9;
                        border-radius: 5px;
                        box-shadow: 0px 0px 5px rgba(0, 0, 0, 0.1);
                    }
                </style>
                <script>
                    var map;
                    var markers = {};
                    var polylines = {};
                    var ambulanceColors = {}; // Mapeia a cor de cada ambulância
                    var colorIndex = 0; // Índice para escolher cores diferentes

                    function initMap() {
                        map = L.map('map').setView([2.8235, -60.6758], 13);
                        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                            attribution: '&copy; OpenStreetMap contributors'
                        }).addTo(map);
                        updateMap();
                    }

                    function updateMap() {
                        fetch('/get-location')
                            .then(response => response.json())
                            .then(data => {
                                let infoBox = document.getElementById("infoBox");
                                infoBox.innerHTML = "<h3>Ambulâncias Ativas</h3>";

                                Object.keys(data).forEach((id, index) => {
                                    var location = { lat: data[id].lat, lng: data[id].lon };

                                    // Definir uma cor única para cada ambulância
                                    if (!ambulanceColors[id]) {
                                        ambulanceColors[id] = ["red", "blue", "green", "purple", "orange", "brown"][colorIndex % 6];
                                        colorIndex++;
                                    }

                                    if (!markers[id]) {
                                         var ambulanceIcon = L.divIcon({
                                            className: 'ambulance-icon',
                                            html: '<span style="font-size: 30px;">🚑</span>', // Aumenta o tamanho do emoji via CSS
                                            iconSize: [30, 30],
                                            iconAnchor: [10, 20],
                                            popupAnchor: [0, -0]
                                            });

                                        markers[id] = L.marker(location, { icon: ambulanceIcon, title: 'Ambulância ' + id })
                                            .addTo(map)
                                            .bindPopup("Ambulância " + id);
                                    } else {
                                        markers[id].setLatLng(location);
                                    }

                                    if (!polylines[id]) {
                                        polylines[id] = L.polyline(data[id].route, { color: ambulanceColors[id] }).addTo(map);
                                    } else {
                                        polylines[id].setLatLngs(data[id].route);
                                    }

                                    // Atualizar o painel de informações
                                    infoBox.innerHTML += "<div class='ambulance-info'><b>ID:</b> " + id + 
                                    " | <b>Lat:</b> " + location.lat.toFixed(6) + 
                                    " | <b>Lon:</b> " + location.lng.toFixed(6) + 
                                    " | <span style='color:" + ambulanceColors[id] + "'>⬤</span></div>";

                                });
                            })
                            .catch(error => console.error('Erro ao obter localização:', error));
                    }

                    window.onload = function() {
                        initMap();
                        setInterval(updateMap, 5000);
                    };
                </script>
            </head>
            <body>
                <h1>Localização das Ambulâncias</h1>
                <div id="infoBox"></div>
                <div id="map"></div>
            </body>
        </html>
    `);
});

app.get("/get-location", (req, res) => {
    res.json(ambulances);
});

// Atualiza a localização de uma ambulância específica
app.get("/update", (req, res) => {
    let { id, lat, lon } = req.query;
    if (!id || !lat || !lon) {
        return res.send("Erro: Passe os parâmetros id, lat e lon.");
    }
    lat = parseFloat(lat);
    lon = parseFloat(lon);
    if (!ambulances[id]) {
        ambulances[id] = { lat, lon, route: [] };
    }
    ambulances[id].lat = lat;
    ambulances[id].lon = lon;
    ambulances[id].route.push([lat, lon]);
    console.log(`Ambulância ${id} atualizada: ${lat}, ${lon}`);
    res.send(`Localização da ambulância ${id} atualizada!`);
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Servidor rodando na porta ${PORT}`));
