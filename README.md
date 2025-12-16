# 🏬 Mall Management System

A comprehensive desktop application for managing shopping mall operations, built with **Qt C++** and integrated with **Arduino-based IoT systems** for enhanced automation and monitoring.

---


##  Overview

The Mall Management System is a robust desktop application designed to streamline and automate various aspects of shopping mall operations. It provides a centralized platform for managing stores, tenants, staff, security, and real-time monitoring through integrated Arduino-based IoT devices.

---

##  Features

### 🏪 Core Management Features
- **Store Management**: Add, update, and track stores within the mall
- **Tenant Management**: Manage lease agreements, contracts, and tenant information
- **Staff Management**: Employee records, scheduling, and attendance tracking
- **Visitor Analytics**: Track foot traffic and visitor patterns
- **Financial Management**: Rent collection, expense tracking, and financial reporting
- **Maintenance Scheduling**: Plan and track maintenance activities

### 🔌 Arduino IoT Integration
- **Access Control System**: RFID/card-based entry management
- **Environmental Monitoring**: Temperature, humidity, and air quality sensors
- **Parking Management**: Real-time parking space availability tracking
- **Security System**: Motion detection and alarm integration
- **Energy Management**: Monitor and control lighting and HVAC systems
- **Emergency Response**: Automated alerts and evacuation systems


---

##  Technology Stack


- **Framework**: Qt 6.x (Qt Widgets)
- **Language**: C++17/C++20
- **Database**: MySQL
- **Communication**: Qt Serial Port for Arduino integration

### Hardware Integration
<p align="left">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/arduino/arduino-original.svg" alt="Arduino" width="50" height="50"/>
</p>

- **Microcontroller**: Arduino Uno/Mega
- **Sensors**: DHT22, PIR, RFID-RC522, Ultrasonic sensors
- **Communication Protocol**: Serial (USB)
- **Components**: LCD displays, LED indicators, buzzers, relays

---

##  System Architecture

```
┌─────────────────────────────────────────┐
│         Qt C++ Desktop App              │
│  ┌──────────────────────────────────┐   │
│  │     User Interface (Qt Widgets)  │   │
│  └──────────────────────────────────┘   │
│  ┌──────────────────────────────────┐   │
│  │   Business Logic Layer           │   │
│  └──────────────────────────────────┘   │
│  ┌──────────────────────────────────┐   │
│  │   Database Layer (Qt SQL)        │   │
│  └──────────────────────────────────┘   │
│  ┌──────────────────────────────────┐   │
│  │   Arduino Communication Layer    │   │
│  │      (Qt Serial Port)            │   │
│  └──────────────────────────────────┘   │
└─────────────────────────────────────────┘
                    │
                    │ Serial/USB/Bluetooth
                    │
┌─────────────────────────────────────────┐
│        Arduino System Layer             │
│  ┌──────────────────────────────────┐   │
│  │   Sensor Data Collection         │   │
│  └──────────────────────────────────┘   │
│  ┌──────────────────────────────────┐   │
│  │   Actuator Control               │   │
│  └──────────────────────────────────┘   │
│  ┌──────────────────────────────────┐   │
│  │   RFID Access Control            │   │
│  └──────────────────────────────────┘   │
└─────────────────────────────────────────┘
```

---

## 🔌 Arduino Integration


```cpp
// Example command format
serial = new QSerialPort(this);
serial->setPortName("COM3");
serial->setBaudRate(QSerialPort::Baud9600);

```

### Supported Arduino Modules

1. **Access Control Module**
   - RFID card reader integration
   - Real-time access logging
   - Authorized user database sync

2. **Environmental Monitoring Module**
   - Temperature and humidity sensors
   - Air quality monitoring
   - Automated HVAC control

3. **Parking Management Module**
   - Ultrasonic sensors for parking detection
   - LED indicators for available spots
   - Entry/exit gate automation

4. **Security Module**
   - PIR motion sensors
   - Emergency alarm system
   - Integration with surveillance cameras

---

## 📦 Installation

### Prerequisites

- **Qt Framework**: Qt 6.2 or higher
- **C++ Compiler**: GCC 9+ / MSVC 2019+ / Clang 10+
- **CMake**: Version 3.16 or higher
- **Database**: SQLite (included) or MySQL/PostgreSQL server
- **Arduino IDE**: For uploading sketches to Arduino boards


---


### Daily Operations

- **Dashboard**: View real-time mall statistics and alerts
- **Store Management**: Monitor store occupancy and performance
- **Access Control**: Grant/revoke access permissions
- **Monitoring**: Check environmental data and security status
- **Reports**: Generate daily, weekly, or monthly reports

---





⭐ **If you find this project useful, please consider giving it a star!**
