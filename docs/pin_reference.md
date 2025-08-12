# TemplateHAT Pin Reference

## Pin Naming Convention

To avoid conflicts with Teensy core definitions, the TemplateHAT uses a consistent naming convention:

- **System Pins**: Use standard names (e.g., `PIN_CAN_TX`, `PIN_LED_STATUS`)
- **HAT-Specific Pins**: Use `HAT_` prefix (e.g., `HAT_I2C_SDA`, `HAT_SPI_MOSI`)

## Pin Assignments (Teensy 4.1)

### System/Core Pins
```
PIN_CAN_TX        22    CAN transmit to SN65HVD230
PIN_CAN_RX        23    CAN receive from SN65HVD230
PIN_LED_STATUS    13    System status LED
PIN_LED_ERROR     12    Error indication LED
PIN_LED_COMM      11    Communication activity LED
PIN_BUTTON_TEST   10    Test button input
```

### HAT-Specific Communication Pins
```
HAT_I2C_SDA       18    I2C data line
HAT_I2C_SCL       19    I2C clock line
HAT_SPI_MOSI      26    SPI master out, slave in
HAT_SPI_MISO       1    SPI master in, slave out
HAT_SPI_SCK       27    SPI clock
HAT_SPI_CS         0    SPI chip select
```

### HAT-Specific Analog Pins
```
HAT_ANALOG_VOLTAGE  14    Voltage monitoring
HAT_ANALOG_CURRENT  15    Current monitoring
HAT_ANALOG_TEMP     16    Temperature monitoring
```

### HAT-Specific Digital I/O Pins
```
HAT_DIGITAL_OUT_1    2    Digital output 1
HAT_DIGITAL_OUT_2    3    Digital output 2
HAT_DIGITAL_OUT_3   20    Digital output 3
HAT_DIGITAL_OUT_4   21    Digital output 4
HAT_DIGITAL_IN_1     4    Digital input 1
HAT_DIGITAL_IN_2     5    Digital input 2
```

### HAT-Specific PWM Pins
```
HAT_PWM_OUT_1        6    PWM output 1
HAT_PWM_OUT_2        7    PWM output 2
HAT_PWM_OUT_3        8    PWM output 3
HAT_PWM_OUT_4        9    PWM output 4
```

## Usage Examples

### Using HAT-Specific Pins in Code

```cpp
// Initialize HAT-specific I2C
Wire.begin(HAT_I2C_SDA, HAT_I2C_SCL);

// Configure HAT-specific digital outputs
pinMode(HAT_DIGITAL_OUT_1, OUTPUT);
digitalWrite(HAT_DIGITAL_OUT_1, HIGH);

// Read HAT-specific analog input
int voltage = analogRead(HAT_ANALOG_VOLTAGE);

// Configure HAT-specific PWM output
analogWrite(HAT_PWM_OUT_1, 128);
```

### Component Configuration

```cpp
static const ComponentConfig_t COMPONENT_CONFIGS[] = {
    {HAT_DIGITAL_OUT_1, false, ADDR_COMPONENT_1, "Motor Controller"},
    {HAT_DIGITAL_OUT_2, false, ADDR_COMPONENT_2, "Servo Driver"},
    {HAT_PWM_OUT_1, false, ADDR_COMPONENT_3, "LED Strip"},
    {HAT_PWM_OUT_2, false, ADDR_COMPONENT_4, "Fan Controller"}
};
```

## Customization Guidelines

### For New HAT Development

1. **Keep System Pins**: Don't modify `PIN_CAN_*`, `PIN_LED_*`, `PIN_BUTTON_*`
2. **Customize HAT Pins**: Modify `HAT_*` definitions as needed for your hardware
3. **Update Component Config**: Adjust `COMPONENT_CONFIGS[]` array for your components
4. **Document Changes**: Update this reference for your specific HAT

### Pin Selection Best Practices

1. **CAN Pins (22, 23)**: Reserved for CAN communication - do not use for other purposes
2. **Status LEDs (11, 12, 13)**: Keep for debugging and status indication
3. **I2C Pins (18, 19)**: Default I2C pins for Teensy 4.1 - good choice for sensors
4. **SPI Pins**: Use alternative SPI pins to avoid conflicts with SD card or other SPI devices
5. **Analog Pins**: Use pins 14-16 for analog inputs, they have good ADC performance
6. **PWM Pins**: Most digital pins support PWM, choose based on your layout needs

## Conflict Resolution

If you encounter pin conflicts:

1. **Check Teensy Core Definitions**: Look in `pins_arduino.h` for reserved names
2. **Use HAT_ Prefix**: Always prefix HAT-specific pins with `HAT_`
3. **Avoid Standard Names**: Don't use names like `PIN_SPI_MOSI` that might conflict
4. **Test Compilation**: Verify no redefinition warnings during compilation

## Pin Availability

### Available for HAT Use
- Pins 0-10, 14-21, 24-41 (excluding system pins)
- Most pins support digital I/O
- Pins 14-27 support analog input
- Most pins support PWM output

### Reserved/Unavailable
- Pin 22: CAN TX (reserved)
- Pin 23: CAN RX (reserved)
- Pin 13: Status LED (recommended to keep)
- Pin 12: Error LED (recommended to keep)
- Pin 11: Comm LED (recommended to keep)
- Pin 10: Test button (recommended to keep)

This reference ensures consistent, conflict-free pin usage across all SIRI HAT modules.