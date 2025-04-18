#include <WiFi.h>
#include <HTTPClient.h>

#define S0  14
#define S1  27
#define S2  26
#define S3  25
#define OUT 33

const char* ssid = "-";
const char* password = "-";
const char* serverUrl = "http://<ip-do-servidor>:5000/rgb";  // Troque pelo seu IP do servidor Flask

void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT); digitalWrite(S0, HIGH);
  pinMode(S1, OUTPUT); digitalWrite(S1, LOW);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  unsigned int red = pulseIn(OUT, LOW);

  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  unsigned int green = pulseIn(OUT, LOW);

  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  unsigned int blue = pulseIn(OUT, LOW);

  // Aqui você pode fazer algum ajuste de escala para garantir que os valores estejam entre 0 e 255
  // Por exemplo, se o sensor retornar valores muito altos, você pode fazer algo assim:
  red = map(red, 0, 1024, 0, 255);
  green = map(green, 0, 1024, 0, 255);
  blue = map(blue, 0, 1024, 0, 255);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"red\":" + String(red) + ",\"green\":" + String(green) + ",\"blue\":" + String(blue) + "}";
    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error in HTTP request");
    }

    http.end();
  }

  delay(1000);
}

