#ifndef GAME_STATE_H__
#define GAME_STATE_H__
#include "type.h"
#include "stdint.h"
#include "game_objects.h"
#include "lcd_display.h"
#include "Main.h"
#include "joystick.h"
#include "potentiometer.h"
#include "led.h"
#include "timer.h"
#include "glcd_scroll.h"
#include "glcd.h"

//=================================================================
//=====================DEFINITIONS=================================
//=================================================================
#define W 0x001F
#define B 0x07E0

//=================================================================
//=====================FUNCTION SIGNATURES=========================
//=================================================================
void create_block(U32 h, U32 l, U32 s, U32 x, U32 y);
void create_stack(U32 h, U32 l, U32 x, U32 y);
void init_game(void);

#endif

