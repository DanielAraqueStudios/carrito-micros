/*
 * TEST 2: MOTORES DC CON PUENTE H
 * ================================
 * Prueba los dos motores traseros del robot
 * Control con puente H (L298N o TB6612)
 * 
 * CONFIGURACION:
 * - Motor Izquierdo (Motor A)
 * - Motor Derecho (Motor B)
 * - 1 Rueda loca delantera (sin motor)
 * 
 * CONEXIONES PUENTE H:
 * Motor A (Izquierdo):
 *   IN1 -> GPIO 15
 *   IN2 -> GPIO 16
 *   ENA -> GPIO 6 (PWM para velocidad)
 * 
 * Motor B (Derecho):
 *   IN3 -> GPIO 17
 *   IN4 -> GPIO 7
 *   ENB -> GPIO 4 (PWM para velocidad)
 * 
 * Alimentación:
 *   12V/VCC del puente H -> Batería 6-12V
 *   GND -> Común con ESP32
 *   5V del puente H -> No conectar (usa regulador externo)
 * 
 * INSTRUCCIONES:
 * 1. Elevar el robot (ruedas sin tocar el suelo)
 * 2. Subir código al ESP32
 * 3. Abrir Serial Monitor a 115200 baud
 * 4. Observar movimientos y mensajes
 */

// Pines Motor A (Izquierdo)
#define IN1 15
#define IN2 16
#define ENA 6

// Pines Motor B (Derecho)
#define IN3 17
#define IN4 7
#define ENB 4

// Velocidades de prueba
#define VELOCIDAD_BAJA 150
#define VELOCIDAD_MEDIA 200
#define VELOCIDAD_ALTA 255

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configurar pines como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Detener motores al inicio
  detenerMotores();
  
  Serial.println("================================");
  Serial.println("TEST MOTORES DC CON PUENTE H");
  Serial.println("================================");
  Serial.println("IMPORTANTE: Elevar el robot");
  Serial.println("Iniciando pruebas en 5 segundos...\n");
  delay(5000);
}

void loop() {
  // Secuencia de pruebas
  
  // Test 1: Avanzar
  Serial.println("\n>>> TEST 1: AVANZAR (3 velocidades)");
  avanzar(VELOCIDAD_BAJA);
  Serial.println("Velocidad BAJA (150)");
  delay(2000);
  
  avanzar(VELOCIDAD_MEDIA);
  Serial.println("Velocidad MEDIA (200)");
  delay(2000);
  
  avanzar(VELOCIDAD_ALTA);
  Serial.println("Velocidad ALTA (255)");
  delay(2000);
  
  detenerMotores();
  Serial.println("DETENIDO");
  delay(2000);
  
  // Test 2: Retroceder
  Serial.println("\n>>> TEST 2: RETROCEDER");
  retroceder(VELOCIDAD_MEDIA);
  Serial.println("Retrocediendo a velocidad media");
  delay(3000);
  
  detenerMotores();
  Serial.println("DETENIDO");
  delay(2000);
  
  // Test 3: Girar Izquierda
  Serial.println("\n>>> TEST 3: GIRAR IZQUIERDA");
  girarIzquierda(VELOCIDAD_MEDIA);
  Serial.println("Girando a la izquierda");
  delay(2000);
  
  detenerMotores();
  Serial.println("DETENIDO");
  delay(2000);
  
  // Test 4: Girar Derecha
  Serial.println("\n>>> TEST 4: GIRAR DERECHA");
  girarDerecha(VELOCIDAD_MEDIA);
  Serial.println("Girando a la derecha");
  delay(2000);
  
  detenerMotores();
  Serial.println("DETENIDO");
  delay(2000);
  
  // Test 5: Motor Izquierdo solo
  Serial.println("\n>>> TEST 5: MOTOR IZQUIERDO SOLO");
  motorIzquierdoAdelante(VELOCIDAD_MEDIA);
  Serial.println("Solo motor izquierdo");
  delay(2000);
  
  detenerMotores();
  delay(2000);
  
  // Test 6: Motor Derecho solo
  Serial.println("\n>>> TEST 6: MOTOR DERECHO SOLO");
  motorDerechoAdelante(VELOCIDAD_MEDIA);
  Serial.println("Solo motor derecho");
  delay(2000);
  
  detenerMotores();
  Serial.println("\n=== CICLO DE PRUEBAS COMPLETADO ===");
  Serial.println("Esperando 5 segundos para repetir...\n");
  delay(5000);
}

// ===== FUNCIONES DE MOVIMIENTO =====

void avanzar(int velocidad) {
  // Ambos motores hacia adelante
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void retroceder(int velocidad) {
  // Ambos motores hacia atrás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void girarIzquierda(int velocidad) {
  // Motor izquierdo atrás, motor derecho adelante
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void girarDerecha(int velocidad) {
  // Motor izquierdo adelante, motor derecho atrás
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void motorIzquierdoAdelante(int velocidad) {
  // Solo motor izquierdo
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, 0);
}

void motorDerechoAdelante(int velocidad) {
  // Solo motor derecho
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, velocidad);
}

void detenerMotores() {
  // Apagar ambos motores
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
