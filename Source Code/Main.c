#include <stdint.h>
#include "led.h"
#include <stdbool.h>
#include <rtl.h>
#include "joystick.h"
#include "potentiometer.h"
#include <stdio.h>
#include "timer.h"
#include "glcd_scroll.h"
#include "glcd.h"
#include "Main.h"
#include "type.h"

//=================================================================
//=====================GLOBAL VARIABLES============================
//=================================================================
  t_block* block = (t_block*)0x10000000;
  t_stack* stack = (t_stack*)0x10002567;
  
  OS_MUT sem1;
  OS_TID tid_move_block;
  
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

//outputs a graphic to the LCD
//parameters: type is either B (block) or S (stack) and mode, which is used for the block, is either 1 (set) or 0 (clear)
void draw_to_LCD(char type, U32 mode){
   if (type == 'B'){
     if (mode == 0)
       GLCD_Bitmap(block->x, block->y, block->length, block->height, (unsigned char*)(block->clear_block));
     else if (mode == 1)
       GLCD_Bitmap(block->x, block->y, block->length, block->height, (unsigned char*)(block->set_block));
   }
   else if (type == 'S'){
       GLCD_Bitmap(stack->x, stack->y, stack->length, stack->height, (unsigned char*)(stack->top_row));
  }   
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
    create_block(8, 80, 3, 160, stack->height + 8);
    create_stack(8, 80, 80, 0);
    
    //draw initial graphics on the LCD
    draw_to_LCD('S', 1);
    draw_to_LCD('B', 1);
}

//=================================================================
//==========================TASKS==================================
//=================================================================
//task that moves the block back and forth on the LCD
__task void move_block(){
  
  bool direction;//going to the right is 1, left is 0
  S32 movement_value;
  direction = 1;
  movement_value = -1;

  while(1){
    //os_mut_wait(&sem1, 0xffff);
      //clears the previous instance of the block
      draw_to_LCD('B', 0);
       
      //check if the block's direction needs to be reversed
      //currently going to the right
      if (block->x <= 0 && direction == 1) {
        direction = 0;
        movement_value = 1;
      }
      
      //currently going to the left
      else if (block->x >= 240-block->length && direction == 0) {
        direction = 1;
        movement_value = -1;
      }

      //updates the position of the block
      block->x = block->x + movement_value;
      
      //draws the new instance of the block
      draw_to_LCD('B', 1);
      
      //wait for a bit until the block's can be updated again
      timer_delay(50/block->speed);
    //os_mut_release(blah);
  }
}

//task that is responsible for initializing all other tasks
__task void init_tasks() {
    //initialize muti
    os_mut_init (&sem1);
    
    //create the tasks
    tid_move_block = os_tsk_create(move_block, 1);
  
    //delete itself
    os_tsk_delete_self();
}

//=================================================================
//=======================MAIN FUNCTION=============================
//=================================================================
int main() {
    printf("Begin Game");
    init_game();
    os_sys_init(init_tasks);
    return 0;

  //add vertical scroll in pixels values being 8 
}


    
