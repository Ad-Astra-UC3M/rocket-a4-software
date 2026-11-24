#ifndef APP_SENSOR_HUB_H
#define APP_SENSOR_HUB_H

#include <stdint.h>
#include <stdbool.h>

void SENSOR_HUB_Init(void);
void SENSOR_HUB_Update(void);

float SENSOR_HUB_GetAltitude(void); 
float SENSOR_HUB_GetAccelZ(void);
#endif