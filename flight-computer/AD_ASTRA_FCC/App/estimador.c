#include "estimador.h"
#include <math.h>

static float s_altitud_filtrada = 0.0f;
static float s_altitud_previa = 0.0f;
static float s_velocidad_vertical = 0.0f;
static float s_accel_x = 0.0f;
static bool s_inicializado = false;

void Estimador_Init(float altitud_base)
{
    s_altitud_filtrada = altitud_base;
    s_altitud_previa = altitud_base;
    s_velocidad_vertical = 0.0f;
    s_accel_x = 0.0f;
    s_inicializado = true;
}

void Estimador_Update(float altitud_raw, float ax, float dt)
{
    if (!s_inicializado || dt <= 0.0f)
        return;

    float delta_alt = altitud_raw - s_altitud_filtrada;

    if (fabsf(delta_alt) > 50.0f)
    {
        altitud_raw = s_altitud_filtrada + (delta_alt > 0.0f ? 5.0f : -5.0f);
    }

    const float alpha = 0.25f;
    s_altitud_filtrada = s_altitud_filtrada + alpha * (altitud_raw - s_altitud_filtrada);

    s_velocidad_vertical = (s_altitud_filtrada - s_altitud_previa) / dt;
    s_altitud_previa = s_altitud_filtrada;

    s_accel_x = ax;
}

float Estimador_GetAltitude(void)
{
    return s_altitud_filtrada;
}

float Estimador_GetVerticalVelocity(void)
{
    return s_velocidad_vertical;
}

float Estimador_GetAccelX(void)
{
    return s_accel_x;
}