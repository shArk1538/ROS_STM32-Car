#ifndef __LED_H
#define __LED_H
#include "INCLUDE.h"

////////////////////////////////////////////////////////////////////////


//					D13: LED1->PB0 ; D14: LED2->PB1
//					LED��ʼ��������


////////////////////////////////////////////////////////////////////////

#define LED1 PBout(0)
#define LED2 PBout(1)

void LED_Init(void);

#endif
