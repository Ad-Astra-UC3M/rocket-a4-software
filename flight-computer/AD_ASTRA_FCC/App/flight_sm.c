#include "flight_sm.h"
#include <stdint.h>
#include <stdbool.h>
#include "estimador.h"

// Umbrales físicos de vuelo
#define LAUNCH_ACCEL_THRESHOLD 20.0f
#define MIN_ALTITUDE_THRESHOLD 150.0f
#define MAIN_CHUTE_ALT_HIGH 600.0f
#define BURNOUT_PRECAUTION_MS 1500U

static float float_abs(float val) {
    return (val < 0.0f) ? -val : val;
}

static FlightState_t current_state = STATE_1_STANDBY_IDLE;
static uint8_t status_data1 = 0x00;

static uint32_t burnout_timer_ms = 0;
static uint32_t landed_timer_ms = 0;

void FlightSM_Init(void) {
    current_state = STATE_1_STANDBY_IDLE;
    status_data1 = 0x00;
    burnout_timer_ms = 0;
    landed_timer_ms = 0;
}

void FlightSM_Update(uint32_t now_ms) {
    float altitude = Estimador_GetAltitude();
    float vertical_vel = Estimador_GetVerticalVelocity();
    float accel_x = Estimador_GetAccelX();

    switch (current_state) {

        case STATE_1_STANDBY_IDLE:
            status_data1 = 0x00;
            if (accel_x > LAUNCH_ACCEL_THRESHOLD) {
                status_data1 |= 0x01; // Bit 0: Lanzamiento
                current_state = STATE_2_LAUNCH_POWERED;
            }
            break;

        case STATE_2_LAUNCH_POWERED:
            if (accel_x <= 0.0f && altitude > 0.0f) {
                burnout_timer_ms = now_ms;
                current_state = STATE_3_COASTING_UNPOWERED;
            }
            break;

        case STATE_3_COASTING_UNPOWERED:
            if (now_ms - burnout_timer_ms >= BURNOUT_PRECAUTION_MS) {
                status_data1 |= 0x02; // Bit 1: Precaución burnout (1.5s)
            }

            if (altitude >= MIN_ALTITUDE_THRESHOLD) {
                status_data1 |= 0x04; // Bit 2: Altitud mínima (150m)
            }

            if (vertical_vel <= 0.0f && (status_data1 & 0x04)) {
                status_data1 |= 0x30; // Bit 4 (Apogeo) + Bit 5 (Disparo Pyro 1)
                current_state = STATE_4_APOGEE_DROGUE;
            }
            break;

        case STATE_4_APOGEE_DROGUE:
            if (vertical_vel < 0.0f && altitude <= MAIN_CHUTE_ALT_HIGH) {
                status_data1 |= 0xC0; // Bit 6 (Cota 600m) + Bit 7 (Disparo Pyro 2)
                current_state = STATE_5_MAIN_DESCENT;
            }
            break;

        case STATE_5_MAIN_DESCENT:
            if (float_abs(vertical_vel) < 0.5f && altitude < 50.0f) {
                if (landed_timer_ms == 0)
                    landed_timer_ms = now_ms;

                if (now_ms - landed_timer_ms >= 2000U) {
                    // 2 segundos en tierra
                    status_data1 = 0xFF; // Misión completada
                    current_state = STATE_6_LANDED;
                }
            } else {
                landed_timer_ms = 0;
            }
            break;

        case STATE_6_LANDED:
            break;
    }
}