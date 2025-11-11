# Configuración ESP32-S3 para el Proyecto
## Robot Móvil Autónomo - UMNG

---

## 🆕 DIFERENCIAS IMPORTANTES ESP32-S3

### ESP32 Original vs ESP32-S3:

| Característica | ESP32 (Original) | ESP32-S3 |
|----------------|------------------|----------|
| **CPU** | Dual-core Xtensa LX6 | Dual-core Xtensa LX7 |
| **Frecuencia** | 240 MHz | 240 MHz |
| **RAM** | 520 KB | 512 KB |
| **Flash** | 4 MB | 4-16 MB |
| **PSRAM** | Opcional | Hasta 8 MB |
| **WiFi** | 802.11 b/g/n | 802.11 b/g/n |
| **Bluetooth** | Classic + BLE 4.2 | **SOLO BLE 5.0** ⚠️ |
| **USB** | Externo (CH340) | **USB Nativo** ✅ |
| **GPIO** | 34 pines | 45 pines |

### ⚠️ CAMBIO CRÍTICO: BLUETOOTH

**ESP32 Original:** Bluetooth Classic + BLE  
**ESP32-S3:** **SOLO Bluetooth 5.0 BLE** (Low Energy)

**Esto significa:**
- ❌ NO puedes usar `BluetoothSerial.h`
- ✅ DEBES usar bibliotecas BLE (`BLEDevice.h`)
- 📱 Apps diferentes (nRF Connect en lugar de Serial Bluetooth Terminal)

---

## 🔧 CONFIGURACIÓN EN ARDUINO IDE

### Paso 1: Instalar Soporte ESP32-S3

1. **File** → **Preferences**
2. En "Additional Board Manager URLs":
```
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```
3. **Tools** → **Board** → **Boards Manager**
4. Buscar "esp32"
5. Instalar **"esp32 by Espressif Systems"** versión **2.0.14 o superior**

### Paso 2: Seleccionar Board ESP32-S3

**Tools Menu - Configuración para ESP32-S3:**

```
Board:                ESP32S3 Dev Module
Upload Speed:         115200
USB Mode:             Hardware CDC and JTAG
USB CDC On Boot:      Enabled  ← IMPORTANTE
USB Firmware MSC On Boot: Disabled
USB DFU On Boot:      Disabled
Upload Mode:          UART0 / Hardware CDC
CPU Frequency:        240MHz (WiFi)
Flash Mode:           QIO 80MHz
Flash Size:           8MB  (o 4MB según tu placa)
Partition Scheme:     8M with spiffs (3MB APP/1.5MB SPIFFS)
Core Debug Level:     None
PSRAM:                OPI PSRAM  (si tu placa tiene PSRAM)
Arduino Runs On:      Core 1
Events Run On:        Core 1
```

### ⚠️ CONFIGURACIÓN CRÍTICA:

**USB CDC On Boot: Enabled** → Necesario para Serial Monitor  
**Flash Size:** Verifica tu placa (4MB, 8MB o 16MB)  
**PSRAM:** Habilitar si tu ESP32-S3 tiene PSRAM (la mayoría lo tienen)

---

## 📌 PINES GPIO ESP32-S3

### Pines Seguros para Usar:

Los pines **3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 46** son seguros.

### Pines a EVITAR:

| Pin | Razón |
|-----|-------|
| GPIO 0 | Boot mode / Strapping pin |
| GPIO 19, 20 | USB D- y D+ (si usas USB nativo) |
| GPIO 26-32 | SPI Flash / PSRAM (no usar) |
| GPIO 33-37 | SPI Flash / PSRAM (no usar) |
| GPIO 43, 44 | UART0 TX/RX |
| GPIO 45 | Strapping pin |

### ✅ Nuestra Configuración (Validada para ESP32-S3):

```
Motores:  GPIO 4, 6, 7, 15, 16, 17
HC-SR04:  GPIO 5, 18
Servo:    GPIO 13
MPU6050:  GPIO 8 (SDA), GPIO 3 (SCL)
```

**Todos estos pines son seguros en ESP32-S3** ✅

---

## 📡 BLUETOOTH EN ESP32-S3

### ⚠️ Problema Principal:

ESP32-S3 **NO soporta Bluetooth Classic**, solo **BLE 5.0**.

### Solución: Usar BLE en lugar de Bluetooth Classic

**Antes (ESP32 original):**
```cpp
#include "BluetoothSerial.h"  // ❌ NO funciona en ESP32-S3
BluetoothSerial SerialBT;
```

**Ahora (ESP32-S3):**
```cpp
#include <BLEDevice.h>         // ✅ Funciona en ESP32-S3
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
```

### 📱 Apps Compatibles con BLE:

| Plataforma | App Recomendada | Gratis |
|------------|-----------------|--------|
| Android | **nRF Connect** | ✅ |
| Android | Serial Bluetooth Terminal (modo BLE) | ✅ |
| iOS | **nRF Connect** | ✅ |
| iOS | LightBlue Explorer | ✅ |
| PC/Mac | nRF Connect Desktop | ✅ |

---

