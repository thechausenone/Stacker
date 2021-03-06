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
#include <stdbool.h>

//=================================================================
//=====================FUNCTION SIGNATURES=========================
//=================================================================
void create_block(U32 h, U32 l, U32 x, U32 y, U32 s);
void create_stack(U32 l, U32 x, U32 y);
void init_game(void);
void init_peripherals(void);
void spawn_block(void);
//S32 reverse_direction(S32 direction);

#endif

