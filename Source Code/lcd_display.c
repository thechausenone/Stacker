#include "lcd_display.h"
//=================================================================
//=====================FUNCTIONS===================================
//=================================================================

//outputs a graphic to the LCD
//parameters: type is either B (block) or S (stack) and mode, which is used for the block, is either 1 (set) or 0 (clear)
void draw_to_LCD(char type, U32 mode){
  
  //draw the bitmap at the appropriate location
  if (type == 'B'){
    if (mode == 0){
      GLCD_Bitmap(block->x, block->y, block->length, block->height, (unsigned char*)(block->clear_block));
    }
    else if (mode == 1) {
      GLCD_Bitmap(block->x, block->y, block->length, block->height, (unsigned char*)(block->set_block));
    }
    else if (mode == 1){
      GLCD_Bitmap(block->x, block->y, block->length, block->height, (unsigned char*)(block->set_block));
    }
  }
  else if (type == 'S'){
      GLCD_Bitmap(stack->x, stack->y, stack->length, block->height, (unsigned char*)(block->set_block));
  }  
}

//outputs text to the LCD based on the end game condition achieved
void draw_text(U32 score, U32 mode) {
  
  //display main text
  if (mode == 0)
    GLCD_DisplayString(5, 3, 1, "GAME OVER!");
  else if (mode == 1)
    GLCD_DisplayString(5, 3, 1, "YOU WIN!");
  
  //display final score
  draw_stat(score, 2);
}

//updates the current stat being displayed on the LCD (score/speed)
void draw_stat(U32 stat, U32 mode) {
   unsigned char stat_out[64];//warning b/c not declared as "const char* stat_out"
  
  //depending on the mode, display different text to the screen
  if (mode == 0){
    sprintf(stat_out, "Score: %d ", stat);
    GLCD_DisplayString(0, 0, 1, stat_out);
  }
  else if (mode == 1){
    sprintf(stat_out, "Speed: %d      ", stat);
    GLCD_DisplayString(0, 0, 1, stat_out);
  }
  else if (mode == 2){
    sprintf(stat_out, "score: %d", stat);
    GLCD_DisplayString(6, 2, 1, stat_out);
  }
}



