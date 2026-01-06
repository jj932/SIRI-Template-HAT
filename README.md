# Teensy CAN Bridge

This project implements a CAN bridge using a Teensy microcontroller to connect two independent CAN networks.

## CAN Networks

### 1. Jetson ↔ Teensy Network
- Uses the **FlexCANT4** library  
- Standard CAN

### 2. Teensy ↔ Peripheral Network
- Uses the **ACAN2517FD** library  
- **CANFD** (required for our peripherals)

---

## System Architecture

The Teensy acts as a bridge between the two CAN networks. Data is **not forwarded directly**. Instead, data is written into memory buffers and read asynchronously.

### Jetson → Peripherals (Drive Data Flow)

1. The **FlexCANT4** interface receives CAN messages from the Jetson network.
2. Messages are **stored in an array in memory**.
3. The **ACAN2517FD** interface reads the stored values when required and transmits them to the peripherals via CANFD.

---

### Peripherals → Jetson (Telemetry Data Flow)

Telemetry follows the same pattern in reverse:

1. The **ACAN2517FD** interface receives telemetry messages from the peripherals.
2. Telemetry values are **stored in a dedicated telemetry array**.
3. The **FlexCANT4** interface reads the telemetry values and transmits them to the Jetson network.

---

## Design Philosophy

To standardise our Teensy codebase, we use the following model:

- Each CAN network runs **asynchronously**
- All CAN data is written to and read from **shared memory arrays**
- Other parts of the system access these buffers as needed

This approach provides:

- Decoupled communication  
- Consistent behaviour across projects  
- Easier debugging  
- Improved reliability  

---

## Libraries Used

| Network | Library | Protocol |
|--------|--------|---------|
| Jetson ↔ Teensy | FlexCANT4 | CAN |
| Teensy ↔ Peripherals | ACAN2517FD | CANFD |

---

## Files
We will look at the role of each file in the repository.
- can_interface.cpp and can_interface.h: it's task is to handle the first CAN network. We want it to implement sending and receiving for now. Future iterations should allow handle errors more effectively. Rather than using threading use the inbuilt can.onReceive(fn) function to call the function that will update the shared memory arrays.
- can_protocol.cpp and can_protocol.h: these are the constants we will use for addressing, for setting CAN Baud rates.

## Summary

The Teensy functions as a CAN bridge by storing incoming data in memory buffers and forwarding it asynchronously between two independent CAN networks. This architecture supports both drive communication and telemetry in a clean and reliable way.
