#ifndef ESTIMADOR_H
#define ESTIMADOR_H

#include <stdint.h>
#include <stdbool.h>

void Estimador_Init(float altitud_base);
void Estimador_Update(float altitud_raw, float ax, float dt);

float Estimador_GetAltitude(void);
float Estimador_GetVerticalVelocity(void);
float Estimador_GetAccelX(void);

// Compatibilidad por si main.c usa mayúsculas
#define ESTIMADOR_Init(alt) Estimador_Init(alt)
#define ESTIMADOR_Update(alt, ax, dt) Estimador_Update(alt, ax, dt)

#endif // ESTIMADOR_H