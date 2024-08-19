#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* serverName = "http://192.168.x.x:8080";
String notas[] = {"C4", "D4", "E4", "F4", "G4", "A4", "B4"};
String nota = "C4";
int indiceNota = 0;
#define NUM_SENSORS 8
int trigPins[NUM_SENSORS] = {13, 14, 26, 33, 15, 16, 17, 19};
int echoPins[NUM_SENSORS] = {12, 27, 25, 32, 2, 4, 5, 18};
#define BUZZER_PIN 21
int notes[NUM_SENSORS] = {261, 293, 329, 349, 392, 440, 493, 523};
long distances[NUM_SENSORS];
int currentNote = -1;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado a la red Wi-Fi");
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  pinMode(BUZZER_PIN, OUTPUT);
}
void loop() {
  long closestDistance = 1000;
  int closestSensor = -1;
  for (int i = 0; i < NUM_SENSORS; i++) {
    long duration, distance;
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);
    duration = pulseIn(echoPins[i], HIGH);
    distance = (duration / 2) / 29.1;
    distances[i] = distance;
    if (distance < 20 && distance < closestDistance) {
      closestDistance = distance;
      closestSensor = i;
    }
    delay(10);
  }
  if (closestSensor != -1 && closestSensor != currentNote) {
    if (currentNote != -1) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "text/plain");
      http.POST("stop");
      http.end();
    }
    nota = notas[closestSensor];
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "text/plain");
    http.POST(nota);
    http.end();
    currentNote = closestSensor;
  } else if (closestSensor == -1) {
    if (currentNote != -1) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "text/plain");
      http.POST("stop");
      http.end();

      currentNote = -1;
    }
  }
  delay(10);
}
