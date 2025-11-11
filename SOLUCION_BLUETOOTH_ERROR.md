# Solución: Error de Bluetooth en ESP32
## "Bluetooth is not enabled! Please run `make menuconfig` to enable it"

---

## ❌ Error Completo:

```
error: #error Bluetooth is not enabled! Please run `make menuconfig` to enable it
exit status 1
Compilation error: #error Bluetooth is not enabled!
```

---

## ✅ SOLUCIÓN RÁPIDA (3 pasos):

### Paso 1: Cambiar Partition Scheme en Arduino IDE

1. Abrir Arduino IDE
2. Ir al menú: **Tools** → **Partition Scheme**
3. Seleccionar una de estas opciones (cualquiera funciona):
   - ✅ **"Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"** ← Recomendado
   - ✅ "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
   - ✅ "No OTA (2MB APP/2MB SPIFFS)"
   - ✅ "Huge APP (3MB No OTA/1MB SPIFFS)"

4. **NO seleccionar:**
   - ❌ "Minimal (1.3MB APP/700KB SPIFFS)" - Sin Bluetooth

### Paso 2: Verificar Board Settings

En Arduino IDE, verificar:
- **Board:** "ESP32 Dev Module" o "ESP32-WROOM-DA Module"
- **Upload Speed:** 115200
- **Flash Frequency:** 80MHz
- **Flash Mode:** QIO
- **Flash Size:** 4MB (32Mb)
- **Partition Scheme:** Default 4MB with spiffs ✅
- **Core Debug Level:** "None"
- **PSRAM:** Disabled

### Paso 3: Volver a Compilar

1. Click en **Verify** (✓) o **Upload** (→)
2. Esperar compilación exitosa
3. Subir código al ESP32

---

## 🔍 ¿Por qué ocurre este error?

El ESP32 tiene memoria Flash limitada (4MB). Debe dividirse entre:
- **APP** - Tu código
- **OTA** - Actualizaciones Over-The-Air (opcional)
- **SPIFFS** - Sistema de archivos
- **Bluetooth/WiFi** - Stack de comunicaciones

Si seleccionas un "Partition Scheme" muy pequeño (como "Minimal"), **no hay espacio para Bluetooth**.

---

## 📋 Configuración Recomendada Completa:

```
Tools Menu - Configuración Óptima:
═══════════════════════════════════
Board:              ESP32 Dev Module
Upload Speed:       115200
CPU Frequency:      240MHz (WiFi/BT)
Flash Frequency:    80MHz
Flash Mode:         QIO
Flash Size:         4MB (32Mb)
Partition Scheme:   Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)
Core Debug Level:   None
PSRAM:              Disabled
Arduino Runs On:    Core 1
Events Run On:      Core 1
```

---

## 🔧 Solución Alternativa (Si no funciona lo anterior):

### Opción A: Reinstalar ESP32 Board Manager

1. Arduino IDE → **Tools** → **Board** → **Boards Manager**
2. Buscar "ESP32"
3. Desinstalar versión actual
4. Instalar versión **2.0.14** o superior (última estable)
5. Reiniciar Arduino IDE

### Opción B: Actualizar la URL del Board Manager

1. **File** → **Preferences**
2. En "Additional Board Manager URLs" agregar:
```
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```
3. **Tools** → **Board** → **Boards Manager**
4. Actualizar "esp32 by Espressif Systems"

---

## 🧪 Código de Prueba Mínimo (Verificar Bluetooth)

Prueba este código simple para verificar que Bluetooth funciona:

```cpp
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Test"); // Nombre del dispositivo
  Serial.println("Bluetooth iniciado - Busca 'ESP32_Test'");
}

void loop() {
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
}
```

Si este código compila y sube correctamente, ¡Bluetooth está funcionando! ✅

---

## 📊 Comparación de Partition Schemes:

| Partition Scheme | APP | SPIFFS | OTA | Bluetooth | WiFi |
|------------------|-----|--------|-----|-----------|------|
| **Default 4MB** ✅ | 1.2MB | 1.5MB | ❌ | ✅ | ✅ |
| Minimal SPIFFS | 1.9MB | 190KB | ✅ | ✅ | ✅ |
| No OTA | 2MB | 2MB | ❌ | ✅ | ✅ |
| Huge APP | 3MB | 1MB | ❌ | ✅ | ✅ |
| **Minimal** ❌ | 1.3MB | 700KB | ❌ | ❌ | ⚠️ |

---

## ⚠️ Errores Comunes Relacionados:

### Error: "A fatal error occurred: Failed to connect"
**Solución:** Mantener presionado el botón **BOOT** mientras sube el código.

### Error: "Sketch too big"
**Solución:** Cambiar a "Huge APP" partition scheme.

### Warning: "BT controller not initialized"
**Solución:** Ya resuelto con partition scheme correcto.

---

## ✅ Checklist de Verificación:

- [ ] Partition Scheme = "Default 4MB with spiffs" o similar
- [ ] ESP32 Board instalado (versión 2.0.x o superior)
- [ ] Puerto COM correcto seleccionado
- [ ] Cable USB de datos (no solo carga)
- [ ] ESP32 conectado correctamente
- [ ] Código del test_bluetooth.ino abierto
- [ ] Compilar → Sin errores
- [ ] Subir → Exitoso
- [ ] Serial Monitor abierto a 115200 baud
- [ ] Ver mensaje "Bluetooth INICIADO"

---

## 🎯 Resultado Esperado:

Después de aplicar la solución, al compilar deberías ver:

```
Sketch uses XXXXX bytes (XX%) of program storage space.
Global variables use XXXXX bytes (XX%) of dynamic memory.
```

Y al subir y abrir Serial Monitor:

```
================================
TEST BLUETOOTH CLASSIC ESP32
================================

Bluetooth INICIADO
Nombre: ESP32_Robot

Esperando conexión...
```

---

## 📱 Probar la Conexión:

1. **En tu móvil:**
   - Activar Bluetooth
   - Buscar dispositivos
   - Conectar a "ESP32_Robot"

2. **Descargar app:**
   - Android: "Serial Bluetooth Terminal" (Play Store)
   - iOS: "Bluetooth Terminal" (App Store)

3. **Conectar en la app:**
   - Seleccionar "ESP32_Robot"
   - Enviar comandos: F, B, L, R, S

4. **Ver en Serial Monitor:**
   - Mensajes recibidos
   - Respuestas enviadas

---

## 🆘 Si NADA Funciona:

1. **Verificar tu placa ESP32:**
   - Algunos clones baratos NO tienen Bluetooth
   - Buscar en la placa: "ESP32-WROOM-32"
   - Si dice solo "ESP8266" → NO tiene Bluetooth (es otra placa)

2. **Probar con código de ejemplo oficial:**
   - File → Examples → BluetoothSerial → SerialToSerialBT
   - Si este tampoco compila, problema de hardware o instalación

3. **Última opción - PlatformIO:**
   - Usar PlatformIO en lugar de Arduino IDE
   - Configuración automática de particiones

---

**Última actualización:** 11 de Noviembre de 2025  
**Proyecto:** Robot Móvil Autónomo - UMNG
