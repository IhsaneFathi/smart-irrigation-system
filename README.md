# 🌱 Smart Irrigation System — IoT Based

## 📌 Project Overview

This project is an IoT-based Smart Irrigation System designed to optimize water usage in agriculture through real-time monitoring and automatic irrigation control.

The system uses an ESP32 microcontroller connected to multiple sensors to monitor environmental conditions such as:

- Soil moisture
- Temperature & humidity
- Rain detection
- Light intensity
- Motion detection
- Water level

Data is sent to the cloud using the MQTT protocol and visualized on the ThingsBoard platform.

---

# 🎯 Objectives

The main objectives of this project are:

- Reduce water waste in agriculture
- Automate irrigation based on soil conditions
- Enable remote monitoring and control
- Provide real-time telemetry using IoT technologies

---

# 🏗️ System Architecture

The architecture consists of:

1. Sensors connected to ESP32
2. ESP32 sends data using MQTT
3. ThingsBoard cloud platform receives telemetry
4. Automatic or manual irrigation control
5. Water pump activation through relay module

---

# 🔧 Hardware Components

| Component | Description |
|---|---|
| ESP32 | Main WiFi microcontroller |
| Soil Moisture Sensor | Measures soil humidity |
| DHT22 | Temperature & humidity sensor |
| LDR Sensor | Light intensity sensor |
| Rain Sensor | Rain detection |
| PIR Sensor | Motion detection |
| Relay Module | Controls the pump |
| Water Pump | Irrigation actuator |

---

# 💻 Technologies Used

- Embedded C / Arduino
- ESP32
- MQTT Protocol
- ThingsBoard IoT Platform
- WiFi Communication
- Wokwi Simulation

---

# ⚙️ Features

✅ Automatic irrigation based on soil moisture  
✅ Manual/Automatic mode switching  
✅ Remote pump control  
✅ Real-time cloud monitoring  
✅ Rain detection safety system  
✅ Motion detection integration  
✅ Smart night/rain logic  

---

# 📡 MQTT & Cloud

The ESP32 communicates with ThingsBoard using MQTT telemetry.

Platform used:
- ThingsBoard Cloud

MQTT Broker:
```cpp
mqtt.thingsboard.cloud
