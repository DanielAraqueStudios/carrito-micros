/*
 * TEST 4: BLUETOOTH BLE (ESP32-S3)
 * =================================
 * Prueba de comunicación Bluetooth del ESP32-S3
 * NOTA: ESP32-S3 usa Bluetooth 5.0 BLE (no Classic)
 * 
 * IMPORTANTE - DIFERENCIAS ESP32-S3:
 * - ESP32-S3 solo soporta BLE (Bluetooth Low Energy)
 * - NO soporta Bluetooth Classic
 * - Usar bibliotecas BLE en lugar de BluetoothSerial
 * 
 * CARACTERISTICAS:
 * - Bluetooth 5.0 BLE
 * - Nombre del dispositivo: "ESP32_Robot"
 * - Envía datos cada 1 segundo
 * - Recibe comandos desde app móvil
 * 
 * CONFIGURACIÓN EN ARDUINO IDE PARA ESP32-S3:
 * 1. Board: "ESP32S3 Dev Module"
 * 2. USB CDC On Boot: "Enabled" (para Serial Monitor)
 * 3. USB DFU On Boot: "Disabled"
 * 4. Flash Size: "8MB" o "4MB"
 * 5. Partition Scheme: "Default 4MB with spiffs" o "8MB with spiffs"
 * 6. PSRAM: "Enabled" (si tu placa tiene PSRAM)
 * 
 * INSTRUCCIONES:
 * 1. Configurar Arduino IDE como arriba
 * 2. Subir código al ESP32-S3
 * 3. Abrir Serial Monitor a 115200 baud
 * 4. Usar app BLE (NO Bluetooth Classic):
 *    - Android: "nRF Connect" o "Serial Bluetooth Terminal" (modo BLE)
 *    - iOS: "nRF Connect" o "LightBlue"
 * 5. Buscar "ESP32_Robot" en dispositivos BLE
 * 6. Conectar y enviar comandos
 * 
 * APPS RECOMENDADAS PARA BLE:
 * - Android/iOS: nRF Connect (Nordic Semiconductor)
 * - Android: Serial Bluetooth Terminal (activar modo BLE)
 * - iOS: LightBlue Explorer
 */

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUIDs para el servicio BLE (estándar UART)
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Variables de control
int contadorMensajes = 0;
unsigned long tiempoAnterior = 0;
String mensajeRecibido = "";

// Callback para eventos de conexión
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("\n*** DISPOSITIVO BLE CONECTADO ***\n");
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("\n*** DISPOSITIVO BLE DESCONECTADO ***\n");
  }
};

// Callback para recibir datos
class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.print("\n[BLE RECIBIDO] ");
      
      for (int i = 0; i < rxValue.length(); i++) {
        char comando = rxValue[i];
        Serial.print(comando);
        
        // Procesar comando
        procesarComando(comando);
      }
      Serial.println();
    }
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("================================");
  Serial.println("TEST BLUETOOTH BLE ESP32-S3");
  Serial.println("================================");
  Serial.println("Placa: ESP32-S3 DevKit");
  Serial.println("Bluetooth: 5.0 BLE (Low Energy)");
  Serial.println();
  
  // Crear dispositivo BLE
  BLEDevice::init("ESP32_Robot");
  
  // Crear servidor BLE
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  // Crear servicio BLE
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Crear característica TX (transmitir a app)
  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pTxCharacteristic->addDescriptor(new BLE2902());
  
  // Crear característica RX (recibir de app)
  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
  );
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  
  // Iniciar servicio
  pService->start();
  
  // Iniciar advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("BLE INICIADO");
  Serial.println("Nombre: ESP32_Robot");
  Serial.println("\nEsperando conexión BLE...");
  Serial.println("Pasos:");
  Serial.println("1. Abrir app BLE en tu dispositivo");
  Serial.println("2. Escanear dispositivos BLE");
  Serial.println("3. Conectar a 'ESP32_Robot'");
  Serial.println("4. Buscar servicio UART");
  Serial.println("5. Enviar comandos");
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
  // Enviar datos cada 1 segundo si hay conexión
  if (deviceConnected && (millis() - tiempoAnterior >= 1000)) {
    tiempoAnterior = millis();
    contadorMensajes++;
    
    // Crear mensaje
    String mensaje = "Msg #" + String(contadorMensajes) + 
                    " | T: " + String(millis() / 1000) + "s\n";
    
    // Enviar por BLE
    pTxCharacteristic->setValue(mensaje.c_str());
    pTxCharacteristic->notify();
    
    // Mostrar en Serial Monitor
    Serial.print("[BLE ENVIADO] ");
    Serial.print(mensaje);
    
    // Enviar datos simulados de sensores
    String datos = "Dist: " + String(random(10, 100)) + 
                  "cm | Bat: " + String(random(70, 100)) + "%\n";
    pTxCharacteristic->setValue(datos.c_str());
    pTxCharacteristic->notify();
  }
  
  // Gestionar desconexión
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising();
    Serial.println("Reiniciando advertising BLE...");
    oldDeviceConnected = deviceConnected;
  }
  
  // Gestionar nueva conexión
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
  
  delay(10);
}

void procesarComando(char comando) {
  Serial.print(" - ");
  
  switch (comando) {
    case 'F':
    case 'f':
      Serial.println("ADELANTE");
      enviarRespuesta("OK: Adelante\n");
      break;
      
    case 'B':
    case 'b':
      Serial.println("ATRAS");
      enviarRespuesta("OK: Atrás\n");
      break;
      
    case 'L':
    case 'l':
      Serial.println("IZQUIERDA");
      enviarRespuesta("OK: Izquierda\n");
      break;
      
    case 'R':
    case 'r':
      Serial.println("DERECHA");
      enviarRespuesta("OK: Derecha\n");
      break;
      
    case 'S':
    case 's':
      Serial.println("STOP");
      enviarRespuesta("OK: Detenido\n");
      break;
      
    case 'V':
    case 'v':
      Serial.println("SOLICITUD DE VELOCIDAD");
      enviarRespuesta("Velocidad: 180/255\n");
      break;
      
    default:
      Serial.print("DESCONOCIDO (");
      Serial.print(comando);
      Serial.println(")");
      enviarRespuesta("ERROR: Comando no reconocido\nUsa: F B L R S V\n");
      break;
  }
}

void enviarRespuesta(String mensaje) {
  if (deviceConnected) {
    pTxCharacteristic->setValue(mensaje.c_str());
    pTxCharacteristic->notify();
  }
}
