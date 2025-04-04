# ESP32 PDU Controller

## Why does this exist?

I have a relatively large home lab, and one of the pain points when using Canonical MaaS is turning the PCs on and off.

Since I couldn’t find a ready-made and affordable solution, I decided to build my own PDU.

---

This project implements a **PDU (Power Distribution Unit)** using an ESP32, allowing remote control of 12 outlets via HTTP. Each outlet includes:

- A **main power relay** (keeps the equipment powered on/off);
- An **ACPI pulse relay** (simulates a "power button" press for 50 ms).

Control is done using **PCF8574 I2C expanders**, with a simple HTTP interface and **JSON** responses.

---

## Features

- HTTP control for 12 individual outlets
- Commands to power on, power off, and trigger the ACPI pulse (for power on or shutdown)
- REST API with JSON responses
- Outlet status query, including power and ACPI state

---

## HTTP API

All routes follow the pattern:

http://<esp_ip>/outlet/<n>/<action>


| Method | Endpoint                 | Description                                      |
|--------|--------------------------|--------------------------------------------------|
| POST   | `/outlet/<n>/poweron`    | Powers on outlet `n`                             |
| POST   | `/outlet/<n>/poweroff`   | Powers off outlet `n`                            |
| POST   | `/outlet/<n>/acpi-on`    | Sends ACPI "power on" pulse to outlet `n`        |
| POST   | `/outlet/<n>/acpi-off`   | Sends ACPI "shutdown" pulse to outlet `n`        |
| GET    | `/outlet/<n>/status`     | Returns the status of outlet `n` in JSON         |

---

## Example Responses

**Success (201):**

```json
{ "status": "Outlet powered on" }
```

**Error (400)**
```json
{ "error": "Invalid outlet number" }
```

**Status(200)**
```json
{
  "outlet": 3,
  "power": true,
  "acpi": false
}
```
---
## Hardware
- ESP32 DevKit
- 3x PCF8574 (I2C expanders)
- Relay modules
- 5V power supply with enough current (e.g., 5V 5A "honeycomb" power supply used in this project)

### Pin Mapping
- I2C:
    - SDA = GPIO 22
    - SCL = GPIO 23

PCF8574 addresses:
- `0x20`, `0x21`, `0x22`

Each outlet and its corresponding "button" (pulse relay) is explicitly mapped in the tomadas_map.cpp file.

---
## Development
This project uses PlatformIO as a VSCode extension.

Dependencies are listed in the platformio.ini file, including:

- ESPAsyncWebServer
- AsyncTCP
- ArduinoJson
- PCF8574
