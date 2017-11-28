#ifndef LCD_DISPLAY_H__
#define LCD_DISPLAY_H__
#include "type.h"
#include "game_objects.h"
#include "glcd_scroll.h"
#include "glcd.h"
#include <stdio.h>

//=================================================================
//=====================FUNCTION SIGNATURES=========================
//=================================================================
void draw_to_LCD(char type, U32 mode);
void draw_text(U32 score, U32 mode);
void draw_stat(U32 stat, U32 mode);

#endif
