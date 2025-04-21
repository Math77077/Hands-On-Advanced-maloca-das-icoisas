#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "CIT_Alunos";
const char* password = "----";

// Server endpoint (change this to your Python server address)
const char* serverName = "----/data";  // Altere o endereço para o endpoint correto

// CallMeBot API credentials
const String phone_number = "----";  // Substitua por seu número (incluindo o código de país)
const String apikey = "---";        // Substitua pela sua chave API do CallMeBot

// TCS3200 pins
#define S0 14
#define S1 27
#define S2 26
#define S3 25
#define sensorOut 33

// AD8232 pins
#define LO_PLUS 19
#define LO_MINUS 18
#define HEART_OUTPUT 34

// Função para codificar a URL
String urlEncode(String str) {
  String encoded = "";
  for (unsigned int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (c == ' ') {
      encoded += "%20";
    } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
      encoded += c;
    } else {
      encoded += "%" + String(c, HEX);
    }
  }
  return encoded;
}

void sendMessageViaCallMeBot(String message) {
  HTTPClient http;
  String encodedMessage = urlEncode(message);  // Codificar a mensagem antes de enviar
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&text=" + encodedMessage + "&apikey=" + apikey;
  
  Serial.println("Enviando para CallMeBot: " + url); // Log da URL para depuração

  http.begin(url);  // Inicia a requisição HTTP
  int httpResponseCode = http.GET();  // Envia a requisição GET para o CallMeBot
  
  Serial.print("Código da resposta HTTP: ");
  Serial.println(httpResponseCode);

  // Depurar a resposta do CallMeBot
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Resposta do CallMeBot: " + response);
    
    if (response.indexOf("error") != -1) {
      Serial.println("Erro ao enviar mensagem: " + response);
    } else {
      Serial.println("Mensagem enviada com sucesso!");
    }
  } else {
    Serial.println("Erro na comunicação com CallMeBot: " + String(httpResponseCode));
  }
  
  http.end();  // Finaliza a requisição HTTP
}

void setup() {
  Serial.begin(115200);

  // TCS3200 pin setup
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // AD8232 pins
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);
  pinMode(HEART_OUTPUT, INPUT);

  // Set TCS3200 scaling
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Conecta ao Wi-Fi
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
  // Verifica se o Wi-Fi está conectado
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado. Tentando reconectar...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi reconectado.");
  }

  // Read RGB from TCS3200
  int red = readColor('R');
  int green = readColor('G');
  int blue = readColor('B');

  // Leitura do batimento cardíaco (valor do sinal AD8232)
  int heartSignal = analogRead(HEART_OUTPUT); // AD8232 signal

  // Check lead-off
  bool loPlus = digitalRead(LO_PLUS);
  bool loMinus = digitalRead(LO_MINUS);
  bool leadsOff = loPlus || loMinus;

  if (leadsOff) {
    Serial.println("Leads are off! Connect sensors properly.");
  }

  Serial.printf("R: %d, G: %d, B: %d, Heartbeat Signal: %d\n", red, green, blue, heartSignal);

  // Verificar icterícia via GET no servidor
  bool jaundice = false;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);  // Inicia a requisição GET ao servidor Python
    int httpResponseCode = http.GET();  // Envia a requisição GET

    if (httpResponseCode == 200) {
      String response = http.getString();
      Serial.println("Resposta do servidor: " + response);

      // Verifica se a resposta contém "jaundice": 1 (Icterícia detectada)
      if (response.indexOf("\"jaundice\":1") != -1) {
        jaundice = true;
      } else {
        jaundice = false;  // Se não encontrar "jaundice": 1, não é icterícia
      }
    } else {
      Serial.println("Erro ao fazer requisição ao servidor: " + String(httpResponseCode));
    }

    http.end();  // Finaliza a requisição HTTP
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Enviar alerta caso icterícia seja detectada
  if (jaundice) {
    String alertMessage = "Alerta: Icterícia detectada!";
    Serial.println(alertMessage);  // Print alert to Serial Monitor
    
    // Enviar mensagem para o WhatsApp via CallMeBot
    sendMessageViaCallMeBot(alertMessage);
  }

  // Send to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String payload = String("{\"red\":") + red + ",\"green\":" + green +
                     ",\"blue\":" + blue + ",\"heartbeat\":" + heartSignal + "}" ;

    int httpResponseCode = http.POST(payload);

    Serial.print("POST Response: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(2000); // Send data every 2s
}
