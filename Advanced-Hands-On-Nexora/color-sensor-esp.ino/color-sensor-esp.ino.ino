#include <WiFi.h>
#include <HTTPClient.h>

#define S0  14
#define S1  27
#define S2  26
#define S3  25
#define OUT 33

#define AD8232_OUTPUT_PIN  5
#define AD8232_SDN_PIN     21

// Configurações da Wi-Fi e servidor
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
const char* serverUrl = "http://SEU_SERVIDOR:5000/rgb";  // Ex: http://192.168.0.100:5000/rgb

// Limites de calibração (ajuste de acordo com seu sensor)
const int MIN_PULSE = 20;
const int MAX_PULSE = 500;

void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT); digitalWrite(S0, HIGH);
  pinMode(S1, OUTPUT); digitalWrite(S1, LOW);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  pinMode(AD8232_SDN_PIN, OUTPUT);
  digitalWrite(AD8232_SDN_PIN, HIGH);  // Liga o sensor de batimentos

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

// Função para mapear os pulsos para RGB (0 a 255)
int mapRGB(unsigned int pulse) {
  pulse = constrain(pulse, MIN_PULSE, MAX_PULSE);
  return map(pulse, MIN_PULSE, MAX_PULSE, 255, 0);
}

void loop() {
  // Leitura RED
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  unsigned int redPulse = pulseIn(OUT, LOW);
  int red = mapRGB(redPulse);

  // Leitura GREEN
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  unsigned int greenPulse = pulseIn(OUT, LOW);
  int green = mapRGB(greenPulse);

  // Leitura BLUE
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  unsigned int bluePulse = pulseIn(OUT, LOW);
  int blue = mapRGB(bluePulse);

  // Batimento cardíaco (valor analógico bruto)
  int batimento = analogRead(AD8232_OUTPUT_PIN);

  Serial.printf("RGB: (%d, %d, %d) | Batimento: %d\n", red, green, blue, batimento);

  // Envia os dados
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"red\":" + String(red) +
                  ",\"green\":" + String(green) +
                  ",\"blue\":" + String(blue) +
                  ",\"batimento\":" + String(batimento) + "}";

    int httpResponseCode = http.POST(json);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Resposta do servidor: " + response);
    } else {
      Serial.println("Erro ao enviar HTTP POST");
    }
    http.end();
  }

  delay(1000);
}

