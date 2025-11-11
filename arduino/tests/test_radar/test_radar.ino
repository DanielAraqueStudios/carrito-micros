/*
 * TEST 6: RADAR - SERVO + HC-SR04
 * ================================
 * Sistema de radar usando servomotor y sensor ultrasónico
 * El servo barre de 0° a 180° mientras el sensor mide distancias
 * 
 * CONFIGURACION:
 * - Servomotor con HC-SR04 montado encima
 * - Barrido de 180° para detección de obstáculos
 * - Datos enviados por Serial para visualización
 * 
 * CONEXIONES:
 * HC-SR04:
 *   VCC  -> 5V
 *   GND  -> GND
 *   TRIG -> GPIO 5
 *   ECHO -> GPIO 18
 * 
 * Servomotor:
 *   Señal (Naranja) -> GPIO 13
 *   VCC (Rojo)      -> 5V (preferible fuente externa)
 *   GND (Marrón)    -> GND común
 * 
 * NOTA:
 * Si el ESP32 se reinicia al mover el servo, usar fuente
 * externa de 5V/1A para alimentar el servo (GND común).
 * 
 * INSTRUCCIONES:
 * 1. Montar HC-SR04 sobre el servo
 * 2. Conectar según diagrama
 * 3. Subir código al ESP32
 * 4. Abrir Serial Monitor a 115200 baud
 * 5. Observar barrido y detección de obstáculos
 * 
 * FORMATO DE SALIDA SERIAL:
 * angulo,distancia
 * Ejemplo: 45,23.5 (45° y 23.5cm)
 */

#include <ESP32Servo.h>

// Pines
#define SERVO_PIN 13
#define TRIG_PIN 5
#define ECHO_PIN 18

// Configuración del radar
#define ANGULO_MIN 0
#define ANGULO_MAX 180
#define PASO_ANGULO 5       // Incremento en grados
#define DELAY_ESTABILIZACION 50  // Tiempo para que servo se estabilice (ms)

// Objeto servo
Servo servoRadar;

// Variables de control
int anguloActual = 90;
bool direccionDerecha = true;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configurar HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Configurar servo
  servoRadar.attach(SERVO_PIN, 500, 2400);
  
  Serial.println("===============================");
  Serial.println("TEST RADAR - SERVO + HC-SR04");
  Serial.println("===============================");
  Serial.println("Formato: angulo,distancia");
  Serial.println("Iniciando barrido...\n");
  
  delay(2000);
  
  // Centrar servo
  servoRadar.write(90);
  delay(500);
}

void loop() {
  // Mover servo al ángulo actual
  servoRadar.write(anguloActual);
  delay(DELAY_ESTABILIZACION);
  
  // Medir distancia
  float distancia = leerDistancia();
  
  // Enviar datos por Serial (formato CSV)
  Serial.print(anguloActual);
  Serial.print(",");
  Serial.print(distancia, 1);
  
  // Mostrar información adicional
  if (distancia < 20) {
    Serial.print(" [OBSTACULO CERCA!]");
  } else if (distancia > 200) {
    Serial.print(" [SIN OBSTACULO]");
  }
  
  Serial.println();
  
  // Calcular siguiente ángulo (barrido tipo ping-pong)
  if (direccionDerecha) {
    anguloActual += PASO_ANGULO;
    if (anguloActual >= ANGULO_MAX) {
      anguloActual = ANGULO_MAX;
      direccionDerecha = false;
      Serial.println(">>> Invirtiendo dirección <<<");
    }
  } else {
    anguloActual -= PASO_ANGULO;
    if (anguloActual <= ANGULO_MIN) {
      anguloActual = ANGULO_MIN;
      direccionDerecha = true;
      Serial.println(">>> Invirtiendo dirección <<<");
      Serial.println("=== BARRIDO COMPLETO ===\n");
    }
  }
}

// Función para medir distancia con HC-SR04
float leerDistancia() {
  // Limpiar trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Generar pulso de 10us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Leer tiempo del eco
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout 30ms
  
  // Calcular distancia en cm
  if (duracion == 0) {
    return 999.0; // Sin respuesta
  }
  
  float distancia = duracion * 0.034 / 2;
  return distancia;
}
