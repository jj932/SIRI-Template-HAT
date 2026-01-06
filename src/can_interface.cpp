/**
 * @file can_interface.cpp
 * @brief CAN network interface implementation
 * @author SIRI Electrical Team
 * @date 2025
 */

#include "can_interface.h"
#include "message_construction.h"
#include "hat_config.h"
#include "hardware_map.h"
#include <FlexCAN_T4.h>
#include "Arduino.h"


// Pointer for function for callback
CANInterface *CANInterfaceInstance = nullptr;

float angular_vel[4] = {0};
float steering_angle[4] = {0};

// FlexCAN instance
FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> can;

//Callback function
void canSniffCallback(const CAN_message_t &msg) {
    CAN_message_t msg_copy = msg;
    if (CANInterfaceInstance != nullptr) {
        CANInterfaceInstance->receiveMessage(msg_copy);
    }
}

// Constructor implementation
CANInterface::CANInterface() {
    CANInterfaceInstance=this;
    Serial.begin(HAT_SERIAL_BAUD_RATE);
    can.begin();
    can.setBaudRate(CAN_BAUDRATE);
    can.enableMBInterrupts();
    can.onReceive(canSniffCallback);
}

CANInterface::~CANInterface() {
    // Destructor implementation
}

// Initialize CAN interface
bool CANInterface::initialize() {
    //Reject everything first
    can.setMBFilter(REJECT_ALL);

    //Wheel position / velocity message IDs
    can.setMBFilterRange(MB0, 0x200, 0x2FF);
    can.setMBFilterRange(MB1, 0x200, 0x2FF);
    can.setMBFilterRange(MB2, 0x200, 0x2FF);
    can.setMBFilterRange(MB3, 0x200, 0x2FF);

    //Emergency messages
    can.setMBFilter(MB4, MSG_TYPE_EMERGENCY_STOP);
    can.setMBFilter(MB5, MSG_TYPE_EMERGENCY_BATTERY);
    can.setMBFilter(MB6, MSG_TYPE_EMERGENCY_THERMAL);
    can.setMBFilter(MB7, MSG_TYPE_SYSTEM_SHUTDOWN);

    can.setMBFilterRange(MB8, 0x00, 0x7FF);

    can.mailboxStatus();

    // Initialise the wheel_pos to 0 and the steering positions to 0
    for (int i = 0; i < 4; ++i) {
        angular_vel[i] = 0;
        steering_angle[i] = 0;
    }

    return true;
}

bool CANInterface::sendMessage(const CAN_message_t& message) {
    // Send CAN message
    CAN_message_t messageCopy = message;
    for (int i = 0; i < 4; ++i) {
        messageCopy.len = 8;
        if (i == 0) {
            messageCopy.id = PRIORITY_DRIVE | MESSAGE_DRIVE_FRONT_LEFT_ENCODER;
        } else if (i == 1) {
            messageCopy.id = PRIORITY_DRIVE | MESSAGE_DRIVE_FRONT_RIGHT_ENCODER;
        } else if (i == 2) {
            messageCopy.id = PRIORITY_DRIVE | MESSAGE_DRIVE_REAR_LEFT_ENCODER;
        } else if (i == 3) {
            messageCopy.id = PRIORITY_DRIVE | MESSAGE_DRIVE_REAR_RIGHT_ENCODER;
        }

        floatToBytes(steering_angle[i], messageCopy.buf);
        floatToBytes(angular_vel[i], messageCopy.buf + 4);
        can.write(messageCopy);
    }
    return true; 
}

bool CANInterface::receiveMessage(CAN_message_t& message) {
    // Receive CAN message

    float theta = 0.0f;
    float omega = 0.0f;

    for (int i = 0; i < message.len; i++) {
    Serial.print(message.buf[i], HEX);
    Serial.print(" ");
    }
    Serial.println();

    memcpy(&theta, message.buf, sizeof(float));
    memcpy(&omega, message.buf + sizeof(float), sizeof(float));

    Serial.println(theta);
    Serial.println(omega); 

    if (message.id == (PRIORITY_DRIVE | MESSAGE_DRIVE_FRONT_LEFT)) {
        angular_vel[0] = omega;
        steering_angle[0] = theta;
    } else if (message.id == (PRIORITY_DRIVE | MESSAGE_DRIVE_FRONT_RIGHT)) {
        angular_vel[1] = omega;
        steering_angle[1] = theta;
    } else if (message.id == (PRIORITY_DRIVE | MESSAGE_DRIVE_REAR_LEFT)) {
        angular_vel[2] = omega;
        steering_angle[2] = theta;
    } else if (message.id == (PRIORITY_DRIVE | MESSAGE_DRIVE_REAR_RIGHT)) {
        angular_vel[3] = omega;
        steering_angle[3] = theta;
    }

    Serial.println(angular_vel[0]);


    return true;
}