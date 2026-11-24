
#ifndef FLIGHT_SM_H
#define FLIGHT_SM_H

#include <stdbool.h>
#include <stdint.h>

#define LAUNCH_ACCEL_THRESHOLD 20.0f
#define BURNOUT_PRECAUTION_MS 1500U
#define MIN_ALTITUDE_THRESHOLD 150.0f
#define MAIN_CHUTE_ALT_HIGH 600.0f

#define BIT_LAUNCH_DETECTED (1 << 0)
#define BIT_BURNOUT_EXPIRED (1 << 1)
#define BIT_MIN_ALT_EXCEEDED (1 << 2)
#define BIT_ALTITUDE_DECREASING (1 << 4)
#define BIT_DROGUE_COMMANDED (1 << 5)
#define BIT_BELOW_MAIN_ALT (1 << 6)
#define BIT_MAIN_COMMANDED (1 << 7)

typedef enum
{
    ESTADO_RAMPA = 0,
    ESTADO_PROPULSADO,
    ESTADO_COASTING,
    ESTADO_APOGEO_DROGUE,
    ESTADO_DESCENSO_MAIN,
    ESTADO_ATERRIZADO
} EstadoVuelo_t;

void FlightSM_Init(void);
void FlightSM_Update(uint32_t tiempo_ms, float dt);

EstadoVuelo_t FlightSM_GetEstado(void);
uint16_t FlightSM_GetStatusBits(void);
bool FlightSM_IsPyro1Active(void);
bool FlightSM_IsPyro2Active(void);

#endif // FLIGHT_SM_H