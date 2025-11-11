/*
 * TEST 3: SENSOR MPU6050 (IMU 6 EJES)
 * ====================================
 * Prueba del acelerómetro y giroscopio
 * Muestra aceleración, ángulos y temperatura
 * 
 * CONEXIONES:
 * MPU6050 VCC -> 3.3V (o 5V)
 * MPU6050 GND -> GND
 * MPU6050 SDA -> GPIO 21 (I2C Data)
 * MPU6050 SCL -> GPIO 22 (I2C Clock)
 * MPU6050 AD0 -> GND (dirección I2C 0x68)
 * 
 * BIBLIOTECA REQUERIDA:
 * Adafruit MPU6050 (instalar desde Library Manager)
 * Adafruit Unified Sensor (dependencia)
 * 
 * INSTRUCCIONES:
 * 1. Instalar biblioteca: Sketch > Include Library > Manage Libraries
 *    Buscar "Adafruit MPU6050" e instalar
 * 2. Subir código al ESP32
 * 3. Abrir Serial Monitor a 115200 baud
 * 4. Colocar sensor en superficie plana
 * 5. Inclinar y mover para ver cambios
 */

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=============================");
  Serial.println("TEST SENSOR MPU6050 (IMU)");
  Serial.println("=============================");
  
  // Inicializar I2C
  Wire.begin(21, 22); // SDA, SCL
  
  // Inicializar MPU6050
  Serial.print("Buscando MPU6050... ");
  if (!mpu.begin()) {
    Serial.println("FALLO!");
    Serial.println("Verificar conexiones:");
    Serial.println("- SDA -> GPIO 21");
    Serial.println("- SCL -> GPIO 22");
    Serial.println("- VCC -> 3.3V");
    Serial.println("- GND -> GND");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("OK!");
  
  // Configurar rangos del sensor
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  Serial.println("\nConfiguración:");
  Serial.println("- Acelerómetro: ±8G");
  Serial.println("- Giroscopio: ±500°/s");
  Serial.println("- Filtro: 21 Hz");
  Serial.println("\nIniciando lecturas...\n");
  delay(2000);
}

void loop() {
  // Obtener datos del sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Calcular ángulos de inclinación (pitch y roll)
  float pitch = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  float roll = atan2(-a.acceleration.x, a.acceleration.z) * 180 / PI;
  
  // Mostrar aceleración (m/s²)
  Serial.println("=== ACELERACION (m/s²) ===");
  Serial.print("X: ");
  Serial.print(a.acceleration.x, 2);
  Serial.print(" | Y: ");
  Serial.print(a.acceleration.y, 2);
  Serial.print(" | Z: ");
  Serial.println(a.acceleration.z, 2);
  
  // Mostrar velocidad angular (rad/s)
  Serial.println("\n=== GIROSCOPIO (rad/s) ===");
  Serial.print("X: ");
  Serial.print(g.gyro.x, 2);
  Serial.print(" | Y: ");
  Serial.print(g.gyro.y, 2);
  Serial.print(" | Z: ");
  Serial.println(g.gyro.z, 2);
  
  // Mostrar ángulos calculados
  Serial.println("\n=== ANGULOS (grados) ===");
  Serial.print("Pitch (inclinación): ");
  Serial.print(pitch, 1);
  Serial.println("°");
  Serial.print("Roll (balanceo): ");
  Serial.print(roll, 1);
  Serial.println("°");
  
  // Mostrar temperatura
  Serial.println("\n=== TEMPERATURA ===");
  Serial.print(temp.temperature, 1);
  Serial.println(" °C");
  
  // Detectar movimiento significativo
  float aceleracionTotal = sqrt(
    a.acceleration.x * a.acceleration.x +
    a.acceleration.y * a.acceleration.y +
    a.acceleration.z * a.acceleration.z
  );
  
  Serial.println("\n=== ESTADO ===");
  if (aceleracionTotal > 12.0) {
    Serial.println("[MOVIMIENTO DETECTADO!]");
  } else if (abs(pitch) > 10 || abs(roll) > 10) {
    Serial.println("[INCLINADO]");
  } else {
    Serial.println("[REPOSO - HORIZONTAL]");
  }
  
  Serial.println("\n=====================================\n");
  
  delay(500); // Leer cada 500ms
}
