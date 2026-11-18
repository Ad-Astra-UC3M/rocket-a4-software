#include "estimador.h"

static float estimador_altitud = 0.0f;
static float estimador_velocidad_v = 0.0f;
static float estimador_acel_x = 0.0f;

void ESTIMADOR_Init(void) {
    estimador_altitud = 0.0f;
    estimador_velocidad_v = 0.0f;
    estimador_acel_x = 0.0f;
}

void ESTIMADOR_Update(void) {
    // Aquí el estimador actualiza sus valores con las lecturas de los sensores
}

// --- Funciones que pide la máquina de estados (flight_sm.c) ---

float Estimador_GetAltitude(void) {
    return estimador_altitud;
}

float Estimador_GetVerticalVelocity(void) {
    return estimador_velocidad_v;
}

float Estimador_GetAccelX(void) {
    return estimador_acel_x;
}