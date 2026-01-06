/**
 * @file TemplateHAT_Firmware.h
 * @brief Header file for TemplateHAT firmware
 * @author SIRI Electrical Team
 * @date 2025
 * 
 * Contains function declarations, global objects, and constants
 * for the TemplateHAT Arduino firmware.
 */

#ifndef TEMPLATEHAT_FIRMWARE_H
#define TEMPLATEHAT_FIRMWARE_H

#include <Arduino.h>
#include <array>
#include "ACAN2517FD.h"
#include "can_interface.h"
#include "state_machine.h"
#include "component_ctrl.h"
#include "hardware_map.h"
#include "hat_config.h"

// --- Global objects ---
extern CANInterface canInterface;
extern HATStateMachine stateMachine;
extern ComponentController componentController;

// --- Timing variables ---
extern unsigned long lastHeartbeat;
extern unsigned long lastTelemetry;
extern unsigned long lastStateCheck;

// --- Function declarations ---

/**
 * @brief Initialize all hardware pins and peripherals
 */
void initializeHardware();

/**
 * @brief Initialize subsystems like CAN, state machine, components
 */
void initializeSubsystems();

/**
 * @brief Process CAN messages for drive and steering nodes
 * @return Array of CANFDMessage (8 messages)
 */
std::array<CANFDMessage, 8> processCANMessages();

/**
 * @brief Update the state machine based on timeouts
 * @param currentTime Current millis()
 */
void updateStateMachine(unsigned long currentTime);

/**
 * @brief Update component controller with CAN messages
 * @param msg Array of CANFDMessage
 */
void updateComponents(std::array<CANFDMessage, 8> msg);

/**
 * @brief Update status LEDs based on current system state
 */
void updateStatusIndicators();

/**
 * @brief Handle any initialization errors
 */
void handleInitializationError();

/**
 * @brief Update status LEDs based on the state machine
 */
void updateStatusLEDs();

#endif // TEMPLATEHAT_FIRMWARE_H
