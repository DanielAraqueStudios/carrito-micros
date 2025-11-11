/*
 * TEST 4: BLUETOOTH CLASSIC (Serial)
 * ===================================
 * Prueba de comunicación Bluetooth del ESP32
 * Envía y recibe datos por Bluetooth
 * 
 * CARACTERISTICAS:
 * - Bluetooth Classic (no BLE)
 * - Nombre del dispositivo: "ESP32_Robot"
 * - Envía datos cada 1 segundo
 * - Recibe comandos desde app móvil
 * 
 * INSTRUCCIONES:
 * 1. Subir código al ESP32
 * 2. Abrir Serial Monitor a 115200 baud
 * 3. Emparejar Bluetooth desde PC/Móvil:
 *    - Buscar dispositivo "ESP32_Robot"
 *    - PIN: 1234 (si se solicita)
 * 4. Usar app "Serial Bluetooth Terminal" (Android/iOS)
 * 5. Conectar al dispositivo
 * 6. Enviar comandos: F (adelante), B (atrás), L (izq), R (der), S (stop)
 * 
 * APPS RECOMENDADAS:
 * - Android: Serial Bluetooth Terminal
 * - iOS: BLE Terminal
 * - PC: Bluetooth Terminal (Windows Store)
 */

#include "BluetoothSerial.h"

// Verificar si Bluetooth está disponible
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

// Variables de control
int contadorMensajes = 0;
unsigned long tiempoAnterior = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("================================");
  Serial.println("TEST BLUETOOTH CLASSIC ESP32");
  Serial.println("================================");
  
  // Iniciar Bluetooth con nombre del dispositivo
  SerialBT.begin("ESP32_Robot");
  
  Serial.println("\nBluetooth INICIADO");
  Serial.println("Nombre: ESP32_Robot");
  Serial.println("\nEsperando conexión...");
  Serial.println("Pasos:");
  Serial.println("1. Abrir Bluetooth en tu dispositivo");
  Serial.println("2. Buscar 'ESP32_Robot'");
  Serial.println("3. Emparejar (PIN: 1234 si solicita)");
  Serial.println("4. Conectar desde app Serial Bluetooth Terminal");
  Serial.println("\nComandos disponibles:");
  Serial.println("F = Adelante");
  Serial.println("B = Atrás");
  Serial.println("L = Izquierda");
  Serial.println("R = Derecha");
  Serial.println("S = Stop");
  Serial.println("V = Velocidad");
  Serial.println("================================\n");
}

void loop() {
  // Enviar datos cada 1 segundo
  if (millis() - tiempoAnterior >= 1000) {
    tiempoAnterior = millis();
    contadorMensajes++;
    
    // Enviar por Bluetooth
    SerialBT.print("Mensaje #");
    SerialBT.print(contadorMensajes);
    SerialBT.print(" | Tiempo: ");
    SerialBT.print(millis() / 1000);
    SerialBT.println("s");
    
    // Mostrar en Serial Monitor
    Serial.print("[BT ENVIADO] Mensaje #");
    Serial.print(contadorMensajes);
    Serial.print(" | Tiempo: ");
    Serial.print(millis() / 1000);
    Serial.println("s");
    
    // Enviar datos simulados de sensores
    SerialBT.print("Distancia: ");
    SerialBT.print(random(10, 100));
    SerialBT.print(" cm | Batería: ");
    SerialBT.print(random(70, 100));
    SerialBT.println("%");
  }
  
  // Verificar si hay datos recibidos por Bluetooth
  if (SerialBT.available()) {
    char comando = SerialBT.read();
    
    // Mostrar comando recibido
    Serial.print("\n[BT RECIBIDO] Comando: ");
    Serial.print(comando);
    Serial.print(" - ");
    
    // Procesar comando
    switch (comando) {
      case 'F':
      case 'f':
        Serial.println("ADELANTE");
        SerialBT.println("OK: Adelante");
        break;
        
      case 'B':
      case 'b':
        Serial.println("ATRAS");
        SerialBT.println("OK: Atrás");
        break;
        
      case 'L':
      case 'l':
        Serial.println("IZQUIERDA");
        SerialBT.println("OK: Izquierda");
        break;
        
      case 'R':
      case 'r':
        Serial.println("DERECHA");
        SerialBT.println("OK: Derecha");
        break;
        
      case 'S':
      case 's':
        Serial.println("STOP");
        SerialBT.println("OK: Detenido");
        break;
        
      case 'V':
      case 'v':
        Serial.println("SOLICITUD DE VELOCIDAD");
        SerialBT.println("Velocidad: 180/255");
        break;
        
      default:
        Serial.print("DESCONOCIDO (");
        Serial.print(comando);
        Serial.println(")");
        SerialBT.println("ERROR: Comando no reconocido");
        SerialBT.println("Usa: F, B, L, R, S, V");
        break;
    }
  }
  
  // Verificar estado de conexión
  static bool estadoAnterior = false;
  bool estadoActual = SerialBT.hasClient();
  
  if (estadoActual != estadoAnterior) {
    estadoAnterior = estadoActual;
    if (estadoActual) {
      Serial.println("\n*** DISPOSITIVO CONECTADO ***\n");
      SerialBT.println("=========================");
      SerialBT.println("ESP32 Robot Conectado!");
      SerialBT.println("=========================");
      SerialBT.println("Comandos: F B L R S V");
    } else {
      Serial.println("\n*** DISPOSITIVO DESCONECTADO ***\n");
    }
  }
  
  delay(10); // Pequeña pausa para estabilidad
}
