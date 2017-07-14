#include "lcd_display.h"

//=================================================================
//=====================FUNCTIONS===================================
//=================================================================

//outputs a graphic to the LCD
//parameters: type is either B (block) or S (stack) and mode, which is used for the block, is either 1 (set) or 0 (clear)
void draw_to_LCD(char type, U32 mode, U32 display_size){
  
  //draw the bitmap at the appropriate location
  if (type == 'B'){
    if (mode == 0){
      GLCD_Bitmap(block->x, block->y, block->length, block->height, (unsigned char*)(block->clear_block));
    }
    else if (mode == 1){
      GLCD_Bitmap(block->x, block->y, block->length, block->height, (unsigned char*)(block->set_block));
    }
  }
  else if (type == 'S'){
      GLCD_Bitmap(stack->x, stack->y, stack->length, stack->height, (unsigned char*)(block->set_block));
  }  
}

//updates the current stat display on the LCD
//parameters: display state #
void string_to_LCD(U32 display){
	GLCD_SetTextColor(B);
	GLCD_DisplayString(0, 300, 1, (unsigned char*)("Hello"));
}
