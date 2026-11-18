
#ifndef FLIGHT_SM_H
#define FLIGHT_SM_H

#include <stdint.h>
#include <stdbool.h>

// Los 6 Estados de Vuelo normativos
typedef enum {
    STATE_1_STANDBY_IDLE = 0,       // 1. Inicio en Rampa (Data 1 = 0x00)
    STATE_2_LAUNCH_POWERED,         // 2. Despegue / Ascenso en Potencia (Data 1 = 0x01)
    STATE_3_COASTING_UNPOWERED,     // 3. Vuelo Libre / Ascenso sin Potencia (Data 1 = 0x07)
    STATE_4_APOGEE_DROGUE,          // 4. Apogeo y Paracaídas de Deriva (Data 1 = 0x37)
    STATE_5_MAIN_DESCENT,           // 5. Descenso y Paracaídas Principal (Data 1 = 0xF7)
    STATE_6_LANDED                  // 6. Aterrizaje / Cohete en Tierra (Data 1 = 0xFF)
} FlightState_t;

// Prototipos de funciones
void FlightSM_Init(void);
void FlightSM_Update(uint32_t now_ms);

#endif