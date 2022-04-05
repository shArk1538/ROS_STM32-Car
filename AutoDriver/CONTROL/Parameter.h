#ifndef __PARAMETER_H
#define __PARAMETER_H
#include "INCLUDE.h"

extern uint8_t SendFlag;
extern uint8_t ControlFlag;
extern uint8_t StraightLineFlag;

extern int16_t Speed1;
extern int16_t Speed2;
extern float Distance;
extern int16_t MidDistance;

extern int16_t SpeedVal1;
extern int16_t SpeedVal2;

void PARA_Init(void);

#endif
