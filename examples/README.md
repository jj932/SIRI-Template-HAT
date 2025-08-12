# TemplateHAT Examples

This directory contains example sketches demonstrating various aspects of the TemplateHAT firmware.

## Examples

### can_test.ino
A simple CAN communication test that demonstrates:
- Basic SN65HVD230 CAN transceiver setup
- Sending and receiving CAN messages
- Extended CAN ID format usage
- Message parsing and handling
- LED status indicators

**Hardware Requirements:**
- Teensy 4.1
- SN65HVD230 CAN transceiver
- CAN bus with proper termination

**Usage:**
1. Open `can_test.ino` in Arduino IDE
2. Select Teensy 4.1 as the board
3. Upload to hardware
4. Open Serial Monitor at 115200 baud
5. Connect to CAN bus to see message exchange

**Expected Output:**
```
=== TemplateHAT CAN Test - SN65HVD230 ===
Initializing CAN interface...
CAN interface initialized at 500000 bps
Ready to send/receive messages...

[    2000] TX: ID=0x07FF0023, Data=07 01 00 00 07 D0 
[   10000] === CAN Statistics ===
Messages sent: 4
Messages received: 0
Uptime: 10 seconds
=====================
```

## Integration with Main Firmware

These examples demonstrate concepts that are integrated into the main TemplateHAT firmware:

- **CAN Interface**: The `can_test.ino` shows the basic CAN setup that's implemented in `src/can_interface.cpp`
- **Message Handling**: Examples of parsing and responding to different message types
- **Hardware Integration**: Proper pin assignments and LED indicators

## Testing with Other Devices

To test CAN communication:

1. **With Jetson Nano**: Use the CAN utilities on the Jetson side
2. **With Another HAT**: Load the same example on multiple Teensy boards
3. **With CAN Analyzer**: Use a commercial CAN bus analyzer tool

## Troubleshooting

**No CAN Messages:**
- Check CAN bus wiring (CANH, CANL, GND)
- Verify 120Ω termination resistors at both ends
- Confirm baud rate matches other devices (500 kbps default)

**Compilation Errors:**
- Ensure FlexCAN_T4 library is installed
- Verify Teensyduino is properly installed
- Check board selection is Teensy 4.1

**No Serial Output:**
- Verify baud rate is set to 115200
- Check USB connection and drivers
- Ensure Serial Monitor is open