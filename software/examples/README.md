# HUSB238 USB-C PD Module - Software Examples

This directory contains Arduino examples for the HUSB238 USB-C Power Delivery module using the Adafruit HUSB238 library.

## Library Requirements

- **Adafruit HUSB238 Library**: Official library for HUSB238 USB PD controller
  ```
  Install via Arduino Library Manager: "Adafruit HUSB238"
  ```
- **Wire Library**: Standard I2C communication (included with Arduino)

## Hardware Setup

- **I2C SDA**: Pin 6
- **I2C SCL**: Pin 7
- **I2C Address**: `0x08` (HUSB238_I2CADDR_DEFAULT)

## Examples

### Example 1: SCPI Command Interface

Located in `example_1/example_1.ino`

This example implements a SCPI-like command interface for controlling the HUSB238 USB PD controller via Serial communication.

#### Features
- Query device identification and status
- List available USB PD voltages
- Set specific PD voltage levels (5V, 9V, 12V, 15V, 18V, 20V)
- Query current capabilities
- Automatic voltage sweep testing

#### Supported Commands

| Command | Description | Example Response |
|---------|-------------|------------------|
| `*IDN?` | Device identification | `UNIT-DEVLAB,HUSB238,USBPD,1.0` |
| `STAT?` | Attachment status | `ATTACHED` or `UNATTACHED` |
| `PD:LIST?` | List available voltages | `5V 9V 15V 20V` |
| `PD:GET?` | Get current voltage | `PD=15` |
| `PD:SET <v>` | Set voltage (5,9,12,15,18,20) | `OK:SET 15V` or `ERR:UNAVAILABLE` |
| `PD:SWEEP` | Sweep through all voltages | `SWEEP 5V` ... `SWEEP DONE` |
| `CURR:GET?` | Get current capability | `CURR=3.0A` |
| `CURR:MAX? <v>` | Get max current at voltage | `MAX_CURR@15V=3.0A` |

#### Usage Example

1. **Upload the sketch** to your Arduino-compatible board
2. **Open Serial Monitor** at 115200 baud
3. **Send commands** (ensure "Newline" or "Both NL & CR" is selected)

```
*IDN?
UNIT-DEVLAB,HUSB238,USBPD,1.0

STAT?
ATTACHED

PD:LIST?
5V 9V 15V 20V

PD:SET 15
OK:SET 15V

CURR:GET?
CURR=3.0A

PD:SWEEP
SWEEP 5V
SWEEP 9V
SWEEP 15V
SWEEP 20V
SWEEP DONE
```

## Recommended Adafruit HUSB238 Library Usage

### Basic Initialization

```cpp
#include <Wire.h>
#include "Adafruit_HUSB238.h"

Adafruit_HUSB238 husb238;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  
  if (!husb238.begin(HUSB238_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("Failed to initialize HUSB238");
    while(1);
  }
}
```

### Check Connection Status

```cpp
if (husb238.isAttached()) {
  Serial.println("USB-C device attached");
}
```

### Query Available Voltages

```cpp
HUSB238_PDSelection voltages[] = {
  PD_SRC_5V, PD_SRC_9V, PD_SRC_12V, 
  PD_SRC_15V, PD_SRC_18V, PD_SRC_20V
};

for (int i = 0; i < 6; i++) {
  if (husb238.isVoltageDetected(voltages[i])) {
    Serial.print("Available: ");
    Serial.println(voltageValues[i]);
  }
}
```

### Set and Request PD Voltage

```cpp
// Select desired voltage
husb238.selectPD(PD_SRC_15V);

// Request the voltage change
husb238.requestPD();

// Verify the change
HUSB238_VoltageSetting current = husb238.getPDSrcVoltage();
```

### Query Current Capability

```cpp
// Get current for active voltage
HUSB238_CurrentSetting curr = husb238.getPDSrcCurrent();

// Get max current for specific voltage
HUSB238_CurrentSetting maxCurr = husb238.currentDetected(PD_SRC_20V);
```

### Check PD Response

```cpp
HUSB238_ResponseCodes response = husb238.getPDResponse();

if (response == SUCCESS) {
  // PD negotiation successful
} else if (response == NO_RESPONSE) {
  // No PD response from source
}
```

## Testing Recommendations

### 1. **Voltage Availability Test**
   - Connect various USB-C PD adapters
   - Use `PD:LIST?` to verify detected voltages
   - Confirm expected profiles (e.g., 45W = 5V/9V/15V/20V)

### 2. **Voltage Switching Test**
   - Use `PD:SET` to request each available voltage
   - Verify actual output with multimeter
   - Confirm negotiation success via response codes

### 3. **Current Capability Test**
   - Query `CURR:MAX?` for each voltage
   - Compare with adapter specifications
   - Verify power = voltage × current ≤ adapter rating

### 4. **Automated Sweep Test**
   - Use `PD:SWEEP` command
   - Monitor voltage transitions
   - Check for stability between switches (1.5s delay)

### 5. **Error Handling Test**
   - Request unavailable voltage: expect `ERR:UNAVAILABLE`
   - Send invalid voltage: expect `ERR:INVALID_VOLTAGE`
   - Test unattached state: expect `UNATTACHED`

### 6. **Integration Test**
   - Power actual load at each voltage level
   - Monitor for voltage drops under load
   - Verify sustained current delivery

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `Couldn't find HUSB238` | Check I2C wiring (SDA/SCL), verify I2C address |
| `ERR:UNAVAILABLE` | Requested voltage not supported by connected adapter |
| `NO_RESPONSE` | PD communication failed, check cable/adapter |
| Voltage won't change | Ensure `requestPD()` is called after `selectPD()` |

## Additional Resources

- [Adafruit HUSB238 Library Documentation](https://github.com/adafruit/Adafruit_HUSB238)
- [HUSB238 Datasheet](https://www.hynetek.com/downfile.aspx?id=332)
- [USB Power Delivery Specification](https://www.usb.org/usb-charger-pd)

## License

See main project LICENSE file.