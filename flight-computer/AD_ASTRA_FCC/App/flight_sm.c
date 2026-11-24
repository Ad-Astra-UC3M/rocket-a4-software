#include "flight_sm.h"
#include "estimador.h"

static EstadoVuelo_t s_estado_actual = ESTADO_RAMPA;
static uint16_t s_status_bits = 0;
static uint32_t s_tiempo_despegue_ms = 0;
static bool s_pyro1_activo = false;
static bool s_pyro2_activo = false;

void FlightSM_Init(void)
{
    s_estado_actual = ESTADO_RAMPA;
    s_status_bits = 0;
    s_tiempo_despegue_ms = 0;
    s_pyro1_activo = false;
    s_pyro2_activo = false;
}

void FlightSM_Update(uint32_t tiempo_ms, float dt)
{
    (void)dt;

    float altitud = Estimador_GetAltitude();
    float vz = Estimador_GetVerticalVelocity();
    float ax = Estimador_GetAccelX();

    switch (s_estado_actual)
    {
        case ESTADO_RAMPA:
            if (ax >= LAUNCH_ACCEL_THRESHOLD)
            {
                s_estado_actual = ESTADO_PROPULSADO;
                s_tiempo_despegue_ms = tiempo_ms;
                s_status_bits |= BIT_LAUNCH_DETECTED;
            }
            break;

        case ESTADO_PROPULSADO:
            if ((tiempo_ms - s_tiempo_despegue_ms) >= BURNOUT_PRECAUTION_MS)
            {
                s_status_bits |= BIT_BURNOUT_EXPIRED;
                s_estado_actual = ESTADO_COASTING;
            }
            break;

        case ESTADO_COASTING:
            if (altitud >= MIN_ALTITUDE_THRESHOLD)
            {
                s_status_bits |= BIT_MIN_ALT_EXCEEDED;
            }

            if ((s_status_bits & BIT_MIN_ALT_EXCEEDED) && (vz < -1.5f))
            {
                s_status_bits |= BIT_ALTITUDE_DECREASING;
                s_status_bits |= BIT_DROGUE_COMMANDED;

                s_pyro1_activo = true;
                s_estado_actual = ESTADO_APOGEO_DROGUE;
            }
            break;

        case ESTADO_APOGEO_DROGUE:
            if (altitud <= MAIN_CHUTE_ALT_HIGH)
            {
                s_status_bits |= BIT_BELOW_MAIN_ALT;
                s_status_bits |= BIT_MAIN_COMMANDED;

                s_pyro2_activo = true;
                s_estado_actual = ESTADO_DESCENSO_MAIN;
            }
            break;

        case ESTADO_DESCENSO_MAIN:
            if (vz > -0.5f && vz < 0.5f && altitud < 50.0f)
            {
                s_estado_actual = ESTADO_ATERRIZADO;
            }
            break;

        case ESTADO_ATERRIZADO:
            s_pyro1_activo = false;
            s_pyro2_activo = false;
            break;

        default:
            break;
    }
}

EstadoVuelo_t FlightSM_GetEstado(void)
{
    return s_estado_actual;
}

uint16_t FlightSM_GetStatusBits(void)
{
    return s_status_bits;
}

bool FlightSM_IsPyro1Active(void)
{
    return s_pyro1_activo;
}

bool FlightSM_IsPyro2Active(void)
{
    return s_pyro2_activo;
}