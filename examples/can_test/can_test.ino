/**
 * @file can_test.ino
 * @brief Simple CAN test for SN65HVD230 integration
 * @author SIRI Electrical Team
 * @date 2025
 * 
 * This example demonstrates basic CAN communication using the SN65HVD230
 * transceiver with the TemplateHAT firmware structure.
 * 
 * Hardware: Teensy 4.1 with SN65HVD230 CAN transceiver
 * Connections:
 * - CAN TX: Pin 22
 * - CAN RX: Pin 23
 * - Status LED: Pin 13
 * - Comm LED: Pin 11
 */

#include <FlexCAN_T4.h>

// CAN interface
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

// Configuration
#define CAN_BAUDRATE 500000
#define HAT_NODE_ID 0x07
#define PIN_LED_STATUS 13
#define PIN_LED_COMM 11

// Message counters
uint32_t txCount = 0;
uint32_t rxCount = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {
        delay(10);
    }
    
    Serial.println("=== TemplateHAT CAN Test - SN65HVD230 ===");
    Serial.println("Initializing CAN interface...");
    
    // Initialize LEDs
    pinMode(PIN_LED_STATUS, OUTPUT);
    pinMode(PIN_LED_COMM, OUTPUT);
    digitalWrite(PIN_LED_STATUS, LOW);
    digitalWrite(PIN_LED_COMM, LOW);
    
    // Initialize CAN
    can1.begin();
    can1.setBaudRate(CAN_BAUDRATE);
    can1.setMaxMB(16);
    can1.enableFIFO();
    can1.enableFIFOInterrupt();
    
    Serial.print("CAN interface initialized at ");
    Serial.print(CAN_BAUDRATE);
    Serial.println(" bps");
    Serial.println("Ready to send/receive messages...\n");
    
    // Status LED on to indicate ready
    digitalWrite(PIN_LED_STATUS, HIGH);
}

void loop() {
    // Send test message every 2 seconds
    static unsigned long lastTx = 0;
    if (millis() - lastTx >= 2000) {
        sendTestMessage();
        lastTx = millis();
    }
    
    // Check for received messages
    receiveMessages();
    
    // Print statistics every 10 seconds
    static unsigned long lastStats = 0;
    if (millis() - lastStats >= 10000) {
        printStatistics();
        lastStats = millis();
    }
    
    delay(10);
}

void sendTestMessage() {
    CAN_message_t msg;
    
    // Build extended CAN ID (29-bit)
    // Priority: 7, Source: HAT_NODE_ID, Target: Broadcast (0xFF), MsgType: Heartbeat (0x23)
    msg.id = (7UL << 24) | (HAT_NODE_ID << 16) | (0xFF << 8) | 0x23;
    msg.flags.extended = 1;
    msg.len = 6;
    
    // Message payload
    msg.buf[0] = HAT_NODE_ID;           // Source node ID
    msg.buf[1] = 0x01;                  // Status: alive
    msg.buf[2] = (millis() >> 24) & 0xFF; // Timestamp
    msg.buf[3] = (millis() >> 16) & 0xFF;
    msg.buf[4] = (millis() >> 8) & 0xFF;
    msg.buf[5] = millis() & 0xFF;
    
    if (can1.write(msg)) {
        txCount++;
        Serial.printf("[%8lu] TX: ID=0x%08lX, Data=", millis(), msg.id);
        for (int i = 0; i < msg.len; i++) {
            Serial.printf("%02X ", msg.buf[i]);
        }
        Serial.println();
        
        // Flash comm LED
        digitalWrite(PIN_LED_COMM, HIGH);
        delay(50);
        digitalWrite(PIN_LED_COMM, LOW);
    } else {
        Serial.println("Failed to send CAN message");
    }
}

void receiveMessages() {
    CAN_message_t msg;
    
    while (can1.read(msg)) {
        rxCount++;
        
        // Parse CAN ID
        uint8_t priority = (msg.id >> 24) & 0x1F;
        uint8_t sourceID = (msg.id >> 16) & 0xFF;
        uint8_t targetID = (msg.id >> 8) & 0xFF;
        uint8_t msgType = msg.id & 0xFF;
        
        Serial.printf("[%8lu] RX: ID=0x%08lX P=%d S=0x%02X T=0x%02X MT=0x%02X Data=", 
                     millis(), msg.id, priority, sourceID, targetID, msgType);
        
        for (int i = 0; i < msg.len; i++) {
            Serial.printf("%02X ", msg.buf[i]);
        }
        Serial.println();
        
        // Flash comm LED
        digitalWrite(PIN_LED_COMM, HIGH);
        delay(50);
        digitalWrite(PIN_LED_COMM, LOW);
        
        // Handle specific message types
        handleReceivedMessage(msg, msgType);
    }
}

void handleReceivedMessage(const CAN_message_t& msg, uint8_t msgType) {
    switch (msgType) {
        case 0x23: // Heartbeat
            Serial.println("  -> Heartbeat received");
            break;
        case 0x10: // Status request
            Serial.println("  -> Status request received");
            sendStatusResponse();
            break;
        case 0xF0: // Emergency stop
            Serial.println("  -> EMERGENCY STOP received!");
            handleEmergencyStop();
            break;
        default:
            Serial.printf("  -> Unknown message type: 0x%02X\n", msgType);
            break;
    }
}

void sendStatusResponse() {
    CAN_message_t msg;
    
    // Build status response message
    msg.id = (7UL << 24) | (HAT_NODE_ID << 16) | (0xFF << 8) | 0x11; // Status response
    msg.flags.extended = 1;
    msg.len = 4;
    
    msg.buf[0] = HAT_NODE_ID;
    msg.buf[1] = 0x01; // Status OK
    msg.buf[2] = txCount & 0xFF;
    msg.buf[3] = rxCount & 0xFF;
    
    if (can1.write(msg)) {
        txCount++;
        Serial.println("Status response sent");
    }
}

void handleEmergencyStop() {
    // Flash status LED rapidly
    for (int i = 0; i < 10; i++) {
        digitalWrite(PIN_LED_STATUS, HIGH);
        delay(100);
        digitalWrite(PIN_LED_STATUS, LOW);
        delay(100);
    }
    digitalWrite(PIN_LED_STATUS, HIGH);
}

void printStatistics() {
    Serial.println("\n=== CAN Statistics ===");
    Serial.printf("Messages sent: %lu\n", txCount);
    Serial.printf("Messages received: %lu\n", rxCount);
    Serial.printf("Uptime: %lu seconds\n", millis() / 1000);
    Serial.println("=====================\n");
}