/*
 * TEST 7: RADAR AVANZADO CON DETECCION DE OBSTACULOS
 * ===================================================
 * Sistema completo de radar con análisis de entorno
 * Detecta obstáculos y determina mejor dirección para navegar
 * 
 * CARACTERISTICAS:
 * - Barrido completo de 180° (0° a 180°)
 * - Detección de obstáculos en tiempo real
 * - Mapa de distancias por sectores
 * - Recomendación de dirección libre
 * 
 * CONEXIONES:
 * HC-SR04:
 *   VCC  -> 5V
 *   GND  -> GND
 *   TRIG -> GPIO 5
 *   ECHO -> GPIO 18
 * 
 * Servomotor:
 *   Señal -> GPIO 13
 *   VCC   -> 5V
 *   GND   -> GND común
 * 
 * INSTRUCCIONES:
 * 1. Montar HC-SR04 sobre servo
 * 2. Conectar todo según diagrama
 * 3. Subir código
 * 4. Abrir Serial Monitor a 115200 baud
 * 5. Colocar obstáculos en diferentes posiciones
 */

#include <ESP32Servo.h>

// Pines
#define SERVO_PIN 13
#define TRIG_PIN 5
#define ECHO_PIN 18

// Configuración
#define DISTANCIA_SEGURA 30  // cm - Distancia mínima de seguridad

// Objeto servo
Servo servoRadar;

// Array para almacenar distancias
const int numAngulos = 19;  // 0°, 10°, 20°, ..., 180°
int angulos[numAngulos];
float distancias[numAngulos];

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configurar HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Configurar servo
  servoRadar.attach(SERVO_PIN, 500, 2400);
  
  Serial.println("========================================");
  Serial.println("RADAR AVANZADO - DETECCION DE ENTORNO");
  Serial.println("========================================");
  Serial.println("Iniciando sistema...\n");
  
  delay(1000);
  
  // Inicializar array de ángulos
  for (int i = 0; i < numAngulos; i++) {
    angulos[i] = i * 10;  // 0, 10, 20, ..., 180
  }
  
  // Centrar servo
  servoRadar.write(90);
  delay(1000);
}

void loop() {
  // Realizar barrido completo
  Serial.println("\n>>> INICIANDO BARRIDO COMPLETO <<<\n");
  realizarBarrido();
  
  // Mostrar mapa de distancias
  mostrarMapaDistancias();
  
  // Analizar entorno
  analizarEntorno();
  
  // Esperar antes del siguiente barrido
  Serial.println("\nEsperando 3 segundos para próximo barrido...");
  delay(3000);
}

// Realizar barrido de 0° a 180°
void realizarBarrido() {
  for (int i = 0; i < numAngulos; i++) {
    int angulo = angulos[i];
    
    // Mover servo
    servoRadar.write(angulo);
    delay(100);  // Esperar estabilización
    
    // Tomar 3 mediciones y promediar
    float suma = 0;
    int medicionesValidas = 0;
    
    for (int j = 0; j < 3; j++) {
      float dist = leerDistancia();
      if (dist < 400) {  // Medición válida
        suma += dist;
        medicionesValidas++;
      }
      delay(10);
    }
    
    // Guardar promedio
    if (medicionesValidas > 0) {
      distancias[i] = suma / medicionesValidas;
    } else {
      distancias[i] = 999;  // Sin obstáculo
    }
    
    // Mostrar en Serial
    Serial.print(angulo);
    Serial.print("°: ");
    Serial.print(distancias[i], 1);
    Serial.print(" cm");
    
    if (distancias[i] < DISTANCIA_SEGURA) {
      Serial.print(" [OBSTACULO!]");
    }
    Serial.println();
  }
}

// Mostrar mapa visual de distancias
void mostrarMapaDistancias() {
  Serial.println("\n=== MAPA DE DISTANCIAS ===");
  Serial.println("Derecha (0°)    Centro (90°)    Izquierda (180°)");
  
  for (int i = 0; i < numAngulos; i++) {
    Serial.print(angulos[i]);
    Serial.print("°: ");
    
    // Crear barra visual
    int barras = (int)(distancias[i] / 5);  // 1 barra = 5cm
    if (barras > 30) barras = 30;  // Máximo 30 barras
    
    for (int j = 0; j < barras; j++) {
      Serial.print("█");
    }
    
    Serial.print(" ");
    Serial.print(distancias[i], 1);
    Serial.println(" cm");
  }
}

// Analizar entorno y recomendar dirección
void analizarEntorno() {
  Serial.println("\n=== ANALISIS DEL ENTORNO ===");
  
  // Dividir en 3 sectores
  float distDerecha = calcularDistanciaPromedio(0, 6);    // 0° - 60°
  float distCentro = calcularDistanciaPromedio(6, 12);    // 60° - 120°
  float distIzquierda = calcularDistanciaPromedio(12, 18); // 120° - 180°
  
  Serial.print("Sector DERECHA (0°-60°): ");
  Serial.print(distDerecha, 1);
  Serial.println(" cm");
  
  Serial.print("Sector CENTRO (60°-120°): ");
  Serial.print(distCentro, 1);
  Serial.println(" cm");
  
  Serial.print("Sector IZQUIERDA (120°-180°): ");
  Serial.print(distIzquierda, 1);
  Serial.println(" cm");
  
  // Determinar mejor dirección
  Serial.println("\n>>> RECOMENDACION <<<");
  
  if (distCentro > DISTANCIA_SEGURA) {
    Serial.println("✓ AVANZAR AL FRENTE - Camino despejado");
  } else if (distDerecha > distIzquierda && distDerecha > DISTANCIA_SEGURA) {
    Serial.println("→ GIRAR A LA DERECHA - Mejor espacio");
  } else if (distIzquierda > distDerecha && distIzquierda > DISTANCIA_SEGURA) {
    Serial.println("← GIRAR A LA IZQUIERDA - Mejor espacio");
  } else {
    Serial.println("⚠ RETROCEDER - Obstáculos en todas direcciones");
  }
  
  // Buscar ángulo con mayor distancia
  int anguloMaxDist = 0;
  float maxDistancia = 0;
  
  for (int i = 0; i < numAngulos; i++) {
    if (distancias[i] > maxDistancia && distancias[i] < 300) {
      maxDistancia = distancias[i];
      anguloMaxDist = angulos[i];
    }
  }
  
  Serial.print("\nMejor ángulo: ");
  Serial.print(anguloMaxDist);
  Serial.print("° con ");
  Serial.print(maxDistancia, 1);
  Serial.println(" cm");
}

// Calcular distancia promedio en un rango de índices
float calcularDistanciaPromedio(int inicio, int fin) {
  float suma = 0;
  int count = 0;
  
  for (int i = inicio; i <= fin && i < numAngulos; i++) {
    if (distancias[i] < 300) {  // Solo distancias válidas
      suma += distancias[i];
      count++;
    }
  }
  
  if (count > 0) {
    return suma / count;
  }
  return 999;  // Sin obstáculos
}

// Función para medir distancia con HC-SR04
float leerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duracion == 0) {
    return 999.0;
  }
  
  float distancia = duracion * 0.034 / 2;
  return distancia;
}
