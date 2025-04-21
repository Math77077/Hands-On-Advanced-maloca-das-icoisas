#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "CIT_Alunos";
const char* password = "-";

// Server endpoint (change this to your Python server address)
const char* serverName = "-";

// TCS3200 pins
#define S0 14
#define S1 27
#define S2 26
#define S3 25
#define sensorOut 33

// AD8232 pins
//#define SDN 21
#define LO_PLUS 19
#define LO_MINUS 18
#define HEART_OUTPUT 34

void setup() {
  Serial.begin(115200);

  // TCS3200 pin setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // AD8232 pins
  //pinMode(SDN, OUTPUT);
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);
  pinMode(HEART_OUTPUT, INPUT);

  // Turn on AD8232
  //digitalWrite(SDN, HIGH);

  // Set TCS3200 scaling
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected.");
}

int readColor(char color) {
  if (color == 'R') {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
  } else if (color == 'G') {
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
  } else if (color == 'B') {
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
  }
  delay(100);
  return pulseIn(sensorOut, LOW);
}

void loop() {
  // Read RGB from TCS3200
  int red = readColor('R');
  int green = readColor('G');
  int blue = readColor('B');

  // Read heartbeat
  int heartbeat = analogRead(HEART_OUTPUT); // AD8232 signal

  // Check lead-off
  bool loPlus = digitalRead(LO_PLUS);
  bool loMinus = digitalRead(LO_MINUS);
  bool leadsOff = loPlus || loMinus;

  if (leadsOff) {
    Serial.println("Leads are off! Connect sensors properly.");
    heartbeat = 0;
  }

  Serial.printf("R: %d, G: %d, B: %d, Heartbeat: %d\n", red, green, blue, heartbeat);

  // Send to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String payload = String("{\"red\":") + red + ",\"green\":" + green +
                     ",\"blue\":" + blue + ",\"heartbeat\":" + heartbeat + "}";

    int httpResponseCode = http.POST(payload);

    Serial.print("POST Response: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(2000); // Send data every 2s
}
