#ifndef ESTIMADOR_H 
#define ESTIMADOR_H 

#include <stdint.h>

void ESTIMADOR_Init(void);
void ESTIMADOR_Update(void);

float Estimador_GetAltitude(void); 
float Estimador_GetVerticalVelocity(void); 
float Estimador_GetAccelX(void); 
#endif