#ifndef MAIN_H_
#define MAIN_H_
#include "type.h"
#include <math.h>
#include "lcd_display.h"
#include "stat_display.h"
#include "game_state.h"
#include "game_objects.h"
#include "potentiometer.h"
#include "joystick.h"
#include <stdint.h>
#include <stdbool.h>
#include <rtl.h>
#include <stdio.h>
#include <lpc17xx.h>


//=================================================================
//=====================DEFINITIONS=================================
//=================================================================
#define W 0x001F
#define B 0x07E0

//=================================================================
//=====================FUNCTION SIGNATURES=========================
//=================================================================
void EINT3_IRQHandler(void);

#endif
