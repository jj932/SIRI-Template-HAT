/**
 * @file hardware_map.h
 * @brief Pin and address mappings for HAT hardware
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef HARDWARE_MAP_H
#define HARDWARE_MAP_H

#include <stdint.h>
#include "hat_config.h"


// Desired locations for drive and steer
extern float angular_vel[4];
extern float steering_angle[4];

// Actual locations for drive and steer
//float angular_vel_telemetry[4];
//float wheel_pos_telemetry[4];
//float steering_angle_telemetry[4];

// GPIO Pin Definitions (Teensy 4.1)
#define PIN_CAN_TX 28
#define PIN_CAN_RX 29
#define PIN_LED_STATUS 13

// SPI Pins
#define SPI_CS 10
#define SPI_MOSI 11
#define SPI_MISO 12
#define SPI_SCK 13
#define INT_PIN 2

#define PRIORITY_ZERO 0x00 << 8
#define PRIORITY_JETSON 0x01 << 8
#define PRIORITY_DRIVE 0x02 << 8
#define PRIORITY_ARM 0x03 << 8
#define PRIORITY_BPS 0x04 << 8
#define PRIORITY_SCIENCE 0x05 << 8
#define PRIORITY_SENSE 0x06 << 8

// note 0x00 - 0x10 reserved for future use/high priority drive messages
// for drive messages upper four byte is used for "traction", lower four bytes for "steering"
#define MESSAGE_DRIVE_FRONT_LEFT 0x10
#define MESSAGE_DRIVE_FRONT_RIGHT 0x11
#define MESSAGE_DRIVE_REAR_LEFT 0x12
#define MESSAGE_DRIVE_REAR_RIGHT 0x13

#define MESSAGE_DRIVE_FRONT_LEFT_ENCODER 0x20
#define MESSAGE_DRIVE_FRONT_RIGHT_ENCODER 0x21
#define MESSAGE_DRIVE_REAR_LEFT_ENCODER 0x22
#define MESSAGE_DRIVE_REAR_RIGHT_ENCODER 0x23

static constexpr uint8_t NODE_DRIVE_FL = 4;
static constexpr uint8_t NODE_DRIVE_FR = 2;
static constexpr uint8_t NODE_DRIVE_RL = 3;
static constexpr uint8_t NODE_DRIVE_RR = 1;

static constexpr uint8_t NODE_STEER_FL = 5;
static constexpr uint8_t NODE_STEER_FR = 6;
static constexpr uint8_t NODE_STEER_RL = 7;
static constexpr uint8_t NODE_STEER_RR = 8;















// CAN Address Mappings
#define ADDR_STATE_QUERY (HAT_BASE_ADDRESS + 0xF0)
#define ADDR_STATE_RESPONSE (HAT_BASE_ADDRESS + 0xF1)
#define ADDR_STATE_TRANSITION (HAT_BASE_ADDRESS + 0xF2)
#define ADDR_EMERGENCY_STOP (HAT_BASE_ADDRESS + 0xF3)
#define ADDR_AUTHORITY_CHECK (HAT_BASE_ADDRESS + 0xF4)
#define ADDR_TIMEOUT_CONFIG (HAT_BASE_ADDRESS + 0xF5)

// Component Address Mappings (Template - to be customized per HAT)
#define ADDR_COMPONENT_1 (HAT_COMPONENT_BASE_ADDR + 0x00)
#define ADDR_COMPONENT_2 (HAT_COMPONENT_BASE_ADDR + 0x01)
#define ADDR_COMPONENT_3 (HAT_COMPONENT_BASE_ADDR + 0x02)
#define ADDR_COMPONENT_4 (HAT_COMPONENT_BASE_ADDR + 0x03)

// Hardware Configuration Structures
typedef struct {
    uint8_t pin;
    bool inverted;
    uint32_t address;
    const char* name;
} ComponentConfig_t;

typedef struct {
    uint8_t txPin;
    uint8_t rxPin;
    uint32_t baudrate;
    uint8_t nodeID;
} CANConfig_t;

typedef struct {
    uint8_t statusLED;
    uint8_t errorLED;
    uint8_t commLED;
    uint8_t testButton;
} StatusConfig_t;

// Hardware Configuration Constants
extern const CANConfig_t CAN_CONFIG;
extern const StatusConfig_t STATUS_CONFIG;
extern const ComponentConfig_t COMPONENT_CONFIGS[];

#endif // HARDWARE_MAP_H