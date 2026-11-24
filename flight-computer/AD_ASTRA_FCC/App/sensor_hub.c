#include "sensor_hub.h"

static float s_altitud_raw = 0.0f;
static float s_accel_z = 0.0f;

void SENSOR_HUB_Init(void)
{
    s_altitud_raw = 0.0f;
    s_accel_z = 0.0f;
}

void SENSOR_HUB_Update(void)
{
    // Lectura continua de drivers de sensores
}

float SENSOR_HUB_GetAltitude(void)
{
    return s_altitud_raw;
}

float SENSOR_HUB_GetAccelZ(void)
{
    return s_accel_z;
}