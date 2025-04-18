#include <WiFi.h>
#include <HTTPClient.h>

#define S0  14
#define S1  27
#define S2  26
#define S3  25
#define OUT 33

#define AD8232_OUTPUT_PIN  5    // Pino de leitura do sinal de batimento cardíaco (D5)
#define AD8232_SDN_PIN     21   // Pino de controle de desligamento (D21)

// Configurações da Wi-Fi e servidor
const char* ssid = "-";
const char* password = "-";
const char* serverUrl = "-";  // Troque para o IP do servidor Flask

void setup() {
  Serial.begin(115200);

  // Configuração dos pinos do TCS3200
  pinMode(S0, OUTPUT); digitalWrite(S0, HIGH);
  pinMode(S1, OUTPUT); digitalWrite(S1, LOW);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Configuração do pino SDN para o AD8232
  pinMode(AD8232_SDN_PIN, OUTPUT);
  digitalWrite(AD8232_SDN_PIN, HIGH);  // Ativa o AD8232 (desliga quando LOW)

  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  // Leitura do sensor TCS3200 (RGB)
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  unsigned int red = pulseIn(OUT, LOW);

  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  unsigned int green = pulseIn(OUT, LOW);

  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  unsigned int blue = pulseIn(OUT, LOW);

  // Leitura do sinal do AD8232 (batimento cardíaco)
  int batimento = analogRead(AD8232_OUTPUT_PIN);  // Leitura do sinal analógico do AD8232

  // Enviar os dados para o servidor se Wi-Fi estiver conectado
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Montando o JSON com os dados dos sensores
    String json = "{\"red\":" + String(red) + ",\"green\":" + String(green) + ",\"blue\":" + String(blue) +
                  ",\"batimento\":" + String(batimento) + "}";

    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error in HTTP request");
    }

    http.end();
  }

  delay(1000);  // Espera de 1 segundo entre as leituras
}


