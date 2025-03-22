#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Starlink_CIT";  // Substitua pelo seu SSID
const char* password = "Ufrr@2024Cit";

HardwareSerial mySerial(1);  // Usando UART1 do ESP32 (GPIO16, GPIO17)
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int ecgPin = 34;
bool autenticado = false;
String servidorURL = "http://192.168.1.76:5000/dados";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    mySerial.begin(57600, SERIAL_8N1, 16, 17);  // Configuração UART para biometria

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    finger.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Sensor de biometria detectado!");
    } else {
        Serial.println("Falha ao encontrar sensor de biometria.");
        while (1);  // Loop infinito se o sensor não for encontrado
    }
}

void loop() {
    if (!autenticado) {
        Serial.println("Aguardando autenticação...");
        autenticado = verificarDigital();
        if (!autenticado) {
            delay(1000);  // Evita flood de mensagens no Serial
            return;       // Retorna para evitar execução do código abaixo
        }
    }

    // Só coleta e envia os dados se o usuário estiver autenticado
    int valorECG = analogRead(ecgPin);
    Serial.println("ECG: " + String(valorECG));

    // Agora que o usuário está autenticado, envia o ID do usuário e o valor do ECG
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(servidorURL);
        http.addHeader("Content-Type", "application/json");

        // Construindo o payload JSON com o valor do ECG e o ID do usuário
        String jsonPayload = "{\"ecg\": " + String(valorECG) + ", \"id\": " + String(finger.fingerID) + "}";
        
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            Serial.println("Dados enviados com sucesso!");
        } else {
            Serial.println("Erro ao enviar dados: " + String(httpResponseCode));
        }
        http.end();
    } else {
        Serial.println("WiFi desconectado, tentando reconectar...");
        WiFi.begin(ssid, password);
    }

    delay(100);  // Pequeno delay para evitar consumo excessivo
}

// Função para validar a digital
bool verificarDigital() {
    Serial.println("Posicione o dedo...");
    int resultado = finger.getImage();
    if (resultado != FINGERPRINT_OK) return false;

    resultado = finger.image2Tz();
    if (resultado != FINGERPRINT_OK) return false;

    resultado = finger.fingerFastSearch();
    if (resultado == FINGERPRINT_OK) {
        Serial.println("Usuário autenticado!");
        return true;
    } else {
        Serial.println("Autenticação falhou!");
        return false;
    }
}
