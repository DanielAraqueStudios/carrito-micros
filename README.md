# Robot Móvil Autónomo con ESP32
## Proyecto Final - Laboratorio de Microcontroladores

[![Universidad Militar Nueva Granada](https://img.shields.io/badge/UMNG-Ingeniería%20Mecatrónica-green)](https://www.umng.edu.co/)
[![ESP32](https://img.shields.io/badge/ESP32-Arduino-blue)](https://www.espressif.com/)
[![License](https://img.shields.io/badge/License-Academic-yellow)](LICENSE)

---

## 📋 Descripción del Proyecto

Sistema de control para un **robot móvil de navegación autónoma** implementado con **ESP32**, capaz de:
- Navegar autónomamente evitando obstáculos mediante sensor ultrasónico HC-SR04
- Controlar velocidad y dirección de motores DC
- Transmitir datos de posición en tiempo real vía Bluetooth
- Estimar trayectoria usando sensor inercial MPU6050
- Visualizar movimiento en aplicación GUI desktop/móvil

**Desarrollado por:** Daniel Araque  
**Programa:** Ingeniería en Mecatrónica - UMNG  
**Semestre:** VI - 2025  
**Curso:** Laboratorio de Microcontroladores

---

## 🎯 Objetivos

### Objetivo General
Diseñar e implementar un robot móvil de navegación autónoma usando ESP32, con control de velocidad/dirección basado en detección de obstáculos y reporte de movimiento a aplicación visual.

### Objetivos Específicos
1. Implementar sistema de evasión de obstáculos con sensor HC-SR04
2. Desarrollar control PWM para motores DC con driver L298N/TB6612
3. Integrar sensor MPU6050 para estimación de trayectoria
4. Establecer comunicación Bluetooth para transmisión de datos
5. Crear interfaz gráfica para visualización y control remoto
6. Documentar diseño electrónico y mecánico en planos CAD

---

## 🔧 Componentes de Hardware

### Microcontrolador
- **ESP32 DevKit V1** (30 pines GPIO, WiFi/Bluetooth integrado)

### Sensores
- **HC-SR04** - Sensor ultrasónico (detección de obstáculos 2-400cm)
  - Montado sobre servomotor para visión panorámica de 180°
- **MPU6050** - IMU 6 ejes (acelerómetro + giroscopio, comunicación I2C)
- **Servomotor SG90** - Control de orientación del sensor ultrasónico (0-180°)

### Actuadores
- **2x Motores DC** con reductora (6V, encoder opcional)
- **Driver L298N** o TB6612FNG (control PWM bidireccional)

### Alimentación
- **Batería LiPo 7.4V** (2S) o 4x pilas AA (6V)
- **Regulador de voltaje 5V** (para ESP32 y sensores)

### Estructura Mecánica
- Chasis acrílico/impreso 3D
- Ruedas con llanta de goma
- Rueda loca/castor
- Soportes para sensores y electrónica

---

## 📐 Conexiones de Pines

### ESP32 Pinout

| Componente | ESP32 Pin | Función |
|------------|-----------|---------|
| **HC-SR04** |  |  |
| Trigger | GPIO 5 | Salida Digital |
| Echo | GPIO 18 | Entrada Digital |
| **Servomotor SG90** |  |  |
| Señal (PWM) | GPIO 13 | Control de posición |
| **MPU6050** |  |  |
| SDA | GPIO 8 | I2C Data |
| SCL | GPIO 3 | I2C Clock |
| **L298N Driver** |  |  |
| IN1 | GPIO 15 | Motor A Dirección |
| IN2 | GPIO 16 | Motor A Dirección |
| IN3 | GPIO 17 | Motor B Dirección |
| IN4 | GPIO 7 | Motor B Dirección |
| ENA | GPIO 6 | Motor A PWM (Velocidad) |
| ENB | GPIO 4 | Motor B PWM (Velocidad) |
| **Alimentación** |  |  |
| VIN | 5V | Desde regulador |
| GND | GND | Común |

> **Nota:** Esta configuración usa solo los pines: 3, 4, 5, 6, 7, 8, 13, 15, 16, 17, 18 (11 pines de 17 disponibles)

> **Nota:** Todos los componentes deben compartir tierra común (GND)

---

## 🚀 Funcionalidades del Sistema

### 1. Navegación Autónoma
- **Sistema de radar panorámico**: Servo con HC-SR04 barre 180° (0° a 180°)
- **Detección inteligente**: Análisis de entorno en 3 sectores (Derecha, Centro, Izquierda)
- **Distancia de seguridad**: 30cm (configurable)
- **Algoritmo de evasión**:
  - Barrido completo del entorno cada ciclo
  - Análisis de mejor dirección disponible
  - Si obstáculo frontal → Retroceder
  - Buscar sector con mayor espacio libre
  - Girar hacia dirección despejada y continuar

### 2. Control de Motores
- **PWM de 8 bits** (0-255) para velocidad variable
- **Tres velocidades**: Baja (120), Media (180), Alta (240)
- **Movimientos**: Avanzar, Retroceder, Girar izquierda/derecha, Detenerse

### 3. Estimación de Trayectoria (MPU6050)
- Lectura de aceleración en ejes X, Y, Z
- Cálculo de ángulos de inclinación (pitch/roll)
- Integración numérica para estimar desplazamiento
- Frecuencia de muestreo: 50Hz

### 4. Comunicación Bluetooth
- **Protocolo**: Bluetooth Classic (SPP - Serial Port Profile)
- **Nombre del dispositivo**: "ESP32_Robot"
- **Baudrate**: 115200 bps
- **Formato de datos**: JSON strings
```json
{
  "x": 123.45,
  "y": 67.89,
  "angle": 45.2,
  "distance": 35,
  "speed": 180
}
```

### 5. Interfaz Gráfica (GUI)
- **Plataforma**: Python (Tkinter) / Processing / MIT App Inventor
- **Visualización en tiempo real**:
  - Gráfico XY de trayectoria estimada
  - Indicadores de distancia a obstáculo
  - Velocidad actual
- **Controles**:
  - Slider de velocidad (0-255)
  - Botones de movimiento manual
  - Botón de modo autónomo ON/OFF

---

## 📁 Estructura del Proyecto

```
carrito-micros/
├── README.md                    # Este archivo
├── arduino/
│   ├── main_robot/
│   │   └── main_robot.ino      # Código principal ESP32
│   ├── libraries/              # Bibliotecas necesarias
│   │   ├── MPU6050/
│   │   └── ESP32Servo/
│   └── tests/                  # Códigos de prueba individuales
│       ├── test_ultrasonico/
│       │   └── test_ultrasonico.ino
│       ├── test_motores/
│       │   └── test_motores.ino
│       ├── test_mpu6050/
│       │   └── test_mpu6050.ino
│       ├── test_bluetooth/
│       │   └── test_bluetooth.ino
│       ├── test_servo/
│       │   └── test_servo.ino
│       ├── test_radar/
│       │   └── test_radar.ino          # Servo + HC-SR04 básico
│       └── test_radar_avanzado/
│           └── test_radar_avanzado.ino # Sistema radar inteligente
├── gui/
│   ├── desktop_app.py          # Aplicación Python/Tkinter
│   ├── requirements.txt        # Dependencias Python
│   └── assets/                 # Iconos, imágenes
├── cad/
│   ├── electrical/
│   │   ├── schematic.pdf       # Esquemático eléctrico
│   │   └── pcb_layout.pdf      # Layout (si aplica)
│   └── mechanical/
│       ├── chassis.stl         # Modelo 3D del chasis
│       ├── sensor_mount.stl    # Soporte de sensores
│       └── assembly.pdf        # Plano de ensamblaje
├── docs/
│   ├── ieee_report.pdf         # Informe técnico IEEE
│   ├── user_manual.pdf         # Manual de usuario
│   ├── block_diagram.png       # Diagrama de bloques
│   └── flowchart.png           # Diagrama de flujo
├── media/
│   ├── demo_video.mp4          # Video demostración
│   └── photos/                 # Fotografías del prototipo
└── LICENSE                     # Licencia académica
```

---

## 📚 Bibliotecas Requeridas

### Arduino IDE
Instalar desde el Library Manager:

```
- Wire.h              (Incluida en Arduino - Comunicación I2C)
- BluetoothSerial.h   (Incluida en ESP32 Core)
- ESP32Servo.h        (Control de servomotores en ESP32)
- Adafruit_MPU6050.h  (Gestión del sensor MPU6050)
- Adafruit_Sensor.h   (Dependencia de MPU6050)
```

### Python (GUI Desktop)
```bash
pip install -r requirements.txt
```

Contenido de `requirements.txt`:
```
pyserial==3.5
matplotlib==3.7.1
tkinter
numpy==1.24.3
```

---

## 🛠️ Instalación y Configuración

### 1. Configurar Arduino IDE

```bash
# Instalar soporte para ESP32
1. File → Preferences → Additional Board Manager URLs:
   https://dl.espressif.com/dl/package_esp32_index.json

2. Tools → Board → Boards Manager → Buscar "ESP32" → Install

3. Tools → Board → ESP32 Arduino → "ESP32 Dev Module"

4. Tools → Upload Speed → 115200
```

### 2. Clonar Repositorio

```bash
git clone https://github.com/DanielAraqueStudios/carrito-micros.git
cd carrito-micros
```

### 3. Cargar Código al ESP32

```bash
1. Conectar ESP32 por USB
2. Abrir arduino/main_robot/main_robot.ino
3. Verificar selección de puerto COM correcto
4. Click en "Upload" (→)
5. Esperar mensaje "Done uploading"
```

### 4. Ejecutar Aplicación GUI

```bash
cd gui
python desktop_app.py
```

---

## 🧪 Pruebas y Validación

### Metodología de Sprints

El desarrollo sigue una metodología de **sprints** con pruebas incrementales:

**Sprint 1:** Pruebas individuales de componentes  
**Sprint 2:** Integración de subsistemas  
**Sprint 3:** Sistema completo y optimización  

### Pruebas Unitarias (Tests Individuales)

Cada componente tiene su programa de prueba independiente con salida por Serial Monitor.

#### Test 1: Bluetooth (test_bluetooth.ino)
```cpp
// Archivo: tests/test_bluetooth/test_bluetooth.ino
// Verificar comunicación Bluetooth Classic
// Nombre: "ESP32_Robot" | Baudrate: 115200
// Comandos: F/B/L/R/S/V
```
**Validación:**
- ✅ Emparejamiento exitoso
- ✅ Envío de datos cada 1 segundo
- ✅ Recepción de comandos sin demora
- ✅ Conexión estable por 30+ minutos

#### Test 2: Servomotor (test_servo.ino)
```cpp
// Archivo: tests/test_servo/test_servo.ino
// Probar movimiento del servo de 0° a 180°
// Barridos lentos, rápidos y posiciones clave
```
**Validación:**
- ✅ Movimiento suave sin vibraciones
- ✅ Alcanza posiciones exactas (0°, 90°, 180°)
- ✅ Sin reinicios del ESP32 durante operación

#### Test 3: Sensor Ultrasónico (test_ultrasonico.ino)
```cpp
// Archivo: tests/test_ultrasonico/test_ultrasonico.ino
// Test SOLO del HC-SR04 (sin servo)
// Sensor fijo midiendo distancias
// Clasificación automática: Muy cerca / Cerca / Media / Lejos
```
**Validación:**
- ✅ Mediciones estables (±2cm de error)
- ✅ Detección confiable de 5-200cm
- ✅ Respuesta < 50ms por lectura
- ✅ Formato claro con clasificación de distancia

#### Test 4: Sistema Radar Básico (test_radar.ino)
```cpp
// Archivo: tests/test_radar/test_radar.ino
// Servo + HC-SR04 en barrido continuo
// Salida CSV: angulo,distancia
```
**Validación:**
- ✅ Barrido completo de 180° sin interrupciones
- ✅ Mediciones consistentes en cada ángulo
- ✅ Formato CSV correcto para visualización

#### Test 5: Radar Avanzado (test_radar_avanzado.ino)
```cpp
// Archivo: tests/test_radar_avanzado/test_radar_avanzado.ino
// Sistema completo con análisis de entorno
// Detección inteligente en 3 sectores
// Recomendación automática de dirección
```
**Validación:**
- ✅ Mapa de distancias completo (19 puntos)
- ✅ Identificación correcta de mejor dirección
- ✅ Análisis por sectores preciso

#### Test 6: Control de Motores (test_motores.ino)
```cpp
// Archivo: tests/test_motores/test_motores.ino
// Probar 2 motores con Puente H
// 6 pruebas: Avanzar (3 velocidades), Retroceder, Girar I/D
// IMPORTANTE: Elevar robot antes de probar
```
**Validación:**
- ✅ Ambos motores giran correctamente
- ✅ 3 velocidades funcionales (150, 200, 255)
- ✅ Giros precisos sin deriva
- ✅ Control PWM estable

#### Test 7: MPU6050 (test_mpu6050.ino)
```cpp
// Archivo: tests/test_mpu6050/test_mpu6050.ino
// Calibración del sensor (superficie plana)
// Validar lecturas de aceleración y ángulos
```
**Validación:**
- ✅ Comunicación I2C exitosa (dirección 0x68)
- ✅ Lecturas de aceleración estables
- ✅ Cálculo de ángulos pitch/roll correcto
- ✅ Detección de movimiento funcional

### Pruebas Integradas

| Prueba | Descripción | Criterio de Éxito |
|--------|-------------|-------------------|
| **Sistema Radar** | Barrido 180° con detección | Mapa completo sin errores |
| **Navegación** | Robot en pista con obstáculos | Evita 10/10 obstáculos |
| **Estabilidad BT** | Operación continua | 30 min sin desconexión |
| **Precisión Trayectoria** | Recorrido cuadrado 1m×1m | Error < 15% |
| **Control Remoto** | Cambio de velocidad desde GUI | Respuesta < 500ms |

### Orden Recomendado de Pruebas

```
Sprint 1 - Pruebas Individuales:
1. ✅ test_bluetooth       (sin hardware adicional)
2. ✅ test_servo           (solo servomotor)
3. ✅ test_ultrasonico     (HC-SR04 fijo en mesa)
4. ✅ test_radar           (servo + HC-SR04 básico)
5. ✅ test_radar_avanzado  (sistema radar completo)
6. ✅ test_mpu6050         (requiere biblioteca Adafruit)
7. ✅ test_motores         (⚠️ elevar robot del suelo)

Sprint 2 - Integración:
- Radar + Motores (navegación básica)
- MPU6050 + Motores (tracking de trayectoria)
- Bluetooth + Control completo

Sprint 3 - Sistema Final:
- Todos los componentes integrados
- Ajuste fino de algoritmos
- Optimización de consumo
```

---

## 📊 Resultados Esperados

### Métricas de Desempeño

- **Autonomía**: 45 minutos con batería 2200mAh
- ✅ **Velocidad máxima**: 0.5 m/s
- ✅ **Rango de detección radar**: 180° (0° a 180°)
- ✅ **Resolución angular**: 10° (19 posiciones)
- ✅ **Distancia de detección**: 2-300 cm (efectiva 10-200cm)
- ✅ **Tiempo de barrido completo**: ~4 segundos
- ✅ **Tiempo de reacción**: < 100ms desde detección a frenado
- ✅ **Frecuencia de transmisión BT**: 10Hz (100ms/paquete)
- ✅ **Precisión de trayectoria**: ±10cm en recorrido de 2m

### Entregables

1. ✅ Prototipo físico funcional
2. ✅ Código fuente documentado (GitHub)
3. ✅ Aplicación GUI operativa
4. ✅ Planos CAD electrónicos y mecánicos
5. ✅ Informe técnico formato IEEE
6. ✅ Video demostración (3-5 minutos)
7. ✅ Manual de usuario

---

## 🐛 Troubleshooting

### Problemas Comunes

**ESP32 no se detecta en Arduino IDE**
```
- Instalar driver CH340/CP2102 según chip USB
- Presionar botón BOOT al cargar código
- Verificar cable USB con capacidad de datos
```

**MPU6050 no responde**
```
- Verificar conexiones SDA/SCL
- Confirmar dirección I2C (0x68 o 0x69)
- Usar Wire.begin(21, 22) explícitamente
```

**Servo vibra o se mueve erráticamente**
```
- Reducir velocidad de barrido (aumentar delays)
- Verificar alimentación estable de 5V
- Usar fuente externa si el ESP32 se reinicia
- Agregar capacitor 100µF en VCC del servo
```

**Radar da lecturas inconsistentes**
```
- Aumentar delay de estabilización del servo (>50ms)
- Tomar promedio de múltiples mediciones
- Verificar que HC-SR04 esté bien montado (sin vibración)
- Alejar HC-SR04 de superficies metálicas
```

**Bluetooth no empareja**
```
- Reiniciar ESP32
- Olvidar dispositivo en configuración Bluetooth del PC/móvil
- Verificar que no esté conectado a otro dispositivo
```

**Robot gira en círculos**
```
- Calibrar velocidades de motores (pueden ser diferentes)
- Verificar polaridad de conexiones L298N
- Ajustar valores PWM para compensar desbalance
```

---

## 📖 Documentación Adicional

- **[Datasheet ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)**
- **[HC-SR04 Guide](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf)**
- **[MPU6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)**
- **[L298N Tutorial](https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/)**

---

## 👥 Contribuciones

Este es un proyecto académico individual. Sin embargo, sugerencias y mejoras son bienvenidas:

1. Fork el repositorio
2. Crea una rama para tu feature (`git checkout -b feature/mejora`)
3. Commit tus cambios (`git commit -m 'Agrega nueva funcionalidad'`)
4. Push a la rama (`git push origin feature/mejora`)
5. Abre un Pull Request

---

## 📄 Licencia

Este proyecto es desarrollado con fines **académicos** para la Universidad Militar Nueva Granada.

**Derechos de Autor © 2025 - Daniel Araque Studios**

> El uso no autorizado de su contenido así como reproducción total o parcial por cualquier persona o entidad, estará en contra de los derechos de autor.

---

## 🙏 Agradecimientos

- **I.E. Robinson Jiménez Moreno PhD.** - Guía de proyecto
- **I.E. Juan Ricardo Clavijo Mendoza MSc.** - Supervisor de laboratorio
- **Universidad Militar Nueva Granada** - Programa de Ingeniería Mecatrónica
- Comunidad de **Arduino** y **ESP32** por documentación y bibliotecas

---

## 📞 Contacto

**Desarrollador:** Daniel Araque  
**Universidad:** Universidad Militar Nueva Granada  
**Programa:** Ingeniería en Mecatrónica  
**Repositorio:** [github.com/DanielAraqueStudios/carrito-micros](https://github.com/DanielAraqueStudios/carrito-micros)

---

<div align="center">

**⭐ Si este proyecto te resulta útil, considera darle una estrella en GitHub ⭐**

![Robot Demo](media/robot_preview.png)

*Robot móvil autónomo ESP32 - Proyecto Final Micros 2025*

</div>
