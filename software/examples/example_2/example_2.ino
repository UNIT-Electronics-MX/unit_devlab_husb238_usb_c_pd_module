/*
 * HUSB238 USB-C PD Module - Example 2: Basic Usage
 * 
 * Ejemplo básico: detectar voltajes y cambiar a 15V
 */

#include <Wire.h>
#include "Adafruit_HUSB238.h"

#define I2C_SDA 6
#define I2C_SCL 7

Adafruit_HUSB238 husb238;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!husb238.begin(HUSB238_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("ERROR: No se encontró HUSB238");
    while (1) delay(100);
  }

  Serial.println("HUSB238 OK\n");
}

void loop() {
  if (!husb238.isAttached()) {
    Serial.println("Sin conexión USB-C");
    delay(2000);
    return;
  }

  // Mostrar voltajes disponibles
  Serial.println("Voltajes disponibles:");
  if (husb238.isVoltageDetected(PD_SRC_5V))  Serial.println("  5V");
  if (husb238.isVoltageDetected(PD_SRC_9V))  Serial.println("  9V");
  if (husb238.isVoltageDetected(PD_SRC_12V)) Serial.println("  12V");
  if (husb238.isVoltageDetected(PD_SRC_15V)) Serial.println("  15V");
  if (husb238.isVoltageDetected(PD_SRC_20V)) Serial.println("  20V");
  
  // Voltaje actual
  Serial.print("\nVoltaje actual: ");
  Serial.print(husb238.getPDSrcVoltage());
  Serial.println("V");

  // Cambiar a 15V si está disponible
  if (husb238.isVoltageDetected(PD_SRC_15V)) {
    Serial.println("\nCambiando a 15V...");
    husb238.selectPD(PD_SRC_15V);
    husb238.requestPD();
    delay(500);
    Serial.print("Nuevo voltaje: ");
    Serial.print(husb238.getPDSrcVoltage());
    Serial.println("V");
  }
  
  Serial.println("\n---\n");
  delay(5000);
}
