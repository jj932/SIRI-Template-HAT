/**
 * @file TemplateHAT_Firmware.ino
 * @brief Main Arduino sketch for TemplateHAT
 * @author SIRI Electrical Team
 * @date 2025
 * 
 * Hardware: Teensy 4.1 with TJA1050 CAN transceiver
 * IDE: Arduino IDE with Teensyduino add-on
 * 
 * Required Libraries:
 * - FlexCAN_T4 (install via Library Manager)
 * - ArduinoJson (install via Library Manager)
 */

#include "hat_config.h"
#include <array>
#include "ACAN2517FD.h"
#include "SPI.h"
#include "message_construction.h"
#include "state_machine.h"
#include "component_ctrl.h"
#include "hardware_map.h"
#include "motor_control.h"
#include "Arduino.h"

// Global objects
CANInterface canInterface;
HATStateMachine stateMachine;
ComponentController componentController;

// Timing variables
unsigned long lastHeartbeat = 0;
unsigned long lastTelemetry = 0;
unsigned long lastStateCheck = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(HAT_SERIAL_BAUD_RATE);
    delay(10000);
    
    #if HAT_DEBUG_ENABLED
    Serial.println("=== SIRI DriveHAT Starting ===");
    Serial.print("HAT Name: ");
    Serial.println(HAT_NAME);
    Serial.print("Version: ");
    Serial.println(HAT_VERSION);
    Serial.print("Node ID: 0x");
    Serial.println(HAT_NODE_ID, HEX);
    #endif
    
    // Initialize hardware
    initializeHardware();
    
    // Initialize subsystems
    initializeSubsystems();
    
    #if HAT_DEBUG_ENABLED
    Serial.println("=== Initialization Complete ===");
    #endif
}

void loop() {
    unsigned long currentTime = millis();
    
    // Process CAN messages
    std::array<CANFDMessage, 8> msg = processCANMessages();
    
    // Update state machine
    updateStateMachine(currentTime);
    
    // Update components
    updateComponents(msg);
    
    // Handle status indicators
    updateStatusIndicators();

    // Small delay to prevent overwhelming the system
    delay(1);
}

void initializeHardware() {
    // Initialize GPIO pins
    pinMode(PIN_LED_STATUS, OUTPUT);
    
    // Initialize status LEDs
    digitalWrite(PIN_LED_STATUS, LOW);
}

void initializeSubsystems() {
    // Initialize CAN interface
    if (!canInterface.initialize()) {
        #if HAT_DEBUG_ENABLED
        Serial.println("ERROR: CAN interface initialization failed");
        #endif
        // Handle initialization error
        handleInitializationError();
        return;
    }
    
    // Initialize state machine
    if (!stateMachine.initialize()) {
        #if HAT_DEBUG_ENABLED
        Serial.println("ERROR: State machine initialization failed");
        #endif
        handleInitializationError();
        return;
    }
    
    // Initialize component controller
    if (!componentController.initialize()) {
        #if HAT_DEBUG_ENABLED
        Serial.println("ERROR: Component controller initialization failed");
        #endif
        handleInitializationError();
        return;
    }
}


std::array<CANFDMessage, 8> processCANMessages() {
    std::array<CANFDMessage, 8> messages;

    // --- Node ID mapping (example) ---
    static constexpr uint8_t driveNode[4] = {
        NODE_DRIVE_FL,
        NODE_DRIVE_FR,
        NODE_DRIVE_RL,
        NODE_DRIVE_RR
    };

    static constexpr uint8_t steerNode[4] = {
        NODE_STEER_FL,
        NODE_STEER_FR,
        NODE_STEER_RL,
        NODE_STEER_RR
    };

    // --- DRIVE VELOCITY MESSAGES (index 0–3) ---
    for (int i = 0; i < 4; ++i) {
        messages[i] = buildVelocityMsg(
            driveNode[i],       // CAN node ID
            angular_vel[i],     // float velocity (rad/s)
            0.0f                // torque FF
        );
    }

    // --- STEERING POSITION MESSAGES (index 4–7) ---
    for (int i = 0; i < 4; ++i) {
        messages[4 + i] = buildPositionMsg(
            steerNode[i],        // CAN node ID
            steering_angle[i]    // float position (radians)
        );
    }

    return messages;
}


void updateStateMachine(unsigned long currentTime) {
    // Check for state timeouts
    if (currentTime - lastStateCheck >= HAT_STATE_TIMEOUT_MS) {
        stateMachine.handleTimeout();
        lastStateCheck = currentTime;
    }
}

void updateComponents(std::array<CANFDMessage, 8> msg) {
    // Update component states based on current HAT state
    componentController.update(msg);
}

void updateStatusIndicators() {
    // Update status LEDs based on system state
    updateStatusLEDs();
}

void handleInitializationError() {
    // Handle initialization errors
    digitalWrite(PIN_LED_STATUS, HIGH);
    // Additional error handling logic
    while (1) {
        digitalWrite(PIN_LED_STATUS, HIGH);
        delay(200);
        digitalWrite(PIN_LED_STATUS, LOW);
        delay(200);
    }
}

void updateStatusLEDs() {
    // Update status LEDs based on current state
    HAT_State_t currentState = stateMachine.getCurrentState();
    
    switch (currentState) {
        case STATE_POWER_OFF:
            digitalWrite(PIN_LED_STATUS, LOW);
            break;
        case STATE_DISARMED:
            // Slow blink - default state, waiting for Jetson pings
            digitalWrite(PIN_LED_STATUS, (millis() / 1000) % 2);
            break;
        case STATE_UNLOCKED:
            // Fast blink
            digitalWrite(PIN_LED_STATUS, (millis() / 250) % 2);
            break;
        case STATE_POWER_ARMED:
            digitalWrite(PIN_LED_STATUS, HIGH);
            break;
        case STATE_LOCKED:
            digitalWrite(PIN_LED_STATUS, LOW);
            break;
        case STATE_EMERGENCY_STOP:
            // Rapid flash
            digitalWrite(PIN_LED_STATUS, (millis() / 50) % 2);
            break;
    }
}