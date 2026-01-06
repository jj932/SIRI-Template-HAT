/**
 * @file can_interface.h
 * @brief CAN network interface header
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <stdint.h>
#include "message_construction.h"
#include "state_machine.h"
#include <FlexCAN_T4.h>

class CANInterface {
public:
    // Constructor/Destructor
    CANInterface();
    ~CANInterface();
    
    // Initialization
    bool initialize();
    
    // Message Transmission
    bool sendMessage(const CAN_message_t& message);
    
    // Message Reception
    bool receiveMessage(CAN_message_t& message);

};

extern CANInterface *CANInterfaceInstance;

#endif // CAN_INTERFACE_H