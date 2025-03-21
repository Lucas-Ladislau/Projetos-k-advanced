#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Starlink_CIT"; // Substitua pelo seu SSID
const char* password = "Ufrr@2024Cit"; // Substitua pela sua senha

int ecgPin = 34;
int valorECG = 0;

String servidorURL = "http://192.168.1.76:5000/dados"; // URL do servidor

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    // Espera até conectar ao Wi-Fi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi!");
    Serial.print("IP_Address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    valorECG = analogRead(ecgPin);
    Serial.println(valorECG);

    // Criar JSON com os dados do ECG
    String jsonPayload = "{\"ecg\": " + String(valorECG) + "}";

    // Enviar dados para o servidor
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(servidorURL);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            Serial.println("Dados enviados com sucesso!");
        } else {
            Serial.println("Erro ao enviar dados: " + String(httpResponseCode));
        }
        http.end();
    }
    delay(100); // Delay para a próxima leitura
}
