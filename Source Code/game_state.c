#include "game_state.h"

//=================================================================
//=====================FUNCTIONS===================================
//=================================================================
//initializes the values for the block
void create_block(U32 h, U32 l, U32 s, U32 x, U32 y){
  int i;
    block->height = h;
    block->length = l;
    block->speed = s;
    
  for (i = 0; i< l*h; i++) {
    block->clear_block[i] = B;
    block->set_block[i] = W;
  }
  
    block->x = x;
    block->y = y;
}

//initializes the values for the stack
void create_stack(U32 h, U32 l, U32 x, U32 y){
    int i;
    stack->height = h;
    stack->length = l;
  
    for (i = 0; i < 8*80; i++)
        stack->top_row[i] = W;
        
    stack->x = x;
    stack->y = y;
    
    return;
}

//initializes all required peripherals and other stuff
void init_game() {
    //other
    //SystemInit();    
 
    //peripherals
    GLCD_Init();
    GLCD_Clear(B);
    timer_setup();
    LED_setup();
    pushbutton_setup();
    joystick_setup(); //left: 8, up: 16, right: 32, down: 64, select: 1, default: 0
    potentiometer_setup(); // has to be in a task
    //gameplay objects
    create_block(8, 80, 3, 160, stack->height);
    create_stack(8, 80, 80, 0);
    
    //draw initial graphics on the LCD
    draw_to_LCD('S', 1);
    draw_to_LCD('B', 1);
}
