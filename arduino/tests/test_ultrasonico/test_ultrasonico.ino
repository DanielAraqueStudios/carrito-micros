/*
 * TEST 1: SENSOR ULTRASONICO HC-SR04
 * ====================================
 * Prueba simple del sensor de distancia
 * Muestra las mediciones en el Serial Monitor
 * 
 * CONEXIONES:
 * HC-SR04 VCC  -> 5V
 * HC-SR04 GND  -> GND
 * HC-SR04 TRIG -> GPIO 5
 * HC-SR04 ECHO -> GPIO 18
 * 
 * INSTRUCCIONES:
 * 1. Subir el código al ESP32
 * 2. Abrir Serial Monitor a 115200 baud
 * 3. Colocar objetos a diferentes distancias
 * 4. Verificar lecturas en centímetros
 */

// Pines del sensor ultrasónico
#define TRIG_PIN 5
#define ECHO_PIN 18

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.println("=================================");
  Serial.println("TEST SENSOR ULTRASONICO HC-SR04");
  Serial.println("=================================");
  Serial.println("Iniciando en 3 segundos...\n");
  delay(3000);
}

void loop() {
  // Leer distancia del sensor
  float distancia = leerDistancia();
  
  // Mostrar resultado en Serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm");
  
  // Indicar estado
  if (distancia < 20) {
    Serial.println(" [OBSTACULO CERCA!]");
  } else if (distancia > 200) {
    Serial.println(" [FUERA DE RANGO]");
  } else {
    Serial.println(" [OK]");
  }
  
  delay(200); // Leer cada 200ms
}

// Función para medir distancia con HC-SR04
float leerDistancia() {
  // Generar pulso de 10us en TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Leer tiempo del pulso en ECHO
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout 30ms
  
  // Calcular distancia en cm (velocidad sonido = 343 m/s)
  float distancia = duracion * 0.034 / 2;
  
  // Si no hay respuesta, retornar 999
  if (duracion == 0) {
    return 999.0;
  }
  
  return distancia;
}
