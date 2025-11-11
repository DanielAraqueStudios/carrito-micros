/*
 * TEST 5: SERVOMOTOR
 * ==================
 * Prueba simple del servomotor (SG90 o similar)
 * Movimiento de 0° a 180° para el sensor ultrasónico
 * 
 * CONEXIONES:
 * Servo Cable Naranja (Señal) -> GPIO 13
 * Servo Cable Rojo (VCC)      -> 5V
 * Servo Cable Marrón (GND)    -> GND
 * 
 * NOTA IMPORTANTE:
 * - El servo puede consumir hasta 500mA
 * - Si el ESP32 se reinicia, usar fuente externa para el servo
 * - Conectar GND común entre ESP32 y fuente externa
 * 
 * INSTRUCCIONES:
 * 1. Conectar servo según diagrama
 * 2. Subir código al ESP32
 * 3. Abrir Serial Monitor a 115200 baud
 * 4. Observar movimientos del servo
 */

#include <ESP32Servo.h>

// Pin del servo
#define SERVO_PIN 13

// Crear objeto servo
Servo miServo;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("===========================");
  Serial.println("TEST SERVOMOTOR");
  Serial.println("===========================");
  
  // Configurar servo
  miServo.attach(SERVO_PIN, 500, 2400); // Pin, min pulse, max pulse
  
  Serial.println("Servo conectado en GPIO 13");
  Serial.println("Iniciando pruebas...\n");
  delay(2000);
  
  // Centrar servo al inicio
  miServo.write(90);
  Serial.println("Servo centrado (90°)");
  delay(1000);
}

void loop() {
  // TEST 1: Barrido completo lento
  Serial.println("\n>>> TEST 1: Barrido 0° a 180° (lento)");
  for (int angulo = 0; angulo <= 180; angulo += 10) {
    miServo.write(angulo);
    Serial.print("Ángulo: ");
    Serial.print(angulo);
    Serial.println("°");
    delay(300);
  }
  delay(1000);
  
  // TEST 2: Barrido inverso lento
  Serial.println("\n>>> TEST 2: Barrido 180° a 0° (lento)");
  for (int angulo = 180; angulo >= 0; angulo -= 10) {
    miServo.write(angulo);
    Serial.print("Ángulo: ");
    Serial.print(angulo);
    Serial.println("°");
    delay(300);
  }
  delay(1000);
  
  // TEST 3: Posiciones clave
  Serial.println("\n>>> TEST 3: Posiciones clave");
  
  Serial.println("Derecha (0°)");
  miServo.write(0);
  delay(1500);
  
  Serial.println("Centro (90°)");
  miServo.write(90);
  delay(1500);
  
  Serial.println("Izquierda (180°)");
  miServo.write(180);
  delay(1500);
  
  Serial.println("Centro (90°)");
  miServo.write(90);
  delay(1500);
  
  // TEST 4: Barrido rápido
  Serial.println("\n>>> TEST 4: Barrido rápido continuo");
  for (int i = 0; i < 3; i++) {
    Serial.print("Ciclo ");
    Serial.println(i + 1);
    
    for (int angulo = 0; angulo <= 180; angulo += 5) {
      miServo.write(angulo);
      delay(15);
    }
    
    for (int angulo = 180; angulo >= 0; angulo -= 5) {
      miServo.write(angulo);
      delay(15);
    }
  }
  
  // Centrar y pausar
  miServo.write(90);
  Serial.println("\n=== CICLO COMPLETADO ===");
  Serial.println("Esperando 3 segundos...\n");
  delay(3000);
}
