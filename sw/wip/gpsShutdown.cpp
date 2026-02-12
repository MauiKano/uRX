#include <Arduino.h>

// Pin Definitionen (wie besprochen)
#define GPS_EN_PIN  15  // Schaltet BC847 (Low-Side-Switch)
#define GPS_TX_PIN  16  // ESP32 TX -> GPS RX
#define GPS_RX_PIN  17  // ESP32 RX <- GPS TX

void setup() {
  Serial.begin(115200);
  
  // Initialisiere den Power-Pin (GPS ist am Anfang AUS)
  pinMode(GPS_EN_PIN, OUTPUT);
  digitalWrite(GPS_EN_PIN, LOW); 
}

void powerOnGPS() {
  Serial.println("GPS: Einschalten...");
  
  // 1. Masse zuschalten
  digitalWrite(GPS_EN_PIN, HIGH);
  
  // 2. Kurz warten, bis GPS stabil ist (Kapazitäten laden)
  delay(50);
  
  // 3. UART neu initialisieren
  Serial2.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  
  Serial.println("GPS: Aktiv.");
}

void powerOffGPS() {
  Serial.println("GPS: Vorbereitung zum Deaktivieren (Radio-Modus)...");
  
  // 1. UART stoppen
  Serial2.end();
  
  // 2. WICHTIG: Pins hochohmig machen (verhindert Phantom-Strom)
  pinMode(GPS_TX_PIN, INPUT);
  pinMode(GPS_RX_PIN, INPUT);
  
  // 3. Masse trennen (BC847 sperrt)
  digitalWrite(GPS_EN_PIN, LOW);
  
  Serial.println("GPS: Vollständig isoliert. Radio-Empfang ist nun störungsfrei.");
}

void loop() {
  // Beispiel-Logik:
  powerOnGPS();
  delay(10000); // 10 Sekunden Daten sammeln
  
  powerOffGPS();
  delay(60000); // 60 Sekunden Navtex-Empfang ohne GPS-Störungen
}
