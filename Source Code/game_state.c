#include "game_state.h"

//=================================================================
//=====================FUNCTIONS===================================
//=================================================================
//initializes the values for the block object
void create_block(U32 h, U32 l, U32 s, U32 x, U32 y){
  U32 i;
  block->height = h;
  block->length = l;
  block->speed = s;
  block->x = x;
  block->y = y;
  
  for (i = 0; i< l*h; i++) {
    block->clear_block[i] = B;
    block->set_block[i] = W;
  }
}

//initializes the values for the stack object
void create_stack(U32 h, U32 l, U32 x, U32 y){
  stack->height = h;
  stack->length = l;
  stack->x = x;
  stack->y = y;
}

//initializes all required peripherals and gameplay objects
void init_game() {
  
  //peripherals
  GLCD_Init();
  GLCD_Clear(B);
  timer_setup();
  LED_setup();
  pushbutton_setup();
  joystick_setup();
  potentiometer_setup(); 
  
  //gameplay objects
  create_block(8, 80, 1, 160, 8);
  create_stack(8, 80, 80, 0);
  
  //draw initial graphics on the LCD
  draw_to_LCD('S', 1, stack->height*stack->length);
  draw_to_LCD('B', 1, block->height*block->length);

  //draw the string to the LCD
  //string_to_LCD(3);
}
