#include <WiFi.h>

const char* ssid = "Intelli-Pump System";
const char* password = "12345";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configurar el ESP32 en modo de acceso punto de acceso
  WiFi.softAP(ssid, password);

  // Obtener y mostrar la dirección IP del punto de acceso
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dirección IP del punto de acceso: ");
  Serial.println(IP);
}

void loop() {
  // Tu código principal aquí
}

