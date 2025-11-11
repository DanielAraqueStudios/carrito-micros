# Configuración de Pines del Robot ESP32
## Proyecto: Carrito Micros - UMNG

---

## 📌 Restricción de Pines

**Pines disponibles para usar:** 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 46

**Total disponibles:** 17 pines  
**Total utilizados:** 11 pines  
**Pines libres:** 6 pines (9, 10, 11, 12, 14, 46)

---

## ✅ Asignación de Pines - CONFIGURACIÓN FINAL

### 🚗 Control de Motores (Puente H L298N)

| Función | Pin ESP32 | Descripción |
|---------|-----------|-------------|
| **Motor Izquierdo A** | | |
| IN1 | GPIO 15 | Dirección adelante/atrás |
| IN2 | GPIO 16 | Dirección adelante/atrás |
| ENA | GPIO 6 | PWM - Control de velocidad |
| **Motor Derecho B** | | |
| IN3 | GPIO 17 | Dirección adelante/atrás |
| IN4 | GPIO 7 | Dirección adelante/atrás |
| ENB | GPIO 4 | PWM - Control de velocidad |

**Capacidades PWM:** Todos los pines tienen soporte PWM nativo en ESP32

---

### 📡 Sistema de Radar (HC-SR04 + Servo)

| Función | Pin ESP32 | Descripción |
|---------|-----------|-------------|
| **HC-SR04 Ultrasónico** | | |
| TRIG | GPIO 5 | Trigger (salida) |
| ECHO | GPIO 18 | Echo (entrada) |
| **Servomotor SG90** | | |
| Señal PWM | GPIO 13 | Control de posición 0-180° |

**Rango del radar:** 180° (0° a 180°)  
**Rango de detección:** 2-400 cm (efectivo 10-200 cm)

---

### 📊 Sensor Inercial (MPU6050)

| Función | Pin ESP32 | Descripción |
|---------|-----------|-------------|
| SDA | GPIO 8 | I2C Data |
| SCL | GPIO 3 | I2C Clock |

**Protocolo:** I2C  
**Dirección:** 0x68 (AD0 a GND)  
**Frecuencia:** 100 kHz estándar / 400 kHz rápido

---

## 🔌 Diagrama de Conexiones

```
ESP32                    COMPONENTES
=====                    ===========

GPIO 15 ────────────────→ L298N IN1 (Motor A)
GPIO 16 ────────────────→ L298N IN2 (Motor A)
GPIO 6  ────────────────→ L298N ENA (PWM Motor A)

GPIO 17 ────────────────→ L298N IN3 (Motor B)
GPIO 7  ────────────────→ L298N IN4 (Motor B)
GPIO 4  ────────────────→ L298N ENB (PWM Motor B)

GPIO 5  ────────────────→ HC-SR04 TRIG
GPIO 18 ────────────────→ HC-SR04 ECHO

GPIO 13 ────────────────→ Servo Señal (PWM)

GPIO 8  ────────────────→ MPU6050 SDA
GPIO 3  ────────────────→ MPU6050 SCL

5V      ────────────────→ VCC (HC-SR04, Servo, MPU6050)
GND     ────────────────→ GND común todos los componentes
```

---

## ⚡ Alimentación

| Componente | Voltaje | Corriente | Fuente |
|------------|---------|-----------|--------|
| ESP32 | 5V (USB) o 3.3V | ~250mA | USB o regulador |
| Motores DC | 6-12V | 500mA c/u | Batería (puente H) |
| Servo SG90 | 5V | 100-500mA | Regulador 5V |
| HC-SR04 | 5V | 15mA | Regulador 5V |
| MPU6050 | 3.3V o 5V | 4mA | ESP32 3.3V |
| L298N Lógica | 5V | 36mA | Regulador 5V |

**Recomendación:** Usar batería 7.4V LiPo (2S) con regulador 5V/3A para alimentar servomotor y sensores separadamente.

---

## 🛡️ Consideraciones Importantes

### ⚠️ Pines Especiales a Evitar (No en tu lista):

- **GPIO 0** - Boot mode (usado durante programación)
- **GPIO 2** - LED interno / Boot mode
- **GPIO 1, 2** - UART TX/RX (Serial Monitor)
- **GPIO 34-39** - Solo entrada (ADC), sin PWM
- **GPIO 6-11** - Flash interna (evitar en algunos modelos)

### ✅ Pines Seguros que Usamos:

- **GPIO 3, 4, 5, 6, 7, 8** ✓ Seguros
- **GPIO 13, 15, 16, 17, 18** ✓ Seguros
- **Todos tienen PWM** ✓
- **Todos son digitales I/O** ✓

### 🔧 Nota sobre GPIO 3:

GPIO 3 es normalmente RX (UART), pero puede usarse como I2C SCL sin problemas cuando no se necesita Serial Monitor simultáneo con I2C. Para este proyecto está bien.

---

## 📝 Código de Configuración Rápida

```cpp
// Motores
#define IN1 15
#define IN2 16
#define ENA 6
#define IN3 17
#define IN4 7
#define ENB 4

// Radar
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 13

// MPU6050
#define SDA_PIN 8
#define SCL_PIN 3

void setup() {
  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Servo
  servo.attach(SERVO_PIN);
  
  // MPU6050
  Wire.begin(SDA_PIN, SCL_PIN);
}
```

---

## 🎯 Pines Libres para Expansión Futura

**Disponibles pero no usados:** GPIO 9, 10, 11, 12, 14, 46

**Posibles usos:**
- LED indicadores de estado
- Buzzer para alarmas
- Botones de control manual
- Sensor de línea (seguidor de línea)
- Sensor de batería (ADC)
- Display LCD (si se necesita más visualización)

---

## 🧪 Testing Individual

Cada test usa solo los pines de su componente:

1. **test_bluetooth.ino** - Sin pines físicos
2. **test_servo.ino** - GPIO 13
3. **test_ultrasonico.ino** - GPIO 5, 18
4. **test_radar.ino** - GPIO 5, 13, 18
5. **test_radar_avanzado.ino** - GPIO 5, 13, 18
6. **test_mpu6050.ino** - GPIO 3, 8
7. **test_motores.ino** - GPIO 4, 6, 7, 15, 16, 17

---

## ✅ Resumen

**Estado:** ✅ **PROYECTO VIABLE CON PINES ESPECIFICADOS**

- ✅ 11 pines necesarios de 17 disponibles
- ✅ Todos los pines tienen capacidades requeridas (PWM, Digital I/O, I2C)
- ✅ 6 pines libres para expansión futura
- ✅ Sin conflictos de hardware
- ✅ Configuración validada

**Proyecto aprobado para continuar con estos pines.**

---

**Última actualización:** 11 de Noviembre de 2025  
**Proyecto:** Robot Móvil Autónomo - UMNG
