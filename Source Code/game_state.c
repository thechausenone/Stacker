#include "game_state.h"

//=================================================================
//=====================FUNCTIONS===================================
//=================================================================
//initializes the values for the block object
void create_block(U32 h, U32 l, U32 x, U32 y, U32 s){
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
void create_stack(U32 l, U32 x, U32 y){
  stack->length = l;
  stack->x = x;
  stack->y = y;
}

void init_game() {
  //gameplay objects
  create_block(8, 80, 160, 304, 1);
  create_stack(80, 80, 312);
  
  //draw initial graphics on the LCD
  draw_to_LCD('S', 1);
  draw_to_LCD('B', 1);
}

//initializes all required peripherals and gameplay objects
void init_peripherals() {
  //peripherals
  GLCD_Init();
  GLCD_Clear(B);
  timer_setup();
  LED_setup();
  pushbutton_setup();
  joystick_setup();
  potentiometer_setup(); 
}

//spawns a block based on what its previous spawn location was
void spawn_block() {
  if (spawn_pos == 0) {
    block->x = 240 - block->length;
    spawn_pos = 1;
  }
  else if (spawn_pos == 1) {
    block->x = 240 - block->length;//should be 0  to alternate
    spawn_pos = 0;
  }
}

//reverse the direction of the block based on what its previous direction was
// S32 reverse_direction(S32 direction){
// 	
// 	//currently going to the right (==1)
// 	if (block->x <= 0 && direction == 0) 
// 		direction = 1;
// 	
// 	//currently going to the left (==0)
// 	else if (block->x >= 240-block->length && direction == 1)
// 		direction = 0;
// 	
// 	return direction;
// }



