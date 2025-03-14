// Inclusão das Bibliotecas
#include <WiFi.h> // Biblioteca para conexão Wi-Fi
#include <HTTPClient.h> // Biblioteca para fazer requisições HTTP
#include <TinyGPS++.h> // Biblioteca para comunicação com o GPS

// Configuração das Credenciais Wi-Fi e Servidor
const char* ssid = "SSID"; // Nome da rede Wi-Fi
const char* password = "password"; // Senha do Wi-Fi
const char* server = "https://simple-gray-papyrus.glitch.me"; // URL do servidor

// Configuração do GPS
#define RXD2 16 // Pino RX do ESP32 conectado ao TX do GPS
#define TXD2 17 // Pino TX do ESP32 conectado ao RX do GPS
#define GPS_BAUD 9600 // Taxa de comunicação do GPS
TinyGPSPlus gps; // Instância do objeto GPS
HardwareSerial gpsSerial(2); // Configuração da porta serial 2 para comunicação com o GPS
// Função setup()
void setup() {
  Serial.begin(115200); // Inicia a comunicação serial para monitoramento
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2); // Inicia a comunicação com o GPS
  Serial.println("Serial 2 do GPS iniciada");

  // Conectando-se ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");
}
// Função loop()
void loop() {
  unsigned long start = millis(); // Marca o tempo inicial
  bool newData = false; // Variável para verificar se há novos dados do GPS

  // Leitura dos Dados do GPS
  while (millis() - start < 1000) {
    while (gpsSerial.available() > 0) { 
      if (gps.encode(gpsSerial.read())) {
        newData = true;
      }
    }
  }

  // Verificação da Localização
  if (newData && gps.location.isValid()) {
    float lat = gps.location.lat(); // Obtém a latitude
    float lon = gps.location.lng(); // Obtém a longitude

    // Envio dos Dados ao Servidor
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(server) + "/update?lat=" + String(lat, 6) + "&lon=" + String(lon, 6);
      http.begin(url);
      int httpCode = http.GET();
      http.end();

      // Verificação da Resposta HTTP
      if (httpCode > 0) {
        Serial.println("Localização enviada com sucesso!");
      } else {
        Serial.println("Falha ao enviar a localização!");
      }
    } else {
      // Tentativa de Reconexão ao Wi-Fi
      Serial.println("Wi-Fi desconectado, tentando reconectar...");
      WiFi.begin(ssid, password);
    }
  } else {
    Serial.println("Aguardando coordenadas válidas do GPS...");
  }

  // Atraso Antes do Novo Ciclo
  delay(5000);
}
