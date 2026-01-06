/**
 * @file component_ctrl.h
 * @brief Component controller header
 * @author SIRI Electrical Team
 * @date 2025
 */

#ifndef COMPONENT_CTRL_H
#define COMPONENT_CTRL_H

#include <stdint.h>
#include <array>
#include "ACAN2517FD.h"
#include "hardware_map.h"

class ComponentController {
public:
    // Constructor/Destructor
    ComponentController();
    ~ComponentController();
    
    // Initialization
    bool initialize();
    void update(std::array<CANFDMessage, 8> msg);
    
    // Safety Functions
    void emergencyStop();
};

#endif // COMPONENT_CTRL_H