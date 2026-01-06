/**
 * @file component_ctrl.cpp
 * @brief Component controller implementation
 * @author SIRI Electrical Team
 * @date 2025
 */

#include "component_ctrl.h"
#include "hat_config.h"
#include "hardware_map.h"
#include "ACAN2517FD.h"
#include "hardware_map.h"
#include "can_interface.h"
#include "Arduino.h"

ACAN2517FD* canController = nullptr; //Pointer to the component pin for dynamic initialization

ComponentController::ComponentController() {
    // Constructor implementation
}

ComponentController::~ComponentController() {
    // Destructor implementation
}

bool ComponentController::initialize() {
    SPI.begin();
    canController = new ACAN2517FD(SPI_CS, SPI, INT_PIN);

    ACAN2517FDSettings settings (ACAN2517FDSettings::OSC_20MHz,
                               CAN_BAUDRATE, ACAN2517FDSettings::DATA_BITRATE_x1) ;

    //settings.mRequestedMode = ACAN2517FDSettings::InternalLoopBack;

    const uint32_t errorCode = canController->begin(settings, [] { canController->isr(); }) ;

    if (errorCode != 0) {
    Serial.print("ACAN error: 0x");
    Serial.println(errorCode, HEX);
    }

    bool returnable = (errorCode == 0);

    return returnable;
    
}

void ComponentController::update(std::array<CANFDMessage, 8> msg) {
    // Velocity control
    const uint8_t nodeMapDrive[4] = {
        NODE_DRIVE_FL,
        NODE_DRIVE_FR,
        NODE_DRIVE_RL,
        NODE_DRIVE_RR
    };

    const uint8_t nodeMapSteer[4] = {
        NODE_STEER_FL,
        NODE_STEER_FR,
        NODE_STEER_RL,
        NODE_STEER_RR
    };


    for (int i = 0; i < 4; ++i) {

        CANFDMessage vel = buildVelocityMsg(
            nodeMapDrive[i],
            angular_vel[i],
            0.0f
        );

        delay(1);

        bool ok_vel = canController->tryToSend(vel);
        if (!ok_vel) {
            Serial.println("CAN TX Failure");
        }

        CANFDMessage pos = buildPositionMsg(
            nodeMapSteer[i],
            steering_angle[i],
            0.0f
        );

        bool ok_pos = canController->tryToSend(pos);
        if (!ok_pos) {
            Serial.println("CAN TX Failure");
        }
    }
}

void ComponentController::emergencyStop() {
    // Emergency stop all components
}