# SN65HVD230 CAN Transceiver Integration Guide

## Overview

The TemplateHAT firmware includes a complete driver implementation for the SN65HVD230 CAN transceiver, providing robust CAN bus communication for SIRI rover HAT modules.

## Hardware Connections

### Teensy 4.1 to SN65HVD230
```
Teensy 4.1    SN65HVD230    Function
Pin 22    ->  TXD           CAN TX Data (PIN_CAN_TX)
Pin 23    ->  RXD           CAN RX Data (PIN_CAN_RX)
3.3V      ->  VCC           Power Supply
GND       ->  GND           Ground
```

### HAT-Specific Pin Definitions
All HAT-specific pins use the `HAT_` prefix to avoid conflicts with Teensy core definitions:
```
I2C:     HAT_I2C_SDA (18), HAT_I2C_SCL (19)
SPI:     HAT_SPI_MOSI (26), HAT_SPI_MISO (1), HAT_SPI_SCK (27), HAT_SPI_CS (0)
Analog:  HAT_ANALOG_VOLTAGE (14), HAT_ANALOG_CURRENT (15), HAT_ANALOG_TEMP (16)
Digital: HAT_DIGITAL_OUT_1-4, HAT_DIGITAL_IN_1-2
PWM:     HAT_PWM_OUT_1-4
```

### SN65HVD230 to CAN Bus
```
SN65HVD230    CAN Bus       Function
CANH      ->  CAN High      Differential High
CANL      ->  CAN Low       Differential Low
GND       ->  GND           Ground Reference
```

## Software Implementation

### FlexCAN_T4 Configuration

The driver uses the FlexCAN_T4 library with the following configuration:

```cpp
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

// Initialization
can1.begin();
can1.setBaudRate(500000);  // 500 kbps
can1.setMaxMB(16);         // 16 mailboxes
can1.enableFIFO();         // Enable FIFO mode
can1.enableFIFOInterrupt();
```

### Message Format

The implementation uses 29-bit extended CAN IDs with the following structure:

```
Bits 28-24: Priority (5 bits)
Bits 23-16: Source Node ID (8 bits)
Bits 15-8:  Target Node ID (8 bits) - 0xFF for broadcast
Bits 7-0:   Message Type/Register (8 bits)
```

### Key Features

1. **Extended CAN ID Support**: Full 29-bit addressing
2. **Message Filtering**: Configurable filters for relevant messages
3. **Error Handling**: Comprehensive error detection and recovery
4. **Statistics Tracking**: Message counts and error rates
5. **Debug Logging**: Detailed message logging for development
6. **LED Indicators**: Visual feedback for communication activity

## Usage Examples

### Basic Message Transmission

```cpp
CANMessage_t message;
message.id = buildCANID(priority, sourceID, targetID, msgType);
message.length = dataLength;
// Fill message.data[] with payload
canInterface.sendMessage(message);
```

### Message Reception

```cpp
CANMessage_t message;
if (canInterface.receiveMessage(message)) {
    // Process received message
    uint8_t priority, sourceID, targetID, msgType;
    parseCANID(message.id, &priority, &sourceID, &targetID, &msgType);
    // Handle based on message type
}
```

### Heartbeat Transmission

```cpp
// Automatic heartbeat every 1 second
canInterface.sendHeartbeat();
```

## Configuration Options

### Baud Rate Selection

The default baud rate is 500 kbps to match Jetson configuration:

```cpp
#define CAN_BAUDRATE 500000   // 500 kbps
```

Alternative rates can be configured:
- 125 kbps: `#define CAN_BAUDRATE 125000`
- 250 kbps: `#define CAN_BAUDRATE 250000`
- 1 Mbps: `#define CAN_BAUDRATE 1000000`

### Node ID Assignment

Each HAT must have a unique node ID:

```cpp
#define HAT_NODE_ID 0x07  // Template HAT
```

Refer to `hw_inf_mappings.md` for assigned node IDs.

### Message Filtering

Configure filters to accept only relevant messages:

```cpp
void setupCANFilters() {
    // Accept messages for this HAT or broadcast
    can1.setFIFOFilter(ACCEPT_ALL);  // Or specific filters
}
```

## Testing and Validation

### CAN Test Example

Use the provided `examples/can_test/can_test.ino` to verify:

1. **Basic Communication**: Send/receive test messages
2. **Message Parsing**: Correct ID field extraction
3. **Error Handling**: Response to bus errors
4. **Statistics**: Message count tracking

### Expected Test Output

```
=== TemplateHAT CAN Test - SN65HVD230 ===
Initializing CAN interface...
CAN interface initialized at 500000 bps
Ready to send/receive messages...

[    2000] TX: ID=0x07FF0023, Data=07 01 00 00 07 D0 
[    4000] TX: ID=0x07FF0023, Data=07 01 00 00 0F A0 
```

### Integration Testing

1. **With Jetson**: Verify communication with rover computer
2. **Multi-HAT**: Test message routing between HATs
3. **Bus Loading**: Validate performance under high message rates

## Troubleshooting

### Common Issues

**No CAN Communication:**
- Check physical connections (CANH, CANL, GND)
- Verify 120Ω termination resistors at bus ends
- Confirm matching baud rates across all devices

**Message Corruption:**
- Check for proper bus termination
- Verify power supply stability (3.3V for SN65HVD230)
- Ensure adequate ground connections

**High Error Rates:**
- Monitor bus loading and message timing
- Check for electromagnetic interference
- Verify CAN transceiver power supply quality

### Debug Features

Enable debug output for detailed logging:

```cpp
#define HAT_DEBUG_ENABLED 1
```

This provides:
- Message transmission/reception logs
- CAN ID parsing details
- Error condition reporting
- Statistics updates

### LED Indicators

- **Status LED (Pin 13)**: System state indication
- **Error LED (Pin 12)**: CAN error conditions
- **Comm LED (Pin 11)**: Communication activity flash

## Performance Characteristics

### Message Throughput

- **Maximum Rate**: ~8000 messages/second at 500 kbps
- **Typical Usage**: 100-1000 messages/second for telemetry
- **Heartbeat Rate**: 1 message/second per HAT

### Latency

- **Transmission**: <1ms for high-priority messages
- **Processing**: <100μs for message handling
- **End-to-End**: <5ms typical system response

### Reliability

- **Error Detection**: CRC, bit stuffing, frame format checks
- **Error Recovery**: Automatic retransmission on errors
- **Bus Monitoring**: Continuous error rate tracking

## Integration with SIRI System

The SN65HVD230 driver integrates seamlessly with:

- **State Machine**: CAN commands trigger state transitions
- **Telemetry**: Sensor data transmitted via CAN
- **Component Control**: Remote component commands via CAN
- **Emergency Systems**: Immediate emergency stop propagation

This implementation provides a robust, tested foundation for CAN communication in SIRI rover HAT modules.