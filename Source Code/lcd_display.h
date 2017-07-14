#ifndef LCD_DISPLAY_H__
#define LCD_DISPLAY_H__
#include "type.h"
#include "game_objects.h"
#include "glcd_scroll.h"
#include "glcd.h"

//=================================================================
//=====================FUNCTION SIGNATURES=========================
//=================================================================
void draw_to_LCD(char type, U32 mode, U32 display_size);
void string_to_LCD(U32 display);

#endif