## 🚀 CÓMO SUBIR CÓDIGO AL ESP32-S3

### Método 1: Via USB (Recomendado)

1. Conectar ESP32-S3 por USB-C
2. Seleccionar puerto COM en Tools
3. Click "Upload" (→)
4. **Si falla:** Mantener presionado **BOOT** mientras sube

### Método 2: Forzar modo Boot

1. Mantener presionado **BOOT**
2. Presionar y soltar **RESET**
3. Soltar **BOOT**
4. Click "Upload"

---

## 🧪 CÓDIGO DE PRUEBA ESP32-S3

### Test Básico (Verificar ESP32-S3 funciona):

```cpp
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 Funcionando!");
  Serial.print("Chip: ");
  Serial.println(ESP.getChipModel());
  Serial.print("Cores: ");
  Serial.println(ESP.getChipCores());
  Serial.print("Frecuencia: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
}

void loop() {
  Serial.println("Test OK");
  delay(1000);
}
```

**Salida esperada:**
```
ESP32-S3 Funcionando!
Chip: ESP32-S3
Cores: 2
Frecuencia: 240 MHz
Test OK
```

---

## 📊 MEMORIA Y RENDIMIENTO

### ESP32-S3 con 8MB Flash:

```
Partition Scheme: 8M with spiffs (3MB APP/1.5MB SPIFFS)

┌─────────────────────────────────┐
│  APP (3 MB)                     │  ← Tu código
├─────────────────────────────────┤
│  SPIFFS (1.5 MB)                │  ← Archivos
├─────────────────────────────────┤
│  OTA (3 MB)                     │  ← Actualizaciones
├─────────────────────────────────┤
│  NVS, WiFi, BLE, etc (512 KB)  │  ← Sistema
└─────────────────────────────────┘
```

**Ventaja:** Mucho más espacio que ESP32 original (4MB)

---

## ⚡ CONSUMO DE CORRIENTE

| Modo | ESP32 Original | ESP32-S3 |
|------|----------------|----------|
| Activo (WiFi) | ~160 mA | ~130 mA ✅ |
| Activo (BLE) | ~110 mA | ~85 mA ✅ |
| Deep Sleep | ~10 µA | ~7 µA ✅ |

**ESP32-S3 es más eficiente energéticamente** ✅

---

## 🔌 ALIMENTACIÓN ESP32-S3

### Opciones:

1. **USB-C** (5V) → Regulador interno a 3.3V
2. **5V Pin** → Directo al regulador interno
3. **3.3V Pin** → Directo (sin regulador)

### Recomendación para el Proyecto:

```
Batería 7.4V LiPo (2S)
    ↓
Regulador Buck 5V/3A
    ├─→ ESP32-S3 (5V pin)
    ├─→ Servo (5V)
    ├─→ HC-SR04 (5V)
    └─→ MPU6050 (via ESP32 3.3V)
```

---

## 📝 CHECKLIST PARA ESP32-S3

- [ ] Board: ESP32S3 Dev Module
- [ ] USB CDC On Boot: Enabled ⚠️
- [ ] Flash Size: 8MB (verificar tu placa)
- [ ] Partition Scheme: 8M with spiffs
- [ ] PSRAM: OPI PSRAM (si está disponible)
- [ ] Puerto COM correcto
- [ ] Código usa BLE (no Bluetooth Classic)
- [ ] Apps BLE instaladas (nRF Connect)

---

## 🆚 RESUMEN: ¿Qué Cambia en el Proyecto?

| Componente | ESP32 | ESP32-S3 | Cambios |
|------------|-------|----------|---------|
| **Motores** | ✅ | ✅ | Ninguno |
| **HC-SR04** | ✅ | ✅ | Ninguno |
| **Servo** | ✅ | ✅ | Ninguno |
| **MPU6050** | ✅ | ✅ | Ninguno |
| **Bluetooth** | Classic | **BLE** | ⚠️ Cambiar código |
| **WiFi** | ✅ | ✅ | Ninguno |
| **USB** | Externo | **Nativo** | ✅ Mejor |
| **Pines GPIO** | ✅ | ✅ | Validados |

**Conclusión:** Solo el Bluetooth requiere cambios en el código ✅

---

## 🎯 VENTAJAS DE USAR ESP32-S3

✅ **Más rápido** (CPU mejorada)  
✅ **Más memoria** (8MB Flash vs 4MB)  
✅ **USB nativo** (no necesita CH340)  
✅ **Mejor BLE** (5.0 vs 4.2)  
✅ **Menor consumo** energético  
✅ **Más GPIOs** disponibles  
✅ **PSRAM integrado** (la mayoría de placas)  

---

## 📚 RECURSOS ADICIONALES

- **Datasheet ESP32-S3:** https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- **Arduino ESP32-S3 Guide:** https://docs.espressif.com/projects/arduino-esp32/en/latest/
- **BLE Examples:** File → Examples → ESP32 BLE Arduino

---

**Última actualización:** 11 de Noviembre de 2025  
**Proyecto:** Robot Móvil Autónomo con ESP32-S3 - UMNG
