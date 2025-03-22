#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int idAtual = 1;  // ID inicial (começa em 1 e será incrementado a cada novo usuário registrado)

void setup() {
  Serial.begin(115200);
  mySerial.begin(57600, SERIAL_8N1, 16, 17);

  finger.begin(57600);
  if (!finger.verifyPassword()) {
    Serial.println("Erro: Sensor não encontrado!");
    while (1);
  }

  Serial.println("Sensor de digitais pronto.");
}

void loop() {
  Serial.println("Escolha uma opção:");
  Serial.println("1 - Registrar nova digital");
  Serial.println("2 - Autenticar digital");
  while (Serial.available() == 0);  // Aguarda a escolha do usuário
  int opcao = Serial.parseInt();

  if (opcao == 1) {
    registrarDigital();
  } else if (opcao == 2) {
    autenticarDigital();
  }
  delay(2000);
}

void registrarDigital() {
  int resultado = -1;
  Serial.println("Posicione o dedo para registrar.");

  // Aguarda que o usuário posicione o dedo
  resultado = finger.getImage();
  if (resultado != FINGERPRINT_OK) {
    Serial.println("Falha na captura da digital.");
    return;
  }

  resultado = finger.image2Tz(1);
  if (resultado != FINGERPRINT_OK) {
    Serial.println("Falha ao converter a imagem para template.");
    return;
  }

  Serial.println("Agora posicione o mesmo dedo novamente.");

  // Espera para o segundo toque do dedo
  resultado = finger.getImage();
  if (resultado != FINGERPRINT_OK) {
    Serial.println("Falha na captura da digital.");
    return;
  }

  resultado = finger.image2Tz(2);
  if (resultado != FINGERPRINT_OK) {
    Serial.println("Falha ao converter a imagem para template.");
    return;
  }

  resultado = finger.createModel();
  if (resultado != FINGERPRINT_OK) {
    Serial.println("Falha ao criar o modelo de impressão digital.");
    return;
  }

  Serial.print("Modelo de digital criado, agora salvando ID: ");
  Serial.println(idAtual);  // Exibe o ID atual para o usuário

  resultado = finger.storeModel(idAtual);  // Armazena o modelo no sensor com o ID fornecido
  if (resultado == FINGERPRINT_OK) {
    Serial.println("Digital registrada com sucesso!");

    // Incrementa o ID para o próximo usuário
    idAtual++;
  } else {
    Serial.println("Falha ao salvar a digital.");
  }
}

void autenticarDigital() {
  Serial.println("Posicione o dedo para autenticação...");

  int resultado = finger.getImage();
  if (resultado != FINGERPRINT_OK) {
    Serial.println("Falha na captura da digital.");
    return;
  }

  resultado = finger.image2Tz();
  if (resultado != FINGERPRINT_OK) {
    Serial.println("Falha ao converter a imagem para template.");
    return;
  }

  resultado = finger.fingerFastSearch();
  if (resultado == FINGERPRINT_OK) {
    Serial.print("Usuário autenticado! ID: ");
    Serial.println(finger.fingerID);  // ID do usuário autenticado
  } else {
    Serial.println("Falha na autenticação!");
  }
}
