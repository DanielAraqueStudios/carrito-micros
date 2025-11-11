/*
 * TEST 1: SENSOR ULTRASONICO HC-SR04 (SIMPLE)
 * ============================================
 * Prueba básica del sensor de distancia HC-SR04
 * Solo muestra las distancias en centímetros por Serial
 * 
 * IMPORTANTE: Este test es SOLO para el sensor HC-SR04
 * No incluye servomotor - Sensor fijo apuntando al frente
 * 
 * CONEXIONES:
 * HC-SR04 VCC  -> 5V
 * HC-SR04 GND  -> GND
 * HC-SR04 TRIG -> GPIO 5
 * HC-SR04 ECHO -> GPIO 18
 * 
 * INSTRUCCIONES:
 * 1. Conectar HC-SR04 según diagrama
 * 2. Subir código al ESP32
 * 3. Abrir Serial Monitor a 115200 baud
 * 4. Colocar objetos a diferentes distancias (10-200cm)
 * 5. Verificar que las mediciones sean precisas
 * 
 * PRUEBAS RECOMENDADAS:
 * - Medir con regla a 10cm, 30cm, 50cm, 100cm
 * - Verificar error < 2cm
 * - Probar con diferentes materiales (cartón, plástico, metal)
 */

// Pines del sensor ultrasónico
#define TRIG_PIN 5
#define ECHO_PIN 18

// Contador de lecturas
int numeroLectura = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.println("==========================================");
  Serial.println("TEST HC-SR04 - MEDICION DE DISTANCIA");
  Serial.println("==========================================");
  Serial.println("Sensor fijo - Sin servomotor");
  Serial.println("Rango efectivo: 2-400 cm");
  Serial.println("Precisión esperada: ±2 cm");
  Serial.println("\nConexiones:");
  Serial.println("TRIG -> GPIO 5");
  Serial.println("ECHO -> GPIO 18");
  Serial.println("VCC  -> 5V");
  Serial.println("GND  -> GND");
  Serial.println("\nIniciando lecturas en 3 segundos...\n");
  delay(3000);
}

void loop() {
  numeroLectura++;
  
  // Leer distancia del sensor
  float distancia = leerDistancia();
  
  // Mostrar número de lectura
  Serial.print("[");
  Serial.print(numeroLectura);
  Serial.print("] ");
  
  // Mostrar distancia
  Serial.print("Distancia: ");
  
  if (distancia >= 999) {
    Serial.print("--- cm [SIN OBJETO / FUERA DE RANGO]");
  } else {
    Serial.print(distancia, 1);  // 1 decimal
    Serial.print(" cm");
    
    // Clasificar distancia
    if (distancia < 5) {
      Serial.print(" [DEMASIADO CERCA - < 5cm]");
    } else if (distancia < 15) {
      Serial.print(" [MUY CERCA! ⚠️]");
    } else if (distancia < 30) {
      Serial.print(" [CERCA]");
    } else if (distancia < 100) {
      Serial.print(" [DISTANCIA MEDIA]");
    } else if (distancia < 200) {
      Serial.print(" [LEJOS]");
    } else {
      Serial.print(" [MUY LEJOS]");
    }
  }
  
  Serial.println();
  
  // Separador cada 10 lecturas
  if (numeroLectura % 10 == 0) {
    Serial.println("------------------------------------------");
  }
  
  delay(300); // Leer cada 300ms
}

// Función para medir distancia con HC-SR04
float leerDistancia() {
  // Limpiar TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Generar pulso de 10us en TRIG
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Leer tiempo del pulso en ECHO (timeout 30ms = ~500cm)
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
  
  // Si no hay respuesta, retornar 999
  if (duracion == 0) {
    return 999.0;
  }
  
  // Calcular distancia en cm
  // Velocidad del sonido = 343 m/s = 0.0343 cm/us
  // Distancia = (tiempo * velocidad) / 2
  float distancia = (duracion * 0.0343) / 2.0;
  
  return distancia;
}
