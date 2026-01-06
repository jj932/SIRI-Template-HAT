/**
 * @file message_construction.cpp
 * @brief CAN protocol utility functions implementation
 * @author SIRI Electrical Team
 * @date 2025
 */

#include "message_construction.h"
#include "Arduino.h"

void floatToBytes(float f, uint8_t *out) {
    uint8_t *p = (uint8_t *)&f;
    for (int i = 0; i < 4; i++) out[i] = p[i];
}

CANFDMessage buildVelocityMsg(uint8_t node_id, float vel, float torque_ff = 0.0f) {
    CANFDMessage m;

    const uint8_t cmd = 0x0C; // Set_Input_Vel
    m.id  = (cmd << 5) | node_id;
    m.ext = false;
    m.len = 8;

    floatToBytes(vel,       &m.data[0]);
    floatToBytes(torque_ff, &m.data[4]);

    return m;
}

CANFDMessage buildPositionMsg(uint8_t node_id, float pos, float vel_ff = 0.0f) {
    CANFDMessage m;

    const uint8_t cmd = 0x0B; // Set_Input_Pos
    m.id  = (cmd << 5) | node_id;
    m.ext = false;
    m.len = 8;

    floatToBytes(pos,    &m.data[0]);
    floatToBytes(vel_ff, &m.data[4]);

    return m;
}